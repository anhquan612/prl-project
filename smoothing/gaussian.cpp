#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat gaussKernel(int ksize, double sigma) {
    vector<vector<double>> x, y;
    Mat kernel = Mat::zeros(Size(ksize, ksize), CV_64FC1);
    int size = ksize/2;
    double norm = 1/(2.0*M_PI*pow(sigma,2));
    for (int i = -size; i < size+1; ++i) {
        vector<double> el4x, el4y;
        for (int j = -size; j < size+1; ++j) {
            el4x.push_back((double) i);
            el4y.push_back((double) j);
        }
        x.push_back(el4x);
        y.push_back(el4y);
    }
    #pragma omp parallel for collapse(2) shared(kernel)
    for (int i = 0; i < ksize; ++i) {
        for (int j = 0; j < ksize; ++j) {
            kernel.at<double>(i,j) = norm * exp(-((pow(x.at(i).at(j),2)+pow(y.at(i).at(j),2))/(2.0*pow(sigma,2))));
        }
    }
    return kernel;
}

Mat gauss(Mat im, int ksize, double sigma) {
    Mat kernel = gaussKernel(ksize, sigma);
    return convolve2d(im, kernel);
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
                Gaussian(im, 5, 1.4, t);
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
    cout << "Path of image: ";
    cin >> path;
    im = imread(path, IMREAD_GRAYSCALE);
    res = Gaussian(im, 5, 1.4);
    res.convertTo(res2, CV_8UC1);
    showImage(res2);
    tuple<vector<string>, vector<double>> execTimeData = calExecTime2(500, 4000, 1);
    writeToCSVFile("csv/gaussian.csv", execTimeData);
    return 0;
}