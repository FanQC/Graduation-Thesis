#include <cv.h>
#include <highgui.h>
#include <vector>
#include<Windows.h>
#include "gdal_priv.h"
#include"ogrsf_frmts.h"
#include<tchar.h>
#include<cstdlib>
#include"StructDef.h"
using namespace std;
using namespace cv;

#pragma once
class processing
{
public:
	Mat Mythreshold(Mat &image,int sigma) ;//二值化处理（0,1）
	Mat Mythreshold2(Mat &image,int sigma); //二值化处理（0,255）
	Mat thining(Mat &img) ; //细化成单像素线处理
	Mat MagnifyImg(Mat &img);// 放大影像至上下左右各拓宽一行 以便处理边界
	Mat ClearTrack(Mat &ClassImg,Mat &img,vector<EndPoint> &EndList,int threashold);//基于跟踪算法去除毛刺
	TrackPoint processing::FindNext_P1(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg);//顺时针 寻找下一有值点的x y
	TrackPoint processing::FindNext_P2(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg);//逆时针 寻找下一有值点的x y
	Mat ReduceImg(Mat &img);// 复原影像
	Mat linkAnalysis(Mat binaryImage);
	void linkFind(vector<Point> &searchList, vector<Point> &tempList, Mat labelMap, vector<Point> &categoryMember);
	void ListMake(vector<EndPoint>* EndList,vector<NodePoint>* NodeList,Mat &img,Mat* ClassImg);
	Mat Clear_Img(Mat &img);
	bool vectorize(std::string outputPath, vector<Line> PointPair);
	processing(void);
	~processing(void);
};

