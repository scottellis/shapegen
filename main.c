/*
  Utility for creating a variety of geometric shapes of varying sizes
  and orientations and randomly placing them in tiff image files to
  be used for raw image processing by VisualSpreadsheet.
*/

#include <stdlib.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

#define RANDOM_SEED 0x1337

void init_rng();
void drawStuff(IplImage *img);
int random_range(int low, int high);
void drawCircles(IplImage *img, int num);


int main(int argc, char **argv)
{
	int i;
	IplImage *img;
	CvSize size = { 1024, 768 };
	
	//srandom(RANDOM_SEED);
	init_rng();
	
	img = cvCreateImage(size, IPL_DEPTH_8U, 1);
	
	cvNamedWindow("shapegen", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("shapegen", 100, 100);
	
	for (i = 0; i < 10; i++) {
		drawStuff(img);

		cvShowImage("shapegen", img);
	
		cvWaitKey(0);
	}
	
	cvReleaseImage(&img);
	
	return 0;
}

void init_rng()
{
	time_t tt;
	
	time(&tt);
	
	srandom((unsigned int)tt);
}

void drawStuff(IplImage *img)
{
	CvScalar s;
	
	s.val[0] = 0;

	cvSet(img, s, 0);
		
	drawCircles(img, 20);
}

void drawCircles(IplImage *img, int num)
{
	int i;
	CvPoint center;
	CvScalar color;
	int radius;
	
	for (i = 0; i < num; i++) {
		center.x = random_range(50, 950);
		center.y = random_range(50, 700);
		color.val[0] = random_range(90, 255);
		radius = random_range(5, 50);
		cvCircle(img, center, radius, color, 1, 8, 0);
	}	
}

int random_range(int low, int high)
{
	double r = random() / (double) RAND_MAX;
	
	return low + ((high - low) * r);	
}
