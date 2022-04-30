/*
 * @Author: luoqi 
 * @Date: 2022-04-23 17:11:28 
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-04-23 18:26:54
 */

#include "qcanny.h"

//=============反正切函数============LQ==
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
uint8 Atan2(float y, float x)
{
	float tanNum;
	uint8 alpha; //返回角度
	tanNum = y/x;
		if( tanNum> -0.41421365 && tanNum< 0.41421356 )
		{
			alpha = 0; //水平方向
		}
		else if( tanNum>= 0.41421356 && tanNum< 2.41421356)
		{
			alpha = 1; //右上、左下
		}
		else if( tanNum<= -0.41421356 && tanNum> -2.41421362)
		{
			alpha = 3; //左上、右下
		}
		else 
		{
			alpha = 2; //竖直方向
		}
 
    return alpha; //方向
} //end of function Atan2

float Fsqrt(float x) 
{
  float xhalf = 0.5f * x;
  int i = *(int*)&x;         // evil floating point bit level hacking
  //i = 0x5f3759df - (i >> 1);  // what the fuck?
  i  = 0X5F3504F3 - ( i >> 1 ); //精度更高
  x = *(float*)&i;
  x = x*(1.5f-(xhalf*x*x));
  return 1/x;
} //end of function Fsqrt

void CannyEdgeTest( uint8 org[70][120], uint8 lowThr )
{
	uint8 Sector[70][120] = {0}; //像素梯度所在扇区
	float Temp[9] = {0}; //双阈值检测临时数组
	//float TemMax = 0; //临时最大值                                                                    
	//float theta = 0; //梯度方向
	int Xg = 0; //X方向梯度
	int Yg = 0; //Y方向梯度
	
	uint8 x, y;
	uint8 ratio = 2; //高低阈值检测比例
	uint8 highThr =  50; // ratio*lowThr; //高阈值
	uint8 loss = 2; //非极大值抑制补偿,增加边缘
	
	//==========求解梯度，幅度和方向===========
	for(y=0; y<69; y++) //最下面一行不处理
	{
		for(x=0; x<119; x++) //最上一列不处理
		{
			Xg =  org[y][x] + org[y+1][x] - org[y][x+1] - org[y+1][x+1]; //计算X方向梯度
			Yg = -org[y][x] + org[y+1][x] - org[y][x+1] + org[y+1][x+1]; //计算Y方向梯度
			A[y][x] = Fsqrt(Xg*Xg+Yg*Yg); //求幅值，快速开平方算法
			Sector[y][x] = Atan2(Yg, Xg);  //求梯度方向分区 			
		}
	}

	//===============非极大值抑制================
	for(y=1; y<69; y++)
	{
		for(x=0; x<119; x++)
		{
			if(0 == Sector[y][x])) //水平方向
			{
				if( (A[y][x] > A[y][x+1] - loss) && A[y][x] > (A[y][x-1] - loss) )
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
				
			}
			if(1 == Sector[y][x]) //右上、左下
			{
				if( A[y][x] > (A[y-1][x+1] - loss) && A[y][x] > (A[y+1][x-1] - loss) )
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			}
			if(2 == Sector[y][x]) //竖直方向
			{
				if( A[y][x] > (A[y-1][x] - loss) && A[y][x] > (A[y+1][x] - loss) )
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			}
			if(3 == Sector[y][x]) //左上、右下
			{
				if( A[y][x] > (A[y-1][x-1] - loss) && A[y][x] > (A[y+1][x+1] - loss) )
				{
					A[y][x] = A[y][x];
				}
				else
				{
					A[y][x] = 0;
				}
			}
		}
	}
	
	//=============双阈值检测=================
	for(y=1; y<68; y++)
	{
		for(x=1; x<119; x++)
		{
			if( A[y][x] < lowThr ) //低于低阈值
			{
				ImgEdge[y][x] = 0;
			}
			else if( A[y][x] > highThr ) //高于高阈值
			{
				ImgEdge[y][x] = 1; 
			}
			else 
			{
				Temp[0] = A[y+1][x-1]; Temp[1] = A[y+1][x]; Temp[2] = A[y+1][x+1];
				Temp[3] = A[y][x-1];   Temp[4] = A[y][x];   Temp[5] = A[y][X+1];
				Temp[6] = A[y-1][x-1]; Temp[7] = A[y-1][x]; Temp[8] = A[y-1][x+1]; //3x3区域内点
				Quicksort(Temp, 9, 0, 8); //快速排序,低到高
				
				if(Temp[8] > highThr)
				{
					ImgEdge[y][x] = 1;
					A[y][x] = 1;
				} //end of if
				else
				{
					ImgEdge[y][x] = 0;
					A[y][x] = 0;
				}
			}
		}
	}
} 