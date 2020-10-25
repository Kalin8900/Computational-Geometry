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

    void on_countBtn_pressed();

    void on_countBtn_released();

    void on_saveDatatoFileBtn_clicked();

    void on_readBtn_clicked();

    void on_logClearBtn_clicked();

    void on_saveLogBtn_clicked();

    void on_changeABColorBtn_clicked();

    void on_changeCDColorBtn_clicked();

    void on_ABWidthInput_textChanged();

    void on_CDWidthInput_textChanged();

    void on_ABComboBox_currentIndexChanged(int index);

    void on_CDComboBox_currentIndexChanged(int index);

    void on_updateBtn_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_CloseBtn_clicked();

private:
    enum POINT{APOINT, BPOINT, CPOINT, DPOINT};

    using inputsVector = QVector<QPair<QPair<QTextEdit *, QTextEdit *>, POINT>>;
    Ui::MainWindow *ui;
    inputsVector pointInputs;
    std::array<bool, 4> validPoints;

    const static QVector<Qt::PenStyle> penStyles;

    bool readyToCount;
    bool arePointsVisible;
    LogGUI *log;
    Canvas *canvas_;

    void drawLines();
    void drawPoints();
    void draw();
    void calculatePointP();
    void savePointToCanvas(QTextEdit *xInput, QTextEdit *yInput, POINT);
    void loadPointsToGUI();
    void fillMenu(QComboBox *cb);
    void getValuesFromGUI();
};
#endif // MAINWINDOW_H
