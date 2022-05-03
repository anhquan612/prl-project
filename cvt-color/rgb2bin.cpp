#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat im, bin;
int mythreshold;

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
                RGB2BIN(im, 128, t);
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
    cout << "Threshold: ";
    cin >> mythreshold;
    im = imread(path);
    bin = RGB2BIN(im, mythreshold);
    showImage(im, bin);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 5);
        writeToCSVFile("csv/rgb2bin.csv", execTimeData);
    }
    return 0;
}