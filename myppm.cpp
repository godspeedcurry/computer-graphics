#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <algorithm>
using namespace std;
#define EPS 1e-9
const double PI = acos(-1.0);
/*
 * int PPMWriteImage(const char* filePath, const unsigned char* buffer, unsigned int width, unsigned int height)
 * Parameters:
 *     filePath: target image file path.
 *     buffer: image data buffer, RGB order, 8 bits per channel, 24 bits per pixel.
 *     width: image width.
 *     height: image height.
 * Return:
 *     1 (success), 0 (error)
 */
const int WIDTH = 800;
const int HEIGHT = 800;
const unsigned char blue[3]={0, 121, 215};
void fillColor(unsigned char *target){
	memcpy(target, blue, sizeof(blue));
}
inline int sq(int x){
	return x*x;
}
inline double sqd(double x){
	return x*x;
}
inline bool check(int i,int j,int ra,int rb,int x,int y,double theta){
	return sq(rb)*sqd((i-x)*cos(theta)+(j-y)*sin(theta))+sqd((i-x)*sin(theta)-(j-y)*cos(theta))*sq(ra)<=1.0*sq(ra*rb);
}
inline void scanConvertToEllipse(unsigned char target[HEIGHT][WIDTH][3],unsigned int ra,unsigned int rb,unsigned int x,unsigned int y,double theta){
	unsigned char (*p)[3] = &target[0][0];
	for(int i=0;i<HEIGHT;++i){
		for(int j=0;j<WIDTH;++j){
			if(check(i,j,ra,rb,x,y,theta)){
				p=&target[i][j];
				memcpy(p, blue, sizeof(blue));	
			}		
		}
	}
}
static int PPMWriteImage(const char* filePath, const unsigned char* buffer, unsigned int width, unsigned int height)
{
	FILE* fp;
	unsigned int total = width * height * 3;
	assert(filePath != NULL);
	assert(buffer != NULL);
	fp = fopen(filePath, "wb");
	if (fp == NULL)
	{
		fprintf(stderr, "Open target image file [%s] failed!\n", filePath);
		return 0;
	}
	/* Write file header */
	fprintf(fp, "P3 %u %u 255\n", width, height);
	/* Write file content */
	while (total-- > 0)
	{
		fprintf(fp, " %u", *buffer++);
	}
	fclose(fp);
	return 1;
}
void welcome(){
	printf("Position of Up Left is 0,0\n");
	printf("Size of the Background is 800*800\n");
	printf("Position of the center is 400*400\n");
	printf("Valid input: 128 64 400 400 30\n");
}
int main(){
	welcome();
	unsigned char myEllipse[HEIGHT][WIDTH][3];
	unsigned int ra,rb,x,y;
	double theta;
	printf("Please input ra,rb,x,y,rotate degree of your Ellipse:");
	scanf("%d%d%d%d%lf",&ra,&rb,&x,&y,&theta);
	if(ra>rb) swap(ra,rb);
	theta=theta*PI/180;
	scanConvertToEllipse(myEllipse,ra,rb,x,y,theta);
	PPMWriteImage("test.ppm", &myEllipse[0][0][0], WIDTH, HEIGHT);
	return 0;
}
