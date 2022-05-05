#include "helper.h"

void showImage(cv::Mat im, cv::Mat output1) {
    cv::imshow("Original image", im);
    cv::imshow("Output1", output1);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

double calExecTime(void (*func)(), int iters) {
    double sum, start, end;
    int i;
    sum = 0;
    for (i = 0; i < iters; ++i) {
        start = omp_get_wtime();
        func();
        end = omp_get_wtime();
        sum += (end-start);
    }
    return sum/iters;
}

void writeToCSVFile(std::string path, std::tuple<std::vector<std::string>, std::vector<double>> execTimeData) {
    std::vector<std::string> rowHeaders = std::get<0>(execTimeData);
    std::vector<double> execTimes = std::get<1>(execTimeData);
    std::fstream fout;
    fout.open(path, std::ios::out);
    int maxThreads = 4;
    // write header
    fout << " , ";
    for (int t = 1; t <= maxThreads; t*=2) {
        std::string header = "threads=" + std::__cxx11::to_string(t);
        if (t != maxThreads) {
            fout << header << ", ";
        }
        else {
            fout << header << "\n";
        }
    }
    for (unsigned i = 0; i < execTimes.size(); ++i) {
        if (i % (maxThreads-1) == 0) {
            fout << rowHeaders.at(i/(maxThreads-1)) << ", ";
        }
        if ((i+1) % (maxThreads-1) != 0) {
            fout << execTimes.at(i)*1000 << ", ";
        }
        else {
            fout << execTimes.at(i)*1000 << "\n";
        }
    }
}