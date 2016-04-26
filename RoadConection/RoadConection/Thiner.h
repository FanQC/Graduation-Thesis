// Thiner.h: interface for the CThiner class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THINER_H__85ADF0DC_AB7B_4E4D_9176_05D480CB94B8__INCLUDED_)
#define AFX_THINER_H__85ADF0DC_AB7B_4E4D_9176_05D480CB94B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "global.h"
//文字图像的细化
//参考 A Parallel Thinning Algorithm with Two-Subiteration
//     that Generates One-Pixel-Wide Skeletons.
//作者 Y.Y.Zhang & P.S.P. Wang
// Proceedings of ICPR'96
class CThiner  
{
public:
	void PTATA();
	void GHTA();
	void LoadData(unsigned char (*pData)[NORMALIZEDSIZE+2]);
	CThiner();
	virtual ~CThiner();
	unsigned char (*m_pTD)[NORMALIZEDSIZE+2];
private:
	void CreateGHTbl();
	//原始的未细化的文字图像,外边框象素点值均为 0(背景点)
	//细化后其内容不变(但要改变m_TData1/m_TData2)
	unsigned char (*m_pData)[NORMALIZEDSIZE+2];
	//用于保存细化结果
	unsigned char m_TData1[NORMALIZEDSIZE+2][NORMALIZEDSIZE+2];
	unsigned char m_TData2[NORMALIZEDSIZE+2][NORMALIZEDSIZE+2];

	int m_GHTbOdd[256];
	int m_GHTbEvn[256];

	static int m_PTATb[256];
};

#endif // !defined(AFX_THINER_H__85ADF0DC_AB7B_4E4D_9176_05D480CB94B8__INCLUDED_)
