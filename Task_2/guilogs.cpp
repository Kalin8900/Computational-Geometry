#include "guilogs.h"
#include <QDebug>

LogGUI::LogGUI(QTextEdit *loggingPlace)
    : logPlace_(loggingPlace)
{

}

QTextEdit *LogGUI::logPlace() const
{
    return logPlace_;
}

void LogGUI::clearLog()
{
    logPlace_->clear();
}

void operator<<(LogGUI *l, const QString &msg)
{
    l->logPlace()->append(msg + "\n");
}

