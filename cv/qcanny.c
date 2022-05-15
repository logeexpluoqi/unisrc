/*
 * @Author: luoqi 
 * @Date: 2022-04-23 17:11:28 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-30 23:29:42
 */

#include "qcanny.h"

static float max_arr(float *data, int len);
static float qsqrt(float x);
static int atan_dir(float x, float y);

int qcanny(int **img, float th_low, float th_high, int compensation)
{
	int x, y;
	int grad_x, grad_y;
	int amp_dir[QCANNY_IMG_XSIZE][QCANNY_IMG_YSIZE] = {0};
	float img_amp[QCANNY_IMG_XSIZE][QCANNY_IMG_YSIZE] = {0};
	float bi_threshold[9] = {0};

	for(y = 0; y < QCANNY_IMG_YSIZE - 1; y++){
		for(x = 0; x < QCANNY_IMG_XSIZE - 1; x++){
			grad_x =  img[y][x] + img[y+1][x] - img[y][x+1] - img[y+1][x+1];
			grad_y = -img[y][x] + img[y+1][x] - img[y][x+1] + img[y+1][x+1];
			img_amp[y][x] = qsqrt(grad_x * grad_x + grad_y * grad_y);
			amp_dir[y][x] = atan_dir(grad_y, grad_x);
		}
	}

	//===============非极大值抑制================
	for(y = 1; y < QCANNY_IMG_YSIZE - 1; y++){
		for(x = 0; x < QCANNY_IMG_XSIZE - 1; x++){
			switch(amp_dir[y][x]){
				case 0: // horizontal
					if(img_amp[y][x] > (img_amp[y][x+1] - compensation) && img_amp[y][x] > (img_amp[y][x-1] - compensation)){
						img_amp[y][x] = img_amp[y][x];
					}else{
						img_amp[y][x] = 0;
					}
					break;
				case 1: // upper right and lower left
					if(img_amp[y][x] > (img_amp[y-1][x+1] - compensation) && img_amp[y][x] > (img_amp[y+1][x-1] - compensation)){
						img_amp[y][x] = img_amp[y][x];
					}else{
						img_amp[y][x] = 0;
					}
					break;
				case 2: // vertical
					if( img_amp[y][x] > (img_amp[y-1][x] - compensation) && img_amp[y][x] > (img_amp[y+1][x] - compensation)){
						img_amp[y][x] = img_amp[y][x];
					}else{
						img_amp[y][x] = 0;
					}
					break;
				case 3: // upper left and lower right
					if( img_amp[y][x] > (img_amp[y-1][x-1] - compensation) && img_amp[y][x] > (img_amp[y+1][x+1] - compensation)){
						img_amp[y][x] = img_amp[y][x];
					}else{
						img_amp[y][x] = 0;
					}
					break;
				default:
					break;
			}
		}
	}
	
	/*********双阈值检测**********/
	for(y = 1; y < QCANNY_IMG_YSIZE - 2; y++){
		for(x = 1; x < QCANNY_IMG_XSIZE - 1; x++){
			if( img_amp[y][x] < th_low ){
				img[y][x] = 0;
			}else if( img_amp[y][x] > th_high ){
				img[y][x] = 255; 
			}else {
				bi_threshold[0] = img_amp[y+1][x-1]; bi_threshold[1] = img_amp[y+1][x]; bi_threshold[2] = img_amp[y+1][x+1];
				bi_threshold[3] = img_amp[y][x-1];   bi_threshold[4] = img_amp[y][x];   bi_threshold[5] = img_amp[y][x+1];
				bi_threshold[6] = img_amp[y-1][x-1]; bi_threshold[7] = img_amp[y-1][x]; bi_threshold[8] = img_amp[y-1][x+1];
				
				if(max_arr(bi_threshold, 9) > th_high){
					img[y][x] = 255;
					img_amp[y][x] = 1;
				}else{
					img[y][x] = 0;
					img_amp[y][x] = 0;
				}
			}
		}
	}
	return 0;
} 


float max_arr(float *data, int len)
{
	int i;
	float max = data[0];
	if(len == 1){
		return data[0];
	}

	for(i = 1; i < len; i++){
		if(max < data[i]){
			max = data[i];
		}else{
			continue;
		}
	}
	return max;
}

float qsqrt(float x)
{
	float xhalf = 0.5f * x;
	int i = *(int*)&x;         
	i  = 0x5f3504f3 - ( i >> 1 );
	x = *(float*)&i;
	x = x*(1.5f-(xhalf * x * x));

	return 1/x;
}

/********************************************
*  直接计算几个分界点的值,只需判断 y/x 的值所
* 对应的角度在哪个范围即可，不用实时求atan
*--------------------------------------------
*    角度范围        |         弧度范围
*--------------------------------------------
* 0     ~ 22.5  ------> 0         ~ 0.3926990
* 22.5  ~ 67.5  ------> 0.3926990 ~ 1.1780972
* 67.5  ~ 112.5 ------> 1.1780972 ~ 1.9634954
* 112.5 ~ 157.5 ------> 1.9634954 ~ 2.7488935
* 157.5 ~ 180   ------> 2.7488935 ~ 3.1415926
*--------------------------------------------
*         y/x值对应弧度
*  0          ----  0.41421356  水平方向
*  0.41421347 ----  2.41421356  右上、左下
*  2.41421326 ---- -2.41421356  竖直方向
* -2.41421362 ---- -0.41421356  左上、右下
* -0.41421365 ----  0           水平方向
********************************************/
int atan_dir(float x, float y)
{
	float atan = y/x;
	if( atan > -0.41421365 && atan < 0.41421356){
		return 0; //水平方向
	}else if( atan >= 0.41421356 && atan < 2.41421356){
		return 1; //右上、左下
	}else if( atan <= -0.41421356 && atan > -2.41421362){
		return 3; //左上、右下
	}else {
		return 2; //竖直方向
	}
}
