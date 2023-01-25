#include "CmdADB.h"
#include<QDebug>
#include"processwindows.h"

CmdADB::CmdADB(QObject *parent) : QObject(parent)
{
    executeCommand = new QProcess(this);
}
QList<QStringList> CmdADB::ReadListMobi()
{
    QString command = "adb.exe devices";

    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    QStringList list = output.split("\r\n",QString::SkipEmptyParts);
    list.removeAt(0);

    QList<QStringList> lists;
    foreach(QString output, list)
    {
        list = output.split("\t",QString::SkipEmptyParts);
        lists.append(list);
    }

    return lists;
}
void CmdADB::ScreenShoot(QString NameDevice)
{
    ProcessWindows *prow = new ProcessWindows();
    QString command = "adb.exe -s "+NameDevice+" shell screencap -p /sdcard/"+prow->MD5(NameDevice)+".jpg";
    qDebug()<<command;
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    qDebug()<<output;

    command = "adb.exe -s "+NameDevice+" pull -p /sdcard/"+prow->MD5(NameDevice)+".jpg "+prow->MD5(NameDevice)+".jpg";
    qDebug()<<command;
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    output = executeCommand->readAllStandardOutput();
    qDebug()<<output;
}
void CmdADB::Tab(QString NameDevice, int x, int y)
{
    QString command = "adb.exe -s "+NameDevice+" shell input tap "+QString::number(x)+" "+QString::number(y);
    qDebug()<<command;
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    qDebug()<<output;
}
void CmdADB::Key(QString NameDevice, int code)
{
    QString command = "adb.exe -s "+NameDevice+" shell input keyevent "+QString::number(code);
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    qDebug()<<output;
}
void CmdADB::Swipe(QString NameDevice, int x, int y, int toX, int toY, int speed)
{
    QString command = "adb.exe -s "+NameDevice+" shell input swipe "
            +QString::number(x)+" "
            +QString::number(y)+" "
            +QString::number(toX)+" "
            +QString::number(toY)+" "
            +QString::number(speed);
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    qDebug()<<output;
}
void CmdADB::StartApplication(QString NameDevice, QString NameApp)
{
    QString command = "adb.exe -s "+NameDevice+" shell am start -n com.package."+NameApp+"/com.package."+NameApp+".ActivityName";
    executeCommand->start(command);
    executeCommand->waitForFinished(-1);
    QString output = executeCommand->readAllStandardOutput();
    qDebug()<<output;
}
