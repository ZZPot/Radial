#include "RadialMachine.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

#define WND_NAME_ORIG	"Original"
#define WND_NAME_BLUR	"Blur"
#define WND_SIZE		500

cv::Size wnd_size(WND_SIZE, WND_SIZE);

void mouseDrawLine(int event_type, int x, int y, int flags, void *userdata);

int main()
{
	cv::Mat img_pattern = cv::Mat::zeros(wnd_size, CV_8UC3);
	cv::imshow(WND_NAME_ORIG, img_pattern);
	//cv::imshow(WND_NAME_BLUR, img_pattern);
	radial_machine rmach(7);
	cv::setMouseCallback(WND_NAME_ORIG, mouseDrawLine, &rmach);
	cv::waitKey(0);
	return 0;
}

void mouseDrawLine(int event_type, int x, int y, int flags, void *userdata)
{
	static cv::Point prev_pos(x, y);
	switch(event_type)
	{
	case cv::EVENT_LBUTTONDOWN:
		prev_pos.x = x;
		prev_pos.y = y;
		break;
	case cv::EVENT_MOUSEMOVE:
		if(flags & cv::EVENT_FLAG_LBUTTON)
		{
			radial_machine* rmach = (radial_machine*)userdata;
			cv::imshow(WND_NAME_ORIG, rmach->DrawRays(
					rmach->DrawLine(
									prev_pos, cv::Point(x, y),
									colors[0], 2
									), 
					colors[1]
					)
				);
			//cv::Mat temp;
			//cv::blur(*img_pattern, temp, cv::Size(3, 3));
			//cv::imshow(WND_NAME_BLUR, temp);
			prev_pos.x = x;
			prev_pos.y = y;
		}
		break;
	}
}