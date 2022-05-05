#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, blurred, blurred2;
int ksize = 5;

void averagingSMT() {
    Mat kernel = Mat::ones(Size(ksize, ksize), CV_64FC1);
    int sums = ksize*ksize;
    #pragma omp parallel for collapse(2) shared(kernel)
    for (int i = 0; i < ksize; ++i) {
        for (int j = 0; j < ksize; ++j) {
            kernel.at<double>(i,j) /= sums;
        }
    }
    blurred = convolve2d(im, kernel);
    blurred.convertTo(blurred2, CV_8UC1);
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
                blurByAvg(im, 5, t);
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
    int ev;
    cout << "Path of image: ";
    cin >> path;
    im = imread(path, IMREAD_GRAYSCALE);
    averagingSMT();
    showImage(im, blurred2);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(480, 3840, 1);
        writeToCSVFile("csv/averagingblur.csv", execTimeData);
    }
    return 0;
}