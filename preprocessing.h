#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <opencv2/opencv.hpp>
#include <omp.h>
#include <tuple>
#include <vector>

// convert rgb image to grayscale image
cv::Mat RGB2GRAY(cv::Mat im, int numThreads=4);

// convert grayscale image to binary image
cv::Mat GRAY2BIN(cv::Mat im, int mythreshold=128, int numThreads=4);

// convert rgb image to binary image
cv::Mat RGB2BIN(cv::Mat im, int mythreshold=128, int numThreads=4);

// convolution operator
cv::Mat convolve2d(cv::Mat im, cv::Mat kernel, int numThreads=4);

// Sobel operator
std::tuple<cv::Mat, cv::Mat> Sobel(cv::Mat im, int numThreads=4);

// Prewitt operator
std::tuple<cv::Mat, cv::Mat> Prewitt(cv::Mat im, int numThreads=4);

// Canny edge detector
cv::Mat Canny(cv::Mat im, int lowerThreshold, int higherThreshold, int numThreads=4);

#endif