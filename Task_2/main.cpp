#include "mainwindow.h"
#include "logger.h"
#include <QApplication>

/****
 * TODO list:
 * Scaling generated image,
 * Some functions or methods so that the code is less duplicated
 * Streamline GUIlogs, shorten messages, add where needed
 * Make a check whether the file was selected when reading and writing and do something if not
 ****/

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.show();


    return a.exec();
}
