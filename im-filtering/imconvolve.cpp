#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat resx, im, kernel;

void convolve() {
    const int dx = kernel.rows/2;
    const int dy = kernel.cols/2;
    Mat res, impadded, fkernel;
    res = Mat::zeros(Size(im.cols, im.rows), CV_64FC1);
    impadded = Mat::zeros(Size(im.cols+dx*2, im.rows+dy*2), CV_8UC1);
    flip(kernel, fkernel, -1);
    #pragma omp parallel for collapse(2) shared(im, impadded) schedule(static)
    for (int i = 1; i < impadded.rows-1; ++i) {
        for (int j = 1; j < impadded.cols-1; ++j) {
            impadded.at<uchar>(i,j) = im.at<uchar>(i-1,j-1);
        }
    }
    double tmp;
    #pragma omp parallel for shared(im, impadded, fkernel, res) private(tmp) schedule(static)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            tmp = 0;
            for (int k = 0; k < fkernel.rows; ++k) {
                for (int l = 0; l < fkernel.cols; ++l) {
                    tmp += fkernel.at<double>(k,l)*impadded.at<uchar>(i+k,j+l);
                }
            }
            res.at<double>(i,j) = tmp;
        }
    }
    res.convertTo(resx, CV_8UC1);
}

void convolve2() {
    const int dx = (kernel.rows-1)/2;
    const int dy = (kernel.cols-1)/2;
    Mat res, fkernel;
    res = Mat::zeros(Size(im.cols, im.rows), CV_64FC1);
    flip(kernel, fkernel, -1);
    double tmp;
    #pragma omp parallel for collapse(2) shared(im, fkernel, res) private(tmp) schedule(static)
    for (int i = dx; i < im.rows-dx; ++i) {
        for (int j = dy; j < im.cols-dy; ++j) {
            tmp = 0;
            for (int k = -dx; k < dx+1; ++k) {
                for (int l = -dy; l < dy+1; ++l) {
                    tmp += im.at<uchar>(i+k,j+l)*fkernel.at<double>(dx+k, dy+l);
                }
            }
            res.at<double>(i,j) = tmp;
        }
    }
    res.convertTo(resx, CV_8UC1);
}

tuple<vector<string>, vector<double>> calExecTime2(int minSize, int maxSize, int iters=10) {
    vector<string> rowHeaders;
    vector<double> execTimes;
    for (int s = minSize; s <= maxSize; s*=2) {
        string sizestr = to_string(s);
        for (int t = 1; t < 5; t*=2) {
            string path = "../img/test" + sizestr + ".jpg";
            Mat im = imread(path, IMREAD_GRAYSCALE);
            double execTime, start, end;
            execTime = 0;
            for (int i = 0; i < iters; ++i) {
                start = omp_get_wtime();
                convolve2d(im, kernel, t);
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
    kernel = (Mat_<double>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    string path;
    cout << "Path of image: ";
    cin >> path;
    im = imread(path, IMREAD_GRAYSCALE);
    convolve2();
    showImage(resx);
    tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 5);
    writeToCSVFile("../csv/convolve2d.csv", execTimeData);
    // cout << calExecTime(&convolve2) << "\n";
    return 0;
}