#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, gray;

void srlRGB2GRAY() {
    gray = Mat::zeros(Size(im.cols, im.rows), CV_8UC1);
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            Vec3b color = im.at<Vec3b>(i,j);
            gray.at<uchar>(i,j) = (color[0]+color[1]+color[2])/3;
        }
    }
}

void prlRGB2GRAY() {
    gray = Mat::zeros(Size(im.cols, im.rows), CV_8UC1);
    #pragma omp parallel for collapse(2) shared(im, gray)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            Vec3b color = im.at<Vec3b>(i,j);
            gray.at<uchar>(i,j) = (color[0]+color[1]+color[2])/3;
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
            Mat im = imread(path);
            double execTime, start, end;
            execTime = 0;
            for (int i = 0; i < iters; ++i) {
                start = omp_get_wtime();
                RGB2GRAY(im, t);
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
    im = imread(path, IMREAD_COLOR);
    prlRGB2GRAY();
    showImage(im, gray);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 5);
        writeToCSVFile("csv/rgb2gray.csv", execTimeData);
    }
    return 0;
}
