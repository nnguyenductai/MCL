#ifndef CV_H
#define CV_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include<QImage>
#include<QRect>
#include<QList>
#include<QString>

class CV
{

public:
    CV();
    cv::Mat QImageToMat(const QImage & image);
    QList<QRect> Template_Matching(cv::Mat matToFind,cv::Mat matTemplate,float threshold,int thresh);
    void SURFmatching(QString path);
    void transparentPNG(QString path);
    void Morphology(QString path);
    cv::Mat IMAGE;
    void TESTThresh(QString NameFile, int thresh);
};

#endif // CV_H
