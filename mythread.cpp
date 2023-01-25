#include "mythread.h"
#include<QDebug>
#include<QFileInfoList>
#include<QDir>
#include<QMutex>
#include<QtCore>
#include<QRect>
#include "cv.h"
#include "processwindows.h"
#include<windows.h>
#include<QCursor>
#include<QList>
#include<QString>
#include<unistd.h>
#include"config.h"
#include"mainwindow.h"
#include"CmdADB.h"

MyThread::MyThread(QObject *parent) :

    QThread(parent)

{
    this->bStop = false;
    this->unsleep = 0;
}

void MyThread::getJsonProcess(QJsonObject JsonObj)
{
    QJsonDocument doc(JsonObj);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    emit JsonProcess(strJson);
}

void MyThread::run()
{
    CmdADB *ADB = new CmdADB();
    while(!this->bStop)
    {
        ProcessWindows *processWD = new ProcessWindows();
        //CmdADB *ADB = new CmdADB();
        Config *config = new Config();
        QDir *dir = new QDir();

        if(processWD->ListNoWindow.at(this->noWindow).toInt()==0)
        {
            QPixmap pixmap = processWD->ScreenShoot(this->name);
            pixmap.save(processWD->MD5(this->name)+".jpg","jpg");

            qDebug()<<"No Window false";
            dir->setPath(config->PathFileIconAdsShowWindows);
        }else{
            ADB->ScreenShoot(this->name);
            dir->setPath(config->PathFileIconAds);
        }
        CV *cv2 = new CV;

        if(!dir->exists()){
            this->bStop = true;
            return;
        }

        QJsonObject *JsonPro = new QJsonObject();
        JsonPro->insert("ThreadName",QJsonValue(this->name));
        JsonPro->insert("Wr",QJsonValue(QString("false")));
        getJsonProcess(*JsonPro);

        delete JsonPro;
        JsonPro = NULL;

        cv::Mat matToFind = cv::imread(QString(processWD->MD5(this->name)+".jpg").toStdString());
        //cv::imwrite(QString(processWD->MD5(this->name)+"_default.jpg").toStdString(),matToFind);

        dir->setFilter(QDir::Files | QDir::NoDotAndDotDot);
        dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir->setSorting(QDir::Time);

        QFileInfoList *listF = new QFileInfoList();
        *listF = dir->entryInfoList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);



         for (int i = 0; i < listF->size(); ++i) {
                QFileInfo *fileInfo = new QFileInfo();
                *fileInfo = listF->at(i);
                QString pathTemplate = QString(fileInfo->path()+'/'+fileInfo->fileName());
                QStringList *list = new QStringList();
                        *list= fileInfo->fileName().split("_",QString::SkipEmptyParts);

                int thresh = list->at(0).toInt();


                cv::Mat matTemplate = cv::imread(pathTemplate.toStdString());
                QString saveImage = processWD->MD5(this->name)+".jpg";

                    QList<QRect>*Geo = new QList<QRect>();
                    *Geo = cv2->Template_Matching(matToFind,matTemplate,config->Threshold_max,thresh);


                    imwrite(QString(saveImage).toStdString(),cv2->IMAGE);

                    if(Geo->length()>0){
                        matToFind = cv2->IMAGE;
                        QJsonObject *JsonPro = new QJsonObject();
                        JsonPro->insert("ThreadName",QJsonValue(this->name));
                        JsonPro->insert("Template",QJsonValue(pathTemplate));
                        JsonPro->insert("Count",QJsonValue(QString::number(Geo->length())));
                        JsonPro->insert("Threshold",QJsonValue(QString::number(thresh)));
                        JsonPro->insert("Wr",QJsonValue(QString("true")));
                        getJsonProcess(*JsonPro);

                        delete JsonPro;
                        JsonPro = NULL;
                        cv2->IMAGE.release();
                    }

                    if(Geo->size()>0 && processWD->ListNoWindow.at(this->noWindow).toInt()==0){
                        foreach (QRect rect, *Geo) {
                           QRect W_geo = processWD->getXY_WH(this->name);
                           int geo_x = W_geo.x();
                           int geo_y = W_geo.y();
                           SetCursorPos(geo_x+rect.x()+ rect.width()/2, geo_y+rect.y()+ rect.height()/2);
                           mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                           mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                        }        

                    }
                    else{

                           if(Geo->length()>0 && list->at(1) == "sleep" && this->unsleep >3)
                            {
                               ADB->Key(this->name,26); //sleep
                               usleep((unsigned long) 300* 1000000);// 5 phut
                               ADB->Key(this->name,26); // unsleep
                               ADB->Swipe(this->name,100,1400,100,0,100);
                               this->unsleep =0;
                               break;
                            }
                            else if(Geo->length()==0)
                                ADB->Key(this->name,4);

                            if(Geo->length()>0 && list->at(1) != "sleep")
                                foreach (QRect rect, *Geo){

                                    ADB->Tab(this->name,rect.x()+ rect.width()/2,rect.y()+ rect.height()/2);

                                }


                        }


                    matTemplate.release();

                    delete Geo;
                    delete list;

                    Geo = NULL;
                    list = NULL;

                 }

            this->unsleep++;

            delete listF;
            listF = NULL;

            delete dir;
            dir = NULL;

            matToFind.release();
            cv2->IMAGE.release();
            usleep((unsigned long) config->RunTimeBegin * 1000000);
            delete processWD;
            delete cv2;
        }
        delete ADB;

}
