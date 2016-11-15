#pragma once
#include <opencv2/core.hpp>
#include <vector>

#define DEFAULT_RAYS	8
#define DEFAULT_WIDTH	500
#define DEFAULT_HEIGHT	500
#define CANVAS_SIZE		500

#define IN_RANGE(val, _min, _max) ((_min) <= (val)) &&  ((val) <= (_max))

class radial_machine
{
public:
	radial_machine(unsigned rays = DEFAULT_RAYS);
	cv::Mat DrawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar& color, int thickness = 1, int lock_sector = false, int lineType = cv::LINE_8);
	cv::Mat GetRadial();
	cv::Mat DrawRays(cv::Mat img, cv::Scalar color);
	void LockSector(cv::Point pt);
	void SetRays(unsigned rays);
	unsigned GetRays();
	void Resize(cv::Size new_size);
protected:
	cv::Point WindowToLogic(cv::Point p);
	cv::Rect GetROIrect();
protected:
	cv::Mat _img_pattern;
	cv::Mat _img_radial;
	unsigned _locked_sector;
	unsigned _rays;
	cv::Size _viewport;
};

extern std::vector<cv::Scalar> colors;