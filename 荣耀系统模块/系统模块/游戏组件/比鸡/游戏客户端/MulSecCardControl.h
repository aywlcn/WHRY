#ifndef MUL_SECTION_CARD_CONTROL_HEAD_FILE
#define MUL_SECTION_CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////

//扑克定义
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K

//////////////////////////////////////////////////////////////////////////
//宏定义
#define MSCC_SECTION_NUM			3
#define MSCC_SECTION_ONE			3
#define MSCC_SECTION_TWO			5
#define MSCC_SECTION_THREE			5

//扑克控件
class CMulSecCardControl : public CWnd
{
	//配置变量
protected:
	bool							m_bDisplay;							//显示标志

	//间隔变量
public:
	DWORD							m_dwCardHSpace;						//横向间隔
	DWORD							m_dwCardVSpace;						//竖向间隔

	//位置变量
protected:
	CPoint							m_BenchmarkPos;						//基准位置（扑克牌控件相对桌面的位置）
	enXCollocateMode				m_XCollocateMode;					//显示模式
	enYCollocateMode				m_YCollocateMode;					//显示模式

	//分段变量
protected:
	BYTE							m_bSecCount;						//分段个数
	BYTE							m_bSecArray[MSCC_SECTION_NUM];		//分段数组	

	//运行变量
protected:
	DWORD							m_dwCurrentIndex;					//点击索引

	//内部数据
protected:
	CRgn							m_CardRegion;						//控件区域
	CWnd *							m_pSinkWindow;						//回调窗口
	CWHArray<tagCardItem>		m_CardDataItem;						//扑克数据

	//资源变量
public:
	static bool						m_bLoad;							//加载标志
	static CSize					m_CardSize;							//扑克大小
	static CBitImage				m_ImageCard;						//图片资源

	//函数定义
public:
	//构造函数
	CMulSecCardControl();
	//析构函数
	virtual ~CMulSecCardControl();

	//配置函数
public:
	//设置窗口
	void SetSinkWindow(CWnd * pSinkWindow);

	//控件控制
public:
	//设置显示
	void SetDisplayFlag(bool bDisplay);
	//设置间距
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace);
	//基准位置
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//基准位置
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//扑克控制
public:
	//获取数目
	DWORD GetCardCount();
	//设置扑克
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//获取扑克
	DWORD GetCardData(BYTE bCardData[], DWORD dwMaxCount);
	//获取扑克
	DWORD GetCardData(BYTE bCardData);
	//设置扑克
	bool SetCardItem(BYTE bCardData, DWORD dwIndex);
	//设置扑克
	DWORD SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount);

	//内部函数
private:
	//调整位置
	void RectifyControl();
	//索引切换
	DWORD SwitchCardPoint(const CPoint & MousePoint);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//建立消息
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//鼠标消息
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//光标消息
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
