/*
  Utility for creating a variety of geometric shapes of varying sizes
  and orientations and randomly placing them in tiff image files to
  be used for raw image processing by VisualSpreadsheet.
*/

#ifdef WIN32
#include <Windows.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define CV_NO_BACKWARD_COMPATIBILITY
#include <opencv\cv.h>
#include <opencv\highgui.h>

#define RANDOM_SEED 0x1337
#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 384

#define BACKGROUND_COLOR 180

#define ESCAPE_KEY 27

struct shape_param {
	int min_count;
	int max_count;
	int min_color;
	int max_color;
	int fill;
};

void init_rng();
int generate_cal_image(const char *dir, int num_cal_images);
void draw_loop(const char *dir, int start_image_num, int num_images);
void draw_shapes(IplImage *img);
int random_range(int low, int high);
void draw_circles(IplImage *img, int min_cnt, int max_cnt, int with_centers);
void draw_ellipses(IplImage *img, int min_cnt, int max_cnt);
void draw_squares(IplImage *img, CvScalar color, int min_cnt, int max_cnt);
void draw_rectangles(IplImage *img, CvScalar color, int min_cnt, int max_cnt);

void save_image(const char *dir, IplImage *img, int index);

const char project_dir[] = "C:\\Users\\scott\\My Documents\\FluidData\\ShapeGenBW\\Circle_Ellipse_1\\";

int main(int argc, char **argv)
{
	int num_cal_images = 32;
	int num_images = 100;

	init_rng();
		
	cvNamedWindow("shapegen", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("shapegen", 10, 10);

	if (generate_cal_image(project_dir, num_cal_images)) {
		draw_loop(project_dir, num_cal_images, num_images);
		//draw_loop(NULL, num_cal_images, 20);
	}

	return 0;
}

void save_image(const char *dir, IplImage *img, int index)
{
	char buff[MAX_PATH];

	if (!dir || !*dir)
		return;

	if (index < 0)
		sprintf_s(buff, sizeof(buff), "%s\\cal_image_000001.tif", dir);
	else
		sprintf_s(buff, sizeof(buff), "%s\\rawfile_%06d.tif", dir, index);

	cvSaveImage(buff, img, 0);
}

/*
  Generate a calibration image based on the average of some
  accumulated images.
*/
int generate_cal_image (const char *dir, int num_cal_images)
{
	int key, i;
	CvScalar bkgd_color, one;
	IplImage *img, *cal_img, *sum_img, *mask;
	CvSize size = { IMAGE_WIDTH, IMAGE_HEIGHT };

	img = cvCreateImage(size, IPL_DEPTH_8U, 1);
	if (!img)
		return 0;
	
	cal_img = cvCreateImage(size, IPL_DEPTH_8U, 1);
	if (!cal_img) {
		cvReleaseImage(&img);
		return 0;
	}

	mask = cvCreateImage(size, IPL_DEPTH_8U, 1);
	if (!mask) {
		cvReleaseImage(&img);
		cvReleaseImage(&cal_img);
		return 0;
	}

	sum_img = cvCreateImage(size, IPL_DEPTH_32F, 1);
	if (!sum_img) {
		cvReleaseImage(&img);
		cvReleaseImage(&cal_img);
		cvReleaseImage(&mask);
		return 0;
	}

	bkgd_color.val[0] = BACKGROUND_COLOR;
	cvSet(img, bkgd_color, NULL);
	cvSet(cal_img, bkgd_color, NULL);
	
	one.val[0] = 1.0;
	cvSet(mask, one, NULL);

	cvZero(sum_img);
	
	for (i = 0; i < num_cal_images; i++) {		
		draw_shapes(img);
		save_image(dir, img, i);

		cvAcc(img, sum_img, mask);
		cvZero(cal_img);

		cvConvertScale(sum_img, cal_img, 1.0 / (i + 1), 0);
		cvShowImage("shapegen", cal_img);

		if (dir)
			key = 0xff & cvWaitKey(500);
		else
			key = 0xff & cvWaitKey(0);

		if (key == ESCAPE_KEY)
			break;
	}

	if (key != ESCAPE_KEY)
		save_image(dir, cal_img, -1);

	cvReleaseImage(&img);
	cvReleaseImage(&cal_img);
	cvReleaseImage(&sum_img);
	cvReleaseImage(&mask);

	return (key != ESCAPE_KEY);
}

void draw_loop(const char *dir, int start_image_num, int num_images)
{
	int key, i;
	IplImage *img;
	CvSize size = { IMAGE_WIDTH, IMAGE_HEIGHT };

	img = cvCreateImage(size, IPL_DEPTH_8U, 1); //3);
	if (!img)
		return;

	for (i = start_image_num; i < start_image_num + num_images; i++) {
		draw_shapes(img);
		cvShowImage("shapegen", img);
		save_image(dir, img, i);

 		if (dir)
			key = 0xff & cvWaitKey(500);
		else
			key = 0xff & cvWaitKey(0);

		if (key == ESCAPE_KEY)
			break;
	}

	cvReleaseImage(&img);
}

void draw_shapes(IplImage *img)
{
	CvScalar bkgd_clr = cvScalarAll(BACKGROUND_COLOR);

	cvSet(img, bkgd_clr, 0);
	draw_circles(img, 0, 3, 0);
	draw_circles(img, 0, 2, 1);
	draw_ellipses(img, 0, 3);
}

void draw_circles(IplImage *img, int min_cnt, int max_cnt, int with_centers)
{
	int i, val, cnt;
	CvPoint center;
	int radius;
	CvScalar color;
		
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		val = random_range(BACKGROUND_COLOR - 60, BACKGROUND_COLOR - 20);
		color = cvScalarAll(val);

		center.x = random_range(50, IMAGE_WIDTH - 50);
		center.y = random_range(50, IMAGE_HEIGHT - 50);
		radius = random_range(10, 40);
		cvCircle(img, center, radius, color, 1, CV_AA, 0);

		if (with_centers) {
			val = random_range(BACKGROUND_COLOR + 8, BACKGROUND_COLOR + 30);
			color = cvScalarAll(val);

			val = random_range(4, radius);
			cvCircle(img, center, val, color, CV_FILLED, CV_AA, 0);
		}
	}	
}

void draw_ellipses(IplImage *img, int min_cnt, int max_cnt)
{
	int i, val, cnt;
	CvPoint center;
	CvSize axes;
	double angle;
	CvScalar color;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		val = random_range(BACKGROUND_COLOR - 60, BACKGROUND_COLOR - 20);
		color = cvScalarAll(val);

		center.x = random_range(50, IMAGE_WIDTH - 50);
		center.y = random_range(50, IMAGE_HEIGHT - 50);
		axes.width = random_range(5, 40);
		axes.height = random_range(5, 40);
		angle = random_range(0, 180);
		cvEllipse(img, center, axes, angle, 0.0, 360.0, color, 1, CV_AA, 0);
	}	
}

void draw_squares(IplImage *img, CvScalar color, int min_cnt, int max_cnt)
{
	int i, cnt, side;
	CvPoint topLeft, bottomRight;
	
	cnt = random_range(min_cnt, max_cnt);

	for (i = 0; i < cnt; i++) {
		topLeft.x = random_range(50, IMAGE_WIDTH - 50);
		topLeft.y = random_range(50, IMAGE_HEIGHT - 50);
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
		topLeft.x = random_range(50, IMAGE_WIDTH - 50);
		topLeft.y = random_range(50, IMAGE_HEIGHT - 50);
		height = random_range(5, 50);
		width = random_range(5, 50);
		bottomRight.x = width + topLeft.x;
		bottomRight.y = height + topLeft.y;
		cvRectangle(img, topLeft, bottomRight, color, CV_FILLED, 8, 0);
	}					
}

void init_rng()
{
#ifdef WIN32
	srand(GetTickCount());
#else
	time_t tt;
	time(&tt);
	srandom((unsigned int)tt);
#endif
}

int random_range(int low, int high)
{
#ifdef WIN32
	double r = rand() / (double) RAND_MAX;
#else
	double r = random() / (double) RAND_MAX;
#endif

	return (int) (low + ((high - low) * r));	
}
