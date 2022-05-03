#include <iostream>
#include <math.h>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat gray, gradient, gradient2, theta;

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

tuple<vector<string>, vector<double>> calExecTime2(int minSize, int maxSize, int iters=10) {
    vector<string> rowHeaders;
    vector<double> execTimes;
    for (int s = minSize; s <= maxSize; s*=2) {
        string sizestr = to_string(s);
        for (int t = 1; t < 5; t*=2) {
            string path = "img/test" + sizestr + ".jpg";
            Mat im = imread(path, IMREAD_GRAYSCALE);
            double execTime, start, end;
            execTime = 0;
            for (int i = 0; i < iters; ++i) {
                start = omp_get_wtime();
                Sobel(im, t);
                end = omp_get_wtime();
                execTime += end-start;
            }
            execTime /= iters;
            execTimes.push_back(execTime);
        }
        rowHeaders.push_back(sizestr+"x"+sizestr);
    }
    return {rowHeaders, execTimes};
}

int main() {
    string path;
    cout << "Path of image: ";
    cin >> path;
    gray = imread(path, IMREAD_GRAYSCALE);
    prlSobel();
    gradient.convertTo(gradient2, CV_8UC1);
    showImage(gradient2);
    tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 1);
    writeToCSVFile("csv/sobel.csv", execTimeData);
    return 0;
}