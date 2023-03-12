#include "processwindows.h"
#include "ui_processwindows.h"
#include<Windows.h>
#include<QWindow>
#include <QPixmap>
#include<QScreen>
#include<QDebug>
#include<QMessageBox>
#include<QString>
#include<QDir>
#include<QCryptographicHash>
#include<QBuffer>
#include"CmdADB.h"

ProcessWindows::ProcessWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProcessWindows)
{
    ui->setupUi(this);
    this->filename_window = "SetupWindow.config";
    this->row = 1;
    if(this->ListNameWindow.count()==0)
        this->loadfile();
}

ProcessWindows::~ProcessWindows()
{
    delete ui;
}
QRect ProcessWindows::getXY_WH(QString nameProcess)
{
    HWND window = FindWindow(NULL,(const wchar_t*)nameProcess.utf16());
    if(window == 0)
        return QRect();

    SetForegroundWindow(window);
    SetForegroundWindow(window);

    WId wid = (WId)window;
    QWindow *m_window;
    m_window = QWindow::fromWinId(wid);
    return m_window->geometry();
}
QPixmap ProcessWindows::ScreenShoot(QString nameProcess)
{
    HWND window = FindWindow(NULL,(const wchar_t*)nameProcess.utf16());
    if(window == 0)
        return QPixmap();
    SetForegroundWindow(window);

    WId wid = (WId)window;
    QWindow *m_window;
    m_window = QWindow::fromWinId(wid);
    QRect g = m_window->geometry();
    QScreen *screen = m_window->screen();
    return screen->grabWindow(0,g.x(),g.y(),g.width(),g.height());
}

QString ProcessWindows::MD5(QString str)
{
    return QString(QCryptographicHash::hash((str.toUtf8())
                                                    ,QCryptographicHash::Md5).toHex());
}
QByteArray ProcessWindows::CheckSha512(QString fileName)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha512);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}
void ProcessWindows::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QListWidgetItem *LWItem =  ui->listWidget->item(index.row());

    if(this->row>8) this->row = 1;

    switch (this->row) {
        case 1 : ui->lineEdit_nameWindow1->setText(LWItem->text());
            break;
        case 2 : ui->lineEdit_nameWindow2->setText(LWItem->text());
             break;
        case 3 : ui->lineEdit_nameWindow3->setText(LWItem->text());
            break;
        case 4 : ui->lineEdit_nameWindow4->setText(LWItem->text());
            break;
        case 5 : ui->lineEdit_nameWindow5->setText(LWItem->text());
            break;
        case 6 : ui->lineEdit_nameWindow6->setText(LWItem->text());
            break;
        case 7 : ui->lineEdit_nameWindow7->setText(LWItem->text());
            break;
        case 8 : ui->lineEdit_nameWindow8->setText(LWItem->text());
            break;
    }
    row++;
}

void ProcessWindows::on_pushButton_testprocess_clicked()
{
    DWORD dwPid = 0;
    HWND hWnd;
    ui->listWidget->clear();
    for (hWnd = ::FindWindowEx(NULL, NULL, NULL, NULL);
         hWnd != NULL;
         hWnd = ::FindWindowEx(NULL, hWnd , NULL, NULL))
            {
                    ::GetWindowThreadProcessId(hWnd, &dwPid);
                    const int length = GetWindowTextLengthW(hWnd);
                    wchar_t* buffer = new wchar_t[static_cast<std::size_t>(length) + 1];
                    GetWindowTextW(hWnd, buffer, length + 1);
                    std::wstring windowTitle(buffer);
                    delete[] buffer;

                    // List visible windows with a non-empty title
                    if ((IsWindowVisible(hWnd) != FALSE) && (length != 0))
                    {
                        QStringList StrList;
                        StrList.append(QString::fromWCharArray(windowTitle.c_str()));
                       ui->listWidget->addItems(StrList);
                    }
            }
}

void ProcessWindows::on_pushButton_SaveNameWindow_clicked()
{
    QFile file(this->filename_window);
    file.remove();
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream
            << "#W1="+ui->lineEdit_nameWindow1->text().trimmed()
            <<"\r\n"
           << "#W1_no_Window="+QString::number((int)ui->checkBox_noWindow_1->isChecked())
           <<"\r\n"
            << "#W2="+ui->lineEdit_nameWindow2->text().trimmed()
            <<"\r\n"
           << "#W2_no_Window="+QString::number((int)ui->checkBox_noWindow_2->isChecked())
           <<"\r\n"
            << "#W3="+ui->lineEdit_nameWindow3->text().trimmed()
            <<"\r\n"
           << "#W3_no_Window="+QString::number((int)ui->checkBox_noWindow_3->isChecked())
           <<"\r\n"
            << "#W4="+ui->lineEdit_nameWindow4->text().trimmed()
            <<"\r\n"
           << "#W4_no_Window="+QString::number((int)ui->checkBox_noWindow_4->isChecked())
           <<"\r\n"
            << "#W5="+ui->lineEdit_nameWindow5->text().trimmed()
            <<"\r\n"
           << "#W5_no_Window="+QString::number((int)ui->checkBox_noWindow_5->isChecked())
           <<"\r\n"
            << "#W6="+ui->lineEdit_nameWindow6->text().trimmed()
            <<"\r\n"
           << "#W6_no_Window="+QString::number((int)ui->checkBox_noWindow_6->isChecked())
           <<"\r\n"
            << "#W7="+ui->lineEdit_nameWindow7->text().trimmed()
            <<"\r\n"
           << "#W7_no_Window="+QString::number((int)ui->checkBox_noWindow_7->isChecked())
           <<"\r\n"
            << "#W8="+ui->lineEdit_nameWindow8->text().trimmed()
               <<"\r\n"
        << "#W8_no_Window="+QString::number((int)ui->checkBox_noWindow_7->isChecked())
        <<"\r\n";
    }
    file.close();
    this->loadfile();
}
void ProcessWindows::loadfile()
{
    QFile file(this->filename_window);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "error", file.errorString());
        this->on_pushButton_SaveNameWindow_clicked();
        this->close();
    }

    QTextStream in(&file);
     in.setCodec("UTF-8");

   this->ListNameWindow = QStringList();

    while(!in.atEnd()) {
        QString line = in.readAll();
        QStringList fields = line.split("\r\n");
        foreach (QString var, fields) {
            foreach (QString conf, var.split("#")) {
                if(conf.length()>0)
                {
                    if(conf.split("W1=").length()>1 && conf.split("W1=").at(1).length()>0){
                         ui->lineEdit_nameWindow1->setText(conf.split("W1=").at(1));
                         this->ListNameWindow.append(conf.split("W1=").at(1));
                    }
                    if(conf.split("W1_no_Window=").length()>1 && conf.split("W1_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_1->setChecked(((bool)(conf.split("W1_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W1_no_Window=").at(1));
                    }
                    if(conf.split("W2=").length()>1 && conf.split("W2=").at(1).length()>0){
                         ui->lineEdit_nameWindow2->setText(conf.split("W2=").at(1));
                         this->ListNameWindow.append(conf.split("W2=").at(1));
                    }
                    if(conf.split("W2_no_Window=").length()>1 && conf.split("W2_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_2->setChecked(((bool)(conf.split("W2_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W2_no_Window=").at(1));
                    }

                    if(conf.split("W3=").length()>1 && conf.split("W3=").at(1).length()>0){
                         ui->lineEdit_nameWindow3->setText(conf.split("W3=").at(1));
                         this->ListNameWindow.append(conf.split("W3=").at(1));
                    }
                    if(conf.split("W3_no_Window=").length()>1 && conf.split("W3_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_3->setChecked(((bool)(conf.split("W3_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W3_no_Window=").at(1));
                    }

                    if(conf.split("W4=").length()>1 && conf.split("W4=").at(1).length()>0){
                         ui->lineEdit_nameWindow4->setText(conf.split("W4=").at(1));
                         this->ListNameWindow.append(conf.split("W4=").at(1));
                    }
                    if(conf.split("W4_no_Window=").length()>1 && conf.split("W4_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_4->setChecked(((bool)(conf.split("W4_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W4_no_Window=").at(1));
                    }

                    if(conf.split("W4_no_Window=").length()>1 && conf.split("W4_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_4->setChecked(((bool)(conf.split("W4_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W4_no_Window=").at(1));
                    }

                    if(conf.split("W5=").length()>1 && conf.split("W5=").at(1).length()>0){
                         ui->lineEdit_nameWindow5->setText(conf.split("W5=").at(1));
                         this->ListNameWindow.append(conf.split("W5=").at(1));
                    }
                    if(conf.split("W5_no_Window=").length()>1 && conf.split("W5_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_5->setChecked(((bool)(conf.split("W5_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W5_no_Window=").at(1));
                    }

                    if(conf.split("W6=").length()>1 && conf.split("W6=").at(1).length()>0){
                         ui->lineEdit_nameWindow6->setText(conf.split("W6=").at(1));
                         this->ListNameWindow.append(conf.split("W6=").at(1));
                    }
                    if(conf.split("W6_no_Window=").length()>1 && conf.split("W6_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_6->setChecked(((bool)(conf.split("W6_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W6_no_Window=").at(1));
                    }

                    if(conf.split("W7=").length()>1 && conf.split("W7=").at(1).length()>0){
                         ui->lineEdit_nameWindow7->setText(conf.split("W7=").at(1));
                         this->ListNameWindow.append(conf.split("W7=").at(1));
                    }
                    if(conf.split("W7_no_Window=").length()>1 && conf.split("W7_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_7->setChecked(((bool)(conf.split("W7_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W7_no_Window=").at(1));
                    }

                    if(conf.split("W8=").length()>1 && conf.split("W8=").at(1).length()>0){
                         ui->lineEdit_nameWindow8->setText(conf.split("W8=").at(1));
                         this->ListNameWindow.append(conf.split("W8=").at(1));
                   }
                    if(conf.split("W8_no_Window=").length()>1 && conf.split("W8_no_Window=").at(1).length()>0){
                         ui->checkBox_noWindow_8->setChecked(((bool)(conf.split("W8_no_Window=").at(1).toInt())));
                         this->ListNoWindow.append(conf.split("W8_no_Window=").at(1));
                    }

                }
            }
        }
    }
    qDebug()<<"load file PRO: "<<this->ListNameWindow;
    file.close();
}

void ProcessWindows::on_pushButton_Clear_clicked()
{
    ui->lineEdit_nameWindow1->clear();
    ui->lineEdit_nameWindow2->clear();
    ui->lineEdit_nameWindow3->clear();
    ui->lineEdit_nameWindow4->clear();
    ui->lineEdit_nameWindow5->clear();
    ui->lineEdit_nameWindow6->clear();
    ui->lineEdit_nameWindow7->clear();
    ui->lineEdit_nameWindow8->clear();
    this->row = 1;
}

void ProcessWindows::on_pushButton_ScanMobi_clicked()
{
    ui->listWidget->clear();
    CmdADB *ADB = new CmdADB();
    QList<QStringList> lists = ADB->ReadListMobi();
    QStringList StrList;
    foreach (QStringList list, lists) {
        StrList.append(list.at(0));
        StrList.append("Signal:"+list.at(1));
    }

   ui->listWidget->addItems(StrList);
}
