#include "preprocessing.h"

cv::Mat RGB2GRAY(cv::Mat im, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat gray = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_8UC1);
    #pragma omp parallel for collapse(2) shared(im, gray)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            cv::Vec3b color = im.at<cv::Vec3b>(i,j);
            gray.at<uchar>(i,j) = (color[0]+color[1]+color[2])/3;
        }
    }
    return gray;
}

cv::Mat GRAY2BIN(cv::Mat gray, int mythreshold, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat bin = cv::Mat::zeros(cv::Size(gray.cols, gray.rows), CV_8UC1);
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
    return bin;
}

cv::Mat RGB2BIN(cv::Mat im, int mythreshold, int numThreads) {
    cv::Mat gray = RGB2GRAY(im, numThreads);
    cv::Mat bin = GRAY2BIN(gray, mythreshold, numThreads);
    return bin;
}

cv::Mat convolve2d(cv::Mat im, cv::Mat kernel, int numThreads) {
    omp_set_num_threads(numThreads);
    const int dx = (kernel.rows-1)/2;
    const int dy = (kernel.cols-1)/2;
    cv::Mat output1, fkernel;
    output1 = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    flip(kernel, fkernel, -1);
    double val;
    #pragma omp parallel for collapse(2) shared(im, fkernel, output1) private(val) schedule(static)
    for (int i = dx; i < im.rows-dx; ++i) {
        for (int j = dy; j < im.cols-dy; ++j) {
            val = 0;
            for (int k = -dx; k < dx+1; ++k) {
                for (int l = -dy; l < dy+1; ++l) {
                    val += im.at<uchar>(i+k,j+l)*fkernel.at<double>(dx+k, dy+l);
                }
            }
            output1.at<double>(i,j) = val;
        }
    }
    return output1;
}

std::tuple<cv::Mat, cv::Mat> Sobel(cv::Mat im, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat Kx = (cv::Mat_<double>(3,3) << -1,0,1,-2,0,2,-1,0,1);
    cv::Mat Ky = (cv::Mat_<double>(3,3) << -1,-2,-1,0,0,0,1,2,1);
    cv::Mat Gx = convolve2d(im, Kx);
    cv::Mat Gy = convolve2d(im, Ky);
    cv::Mat gradient = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    cv::Mat theta = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    #pragma omp parallel for collapse(2) shared(gradient, theta, Gx, Gy) schedule(static)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    #pragma omp parallel for collapse(2) shared(gradient) schedule(static)
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            gradient.at<double>(i,j) = gradient.at<double>(i,j)/maxValueGradient * 255.0;
        }
    }
    return {gradient, theta};
}

std::tuple<cv::Mat, cv::Mat> Prewitt(cv::Mat im, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat Kx = (cv::Mat_<double>(3,3) << -1,0,1,-1,0,1,-1,0,1);
    cv::Mat Ky = (cv::Mat_<double>(3,3) << -1,-1,-1,0,0,0,1,1,1);
    cv::Mat Gx = convolve2d(im, Kx);
    cv::Mat Gy = convolve2d(im, Ky);
    cv::Mat gradient = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    cv::Mat theta = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    #pragma omp parallel for collapse(2) shared(gradient, theta, Gx, Gy) schedule(static)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    #pragma omp parallel for collapse(2) shared(gradient) schedule(static)
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            gradient.at<double>(i,j) = gradient.at<double>(i,j)/maxValueGradient * 255.0;
        }
    }
    return {gradient, theta};
}