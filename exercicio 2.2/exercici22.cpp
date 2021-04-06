#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**) {

    cv::Mat image;
    cv::Vec3b val;

    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    image = cv::imread("Resources/biel.png", cv::IMREAD_GRAYSCALE);
    if (!image.data)
        std::cout << "nao abriu biel.png" << std::endl;

    cv::namedWindow("negativo", cv::WINDOW_AUTOSIZE);

    cout << "Digite as coordenadas de P1 (x,y):" << endl;
    cout << "x1: ";
    cin >> x1;
    cout << "y1: ";
    cin >> y1;

    cout << endl;

    cout << "Digite as coordenadas de P2 (x,y):" << endl;
    cout << "x2: ";
    cin >> x2;
    cout << "y2: ";
    cin >> y2;

    for (int i = x1;i < x2;i++) {
        for (int j = y1;j < y2;j++) {
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);
        }
    }
    
    cv::imshow("negativo", image);

    //Exercício 2.2 - 1
    Mat image_org = cv::imread("Resources/biel.png", cv::IMREAD_GRAYSCALE);
    Mat image2 = image_org.clone();

    Size size = image2.size();

    Mat img_part1(image_org, Rect(0, 0, size.height / 2, size.width / 2));
    Mat img_part2(image_org, Rect(size.width / 2, 0, size.height / 2, size.width / 2));
    Mat img_part3(image_org, Rect(0, size.height / 2, size.height / 2, size.width / 2));
    Mat img_part4(image_org, Rect(size.height / 2, size.width / 2, size.height / 2, size.width / 2));

    img_part4.copyTo(image2(Rect(0, 0, size.height / 2, size.width / 2)));
    img_part1.copyTo(image2(Rect(size.height / 2, size.width / 2, size.height / 2, size.width / 2)));
    img_part2.copyTo(image2(Rect(0, size.height / 2, size.height / 2, size.width / 2)));
    img_part3.copyTo(image2(Rect(size.width / 2, 0, size.height / 2, size.width / 2)));

    namedWindow("Trocar regioes", WINDOW_AUTOSIZE);

    cv::imshow("Trocar regioes", image2);
    cv::waitKey();

    return 0;
}