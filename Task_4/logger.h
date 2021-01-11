#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QTextStream>

class Logger : public QObject
{
    Q_OBJECT
public:


    static void attach();
    static void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    static void turnLoggingOff() { logging = false; };
    static void turnLoggingOn() { logging = true; };
    static bool toggleLogging();

    static QString getLogFile();
    static void setLogFile(const QString &value);

    static bool isLogging();

    Logger(const Logger &other) = delete;
    Logger operator=(const Logger &other) = delete;
private:
    explicit Logger(QObject *parent = nullptr);
    static Logger &getLogger();

    static QString logFile;
    static bool logging;

};

#endif // LOGGER_H
