CPPFLAGS=-fopenmp `pkg-config --cflags --libs opencv4`
REQUIRED=preprocessing.cpp helper.cpp

imconvolve: imconvolve.cpp $(REQUIRED)
	g++ imconvolve.cpp $(REQUIRED) -o imconvolve $(CPPFLAGS)

grayscale: ./cvt-color/grayscale.cpp $(REQUIRED)
	g++ ./cvt-color/grayscale.cpp $(REQUIRED) -o grayscale $(CPPFLAGS)

gray2bin: ./cvt-color/gray2bin.cpp $(REQUIRED)
	g++ ./cvt-color/gray2bin.cpp $(REQUIRED) -o gray2bin $(CPPFLAGS)

rgb2bin: ./cvt-color/rgb2bin.cpp $(REQUIRED)
	g++ ./cvt-color/rgb2bin.cpp $(REQUIRED) -o rgb2bin $(CPPFLAGS)

sobel: ./edge-detection/sobel.cpp $(REQUIRED)
	g++ ./edge-detection/sobel.cpp $(REQUIRED) -o sobel $(CPPFLAGS)

prewitt: ./edge-detection/prewitt.cpp $(REQUIRED)
	g++ ./edge-detection/prewitt.cpp $(REQUIRED) -o prewitt $(CPPFLAGS)

laplace: ./sharpening/laplace.cpp $(REQUIRED)
	g++ ./sharpening/laplace.cpp $(REQUIRED) -o laplace $(CPPFLAGS)

usm: ./sharpening/usm.cpp $(REQUIRED)
	g++ ./sharpening/usm.cpp $(REQUIRED) -o usm $(CPPFLAGS)

averaging: ./smoothing/averaging.cpp $(REQUIRED)
	g++ ./smoothing/averaging.cpp $(REQUIRED) -o averaging $(CPPFLAGS)

median: ./smoothing/median.cpp $(REQUIRED)
	g++ ./smoothing/median.cpp $(REQUIRED) -o median $(CPPFLAGS)

gaussian: ./smoothing/gaussian.cpp $(REQUIRED)
	g++ ./smoothing/gaussian.cpp $(REQUIRED) -o gaussian $(CPPFLAGS)