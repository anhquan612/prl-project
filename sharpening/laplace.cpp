#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, output1, output2;
double weight = 0.1;

void srlLaplaceSPN() {
    output1 = Mat::zeros(Size(im.cols, im.rows), CV_64FC1);
    Mat kernel = (Mat_<double>(3,3) << 0,1,0,1,-4,1,0,1,0);
    Mat convolved = convolve2d(im, kernel);
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            output1.at<double>(i,j) = im.at<uchar>(i,j) - weight*convolved.at<double>(i,j);
        }
    }
}

void prlLaplaceSPN() {
    output1 = Mat::zeros(Size(im.cols, im.rows), CV_64FC1);
    Mat kernel = (Mat_<double>(3,3) << 0,1,0,1,-4,1,0,1,0);
    Mat convolved = convolve2d(im, kernel);
    #pragma omp parallel for collapse(2) shared(output1, im, convolved)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            output1.at<double>(i,j) = im.at<uchar>(i,j) - weight*convolved.at<double>(i,j);
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
                LaplaceSPN(im, 0.1, t);
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
    prlLaplaceSPN();
    output1.convertTo(output2, CV_8UC1);
    showImage(im, output2);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 1);
        writeToCSVFile("csv/laplacespn.csv", execTimeData);
    }
    return 0;
}