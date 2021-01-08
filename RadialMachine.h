#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "params.h"

#define DEFAULT_RAYS	6
#define CANVAS_SIZE		WND_SIZE

#define IN_RANGE(val, _min, _max) ((_min) <= (val)) &&  ((val) <= (_max))

class radial_machine
{
public:
	radial_machine(unsigned rays = DEFAULT_RAYS);
	cv::Mat DrawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar& color, int thickness = 1, int lock_sector = false, int lineType = cv::LineTypes::LINE_8);
	cv::Mat GetRadial();
	cv::Mat DrawRays(cv::Mat img, cv::Scalar color);
	void SetRays(unsigned rays);
	unsigned GetRays();
protected:
	cv::Mat _img_pattern;
	cv::Mat _img_radial;
	unsigned _locked_sector; // number of sector where drowing allowed
	unsigned _rays; //number of rays
};

extern std::vector<cv::Scalar> colors;