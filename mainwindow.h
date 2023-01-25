#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"mythread.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadConboxID();

public slots:
    void JsonProcess(QString);

private slots:

    void on_actionChoose_Window_triggered();

    void on_pushButton_Play_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void threadStopAll();


    void on_actionConfig_triggered();

    void on_actionTest_Thresh_triggered();

    void ScreenWidowLoad(QString name);

    void on_pushButton_checkpowermobi_clicked();


    void on_pushButton_refes_clicked();

private:
    Ui::MainWindow *ui;
    MyThread *thread0;
    MyThread *thread1;
    MyThread *thread2;
    MyThread *thread3;
};
#endif // MAINWINDOW_H
