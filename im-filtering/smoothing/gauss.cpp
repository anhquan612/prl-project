#include <opencv2/opencv.hpp>
#include <iostream>

static void meshgrid(const cv::Mat &xgv, const cv::Mat &ygv,
              cv::Mat1i &X, cv::Mat1i &Y)
{
  cv::repeat(xgv.reshape(1,1), ygv.total(), 1, X);
  cv::repeat(ygv.reshape(1,1).t(), 1, xgv.total(), Y);
}

// helper function (maybe that goes somehow easier)
static void meshgridTest(const cv::Range &xgv, const cv::Range &ygv,
                         cv::Mat1i &X, cv::Mat1i &Y)
{
  std::vector<int> t_x, t_y;
  for (int i = xgv.start; i <= xgv.end; i++) t_x.push_back(i);
  for (int i = ygv.start; i <= ygv.end; i++) t_y.push_back(i);
  meshgrid(cv::Mat(t_x), cv::Mat(t_y), X, Y);
}

// Small test-main    
int main(int argc, char** argv) {
    cv::Mat1i X, Y;
    meshgridTest(cv::Range(1,3), cv::Range(10, 14), X, Y); 
    std::cerr << X << std::endl;
    std::cerr << Y << std::endl;
    return 0;
}