#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<unistd.h>
#include<QDebug>
#include<QString>

#include"config.h"
#include<QJsonDocument>
#include"cv.h"
#include"testthresh.h"
#include"CmdADB.h"
#include<QDateTime>
#include<QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Stop->setEnabled(false);
    pw = new ProcessWindows();

    Config *conf = new Config();

    this->loadConboxID();

    if(conf->AUTO_RUN)
    {
        //usleep((unsigned long) 3* 1000000);
        this->on_pushButton_Play_clicked();
    }

    delete conf;
    ui->label_imageWindow_1->hide();
    ui->label_imageWindow_2->hide();
    ui->label_imageWindow_3->hide();
    ui->label_imageWindow_4->hide();
    ui->label_imageWindow_5->hide();
    ui->label_imageWindow_6->hide();
    ui->label_imageWindow_7->hide();
    ui->label_imageWindow_8->hide();
}

MainWindow::~MainWindow()
{
    this->threadStopAll();
    delete ui;
}

void MainWindow::on_actionChoose_Window_triggered()
{
    pw = new ProcessWindows(this);
    pw->show();
}

void MainWindow::on_pushButton_Play_clicked()
{

    for(int pow = 0; pow < ui->comboBox_listID->count();pow++)
    {
        QString nameID = ui->comboBox_listID->itemText(pow);
        QFile CheckPower(nameID+".power");
        if(CheckPower.exists())
        {
            // file exists
            CmdADB *ADB = new CmdADB();
            ADB->Key(nameID,26); // unsleep

            ADB->Swipe(nameID,400,1800,400,0,100);
            ADB->ScreenShoot(nameID);

            this->ScreenWidowLoad(nameID);

            delete ADB;

            bool checkp = CheckPower.remove();
            while(!checkp)
                checkp = CheckPower.remove();

        }
        CheckPower.close();
    }

    pw = new ProcessWindows();

    for (int i= 0;i< pw->ListNameWindow.size();i++) {
        switch (i) {
            case 0:{
                    thread0 = new MyThread(this);
                    thread0->name = pw->ListNameWindow.at(i);
                    thread0->noWindow = i;
                    connect(thread0,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    if(!thread0->isRunning())
                        thread0->start();
            }
            break;

            case 1:{
                    thread1 = new MyThread(this);
                    thread1->name = pw->ListNameWindow.at(i);
                    thread1->noWindow = i;
                    connect(thread1,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    if(!thread1->isRunning())
                        thread1->start();
            }
            break;

            case 2:{
                    thread2 = new MyThread(this);
                    thread2->name = pw->ListNameWindow.at(i);
                    thread2->noWindow = i;
                    connect(thread2,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    if(!thread2->isRunning())
                        thread2->start();
            }
            break;

            case 3:{
                    thread3 = new MyThread(this);
                    thread3->name = pw->ListNameWindow.at(i);
                    thread3->noWindow = i;
                    connect(thread3,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                    if(!thread3->isRunning())
                        thread3->start();
            }
            break;

        case 4:{
                thread4 = new MyThread(this);
                thread4->name = pw->ListNameWindow.at(i);
                thread4->noWindow = i;
                connect(thread4,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                if(!thread4->isRunning())
                    thread4->start();
        }
        break;
        case 5:{
                thread5 = new MyThread(this);
                thread5->name = pw->ListNameWindow.at(i);
                thread5->noWindow = i;
                connect(thread5,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                if(!thread5->isRunning())
                    thread5->start();
        }
        break;
        case 6:{
                thread6 = new MyThread(this);
                thread6->name = pw->ListNameWindow.at(i);
                thread6->noWindow = i;
                connect(thread6,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                if(!thread6->isRunning())
                    thread6->start();
        }
        break;
        case 7:{
                thread7 = new MyThread(this);
                thread7->name = pw->ListNameWindow.at(i);
                thread7->noWindow = i;
                connect(thread7,SIGNAL(JsonProcess(QString)), this, SLOT(JsonProcess(QString)));
                if(!thread7->isRunning())
                    thread7->start();
        }
        break;
        }
    }
    ui->label_process->setText("Process Running");
    ui->label_process->setStyleSheet("background-color: rgb(48, 125, 43);\ncolor: rgb(255, 255, 255);");
    ui->pushButton_Play->setEnabled(false);
    ui->pushButton_Stop->setEnabled(true);

}
void MainWindow::ScreenWidowLoad(QString name)
{

    QString *FileName = new QString();
    *FileName = pw->MD5(name)+".jpg";
    QSize *size = new QSize();
           *size = window()->size();
    int cs = 4;
    if( pw->ListNameWindow.length() / 4 > 1 )
        cs = 8;

    *size = QSize(size->width()/cs-98, size->height()/(cs/4)/2-50);

    ui->frame_3->setMaximumSize(*size);
    ui->frame_4->setMaximumSize(*size);
    ui->frame_5->setMaximumSize(*size);
    ui->frame_6->setMaximumSize(*size);
    ui->frame_7->setMaximumSize(*size);
    ui->frame_8->setMaximumSize(*size);
    ui->frame_9->setMaximumSize(*size);
    ui->frame_10->setMaximumSize(*size);



    switch(pw->ListNameWindow.indexOf(QRegExp(name, Qt::CaseInsensitive, QRegExp::W3CXmlSchema11), 0))
    {
        case 0:{
            ui->label_imageWindow_1->clear();
            ui->label_imageWindow_1->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_1->show();
        }
        break;
        case 1:{
            ui->label_imageWindow_2->clear();
            ui->label_imageWindow_2->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_2->show();
        }
        break;
        case 2:{
            ui->label_imageWindow_3->clear();
            ui->label_imageWindow_3->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_3->show();
        }
        break;
        case 3:{
            ui->label_imageWindow_4->clear();
            ui->label_imageWindow_4->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_4->show();
        }
        break;
        case 4:{
            ui->label_imageWindow_5->clear();
            ui->label_imageWindow_5->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_5->show();
        }
        break;
        case 5:{
            ui->label_imageWindow_6->clear();
            ui->label_imageWindow_6->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_6->show();
        }
        break;
        case 6:{
            ui->label_imageWindow_7->clear();
            ui->label_imageWindow_7->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_7->show();
        }
        break;
        case 7:{
            ui->label_imageWindow_8->clear();
            ui->label_imageWindow_8->setPixmap(QPixmap(*FileName));
            ui->label_imageWindow_8->show();
        }
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

        //this->ScreenWidowLoad(JsonObj.value("ThreadName").toString());
    }else
    {
        //pw = new ProcessWindows();
        switch (pw->ListNameWindow.indexOf(JsonObj.value("ThreadName").toString())) {
            case 0:
                ui->label_noteProcess_1->setText("Process 1: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 1:
                ui->label_noteProcess_2->setText("Process 2: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 2:
                ui->label_noteProcess_3->setText("Process 3: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 3:
                ui->label_noteProcess_4->setText("Process 4: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 4:
                ui->label_noteProcess_5->setText("Process 5: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 5:
                ui->label_noteProcess_6->setText("Process 6: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 6:
                ui->label_noteProcess_7->setText("Process 7: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;
            case 7:
                ui->label_noteProcess_8->setText("Process 8: "+QString::number(JsonObj.value("val").toInt())+"/ "+QString::number(JsonObj.value("MaxF").toInt()));
                break;

        }
    }

    this->ScreenWidowLoad(JsonObj.value("ThreadName").toString());
}
void MainWindow::threadStopAll()
{
    for (int i= 0;i< pw->ListNameWindow.size();i++) {
        switch (i) {
            case 0:{
                    thread0->bStop = true;
                    QProcess process;
                    process.moveToThread(thread0);
                    process.kill();
            }
            break;

            case 1:{
                    thread1->bStop = true;
                    QProcess process;
                    process.moveToThread(thread1);
                    process.kill();
            }
            break;

            case 2:{
                    thread2->bStop = true;
                    QProcess process;
                    process.moveToThread(thread2);
                    process.kill();
            }
            break;

            case 3:{
                    thread3->bStop = true;
                    QProcess process;
                    process.moveToThread(thread3);
                    process.kill();
            }
            break;
        case 4:{
                thread4->bStop = true;
                QProcess process;
                process.moveToThread(thread4);
                process.kill();
        }
        break;
        case 5:{
                thread5->bStop = true;
                QProcess process;
                process.moveToThread(thread5);
                process.kill();
        }
        break;
        case 6:{
                thread6->bStop = true;
                QProcess process;
                process.moveToThread(thread6);
                process.kill();
        }
        break;
        case 7:{
                thread7->bStop = true;
                QProcess process;
                process.moveToThread(thread7);
                process.kill();
        }
        break;
        }
    }
    ui->pushButton_Play->setEnabled(true);
    ui->pushButton_Stop->setEnabled(false);

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

    ADB->Swipe(ui->comboBox_listID->currentText(),400,1800,400,0,100);
    ADB->ScreenShoot(ui->comboBox_listID->currentText());

    this->ScreenWidowLoad(ui->comboBox_listID->currentText());

    delete ADB;
}
void MainWindow::loadConboxID()
{
    pw = new ProcessWindows();
    ui->comboBox_listID->clear();
    foreach(QString NameWindow,pw->ListNameWindow)
    ui->comboBox_listID->addItem(NameWindow);
}



void MainWindow::on_pushButton_refes_clicked()
{
    loadConboxID();
}

void MainWindow::on_pushButton_back_clicked()
{
    CmdADB *ADB = new CmdADB();
    ADB->Key(ui->comboBox_listID->currentText(),4); // unsleep

    ADB->ScreenShoot(ui->comboBox_listID->currentText());

    this->ScreenWidowLoad(ui->comboBox_listID->currentText());

    delete ADB;
}

void MainWindow::on_pushButton_home_clicked()
{
    CmdADB *ADB = new CmdADB();
    ADB->Key(ui->comboBox_listID->currentText(),3); // unsleep

    ADB->ScreenShoot(ui->comboBox_listID->currentText());

    this->ScreenWidowLoad(ui->comboBox_listID->currentText());

    delete ADB;
}

void MainWindow::on_pushButton_restartApp_clicked()
{
    CmdADB *ADB = new CmdADB();
    ADB->ReStartApplication(ui->comboBox_listID->currentText(),"com.bfast.bfree");
    ADB->ScreenShoot(ui->comboBox_listID->currentText());

    this->ScreenWidowLoad(ui->comboBox_listID->currentText());

    delete ADB;
}
