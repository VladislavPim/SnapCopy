#pragma once

#include <QSystemTrayIcon>
#include <QMenu>

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT

public:
    explicit TrayIcon(QObject *parent = nullptr);

private slots:
    void onShowHistory();
    void onQuit();

private:
    QMenu *m_menu;
};