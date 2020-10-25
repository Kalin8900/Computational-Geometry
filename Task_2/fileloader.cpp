#include "fileloader.h"

void FileLoader::writeToFile(Canvas *canvas, const QString &path)
{
    QVariantMap map;
    QVariantMap points;

    for(const auto &point : canvas->getPointsVector())
    {
        QVariantMap cords;
        cords.insert("X", point->pos().x());
        cords.insert("Y", point->pos().y());
        points.insert(point->name(), cords);
    }

    map["Points"] = QVariant(points);

    QJsonDocument document = QJsonDocument::fromVariant(map);

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Could not write to file!";
        qCritical() << file.errorString();
        return;
    }

    file.write(document.toJson());
    file.close();
}

void FileLoader::readFromFile(Canvas *canvas, const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical() << "Could not read file!";
        qCritical() << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(data);

    QVariantMap points = qvariant_cast<QVariantMap>(document["Points"]);
    QVector<QPointF> cordinates;
    foreach(QString key, points.keys())
    {
        QVariantMap cords = qvariant_cast<QVariantMap>(points[key]);
        int cnt = 0;
        foreach(QString key, cords.keys())
        {
            float x, y;
            if(key == "X")
                x = cords[key].toFloat();
            else if(key == "Y")
                y = cords[key].toFloat();
            ++cnt;
            if(cnt == 2)
                cordinates.append({x, y});
        }
    }

    if(cordinates.size() != canvas->getPointsVector().size())
        throw std::runtime_error("Points were not loaded correctly");

    for(int i = 0; i < cordinates.size(); ++i)
        canvas->getPointsVector().at(i)->setPos(cordinates.at(i));
}

void FileLoader::saveLogsToFile(QTextEdit *logs, const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        qCritical() << "Could not read file!";
        qCritical() << file.errorString();
        return;
    }

    QTextStream s(&file);

    s << logs->toPlainText();

    file.close();
}
