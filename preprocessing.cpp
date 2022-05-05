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
    cv::Mat output1, fkernel;
    if (im.type() != 0) {
        im.convertTo(im, CV_8UC1);
    }
    const int dx = (kernel.rows-1)/2;
    const int dy = (kernel.cols-1)/2;
    output1 = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    flip(kernel, fkernel, -1);
    double val;
    #pragma omp parallel for shared(im, fkernel, output1) private(val)
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
    cv::Mat Gx = convolve2d(im, Kx, numThreads);
    cv::Mat Gy = convolve2d(im, Ky, numThreads);
    cv::Mat gradient = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    cv::Mat theta = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    #pragma omp parallel for collapse(2) shared(gradient, theta, Gx, Gy)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    #pragma omp parallel for collapse(2) shared(gradient)
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
    cv::Mat Gx = convolve2d(im, Kx, numThreads);
    cv::Mat Gy = convolve2d(im, Ky, numThreads);
    cv::Mat gradient = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    cv::Mat theta = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    #pragma omp parallel for collapse(2) shared(gradient, theta, Gx, Gy)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            gradient.at<double>(i,j) = sqrt(pow(Gx.at<double>(i,j), 2) + pow(Gy.at<double>(i,j), 2));
            theta.at<double>(i,j) = atan2(Gy.at<double>(i,j), Gx.at<double>(i,j));
        }
    }
    double minValueGradient, maxValueGradient;
    minMaxLoc(gradient, &minValueGradient, &maxValueGradient);
    #pragma omp parallel for collapse(2) shared(gradient)
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            gradient.at<double>(i,j) = gradient.at<double>(i,j)/maxValueGradient * 255.0;
        }
    }
    return {gradient, theta};
}

cv::Mat nonmaximumSuppression(cv::Mat gradient, cv::Mat angle, int numThreads=4) {
    omp_set_num_threads(numThreads);
    cv::Mat output1 = cv::Mat::zeros(cv::Size(gradient.cols, gradient.rows), CV_64FC1);
    double a, fr, bh;
    #pragma omp parallel for collapse(2) shared(gradient, angle, output1) private(a, fr, bh)
    for (int i = 0; i < gradient.rows; ++i) {
        for (int j = 0; j < gradient.cols; ++j) {
            a = angle.at<double>(i,j);
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

bool neighborCheck(cv::Mat im, int i, int j, int higherThreshold, int numThreads=4) {
    omp_set_num_threads(numThreads);
    bool res = false;
    std::vector<int> r = {i-1, i, i+1};
    std::vector<int> c = {j-1, j, j+1};
    #pragma omp parallel for collapse(2) shared(r, c)
    for (int k = 0; k < 3; ++k) {
        for (int l = 0; l < 3; ++l) {
            if (res) {
                continue;
            }
            if (im.at<double>(r.at(k),c.at(l)) > higherThreshold) {
                res = true;
            }
        }
    }
    return res;
}

cv::Mat hysteresisThresholding(cv::Mat im, int lowerThreshold, int higherThreshold, int numThreads=4) {
    omp_set_num_threads(numThreads);
    double pixelVal;
    #pragma omp parallel for collapse(2) shared(im) private(pixelVal)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            pixelVal = im.at<double>(i,j);
            if (pixelVal > higherThreshold) {
                im.at<double>(i,j) = 255.0;
            }
            else if (pixelVal >= lowerThreshold && pixelVal <= higherThreshold) {
                if (neighborCheck(im, i, j, higherThreshold, numThreads) == true) {
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

cv::Mat Canny2(cv::Mat im, int lowerThreshold, int higherThreshold, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat kernel, blurred, gradient, theta, thinned, output1;
    std::tuple<cv::Mat, cv::Mat> resFromSobel;
    kernel = (cv::Mat_<double>(5,5) << 2.0/159,4.0/159,5.0/159,4.0/159,2.0/159,4.0/159,9.0/159,12.0/159,9.0/159,4.0/159,5.0/159,12.0/159,15.0/159,12.0/159,5.0/159,4.0/159,9.0/159,12.0/159,9.0/159,4.0/159,2.0/159,4.0/159,5.0/159,4.0/159,2.0/159);
    blurred = convolve2d(im, kernel, numThreads);
    resFromSobel = Sobel(blurred, numThreads);
    gradient = std::get<0>(resFromSobel);
    theta = std::get<1>(resFromSobel);
    #pragma omp parallel for collapse(2) shared(theta)
    for (int i = 0; i < theta.rows; ++i) {
        for (int j = 0; j < theta.cols; ++j) {
            theta.at<double>(i,j) = theta.at<double>(i,j)*180.0/M_PI;
            if (theta.at<double>(i,j) < 0) {
                theta.at<double>(i,j) += 180.0;
            }
        }
    }
    thinned = nonmaximumSuppression(gradient, theta, numThreads);
    output1 = hysteresisThresholding(thinned, lowerThreshold, higherThreshold, numThreads);
    return output1;
}

cv::Mat LaplaceSPN(cv::Mat im, double weight, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat output1 = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    cv::Mat kernel = (cv::Mat_<double>(3,3) << 0,1,0,1,-4,1,0,1,0);
    cv::Mat convolved = convolve2d(im, kernel, numThreads);
    #pragma omp parallel for collapse(2) shared(output1, im, convolved)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            output1.at<double>(i,j) = im.at<uchar>(i,j) - weight*convolved.at<double>(i,j);
        }
    }
    return output1;
}

cv::Mat USM(cv::Mat im, double sigma, double amount, int radius, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat blurred, mask, output1;
    output1 = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_64FC1);
    blurred = Gaussian(im, radius, sigma, numThreads);
    #pragma omp parallel for collapse(2) shared(output1, im, blurred)
    for (int i = 0; i < im.rows; ++i) {
        for (int j = 0; j < im.cols; ++j) {
            output1.at<double>(i,j) = (amount+1)*im.at<uchar>(i,j) - amount*blurred.at<double>(i,j);
        }
    }
    return output1;
}

cv::Mat blurByAvg(cv::Mat im, int ksize, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat output1;
    cv::Mat kernel = cv::Mat::ones(cv::Size(ksize, ksize), CV_64FC1);
    int ksize2 = ksize*ksize;
    #pragma omp parallel for collapse(2) shared(kernel)
    for (int i = 0; i < ksize; ++i) {
        for (int j = 0; j < ksize; ++j) {
            kernel.at<double>(i,j) /= ksize2;
        }
    }
    output1 = convolve2d(im, kernel, numThreads);
    return output1;
}

cv::Mat medBlur(cv::Mat im, int ksize, int numThreads) {
    omp_set_num_threads(numThreads);
    cv::Mat output1;
    output1 = cv::Mat::zeros(cv::Size(im.cols, im.rows), CV_8UC1);
    std::vector<double> mask;
    int idx = ksize/2;
    #pragma omp parallel for collapse(2) shared(output1, im) private(mask)
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
            std::sort(mask.begin(), mask.end());
            output1.at<uchar>(i,j) = mask.at(mask.size()/2);
            mask.clear();
        }
    }
    return output1;
}

cv::Mat GaussianKernel(int ksize, double sigma, int numThreads=4) {
    std::vector<std::vector<double>> x, y;
    cv::Mat kernel = cv::Mat::zeros(cv::Size(ksize, ksize), CV_64FC1);
    int size = ksize/2;
    double norm = 1/(2.0*M_PI*pow(sigma,2));
    for (int i = -size; i < size+1; ++i) {
        std::vector<double> el4x, el4y;
        for (int j = -size; j < size+1; ++j) {
            el4x.push_back((double) i);
            el4y.push_back((double) j);
        }
        x.push_back(el4x);
        y.push_back(el4y);
    }
    omp_set_num_threads(numThreads);
    #pragma omp parallel for collapse(2) shared(kernel)
    for (int i = 0; i < ksize; ++i) {
        for (int j = 0; j < ksize; ++j) {
            kernel.at<double>(i,j) = norm * exp(-((pow(x.at(i).at(j),2)+pow(y.at(i).at(j),2))/(2.0*pow(sigma,2))));
        }
    }
    return kernel;
}

cv::Mat Gaussian(cv::Mat im, int ksize, double sigma, int numThreads) {
    cv::Mat kernel = GaussianKernel(ksize, sigma, numThreads);
    return convolve2d(im, kernel, numThreads);
}