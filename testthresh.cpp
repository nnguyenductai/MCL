#include "testthresh.h"
#include "ui_testthresh.h"
#include"cv.h"


TestThresh::TestThresh(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestThresh)
{
    ui->setupUi(this);

    proW = new ProcessWindows();
    foreach(QString NameWindow,proW->ListNameWindow)
    ui->comboBox->addItem(NameWindow);
}

TestThresh::~TestThresh()
{
    delete ui;
}

void TestThresh::on_horizontalSlider_valueChanged(int value)
{
    ui->lineEdit->setText(QString::number(value));
    QPixmap pixmap(proW->MD5(ui->comboBox->currentText())+".jpg");
    QString saveImage = proW->MD5(ui->comboBox->currentText())+"_thresh.jpg";
    pixmap.save(saveImage,"jpg");


    CV *cv = new CV();

    cv->TESTThresh(saveImage,value);

    ui->label->setPixmap(QPixmap(saveImage));
}
