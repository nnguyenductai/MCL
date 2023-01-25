#ifndef TESTTHRESH_H
#define TESTTHRESH_H

#include <QDialog>
#include<processwindows.h>

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

private:
    Ui::TestThresh *ui;
    ProcessWindows *proW;
};

#endif // TESTTHRESH_H
