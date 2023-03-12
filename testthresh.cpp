#include "testthresh.h"
#include "ui_testthresh.h"
#include"cv.h"
#include<QDebug>
#include<QGraphicsScene>
#include<QGraphicsPixmapItem>
#include<QFileDialog>
#include<QDateTime>

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
    saveImage = proW->MD5(ui->comboBox->currentText())+"_thresh.jpg";
    pixmap.save(saveImage,"jpg");


    CV *cv = new CV();

    cv->TESTThresh(saveImage,value);

    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(saveImage);
    scene->addItem(item);
}

void TestThresh::on_lineEdit_textEdited(const QString &arg1)
{
    ui->horizontalSlider->setValue(arg1.toInt());
    this->on_horizontalSlider_valueChanged(arg1.toInt());
}

void TestThresh::on_lineEdit_2_textEdited(const QString &arg1)
{
    ui->horizontalSlider_2->setValue(arg1.toInt());

}

void TestThresh::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_IconSize->setMinimumWidth(value);
    ui->lineEdit_2->setText(QString::number(value));
}

void TestThresh::on_lineEdit_3_textEdited(const QString &arg1)
{
    ui->horizontalSlider_3->setValue(arg1.toInt());
}

void TestThresh::on_horizontalSlider_3_valueChanged(int value)
{

    ui->lineEdit_3->setText(QString::number(value));
}
void TestThresh::mousePressEvent( QMouseEvent* event )
{
    QPoint remapped = ui->graphicsView->mapFromParent( event->pos() );
    if ( ui->graphicsView->rect().contains( remapped ) )
    {
         QPointF mousePoint = ui->graphicsView->mapToScene( remapped );
         int w = ui->lineEdit_2->text().toInt();
         int h = ui->lineEdit_3->text().toInt();
         int x = mousePoint.x()-(w/2);
         int y = mousePoint.y()-(h/2);
         QPixmap pixCuter(saveImage);
         pixCuter = pixCuter.copy(x,y,w,h);
         ui->label_IconSize->setPixmap(pixCuter);
         ui->lineEdit_nameIcon->setText(ui->lineEdit->text()
                +'_'+proW->MD5(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    }
}

void TestThresh::on_toolButton_clicked()
{
    QString FilepathSave = QFileDialog::getExistingDirectory(this, "Choose path save File Icon");
    ui->lineEdit_pathsaveIcon->setText(FilepathSave+'/');
}

void TestThresh::on_pushButton_save_clicked()
{
    if(ui->lineEdit_pathsaveIcon->text().length()>4)
        ui->label_IconSize->pixmap()->save(ui->lineEdit_pathsaveIcon->text()+ui->lineEdit_nameIcon->text()+".jpg");
}
