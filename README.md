# Exercícios Práticos - Processamento Digital de Imagens

- [Sobre](#-sobre)
- [Exercício 2.2](#-exercício-2.2)
- [Exercício 3.2](#-exercício-2.2)
- [Exercício 4.2](#-exercício-4.2)
- [Exercício 5.2](#-exercício-5.2)
- [Exercício 8.2](#-exercício-8.2)
- [Exercício 9.2](#-exercício-9.2)

## Sobre
Práticas realizadas durante a disciplina de Processamento Digital de Imagens, ministrada pelo professor Agostinho Brito. As práticas realizadas a seguir foram feitas pelo professor e podem ser consultadas no tutorial [Processamento Digital de Imagens com OpenCV](https://agostinhobritojr.github.io/tutorial/pdi/#_pref%C3%A1cio).

Componentes:
- Francisco Kennedi de Macedo Rodrigues
- Thiago de Ara ́ujo Silva Bezerra

## Exercício 2.2
- **Utilizando o programa [exemplos/pixels.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/pixels.cpp) como referência, implemente um programa regions.cpp. Esse programa deverá solicitar ao usuário as coordenadas de dois pontos P1 e P2 localizados dentro dos limites do tamanho da imagem e exibir que lhe for fornecida. Entretanto, a região definida pelo retângulo de vértices opostos definidos pelos pontos P1 e P2 será exibida com o negativo da imagem na região correspondente.**

Foi criada as variáveis x1, x2, y1, y2 que seram recebidas pelo usuário e representam os pontos dos vértices que delimitará a região onde será representado o negativo da imagem (efeito gerado pela expressão *255 - image.at<uchar>(i, j)*):
```cpp
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

```
Terminal:
<p align="center">
  <img alt="biel_original" src="./exercicio 2.2/terminal.png">
</p>

Output:
<p align="center">
  <img alt="biel_original" src="./exercicio 2.2/biel.png">
  <img alt="biel_negativo" src="./exercicio 2.2/output1.png">
</p>

- **Utilizando o programa [exemplos/pixels.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/pixels.cpp) como referência, implemente um programa trocaregioes.cpp. Seu programa deverá trocar os quadrantes em diagonal na imagem. Explore o uso da classe Mat e seus construtores para criar as regiões que serão trocadas.**

Foram criadas 4 variáveis que capturam os pontos de uma região retangular de uma imagem (através do método Rect) e em seguida é realizada a cópia de cada um dos retângulos armazenados nessas variáveis para a posição escolhida (em outra variável) através do método *copyTo*.

```cpp
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
```
Output:
<p align="center">
  <img alt="biel_trocado" src="./exercicio 2.2/output2.png">
</p>

## Exercício 3.2
- **Observando-se o programa [labeling.cpp](https://agostinhobritojr.github.io/tutorial/pdi/exemplos/labeling.cpp) como exemplo, é possível verificar que caso existam mais de 255 objetos na cena, o processo de rotulação poderá ficar comprometido. Identifique a situação em que isso ocorre e proponha uma solução para este problema.**

Uma possível solução para o problema de procurar mais de 255 objetos na cena seria ter um background com uma cor específica, ao encontrar objetos com cor diferente da do background preencher aquela região e incrementar o contador da contagem de objetos na imagem. 

- **Aprimore o algoritmo de contagem apresentado para identificar regiões com ou sem buracos internos que existam na cena. Assuma que objetos com mais de um buraco podem existir. Inclua suporte no seu algoritmo para não contar bolhas que tocam as bordas da imagem. Não se pode presumir, a priori, que elas tenham buracos ou não.**

Primeiramente foi retirado todos as bolhas das bordas da imagem (pois não se sabe se possui buracos ou não), em seguida foi alterada a cor do background para garantir que estivesse com uma cor diferentes do objeto. Um objeto com buracos é contabilizado quando é encontrado um pixel com a cor preta e este objeto é totalmente preenchido com a mesma cor.

```cpp
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
    cv::equalizeHist(image, realce);
    cv::imshow("image", image);
    cv::imshow("realce", realce);
    cv::imwrite("labeling.png", image);
```
Terminal:
<p align="center">
  <img alt="terminal3" src="./exercicio 3.2/terminal.png">
</p>

Output:
<p align="center">
  <img alt="saida3" src="./exercicio 3.2/output_image.png">
  <img alt="outputrealce" src="./exercicio 3.2/output_realce.png">
</p>

