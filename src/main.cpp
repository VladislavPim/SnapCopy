#include <QApplication>
#include <QSysInfo>
#include <windows.h>
#include "SnapBar.h"
#include "AppState.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("SnapCopy");
    app.setOrganizationName("DXStreem");

    // Настраиваем логгер
    QString logPath = QCoreApplication::applicationDirPath() + "/debug.log";
    Logger::instance().setLogFile(logPath);
    Logger::instance().installMessageHandler();

    // Системная информация
    Logger::instance().log(LOG_INFO, "=== SnapCopy Started ===");
    Logger::instance().log(LOG_INFO, "OS: " + QSysInfo::prettyProductName());
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    Logger::instance().log(LOG_INFO, QString("Total RAM: %1 MB").arg(memStatus.ullTotalPhys / (1024*1024)));
    Logger::instance().log(LOG_INFO, "Executable path: " + QCoreApplication::applicationFilePath());

    // Проверка на одиночный экземпляр
    SnapBar bar;
    AppState appState(&bar);
    if (!appState.tryLock()) {
        Logger::instance().log(LOG_WARNING, "Another instance already running, exiting.");
        return 0;
    }

    bar.show();

    int result = app.exec();

    Logger::instance().log(LOG_INFO, "=== SnapCopy Exited ===");
    return result;
}