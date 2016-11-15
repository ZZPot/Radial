#include "RadialMachine.h"
#include <opencv2/imgproc.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

radial_machine::radial_machine(unsigned rays):
_viewport(DEFAULT_WIDTH, DEFAULT_HEIGHT)
{
	SetRays(rays);
	_img_pattern = cv::Mat::zeros(cv::Size(CANVAS_SIZE, CANVAS_SIZE), CV_8UC3);
	_img_radial = cv::Mat::zeros(cv::Size(CANVAS_SIZE, CANVAS_SIZE), CV_8UC3);
	_locked_sector = 0;
}
cv::Mat radial_machine::DrawLine(cv::Point pt1, cv::Point pt2, const cv::Scalar& color, int thickness, int lock_sector, int lineType)
{
	pt1 = WindowToLogic(pt1);
	pt2 = WindowToLogic(pt2);

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
					cv::Scalar::all(255), CV_FILLED);
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
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, deg_per_sector, 1);
	for(unsigned i = 0 ; i < _rays; i++)
	{
		bitwise_or(_img_radial, sector, _img_radial);
		warpAffine(sector, sector, rot_mat, sector.size(), cv::INTER_NEAREST);
	}
	return GetRadial();
}
cv::Mat radial_machine::GetRadial()
{
	return _img_radial(GetROIrect()).clone();
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
void radial_machine::LockSector(cv::Point pt)
{
	pt = WindowToLogic(pt);
	cv::Point center(_img_pattern.cols / 2, _img_pattern.rows / 2);
	double deg_per_sector = 360.0 / _rays;
	double dy = pt.y - center.y;
	double dx = pt.x - center.x;
	double angle;
	if(dx == 0)
		angle = dy > 0 ? 90 : 270;
	else
		angle = 180/M_PI * atan(dy/dx);
	if(dx < 0)
		angle += 180;
	if(angle < 0)
		angle += 360;
	_locked_sector = angle / deg_per_sector;
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
void radial_machine::Resize(cv::Size new_size)
{
	_viewport = new_size;
}
cv::Point radial_machine::WindowToLogic(cv::Point p)
{
	cv::Rect rect = GetROIrect();
	p.x += rect.x;
	p.y += rect.y;
	return p;
}
cv::Rect radial_machine::GetROIrect()
{
	cv::Rect res;
	res.x = (_img_pattern.cols - _viewport.width) / 2;
	res.y = (_img_pattern.rows - _viewport.height) / 2;
	res.width = _viewport.width;
	res.height = _viewport.height;
	return res;
}
std::vector<cv::Scalar> colors ={
								cv::Scalar::all(255),
								cv::Scalar(0, 0, 255)
								};