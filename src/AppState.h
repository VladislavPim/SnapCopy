#pragma once

#include <QObject>
#include <QSharedMemory>

class SnapBar;

class AppState : public QObject
{
    Q_OBJECT
public:
    explicit AppState(SnapBar *bar, QObject *parent = nullptr);
    ~AppState();

    bool tryLock();  // true если успешно (первый экземпляр)

private:
    SnapBar *m_snapBar;
    QSharedMemory m_sharedMemory;
};