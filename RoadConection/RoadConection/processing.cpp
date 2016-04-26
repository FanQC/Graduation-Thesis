#include "StdAfx.h"
#include "processing.h"
#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <vector>
#include<Windows.h>
#include "gdal_priv.h"
#include"ogrsf_frmts.h"
#include<tchar.h>
#include<cstdlib>
#include"StructDef.h"
#include <time.h>

using namespace std;
using namespace cv;


Mat processing::Mythreshold(Mat &image,int sigma) //二值化处理（0,1）
{
	Mat img = image.clone();

	int h = img.rows;
	int w = img.cols;

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			if (img.at<unsigned char>(j, i) < sigma)
			{
				img.at<unsigned char>(j, i) = 0;
			}
			else 
			{
				img.at<unsigned char>(j, i) = 1;
			}

		}
	}

	return img;
}

Mat processing::Mythreshold2(Mat &image,int sigma) //二值化处理（0,255）
{
	Mat img = image.clone();

	int h = img.rows;
	int w = img.cols;

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		{
			if (img.at<unsigned char>(j, i) < sigma)
			{
				img.at<unsigned char>(j, i) = 0;
			}
			else 
			{
				img.at<unsigned char>(j, i) = 255;
			}

		}
	}

	return img;
}

Mat processing::thining(Mat &img) //细化成单像素线处理
{
	int P[8];
	int C[8];
	for(int i=1;i<img.rows-1;i++)
		{
			for(int j=1;j<img.cols-1;j++)
			{

				int sum=0;int Cn;int S=0;
				if(img.at<unsigned char>(i,j)==0) continue;

				P[0]=img.at<unsigned char>(i-1,j-1);
				P[1]=img.at<unsigned char>(i-1,j);
				P[2]=img.at<unsigned char>(i-1,j+1);
				P[3]=img.at<unsigned char>(i,j+1);
				P[4]=img.at<unsigned char>(i+1,j+1);
				P[5]=img.at<unsigned char>(i+1,j);
				P[6]=img.at<unsigned char>(i+1,j-1);
				P[7]=img.at<unsigned char>(i,j-1);  //八邻域DN值
				
				C[0]=abs(P[0]-P[1]);
				C[1]=abs(P[1]-P[2]);
				C[2]=abs(P[2]-P[3]);
				C[3]=abs(P[3]-P[4]);
				C[4]=abs(P[4]-P[5]);
				C[5]=abs(P[5]-P[6]);
				C[6]=abs(P[6]-P[7]);
				C[7]=abs(P[7]-P[0]);

				for(int m=0;m<8;m++)
				{
				    sum=sum+C[m];
					
					
					S=S+P[m];
					
				}
				Cn=sum/2; //连接数
				//cout<<"Cn= "<<Cn<<endl;
				//cout<<"S= "<<S<<endl;

				if(Cn==1 && S!=1)
				{
					img.at<unsigned char>(i,j)=0;
				}
			}
	}
	return img;
}
Mat processing::MagnifyImg(Mat &img)
{
	Mat MagfImg = Mat::zeros(img.rows+2,img.cols+2,CV_8UC1);//创建放大后的空图像，边界点为0
	for(int i=1;i<MagfImg.rows-1;i++)
	{
		for(int j=1;j<MagfImg.cols-1;j++)
		{
			MagfImg.at<unsigned char>(i,j)=img.at<unsigned char>(i-1,j-1);//将图像复制到新图中
			
		}
	}
	/*imshow("before",img);
	cvWaitKey(0);
	imshow("after",MagfImg);
	cvWaitKey(0);*/
	return MagfImg;
}

Mat processing::ReduceImg(Mat &img)// 复原影像
{
	Mat ReducImg= Mat::zeros(img.rows-2,img.cols-2,CV_8UC1);
		for(int i=0;i<ReducImg.rows;i++)
	{
		for(int j=0;j<ReducImg.cols;j++)
		{
			ReducImg.at<unsigned char>(i,j)=img.at<unsigned char>(i+1,j+1);//将图像复制到新图中
			
		}
	}
		return ReducImg;
}
TrackPoint processing::FindNext_P1(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg)//修改至此处  ********************************************************** 顺时针
{
	
	TrackPoint Tpoint;
	if(img.at<unsigned char>(i-1,j-1)==1 && UseLabelImg.at<unsigned char>(i-1,j-1)!=1) {Tpoint.x=i-1;Tpoint.y=j-1;return Tpoint;}
	if(img.at<unsigned char>(i-1,j)==1   && UseLabelImg.at<unsigned char>(i-1,j)!=1)   {Tpoint.x=i-1;Tpoint.y=j;return Tpoint;}
	if(img.at<unsigned char>(i-1,j+1)==1 && UseLabelImg.at<unsigned char>(i-1,j+1)!=1) {Tpoint.x=i-1;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i,j+1)==1   && UseLabelImg.at<unsigned char>(i,j+1)!=1)   {Tpoint.x=i;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i+1,j+1)==1 && UseLabelImg.at<unsigned char>(i+1,j+1)!=1) {Tpoint.x=i+1;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i+1,j)==1   && UseLabelImg.at<unsigned char>(i+1,j)!=1)   {Tpoint.x=i+1;Tpoint.y=j;return Tpoint;}
	if(img.at<unsigned char>(i+1,j-1)==1 && UseLabelImg.at<unsigned char>(i+1,j-1)!=1) {Tpoint.x=i+1;Tpoint.y=j-1;return Tpoint;}
	if(img.at<unsigned char>(i,j-1)==1   && UseLabelImg.at<unsigned char>(i,j-1)!=1)   {Tpoint.x=i;Tpoint.y=j-1;return Tpoint;}
	cout<<"error for finding"<<endl; Tpoint.x=i;Tpoint.y=j;(*ClassImg).at<unsigned char>(i,j)=1;return Tpoint;
}
TrackPoint processing::FindNext_P2(int &i,int &j,Mat &img,Mat &UseLabelImg,Mat* ClassImg) //逆时针
{

	TrackPoint Tpoint;
	if(img.at<unsigned char>(i,j-1)==1   && UseLabelImg.at<unsigned char>(i,j-1)!=1)   {Tpoint.x=i;Tpoint.y=j-1;return Tpoint;}
	if(img.at<unsigned char>(i+1,j-1)==1 && UseLabelImg.at<unsigned char>(i+1,j-1)!=1) {Tpoint.x=i+1;Tpoint.y=j-1;return Tpoint;}
	if(img.at<unsigned char>(i+1,j)==1   && UseLabelImg.at<unsigned char>(i+1,j)!=1)   {Tpoint.x=i+1;Tpoint.y=j;return Tpoint;}
	if(img.at<unsigned char>(i+1,j+1)==1 && UseLabelImg.at<unsigned char>(i+1,j+1)!=1) {Tpoint.x=i+1;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i,j+1)==1   && UseLabelImg.at<unsigned char>(i,j+1)!=1)   {Tpoint.x=i;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i-1,j+1)==1 && UseLabelImg.at<unsigned char>(i-1,j+1)!=1) {Tpoint.x=i-1;Tpoint.y=j+1;return Tpoint;}
	if(img.at<unsigned char>(i-1,j)==1   && UseLabelImg.at<unsigned char>(i-1,j)!=1)  {Tpoint.x=i-1;Tpoint.y=j;return Tpoint;}
    if(img.at<unsigned char>(i-1,j-1)==1 && UseLabelImg.at<unsigned char>(i-1,j-1)!=1) {Tpoint.x=i-1;Tpoint.y=j-1;return Tpoint;}
	cout<<"error for finding"<<endl; Tpoint.x=i;Tpoint.y=j;(*ClassImg).at<unsigned char>(i,j)=1;return Tpoint;
}
Mat processing::ClearTrack(Mat &ClassImg,Mat &img,vector<EndPoint> &EndList,int threashold)
{
	Mat UseLabelImg=Mat::zeros(img.rows,img.cols,CV_8UC1);
	Mat img2=img.clone();
	vector<EndPoint>::iterator it; //迭代变量
	it=EndList.begin();
	int n=0;

	while(it!=EndList.end()) //遍历端点链表
	{
		//Mat img10=img.clone();
		vector<TrackPoint> TrackList; //创建跟踪链表 以保存跟踪点
		TrackPoint newTrackP((*it).x,(*it).y); 
		TrackList.push_back(newTrackP); //存入初始端点
		UseLabelImg.at<unsigned char>((*it).x,(*it).y)=1;//标记点为使用过 1 
		Mat* p=&ClassImg;
		int xn=(FindNext_P1((*it).x,(*it).y,img,UseLabelImg,p)).x;
		int yn=(FindNext_P1((*it).x,(*it).y,img,UseLabelImg,p)).y;

		int xn2=(FindNext_P2((*it).x,(*it).y,img,UseLabelImg,p)).x;
		int yn2=(FindNext_P2((*it).x,(*it).y,img,UseLabelImg,p)).y;

		if(xn!=xn2 || yn!=yn2) 
		{
			int dis1=(xn-(*it).x)*(xn-(*it).x)+(yn-(*it).y)*(yn-(*it).y);
			int dis2=(xn2-(*it).x)*(xn2-(*it).x)+(yn2-(*it).y)*(yn2-(*it).y);    //找下一点 顺时针 逆时针 各找一次 取距离近的
			if (dis1>dis2) {xn=xn2;yn=yn2;}
		} 
		
		UseLabelImg.at<unsigned char>(xn,yn)=1;//标记点为使用过 1
		
		while(TrackList.size()<threashold)
		{
			//img10.at<unsigned char>(xn,yn)=255;
			if(ClassImg.at<unsigned char>(xn,yn)==2 || ClassImg.at<unsigned char>(xn,yn)==1) 
		    {
				UseLabelImg.at<unsigned char>(xn,yn)=0;
			    if(ClassImg.at<unsigned char>(xn,yn)==1)
				{img2.at<unsigned char>(xn,yn)=0;}
			    vector<TrackPoint>::iterator it2;
			    it2=TrackList.begin();
			    while(it2!=TrackList.end())
			    {
				    img2.at<unsigned char>((*it2).x,(*it2).y)=0;
				    it2++;
			    }
				n++;
				break;
				
		    }
			TrackPoint newTrackP1(xn,yn); 
		    TrackList.push_back(newTrackP1); //存入第二点
			int xn1=(FindNext_P1(xn,yn,img,UseLabelImg,p)).x;
			int yn1=(FindNext_P1(xn,yn,img,UseLabelImg,p)).y;
		
			xn2=(FindNext_P2(xn,yn,img,UseLabelImg,p)).x;
			yn2=(FindNext_P2(xn,yn,img,UseLabelImg,p)).y;
			if(xn1!=xn2 || yn1!=yn2) 
		    {
			    int dis1=(xn1-xn)*(xn1-xn)+(yn1-yn)*(yn1-yn);
			    int dis2=(xn2-xn)*(xn2-xn)+(yn2-yn)*(yn2-yn);    //找下一点 顺时针 逆时针 各找一次 取距离近的
				if (dis1>dis2) {xn=xn2;yn=yn2;}else{xn=xn1;yn=yn1;}
		    } 
			else{xn=xn1;yn=yn1;}
			//imshow("position",img10);
			//cvWaitKey(0);
			UseLabelImg.at<unsigned char>(xn,yn)=1;
		}
		
		it++;

	}
	cout<<n<<endl;
	//img2=Mythreshold2(img2,1);
	return img2;
}
void processing::linkFind(vector<Point> &searchList, vector<Point> &tempList, Mat labelMap, vector<Point> &categoryMember)
{
	tempList.clear();

	for (int index=0; index<searchList.size(); index++)
	{
		tempList.push_back(searchList[index]);
		categoryMember.push_back(searchList[index]);
		labelMap.at<uchar>(searchList[index].y, searchList[index].x) = 0;
	} 

	searchList.clear();

	for (int index=0; index<tempList.size(); index++)
	{
		int p_height = tempList[index].y;
		int p_width = tempList[index].x;
		Point tempPoint;

		if (p_height > 1 && p_height < labelMap.rows-1 && p_width > 1 && p_width<labelMap.cols-1)
		{

			for (int i=-1; i<=1; i++)
			{
				for (int j=-1; j<=1; j++)
				{

					if ( (int)labelMap.at<uchar>(p_height+i, p_width+j) > 128)
					{

						tempPoint.y = p_height +i;
						tempPoint.x = p_width +j ;
						searchList.push_back(tempPoint);
						labelMap.at<uchar>(p_height+i, p_width+j) = 0;
					}

				}
			}
		}
	}

	if (!searchList.empty())
	{
		linkFind(searchList, tempList, labelMap, categoryMember);;
	}
	else
	{
		return;
	}


}
Mat processing::linkAnalysis(Mat binaryImage)
{
	Mat road = binaryImage.clone(); 
	Mat labelMap = binaryImage.clone();
	Mat paintMap = binaryImage.clone();
	paintMap = 0;

	vector<vector<Point>>  categoryList;

	for (int i=0; i<labelMap.rows; i++)
	{
		for (int j=0; j<labelMap.cols; j++)
		{
			int binaryValue = (int)binaryImage.at<uchar>(i, j);
			if (binaryValue > 128)
			{
				labelMap.at<uchar>(i, j) = 255;
			}
			else
			{
				labelMap.at<uchar>(i, j) = 0;
			}
		}
	}

	//imwrite("D:\\work2010-2011\\work2010-2011\\My Work\\testImage\\imageForPG\\roadDetection\\bMap.jpg", labelMap);
	int countNum = 0;

	for (int i=1; i<binaryImage.rows-1; i++)
	{
		for (int j=1; j<binaryImage.cols-1; j++)
		{
			vector<Point> categoryMember;
			vector<Point> searchList;
			vector<Point> tempList;
			Point tempPoint;
			tempPoint.x = j;
			tempPoint.y = i;
			searchList.push_back(tempPoint);



			if ((int)labelMap.at<uchar>(i, j) > 128)
			{

				//if (countNum == 544)
				//{
				//	cout << "be careful";
				//}
				linkFind(searchList, tempList, labelMap, categoryMember);	

				//countNum = countNum+1;
				//cout << countNum << "     ";

				categoryList.push_back(categoryMember);	
				categoryMember.clear();

				//imshow("labelMap", labelMap);
				//waitKey(1);
			}

		}
	}

	//cout << categoryList.size() << "\n";

	for (int index=0; index<categoryList.size(); index++)
	{  
		if (categoryList[index].size() >50)
		{
			for (int m=0; m<categoryList[index].size(); m++)
			{
				paintMap.at<uchar>(categoryList[index][m].y, categoryList[index][m].x) = 255;
			}
		}
	}
	//m_paint_map = paintMap.clone();
	/*imshow("paintMap", paintMap);*/
	//waitKey(0);
	imwrite("only_road_map.jpg",paintMap);
	
	
	return paintMap;
}

void processing::ListMake(vector<EndPoint>* EndList,vector<NodePoint>* NodeList,Mat &img,Mat* ClassImg)
{
	
    int P[8];
	int C[8];
	Mat img1=img.clone();
	for(int i=1;i<img.rows-1;i++)
		{
			for(int j=1;j<img.cols-1;j++)
			{

				int sum=0;int Cn;int S=0;
				if(img.at<unsigned char>(i,j)==0) continue;

				P[0]=img.at<unsigned char>(i-1,j-1);
				P[1]=img.at<unsigned char>(i-1,j);
				P[2]=img.at<unsigned char>(i-1,j+1);
				P[3]=img.at<unsigned char>(i,j+1);
				P[4]=img.at<unsigned char>(i+1,j+1);
				P[5]=img.at<unsigned char>(i+1,j);
				P[6]=img.at<unsigned char>(i+1,j-1);
				P[7]=img.at<unsigned char>(i,j-1);  //八邻域DN值
				
				C[0]=abs(P[0]-P[1]);
				C[1]=abs(P[1]-P[2]);
				C[2]=abs(P[2]-P[3]);
				C[3]=abs(P[3]-P[4]);
				C[4]=abs(P[4]-P[5]);
				C[5]=abs(P[5]-P[6]);
				C[6]=abs(P[6]-P[7]);
				C[7]=abs(P[7]-P[0]);

				for(int m=0;m<8;m++)
				{
				    sum=sum+C[m];
					
					
					S=S+P[m];
					
				}

				Cn=sum/2; //连接数
				//cout<<"Cn= "<<Cn<<endl;
				//cout<<"S= "<<S<<endl;

				if(Cn==1)
				{
					EndPoint newEndPoint(i,j);
					EndList->push_back(newEndPoint);
					img1.at<unsigned char>(i-1,j-1)=255; //在图上表示端点
					(*ClassImg).at<unsigned char>(i,j)=1; //1 为EndPoint
				}
				if(Cn==3 || Cn==4 || (Cn==2 && S>=6)) 
				{
					NodePoint newNodePoint(i,j);
					(*NodeList).push_back(newNodePoint);
					//img2.at<unsigned char>(i-1,j-1)=255; //在图上表示节点
					(*ClassImg).at<unsigned char>(i,j)=2; //2 为NodePoint //0 为普通点
				}
				//imshow("EndPoint",img1);
			}
	    }
}

Mat processing::Clear_Img(Mat &img)
{
	for(int i=0;i<img.cols;i++)
	{
			for(int j=0;j<img.rows;j++)
		{
			img.at<unsigned char>(i,j)=0;
		}
	}
	return img;
}

bool processing::vectorize(std::string outputPath, vector<Line> PointPair)
{
	//PointPair里存了很多点对
	const char *pszDriverName = "ESRI Shapefile";
	//如果已有则删除
	if (GetFileAttributes(outputPath.c_str()) > 0){
		DeleteFile(outputPath.c_str());
	}
	//注册GDAL驱动
	GDALAllRegister();
	//注册OGR驱动
	OGRRegisterAll();
	//中文路径支持
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");


	OGRSFDriver *poDriver;
	poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
		pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		exit(1);
	}
	OGRDataSource *poDS;
	poDS = poDriver->CreateDataSource(outputPath.c_str(), NULL);//但文件夹中以前存在point_out.shp文件时会报错;
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		exit(1);
	}

	OGRLayer *poLayer;
	poDS->CreateLayer("road_layer", NULL, wkbMultiLineString, NULL);
	poLayer = NULL;
	//新建的shp文件只有一个图层;
	poLayer = poDS->GetLayer(0);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		exit(1);
	}

	//写目标信息
	for (unsigned int i = 0; i <PointPair.size(); i++){
		Line info = PointPair.at(i);
		//创建features，写入feature到磁盘;
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		//绘制目标外边框
		OGRLineString line;
		
	/*	for (unsigned int j = 0; j < pts->size(); j++)
		{
			OGRPoint pint((*pts)[j].x, (*pts)[j].y);
			line.addPoint(&pint);
		}*/
		OGRPoint pint(info.StartPt.x, info.StartPt.y);
		line.addPoint(&pint);
		OGRPoint pint1(info.EndPt.x, info.EndPt.y);
		line.addPoint(&pint1);
		poFeature->SetGeometry(&line);
		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE){
			printf("第%d条道路创建失败！", i);
		}
		OGRFeature::DestroyFeature(poFeature);
	}
	OGRDataSource::DestroyDataSource(poDS);
	printf("创建矢量数据成功！\n");
	return true;
}
processing::processing(void)
{
}


processing::~processing(void)
{
}
