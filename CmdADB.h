#ifndef CMDADB_H
#define CMDADB_H

#include <QObject>
#include<QList>
#include<QProcess>
#include<QStringList>

class CmdADB : public QObject
{
    Q_OBJECT
public:
    explicit CmdADB(QObject *parent = nullptr);

    QList<QStringList> ReadListMobi();
    void ScreenShoot(QString NameDevice);
    void Tab(QString NameDevice, int x, int y);
    void Key(QString NameDevice, int code);
    void Swipe(QString NameDevice, int x, int y, int toX, int toY, int speed);
    void ReStartApplication(QString NameDevice, QString NameApp);

signals:
private:
    QProcess *executeCommand;

};

#endif // CMDADB_H
