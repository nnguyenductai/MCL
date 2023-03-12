#ifndef MYTHREAD_H

#define MYTHREAD_H


#include <QObject>
#include <QThread>
#include <QString>
#include<QJsonObject>
#include<QList>
#include<QRect>
#include "cv.h"
#include "processwindows.h"
#include"config.h"

class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread();
    explicit MyThread(QObject *parent = 0);

    // overriding the QThread's run() method
    void run();
    bool bStop;
    QString name;
    int noWindow;

signals:
    void JsonProcess(QString);

private:
    void getJsonProcess(QJsonObject);
    int unsleep;
    QList<cv::Mat> *List_matTemplate;
    QStringList *listNameIcon;
    ProcessWindows *processWD;
    Config *config;
};

#endif // MYTHREAD_H
