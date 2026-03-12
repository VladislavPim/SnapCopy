#include "AppState.h"
#include "SnapBar.h"
#include "Logger.h"

AppState::AppState(SnapBar *bar, QObject *parent)
    : QObject(parent)
    , m_snapBar(bar)
    , m_sharedMemory("SnapCopy_SingleInstance_Key")
{
}

AppState::~AppState()
{
}

bool AppState::tryLock()
{
    if (m_sharedMemory.attach()) {
        // уже есть другой экземпляр
        return false;
    }
    if (!m_sharedMemory.create(1)) {
        // не удалось создать (возможно, уже есть)
        return false;
    }
    return true;
}