#pragma once
#include "gdal_priv.h"
#include"ogrsf_frmts.h"
#include<tchar.h>
#include<cstdlib>
#include"StructDef.h"
using namespace std;
using namespace cv;
class DP
{
public:
	double DistP2Line(Pt &pt1,Pt &pt2,Pt &pt3);//点到直线距离计算
	DisPt Dmax(vector<Pt> &LineList);//找最大值
	vector<Pt> TraversLine(Mat &img,Mat &UseLabelImg,Mat* ClassImg,vector<EndPoint>::iterator it);//遍历曲线
	vector<Line> GenarateLine(Mat &img,Mat &UseLabelImg,Mat* ClassImg,vector<EndPoint> EndList,double threashold);//构造逼近线
	vector<vector<Pt>> DivideLine(vector<Pt> &LineList,DisPt &MaxP);//曲线分段
	vector<Pt> IteratorL(vector<Pt> &TraverseL,DisPt &MaxP1,double threashold);
	Mat LineDraw(vector<Line> &GLine,Mat &img);
	DP(void);
	~DP(void);
};

