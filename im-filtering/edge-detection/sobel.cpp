#include <iostream>
#include <math.h>
#include "../../helper.h"
#include "../../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, gray, gradient, gradient2, theta;

void srlSobel() {
    Mat Kx = (Mat_<double>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    Mat Ky = (Mat_<double>(3,3) << -1,-2,-1,0,0,0,1,2,1);
    Mat Gx = convolve2d(gray, Kx);
    Mat Gy = convolve2d(gray, Ky);
    gradient = Mat::zeros(Size(gray.cols, gray.rows), CV_64FC1);
    theta = Mat::zeros(Size(gray.cols, gray.rows), CV_64FC1);
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            gradient.at<double>(i,j) = gradient.at<double>(i,j)/maxValueGradient * 255.0;
        }
    }
}

void prlSobel() {
    Mat Kx = (Mat_<double>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    Mat Ky = (Mat_<double>(3,3) << -1,-2,-1,0,0,0,1,2,1);
    Mat Gx = convolve2d(gray, Kx);
    Mat Gy = convolve2d(gray, Ky);
    gradient = Mat::zeros(Size(gray.cols, gray.rows), CV_64FC1);
    theta = Mat::zeros(Size(gray.cols, gray.rows), CV_64FC1);
    #pragma omp parallel for collapse(2) shared(gradient, theta, Gx, Gy) schedule(static)
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    #pragma omp parallel for collapse(2) shared(gradient) schedule(static)
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            gradient.at<double>(i,j) = gradient.at<double>(i,j)/maxValueGradient * 255.0;
        }
    }
}

int main() {
    string path;
    cout << "Path of image: ";
    cin >> path;
    im = imread(path);
    gray = RGB2GRAY(im);
    prlSobel();
    // tuple<Mat,Mat> res = Sobel(gray);
    // gradient = get<0>(res);
    gradient.convertTo(gradient2, CV_8UC1);
    showImage(gradient2);
    cout << "Execution time (serial):\t" << calExecTime(&srlSobel) << "\n";
    cout << "Execution time (parallel):\t" << calExecTime(&prlSobel) << "\n";
    return 0;
}