#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cv::Mat image, image_Equa;
    int width, height;
    cv::VideoCapture cap;
    vector<Mat> planes, planesEqua;
    cv::Mat histR, histG, histB;
    int nbins = 64;
    float range[] = { 0, 255 };
    const float* histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    int key;

    cap.open(0);

    if (!cap.isOpened())
    {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(CAP_PROP_FRAME_WIDTH);
    height = cap.get(CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << endl;
    std::cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins / 2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0, 0, 0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0, 0, 0));

    while (1)
    {

        cap >> image;	
        flip(image, image, 1);

        cvtColor(image, image_Equa, cv::COLOR_BGRA2GRAY);	
        split(image_Equa, planesEqua);	
        equalizeHist(planesEqua[0], planesEqua[0]);
        merge(planesEqua, image_Equa);	

        split(image, planes);

        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
            &nbins, &histrange,
            uniform, acummulate);
        calcHist(&planes[1], 1, 0, Mat(), histG, 1,
            &nbins, &histrange,
            uniform, acummulate);
        calcHist(&planes[2], 1, 0, Mat(), histB, 1,
            &nbins, &histrange,
            uniform, acummulate);

        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
        normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
        normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

        histImgR.setTo(Scalar(0));
        histImgG.setTo(Scalar(0));
        histImgB.setTo(Scalar(0));

        for (int i = 0; i < nbins; i++)
        {
            line(histImgR,
                Point(i, histh),
                Point(i, histh - cvRound(histR.at<float>(i))),
                Scalar(0, 0, 255), 1, 8, 0);
            line(histImgG,
                Point(i, histh),
                Point(i, histh - cvRound(histG.at<float>(i))),
                Scalar(0, 255, 0), 1, 8, 0);
            line(histImgB,
                Point(i, histh),
                Point(i, histh - cvRound(histB.at<float>(i))),
                Scalar(255, 0, 0), 1, 8, 0);
        }

        histImgR.copyTo(image(Rect(0, 0, nbins, histh)));
        histImgG.copyTo(image(Rect(0, histh, nbins, histh)));
        histImgB.copyTo(image(Rect(0, 2 * histh, nbins, histh)));

        namedWindow("Imagem Original", cv::WINDOW_AUTOSIZE);
        namedWindow("Imagem Equalizada", cv::WINDOW_AUTOSIZE);

        imshow("Imagem Original", image);
        imshow("Imagem Equalizada", image_Equa);

        key = waitKey(30); 
        if (key == 27) break;
    }

    return 0;
}