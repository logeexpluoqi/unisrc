/*
 * @Author: luoqi
 * @Date: 2022-11-22 22:06:28
 * @Last Modified by: luoqi
 * @Last Modified time: 2022-11-23 00:29:36
 */

#include "qtrigf.h"

const float hollyst = 0.017453292519943295769236907684886;

const float sin_table[] = {
    0.0,                                    // sin(0)
    0.17364817766693034885171662676931 ,    // sin(10)
    0.34202014332566873304409961468226 ,    // sin(20)
    0.5 ,                                   // sin(30)
    0.64278760968653932632264340990726 ,    // sin(40)
    0.76604444311897803520239265055542 ,    // sin(50)
    0.86602540378443864676372317075294 ,    // sin(60)
    0.93969262078590838405410927732473 ,    // sin(70)
    0.98480775301220805936674302458952 ,    // sin(80)
    1.0                                     // sin(90)
};

const float cos_table[] = {
    1.0 ,                                   // cos(0)
    0.99984769515639123915701155881391 ,    // cos(1)
    0.99939082701909573000624344004393 ,    // cos(2)
    0.99862953475457387378449205843944 ,    // cos(3)
    0.99756405025982424761316268064426 ,    // cos(4)
    0.99619469809174553229501040247389 ,    // cos(5)
    0.99452189536827333692269194498057 ,    // cos(6)
    0.99254615164132203498006158933058 ,    // cos(7)
    0.99026806874157031508377486734485 ,    // cos(8)
    0.98768834059513772619004024769344      // cos(9)
};

float qfsind(float x)
{
    int sig = 0;

    if(x > 0.0){
        while(x >= 360.0) {
            x = x - 360.0;
        }
    }else{
        while(x < 0.0) {
            x = x + 360.0;
        }
    }

    if(x >= 180.0){
        sig = 1;
        x = x - 180.0;
    }

    x = (x > 90.0) ? (180.0 - x) : x;

    int a = x * 0.1;
    float b = x - 10 * a;
    
    float y = sin_table[a] * cos_table[(int)b] + b * hollyst * sin_table[9 - a];

    return (sig > 0) ? -y : y;
}

float qfcosd(float x)
{
    return qfsind(x + 90.0);
}

float qftand(float x)
{

}

float qfasind(float x)
{
    return 0;
}

float qfacosd(float x)
{
    return 0;
}

float qfatand(float x)
{

}
