#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat gray, bin;
int mythreshold;

void srlGRAY2BIN() {
    bin = Mat::zeros(Size(gray.cols, gray.rows), CV_8UC1);
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            if (gray.at<uchar>(i,j) > mythreshold) {
                bin.at<uchar>(i,j) = 255;
            }
            else {
                bin.at<uchar>(i,j) = 0;
            }
        }
    }
}

void prlGRAY2BIN() {
    bin = Mat::zeros(Size(gray.cols, gray.rows), CV_8UC1);
    #pragma omp parallel for collapse(2) shared(gray, bin)
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            if (gray.at<uchar>(i,j) > mythreshold) {
                bin.at<uchar>(i,j) = 255;
            }
            else {
                bin.at<uchar>(i,j) = 0;
            }
        }
    }
}

tuple<vector<string>, vector<double>> calExecTime2(int minSize, int maxSize, int iters=10) {
    vector<string> rowHeaders;
    vector<double> execTimes;
    for (int s = minSize; s <= maxSize; s*=2) {
        string sizestr = to_string(s);
        for (int t = 1; t < 5; t*=2) {
            string path = "../img/test" + sizestr + ".jpg";
            Mat gray = imread(path, IMREAD_GRAYSCALE);
            double execTime, start, end;
            execTime = 0;
            for (int i = 0; i < iters; ++i) {
                start = omp_get_wtime();
                GRAY2BIN(gray, 128, t);
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
    cout << "Threshold: ";
    cin >> mythreshold;
    gray = imread(path, IMREAD_GRAYSCALE);
    prlGRAY2BIN();
    showImage(bin);
    tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 20);
    writeToCSVFile("../csv/gray2bin.csv", execTimeData);
    return 0;
}
