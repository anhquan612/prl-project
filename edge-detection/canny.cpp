#include <iostream>
#include "../helper.h"
#include "../preprocessing.h"

using namespace std;
using namespace cv;

Mat nonmaximumSuppression(Mat gradient, Mat angle) {
    Mat output1 = Mat::zeros(Size(gradient.cols, gradient.rows), CV_64FC1);
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            double a = angle.at<double>(i,j);
            double fr, bh;
            fr = 255;
            bh = 255;
            if ((0 <= a && a < 22.5) || (157.5 <= a && a <= 180)) {
                fr = gradient.at<double>(i,j-1);
                bh = gradient.at<double>(i,j+1);
            }
            else if (22.5 <= a && a < 67.5) {
                fr = gradient.at<double>(i-1,j+1);
                bh = gradient.at<double>(i+1,j-1);
            }
            else if (67.5 <= a && a < 112.5) {
                fr = gradient.at<double>(i-1,j);
                bh = gradient.at<double>(i+1,j);
            }
            else if (112.5 <= a && a < 157.5) {
                fr = gradient.at<double>(i-1,j-1);
                bh = gradient.at<double>(i+1,j+1);
            }
            if (gradient.at<double>(i,j) >= fr && gradient.at<double>(i,j) >= bh) {
                output1.at<double>(i,j) = gradient.at<double>(i,j);
            }
            else {
                output1.at<double>(i,j) = 0;
            }
        }
    }
    return output1;
}

bool neighborCheck(Mat im, int i, int j, int higherThreshold) {
    vector<int> r = {i-1, i, i+1};
    vector<int> c = {j-1, j, j+1};
    for (int k = 0; k < 3; ++k) {
        for (int l = 0; l < 3; ++l) {
            if (im.at<double>(r.at(k),c.at(l)) > higherThreshold) {
                return true;
            }
        }
    }
    return false;
}

Mat hysteresisThresholding(Mat im, int lowerThreshold, int higherThreshold) {
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            double pixelVal = im.at<double>(i,j);
            if (pixelVal > higherThreshold) {
                im.at<double>(i,j) = 255.0;
            }
            else if (pixelVal >= lowerThreshold && pixelVal <= higherThreshold) {
                if (neighborCheck(im, i, j, higherThreshold) == true) {
                    im.at<double>(i,j) = 255.0;
                }
                else {
                    im.at<double>(i,j) = 0.0;
                }
            }
            else {
                im.at<double>(i,j) = 0.0;
            }
        }
    }
    return im;
}

Mat canny(Mat im, int low, int high) {
    Mat blurred, gradient, theta, thinned, output1;
    tuple<Mat, Mat> resFromSobel;
    Mat kernel = (Mat_<double>(5,5) << 2.0/159,4.0/159,5.0/159,4.0/159,2.0/159,4.0/159,9.0/159,12.0/159,9.0/159,4.0/159,5.0/159,12.0/159,15.0/159,12.0/159,5.0/159,4.0/159,9.0/159,12.0/159,9.0/159,4.0/159,2.0/159,4.0/159,5.0/159,4.0/159,2.0/159);
    blurred = convolve2d(im, kernel);
    resFromSobel = Sobel(blurred);
    gradient = get<0>(resFromSobel);
    theta = get<1>(resFromSobel);
    for (int i = 0; i < theta.rows; ++i) {
        for (int j = 0; j < theta.cols; ++j) {
            theta.at<double>(i,j) = theta.at<double>(i,j)*180.0/M_PI;
            if (theta.at<double>(i,j) < 0) {
                theta.at<double>(i,j) += 180.0;
            }
        }
    }
    thinned = nonmaximumSuppression(gradient, theta);
    output1 = hysteresisThresholding(thinned, low, high);
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
                Canny2(im, 10, 30, t);
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
    Mat im, res, res2;
    cout << "Path of image: ";
    cin >> path;
    im = imread(path, IMREAD_GRAYSCALE);
    res = Canny2(im, 10, 30);
    res.convertTo(res2, CV_8UC1);
    showImage(im, res);
    cout << "Calculate execution time? (0/1) ";
    cin >> ev;
    if (ev == 1) {
        tuple<vector<string>, vector<double>> execTimeData = calExecTime2(480, 7680, 1);
        writeToCSVFile("csv/canny.csv", execTimeData);
    }
    return 0;
}