#ifndef PROCESSWINDOWS_H
#define PROCESSWINDOWS_H

#include <QDialog>

namespace Ui {
class ProcessWindows;
}

class ProcessWindows : public QDialog
{
    Q_OBJECT

public:
    explicit ProcessWindows(QWidget *parent = nullptr);
    ~ProcessWindows();

    QString MD5(QString str);
    QRect getXY_WH(QString nameProcess);
    QPixmap ScreenShoot(QString nameProcess);
    QStringList ListNameWindow;
    QStringList ListNoWindow;

private slots:

    void on_pushButton_testprocess_clicked();

    void on_pushButton_SaveNameWindow_clicked();

    void loadfile();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_Clear_clicked();

    void on_pushButton_ScanMobi_clicked();

private:
    Ui::ProcessWindows *ui;
    QString filename_window;
    int row;

};

#endif // PROCESSWINDOWS_H
