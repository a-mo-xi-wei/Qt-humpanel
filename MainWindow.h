#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "roundprogressbar.h"
#include "framelesscore/framelessmainwindow.h"
#include "qcustomplot.h"

#include <QFont>
#include <QtCharts>


QT_BEGIN_NAMESPACE
namespace Ui {
 class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public FramelessMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void titleDblClick();
    void windowStateChange(bool max);

    void on_btnMenu_Min_clicked();

    void on_btnMenu_Max_clicked();

    void on_btnMenu_Close_clicked();

private:
    void initMainWindowUI();
    void roundPeogreInit();
    void pieInit();
    void lineInit();

private slots:
    void drawLine();

    void on_btn_line_clicked();

    void on_btn__clicked();

private:
    Ui::MainWindow *ui;
    QFont iconfont;
    RoundProgressBar* rpb[6];
    QVector<QCPTextElement *> titel;
    QVector<QCustomPlot*> lineSty;
    double x_time;
};
#endif // MAINWINDOW_H
