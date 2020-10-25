#ifndef GUILOGS_H
#define GUILOGS_H

#include <QTextEdit>

class LogGUI
{
public:
    LogGUI(QTextEdit *loggingPlace);


    QTextEdit *logPlace() const;

    void clearLog();
private:
    QTextEdit *logPlace_;
};

void operator<<(LogGUI *l, const QString &msg);

#endif // GUILOGS_H
