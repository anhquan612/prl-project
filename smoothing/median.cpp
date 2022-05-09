#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, blurred;
int ksize = 3;

void medianSMT() {
    blurred = Mat::zeros(Size(im.cols, im.rows), CV_8UC1);
    vector<double> mask;
    int idx = ksize/2;
    #pragma omp parallel for collapse(2) shared(blurred, im) private(mask)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            for (int k = 0; k < ksize; ++k) {
                if (i+k-idx < 0 || i+k-idx > im.rows-1) {
                    for (int l = 0; l < ksize; ++l) {
                        mask.push_back(0);
                    }
                }
                else {
                    if (j+k-idx < 0 || j+idx > im.cols-1) {
                        mask.push_back(0);
                    }
                    else {
                        for (int l = 0; l < ksize; ++l) {
                            mask.push_back(im.at<uchar>(i+k-idx,j+l-idx));
                        }
                    }
                }
            }
            sort(mask.begin(), mask.end());
            blurred.at<uchar>(i,j) = mask.at(mask.size()/2);
            mask.clear();
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
                medBlur(im, 3, t);
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
    medianSMT();
    showImage(im, blurred);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(480, 7680, 1);
        writeToCSVFile("csv/medianlur.csv", execTimeData);
    }
    return 0;
}