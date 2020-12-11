/*
 * main.cpp
 *
 *  Created on: 10 ���. 2020 �.
 *      Author: ���������
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>

double sinc(double x){
if(x==0) return 1.;
return sin(x)/x;
}

double ff(double x,double y){
return sinc(hypot(x, y));
}

const double PI = acos(-1.0);
constexpr
int
W = 1920,
H = 1280;
constexpr
double
RX=30.,
RY=30.,
RZ=450.,
XMAX=20.,
XMIN=-20.,
YMAX=20.,
YMIN=-20.,
STEP1=0.4,
STEP2=0.01;

#pragma pack(push,1)
struct TGA{
	uint8_t id_len;
	uint8_t pal_type;
	uint8_t img_type;
	uint8_t pal_desc[5];
	uint16_t xpos,ypos;
	uint16_t w,h;
	uint8_t bpp;
	uint8_t img_desc;
};
#pragma pack(pop)

void plot(std::vector<uint32_t>& pixels, int x, int y, uint32_t c,std::vector<int>& h)
{
	if (x < 0 || x >= W)
		return;
	if ( y > h[x])
		return;
	h[x]=y;
	if(y<0)
		return;
	if ( (int)(y * W + x) <= (int)pixels.size())
		pixels[y*W + x] = c;

}

int main(){
	int sx=0,sy=0;
	double z;
	std::fstream f;
	TGA h {};
	std::vector<int> hor;
	hor.resize(W);
	for(auto && a:hor)
		a=H;
	std::vector <uint32_t> p;
	p.resize(W*H);
	for(auto && c:p)
		c=0xFF3cb371;

	h.img_type=2;
	h.w=W;
	h.h=H;
	h.bpp=32;
	h.img_desc=0x28;

	for(double i=XMAX;i>XMIN;i=i-STEP1){
		for(double j=YMAX;j>YMIN;j=j-STEP2){
			z=ff(i,j);
			sx = int( W / 2 - RX * i * cos( PI / 6 ) + RY * j * cos( PI/ 6 ) );
			sy = int( H / 2 + RX * i * sin( PI / 6 ) + RY * j * sin( PI / 6 ) - RZ * z);
			plot(p, sx, sy, 0xff000000,hor);
		}
	}
	for(auto && a:hor)
			a=H;
	for(double j=YMAX;j>YMIN;j=j-STEP1){
			for(double i=XMAX;i>XMIN;i=i-STEP2){
				z=ff(i,j);
				sx = int( W / 2 - RX * i * cos( PI / 6 ) + RY * j * cos( PI/ 6 ) );
				sy = int( H / 2 + RX * i * sin( PI / 6 ) + RY * j * sin( PI / 6 ) - RZ * z);
				plot(p, sx, sy, 0xff000000,hor);
			}
		}



f.open("1.tga",std::ios::out|std::ios::binary);
f.write(reinterpret_cast<char*>(&h),sizeof(TGA));
f.write(reinterpret_cast<char*>(&p[0]),W*H*4);
f.close();
return 0;
}







