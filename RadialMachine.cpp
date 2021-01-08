#include "RadialMachine.h"
#include <opencv2/imgproc.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

radial_machine::radial_machine(unsigned rays)
{
	SetRays(rays);
	_img_pattern = cv::Mat::zeros(cv::Size(CANVAS_SIZE, CANVAS_SIZE), CV_8UC3);
	_img_radial = cv::Mat::zeros(cv::Size(CANVAS_SIZE, CANVAS_SIZE), CV_8UC3);
	_locked_sector = 0;
}
cv::Mat radial_machine::DrawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar& color, int thickness, int lock_sector, int lineType)
{
	if(	!IN_RANGE(pt1.x, 0, _img_pattern.cols - 1) || !IN_RANGE(pt1.y, 0, _img_pattern.rows - 1) ||
		!IN_RANGE(pt2.x, 0, _img_pattern.cols - 1) || !IN_RANGE(pt2.y, 0, _img_pattern.rows - 1))
		return GetRadial(); // do not draw anything outside
	cv::Point center(_img_pattern.cols / 2, _img_pattern.rows / 2);
	double deg_per_sector = 360.0 / _rays;
	if(lock_sector)
	{
		unsigned radius = cv::min(_img_pattern.cols / 2, _img_pattern.rows / 2);
		cv::Mat sector_mask = cv::Mat::zeros(_img_pattern.size(), CV_8UC1); // sector-like mask
		cv::ellipse(sector_mask, center, cv::Size(radius, radius),
					_locked_sector * deg_per_sector, 0, deg_per_sector,
					cv::Scalar::all(255), cv::FILLED);
		cv::Mat temp = cv::Mat::zeros(_img_pattern.size(), CV_8UC3);
		cv::line(temp, pt1, pt2, color, thickness, lineType);
		bitwise_or(_img_pattern, temp, _img_pattern, sector_mask);
	}
	else
	{
		cv::line(_img_pattern, pt1, pt2, color, thickness, lineType);
	}
	_img_radial *= 0;
	cv::Mat sector = _img_pattern.clone();
	bitwise_or(_img_radial, sector, _img_radial);
	for(unsigned i = 1 ; i <= _rays; i++)
	{
		cv::Mat rot_mat = cv::getRotationMatrix2D(center, deg_per_sector * i, 1);
		bitwise_or(_img_radial, sector, _img_radial);
		warpAffine(_img_pattern, sector, rot_mat, sector.size(), cv::INTER_NEAREST);
	}
	return GetRadial();
}
cv::Mat radial_machine::GetRadial()
{
	return _img_radial.clone();
}
cv::Mat radial_machine::DrawRays(cv::Mat img, cv::Scalar color)
{
	cv::Mat res;
	img.copyTo(res);
	unsigned radius = cv::min(_img_pattern.cols / 2, _img_pattern.rows / 2);
	cv::Point center(_img_pattern.cols / 2, _img_pattern.rows / 2);
	double deg_per_sector = 360.0 / _rays;
	for(unsigned i = 0 ; i < _rays; i++)
	{
		cv::Point pt(radius * cos(i * deg_per_sector / 180 * M_PI) + center.x, radius * sin(i * deg_per_sector / 180 * M_PI) + center.y);
		cv::line(res, center, pt, color);
	}
	return res;
}
void radial_machine::SetRays(unsigned rays)
{
	if(rays == 0)
		rays = 1;
	_rays = rays;
}
unsigned radial_machine::GetRays()
{
	return _rays;
}

std::vector<cv::Scalar> colors ={
								cv::Scalar::all(255),
								cv::Scalar(0, 0, 255)
								};