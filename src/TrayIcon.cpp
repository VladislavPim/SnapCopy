#include "TrayIcon.h"
#include <QApplication>

TrayIcon::TrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/resources/icon.png")); // TODO: добавить иконку позже
    setToolTip("SnapCopy");

    m_menu = new QMenu();
    m_menu->addAction("Показать историю", this, &TrayIcon::onShowHistory);
    m_menu->addSeparator();
    m_menu->addAction("Выход", this, &TrayIcon::onQuit);

    setContextMenu(m_menu);
    show();
}

void TrayIcon::onShowHistory()
{
    // TODO: показать окно истории
}

void TrayIcon::onQuit()
{
    QApplication::quit();
}