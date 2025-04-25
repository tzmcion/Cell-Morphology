/*
*   This file collects include libraries used by many hpp files.
**/

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/logger.hpp>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <random> // For optimization, for std::mt19937 and std::random_device, 

#define ROUND_2_INT(f) ((int)(f >= 0.0 ? (f + 0.5) : (f - 0.5)))
#define MIN_RADIUS_FOR_HISTOGRAM 2.0
#define MAX_RADIUS_FOR_HISTOGRAM 82.0
#define PIXEL_TO_RADIUS 1
#define DISTRIBUTION_RESOLUTION 160