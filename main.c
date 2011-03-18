/*
  Utility for creating a variety of geometric shapes of varying sizes
  and orientations and randomly placing them in tiff image files to
  be used for raw image processing by VisualSpreadsheet.
*/

#include <stdlib.h>
#include <time.h>
#define CV_NO_BACKWARD_COMPATIBILITY
#include <cv.h>
#include <highgui.h>

#define RANDOM_SEED 0x1337

void init_rng();
void draw_loop(IplImage *img);
int random_range(int low, int high);
void draw_circles(IplImage *img, CvScalar color, int min_cnt, int max_cnt);
void draw_ellipses(IplImage *img, CvScalar color, int min_cnt, int max_cnt);
void draw_squares(IplImage *img, CvScalar color, int min_cnt, int max_cnt);
void draw_rectangles(IplImage *img, CvScalar color, int min_cnt, int max_cnt);


int main(int argc, char **argv)
{
	IplImage *img;
	CvSize size = { 1024, 768 };
	
	//srandom(RANDOM_SEED);
	init_rng();
	
	img = cvCreateImage(size, IPL_DEPTH_8U, 3);
	
	cvNamedWindow("shapegen", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("shapegen", 100, 100);
	
	draw_loop(img);

	cvReleaseImage(&img);
	
	return 0;
}

void draw_loop(IplImage *img)
{
	int key;
	CvScalar circle_clr, ellipse_clr, square_clr, rect_clr, bkgd_clr;
	
	circle_clr.val[0] = random_range(90, 255);
	circle_clr.val[1] = random_range(90, 255);
	circle_clr.val[2] = random_range(90, 255);		

	ellipse_clr.val[0] = random_range(90, 255);
	ellipse_clr.val[1] = random_range(90, 255);
	ellipse_clr.val[2] = random_range(90, 255);		

	square_clr.val[0] = random_range(90, 255);
	square_clr.val[1] = random_range(90, 255);
	square_clr.val[2] = random_range(90, 255);		

	rect_clr.val[0] = random_range(90, 255);
	rect_clr.val[1] = random_range(90, 255);
	rect_clr.val[2] = random_range(90, 255);		
	
	bkgd_clr.val[0] = 60;
	bkgd_clr.val[1] = 60;
	bkgd_clr.val[2]  = 60;

	// start with a blank calibration image
	cvSet(img, bkgd_clr, 0);
	cvShowImage("shapegen", img);
	key = 0xff & cvWaitKey(0);
	
	while (key != 27) { // ESC
		cvSet(img, bkgd_clr, 0);
		draw_circles(img, circle_clr, 0, 5);
		draw_ellipses(img, ellipse_clr, 0, 5);
		draw_squares(img, square_clr, 0, 5);
		draw_rectangles(img, rect_clr, 0, 5);
		cvShowImage("shapegen", img);
		key = 0xff & cvWaitKey(0);				
	} 	
}

void draw_circles(IplImage *img, CvScalar color, int min_cnt, int max_cnt)
{
	int i, cnt;
	CvPoint center;
	int radius;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		center.x = random_range(50, 950);
		center.y = random_range(50, 700);
		radius = random_range(5, 50);
		cvCircle(img, center, radius, color, CV_FILLED, 8, 0);
	}	
}

void draw_ellipses(IplImage *img, CvScalar color, int min_cnt, int max_cnt)
{
	int i, cnt;
	CvPoint center;
	CvSize axes;
	double angle;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		center.x = random_range(50, 950);
		center.y = random_range(50, 700);
		axes.width = random_range(5, 50);
		axes.height = random_range(5, 50);
		angle = random_range(0, 180);
		cvEllipse(img, center, axes, angle, 0.0, 360.0, color, CV_FILLED, 8, 0);
	}	
}

void draw_squares(IplImage *img, CvScalar color, int min_cnt, int max_cnt)
{
	int i, cnt, side;
	CvPoint topLeft, bottomRight;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		topLeft.x = random_range(50, 950);
		topLeft.y = random_range(50, 700);
		side = random_range(5, 50);
		bottomRight.x = side + topLeft.x;
		bottomRight.y = side + topLeft.y;
		cvRectangle(img, topLeft, bottomRight, color, CV_FILLED, 8, 0);
	}					
}

void draw_rectangles(IplImage *img, CvScalar color, int min_cnt, int max_cnt)
{
	int i, cnt, height, width;
	CvPoint topLeft, bottomRight;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		topLeft.x = random_range(50, 950);
		topLeft.y = random_range(50, 700);
		height = random_range(5, 50);
		width = random_range(5, 50);
		bottomRight.x = width + topLeft.x;
		bottomRight.y = height + topLeft.y;
		cvRectangle(img, topLeft, bottomRight, color, CV_FILLED, 8, 0);
	}					
}

void init_rng()
{
	time_t tt;
	time(&tt);
	srandom((unsigned int)tt);
}

int random_range(int low, int high)
{
	double r = random() / (double) RAND_MAX;
	return low + ((high - low) * r);	
}
