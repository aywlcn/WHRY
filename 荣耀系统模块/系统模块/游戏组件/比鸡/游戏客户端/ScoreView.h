#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//积分视图类
class CScoreView : public CDialog
{
	//变量定义
protected:
	LONG							m_lGameTax[GAME_PLAYER];		    //游戏税收
	LONG							m_lGameScore[GAME_PLAYER];			//游戏得分
	TCHAR							m_szUserName[GAME_PLAYER][MAX_PATH];//用户名字
	int                             m_nTotalDaoShu[GAME_PLAYER];        //总共道数
	bool                            m_bEnd;                             //
	int                             m_nChairWin;                        //
	int                             m_nChairLoss;                       //

	//资源变量
protected:
	CBitImage											m_ImageBack;						//背景图案
	CPngImage                                           m_ImageStart;                       //开始
	CPngImage											m_ImageEnd;                         //结束

public:
   CSkinButton                      m_pClose;                             
   CSkinButton                      m_btStartAgain;                     //重新开始
   CSkinButton                      m_btEndAgain;                       //结束
   CSkinResourceManager *           m_pSkinResourceManager;             //资源管理器

   int								m_nCompare[GAME_PLAYER][3];         //每一敦的总道数
   int								m_nCompareDouble[GAME_PLAYER];      //翻倍的道数
   int								m_nCompareSpecial[GAME_PLAYER];     //特殊牌型的道数
   int								m_cCompareKillThree[GAME_PLAYER];   //<全垒打

	//函数定义
public:
	//构造函数
	CScoreView(void);
	//析构函数
	virtual ~CScoreView(void);
	
	//重载函数
public:
	//初始化函数
	virtual BOOL OnInitDialog();
	//确定消息
	virtual void OnOK() { return; }
	//取消消息
	virtual void OnCancel() { return; }

	

	//功能函数
public:
	//重置积分
	void ResetScore();
	//设置税收
	void SetGameTax(LONG lGameEveryTax[]);
	//设置积分
	void SetGameScore(WORD wChairID,LPCTSTR pszUserName, LONG lScore,int nTotalDaoShu);
	void SetEnd(bool bEnd);

	//消息函数
protected:
	//重画函数
	afx_msg void OnPaint();
	//绘画背景
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//鼠标消息
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//关闭消息
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//////////////////////////////////////////////////////////////////////////

#endif