#pragma once
#include <cv.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
#include <string>
#include <vector>



struct EndPoint
{
	int x,y;
	EndPoint(){};
	EndPoint(int xp,int yp)
	{
		x=xp;
		y=yp;
	}
};

struct NodePoint
{
	int x,y;
	NodePoint(){};
	NodePoint(int xp,int yp)
	{
		x=xp;
		y=yp;
	}
};

struct TrackPoint
{
	int x,y;
	TrackPoint(){};
	TrackPoint(int xp,int yp)
	{
		x=xp;
		y=yp;
	}
};

struct Pt
{
	int x,y,i;
	Pt(){};
	Pt(int xp,int yp,int index)
	{
		x=xp;
		y=yp;
		i=index;
	}
};

struct Line
{
	Pt StartPt;
	Pt EndPt;
	double A;
	double B;
	double C;
};

struct DisPt
{
	int x,y,i;
	double Dmax;
	DisPt(){};
	DisPt(int xp,int yp,int index,double Dmmax)
	{
		x=xp;
		y=yp;
		i=index;
		Dmax=Dmmax;
	}
};
