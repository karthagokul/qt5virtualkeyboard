#include "mockupinputcontext.h"
#include <QDebug>
#include <QEvent>
#include <QQuickView>
#include <QGuiApplication>
#include "mockupkeyeventdispatcher.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QScreen>

class MockupInputContextPrivate
{
public:
    MockupInputContextPrivate();
    ~MockupInputContextPrivate();
    QQuickView *view;
    MockupKeyEventDispatcher keyEventDispatcher;
};

MockupInputContextPrivate::MockupInputContextPrivate(): view(0)
{

}

MockupInputContextPrivate::~MockupInputContextPrivate()
{
    if (view) {
        delete view;
    }
}

MockupInputContext::MockupInputContext() :
    QPlatformInputContext(), d(new MockupInputContextPrivate)
{

}

MockupInputContext::~MockupInputContext()
{

}

bool MockupInputContext::isValid() const
{
    return true;
}

QRectF MockupInputContext::keyboardRect() const
{
    return (d->view)? d->view->geometry(): QRectF();
}

void MockupInputContext::showInputPanel()
{
    if (!d->view) {        
        d->view = new QQuickView();
        d->view->engine()->rootContext()->setContextProperty("keyEventDispatcher",&d->keyEventDispatcher);
        d->view->setSource(QUrl("qrc:/InputPanel.qml"));

        d->view->setResizeMode(QQuickView::SizeRootObjectToView);
        d->view->setClearBeforeRendering(true);

        d->view->setColor(Qt::gray);
        //QSurfaceFormat f(d->view->format());
        //f.setAlphaBufferSize(8);
        //d->view->setFormat(f);

        d->view->setFlags(d->view->flags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint	| Qt::WindowDoesNotAcceptFocus);
    }

    int windowWidth = qApp->focusWindow()->width();
    int windowHeight = qApp->focusWindow()->height();

    // --- this is no longer working correctly with the app having QWidget as the root object ---
    //d->view->setPosition(qApp->focusWindow()->x(), (qApp->focusWindow()->y() + 2.0/3.0 * windowHeight));

    // --- this works provided that the app's RootWidget is centered and frameless (i.e. without title bar) ---
    QSize screenSize = QGuiApplication::primaryScreen()->size();
    int x = (screenSize.width() - windowWidth) / 2;
    int y = (screenSize.height() - windowHeight) / 2;
    d->view->setPosition(x, (y + 2.0/3.0 * windowHeight));

    d->view->setWidth(windowWidth);
    d->view->setHeight(windowHeight/3);

    d->view->show();

    QPlatformInputContext::showInputPanel();
}

void MockupInputContext::hideInputPanel()
{
    if (d->view) {
        d->view->hide();
    }
    QPlatformInputContext::hideInputPanel();
}

bool MockupInputContext::isInputPanelVisible() const
{
    return (d->view)? d->view->isVisible():false;
}


void MockupInputContext::setFocusObject(QObject *object)
{
    d->keyEventDispatcher.setFocusItem(object);
}
