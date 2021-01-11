#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QScopedPointer>
#include <QFileDialog>
#include <QColorDialog>
#include <QComboBox>
#include <array>
#include "canvas.h"
#include "guilogs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTextEdit * getLoggingPlace();

private slots:

    void on_saveDatatoFileBtn_clicked();

    void on_readBtn_clicked();

    void on_logClearBtn_clicked();

    void on_saveLogBtn_clicked();

    void on_changeABColorBtn_clicked();

    void on_ABWidthInput_textChanged();

    void on_ABComboBox_currentIndexChanged(int index);

    void on_updateBtn_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_CloseBtn_clicked();

    void on_drawBtn_clicked();

private:
    Ui::MainWindow *ui;


    const static QVector<Qt::PenStyle> penStyles;

    bool readyToCount;
    bool arePointsVisible;
    bool convexVisible;
    LogGUI *log;
    Canvas *canvas_;

    void drawPoints();
    void draw();
    void loadPointsToGUI();
    void fillMenu(QComboBox *cb);
};
#endif // MAINWINDOW_H
