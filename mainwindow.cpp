#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<unistd.h>
#include<QDebug>
#include<QString>
#include"processwindows.h"
#include"config.h"
#include<QJsonDocument>
#include"cv.h"
#include"testthresh.h"
#include"CmdADB.h"
#include<QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Stop->setEnabled(false);

    Config *conf = new Config();
    if(conf->AUTO_RUN)
    {
        usleep((unsigned long) 3* 1000000);
        this->on_pushButton_Play_clicked();
    }
    this->loadConboxID();
    delete conf;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionChoose_Window_triggered()
{
    ProcessWindows *pw = new ProcessWindows(this);
    pw->show();
}

void MainWindow::on_pushButton_Play_clicked()
{
    ProcessWindows *pw = new ProcessWindows(this);

    for (int i= 0;i< pw->ListNameWindow.size();i++) {
        switch (i) {
            case 0:{
                    thread0 = new MyThread(this);
                    thread0->name = pw->ListNameWindow.at(i);
                    thread0->noWindow = i;
                    connect(thread0,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    thread0->start();
            }
            break;

            case 1:{
                    thread1 = new MyThread(this);
                    thread1->name = pw->ListNameWindow.at(i);
                    thread1->noWindow = i;
                    connect(thread1,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    thread1->start();
            }
            break;

            case 2:{
                    thread2 = new MyThread(this);
                    thread2->name = pw->ListNameWindow.at(i);
                    thread2->noWindow = i;
                    connect(thread2,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    thread2->start();
            }
            break;

            case 3:{
                    thread3 = new MyThread(this);
                    thread3->name = pw->ListNameWindow.at(i);
                    thread3->noWindow = i;
                    connect(thread3,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    thread3->start();
            }
            break;
        }
    }
    ui->label_process->setText("Process Running");
    ui->label_process->setStyleSheet("background-color: rgb(48, 125, 43);\ncolor: rgb(255, 255, 255);");
    ui->pushButton_Play->setEnabled(false);
    ui->pushButton_Stop->setEnabled(true);

    delete pw;
}
void MainWindow::ScreenWidowLoad(QString name)
{
    ProcessWindows * prow = new ProcessWindows();
    QString *FileName = new QString();
    *FileName = prow->MD5(name)+".jpg";
    QSize *size = new QSize();
           *size = window()->size();
    *size = QSize(size->width()/6-20, size->height()/2-20);

    ui->frame_3->setMaximumSize(*size);
    ui->frame_4->setMaximumSize(*size);
    ui->frame_5->setMaximumSize(*size);
    ui->frame_6->setMaximumSize(*size);
    ui->frame_7->setMaximumSize(*size);
    ui->frame_8->setMaximumSize(*size);
    ui->frame_9->setMaximumSize(*size);
    ui->frame_10->setMaximumSize(*size);

    switch(prow->ListNameWindow.indexOf(QRegExp(name, Qt::CaseInsensitive, QRegExp::W3CXmlSchema11), 0))
    {
        case 0:
            ui->label_imageWindow_1->setPixmap(QPixmap(*FileName));
        break;
        case 1:
            ui->label_imageWindow_2->setPixmap(QPixmap(*FileName));
        break;
        case 2:
            ui->label_imageWindow_3->setPixmap(QPixmap(*FileName));
        break;
        case 3:
            ui->label_imageWindow_4->setPixmap(QPixmap(*FileName));
        break;
        case 4:
            ui->label_imageWindow_5->setPixmap(QPixmap(*FileName));
        break;
        case 5:
            ui->label_imageWindow_6->setPixmap(QPixmap(*FileName));
        break;
        case 6:
            ui->label_imageWindow_7->setPixmap(QPixmap(*FileName));
        break;
        case 7:
            ui->label_imageWindow_8->setPixmap(QPixmap(*FileName));
        break;

    }
    delete FileName;
    FileName = NULL;
    delete size;
    size = NULL;
}
void MainWindow::JsonProcess(QString str)
{
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    QJsonObject JsonObj = doc.object();

    if(ui->textEdit->toPlainText().count()> 10000)
        ui->textEdit->clear();
    if(JsonObj.value("Wr").toString() == "true")
    {
        ui->textEdit->append("[===========================]");
        ui->textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        ui->textEdit->append(("Thread Name: "+JsonObj.value("ThreadName").toString()));
        ui->textEdit->append("Template: "+JsonObj.value("Template").toString());
        ui->textEdit->append("Threshold: "+JsonObj.value("Threshold").toString());
        ui->textEdit->append("<p><img src='"+JsonObj.value("Template").toString()+"' width='50px' height='50px'></p>");
        ui->textEdit->append("");
        ui->textEdit->append("Find Count: "+JsonObj.value("Count").toString());
    }

    this->ScreenWidowLoad(JsonObj.value("ThreadName").toString());
}
void MainWindow::threadStopAll()
{
    ProcessWindows *pw = new ProcessWindows(this);
    for (int i= 0;i< pw->ListNameWindow.size();i++) {
        switch (i) {
            case 0:{
                    thread0->bStop = true;

            }
            break;

            case 1:{
                    thread1->bStop = true;

            }
            break;

            case 2:{
                    thread2->bStop = true;

            }
            break;

            case 3:{
                    thread3->bStop = true;

            }
            break;
        }
    }
    ui->pushButton_Play->setEnabled(true);
    ui->pushButton_Stop->setEnabled(false);
    delete pw;
}

void MainWindow::on_pushButton_Stop_clicked()
{
    this->threadStopAll();
    ui->label_process->setText("Process Stop");
    ui->label_process->setStyleSheet("background-color: rgb(165, 51, 51);\ncolor: rgb(255, 255, 255);");

}

void MainWindow::on_pushButton_clicked()
{
    ui->label_Fpt->setText("Fpt Running");
    ui->label_Fpt->setStyleSheet("background-color: rgb(48, 125, 43);\ncolor: rgb(255, 255, 255);");

}

void MainWindow::on_pushButton_2_clicked()
{
    ui->label_Fpt ->setText("Fpt Stop");
    ui->label_Fpt->setStyleSheet("background-color: rgb(165, 51, 51);\ncolor: rgb(255, 255, 255);");

}

void MainWindow::on_actionConfig_triggered()
{
    Config * config = new Config(this);
    config->show();
}

void MainWindow::on_actionTest_Thresh_triggered()
{
    TestThresh *TThresh = new TestThresh(this);
    TThresh->showMaximized();

}

void MainWindow::on_pushButton_checkpowermobi_clicked()
{
    CmdADB *ADB = new CmdADB();
    ADB->Key(ui->comboBox_listID->currentText(),26); // unsleep
    usleep((unsigned long) 1* 1000000);
    ADB->Swipe(ui->comboBox_listID->currentText(),100,1400,100,0,100);
    ADB->ScreenShoot(ui->comboBox_listID->currentText());
    usleep((unsigned long) 1* 1000000);
    this->ScreenWidowLoad(ui->comboBox_listID->currentText());

    delete ADB;
}
void MainWindow::loadConboxID()
{
    ProcessWindows *proW = new ProcessWindows();
    ui->comboBox_listID->clear();
    foreach(QString NameWindow,proW->ListNameWindow)
    ui->comboBox_listID->addItem(NameWindow);
    delete proW;
}



void MainWindow::on_pushButton_refes_clicked()
{
    loadConboxID();
}
