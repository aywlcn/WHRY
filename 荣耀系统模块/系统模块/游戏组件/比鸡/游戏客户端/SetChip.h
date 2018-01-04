#pragma once

#include "Stdafx.h"
#include "Resource.h"

//宏定义
#define SET_CHIP_COUNT					4			//押注档数

//游戏配置
class CSetChip : public CSkinDialog
{
	//变量定义
public:
	LONGLONG						m_nChip;							//压注大小
	LONGLONG						m_wMaxChip ;						//最高下注

	//控件变量
public:
	CSkinButton						m_btOK;								//确定按钮
	CSkinButton						m_btCancel;							//取消按钮

	//函数定义
public:
	//构造函数
	CSetChip();
	//析构函数
	virtual ~CSetChip();

	//重载函数
protected:
	//控件绑定
	virtual void DoDataExchange(CDataExchange * pDX);
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
//	virtual void OnOK();

	//设置函数
public:
	//初始押注
	void SetMaxChips( LONGLONG lMaxChip ) ;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

//////////////////////////////////////////////////////////////////////////
