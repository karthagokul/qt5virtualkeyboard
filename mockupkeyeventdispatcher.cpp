#include "mockupkeyeventdispatcher.h"
#include <QInputMethodEvent>
#include <QCoreApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <QKeyEvent>

MockupKeyEventDispatcher::MockupKeyEventDispatcher(QObject *parent) :
    QObject(parent),m_focusItem(0)
{

}

void MockupKeyEventDispatcher::setFocusItem(QObject *focusItem)
{
    m_focusItem = focusItem;
}

void MockupKeyEventDispatcher::sendKeyToFocusItem(const QString &keyText)
{
    if (!m_focusItem) {
        return;
    }

    // special handling for Backspace (send a QKeyEvent to the receiving text element)
    if (keyText == QString("\x7F")) {
        QKeyEvent event(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QQuickItem *receiver = qobject_cast<QQuickItem *>(m_focusItem);
        receiver->window()->sendEvent(receiver, &event);

    // send the typed character to the text field using QInputMethodEvent::setCommitString()
    } else {
        QInputMethodEvent ev;
        ev.setCommitString(keyText);
        QCoreApplication::sendEvent(m_focusItem,&ev);
    }
}
