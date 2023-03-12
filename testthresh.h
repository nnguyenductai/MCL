#ifndef TESTTHRESH_H
#define TESTTHRESH_H

#include <QDialog>
#include<processwindows.h>
#include<QMouseEvent>
namespace Ui {
class TestThresh;
}

class TestThresh : public QDialog
{
    Q_OBJECT

public:
    explicit TestThresh(QWidget *parent = nullptr);
    ~TestThresh();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_lineEdit_3_textEdited(const QString &arg1);

    void on_horizontalSlider_3_valueChanged(int value);

    void mousePressEvent( QMouseEvent* ev );

    void on_toolButton_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::TestThresh *ui;
    ProcessWindows *proW;
    QString saveImage;
};

#endif // TESTTHRESH_H
