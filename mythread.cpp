#include "mythread.h"
#include<QDebug>
#include<QFileInfoList>
#include<QDir>
#include<QMutex>
#include<QtCore>
#include<QRect>

#include<windows.h>
#include<QCursor>
#include<QList>
#include<QString>
#include<unistd.h>

#include"mainwindow.h"
#include"CmdADB.h"
#include<QFile>

MyThread::MyThread(QObject *parent) :

    QThread(parent)

{
    this->bStop = false;
    this->unsleep = 0;
    processWD = new ProcessWindows();
    QDir *dir = new QDir();
    config = new Config();
    dir->setPath(config->PathFileIconAds);

    dir->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    dir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir->setSorting(QDir::Time | QDir::Reversed);

    QFileInfoList *listF = new QFileInfoList();
    *listF = dir->entryInfoList(QStringList() << "*.jpg" << "*.JPG",QDir::Files);
    List_matTemplate = new QList<cv::Mat>;
    listNameIcon = new QStringList();
    QFileInfo *fileInfo = new QFileInfo();
    foreach(*fileInfo,*listF){
            QString pathTemplate = QString(fileInfo->path()+'/'+fileInfo->fileName());

            listNameIcon->append(fileInfo->fileName());



        List_matTemplate->append(cv::imread(pathTemplate.toStdString()));
        //qDebug()<<"INSERT :"<<List_matTemplate->count();
    }
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
        int stopLoop = 0;

        //CmdADB *ADB = new CmdADB();

        QDir *dir = new QDir();
        QString PATH;

        //if(processWD->ListNoWindow.at(this->noWindow).toInt()==0)
        //{
            //QPixmap pixmap = processWD->ScreenShoot(this->name);
            //pixmap.save(processWD->MD5(this->name)+".jpg","jpg");

           // qDebug()<<"No Window false";
            //PATH = config->PathFileIconAdsShowWindows+'/';
        //}else{
            ADB->ScreenShoot(this->name);
            PATH = config->PathFileIconAds+'/';
        //}


        if(!dir->exists()){
            this->bStop = true;
            return;
        }


        QString saveImage = processWD->MD5(this->name)+".jpg";

        //checksha512
        QString hash = processWD->CheckSha512(saveImage).toHex();
        QString NameFile = "MOUSE/"+hash+".geo";


        if(QFile::exists(NameFile))
        {
            QFile file(NameFile);
            if(!file.open(QIODevice::ReadOnly))
                qDebug()<<"error open file GEO!";
            qDebug()<<"Open file GEO: "+hash;
            QTextStream in(&file);
            //in.setCodec("UTF-8");
            QStringList StrGeo;
            while(!in.atEnd()) {
                QString line = in.readAll().toUtf8();
                 StrGeo= line.split(",");


            }
            ADB->Tab(this->name,StrGeo.at(0).toInt()+ StrGeo.at(2).toInt()/2
                     ,StrGeo.at(1).toInt()+ StrGeo.at(3).toInt()/2);

            qDebug()<<StrGeo.at(0).toInt()
                   << StrGeo.at(1).toInt()
                   <<StrGeo.at(2).toInt()
                  <<StrGeo.at(3).toInt();
            file.close();
            QJsonObject *JsonPro = new QJsonObject();
            JsonPro->insert("ThreadName",QJsonValue(this->name));
            JsonPro->insert("MaxF",QJsonValue(listNameIcon->size()));
            JsonPro->insert("val",QJsonValue("Cache"));
            JsonPro->insert("Wr",QJsonValue(QString("false")));
            getJsonProcess(*JsonPro);
            delete JsonPro;
            JsonPro = NULL;
         }else{
        //

                CV *cv2 = new CV;
                cv::Mat matToFind = cv::imread(saveImage.toStdString());

                int i=0;
                //qDebug()<<"List MatTemp: "<<List_matTemplate->count();
                foreach(cv::Mat matTemplate, *List_matTemplate){
                        if(this->bStop) break;
                        QString NameIcon = listNameIcon->at(i);

                        int thresh = NameIcon.split("_",QString::SkipEmptyParts).at(0).toInt();

                        QList<QRect>*Geo = new QList<QRect>();
                        *Geo = cv2->Template_Matching(matToFind,matTemplate,config->Threshold_max,thresh);


                            if(Geo->length()>0){
                                imwrite(QString(saveImage).toStdString(),cv2->IMAGE);
                                matToFind = cv2->IMAGE;
                                QJsonObject *JsonPro = new QJsonObject();
                                JsonPro->insert("ThreadName",QJsonValue(this->name));
                                JsonPro->insert("Template",QJsonValue(PATH+NameIcon));
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
                                stopLoop++;

                                if(config->QuitNumberAdsShowWindows <= stopLoop)
                                    break;

                            }
                            else{

                                   if(Geo->length()>0 && NameIcon.split("_",QString::SkipEmptyParts).at(1) == "sleep" && this->unsleep >3)
                                    {
                                       ADB->Key(this->name,26); //sleep

                                       QFile file(this->name+".power");
                                       if(file.open(QIODevice::WriteOnly|QIODevice::Text))
                                           file.write("");
                                       file.close();

                                       usleep((unsigned long) 300* 1000000);// 5 phut
                                       ADB->Key(this->name,26); // unsleep
                                       ADB->Swipe(this->name,400,1800,400,0,100);
                                       this->unsleep =0;

                                       QFile filer(this->name+".power");
                                       if(filer.open(QIODevice::WriteOnly|QIODevice::Text))
                                       filer.remove(); // remove file .power
                                       filer.close();
                                       break;
                                    }
                                    /*else if(Geo->length()==0)
                                        ADB->Key(this->name,4);
                                    */
                                    if(Geo->length()>0 && NameIcon.split("_",QString::SkipEmptyParts).at(1) != "sleep")
                                    {
                                        foreach (QRect rect, *Geo){

                                            ADB->Tab(this->name,rect.x()+ rect.width()/2,rect.y()+ rect.height()/2);

                                            if(rect.y()+ rect.height()/2 < 300)
                                            {
                                                QFile sFile(NameFile);
                                                if(sFile.open(QIODevice::ReadWrite))
                                                {
                                                    QTextStream out(&sFile);
                                                    out << QString::number(rect.x())
                                                           +','
                                                           +QString::number(rect.y())
                                                           +','
                                                           +QString::number(rect.width())
                                                           +','
                                                           +QString::number(rect.height());

                                                    // optional, as QFile destructor will already do it:
                                               }
                                               sFile.close();
                                            }

                                        }
                                        stopLoop++;

                                    }
                                    if(config->QuitNumberAds <= stopLoop)
                                        break;
                                }


                            matTemplate.release();

                            delete Geo;

                            Geo = NULL;

                            QJsonObject *JsonPro = new QJsonObject();
                            JsonPro->insert("ThreadName",QJsonValue(this->name));
                            JsonPro->insert("MaxF",QJsonValue(listNameIcon->size()));
                            JsonPro->insert("val",QJsonValue(++i));
                            JsonPro->insert("Wr",QJsonValue(QString("false")));
                            getJsonProcess(*JsonPro);
                            delete JsonPro;
                            JsonPro = NULL;

                         }
                         ADB->Key(this->name,4);
                  //end foreach
                    this->unsleep++;

                    delete dir;
                    dir = NULL;

                    matToFind.release();
                    cv2->IMAGE.release();

                    delete cv2;
            }
            usleep((unsigned long) config->RunTimeBegin * 1000000);

        }
        delete ADB;

}
