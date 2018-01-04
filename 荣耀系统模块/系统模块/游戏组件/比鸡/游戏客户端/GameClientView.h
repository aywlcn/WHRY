#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GameLogic.h"
#include "MulSecCardControl.h"
#include "ScoreView.h"
#include "MysteryDlg.h"
#include "ClientControl.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//消息定义
#define IDM_START					(WM_APP+100)						//开始消息
#define IDM_END                     (WM_APP+101)                        //结束消息
#define IDM_OK				        (WM_APP+102)						//摊牌消息
#define IDM_FRONTCARD				(WM_APP+103)						//设置前墩  
#define IDM_MIDCARD					(WM_APP+104)						//设置中墩
#define IDM_BACKCARD				(WM_APP+105)						//设置后墩
#define IDM_DESCEND_SORT			(WM_APP+106)						//降序消息
#define IDM_ASCEND_SORT				(WM_APP+107)						//升序消息
#define IDM_COLOR_SORT				(WM_APP+108)						//花色消息
#define IDM_AUTO_OUT_CARD           (WM_APP+109)                        //自动代打
#define IDM_RESTORE_CARD			(WM_APP+110)                        //恢复扑克

#define IDM_ONEPARE                 (WM_APP+111)                        //一对
#define IDM_TWOPARE                 (WM_APP+112)                        //两对
#define IDM_THREESAME               (WM_APP+113)                        //三条
#define IDM_STRAIGHT                (WM_APP+114)                        //顺子
#define IDM_FLUSH                   (WM_APP+115)                        //同花
#define IDM_GOURD                   (WM_APP+116)                        //葫芦
#define IDM_FOURSAME                (WM_APP+117)                        //铁支
#define IDM_STRAIGHTFLUSH           (WM_APP+118)                        //同花顺

#define IDM_RECYCLEFRONTCARD        (WM_APP+119)                        //回收第一墩
#define IDM_RECYCLEMIDCARD          (WM_APP+120)                        //回收第二墩
#define IDM_RECYCLEBACKCARD         (WM_APP+121)                        //回收第三墩
#define IDM_RECYCLEALLCARD          (WM_APP+122)                        //回收全部
#define IDM_PAIXU					(WM_APP+123)						//排序
#define IDM_SUBMIT					(WM_APP+124)                        //确定特殊牌消息
#define IDM_CANCEL					(WM_APP+125)                        //取消特殊牌消息

////////////////////////////////////////////////////////////////////////////////////////////////

//扑克类型
enum enCardType
{
	enHandCard,						//手上扑克
	enSegmentCard,					//分段扑克
	enAllSegmentCard,				//所有分段
	enCompareCard,					//比较扑克
	enBankCard,						//庄家扑克
	enErrorType,					//错误类型
};

//游戏视图
class CGameClientView : public CGameFrameViewGDI
{
	//界面变量
protected:
	CString							m_strShowCompare;					//显示比较
	bool							m_bShowCompare;						//显示比较
	enSegmentType					m_enSegmentType ;					//比较类型
	CPoint							m_UserAddrInfo[GAME_PLAYER];		//地址信息
	bool                            m_bEnd;                             //是否结束
	bool                            m_bStart;                           //是否开始
	bool                            m_bCompare;                         //是否开始比牌
	int                             m_nXPos;                            //
	int                             m_nYPos;                            //
	int                             m_nCardCount;                       //
	BYTE                            m_btCompareCardData[5];             //
	CSize                           m_SBigCardSize;                     //大扑克的大小
	bool                            m_bDrawTimer;                       //
	bool                            m_bDrawFrontCardType;               //
	bool                            m_bDrawMidCardType;                 //
	bool                            m_bDrawBackCardType;                //
	int                             m_nFrontCardType;                   //
	int                             m_nMidCardType;                     //
	int                             m_nBackCardType;                    //
	CGameLogic						m_GameLogic;						//游戏逻辑
protected:
	CMD_S_GameEnd                   m_nSGameEnd;                        //
	bool                            m_bDrawPlayer1[5];                  //
	bool                            m_bDrawPlayer2[5];                  //
	bool                            m_bDrawPlayer3[5];                  //
	bool                            m_bDrawPlayer4[5];                  //
	int                             m_nDrawPlayer[GAME_PLAYER];         //
	bool                            m_bPlayer[4];                       //
	int                             m_nCurrentUser;                     //
	LONG                            m_LAllScore[4];                     //累计分数
	LONG                            m_LAllTax[4];                       //累计税收
	bool                            m_bDrawSpecialType[GAME_PLAYER];
	bool                            m_bDrawSpecialCard;                 //是否画特殊牌型分数
    bool                            m_bSendCardAction;                  //是否是发牌动作
	int                             m_nXpoint[4];                       //
	int                             m_nYpoint[4];                       //
	int                             m_nCardSpace[4];                    //
	int                             m_nWinPlayer;                       //第一的玩家
	int                             m_nLosePlayer;                      //最后的玩家    
	bool                            m_bDrawFire;                        //画打枪
	int                             m_nFireGunStatus;                   //打枪12种情况
	bool                            m_bDrawViewCard;                    //
	BYTE                            m_ViewSegmentCard[GAME_PLAYER][3][5];//
	bool                            m_bDrawFireCourse[5]                ;
	int                             m_nDrawFireWhichFrame;              ;//画打枪哪一帧
	CPoint                          m_UserName[4];                       //
    int                             m_nChairWin;                         //
	int                             m_nChairLoss;                        //
	bool                            m_bDragon[GAME_PLAYER];              //倒水
	CPoint							m_HuaXian;

	CPoint							m_TishiFramePoint;					//特殊牌提示底框位置
	CPoint                          m_ThanCardPoint[4];                //比牌位置
    CPoint                          m_ReadyPoint[4];                     //准备位置
	CPoint                          m_BoyPoint[4];                       //难头像位置
	CPoint                          m_GirlPoint[4];                      //女头像位置
	CPoint                          m_FacePoint[4];                      //头像位置
	CPoint                          m_DragonPoint[GAME_PLAYER];          //倒水位置
	CPoint                          m_DrawCardKuang1[GAME_PLAYER];		//特殊牌框位置
	CPoint                          m_FourbaggerPoint[GAME_PLAYER];		//特殊牌框位置
    
	int                             m_nWidth;                            //宽
	int                             m_nHeight;                           //高
	int								m_iPaiKuangZS;						 //牌框闪现帧数

	bool                            m_bClockPoint;                       //是否把时钟画在右下角
	bool                            m_bDrawMyScore;                      //是否画自己的分数
	bool                            m_bShowCard;                         //自己是否摊牌
	WORD                            m_wMeChairID;                        //自己椅子ID

	 

	//状态变量
public:
	WORD							m_wAllUser;							//全垒打用户
	CHAR							m_szSegInfo[4][32];					//分段信息
	TCHAR                           m_szUserNameInfo[4][32];            //用户名称
	LONG                            m_szPassScoreInfo[4];				//上一局成绩
	LONG                            m_szTaxInfo[4];						//税收
	int								m_szbToltalWinDaoShu[GAME_PLAYER];	//水数
	BYTE							m_nFourbaggerFrames;				//全垒打帧数
	bool							m_bFourbagger;						//是否全垒打
	bool							m_bShowFourbagger;					//是否显示全垒打
	bool							m_bTishi;							//是否提示特殊牌 
	bool							m_bCompareAllCard;					//是否直接比全部牌
	bool							m_bDrawCardKuang[GAME_PLAYER];		 //是否画特殊牌底框
	bool                            m_bSpecialTypeview[GAME_PLAYER];    
	bool							m_bShowBorder;						 //是否显示边框
	bool                            m_bDrawHandCard;                     //是否画手中的牌
	bool							m_bDrawShuffle;						///<是否画洗牌 
	bool							m_bOnOffThanCard[GAME_PLAYER];		///<是否画比牌 
	bool							m_bOnOffTishikuang;					///<提示框开关
	bool							m_bOnOffBackAdornment;				///<背景装饰 
	bool                            m_bDrawScore;                       //是否画比较的分数 
	int								m_nShootNum;						///<播放哪个用户打枪
	int								m_nShootState[6][2];				///<打枪状态
	BYTE							m_nSpecialType;						//特殊牌类型
	SCORE							m_lCellScore;						//单元底分
	//CStringArray					Str;

	//字体资源
public:
	CDFontEx						m_Font;								//字体
	
	//控件变量
	//按钮控件
public:
	CSkinButton						    m_btStart;							//开始按钮
	CSkinButton                         m_btEnd;                            //结束按钮
	CSkinButton	                        m_btFrontCard;						//前墩按钮
	CSkinButton							m_btMidCard;						//中墩按钮
	CSkinButton							m_btBackCard;						//后墩按钮

	CSkinButton							m_btDescend;						//降序按钮
	CSkinButton							m_btAscend;							//升序按钮
	CSkinButton							m_btColor;							//花色按钮
	CSkinButton							m_btAutoOutCard;                    //自动代打
	CSkinButton						    m_btRestoreCard;					//恢复扑克

	CSkinButton                         m_btRecycleCard1;                   //回收第一墩         
	CSkinButton                         m_btRecycleCard2;                   //回收第二墩
	CSkinButton                         m_btRecycleCard3;                   //回收第三墩
	CSkinButton                         m_btRecycleAllCard;                 //回收全部
	CSkinButton                         m_btOk;                             //确定

	CSkinButton                         m_btOnePare;                        //一对
	CSkinButton                         m_btTwoPare;                        //两对
	CSkinButton                         m_btThreeSame;                      //三条
	CSkinButton                         m_btStraight;                       //顺子
	CSkinButton                         m_btFlush;                          //同花
	CSkinButton                         m_btGourd;                          //葫芦
	CSkinButton                         m_btFourSame;                       //铁支
	CSkinButton                         m_btStraightFlush;                  //同花顺
	CSkinButton							m_btPaiXu;							//排序
	CSkinButton							m_btSubmit;							//特殊牌确定
	CSkinButton							m_btCancel;							//特殊牌取消
	CButton							m_btOpenAdmin;

	//扑克控件
public:
	CCardControl					m_UserSendCard0;		//用户发牌
	CCardControl					m_UserSendCard[GAME_PLAYER];		//用户发牌
	CCardControl					m_UserShuffleCard[GAME_PLAYER];		//洗牌
	CCardControl					m_UserCardControl[GAME_PLAYER];		//手上扑克
	CCardControl					m_AllSegCardControl[GAME_PLAYER][3];//三个分段
	CCardControl					m_SegCardControl[GAME_PLAYER][3];	//分段扑克
	//CCardControl					m_HandSegCardControl[3];			//手上分段
	CCardControl					m_CompareCarControl[GAME_PLAYER];	//对比扑克
	CCardControl				    m_OverTimeCardControl[GAME_PLAYER]; //超时扑克
	CCardControl                    m_CardType[GAME_PLAYER];            //
	CCardControl                    m_DrawGun;                          //


	//位图变量

public:
	///ZBD
// 	CPngImage                                       m_ImagezsFlower;                   //左上花朵
// 	CPngImage                                       m_ImagezxFlower;                   //左下花朵
// 	CPngImage                                       m_ImageysFlower;                   //右上花朵
// 	CPngImage                                       m_ImageyxFlower;                   //右下花朵
	CPngImage                                       m_ImageDragon;                     //倒水
	CPngImage                                       m_ImageReady;                      //准备资源
	CPngImage                                       m_ImageViewFill;                   //填充资源
	CPngImage									    m_ImageScoreFrame;					//积分榜背景
	CPngImage			                            m_ImageCenter;						//背景资源
	CPngImage										m_ImageClock;						//闹钟
	CPngImage										m_ImageTimer;		                //时间
	CPngImage	                                    m_ITishikuang;                     //提示框
	CPngImage                                       m_ICardFrame;                      //牌底框 
	CPngImage                                       m_ImageCardType;					//牌类型
	CPngImage                                       m_CardSpecialType;					//牌类型
	CPngImage                                       m_EndNumSmall;						//几水数字（小）
	CPngImage                                       m_EndNumBig;						//几水数字（大）
	CPngImage										m_ImageGun1;		                //打枪1
	CPngImage										m_ImageGun2;		                //打枪2
	//CPngImage                                     m_IFire;							//子弹
	CPngImage                                       m_ICrash;							//爆炸
	CPngImage										m_ImageCard;						//图片资源
	CPngImage										m_ImageBigCard;                     //大图片资源
	CPngImage                                       m_ISendCard;						//发牌
	CPngImage                                       m_IShuffleCard ;					//洗牌
	CPngImage                                       m_IThanCard;						//比牌
	CPngImage                                       m_SpecCardFlash;					//特殊牌型底框
	CPngImage                                       m_ScoreFrame;						//底分框
	CPngImage                                       m_TishiFrame;						//特殊牌提示框
	CPngImage                                       m_FourbaggerFlash;					//全垒打动画
	CPngImage                                       m_Fourbagger;						//全垒打

	CPngImage                                       m_IPlusBigNumber;                  //加分大数字
	CPngImage                                       m_IPlusSmallNumber;                //加分小数字
	CPngImage                                       m_IBigSubtractionSign;             //大减号
	CPngImage                                       m_IBigPlusSign;                    //大加号
	CPngImage                                       m_ISmallSubtractionSign;           //小减号
	CPngImage                                       m_ISmallPlusSign;                  //小加号

	CPngImage                                       m_IBackAdornment1;					//背景装饰图1



	//控件组件
public:
	CScoreView						                m_ScoreView;						//积分视图
	CMysteryDlg										m_MysteryDlg;						//控制窗口
	CClientControl									m_ClientControl;					//客户端控制

	//函数定义
public:
	//构造函数
	CGameClientView();
	//析构函数
	virtual ~CGameClientView();
public:
	//更新视图
	void RefreshGameView();
	//继承函数
private:
	//重置界面
	virtual VOID ResetGameView();
	//调整控件
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//绘制界面
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7支持
	virtual bool RealizeWIN7() { return true; }

	//辅助函数
protected:
	//艺术字体
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//绘画字符
	void DrawTextString(CDC * pDC, CDFontEx* pFont, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat);

	//功能函数
public:
	//设置扑克
	void SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg=255) ;
	//显示比较
	void SetShowCompare(bool bShow , const enSegmentType SegmentType=enErr) ;
	//分段信息
	void SetDrawThanCard(bool bThanCard);
	bool GetDrawThanCard();
	void ShowSegInfo(enSegmentType seg, CHAR const szInfo[32]);	
	void SetEnd(bool bEnd);
	void SetStart(bool bStart);
	void SetCompare(bool bCompare);
	void SetCompareCard(int nXPos,int nYPos,BYTE btCompareCardData[],int nCardCount);
	void SetbDrawTimer(bool bDrawTimer);
	void SetbDrawFrontCardType(bool bDraw,int CardType);
	void SetbDrawMidCardType(bool bDraw,int CardType);
    void SetbDrawBackCardType(bool bDraw,int CardType);
	void SetSGameEnd(CMD_S_GameEnd SGameEnd);
	void SetbDrawPlayer(bool bDrawPlayer1[],bool bDrawPlayer2[],bool bDrawPlayer3[],bool bDrawPlayer4[]);
	void SetnDrawPlayer(int nDrawPlayer[]);
	void SetbPlayer(bool bPlayer[]);
	void SetnCurrentUser(int nCurrentUser);
	int GetViewChairID(int nCurrentUser);
	void GetAllUserScoreInfo(LONG szPassScoreInfo[],LONG szTaxInfo[],int szbToltalWinDaoShu[],bool bUserLeft[]);
	void GetUserNameInfo(TCHAR szUserNameInfo1[],TCHAR szUserNameInfo2[],TCHAR szUserNameInfo3[],TCHAR szUserNameInfo4[]);
    void GetbSpecialType(bool bSpecialType[]);
	void GetbDrawSpecial(bool bDrawSpecialCard);
	void SetbDrawScore(bool bDrawScore);
	void SetbSendAction(bool bSendAction);
	void SetnPoint(int xPoint[],int yPoint[]);
	void SetnCardSpace(int nCardSpace[]);
	void SetCardInfo(int xPoint[],int yPoint[],int nCardSpace[]);
	void SetbDrawFire(bool bDrawFire);
	void SetFireGunStatus(int nFireGunStatus);
	void SetbDrawViewCard(bool bDrawViewCard,BYTE cbSegmentCard11[],BYTE cbSegmentCard12[],BYTE cbSegmentCard13[],BYTE cbSegmentCard21[],
		                  BYTE cbSegmentCard22[],BYTE cbSegmentCard23[],BYTE cbSegmentCard31[],BYTE cbSegmentCard32[],BYTE cbSegmentCard33[],
		                  BYTE cbSegmentCard41[],BYTE cbSegmentCard42[],BYTE cbSegmentCard43[]);
	void SetnDrawFireWhichFrame(int nDrawFireWhichFrame);
	void SetUser2CardSpace(int nUser2CardSpace);
	void GetUserWinOrLoss(int &nWinUser,int &nLossUser);
	void SetDragon(int wViewChairID,bool bDragon);
	void CleanUserScoreInfo(int nUserChairID);
	void SetbClockPoint(bool bClcokPoint);
	void SetbDrawMyScore(bool bDrawMyScore);
	void SetbShowCard(bool bShowCard);
	void SetMeChairID(WORD wMeChairID);
public:
	void ResetViewData(bool IsLookon);
	void HideButton();
	void DrawFrontCardType( CDC* pDC ,int nXPos,int nYPos );
    void DrawMidCardType(CDC* pDC ,int nXPos,int nYPos);
	void DrawBackCardType(CDC* pDC ,int nXPos,int nYPos);
	void DrawCompareScore(CDC* pDC ,int nXPos,int nYPos);
	void DrawUserInfo(CDC* pDC ,int nXPos,int nYPos);
	void DrawFire(CDC* pDC ,int nXPos,int nYPos);
	void DrawReady(CDC* pDC ,int nXPos,int nYPos);
	void DrawThanCard( CDC* pDC,int nXPos,int nYPos); ///<画比牌
	void DrawBackAdornment1( CDC* pDC,int nXPos,int nYPos );///<画背景装饰物1
	
	void DrawUserClock(CDC* pDC ,int nXPos,int nYPos);
	void DrawUserTimer(CDC* pDC , int nXPos, int nYPos, WORD wTime, WORD wTimerArea/* =99 */);

	//消息映射
protected:

	//建立函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//新引擎
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//开始按钮
	afx_msg void OnStart() ;
	//设置前墩
	afx_msg void OnFrontCard();
	//设置中墩
	afx_msg void OnMidCard();
	//设置后墩
	afx_msg void OnBackCard();
	//确定特殊牌
	afx_msg void OnSubmit();
	//取消特殊牌
	afx_msg void OnCancel();
	//摊牌
	afx_msg void OnShowCard();
	//回收牌1
	afx_msg void OnRecyeleCard1();
	//回收牌2
	afx_msg void OnRecyeleCard2();
	//回收牌3
	afx_msg void OnRecyeleCard3();
	//回收全部牌
	afx_msg void OnRecyeleAllCard();
	//一对
	afx_msg void OnOnePare();
	//两对
	afx_msg void OnTwoPare();
	//三条
	afx_msg void OnThreeSame();
	//顺子
	afx_msg void OnStraight();
	//同花
	afx_msg void OnFlush();
	//葫芦
	afx_msg void OnGord();
	//铁支
	afx_msg void OnFourSamw();
	//同花顺
	afx_msg void OnStraightFlush();
	//排序
	afx_msg void OnTaxis();
	//管理员控制
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
