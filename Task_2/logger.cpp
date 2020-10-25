#include <stdexcept>
#include "logger.h"

QString Logger::logFile(QDir::currentPath() +
                        QDir::separator() + "log.txt");
bool Logger::logging(false);
const static QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(nullptr);


Logger::Logger(QObject *parent) : QObject(parent)
{

}

Logger &Logger::getLogger()
{
    static Logger log;
    return log;
}

bool Logger::isLogging()
{
    return logging;
}

QString Logger::getLogFile()
{
    return logFile;
}

void Logger::setLogFile(const QString &value)
{
    logFile = value;
}

void Logger::attach()
{
    Logger::logging = true;
    qInstallMessageHandler(Logger::handler);
}

void Logger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(Logger::logging)
    {
        QString toPrint;
        switch (type) {
        case QtInfoMsg:
            toPrint = QString("Info: %1 in %2").arg(msg).arg(context.file);
            break;
        case QtDebugMsg:
            toPrint = QString("Debug: %1 in %2").arg(msg).arg(context.file);
            break;
        case QtWarningMsg:
            toPrint = QString("Warning: %1 in %2").arg(msg).arg(context.file);
            break;
        case QtCriticalMsg:
            toPrint = QString("Critical: %1 in %2").arg(msg).arg(context.file);
            break;
        case QtFatalMsg:
            toPrint = QString("Fatal: %1 in %2").arg(msg).arg(context.file);
            break;
        }

        QFile outFile(Logger::logFile);
        if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream stream(&outFile);
            stream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << " - " << toPrint <<
                      " line: " << context.line << "\n";
            stream.flush();
            outFile.close();
        } else
            throw std::runtime_error("Couldn't open log file " + Logger::logFile.toStdString());


    }

    (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
}

bool Logger::toggleLogging()
{
    Logger::logging = !Logger::logging;

    return Logger::logging;
}
