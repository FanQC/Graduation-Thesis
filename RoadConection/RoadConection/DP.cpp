#include "StdAfx.h"
#include "DP.h"
#include "StructDef.h"
#include "processing.h"
#include <vector>
double DP::DistP2Line(Pt &pt1,Pt &pt2,Pt &pt3)
{
	double A, B, C, maxDist = 0;
    A = pt2.y - pt1.y;
    B = pt1.x - pt2.x;
    C = pt2.x * pt1.y - pt1.x * pt2.y;
    maxDist = fabs((A * pt3.x + B * pt3.y + C) / sqrt(A * A + B *B));
   return maxDist;
}

DisPt DP::Dmax(vector<Pt> &LineList)
{
	vector<Pt>::iterator it;
	it=LineList.begin();
	DisPt MaxP; 
	double Dmax2=0;
	vector<Pt>::iterator it2;
	it2=LineList.end();
	while(it!=LineList.end())
	{
		
		double Dmax1=DistP2Line((*(LineList.begin())),(*(it2-1)),(*it));
		if(Dmax1>Dmax2)
		{
			Dmax2=Dmax1;
			MaxP.x=(*it).x;
			MaxP.y=(*it).y;
			MaxP.i=(*it).i;
			MaxP.Dmax=Dmax2;
		}
		it++;
	}
	return MaxP;
}

vector<Pt> DP::TraversLine(Mat &img,Mat &UseLabelImg,Mat* ClassImg,vector<EndPoint>::iterator it)
{
	processing mpro;
	Mat img10=img.clone();
	int i=0;
	Mat* p=ClassImg;
	vector<Pt> TrackList;
	Pt newTrackP((*it).x,(*it).y,i); 
	UseLabelImg.at<unsigned char>((*it).x,(*it).y)=1;
	TrackList.push_back(newTrackP); 
	
			int xn=(mpro.FindNext_P1((*it).x,(*it).y,img,UseLabelImg,p)).x;
		    int yn=(mpro.FindNext_P1((*it).x,(*it).y,img,UseLabelImg,p)).y;

		    int xn2=(mpro.FindNext_P2((*it).x,(*it).y,img,UseLabelImg,p)).x;
		    int yn2=(mpro.FindNext_P2((*it).x,(*it).y,img,UseLabelImg,p)).y;

		    if(xn!=xn2 || yn!=yn2) 
		{
			int dis1=(xn-(*it).x)*(xn-(*it).x)+(yn-(*it).y)*(yn-(*it).y);
			int dis2=(xn2-(*it).x)*(xn2-(*it).x)+(yn2-(*it).y)*(yn2-(*it).y);    //找下一点 顺时针 逆时针 各找一次 取距离近的
			if (dis1>dis2) {xn=xn2;yn=yn2;}
		} 
			if(UseLabelImg.at<unsigned char>(xn,yn)==1) return TrackList;
		
		
			
			
			while(ClassImg->at<unsigned char>(xn,yn)!=2 && ClassImg->at<unsigned char>(xn,yn)!=1)
			{
				i++;
				img10.at<unsigned char>(xn,yn)=255;
				Pt newTrackP1(xn,yn,i); 
		        TrackList.push_back(newTrackP1); //存入其他点
			UseLabelImg.at<unsigned char>(xn,yn)=1;//标记点为使用过 1
			int xn1=(mpro.FindNext_P1(xn,yn,img,UseLabelImg,p)).x;
			int yn1=(mpro.FindNext_P1(xn,yn,img,UseLabelImg,p)).y;
		
			xn2=(mpro.FindNext_P2(xn,yn,img,UseLabelImg,p)).x;
			yn2=(mpro.FindNext_P2(xn,yn,img,UseLabelImg,p)).y;
			if(xn1!=xn2 || yn1!=yn2) 
		    {
			    int dis1=(xn1-xn)*(xn1-xn)+(yn1-yn)*(yn1-yn);
			    int dis2=(xn2-xn)*(xn2-xn)+(yn2-yn)*(yn2-yn);    //找下一点 顺时针 逆时针 各找一次 取距离近的
				if (dis1>dis2) {xn=xn2;yn=yn2;}else{xn=xn1;yn=yn1;}
		    } 
			else{xn=xn1; yn=yn1;}
			
			    //imshow("position",img10);
			   // cvWaitKey(0);
			}
		        UseLabelImg.at<unsigned char>(xn,yn)=0;
				if(ClassImg->at<unsigned char>(xn,yn)==1) {UseLabelImg.at<unsigned char>(xn,yn)=1;Pt newTrackP1(xn,yn,i+1); 
		        TrackList.push_back(newTrackP1);}
				cout<<"Traverse over"<<endl;
			    return TrackList;
	
}

vector<Line> DP::GenarateLine(Mat &img,Mat &UseLabelImg,Mat* ClassImg,vector<EndPoint> EndList,double threashold)
{
	vector<Line> LineG;
	vector<EndPoint>::iterator it;
	vector<Pt> TraverseL;
	vector<Pt>::iterator it1;
	
	DisPt MaxP1;
	it=EndList.begin();
	int m=0;
	while(it!=EndList.end())
	{
	TraverseL=TraversLine(img,UseLabelImg,ClassImg,it);
	if(TraverseL.size()<2){it++;continue;}
	it1=TraverseL.end();
	MaxP1=Dmax(TraverseL);

	cout<<MaxP1.Dmax<<endl;

	
		if(MaxP1.Dmax<threashold)
		{
			Line newLine;
			newLine.StartPt=(*TraverseL.begin());
			newLine.EndPt=(*(it1-1));
			LineG.push_back(newLine);
			it++;
			continue;
		}
		else
		{
			vector<Pt> MaxPList;
			MaxPList=IteratorL(TraverseL,MaxP1,threashold);
			
			Line newLine;
			newLine.StartPt=(*TraverseL.begin());
			newLine.EndPt=(*MaxPList.begin());
			LineG.push_back(newLine);
			for(int i=0;i<MaxPList.size()-1;i++)
			{
				Line goLine;
				goLine.StartPt=MaxPList[i];
				goLine.EndPt=MaxPList[i+1];
				LineG.push_back(goLine);
			}
			it1=MaxPList.end();
			Line LastLine;
			LastLine.StartPt=(*(it1-1));
			it1=TraverseL.end();
			LastLine.EndPt=(*(it1-1));
			LineG.push_back(LastLine);
			it++;
		}
	//vector<Line>::iterator go;
	//go=LineG.begin();
	//Mat img10=img.clone();
	//while(go!=LineG.end())
	//{
	//	
	//	Point P1((*go).StartPt.y,(*go).StartPt.x);
	//	Point P2((*go).EndPt.y,(*go).EndPt.x);
	//	line(img10,P1,P2,Scalar(255,0,0));
	//	imshow("Line",img10);
 //       cvWaitKey(0);

	//	go++;
	//}
	}
	return LineG;
}

vector<vector<Pt>> DP::DivideLine(vector<Pt> &LineList,DisPt &MaxP)
{
	vector<vector<Pt>> DivLine;
	vector<Pt>::iterator it;
	it=LineList.begin();
	vector<Pt> LineL;
	vector<Pt> LineR;
	int j=MaxP.i;
	for(int i=(*it).i;i<j+1;i++)
	{
		LineL.push_back(LineList[i-(*it).i]);
	}
	DivLine.push_back(LineL);
	for(int i=j;i<LineList.size();i++)
	{
		LineR.push_back(LineList[i-(*it).i]);
	}
	DivLine.push_back(LineR);
	return DivLine;
}

vector<Pt> DP::IteratorL(vector<Pt> &TraverseL,DisPt &MaxP1,double threashold)
{
	vector<vector<Pt>> DivLine;
	DivLine=DivideLine(TraverseL,MaxP1);
	vector<Pt> MaxPList;
	vector<Pt> MaxPList1;
	vector<Pt>::iterator it1;
	int i=0;
	while(i<2)
	{
		DisPt MaxP;
		MaxP=Dmax(DivLine[i]);
		//it1=DivLine[i].end();
		/*if(MaxP1.i<MaxP.i)
		{
			Pt newPt(MaxP1.x,MaxP1.y,MaxP1.i);
			MaxPList.push_back(newPt);
		}*/
		if(MaxP.Dmax<threashold)
		{
			/*Pt newPt2(MaxP.x,MaxP.y,MaxP.i);
			MaxPList.push_back(newPt2);*/
			
			Pt newPt(MaxP1.x,MaxP1.y,MaxP1.i);
			MaxPList.push_back(newPt);
			
		}
		else 
		{
			
			MaxPList=IteratorL(DivLine[i],MaxP,threashold);
			
		}
	
		i++;

		
	}
	return MaxPList;
}
DP::DP(void)
{
}


DP::~DP(void)
{
}
