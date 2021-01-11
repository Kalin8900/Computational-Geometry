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
      arePointsVisible(true),
      convexVisible(false)
{
    ui->setupUi(this);
    canvas_ = new Canvas(ui->canvas, this);
    log = new LogGUI(ui->logs);

    ui->ABWidthInput->setText(QString::number(canvas_->getDefaultPen().width()));

    fillMenu(ui->ABComboBox);

    log->clearLog();
    canvas_->clear();

    setWindowTitle("Computional Geometry task 4");
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

void MainWindow::draw()
{
    if(readyToCount)
    {
        canvas_->clear();
        drawPoints();
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

void MainWindow::fillMenu(QComboBox *cb)
{
    cb->addItem("Solid");
    cb->addItem("Dot");
    cb->addItem("Dash");
    cb->addItem("Dash Dot");
    cb->addItem("Dash Dot Dot");
    cb->setCurrentIndex(0);
}

//Signals

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
                                                    QDir::currentPath(), tr("Txt Files (*.txt)"));

    if(!fileName.isEmpty())
    {
        FileLoader::readPointsFromFile(canvas_, fileName);

        log << QString("Points readed from file " + fileName) + getCurDateTime();

        if(arePointsVisible)
            drawPoints();
        update();
        readyToCount = true;

        log << QString("Points loaded to GUI ") + getCurDateTime();
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
    canvas_->getDefaultPen().setColor(pickedColor.rgba());

    if(convexVisible)
        canvas_->drawConvexHull();
}

void MainWindow::on_ABWidthInput_textChanged()
{
    if(ui->ABWidthInput->toPlainText().toInt() == 0)
        return;

    canvas_->getDefaultPen().setWidth(ui->ABWidthInput->toPlainText().toInt());

    log << QString("Width of the AB line has changed to ") + QString::number(canvas_->getDefaultPen().width());

    canvas_->clear();

    if(arePointsVisible)
        drawPoints();
    if(convexVisible)
        canvas_->drawConvexHull();
}

void MainWindow::on_ABComboBox_currentIndexChanged(int index)
{
    canvas_->getDefaultPen().setStyle(penStyles.at(index));

    log << QString("Changed line style ") + getCurDateTime();

    canvas_->clear();

    if(arePointsVisible)
        drawPoints();
    if(convexVisible)
        canvas_->drawConvexHull();
}

void MainWindow::on_updateBtn_clicked()
{
    arePointsVisible = true;
    ui->checkBox->setCheckState(Qt::CheckState::Checked);
    canvas_->clear();
    canvas_->drawConvexHull();
    drawPoints();

    log << QString("Refreshed view ") + getCurDateTime();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 > 0)
    {
        if(readyToCount)
            drawPoints();
        log << QString("Points set to visible") + getCurDateTime();
        arePointsVisible = true;
    }
    else
    {
        canvas_->removePoints();
        arePointsVisible = false;
        if(convexVisible)
            canvas_->drawConvexHull();
        log << QString("Points set to invisible") + getCurDateTime();
    }
}

void MainWindow::on_CloseBtn_clicked()
{
    close();
}

void MainWindow::on_drawBtn_clicked()
{
    if(readyToCount)
    {
        canvas_->countConvexHull();
        canvas_->drawConvexHull();
        convexVisible = true;
    }
}
