#include "Logger.h"
#include <QCoreApplication>
#include <QDir>
#include <QSysInfo>
#include <windows.h>

Logger* Logger::s_instance = nullptr;

Logger::Logger(QObject *parent)
    : QObject(parent)
{
    s_instance = this;
}

Logger::~Logger()
{
    if (m_logFile.isOpen()) {
        m_stream.flush();
        m_logFile.close();
    }
}

Logger& Logger::instance()
{
    if (!s_instance) {
        s_instance = new Logger(qApp);
    }
    return *s_instance;
}

void Logger::setLogFile(const QString &filename)
{
    QMutexLocker locker(&m_mutex);
    m_filename = filename;
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
    m_logFile.setFileName(filename);
    rotateLogIfNeeded();
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        m_stream.setDevice(&m_logFile);
    }
}

void Logger::rotateLogIfNeeded()
{
    if (m_logFile.exists() && m_logFile.size() > 5 * 1024 * 1024) {
        QString oldName = m_filename + ".old";
        if (QFile::exists(oldName))
            QFile::remove(oldName);
        QFile::rename(m_filename, oldName);
    }
}

QString Logger::levelToString(LogLevel level)
{
    switch (level) {
    case LOG_INFO:    return "INFO";
    case LOG_WARNING: return "WARNING";
    case LOG_ERROR:   return "ERROR";
    case LOG_DEBUG:   return "DEBUG";
    default:          return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const QString &message)
{
    QMutexLocker locker(&m_mutex);
    if (!m_stream.device()) return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    QString line = QString("[%1] [%2] %3\n").arg(timestamp, levelToString(level), message);
    m_stream << line;
    m_stream.flush();
}

void Logger::installMessageHandler()
{
    qInstallMessageHandler(qtMessageHandler);
}

void Logger::qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    LogLevel level;
    switch (type) {
    case QtDebugMsg:    level = LOG_DEBUG; break;
    case QtInfoMsg:     level = LOG_INFO; break;
    case QtWarningMsg:  level = LOG_WARNING; break;
    case QtCriticalMsg:
    case QtFatalMsg:    level = LOG_ERROR; break;
    default:            level = LOG_INFO; break;
    }
    if (s_instance) {
        s_instance->log(level, msg);
    }
}