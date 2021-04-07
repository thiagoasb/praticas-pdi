#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    cv::Mat image;
    int width, height;
    cv::VideoCapture cap;
    std::vector<cv::Mat> planes;
    cv::Mat histograma, histR;
    int nbins = 64;
    float range[] = { 0, 255 };
    const float* histrange = { range };
    bool uniform = true;
    bool acummulate = false;
    int key;
    double correlacao;
    double tolerancia = 0.995;


    cap.open(0);

    if (!cap.isOpened()) {
        std::cout << "cameras indisponiveis";
        return -1;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "largura = " << width << std::endl;
    std::cout << "altura  = " << height << std::endl;

    int histw = nbins, histh = nbins / 2;
    cap >> histograma;
    split(histograma, planes);

    calcHist(&planes[0], 1, 0, cv::Mat(), histograma, 1,
        &nbins, &histrange,
        uniform, acummulate);

    cv::normalize(histograma, histograma, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    correlacao = 1;

    while (1) {
        cap >> image;
        cv::split(image, planes);
        cv::calcHist(&planes[0], 1, 0, cv::Mat(), histR, 1,
            &nbins, &histrange,
            uniform, acummulate);

        cv::normalize(histR, histR, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
        correlacao = compareHist(histR, histograma, cv::HISTCMP_CORREL);

        histograma = histR.clone();

        if (correlacao < tolerancia)
        {
            cv::putText(image, "Alarme ativado!", cv::Point(10, 55),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
        }


        cv::imshow("image", image);
        key = cv::waitKey(30);
        if (key == 27) break;
    }
    return 0;
}