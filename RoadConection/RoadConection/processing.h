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
	Mat Mythreshold(Mat &image,int sigma) ;//��ֵ������0,1��
	Mat Mythreshold2(Mat &image,int sigma); //��ֵ������0,255��
	Mat thining(Mat &img) ; //ϸ���ɵ������ߴ���
	Mat MagnifyImg(Mat &img);// �Ŵ�Ӱ�����������Ҹ��ؿ�һ�� �Ա㴦��߽�
	Mat ClearTrack(Mat &ClassImg,Mat &img,vector<EndPoint> &EndList,int threashold);//���ڸ����㷨ȥ��ë��
	TrackPoint processing::FindNext_P1(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg);//˳ʱ�� Ѱ����һ��ֵ���x y
	TrackPoint processing::FindNext_P2(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg);//��ʱ�� Ѱ����һ��ֵ���x y
	Mat ReduceImg(Mat &img);// ��ԭӰ��
	Mat linkAnalysis(Mat binaryImage);
	void linkFind(vector<Point> &searchList, vector<Point> &tempList, Mat labelMap, vector<Point> &categoryMember);
	void ListMake(vector<EndPoint>* EndList,vector<NodePoint>* NodeList,Mat &img,Mat* ClassImg);
	Mat Clear_Img(Mat &img);
	bool vectorize(std::string outputPath, vector<Line> PointPair);
	processing(void);
	~processing(void);
};

