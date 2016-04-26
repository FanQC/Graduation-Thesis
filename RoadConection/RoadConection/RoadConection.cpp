// RoadConection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <vector>
#include <Windows.h>
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <tchar.h>
#include <cstdlib>
#include "StructDef.h"
#include <time.h>
#include "processing.h"
#include "DP.h"

using namespace std;
using namespace cv;


void printTime(void)
{
	time_t rawtime;
	struct tm * timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	cout << " - " << asctime(timeinfo);
}



int _tmain(int argc, _TCHAR* argv[])
{
	printTime();

	processing m_pro;
	DP douglas;

	Mat src  = cvLoadImage("E:\\testRoad1.jpg",0);
	cout<<src.channels()<<endl;
	Mat ClassImg = Mat::zeros(src.rows+2,src.cols+2,CV_8UC1);//创建空图像用于存储像元点类型
	//imshow("bb",ClassImg);
	//cvWaitKey(0);
	Mat MagfImg=m_pro.MagnifyImg(src);
	Mat preImg=m_pro.Mythreshold(MagfImg,200);
	//imwrite("E:\\NormaltestRoad.jpg",preImg);
	Mat img=preImg.clone();
	Mat img1=ClassImg.clone();
	Mat img2=ClassImg.clone();
	imshow("aa",src);
	cvWaitKey(0);
	cout<<img.rows<<"  "<<img.cols<<endl;

	img=m_pro.thining(img);
	//img=m_pro.thining(img);

   

	vector<EndPoint> EndList; //端点链表
	vector<NodePoint> NodeList; //节点链表
	vector<EndPoint> *EndL=&EndList;
	vector<NodePoint> *NodeL=&NodeList;
	Mat *p=&ClassImg;
	
	m_pro.ListMake(EndL,NodeL,img,p);//寻找端点链表和节点链表 和 点类型图
	Mat clearImg=m_pro.ClearTrack(ClassImg,img,EndList,10);//去除毛刺
	cout<<EndList.size()<<"   "<<NodeList.size()<<endl;
	Mat UseLabelImg=Mat::zeros(img.rows,img.cols,CV_8UC1);

	EndList.clear();
	NodeList.clear();
	ClassImg=m_pro.Clear_Img(ClassImg);

	Mat ClassImg1 = Mat::zeros(src.rows+2,src.cols+2,CV_8UC1);
	p=&ClassImg1;
	m_pro.ListMake(EndL,NodeL,clearImg,p);//再做一次端点寻找
	cout<<EndList.size()<<"   "<<NodeList.size()<<endl;
	Mat clearImg1=m_pro.Mythreshold2(clearImg,1);
	imshow("Clear",clearImg1);
	cvWaitKey(0);

	vector<Line> LineG;





	LineG=douglas.GenarateLine(clearImg,UseLabelImg,p,EndList,1);   //道格拉斯匹克算法

	cout<<LineG.size()<<endl;

	vector<Line>::iterator go;
	go=LineG.begin();
	Mat img10=img.clone();
	while(go!=LineG.end())
	{
		
		Point P1((*go).StartPt.y,(*go).StartPt.x);
		Point P2((*go).EndPt.y,(*go).EndPt.x);
		line(img10,P1,P2,Scalar(255,0,0));
		imshow("Line",img10);
        cvWaitKey(0);
		go++;
	}
	//画线***************************************************************************
	//m_pro.vectorize("E:\\Download",LineG);
	//EndList.clear();
	//NodeList.clear();
	//ClassImg=m_pro.Clear_Img(ClassImg);

	//m_pro.ListMake(EndL,NodeL,clearImg,p);//再做一次端点寻找
	//Mat clearImg2=m_pro.ClearTrack(ClassImg,clearImg,EndList,10);

	//clearImg=m_pro.linkAnalysis(clearImg);
	//clearImg=m_pro.ReduceImg(clearImg);
    //imwrite("E:\\clearImg.jpg",clearImg);

	cout<<EndList.size()<<"   "<<NodeList.size()<<endl;
	img=m_pro.Mythreshold2(img,1);
	//clearImg=m_pro.Mythreshold2(clearImg,1);
	printTime();
	imshow("thining result",img);
	cvWaitKey(0);
	//imshow("EndPoint",img1);
	//cvWaitKey(0);
	//imshow("NodePoint",img2);
	//cvWaitKey(0);
	
	//clearImg2=m_pro.Mythreshold2(clearImg2,1);
	//imshow("Clear2",clearImg2);

	




	return 0;
}

