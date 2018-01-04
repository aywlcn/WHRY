#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//游戏对话框
class CGameClientDlg : public CGameFrameEngine
{
	//控件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑
	CGameClientView					m_GameClientView;					//游戏视图
public:
	int m_nMiddleWidth;
	int m_nMiddleHeight;
	bool m_bOnOffStart;													///<是否发完牌开始游戏
	int                             m_nPlayer;                          ///<几个玩家
	bool							m_bOnTimeEvent;						///<事件
	bool							m_bSound;							///<背景音乐开关
	//配置变量
protected:
	bool							m_bBackGroundSound;					//背景声音
	DWORD							m_dwCardHSpace;						//扑克象素
	int                             m_nCurrentUser;                     //
	CMD_S_GameEnd                   m_ppBuffer;                        //
	CMD_S_SendCard                  m_Buffer;                          //
	WORD                            m_wDataSize;                        //
	BYTE                            m_btCardType[GAME_PLAYER][3];        //玩家牌的类型
	
	int                             m_nWhichCard;                        //第几敦
	bool                            m_bPlayer[4];                        //玩家是否有
	bool                            m_bDraw;                             //
	bool                            m_bSpecialType[GAME_PLAYER];         //是否有特殊牌型
	int                             m_nSpecialType;                      //特殊牌型的个数
	bool                            m_bShowCard;                         //是否摊牌
	bool                            m_bStrat;                            //开始
	bool                            m_bCompareCard;                      //是否比牌
	bool                            m_bDragon[GAME_PLAYER];              //是否乌龙
	bool							bFirst;

	


	//游戏变量
protected:
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//手上扑克
	BYTE                            m_bAllHandCardData[GAME_PLAYER][HAND_CARD_COUNT];//所有玩家的扑克
	BYTE                            m_bSpecialCardData[GAME_PLAYER][HAND_CARD_COUNT];//
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//备份扑克
	BYTE							m_bHandCardCount ;					//扑克数目
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	UINT_PTR						m_nTimer ;							//定时器ID
	LONG							m_wUserToltalChip;					//总共金币
	bool							m_bFinishSegment;					//完成分段
	bool							m_bReady;							//是否准备
	LONG							m_lGameScore[GAME_PLAYER];			//玩家积分
	BYTE							m_bOverTime[GAME_PLAYER];			//摊牌超时

	tagAnalyseType                  m_bType;                            //类型及组合
	bool                            m_bbThreeCard[3];                   //三墩有无触发
	
public:
	//CDSBuffer						*m_pSoundBackGround;                                   ///<背景声音指针
	//CGameSounds						*m_pSoundBackGround;
	//CMultiAudioPlay *
	//函数定义
public:
	//构造函数
	CGameClientDlg();
	//析构函数
	virtual ~CGameClientDlg();

	//消息处理
protected:
	//发送扑克
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//游戏结束
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//设置分段
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize);
    //玩家摊牌
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//玩家强退
	bool OnSubPlayerLeft(const void * pBuffer , WORD wDataSize) ;
	//控制提示
	bool OnControlPrompt(const void * pBuffer, WORD wDataSize);
    
    //辅助函数
protected:
	//自动代打
	void AutomaticOutCard() ;
	//分段信息
	bool ShowSegInfo() ;
	//设置按钮
	void SetThreeCard();
	//比牌
	bool ComparePlayerCard(const void * pBuffer, WORD wDataSize);
	BYTE GetCardType(int Player,int WhichCard,bool bFrontCard,bool bMidCard);
	int GetCardType(BYTE BCardType,bool bFrontCard,bool bMidCard);
	void GetSpecialCard(BYTE CardData[],BYTE CardCount);
	void ResetDlgData();

	//常规继承
private:
	//初始函数
	virtual bool OnInitGameEngine();
	//重置框架
	virtual bool OnResetGameEngine();
	//游戏设置
	virtual void OnGameOptionSet();

	//时钟事件
public:
	//时钟删除
	virtual bool OnEventGameClockKill(WORD wChairID);
	//时间消息
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//游戏事件
public:
	//旁观状态
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//网络消息
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//游戏场景
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//声音控制
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//消息函数
protected:
	//开始消息
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//结束消息
	LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	//左键扑克
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//设置前墩
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//设置中墩
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//设置后墩
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//玩家摊牌
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//降序排列
	LRESULT	OnDescendSort(WPARAM wParam, LPARAM lParam) ;	
	//升序排列
	LRESULT	OnAscendSort(WPARAM wParam, LPARAM lParam) ;	
	//花色排列
	LRESULT	OnColorSort(WPARAM wParam, LPARAM lParam) ;	
	//自动代打
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//恢复扑克
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam) ;
	//一对
	LRESULT OnOnePareCard(WPARAM wParam, LPARAM lParam );
	//两对
	LRESULT OnTwoPareCard(WPARAM wParam, LPARAM lParam );
	//三条
	LRESULT OnThreeSameCard(WPARAM wParam, LPARAM lParam );
	//顺子
	LRESULT OnStraightCard(WPARAM wParam, LPARAM lParam );
	//同花
	LRESULT OnFlushCard(WPARAM wParam, LPARAM lParam );
	//葫芦
	LRESULT OnGourdCard(WPARAM wParam, LPARAM lParam );
	//铁支
	LRESULT OnFourSameCard(WPARAM wParam, LPARAM lParam );
	//同花顺
	LRESULT OnStraightFlushCard(WPARAM wParam, LPARAM lParam );
	//回收第一墩
	LRESULT OnRecycleFrontCard(WPARAM wParam, LPARAM lParam);
	//回收第二墩
	LRESULT OnRecycleMidCard(WPARAM wParam, LPARAM lParam);
	//回收第三墩
	LRESULT OnRecycleBackCard(WPARAM wParam, LPARAM lParam);
	//回收全部
	LRESULT OnRecycleAllCard(WPARAM wParam, LPARAM lParam);
	//确定特殊牌
	LRESULT OnSubmit(WPARAM wParam, LPARAM lParam);
	//取消特殊牌
	LRESULT OnCancel(WPARAM wParam, LPARAM lParam);
	//管理员请求
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);
   
public:
	afx_msg void OnTimer(UINT nIDEvent);
	//新引擎
	LRESULT OnOperateMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////
