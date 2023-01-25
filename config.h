#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = nullptr);
    ~Config();

    float Threshold_max;
    float Threshold_min;

    int QuitNumberAdsShowWindows;
    int QuitNumberAds;

    int RunTimeBegin;
    int LoopMouseClick;
    bool AUTO_RUN;
    QString PathFileIconAdsShowWindows;
    QString PathFileIconAds;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_save_clicked();

    void loadform();

    void loadConfig();

    void on_toolButton_choodePATH_clicked();

    void on_toolButtonchoosePATH_ADS_clicked();

private:
    Ui::Config *ui;
   QString  filename;
};

#endif // CONFIG_H
