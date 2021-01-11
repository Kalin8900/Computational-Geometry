#ifndef FILELOADER_H
#define FILELOADER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QVariant>
#include <QVariantMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTextStream>
#include <QTextEdit>
#include <QMap>
#include "canvas.h"

class FileLoader
{
public:
    static void writeToFile(Canvas *canvas, const QString &path);

    static void readFromFile(Canvas *canvas, const QString &path);

    static void saveLogsToFile(QTextEdit *logs, const QString &path);

    static void readPointsFromFile(Canvas *canvas, const QString &path);

    FileLoader(const FileLoader &other) = delete;
    FileLoader operator=(const FileLoader &other) = delete;

private:
    FileLoader() = delete;

};

#endif // FILELOADER_H
