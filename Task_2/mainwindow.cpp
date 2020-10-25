#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "fileloader.h"

#include <QMessageBox>
#include <QDebug>

const QVector<Qt::PenStyle> MainWindow::penStyles = {Qt::PenStyle::SolidLine,
                                                     Qt::PenStyle::DotLine,
                                                    Qt::PenStyle::DashLine,
                                                    Qt::PenStyle::DashDotLine,
                                                    Qt::PenStyle::DashDotDotLine
                                                    };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      readyToCount(true), //to init everything
      arePointsVisible(true)
{
    ui->setupUi(this);
    canvas_ = new Canvas(ui->canvas, this);
    log = new LogGUI(ui->logs);

    ui->ABWidthInput->setText(QString::number(canvas_->getPenAB().width()));
    ui->CDWidthInput->setText(QString::number(canvas_->getPenCD().width()));


    fillMenu(ui->ABComboBox);
    fillMenu(ui->CDComboBox);

    pointInputs.append(qMakePair((qMakePair(ui->XaInput, ui->YaInput)), MainWindow::APOINT));
    pointInputs.append(qMakePair((qMakePair(ui->XbInput, ui->YbInput)), MainWindow::BPOINT));
    pointInputs.append(qMakePair((qMakePair(ui->XcInput, ui->YcInput)), MainWindow::CPOINT));
    pointInputs.append(qMakePair((qMakePair(ui->XdInput, ui->YdInput)), MainWindow::DPOINT));

    log->clearLog();
    canvas_->clear();

    setWindowTitle("Computional Geometry task 2");
    setWindowIcon(QIcon("./icon.png"));
    readyToCount = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTextEdit *MainWindow::getLoggingPlace()
{
    return ui->logs;
}


void MainWindow::savePointToCanvas(QTextEdit *xInput, QTextEdit *yInput, POINT type)
{
    auto xVal = xInput->toPlainText();
    auto yVal = yInput->toPlainText();
    bool tempX, tempY;

    switch (type) {
    case MainWindow::APOINT:
        canvas_->setPointA({{xVal.toFloat(&tempX), yVal.toFloat(&tempY)}, "A"});
        (tempX && tempY) ? validPoints.at(0) = 1 : validPoints.at(0) = 0;
        break;
    case MainWindow::BPOINT:
        canvas_->setPointB({{xVal.toFloat(&tempX), yVal.toFloat(&tempY)}, "B"});
        (tempX && tempY) ? validPoints.at(1) = 1 : validPoints.at(1) = 0;
        break;
    case MainWindow::CPOINT:
        canvas_->setPointC({{xVal.toFloat(&tempX), yVal.toFloat(&tempY)}, "C"});
        (tempX && tempY) ? validPoints.at(2) = 1 : validPoints.at(2) = 0;
        break;
    case MainWindow::DPOINT:
        canvas_->setPointD({{xVal.toFloat(&tempX), yVal.toFloat(&tempY)}, "D"});
        (tempX && tempY) ? validPoints.at(3) = 1 : validPoints.at(3) = 0;
        break;
    }
}

void MainWindow::getValuesFromGUI()
{
    for(const auto &point : pointInputs)
        savePointToCanvas(point.first.first, point.first.second, point.second);

    bool temp(true);
    for(const auto b : validPoints)
        temp &= b;
    readyToCount = temp;
    qInfo() << readyToCount;
}



//Utilities

QString getCurDateTime()
{
    return QDateTime::currentDateTime().toString(" hh:mm:ss dd-MM-yyyy");
}

void MainWindow::drawPoints()
{
    for(auto point : canvas_->getPointsVector())
        canvas_->drawPoint(*point);
}

void MainWindow::drawLines()
{
    for(int i = 0, j = 1; j < canvas_->getPointsVector().size(); i += 2, j += 2)
    {
        if(i == 0)
        {
            canvas_->drawLine(canvas_->getPointsVector().at(i)->pos(),
                              canvas_->getPointsVector().at(j)->pos(), Canvas::Line::AB);
        }
        else if(i == 2)
        {
            canvas_->drawLine(canvas_->getPointsVector().at(i)->pos(),
                              canvas_->getPointsVector().at(j)->pos(), Canvas::Line::CD);
        }
    }

    log << QString("Drawn lines ") + getCurDateTime();
}

void MainWindow::draw()
{
    if(readyToCount)
    {
        canvas_->clear();
        drawPoints();
        drawLines();
    }
    else
    {
        QMessageBox msg;
        msg.critical(this, "Error", "Cannot draw because points are not ready");
        msg.setFixedSize(500, 200);

        log << QString("Could not draw becayse points were not ready ")
               + getCurDateTime();
    }
}

//Methods

void MainWindow::loadPointsToGUI()
{
    for(int i = 0; i < pointInputs.size(); ++i)
    {
        pointInputs.at(i).first.first->setText(QString::number(canvas_->getPointsVector().at(i)->pos().x()));
        pointInputs.at(i).first.second->setText(QString::number(canvas_->getPointsVector().at(i)->pos().y()));
    }
}

void MainWindow::fillMenu(QComboBox *cb)
{
    cb->addItem("Solid");
    cb->addItem("Dot");
    cb->addItem("Dash");
    cb->addItem("Dash Dot");
    cb->addItem("Dash Dot Dot");
    cb->setCurrentIndex(0);
}

void MainWindow::calculatePointP()
{
    QVector<QPair<float, float>> differences; //delXAB delYAB delXCD delYCD

    for(int i = 0, j = 1; j < canvas_->getPointsVector().size(); i += 2, j += 2)
    {
        float xDif = canvas_->getPointsVector().at(j)->pos().x()
                - canvas_->getPointsVector().at(i)->pos().x();

        float yDif = canvas_->getPointsVector().at(j)->pos().y()
                - canvas_->getPointsVector().at(i)->pos().y();

        differences.append(qMakePair(xDif, yDif));
    }

    float denominator = differences.at(0).first * differences.at(1).second
            - differences.at(0).second * differences.at(1).first;

    QPair<float, float> acDif = qMakePair(
                canvas_->getPointsVector().at(2)->pos().x()
                - canvas_->getPointsVector().at(0)->pos().x(),
                canvas_->getPointsVector().at(2)->pos().y()
                - canvas_->getPointsVector().at(0)->pos().y()
                );

    float paramOne = acDif.first * differences.at(1).second
            - acDif.second * differences.at(1).first;

    paramOne /= denominator;

    float paramTwo = acDif.first * differences.at(0).second
            - acDif.second * differences.at(0).first;

    paramTwo /= denominator;

    float Xp = canvas_->getPointsVector().at(0)->pos().x()
            + paramOne * differences.at(0).first;

    float Yp = canvas_->getPointsVector().at(0)->pos().y()
            + paramOne * differences.at(0).second;


    ui->XpInput->setText(QString::number(Xp));
    ui->YpInput->setText(QString::number(Yp));
    canvas_->setPointP({{Xp, Yp}, "P"});
}

//Signals

void MainWindow::on_countBtn_pressed()
{
    getValuesFromGUI();

    if(readyToCount)
    {
        calculatePointP();

        draw();

        log << QString("Calculeted intersection point P x: ") + QString::number(canvas_->getPointP().pos().x()) +
               QString(" y: ") + QString::number(canvas_->getPointP().pos().y())
               + getCurDateTime();
    }
    else
    {
        canvas_->clear();

        QMessageBox msg;
        msg.critical(this, "Invalid input", "Given values cannot be converted to floats");
        msg.setFixedSize(500, 200);

        log << QString("Could not perform counting because of the wrong input ")
               + getCurDateTime();
    }
}

void MainWindow::on_countBtn_released()
{
    canvas_->center();
}

void MainWindow::on_saveDatatoFileBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Provide filename to save the data"),
                                                QDir::currentPath(), tr("Json Files (*.json)"));

    FileLoader::writeToFile(canvas_, path);

    log << QString("Points saved to file ") + path + getCurDateTime();
}

void MainWindow::on_readBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Read data from json file"),
                                                    QDir::currentPath(), tr("Json Files (*.json)"));

    if(!fileName.isEmpty())
    {
        FileLoader::readFromFile(canvas_, fileName);

        log << QString("Points readed from file " + fileName) + getCurDateTime();

        loadPointsToGUI();

        log << QString("Points loaded to GUI") + getCurDateTime();
    }
}

void MainWindow::on_logClearBtn_clicked()
{
    log->clearLog();
}

void MainWindow::on_saveLogBtn_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Provide filename to save the data"),
                                                QDir::currentPath(), tr("Text Files (*.txt)"));

    if(!path.isEmpty())
    {
        FileLoader::saveLogsToFile(log->logPlace(), path);

        log << QString("Logs saved to file ") + path + getCurDateTime();
    }

    log << QString("Could not save the logs to file ") + path;

}

void MainWindow::on_changeABColorBtn_clicked()
{
    auto pickedColor = QColorDialog::getColor(Qt::white, this, "Pick AB line color");

    QVariant var(pickedColor);
    log << QString("Color of the AB line changed to ") + var.toString() + getCurDateTime();
    canvas_->getPenAB().setBrush(QBrush(pickedColor));

    if(readyToCount)
        drawLines();
}



void MainWindow::on_changeCDColorBtn_clicked()
{
    auto pickedColor = QColorDialog::getColor(Qt::white, this, "Pick AB line color");

    QVariant var(pickedColor);
    log << QString("Color of the CD line has changed to ") + var.toString() + getCurDateTime();
    canvas_->getPenCD().setBrush(QBrush(pickedColor));

    if(readyToCount)
        drawLines();
}



void MainWindow::on_ABWidthInput_textChanged()
{
    canvas_->getPenAB().setWidth(ui->ABWidthInput->toPlainText().toInt());

    log << QString("Width of the AB line has changed to ") + QString::number(canvas_->getPenAB().width());
    if(readyToCount)
        drawLines();
}

void MainWindow::on_CDWidthInput_textChanged()
{
    canvas_->getPenCD().setWidth(ui->CDWidthInput->toPlainText().toInt());

    log << QString("Width of the CD line has changed to ") + QString::number(canvas_->getPenCD().width());

    if(readyToCount)
        drawLines();
}

void MainWindow::on_ABComboBox_currentIndexChanged(int index)
{
    canvas_->getPenAB().setStyle(penStyles.at(index));


    if(arePointsVisible)
        draw();
    else
    {
        canvas_->clear();
        if(readyToCount)
            drawLines();
    }
}

void MainWindow::on_CDComboBox_currentIndexChanged(int index)
{
    canvas_->getPenCD().setStyle(penStyles.at(index));


    if(arePointsVisible)
        draw();
    else
    {
        canvas_->clear();
        if(readyToCount)
            drawLines();
    }
}

void MainWindow::on_updateBtn_clicked()
{
    draw();
    arePointsVisible = true;
    ui->checkBox->setCheckState(Qt::CheckState::Checked);
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 > 0)
    {
        if(readyToCount)
        {
            drawPoints();
            drawLines();
        }
        arePointsVisible = true;
    }
    else
    {
        canvas_->removePoints();
        arePointsVisible = false;
    }
}

void MainWindow::on_CloseBtn_clicked()
{
    close();
}
