#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat usm1(Mat im, double sigma, int radius=5, double amount=0.5) {
    Mat blurred, mask, output1;
    output1 = Mat::zeros(Size(im.cols, im.rows), CV_64FC1);
    blurred = Gaussian(im, radius, sigma);
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            output1.at<double>(i,j) = (amount+1)*im.at<uchar>(i,j) - amount*blurred.at<double>(i,j);
        }
    }
    return output1;
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
                USM(im, 7.0, 0.5, 5, t);
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
    Mat im, res, res2;
    string path;
    int ev;
    cout << "Path of image: ";
    cin >> path;
    // path = "img/test.jpg";
    im = imread(path, IMREAD_GRAYSCALE);
    res = USM(im, 5.0);
    res.convertTo(res2, CV_8UC1);
    showImage(im, res2);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(480, 7680, 1);
        writeToCSVFile("csv/usm.csv", execTimeData);
    }
    return 0;
}