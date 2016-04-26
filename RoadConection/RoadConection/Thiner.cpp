// Thiner.cpp: implementation of the CThiner class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Thiner.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//下面的细化表是
//参考 A Parallel Thinning Algorithm with Two-Subiteration
//     that Generates One-Pixel-Wide Skeletons.
//作者 Y.Y.Zhang & P.S.P. Wang
// Proceedings of ICPR'96
int CThiner::m_PTATb[]={
	0,0,0,0,0,0,1,0,0,0, 0,0,1,0,1,0,0,0,0,0,
    1,0,1,0,1,0,0,0,1,0, 1,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,0, 0,0,1,0,1,0,1,0,0,0,
	1,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	1,3,0,4,1,0,1,0,1,3, 0,4,1,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,1,3,0,4,1,0,1,0,
	1,3,0,4,1,0,1,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,1,1, 0,2,1,0,1,0,1,1,0,2,
	1,0,1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
	1,1,0,2,1,0,1,0,1,1, 0,2,1,0,1,0};
CThiner::CThiner()
{
	CreateGHTbl();
}

CThiner::~CThiner()
{

}

void CThiner::LoadData(unsigned char (*pData)[NORMALIZEDSIZE+2])
{
	m_pData=pData;
}
//参考 A Parallel Thinning Algorithm with Two-Subiteration
//     that Generates One-Pixel-Wide Skeletons.
//作者 Y.Y.Zhang & P.S.P. Wang
// Proceedings of ICPR'96
void CThiner::CreateGHTbl()
{
	int i,j;
	unsigned char P[8];
	int N1,N2,N,C;
	unsigned char Char;
	unsigned char Mask;
	int *pTb1,*pTb2;

	pTb1=m_GHTbOdd;
	pTb2=m_GHTbEvn;
	for(i=0;i<256;i++)
	{
		Char=i;
		Mask=0x01;
		for(j=0;j<8;j++)
		{
			if((Char&Mask)!=0x00)
			{
				P[j]=1;
			}
			else
				P[j]=0;
			Mask<<=1;
		}
		N1=(P[0]|P[1])+(P[2]|P[3])+(P[4]|P[5])+(P[6]|P[7]);
		N2=(P[1]|P[2])+(P[3]|P[4])+(P[5]|P[6])+(P[7]|P[0]);
		N=MIN(N1,N2);
		C=(((unsigned char)1-P[1])&(P[2]|P[3]))+(((unsigned char)1-P[3])&(P[4]|P[5]))+
			(((unsigned char)1-P[5])&(P[6]|P[7]))+(((unsigned char)1-P[7])&(P[0]|P[1]));
		//C为8-领域连接数
		/*C=(1-P[1])-(1-P[1])*(1-P[2])*(1-P[3])+
			(1-P[3])-(1-P[3])*(1-P[4])*(1-P[5])+
			(1-P[5])-(1-P[5])*(1-P[6])*(1-P[7])+
			(1-P[7])-(1-P[7])*(1-P[0])*(1-P[1]);*/
		if((C==1)&&((N==2)||(N==3)))
		{
			if((((P[1]|P[2])|((unsigned char)1-P[4]))&P[3])==0)
				pTb1[i]=0;//能删
			else
				pTb1[i]=1;//不能删
			if((((P[5]|P[6])|((unsigned char)1-P[0]))&P[7])==0)
				pTb2[i]=0;
			else
				pTb2[i]=1;
		}
		else
		{
			pTb1[i]=1;//不能删
			pTb2[i]=1;//不能删
		}
	}
}
//骨架不是严格的 1 象素宽
void CThiner::GHTA()
{
	int i,j,k;
	bool Change;
	int Count;
	unsigned char Neighbors;
	unsigned char (*pOD)[NORMALIZEDSIZE+2];
	unsigned char (*pTD1)[NORMALIZEDSIZE+2];
	unsigned char (*pTD2)[NORMALIZEDSIZE+2];
	int *pGHTb1,*pGHTb2,*pTb;

	pOD=m_pData;
	pTD1=m_TData1;
	pTD2=m_TData2;
	pGHTb1=m_GHTbOdd;
	pGHTb2=m_GHTbEvn;

	for(i=1;i<NORMALIZEDSIZE+1;i++)
	{
		for(j=1;j<NORMALIZEDSIZE+1;j++)
		{
			if(pOD[i][j]==0)
			{
				pTD1[i][j]=0;
				continue;
			}
			Neighbors=pOD[i-1][j-1]|(pOD[i-1][j]<<1)|
				(pOD[i-1][j+1]<<2)|(pOD[i][j+1]<<3)|
				(pOD[i+1][j+1]<<4)|(pOD[i+1][j]<<5)|
				(pOD[i+1][j-1]<<6)|(pOD[i][j-1]<<7);
			pTD1[i][j]=pGHTb1[Neighbors];
		}
		pTD1[i][0]=pTD1[i][NORMALIZEDSIZE+1]=0;
		pTD1[0][i]=pTD1[NORMALIZEDSIZE+1][i]=0;
		pTD2[i][0]=pTD2[i][NORMALIZEDSIZE+1]=0;
		pTD2[0][i]=pTD2[NORMALIZEDSIZE+1][i]=0;
	}
	pTD1[0][NORMALIZEDSIZE+1]=0;
	pTD1[0][0]=pTD1[NORMALIZEDSIZE+1][0]=0;
	pTD1[NORMALIZEDSIZE+1][NORMALIZEDSIZE+1]=0;
	pTD2[0][NORMALIZEDSIZE+1]=0;
	pTD2[0][0]=pTD1[NORMALIZEDSIZE+1][0]=0;
	pTD2[NORMALIZEDSIZE+1][NORMALIZEDSIZE+1]=0;

	Change=true;
	Count=2;
	for(;Change;)
	{
		Change=false;
		if((Count%2)==0)
		{
			pTb=pGHTb2;
			pTD1=m_TData1;//输入
			pTD2=m_TData2;//输出
		}
		else
		{
			pTb=pGHTb1;
			pTD1=m_TData2;//输入
			pTD2=m_TData1;//输出
		}
		for(i=1;i<NORMALIZEDSIZE+1;i++)
		{
			for(j=1;j<NORMALIZEDSIZE+1;j++)
			{
				if(pTD1[i][j]==0)
				{
					pTD2[i][j]=0;
					continue;
				}
				Neighbors=pTD1[i-1][j-1]|(pTD1[i-1][j]<<1)|
					(pTD1[i-1][j+1]<<2)|(pTD1[i][j+1]<<3)|
					(pTD1[i+1][j+1]<<4)|(pTD1[i+1][j]<<5)|
					(pTD1[i+1][j-1]<<6)|(pTD1[i][j-1]<<7);

				k=pTb[Neighbors];
				pTD2[i][j]=k;
				if(k==0)
					Change=true;
			}
		}
		Count++;
	}
	m_pTD=pTD2;//保存最终的细化结果
	/*FILE *pFile;
	pFile=fopen("C:\\ThinChar.txt","w");
	for(i=0;i<NORMALIZEDSIZE+2;i++)
	{
		for(j=0;j<NORMALIZEDSIZE+2;j++)
		{
			if(pTD2[i][j]==0)
				fprintf(pFile," ");
			else
				fprintf(pFile,"1");
		}
		fprintf(pFile,"\n");
	}
	fclose(pFile);*/
}
//生成的骨架是严格的 1 象素宽
void CThiner::PTATA()
{
	int i,j,k;
	bool Change;
	int Count;
	unsigned char Neighbors,Tmp1,Tmp2,NbN,NbW;
	unsigned char (*pOD)[NORMALIZEDSIZE+2];
	unsigned char (*pTD1)[NORMALIZEDSIZE+2];
	unsigned char (*pTD2)[NORMALIZEDSIZE+2];
	int *pTb;

	pOD=m_pData;
	pTD1=m_TData1;
	pTD2=m_TData2;
	pTb=m_PTATb;

	for(i=1;i<NORMALIZEDSIZE+1;i++)
	{
		for(j=1;j<NORMALIZEDSIZE+1;j++)
		{
			if(pOD[i][j]==0)
			{
				pTD1[i][j]=0;
				continue;
			}
			Neighbors=pOD[i-1][j]|(pOD[i-1][j+1]<<1)|
				(pOD[i][j+1]<<2)|(pOD[i+1][j+1]<<3)|
				(pOD[i+1][j]<<4)|(pOD[i+1][j-1]<<5)|
				(pOD[i][j-1]<<6)|(pOD[i-1][j-1]<<7);
			k=pTb[Neighbors];
			if(k<2)
			{
				pTD1[i][j]=(1-k);
				continue;
			}
			if(i>1)
			{
				NbN=pOD[i-2][j]|(pOD[i-2][j+1]<<1)|
					(pOD[i-1][j+1]<<2)|(pOD[i][j+1]<<3)|
					(pOD[i][j]<<4)|(pOD[i][j-1]<<5)|
					(pOD[i-1][j-1]<<6)|(pOD[i-2][j-1]<<7);
			}
			else
			{
				NbN=(pOD[i-1][j+1]<<2)|(pOD[i][j+1]<<3)|
					(pOD[i][j]<<4)|(pOD[i][j-1]<<5)|
					(pOD[i-1][j-1]<<6);
			}
			if(j>1)
			{
				NbW=pOD[i-1][j-1]|(pOD[i-1][j]<<1)|
					(pOD[i][j]<<2)|(pOD[i+1][j]<<3)|
					(pOD[i+1][j-1]<<4)|(pOD[i+1][j-2]<<5)|
					(pOD[i][j-2]<<6)|(pOD[i-1][j-2]<<7);
			}
			else
			{
				NbW=pOD[i-1][j-1]|(pOD[i-1][j]<<1)|
					(pOD[i][j]<<2)|(pOD[i+1][j]<<3)|
					(pOD[i+1][j-1]<<4);
			}
			switch(k)
			{
			case 2:
				if(pOD[i-1][j]==0)
				{
					pTD1[i][j]=1;
					continue;
				}
				//ASSERT(pOD[i-1][j]==1);
				if(pTb[NbN]==0)//North(p)不可删除
				{
					pTD1[i][j]=0;//删除 p
					continue;
				}
				else
				{
					pTD1[i][j]=1;//保留 p
					continue;
				}
				break;
			case 3:
				if(pOD[i][j-1]==0)
				{
					pTD1[i][j]=1;
					continue;
				}
				//ASSERT(pOD[i][j-1]==1);
				if(pTb[NbW]==0)//West(p)不可删除
				{
					pTD1[i][j]=0;//删除 p
					continue;
				}
				else
				{
					pTD1[i][j]=1;//保留 p
					continue;
				}
				break;
			case 4:
				if((pOD[i-1][j]==0)||(pOD[i][j-1]==0))
				{
					pTD1[i][j]=1;
					continue;
				}
				//ASSERT(pOD[i-1][j]==1);
				//ASSERT(pOD[i][j-1]==1);
				if((pTb[NbN]==0)&&(pTb[NbW]==0))//North(p)&West(p)均不可删
				{
					pTD1[i][j]=0;//删除 p
					continue;
				}
				else
				{
					pTD1[i][j]=1;//保留 p
					continue;
				}
				break;
			default:
				ASSERT(false);
			}
		}
		pTD1[i][0]=pTD1[i][NORMALIZEDSIZE+1]=0;
		pTD1[0][i]=pTD1[NORMALIZEDSIZE+1][i]=0;
		pTD2[i][0]=pTD2[i][NORMALIZEDSIZE+1]=0;
		pTD2[0][i]=pTD2[NORMALIZEDSIZE+1][i]=0;
	}
	pTD1[0][NORMALIZEDSIZE+1]=0;
	pTD1[0][0]=0;
	pTD1[NORMALIZEDSIZE+1][0]=0;
	pTD1[NORMALIZEDSIZE+1][NORMALIZEDSIZE+1]=0;
	
	pTD2[0][NORMALIZEDSIZE+1]=0;
	pTD2[0][0]=0;
	pTD2[NORMALIZEDSIZE+1][0]=0;
	pTD2[NORMALIZEDSIZE+1][NORMALIZEDSIZE+1]=0;

	Change=true;
	Count=2;
	for(;Change;)
	{
		Change=false;
		if((Count%2)==0)
		{
			pTD1=m_TData1;//输入
			pTD2=m_TData2;//输出
		}
		else
		{
			pTD1=m_TData2;//输入
			pTD2=m_TData1;//输出
		}
		for(i=1;i<NORMALIZEDSIZE+1;i++)
		{
			for(j=1;j<NORMALIZEDSIZE+1;j++)
			{
				if(pTD1[i][j]==0)
				{
					pTD2[i][j]=0;
					continue;
				}
				Tmp1=pTD1[i-1][j]|(pTD1[i-1][j+1]<<1)|
					(pTD1[i][j+1]<<2)|(pTD1[i+1][j+1]<<3);
				Tmp2=pTD1[i+1][j]|(pTD1[i+1][j-1]<<1)|
					(pTD1[i][j-1]<<2)|(pTD1[i-1][j-1]<<3);
				if((Count%2)==0)
					Neighbors=Tmp2|(Tmp1<<4);
				else
					Neighbors=Tmp1|(Tmp2<<4);
				k=pTb[Neighbors];
				switch(k)
				{
				case 0://不可删除
					pTD2[i][j]=1;
					break;
				case 1://可以删除
					pTD2[i][j]=0;
					Change=true;
					break;
				case 2://看看其北邻居是否可删,只有其北邻居不可删,当前点才可删
					if(pTD1[i-1][j]==0)
					{
						pTD2[i][j]=1;
						continue;
					}
					//ASSERT(pTD1[i-1][j]==1);
					if(i>1)
					{
						Tmp1=pTD1[i-2][j]|(pTD1[i-2][j+1]<<1)|
							(pTD1[i-1][j+1]<<2)|(pTD1[i][j+1]<<3);
						Tmp2=(pTD1[i][j])|(pTD1[i][j-1]<<1)|
							(pTD1[i-1][j-1]<<2)|(pTD1[i-2][j-1]<<3);
					}
					else
					{
						Tmp1=(pOD[i-1][j+1]<<2)|(pOD[i][j+1]<<3);
						Tmp2=(pOD[i][j])|(pOD[i][j-1]<<1)|
							(pOD[i-1][j-1]<<2);
					}
					if((Count%2)==0)
						NbN=Tmp2|(Tmp1<<4);
					else
						NbN=Tmp1|(Tmp2<<4);
					if(pTb[NbN]==0)//North(p)不可删
					{
						pTD2[i][j]=0;
						Change=true;
						break;
					}
					pTD2[i][j]=1;
					break;
				case 3://看看其西邻居是否可删,只有其西邻居不可删,当前点才可删
					if(pTD1[i][j-1]==0)
					{
						pTD2[i][j]=1;
						continue;
					}
					//ASSERT(pTD1[i][j-1]==1);
					if(j>1)
					{
						Tmp1=pTD1[i-1][j-1]|(pTD1[i-1][j]<<1)|
							(pTD1[i][j]<<2)|(pTD1[i+1][j]<<3);
						Tmp2=(pTD1[i+1][j-1])|(pTD1[i+1][j-2]<<1)|
							(pTD1[i][j-2]<<2)|(pTD1[i-1][j-2]<<3);
					}
					else
					{
						Tmp1=pTD1[i-1][j-1]|(pTD1[i-1][j]<<1)|
							(pTD1[i][j]<<2)|(pTD1[i+1][j]<<3);
						Tmp2=pTD1[i+1][j-1];
					}
					if((Count%2)==0)
						NbW=Tmp2|(Tmp1<<4);
					else
						NbW=Tmp1|(Tmp2<<4);
					if(pTb[NbW]==0)//West(p)不可删
					{
						pTD2[i][j]=0;
						Change=true;
						break;
					}
					pTD2[i][j]=1;
					break;
				case 4:
					if((pTD1[i][j-1]==0)||(pTD1[i-1][j]==0))
					{
						pTD2[i][j]=1;
						continue;
					}
					//ASSERT(pTD1[i][j-1]==1);
					//ASSERT(pTD1[i-1][j]==1);
					if(i>1)
					{
						Tmp1=pTD1[i-2][j]|(pTD1[i-2][j+1]<<1)|
							(pTD1[i-1][j+1]<<2)|(pTD1[i][j+1]<<3);
						Tmp2=(pTD1[i][j])|(pTD1[i][j-1]<<1)|
							(pTD1[i-1][j-1]<<2)|(pTD1[i-2][j-1]<<3);
					}
					else
					{
						Tmp1=(pOD[i-1][j+1]<<2)|(pOD[i][j+1]<<3);
						Tmp2=(pOD[i][j])|(pOD[i][j-1]<<1)|
							(pOD[i-1][j-1]<<2);
					}
					if((Count%2)==0)
						NbN=Tmp2|(Tmp1<<4);
					else
						NbN=Tmp1|(Tmp2<<4);
					if(j>1)
					{
						Tmp1=pTD1[i-1][j-1]|(pTD1[i-1][j]<<1)|
							(pTD1[i][j]<<2)|(pTD1[i+1][j]<<3);
						Tmp2=(pTD1[i+1][j-1])|(pTD1[i+1][j-2]<<1)|
							(pTD1[i][j-2]<<2)|(pTD1[i-1][j-2]<<3);
					}
					else
					{
						Tmp1=pTD1[i-1][j-1]|(pTD1[i-1][j]<<1)|
							(pTD1[i][j]<<2)|(pTD1[i+1][j]<<3);
						Tmp2=pTD1[i+1][j-1];
					}
					if((Count%2)==0)
						NbW=Tmp2|(Tmp1<<4);
					else
						NbW=Tmp1|(Tmp2<<4);
					if((pTb[NbN]==0)&&(pTb[NbW]==0))
					{
						pTD2[i][j]=0;
						Change=true;
						break;
					}
					pTD2[i][j]=1;
					break;
				default:
					ASSERT(false);
				}
			}
		}
		Count++;
	}
	m_pTD=pTD2;//保存最终的细化结果
	/*FILE *pFile;
	pFile=fopen("C:\\ThinChar.txt","w");
	for(i=0;i<NORMALIZEDSIZE+2;i++)
	{
		for(j=0;j<NORMALIZEDSIZE+2;j++)
		{
			if(pTD2[i][j]==0)
				fprintf(pFile," ");
			else
				fprintf(pFile,"1");
		}
		fprintf(pFile,"\n");
	}
	fclose(pFile);*/
}
