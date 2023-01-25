#include "cv.h"
#include"config.h"
#include<QDebug>


using namespace cv;
CV::CV()
{

}
void CV::TESTThresh(QString NameFile,int thresh)
{
    cv::Mat matToFind;
    matToFind = cv::imread(NameFile.toStdString());
    cv::cvtColor(matToFind, matToFind, COLOR_BGR2GRAY);
    cv::cvtColor(matToFind, matToFind, COLOR_GRAY2RGB);

    cv::threshold(matToFind, matToFind, thresh,255, THRESH_BINARY);

    imwrite(QString(NameFile).toStdString(),matToFind);
    //imshow("Test",matToFind);
    //waitKey(0);
}
QList<QRect> CV::Template_Matching(cv::Mat matToFind,cv::Mat matTemplate,float threshold,int thresh)
{
    QList<QRect> listGeo;

    //load ảnh
    //cv::Mat matToFind = cv::imread("stock.jpg");
    //cv::Mat matTemplate = cv::imread("coca2.jpg");

    //kiểm tra kích cỡ của ảnh input & template
    if (matTemplate.rows > matToFind.rows || matTemplate.cols > matToFind.cols)
    {
        //std::cout<<("Mat template must be smaller than matInput");
        return listGeo;
    }
    cv::Mat matToFindRGB = matToFind;

    cv::cvtColor(matToFind, matToFind, COLOR_BGR2GRAY);
    cv::cvtColor(matToFind, matToFind, COLOR_GRAY2RGB);

    cv::threshold(matToFind, matToFind, thresh,255, THRESH_BINARY);

    cv::Mat matDisplay, result;
    matToFind.copyTo(matDisplay);
    int result_cols = matToFind.cols - matTemplate.cols + 1;
    int result_rows = matToFind.rows - matTemplate.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);

    //tìm mẫu
    cv::matchTemplate(matToFind, matTemplate, result, cv::TM_CCORR_NORMED);


    cv::threshold(result, result, 0.9, 1., cv::THRESH_TOZERO);

    double minval, maxval;
    //ngưỡng chính xác, giảm xuống thì sẽ tìm được nhiều đối tượng hơn nhưng sai số nhiều hơn



    while (true)
    {

        cv::Point minloc, maxloc;
        cv::minMaxLoc(result, &minval, &maxval, &minloc, &maxloc);

        if (maxval > threshold)
        {
            //vẽ hình chữ nhật lên đối tượng tìm được

            cv::rectangle(
                matToFindRGB,
                maxloc,
                cv::Point(maxloc.x + matTemplate.cols, maxloc.y + matTemplate.rows),
                cv::Scalar(0, 255, 0), 2
            );
            cv::floodFill(result, maxloc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
            listGeo.append(QRect(
                                maxloc.x
                                ,maxloc.y
                                ,matTemplate.cols
                                ,matTemplate.rows
                               ));
        }
        else
            break;
    }
    this->IMAGE = matToFindRGB;
    matToFind.release();
    matTemplate.release();
    matDisplay.release();
    matToFindRGB.release();
    return listGeo;
}

void CV::SURFmatching(QString path)
{
    cv::Mat src = cv::imread(path.toStdString());
        if (!src.data)
            return;

        cv::imshow("src", src);

        // Create binary image from source image
        cv::Mat bw;
        cv::cvtColor(src, bw, cv::COLOR_BGR2GRAY);
        cv::threshold(bw, bw, 40, 255, cv::THRESH_BINARY);
        cv::imshow("bw", bw);

        // Perform the distance transform algorithm
        cv::Mat dist;
        cv::distanceTransform(bw, dist, cv::DIST_L2, 3);

        // Normalize the distance image for range = {0.0, 1.0}
        // so we can visualize and threshold it
        cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
        cv::imshow("dist", dist);

        // Threshold to obtain the peaks
        // This will be the markers for the foreground objects
        cv::threshold(dist, dist, .5, 1., cv::THRESH_BINARY);
        cv::imshow("dist2", dist);

        // Create the CV_8U version of the distance image
        // It is needed for cv::findContours()
        cv::Mat dist_8u;
        dist.convertTo(dist_8u, CV_8U);

        // Find total markers
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(dist_8u, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        int ncomp = contours.size();

        qDebug()<< "Number of coins: " << ncomp;

        // Create the marker image for the watershed algorithm
        cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1);

        // Draw the foreground markers
        for (int i = 0; i < ncomp; i++)
            cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);

        // Draw the background marker
        cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
        //cv::imshow("markers", markers*10000);

        // Perform the watershed algorithm
        cv::watershed(src, markers);

        // Generate random colors
        std::vector<cv::Vec3b> colors;
        for (int i = 0; i < ncomp; i++)
        {
            int b = cv::theRNG().uniform(0, 255);
            int g = cv::theRNG().uniform(0, 255);
            int r = cv::theRNG().uniform(0, 255);

            colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
        }

        // Create the result image
        cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);

        // Fill labeled objects with random colors
        for (int i = 0; i < markers.rows; i++)
        {
            for (int j = 0; j < markers.cols; j++)
            {
                int index = markers.at<int>(i,j);
                if (index > 0 && index <= ncomp)
                    dst.at<cv::Vec3b>(i,j) = colors[index-1];
                else
                    dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
            }
        }

        cv::imshow("dst", dst);
        cv::waitKey(0);
}

void CV::transparentPNG(QString path)
{
    cv::Mat mat = cv::imread(path.toStdString());

    cv::Mat matGRBA;
    cv::cvtColor(mat, matGRBA, cv::COLOR_RGB2BGR);

    for (int y = 0; y < matGRBA.rows; y++)
    {
        for (int x = 0; x < matGRBA.cols; x++)
        {
            cv::Vec4b val = matGRBA.at<cv::Vec4b>(y, x);
            if (val[0] > 200 && val[1] > 200 && val[2] > 200)
            {
                matGRBA.at<cv::Vec4b>(y, x) = cv::Vec4b(0, 0, 0, 0);
            }
        }
    }

    //cv::imwrite("dog_rgba.png", matGRBA);
    cv::imshow("transparent", matGRBA);

    cv::waitKey();

}
void CV::Morphology(QString path)
{
    // Load the image
        Mat src = imread(path.toStdString());
        // Check if image is loaded fine
        if(!src.data)
            qDebug() << "Problem loading image!!!";
        // Show source image
        imshow("src", src);
        // Transform source image to gray if it is not
        Mat gray;
        if (src.channels() == 3)
        {
            cvtColor(src, gray, COLOR_BGR2GRAY);
        }
        else
        {
            gray = src;
        }
        // Show gray image
        imshow("gray", gray);
        // Apply adaptiveThreshold at the bitwise_not of gray, notice the ~ symbol
        Mat bw;
        adaptiveThreshold(~gray, bw, 255, cv::ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
        // Show binary image
        imshow("binary", bw);
        // Create the images that will use to extract the horizontal and vertical lines
        Mat horizontal = bw.clone();
        Mat vertical = bw.clone();
        // Specify size on horizontal axis
        int horizontalsize = horizontal.cols / 30;
        // Create structure element for extracting horizontal lines through morphology operations
        Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontalsize,1));
        // Apply morphology operations
        erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
        dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
        // Show extracted horizontal lines
        imshow("horizontal", horizontal);
        // Specify size on vertical axis
        int verticalsize = vertical.rows / 30;
        // Create structure element for extracting vertical lines through morphology operations
        Mat verticalStructure = getStructuringElement(MORPH_RECT, Size( 1,verticalsize));
        // Apply morphology operations
        erode(vertical, vertical, verticalStructure, Point(-1, -1));
        dilate(vertical, vertical, verticalStructure, Point(-1, -1));
        // Show extracted vertical lines
        imshow("vertical", vertical);
        // Inverse vertical image
        bitwise_not(vertical, vertical);
        imshow("vertical_bit", vertical);
        // Extract edges and smooth image according to the logic
        // 1. extract edges
        // 2. dilate(edges)
        // 3. src.copyTo(smooth)
        // 4. blur smooth img
        // 5. smooth.copyTo(src, edges)
        // Step 1
        Mat edges;
        adaptiveThreshold(vertical, edges, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, -2);
        imshow("edges", edges);
        // Step 2
        Mat kernel = Mat::ones(2, 2, CV_8UC1);
        dilate(edges, edges, kernel);
        imshow("dilate", edges);
        // Step 3
        Mat smooth;
        vertical.copyTo(smooth);
        // Step 4
        blur(smooth, smooth, Size(2, 2));
        // Step 5
        smooth.copyTo(vertical, edges);
        // Show final result
        imshow("smooth", vertical);
        waitKey(0);
}
