#ifndef HELPER_H
#define HELPER_H

#include <fstream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <omp.h>

void showImage(cv::Mat im, cv::Mat output1);
double calExecTime(void (*func)(), int iters=10);
void writeToCSVFile(std::string path, std::tuple<std::vector<std::string>, std::vector<double>> execTimeData);

#endif