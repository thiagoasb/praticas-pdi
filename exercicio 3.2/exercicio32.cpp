#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
    cv::Mat image, realce;
    int width, height;
    int nobjects;

    cv::Point p;
    image = cv::imread("Resources/bolhas.png", cv::IMREAD_GRAYSCALE);

    if (!image.data) {
        std::cout << "imagem nao carregou corretamente\n";
        return(-1);
    }

    width = image.cols;
    height = image.rows;
    std::cout << width << "x" << height << std::endl;

    p.x = 0;
    p.y = 0;

    //remove bordas lateral esquerda e direita 
    for (int i = 0; i < height; i++)
    {
        if (image.at<uchar>(i, 0) == 255)
        {
            p.x = 0;
            p.y = i;
            floodFill(image, p, 0);
        }

        if (image.at<uchar>(i, width - 1) == 255)
        {
            p.x = width - 1;
            p.y = i;
            floodFill(image, p, 0);
        }
    }

    //remove borda superior e inferior
    for (int j = 1; j < width; j++)
    {
        if (image.at<uchar>(0, j) == 255)
        {
            p.x = j;
            p.y = 0;
            floodFill(image, p, 0);
        }

        if (image.at<uchar>(height - 1, j) == 255)
        {
            p.x = j;
            p.y = height - 1;
            floodFill(image, p, 0);
        }
    }

    p.x = 0;
    p.y = 0;
    //pinta o fundo de cinza
    floodFill(image, p, 100);
    
    int comBuracos = 0;
    int semBuracos = 0;
   
    //conta objetos com buracos
    for (int i = 0; i < height; i++) {
        for (int j = 1; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                p.x = j - 1;
                p.y = i;
                if (image.at<uchar>(p.y, p.x) == 255)
                    floodFill(image, p, 0);
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 0) {
                comBuracos++;
                p.x = j;
                p.y = i;
                floodFill(image, p, comBuracos);
            }
        }
    }

    // busca objetos sem buracos
    nobjects = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image.at<uchar>(i, j) == 255) {
                semBuracos++;
                p.x = j;
                p.y = i;
                cv::floodFill(image, p, semBuracos);
            }
        }
    }

    std::cout << "Numero de objetos com buracos: " << comBuracos << std::endl;
    std::cout << "Numero de objetos sem buracos: " << semBuracos << std::endl;
    std::cout << "Numero de objetos total (pela soma): " << comBuracos + semBuracos << std::endl;
    std::cout << "a figura tem " << nobjects << " bolhas\n";
    cv::equalizeHist(image, realce);
    cv::imshow("image", image);
    cv::imshow("realce", realce);
    cv::imwrite("labeling.png", image);
    cv::waitKey();
    return 0;
}