#pragma once

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QDebug>

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_DEBUG
};

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& instance();
    void log(LogLevel level, const QString &message);
    void setLogFile(const QString &filename);
    void installMessageHandler();

private:
    explicit Logger(QObject *parent = nullptr);
    ~Logger();
    void rotateLogIfNeeded();
    QString levelToString(LogLevel level);
    static void qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    QFile m_logFile;
    QTextStream m_stream;
    QMutex m_mutex;
    QString m_filename;
    static Logger *s_instance;
};