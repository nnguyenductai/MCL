#include "config.h"
#include "ui_config.h"
#include<QFile>
#include<QMessageBox>
#include<QTextStream>
#include<QDebug>
#include<QFileDialog>

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
    this->loadConfig();
}

Config::~Config()
{
    delete ui;
}
void Config::loadConfig()
{
    this->filename = "setup.conf";

    QFile file(this->filename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readAll();
        QStringList fields = line.split("\r\n");
        foreach (QString var, fields) {
            foreach (QString conf, var.split("#")) {
                if(conf.length()>0)
                {

                    if(conf.split("PATH_FILE_ICON=").length()>1)
                        ui->lineEdit_PATH_ADS_SHOW->setText(conf.split("PATH_FILE_ICON=").at(1));

                    if(conf.split("PATH_FILE_ICON_ADS=").length()>1)
                        ui->lineEdit_PATH_ADS->setText(conf.split("PATH_FILE_ICON_ADS=").at(1));

                    if(conf.split("QUIT_NUMBER=").length()>1)
                        ui->lineEdit_QuitNumber_adsShow->setText(conf.split("QUIT_NUMBER=").at(1));

                    if(conf.split("QUIT_NUMBER_ADS=").length()>1)
                        ui->lineEdit_QuitNumberAds->setText(conf.split("QUIT_NUMBER_ADS=").at(1));

                    if(conf.split("THRESHOLD_MIN=").length()>1)
                        ui->lineEdit_threshold_min->setText(conf.split("THRESHOLD_MIN=").at(1));

                    if(conf.split("THRESHOLD_MAX=").length()>1)
                        ui->lineEdit_threshold_max->setText(conf.split("THRESHOLD_MAX=").at(1));

                    if(conf.split("TIME_RUN=").length()>1)
                        ui->lineEdit_runbegintime->setText(conf.split("TIME_RUN=").at(1));

                    if(conf.split("LOOP_MOUSE_CLICK=").length()>1)
                        ui->lineEdit_LoopMouseClick->setText(conf.split("LOOP_MOUSE_CLICK=").at(1));

                    if(conf.split("AUTO_RUN=").length()>1){

                       if(conf.split("AUTO_RUN=").at(1).toUpper() == "TRUE")
                            ui->checkBox_runapp->setChecked(true);
                       else  if(conf.split("AUTO_RUN=").at(1).toUpper() == "FALSE")
                           ui->checkBox_runapp->setChecked(false);
                    } 

                }
            }
        }
    }

    file.close();
    this->loadform();
}
void Config::on_pushButton_2_clicked()
{
    this->close();
}

void Config::loadform(){
    this->PathFileIconAdsShowWindows = ui->lineEdit_PATH_ADS_SHOW->text();
    this->PathFileIconAds = ui->lineEdit_PATH_ADS->text();

    this->QuitNumberAdsShowWindows = ui->lineEdit_QuitNumber_adsShow->text().toInt();
    this->QuitNumberAds = ui->lineEdit_QuitNumberAds->text().toInt();

    this->Threshold_min = ui->lineEdit_threshold_min->text().toFloat();
    this->Threshold_max = ui->lineEdit_threshold_max->text().toFloat();

    this->RunTimeBegin = ui->lineEdit_runbegintime->text().toInt();

    this->LoopMouseClick = ui->lineEdit_LoopMouseClick->text().toInt();

    this->AUTO_RUN = ui->checkBox_runapp->isChecked();
}
void Config::on_pushButton_save_clicked()
{
    QString checkbox = ui->checkBox_runapp->isChecked() ? "true" : "false";
    QFile file(this->filename);
    file.remove();
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream
            <<"#PATH_FILE_ICON="+ui->lineEdit_PATH_ADS_SHOW->text()
            <<"\r\n"
           <<"#PATH_FILE_ICON_ADS="+ui->lineEdit_PATH_ADS->text()
           <<"\r\n"
           <<"#THRESHOLD_MIN="+ui->lineEdit_threshold_min->text()
           <<"\r\n"
          <<"#THRESHOLD_MAX="+ui->lineEdit_threshold_max->text()
          <<"\r\n"
          <<"#QUIT_NUMBER="+ui->lineEdit_QuitNumber_adsShow->text()
          <<"\r\n"
         <<"#QUIT_NUMBER_ADS="+ui->lineEdit_QuitNumberAds->text()
         <<"\r\n"
            <<"#TIME_RUN="+ui->lineEdit_runbegintime->text()
            <<"\r\n"
           <<"#LOOP_MOUSE_CLICK="+ui->lineEdit_LoopMouseClick->text()
           <<"\r\n"
            <<"#AUTO_RUN="+ checkbox;
    }

        this->loadConfig();
        this->close();
}

void Config::on_toolButton_choodePATH_clicked()
{
    QString Filepath = QFileDialog::getExistingDirectory(this, "Choose File Icon");
    ui->lineEdit_PATH_ADS_SHOW->setText(Filepath);
}

void Config::on_toolButtonchoosePATH_ADS_clicked()
{
    QString Filepath = QFileDialog::getExistingDirectory(this, "Choose File Icon Ads");
    ui->lineEdit_PATH_ADS->setText(Filepath);
}
