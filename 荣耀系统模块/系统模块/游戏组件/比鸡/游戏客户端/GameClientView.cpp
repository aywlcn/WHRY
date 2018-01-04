#include "StdAfx.h"
#include "Resource.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////
//排序类型
#define DESCEND_SORT						0								//降序
#define ASCEND_SORT							1								//升序
#define COLOR_SORT							2								//花色

//按钮标识 
#define IDC_OK						        100								//摊牌按钮
#define IDC_FRONT_CARD						101								//设置前墩
#define IDC_MID_CARD						102								//设置中墩
#define IDC_BACK_CARD						103								//设置后墩
#define	IDC_START							104								//开始按钮
#define IDC_SET_CHIP						105								//压注按钮
#define IDC_DESCEND_SORT					106								//降序按钮
#define IDC_ASCEND_SORT						107								//升序按钮
#define IDC_COLOR_SORT						108								//花色按钮
#define IDC_AUTO_OUT_CARD				    109                             //自动代打
#define IDC_CANCEL_AUTO_OUT_CARD			110                             //取消代打
#define IDC_RESTORE_CARD					111								//包牌按钮
//#define IDC_STARTAGAIN                      112                           //重新开始
#define IDC_END                             113                             //游戏结束

#define IDC_ONEPARE                         114                             //一对
#define IDC_TWOPARE                         115                             //两对
#define IDC_THREESAME                       116                             //三条
#define IDC_STRAIGHT                        117                             //顺子
#define IDC_FLUSH                           118                             //同花
#define IDC_GOURD                           119                             //葫芦
#define IDC_FOURSAME                        120                             //铁支
#define IDC_STRAIGHTFLUSH                   121                             //同花顺
#define IDC_TAXIS							122                             //排序

#define IDC_RECYCLECARD1                    123                             //回收第一墩
#define IDC_RECYCLECARD2                    124                             //回收第二墩
#define IDC_RECYCLECARD3                    125                             //回收第三墩
#define IDC_RECYCLECARDALL                  126                             //回收全部
#define IDC_SUBMIT                          127                             //确定特殊牌消息
#define IDC_CANCEL                          128                             //取消特殊牌消息

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientView, CGameFrameViewGDI)
	ON_WM_CREATE()
    ON_WM_TIMER()
 	ON_WM_LBUTTONDOWN()
 	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_FRONT_CARD, OnFrontCard)
	ON_BN_CLICKED(IDC_MID_CARD, OnMidCard)
	ON_BN_CLICKED(IDC_BACK_CARD, OnBackCard)
	ON_BN_CLICKED(IDC_OK, OnShowCard)
	ON_BN_CLICKED(IDC_SUBMIT, OnSubmit)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_RECYCLECARD1, OnRecyeleCard1)
	ON_BN_CLICKED(IDC_RECYCLECARD2, OnRecyeleCard2)
	ON_BN_CLICKED(IDC_RECYCLECARD3, OnRecyeleCard3)
	ON_BN_CLICKED(IDC_RECYCLECARDALL, OnRecyeleAllCard)
	ON_BN_CLICKED(IDC_ONEPARE, OnOnePare)
	ON_BN_CLICKED(IDC_TWOPARE, OnTwoPare)
	ON_BN_CLICKED(IDC_THREESAME, OnThreeSame)
	ON_BN_CLICKED(IDC_STRAIGHT, OnStraight)
	ON_BN_CLICKED(IDC_FLUSH, OnFlush)
	ON_BN_CLICKED(IDC_GOURD, OnGord)
	ON_BN_CLICKED(IDC_FOURSAME, OnFourSamw)
	ON_BN_CLICKED(IDC_STRAIGHTFLUSH, OnStraightFlush)
	ON_BN_CLICKED(IDC_TAXIS, OnTaxis)
	ON_BN_CLICKED(IDC_BT_ADMIN,OpenAdminWnd)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////


//构造函数
CGameClientView::CGameClientView(void)
{
	//加载资源
	HINSTANCE hInstance=AfxGetInstanceHandle();	

	m_ImageCenter.LoadImage(hInstance,TEXT("IDB_VIEW_CENTER"));
	m_ImageViewFill.LoadImage(hInstance,TEXT("IDB_VIEW_BACK"));
	m_ImageClock.LoadImage(hInstance,TEXT("IDB_VIEW_CLOCK"));
	m_ImageTimer.LoadImage(hInstance,TEXT("IDB_VIEW_TIMER"));
	m_ImageReady.LoadImage(hInstance,TEXT("IDB_VIEW_READY"));
	m_IThanCard.LoadImage(hInstance,TEXT("IDB_VIEW_READY_CARD"));
	m_ImageDragon.LoadImage(hInstance,TEXT("IDB_VIEW_DAOSHUI"));
	m_ImageScoreFrame.LoadImage(hInstance,TEXT("IDB_VIEW_SCORE_FRAME"));
	m_ITishikuang.LoadImage(hInstance,TEXT("IDB_VIEW_BTHBACK"));
	m_ICardFrame.LoadImage(hInstance,TEXT("IDB_VIEW_CARD_FRAME"));
	m_ImageCardType.LoadImage(hInstance,TEXT("IDB_CARD_TYPE"));
	m_CardSpecialType.LoadImage(hInstance,TEXT("IDB_SPECIAL_CARD_TYPE1"));
	m_EndNumSmall.LoadImage(hInstance,TEXT("IDB_END_NUM_SMALL"));
	m_EndNumBig.LoadImage(hInstance,TEXT("IDB_END_NUM_BIG"));
	m_ImageGun1.LoadImage(hInstance,TEXT("IDB_SHOOT1"));
	m_ImageGun2.LoadImage(hInstance,TEXT("IDB_SHOOT2"));
    m_ICrash.LoadImage(hInstance,TEXT("IDB_HOLE"));

	m_SpecCardFlash.LoadImage(hInstance,TEXT("IDB_SPEC_CARD_FLASH"));
	m_ScoreFrame.LoadImage(hInstance,TEXT("IDB_GAME_INFO"));
	m_TishiFrame.LoadImage(hInstance,TEXT("IDB_BK"));
	m_IPlusBigNumber.LoadImage(hInstance,TEXT("IDB_PLUS_BIG_NUM"));
	m_IPlusSmallNumber.LoadImage(hInstance,TEXT("IDB_PLUS_SMALL_NUM"));
	m_IBigSubtractionSign.LoadImage(hInstance,TEXT("IDB_BIG_SUBTRACTION_SIGN"));
	m_IBigPlusSign.LoadImage(hInstance,TEXT("IDB_BIG_PLUS_SIGN"));
	m_ISmallSubtractionSign.LoadImage(hInstance,TEXT("IDB_SMALL_SUBTRACTION_SIGN"));
	m_ISmallPlusSign.LoadImage(hInstance,TEXT("IDB_SMALL_PLUS_SIGN"));
	m_FourbaggerFlash.LoadImage(hInstance,TEXT("IDB_SP"));
	m_Fourbagger.LoadImage(hInstance,TEXT("IDB_ALL_KILL"));

// 	CString strPath = m_strPath + "左上角.psd";	
//     m_ImagezsFlower = AfxGetViewManager()->CreateSurface( strPath.GetBuffer(), PSD_SURFACE);
// 
// 	strPath = m_strPath + "左下角.psd";
// 	m_ImagezxFlower = AfxGetViewManager()->CreateSurface(strPath.GetBuffer(),PSD_SURFACE);
// 
// 	strPath = m_strPath + "右上角.psd";
// 	m_ImageysFlower = AfxGetViewManager()->CreateSurface(strPath.GetBuffer(),PSD_SURFACE);
//     
//     strPath = m_strPath + "右下角.psd";
//     m_ImageyxFlower = AfxGetViewManager()->CreateSurface(strPath.GetBuffer(),PSD_SUR

//	strPath = m_strPath + "logo1.psd";
//	m_IBackAdornment1 = AfxGetViewManager()->CreateSurface(strPath.GetBuffer(),PSD_SURFACE);


//   // strPath = m_strPath + "fire.psd";
//   // m_IFire = AfxGetViewManager()->CreateSurface(strPath.GetBuffer(),PSD_SURFACE);

	//游戏变量
	m_bTishi =false;
	m_bCompareAllCard = false;
	m_bDrawShuffle =false;		///<是否洗牌完成
	m_bShowBorder = false;		///<是否显示边框
	m_bShowCompare=false;	
	m_enSegmentType=enErr;
	ZeroMemory(m_szSegInfo, sizeof(m_szSegInfo));
	m_HuaXian = 0;
	m_nShootNum = 0;			///<第几个打枪用户
	m_lCellScore = 0;
	//游戏变量
	for(int i=0;i<6;i++)
	{
		m_nShootState[i][0]=-1;
		m_nShootState[i][1]=-1;
	}
	m_bEnd=false;
	m_bStart=false;
	m_bCompare=false;
	m_nXPos=0;
	m_nYPos=0;
	m_nCardCount=0;
	m_bDrawTimer=true;
	m_bDrawFrontCardType=false;
	m_bDrawMidCardType=false;
	m_bDrawBackCardType=false;
	m_nFrontCardType=0;
	m_nSpecialType=0;
	m_nMidCardType=0;
	m_nBackCardType=0;
	m_nCurrentUser=0;
	m_bDrawSpecialCard=false;
	m_bDrawScore=false;
	m_bSendCardAction=true;
	m_nWinPlayer=0;
	m_nLosePlayer=0;
	m_bDrawViewCard=false;
	m_nDrawFireWhichFrame=0;
	m_bDrawFire=false;
	m_nChairWin=0;
	m_nChairLoss=0;
	m_nWidth=0;
	m_nHeight=0;
	m_iPaiKuangZS = 0;
	m_wAllUser=0;
	m_nFourbaggerFrames=0;
	m_bFourbagger=false;
	m_bShowFourbagger=false;
	m_bDrawHandCard=false;
	m_bClockPoint=false;
	m_bDrawMyScore=true;
	m_bShowCard=false;
	ZeroMemory(m_UserName,sizeof(m_UserName));
	ZeroMemory(m_bDrawFireCourse,sizeof(m_bDrawFireCourse));
	ZeroMemory(m_ViewSegmentCard,sizeof(m_ViewSegmentCard));
	ZeroMemory(m_nCardSpace,sizeof(m_nCardSpace));
	ZeroMemory(m_nXpoint,sizeof(m_nXpoint));
	ZeroMemory(m_nYpoint,sizeof(m_nYpoint));
	ZeroMemory(m_bDrawSpecialType,sizeof(m_bDrawSpecialType));
	ZeroMemory(m_bSpecialTypeview,sizeof(m_bSpecialTypeview));
	ZeroMemory(m_LAllTax,sizeof(m_LAllTax));
	ZeroMemory(m_szTaxInfo,sizeof(m_szTaxInfo));
	ZeroMemory(m_LAllScore,sizeof(m_LAllScore));
	ZeroMemory(m_btCompareCardData,sizeof(m_btCompareCardData));
	ZeroMemory(&m_nSGameEnd,sizeof(m_nSGameEnd));
	ZeroMemory(m_bDrawPlayer1,sizeof(m_bDrawPlayer1));
	ZeroMemory(m_bDrawPlayer2,sizeof(m_bDrawPlayer2));
	ZeroMemory(m_bDrawPlayer3,sizeof(m_bDrawPlayer3));
	ZeroMemory(m_bDrawPlayer4,sizeof(m_bDrawPlayer4));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_szUserNameInfo,sizeof(m_szUserNameInfo));
	ZeroMemory(m_szPassScoreInfo,sizeof(m_szPassScoreInfo));
	ZeroMemory(m_szbToltalWinDaoShu,sizeof(m_szbToltalWinDaoShu));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	ZeroMemory(m_bDrawCardKuang,sizeof(m_bDrawCardKuang));
	for(int i=0;i<4;i++)
	{ 
		m_bOnOffThanCard[i] = false;   ///<是否画比牌
	   m_ThanCardPoint[i].SetPoint(0,0);
       m_ReadyPoint[i].SetPoint(0,0);
	   m_BoyPoint[i].SetPoint(0,0);
	   m_GirlPoint[i].SetPoint(0,0);
	   m_FacePoint[i].SetPoint(0,0);
	   m_DragonPoint[i].SetPoint(0,0);
	   m_DrawCardKuang1[i].SetPoint(0,0);
	}
	m_wMeChairID=-1;
	
	//m_SBigCardSize.cx=m_ImageBigCard.GetWidth()/13;
	//m_SBigCardSize.cy=m_ImageBigCard.GetHeight()/4;
   
	return;
}

//析构函数
CGameClientView::~CGameClientView(void)
{
}

int CGameClientView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//创建控件
	CRect CreateRect(0,0,0,0);
	m_ScoreView.Create(IDD_GAME_SCORE,this);
	m_MysteryDlg.Create(IDD_MYSTERY,this);

	m_MysteryDlg.SetSinkWindow(AfxGetMainWnd());
	//m_MysteryDlg.ShowWindow(SW_SHOW);

	//创建扑克
	for (WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		m_UserCardControl[i].SetDirection(true);
		m_UserCardControl[i].SetDisplayFlag(true);
		//m_UserCardControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,20+i);
		m_UserCardControl[i].SetCardSpace(24 , 18 , false);
        //m_UserCardControl[i].SetDrawCard(true);

		for(int j=0;j<3;j++)
		{
			m_SegCardControl[i][j].SetDirection(true);
			m_SegCardControl[i][j].SetDisplayFlag(true);
           // m_SegCardControl[i][j].SetDrawCard(true);
			//m_SegCardControl[i][j].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,30+i);
		}



		//m_OverTimeCardControl[i].Create(NULL, NULL, WS_VISIBLE|WS_CHILD, CreateRect, this, 90+i);
		if(i==0 || i==2) m_OverTimeCardControl[i].SetDirection(true);
		else m_OverTimeCardControl[i].SetDirection(false);
		m_OverTimeCardControl[i].SetDisplayFlag(true);
		m_OverTimeCardControl[i].SetCardSpace(20, 15, 0) ;
       // m_OverTimeCardControl[i].SetDrawCard(true);
		if(1==i)
		{   

			m_UserCardControl[i].SetDirection(false) ;
			m_UserCardControl[i].SetCardSide(enLeft) ;

			for(int j=0;j<3;j++)
			{
				m_SegCardControl[i][j].SetDirection(false) ;
				m_SegCardControl[i][j].SetCardSide(enLeft) ;
				m_SegCardControl[i][j].SetCardSpace(20 , 15 , false);
			}

		}
		else if(3==i)
		{
			m_UserCardControl[i].SetDirection(false) ;
			m_UserCardControl[i].SetCardSide(enRight);

			for(int j=0;j<3;j++)
			{
				m_SegCardControl[i][j].SetDirection(false);
				m_SegCardControl[i][j].SetCardSide(enRight);
				m_SegCardControl[i][j].SetCardSpace(20 , 15 , false);
			}

		}
		else 
		{
			m_UserCardControl[i].SetCardSide(enHorizon);

			for(int j=0;j<3;j++)
			{
				m_SegCardControl[i][j].SetCardSide(enHorizon);
				if(0==j)
				{
					m_SegCardControl[i][j].SetCardSpace(17,15,0);
				}
				if(1==j)
				{
					m_SegCardControl[i][j].SetCardSpace(18,15,0);
				}
				if(2==j)
				{
					m_SegCardControl[i][j].SetCardSpace(18,15,0);
				}
			}

		}
		if(0==i)
		{
			m_UserCardControl[i].SetDisplayFlag(false);

			for(int j=0;j<3;j++)
			{
				m_SegCardControl[i][j].SetCardSpace(10 , 10 , false);
				m_SegCardControl[i][j].SetDisplayFlag(false);
			} 

		}
	}

	m_UserCardControl[2].SetSinkWindow(AfxGetMainWnd()) ;
	m_UserCardControl[2].SetbMyselfUser(true);

	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		//所以分段
		for(BYTE j=0 ; j<3 ; ++j)
		{
			m_AllSegCardControl[i][j].SetDirection(true);
			m_AllSegCardControl[i][j].SetDisplayFlag(true);
			//m_AllSegCardControl[i][j].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,40+i+j);
			m_AllSegCardControl[i][j].SetCardSide(enHorizon);
		}

		//对比扑克
		m_CompareCarControl[i].SetDirection(true);
		m_CompareCarControl[i].SetDisplayFlag(true);
		//m_CompareCarControl[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,60+i);
		m_CompareCarControl[i].SetCardSide(enHorizon) ;
		m_CompareCarControl[i].SetbBigCard(true);
		m_CompareCarControl[i].SetCardSpace(25,15,false);

		//m_CardType[i].Create(NULL,NULL,WS_VISIBLE|WS_CHILD,CreateRect,this,60+i);
	}

	// 字体
	m_Font.CreateFont( this, TEXT("宋体"), 12, 500 );

//	//创建按钮
	m_btStart.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_START);
	//m_btStartAgain.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_STARTAGAIN);
	//m_btEnd.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_END);
	m_btFrontCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_FRONT_CARD);
	m_btMidCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_MID_CARD);
	m_btBackCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_BACK_CARD);
	m_btOk.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_OK);
	m_btSubmit.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_SUBMIT);
	m_btCancel.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_CANCEL);
	//m_btDescend.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_DESCEND_SORT);
	//m_btAscend.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ASCEND_SORT);	
	//m_btColor.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_COLOR_SORT);	
	//m_btAutoOutCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_AUTO_OUT_CARD);
	//m_btRestoreCard.Create(TEXT(""), WS_CHILD,CreateRect,this,IDC_RESTORE_CARD);

	m_btOnePare.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_ONEPARE);
	m_btTwoPare.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TWOPARE);
	m_btThreeSame.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_THREESAME);
	m_btStraight.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_STRAIGHT);
	m_btFlush.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_FLUSH);
	m_btGourd.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_GOURD);
	m_btFourSame.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_FOURSAME);
	m_btStraightFlush.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_STRAIGHTFLUSH);
	m_btPaiXu.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_TAXIS);

	m_btRecycleCard1.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_RECYCLECARD1);
	m_btRecycleCard2.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_RECYCLECARD2);
	m_btRecycleCard3.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_RECYCLECARD3);
	m_btRecycleAllCard.Create(TEXT(""),WS_CHILD,CreateRect,this,IDC_RECYCLECARDALL);

	m_btOpenAdmin.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(4,4,11,11),this,IDC_BT_ADMIN);
	m_btOpenAdmin.ShowWindow(SW_HIDE);

	m_ClientControl.Create(IDD_DIALOG_ADMIN,this);
	m_ClientControl.ShowWindow(SW_HIDE);


	//设置按钮
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btStart.SetButtonImage(IDB_BT_START,hInstance,false,false);
	//m_btEnd.SetButtonImage(IDB_BT_END,hInstance,false);
	m_btFrontCard.SetButtonImage(IDB_BT_FRONT_CARD,hInstance,false,false);
	m_btMidCard.SetButtonImage(IDB_BT_MID_CARD,hInstance,false,false);
	m_btBackCard.SetButtonImage(IDB_BT_BACK_CARD,hInstance,false,false);
	m_btOk.SetButtonImage(IDB_BT_SHOW_CARD,hInstance,false,false);
	m_btSubmit.SetButtonImage(IDB_OK,hInstance,false,false);
	m_btCancel.SetButtonImage(IDB_CANCEL,hInstance,false,false);
	//m_btAutoOutCard.SetButtonImage(IDB_BT_HINT,hInstance,false);
	//m_btRestoreCard.SetButtonImage(IDB_BT_RESTORE_CARD, hInstance,false);
	//m_btColor.SetButtonImage(IDB_BT_COLOR_SORT_CARD,hInstance,false);
	//m_btDescend.SetButtonImage(IDB_BT_DESCEND_SORT_CARD,hInstance,false);
	//m_btAscend.SetButtonImage(IDB_BT_ASCEND_SORT_CARD,hInstance,false);

	m_btOnePare.SetButtonImage(IDB_ONEPARE,hInstance,false,false);
	m_btTwoPare.SetButtonImage(IDB_TWOPARE,hInstance,false,false);
	m_btThreeSame.SetButtonImage(IDB_THREESAME,hInstance,false,false);
	m_btStraight.SetButtonImage(IDB_STRAIGHT,hInstance,false,false);
	m_btFlush.SetButtonImage(IDB_FLUSH,hInstance,false,false);
	m_btGourd.SetButtonImage(IDB_GOURD,hInstance,false,false);
	m_btFourSame.SetButtonImage(IDB_FOURSAME,hInstance,false,false);
	m_btStraightFlush.SetButtonImage(IDB_STRAIGHTFLUSH,hInstance,false,false);
	m_btPaiXu.SetButtonImage(IDB_TAXIS,hInstance,false,false);
	m_btRecycleCard1.SetButtonImage(IDB_RECYCLE,hInstance,false,false);
	m_btRecycleCard2.SetButtonImage(IDB_RECYCLE,hInstance,false,false);
	m_btRecycleCard3.SetButtonImage(IDB_RECYCLE,hInstance,false,false);
	m_btRecycleAllCard.SetButtonImage(IDB_RECYCLEALL,hInstance,false,false);
	//////////////////////////////////////////////////////////////////////////
	//MessageBox(NULL,"chuangjian",NULL);
	return 0;
}
//重置界面
VOID CGameClientView::ResetGameView()
{
	//游戏变量

	for(int i=0;i<6;i++)
	{
		m_nShootState[i][0]=-1;
		m_nShootState[i][1]=-1;
	}
	m_bShowCompare=false;	
	m_enSegmentType=enErr;
	ZeroMemory(m_szSegInfo, sizeof(m_szSegInfo));
	m_bEnd=false;
	m_bStart=false;
	m_bCompare=false;
	m_nXPos=0;
	m_nYPos=0;
	m_nCardCount=0;
	m_bDrawTimer=true;
	m_bDrawFrontCardType=false;
	m_bDrawMidCardType=false;
	m_bDrawBackCardType=false;
	m_nFrontCardType=0;
	m_nMidCardType=0;
	m_nBackCardType=0;
	m_nCurrentUser=0;
	m_bDrawSpecialCard=false;
	m_bDrawScore=false;
	m_bSendCardAction=true;
	m_nWinPlayer=0;
	m_nLosePlayer=0;
	m_bDrawViewCard=false;
	m_nDrawFireWhichFrame=0;
	m_bDrawFire=false;
	m_nChairWin=0;
	m_nChairLoss=0;
	//ZeroMemory(m_UserName,sizeof(m_UserName));
	ZeroMemory(m_bDrawFireCourse,sizeof(m_bDrawFireCourse));
	ZeroMemory(m_ViewSegmentCard,sizeof(m_ViewSegmentCard));
	ZeroMemory(m_nCardSpace,sizeof(m_nCardSpace));
	ZeroMemory(m_nXpoint,sizeof(m_nXpoint));
	ZeroMemory(m_nYpoint,sizeof(m_nYpoint));
	ZeroMemory(m_bDrawSpecialType,sizeof(m_bDrawSpecialType));
	ZeroMemory(m_bSpecialTypeview,sizeof(m_bSpecialTypeview));
	ZeroMemory(m_btCompareCardData,sizeof(m_btCompareCardData));
	ZeroMemory(&m_nSGameEnd,sizeof(m_nSGameEnd));
	ZeroMemory(m_bDrawPlayer1,sizeof(m_bDrawPlayer1));
	ZeroMemory(m_bDrawPlayer2,sizeof(m_bDrawPlayer2));
	ZeroMemory(m_bDrawPlayer3,sizeof(m_bDrawPlayer3));
	ZeroMemory(m_bDrawPlayer4,sizeof(m_bDrawPlayer4));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	//m_SBigCardSize.cx=m_ImageBigCard.GetWidth()/13;
	//m_SBigCardSize.cy=m_ImageBigCard.GetHeight()/4;


	return;
}
void CGameClientView::ResetViewData(bool IsLookon)
{
	//游戏变量
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_UserCardControl[i].m_nI=0;
		m_UserSendCard0.m_nShuffleNum = -1;
			switch(i)
			{
			case 0:m_UserSendCard[i].m_NumCard=-1;break;
			case 1:m_UserSendCard[i].m_NumCard=-3;break;
			case 2:m_UserSendCard[i].m_NumCard=-5;break;
			case 3:m_UserSendCard[i].m_NumCard=-7;break;
			}
	}
	m_nShootNum = 0;			///<哪组（赢，输）打枪用户
	m_bTishi = false;
	m_bCompareAllCard = false;
	m_bShowBorder = false;		///<是否显示边框	
	m_bDrawShuffle = false;		///<是否洗牌完成
	m_bDrawHandCard=false;
	m_bShowCompare=false;	
	m_enSegmentType=enErr;
	ZeroMemory(m_szSegInfo, sizeof(m_szSegInfo));
	m_bEnd=false;
	m_bStart=false;
	m_bCompare=false;
	m_nXPos=0;
	m_nYPos=0;
	m_nCardCount=0;
	m_bDrawTimer=true;
	m_bDrawFrontCardType=false;
	m_bDrawMidCardType=false;
	m_bDrawBackCardType=false;
	m_nFrontCardType=0;
	m_nSpecialType=0;
	m_nMidCardType=0;
	m_nBackCardType=0;
	m_nCurrentUser=0;
	m_bDrawSpecialCard=false;
	m_bDrawScore=false;
	m_bSendCardAction=true;
	m_nWinPlayer=0;
	m_nLosePlayer=0;
	m_bDrawViewCard=false;
	m_nDrawFireWhichFrame=0;
	m_bDrawFire=false;
	m_nChairWin=0;
	m_nChairLoss=0;
	m_iPaiKuangZS = 0;
	m_wAllUser=0;
	m_nFourbaggerFrames=0;
	m_bFourbagger=false;
	m_bShowFourbagger=false;
	m_bDrawMyScore=true;
	m_bShowCard=false;
	//ZeroMemory(m_UserName,sizeof(m_UserName));
	ZeroMemory(m_bDrawFireCourse,sizeof(m_bDrawFireCourse));
	ZeroMemory(m_ViewSegmentCard,sizeof(m_ViewSegmentCard));
	ZeroMemory(m_nCardSpace,sizeof(m_nCardSpace));
	ZeroMemory(m_nXpoint,sizeof(m_nXpoint));
	ZeroMemory(m_nYpoint,sizeof(m_nYpoint));
	ZeroMemory(m_bDrawSpecialType,sizeof(m_bDrawSpecialType));
	ZeroMemory(m_bSpecialTypeview,sizeof(m_bSpecialTypeview));
	ZeroMemory(m_btCompareCardData,sizeof(m_btCompareCardData));
	ZeroMemory(&m_nSGameEnd,sizeof(m_nSGameEnd));
	ZeroMemory(m_bDrawPlayer1,sizeof(m_bDrawPlayer1));
	ZeroMemory(m_bDrawPlayer2,sizeof(m_bDrawPlayer2));
	ZeroMemory(m_bDrawPlayer3,sizeof(m_bDrawPlayer3));
	ZeroMemory(m_bDrawPlayer4,sizeof(m_bDrawPlayer4));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	ZeroMemory(m_bDrawCardKuang,sizeof(m_bDrawCardKuang));
	//m_SBigCardSize.cx=m_ImageBigCard.GetWidth()/13;
	//m_SBigCardSize.cy=m_ImageBigCard.GetHeight()/4;

	if(!IsLookon)
	{
		m_bOnOffTishikuang = true;
		m_btOnePare.ShowWindow(SW_SHOW);  
		m_btOnePare.EnableWindow(FALSE);
		m_btTwoPare.ShowWindow(SW_SHOW);  
		m_btTwoPare.EnableWindow(FALSE);
		m_btThreeSame.ShowWindow(SW_SHOW); ;
		m_btThreeSame.EnableWindow(FALSE);
		m_btStraight.ShowWindow(SW_SHOW);
		m_btStraight.EnableWindow(FALSE);
		m_btFlush.ShowWindow(SW_SHOW); 
		m_btFlush.EnableWindow(FALSE);
		m_btGourd.ShowWindow(SW_SHOW);
		m_btGourd.EnableWindow(FALSE);
		m_btFourSame.ShowWindow(SW_SHOW);
		m_btFourSame.EnableWindow(FALSE); 
		m_btStraightFlush.ShowWindow(SW_SHOW);
		m_btStraightFlush.EnableWindow(FALSE);
		m_btRecycleCard1.ShowWindow(SW_HIDE);
		m_btRecycleCard1.EnableWindow(FALSE);
		m_btRecycleCard2.ShowWindow(SW_HIDE);
		m_btRecycleCard2.EnableWindow(FALSE);
		m_btRecycleCard3.ShowWindow(SW_HIDE);
		m_btRecycleCard3.EnableWindow(FALSE);
		m_btRecycleAllCard.ShowWindow(SW_HIDE);
		m_btRecycleAllCard.EnableWindow(FALSE);
		m_btPaiXu.ShowWindow(SW_SHOW);
		m_btPaiXu.EnableWindow(FALSE);
		m_btFrontCard.ShowWindow(SW_SHOW);
		m_btFrontCard.EnableWindow(FALSE);
		m_btMidCard.ShowWindow(SW_SHOW);
		m_btMidCard.EnableWindow(FALSE);
		m_btBackCard.ShowWindow(SW_SHOW);
		m_btBackCard.EnableWindow(FALSE);
		m_btOk.ShowWindow(SW_SHOW);
		m_btOk.EnableWindow(FALSE);
	}


	return;
}

void CGameClientView::HideButton()
{
	//设置按钮
	m_bOnOffTishikuang = FALSE;
	m_bShowBorder = false;
	m_btOk.ShowWindow(SW_HIDE) ;
	m_btOk.EnableWindow(FALSE) ;
	m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_btFrontCard.EnableWindow(FALSE) ;
	m_btMidCard.ShowWindow(SW_HIDE) ;
	m_btMidCard.EnableWindow(FALSE) ;
	m_btBackCard.ShowWindow(SW_HIDE) ;
	m_btBackCard.EnableWindow(FALSE) ;
	m_btRecycleCard1.ShowWindow(SW_HIDE);
	m_btRecycleCard1.EnableWindow(FALSE);
	m_btRecycleCard2.ShowWindow(SW_HIDE);
	m_btRecycleCard2.EnableWindow(FALSE);
	m_btRecycleCard3.ShowWindow(SW_HIDE);
	m_btRecycleCard3.EnableWindow(FALSE);
	m_btRecycleAllCard.ShowWindow(SW_HIDE);
	m_btRecycleAllCard.EnableWindow(FALSE);

	m_btPaiXu.ShowWindow(SW_HIDE);
	m_btPaiXu.EnableWindow(FALSE);

	m_btOnePare.ShowWindow(SW_HIDE);
	m_btOnePare.EnableWindow(FALSE);
	m_btTwoPare.ShowWindow(SW_HIDE);
	m_btTwoPare.EnableWindow(FALSE);
	m_btThreeSame.ShowWindow(SW_HIDE);
	m_btThreeSame.EnableWindow(FALSE);
	m_btStraight.ShowWindow(SW_HIDE);
	m_btStraight.EnableWindow(FALSE);
	m_btFlush.ShowWindow(SW_HIDE);
	m_btFlush.EnableWindow(FALSE);
	m_btGourd.ShowWindow(SW_HIDE);
	m_btGourd.EnableWindow(FALSE);
	m_btFourSame.ShowWindow(SW_HIDE);
	m_btFourSame.EnableWindow(FALSE);
	m_btStraightFlush.ShowWindow(SW_HIDE);
	m_btStraightFlush.EnableWindow(FALSE);
}

//调整控件
VOID CGameClientView::RectifyControl(INT nWidth, INT nHeight)
{
	//积分视图
	m_nWidth=nWidth;
	m_nHeight=nHeight;

	m_OverTimeCardControl[0].SetBenchmarkPos(nWidth/2/2-50 , nHeight/2-230-30 , enXLeft , enYCenter);
	m_OverTimeCardControl[1].SetBenchmarkPos(+67 , nHeight/2-60+40-30 , enXLeft , enYTop);
	m_OverTimeCardControl[2].SetBenchmarkPos(nWidth/2/2-50 , nHeight-100-30 , enXLeft , enYBottom);
	m_OverTimeCardControl[3].SetBenchmarkPos(nWidth-70 , nHeight/2-60+40-30 , enXRight , enYTop);

	m_CardType[0].SetDrawTypePoint(nWidth/2-100,nHeight/2-200);
	m_CardType[1].SetDrawTypePoint(nWidth/2-200,nHeight/2-50);
	m_CardType[2].SetDrawTypePoint(nWidth/2-100,nHeight/2+200);
	m_CardType[3].SetDrawTypePoint(nWidth/2+200,nHeight/2-50);

	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(0==i)
			{
				m_SegCardControl[i][j].SetBenchmarkPos(nWidth/2-150+20+j*40 , nHeight/2-230-30 , enXCenter , enYCenter);
			}
			if(1==i)
			{
				m_SegCardControl[i][j].SetBenchmarkPos(67 , nHeight/2-100-15+j*40-30 , enXLeft , enYCenter);
			}
			if(2==i)
			{
				if(0==j)
				{
					m_SegCardControl[i][j].SetBenchmarkPos(nWidth/2-41 , nHeight/2-92 , enXCenter , enYBottom);
				}
				if(1==j)
				{
					m_SegCardControl[i][j].SetBenchmarkPos(nWidth/2-60 , nHeight/2-64 , enXCenter , enYBottom);
				}
				if(2==j)
				{
					m_SegCardControl[i][j].SetBenchmarkPos(nWidth/2-78 , nHeight/2-36 , enXCenter , enYBottom);
				}
			}
			if(3==i)
			{
				m_SegCardControl[i][j].SetBenchmarkPos(nWidth-70 , nHeight/2-100-15+j*40-30 , enXRight , enYCenter);
			}
		}
	}

	POINT ptCardControl[GAME_PLAYER] ;
	ptCardControl[0].x = nWidth/2+10 ;
	ptCardControl[0].y =105 ;

	ptCardControl[1].x = nWidth/2-250 ;
	ptCardControl[1].y = nHeight/2-100 ;

	ptCardControl[2].x = nWidth/2+10 ;
	ptCardControl[2].y = nHeight/2+130 ;

	ptCardControl[3].x = nWidth/2+250 ;
	ptCardControl[3].y = nHeight/2-100 ;


	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		//所有分段
		if(i==0)
		{
            m_AllSegCardControl[i][0].SetBenchmarkPos(ptCardControl[i].x-59 , ptCardControl[i].y-40 , enXCenter , enYCenter);
			m_AllSegCardControl[i][1].SetBenchmarkPos(ptCardControl[i].x-15-59 , ptCardControl[i].y+27*1-40 , enXCenter , enYCenter);
			m_AllSegCardControl[i][2].SetBenchmarkPos(ptCardControl[i].x-15-59 , ptCardControl[i].y+27*2-40 , enXCenter , enYCenter);
		}
		else if(i==1)
		{
			m_AllSegCardControl[i][0].SetBenchmarkPos(ptCardControl[i].x-90 , ptCardControl[i].y+27*0+5 , enXCenter , enYCenter);
			m_AllSegCardControl[i][1].SetBenchmarkPos(ptCardControl[i].x-15-90 , ptCardControl[i].y+27*1+5 , enXCenter , enYCenter);
			m_AllSegCardControl[i][2].SetBenchmarkPos(ptCardControl[i].x-15-90 , ptCardControl[i].y+27*2+5 , enXCenter , enYCenter);
		}
		else if(i==2)
		{
			m_AllSegCardControl[i][0].SetBenchmarkPos(ptCardControl[i].x-59 , ptCardControl[i].y+27*0-30 , enXCenter , enYCenter);
			m_AllSegCardControl[i][1].SetBenchmarkPos(ptCardControl[i].x-15-59 , ptCardControl[i].y+27*1-30 , enXCenter , enYCenter);
			m_AllSegCardControl[i][2].SetBenchmarkPos(ptCardControl[i].x-15-59 , ptCardControl[i].y+27*2-30 , enXCenter , enYCenter);
		}
		else
		{
			m_AllSegCardControl[i][0].SetBenchmarkPos(ptCardControl[i].x-15 , ptCardControl[i].y+27*0+5 , enXCenter , enYCenter);
			m_AllSegCardControl[i][1].SetBenchmarkPos(ptCardControl[i].x-30 , ptCardControl[i].y+27*1+5 , enXCenter , enYCenter);
			m_AllSegCardControl[i][2].SetBenchmarkPos(ptCardControl[i].x-30 , ptCardControl[i].y+27*2+5 , enXCenter , enYCenter);
		}
	}
	
	//对比扑克
	m_CompareCarControl[0].SetBenchmarkPos(ptCardControl[0].x-89 , ptCardControl[0].y-20 , enXCenter , enYCenter) ; 
	m_CompareCarControl[1].SetBenchmarkPos(ptCardControl[1].x-120 , ptCardControl[1].y+20, enXCenter , enYCenter) ;
	m_CompareCarControl[2].SetBenchmarkPos(ptCardControl[2].x-96 , ptCardControl[2].y-20, enXCenter , enYCenter) ;
	m_CompareCarControl[3].SetBenchmarkPos(ptCardControl[3].x-40 , ptCardControl[3].y+20, enXCenter , enYCenter) ;
	m_DrawGun.SetBenchmarkPos(nWidth/2,nHeight/2,enXCenter , enYCenter);
	/*m_ptTimer[3].x=nWidth-m_nXBorder-m_nXFace-5;
	m_ptTimer[3].y=nHeight/2-m_nYFace-m_nYTimer-65+70;
	m_ptReady[3].x=nWidth-m_nXBorder-m_nXTimer-65;
	m_ptReady[3].y=nHeight/2-m_nYFace-m_nYTimer/2-65+110;
	m_UserAddrInfo[3].x = m_ptName[3].x;
	m_UserAddrInfo[3].y = m_ptName[3].y + 30;


	m_ptTimer[1].x=m_nXBorder+5;
	m_ptTimer[1].y=nHeight/2-m_nYFace-5-m_nYTimer-60+90;
	m_ptReady[1].x=m_nXBorder+65+m_nXTimer;
	m_ptReady[1].y=nHeight/2-m_nYFace-m_nYTimer/2-65+110;
	m_UserAddrInfo[1].x = m_ptName[1].x;
	m_UserAddrInfo[1].y = m_ptName[1].y + 30;

	m_ptTimer[0].x=nWidth/2-m_nXFace/2-m_nXTimer*2-25;
	m_ptTimer[0].y=m_nYBorder+10;
	m_ptReady[0].x=nWidth/2-m_nXTimer-m_nXFace/2+50;
	m_ptReady[0].y=m_nYBorder+m_nYTimer/2+10;
	m_UserAddrInfo[0].x = m_ptName[0].x;
	m_UserAddrInfo[0].y = m_ptName[0].y + 30;

	m_ptTimer[2].x=nWidth/2-m_nXFace/2-m_nXTimer*2-15-120;
	m_ptTimer[2].y=nHeight-m_nYBorder-m_nYTimer-5;
	m_ptReady[2].x=nWidth/2-m_nXTimer-m_nXFace/2+50;
	m_ptReady[2].y=nHeight-m_nYBorder-m_nYTimer/2-20;
	m_UserAddrInfo[2].x = m_ptName[2].x;
	m_UserAddrInfo[2].y = m_ptName[2].y + 30;*/

	m_UserName[0].x=nWidth/2+5;
	m_UserName[0].y=30;
	m_UserName[1].x=10;
	m_UserName[1].y=nHeight/2+30;
	m_UserName[2].x=10;
	m_UserName[2].y=nHeight-30;
	m_UserName[3].x=nWidth-100;
	m_UserName[3].y=nHeight/2+30;
    //头像位置
	m_FacePoint[0].SetPoint(nWidth/2-50,20);
	m_FacePoint[1].SetPoint(35,nHeight/2-30);
	m_FacePoint[2].SetPoint(35,nHeight-84);
	m_FacePoint[3].SetPoint(nWidth-75,nHeight/2-30);
    //准备位置
	m_ReadyPoint[0].SetPoint(nWidth/2-200,90);
	m_ReadyPoint[1].SetPoint(120,nHeight/2-10);
	m_ReadyPoint[2].SetPoint(nWidth/2-20,nHeight-200);
	m_ReadyPoint[3].SetPoint(nWidth-200,nHeight/2-10);
	//准备比牌图片位置
	m_ThanCardPoint[0].SetPoint(nWidth/2-155,50);
	m_ThanCardPoint[1].SetPoint(15,nHeight/2+65);
	m_ThanCardPoint[2].SetPoint(nWidth/2-385,nHeight-80);
	m_ThanCardPoint[3].SetPoint(nWidth-90,nHeight/2+65);
	
	//倒水位置
	m_DragonPoint[0].SetPoint(nWidth/2-200,nHeight/2-260);
	m_DragonPoint[1].SetPoint(nWidth/2-170,nHeight/2-50);
	m_DragonPoint[2].SetPoint(nWidth/2+85,nHeight/2+180);
	m_DragonPoint[3].SetPoint(nWidth/2+60,nHeight/2-50);

	//特殊牌底框
	m_DrawCardKuang1[0].SetPoint(ptCardControl[0].x-102 , ptCardControl[0].y-41);
	m_DrawCardKuang1[1].SetPoint(ptCardControl[1].x-132 , ptCardControl[1].y+4);
	m_DrawCardKuang1[2].SetPoint(ptCardControl[2].x-102 , ptCardControl[2].y-31);
	m_DrawCardKuang1[3].SetPoint(ptCardControl[3].x-57 , ptCardControl[3].y+4);
	//全垒打位置
	m_FourbaggerPoint[0].SetPoint(nWidth/2-60,nHeight/2-120);
	m_FourbaggerPoint[1].SetPoint(nWidth/2-240,nHeight/2-60);
	m_FourbaggerPoint[2].SetPoint(nWidth/2-60,nHeight/2);
	m_FourbaggerPoint[3].SetPoint(nWidth/2+120,nHeight/2-60);
	//特殊牌提示底框位置
	m_TishiFramePoint.SetPoint(nWidth/2-165,nHeight/2-120);

	//移动按钮
	CRect rcButton;
	HDWP hDwp=BeginDeferWindowPos(18);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;

	m_btStart.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp,m_btStart, NULL, (nWidth - rcButton.Width()) / 2-250, ( nHeight - rcButton.Height() ) / 2 + 200, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btRecycleCard1, NULL, (nWidth+200)/2,nHeight/2-95, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btRecycleCard2, NULL, (nWidth+200)/2,nHeight/2-45, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btRecycleCard3, NULL, (nWidth+200)/2,nHeight/2+5, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btRecycleAllCard, NULL,(nWidth+200)/2,nHeight/2+60, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btSubmit, NULL,m_TishiFramePoint.x+48,m_TishiFramePoint.y+171, 0, 0, uFlags);
	DeferWindowPos(hDwp,m_btCancel, NULL,m_TishiFramePoint.x+195,m_TishiFramePoint.y+171, 0, 0, uFlags);

	m_btFrontCard.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btFrontCard, NULL, ((nWidth-370)/2)+11,nHeight-115, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btMidCard, NULL, ((nWidth-168)/2)+6,nHeight-115, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btBackCard, NULL, (nWidth+10)/2+13,nHeight-115, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btOk, NULL, (nWidth+160)/2+34,nHeight-115, 0, 0, uFlags);

	m_btOnePare.GetWindowRect(&rcButton);
	DeferWindowPos(hDwp, m_btOnePare, NULL, (nWidth-555)/2+45,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btTwoPare, NULL, (nWidth-415)/2+31,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btThreeSame, NULL, (nWidth-275)/2+16,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btStraight, NULL, (nWidth-130)/2-2,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btFlush, NULL, (nWidth+5)/2-14,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btGourd, NULL, (nWidth+145)/2-29,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btFourSame, NULL, (nWidth+285)/2-44,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btStraightFlush, NULL, (nWidth+425)/2-59,nHeight-55, 0, 0, uFlags);
	DeferWindowPos(hDwp, m_btPaiXu, NULL, (nWidth+565)/2-74,nHeight-55, 0, 0, uFlags);
	EndDeferWindowPos(hDwp);
	return;
}

//艺术字体
void CGameClientView::DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos)
{
	//变量定义
	int nStringLength=lstrlen(pszString);
	int nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	int nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	pDC->SetTextColor(crFrame);
	for (int i=0;i<CountArray(nXExcursion);i++)
	{
		pDC->TextOut(nXPos+nXExcursion[i],nYPos+nYExcursion[i],pszString,nStringLength);
	}

	//绘画字体
	pDC->SetTextColor(crText);
	pDC->TextOut(nXPos,nYPos,pszString,nStringLength);

	return;
}

//绘画字符
void CGameClientView::DrawTextString( CDC * pDC, CDFontEx* pFont, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat )
{
	//变量定义
	INT nStringLength=lstrlen(pszString);
	INT nXExcursion[8]={1,1,1,0,-1,-1,-1,0};
	INT nYExcursion[8]={-1,0,1,1,1,0,-1,-1};

	//绘画边框
	for (INT i=0;i<CountArray(nXExcursion);i++)
	{
		//计算位置
		CRect rcFrame;
		rcFrame.top=rcRect.top+nYExcursion[i];
		rcFrame.left=rcRect.left+nXExcursion[i];
		rcFrame.right=rcRect.right+nXExcursion[i];
		rcFrame.bottom=rcRect.bottom+nYExcursion[i];

		//绘画字符
		pFont->DrawText( pDC, pszString, rcFrame, crFrame, nDrawFormat);
	}

	//绘画字符
	pFont->DrawText( pDC, pszString, rcRect, crText, nDrawFormat);
}
//更新视图
void CGameClientView::RefreshGameView()
{
	CRect rect;
	GetClientRect(&rect);
	InvalidGameView(rect.left,rect.top,rect.Width(),rect.Height());

	return;
}
//摊牌函数
// void CGameClientView::OnShowCard()
// {
// 	AfxGetMainWnd()->SendMessage(IDM_OK,0,0);
// 	return ;
// }

// 设置后墩
// void CGameClientView::OnBackCard()
// {
// 	AfxGetMainWnd()->SendMessage(IDM_BACKCARD,0,0);
// 	return ;
// }
//设置中墩
// void CGameClientView::OnMidCard()
// {
// 	AfxGetMainWnd()->SendMessage(IDM_MIDCARD,0,0);
// 	return ;
// }
// //设置前墩
// void CGameClientView::OnFrontCard()
// {
// 	AfxGetMainWnd()->SendMessage(IDM_FRONTCARD,0,0);
// 	return ;
// }
// 
// //开始按钮
// void CGameClientView::OnStart()
// {
// 	m_btStart.ShowWindow(SW_HIDE);
// 	m_btStart.EnableWindow(FALSE);
// 	m_btEnd.ShowWindow(SW_HIDE);
// 	m_btEnd.EnableWindow(FALSE);
// 	AfxGetMainWnd()->SendMessage(IDM_START,0,0);
// 
// 	//更新界面
// 	RefreshGameView(NULL);
// 
// 	return ;
// }

//设置扑克
void CGameClientView::SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg) 
{
	switch(bCardType) 
	{
	case enHandCard:		//手上扑克
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)	m_UserCardControl[i].SetCardData(NULL , 0) ;
			return ;
		}
        
		m_UserCardControl[wChairID].SetCardData(bCardData , bCardCount) ;

		break;

	case enSegmentCard:		//分段扑克
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				for(int j=0;j<3;j++)
				{
					m_SegCardControl[i][j].SetCardData(NULL , 0) ;
				}

				return ;
		}
		for(int i=0;i<3;i++)
		{
			m_SegCardControl[wChairID][i].SetCardData(bCardData , bCardCount) ;
		}


		break;

	case enAllSegmentCard:	//所有分段
		if(INVALID_CHAIR==wChairID)
		{
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
				for(BYTE j=0 ; j<3 ; ++j)
					m_AllSegCardControl[i][j].SetCardData(NULL , 0) ;
			return ;
		}

		m_AllSegCardControl[wChairID][bSeg].SetDisplayFlag(true) ;
		m_AllSegCardControl[wChairID][bSeg].SetCardData(bCardData , bCardCount) ;

		break;
	case enCompareCard:		//比较扑克
		{
			if(INVALID_CHAIR==wChairID) 
			{
				for(WORD i=0 ; i<GAME_PLAYER ; ++i)
					m_CompareCarControl[i].SetCardData(NULL , 0 ) ;
				return ;
			}
			m_CompareCarControl[wChairID].SetCardData(bCardData , bCardCount) ;
			return ;
		}

	case enErrorType:		//错误类型
		//AfxMessageBox("错误扑克类型！") ;
		break;
	}
}


//显示比较
void CGameClientView::SetShowCompare(bool bShow , const enSegmentType SegmentType)
{
	if(false==bShow)
	{
		m_strShowCompare="" ;
		m_bShowCompare=false ;
		//更新界面
		RefreshGameView();
		return ;
	}

	m_bShowCompare = true ;
	switch(SegmentType)
	{
	case enFront:
		m_strShowCompare="前墩比较" ;
		break;

	case enMid:
		m_strShowCompare="中墩比较" ;
		break;

	case enBack:
		m_strShowCompare="后墩比较" ;
		break;

	case enAllSeg:
		m_strShowCompare="三墩比较" ;
		break;

	case enErr:
		{
			//AfxMessageBox("错误比较类型!") ;
			break;
		}
	default:
		{
			//AfxMessageBox("未知比较类型!") ;
			m_bShowCompare=false ;
			break; 
		}
	}
	//更新界面
	RefreshGameView();
}

//降序排列
// void CGameClientView::OnDescendSort()
// {
// 	AfxGetMainWnd()->SendMessage(IDM_DESCEND_SORT,0,0);
// }
// //升序排列
// void CGameClientView::OnAscendSort() 
// {
// 	AfxGetMainWnd()->SendMessage(IDM_ASCEND_SORT,0,0);
// }
// //花色排列
// void CGameClientView::OnColorSort() 
// {
// 	AfxGetMainWnd()->SendMessage(IDM_COLOR_SORT,0,0);
// }

//自动代打
// void CGameClientView::OnAutoOutCard() 
// {
// 	AfxGetMainWnd()->SendMessage(IDM_AUTO_OUT_CARD,1,1);
// 	return;
// }

//分段信息
void CGameClientView::ShowSegInfo(enSegmentType seg, CHAR const szInfo[32])
{
	switch(seg)
	{
	case enErr:	//没有类型
		{
			ZeroMemory(m_szSegInfo, sizeof(m_szSegInfo)) ;

			break ;
		}
	case enFront :		//前段扑克
		{
			CopyMemory(m_szSegInfo[0], szInfo, sizeof(m_szSegInfo[0])) ;

			break ;
		}
	case enMid:	//中段扑克
		{
			CopyMemory(m_szSegInfo[1], szInfo, sizeof(m_szSegInfo[1])) ;

			break ;
		}
	case enBack:	//尾端扑克
		{
			CopyMemory(m_szSegInfo[2], szInfo, sizeof(m_szSegInfo[2])) ;

			break ;
		}
	case enAllSeg:	//底牌扑克
		{
			CopyMemory(m_szSegInfo[3], szInfo, sizeof(m_szSegInfo[3])) ;	
			break ;
		}
	}

	//更新界面
	RefreshGameView() ;
}

//恢复扑克
// void CGameClientView::OnRestoreCard() 
// {
// 	AfxGetMainWnd()->SendMessage(IDM_RESTORE_CARD, 0, 0) ;
// }

void CGameClientView::SetEnd( bool bEnd )
{
	m_bEnd=bEnd;
}

void CGameClientView::SetStart( bool bStart )
{
	m_bStart=bStart;
}
void CGameClientView::SetDrawThanCard(bool bThanCard)
{
	///m_bOnOffThanCard[] = bThanCard;
}
bool CGameClientView::GetDrawThanCard()
{
	return 0;//m_bOnOffThanCard;
}

void CGameClientView::SetCompare( bool bCompare )
{
	m_bCompare=bCompare;
}

void CGameClientView::SetCompareCard( int nXPos,int nYPos,BYTE btCompareCardData[],int nCardCount )
{
	// 	AllocConsole();
	// 	freopen("CON","wt",stdout);
	m_nXPos=nXPos;
	m_nYPos=nYPos;
	CopyMemory(m_btCompareCardData,btCompareCardData,sizeof(btCompareCardData));
	m_nCardCount=nCardCount;
}

void CGameClientView::SetbDrawTimer( bool bDrawTimer )
{
	m_bDrawTimer=bDrawTimer;
}

void CGameClientView::DrawFrontCardType( CDC* pDC,int nXPos,int nYPos )
{
	int iWidth = m_ImageCardType.GetWidth()/9;
	int iHeight = m_ImageCardType.GetHeight();
	BYTE nType = m_nFrontCardType;
	if(4==m_nFrontCardType)
	{
		nType=m_nFrontCardType-1;
	}
	if(5==m_nFrontCardType)
	{
		nType=m_nFrontCardType-1;
	}
	if(6==m_nFrontCardType)
	{
		nType=m_nFrontCardType-1;
	}
	if(7==m_nFrontCardType)
	{
		nType=m_nFrontCardType-1;
	}
	if(8==m_nFrontCardType)
	{
		nType=m_nFrontCardType-2;
	}
	if(9==m_nFrontCardType)
	{
		nType=m_nFrontCardType-2;
	}
	if(10==m_nFrontCardType)
	{
		nType=m_nFrontCardType-2;
	}
	
	m_ImageCardType.DrawImage(pDC,nXPos,nYPos,iWidth,iHeight,iWidth*nType,0);
}

void CGameClientView::DrawMidCardType( CDC* pDC,int nXPos,int nYPos )
{
	int iWidth = m_ImageCardType.GetWidth()/9;
	int iHeight = m_ImageCardType.GetHeight();
	BYTE nType = m_nMidCardType;
	if(4==m_nMidCardType)
	{
		nType=m_nMidCardType-1;
	}
	if(5==m_nMidCardType)
	{
		nType=m_nMidCardType-1;
	}
	if(6==m_nMidCardType)
	{
		nType=m_nMidCardType-1;
	}
	if(7==m_nMidCardType)
	{
		nType=m_nMidCardType-1;
	}
	if(8==m_nMidCardType)
	{
		nType=m_nMidCardType-2;
	}
	if(9==m_nMidCardType)
	{
		nType=m_nMidCardType-2;
	}
	if(10==m_nMidCardType)
	{
		nType=m_nMidCardType-2;
	}

	m_ImageCardType.DrawImage(pDC,nXPos,nYPos,iWidth,iHeight,iWidth*nType,0);
}

void CGameClientView::DrawBackCardType( CDC* pDC,int nXPos,int nYPos )
{
	int iWidth = m_ImageCardType.GetWidth()/9;
	int iHeight = m_ImageCardType.GetHeight();
	BYTE nType = m_nBackCardType;
	if(4==m_nBackCardType)
	{
		nType=m_nBackCardType-1;
	}
	if(5==m_nBackCardType)
	{
		nType=m_nBackCardType-1;
	}
	if(6==m_nBackCardType)
	{
		nType=m_nBackCardType-1;
	}
	if(7==m_nBackCardType)
	{
		nType=m_nBackCardType-1;
	}
	if(8==m_nBackCardType)
	{
		nType=m_nBackCardType-2;
	}
	if(9==m_nBackCardType)
	{
		nType=m_nBackCardType-2;
	}
	if(10==m_nBackCardType)
	{
		nType=m_nBackCardType-2;
	}

	m_ImageCardType.DrawImage(pDC,nXPos,nYPos,iWidth,iHeight,iWidth*nType,0);
}

void CGameClientView::SetbDrawFrontCardType( bool bDraw,int CardType )
{
	m_bDrawFrontCardType=bDraw;
	m_nFrontCardType=CardType;
}

void CGameClientView::SetbDrawMidCardType( bool bDraw,int CardType )
{
	m_bDrawMidCardType=bDraw;
	m_nMidCardType=CardType;
}

void CGameClientView::SetbDrawBackCardType( bool bDraw,int CardType )
{
	m_bDrawBackCardType=bDraw;
	m_nBackCardType=CardType;
}

void CGameClientView::SetSGameEnd( CMD_S_GameEnd SGameEnd )
{
	///<函数得到最赢的玩家，与最失败的玩家
	m_nSGameEnd=SGameEnd;

	//m_bCompareAllCard = m_nSGameEnd.bAllSpecialCard;
	//CopyMemory(m_nShootState, m_nSGameEnd.ShootState, sizeof(m_nShootState)) ;

	CopyMemory( m_ScoreView.m_nCompare, m_nSGameEnd.cbCompareResult, sizeof( m_ScoreView.m_nCompare)) ;
	//CopyMemory(m_ScoreView.m_nCompareDouble, m_nSGameEnd.cbCompareDouble, sizeof(m_ScoreView.m_nCompareDouble)) ;
	//CopyMemory(  m_ScoreView.m_nCompareSpecial, m_nSGameEnd.cbSpecialCompareResult, sizeof(  m_ScoreView.m_nCompareSpecial)) ;
	//CopyMemory(m_ScoreView.m_cCompareKillThree, m_nSGameEnd.cbThreeKillResult, sizeof(m_ScoreView.m_cCompareKillThree)) ;
}

void CGameClientView::DrawCompareScore( CDC* pDC,int nXPos,int nYPos )
{
	CPoint sPoint[4][5];
	sPoint[0][0].x=nXPos/2-200;
	sPoint[0][0].y=40;
	sPoint[0][1].x=nXPos/2-130;
	sPoint[0][1].y=nYPos/2-280;
	sPoint[0][2].x=nXPos/2-130;
	sPoint[0][2].y=nYPos/2-240;
	sPoint[0][3].x=nXPos/2-130;
	sPoint[0][3].y=nYPos/2-200;
	sPoint[0][4].x=nXPos/2-190;
	sPoint[0][4].y=nYPos/2-160;

	sPoint[1][0].x=nXPos/2-220;
	sPoint[1][0].y=nYPos/2-170;
	sPoint[1][1].x=nXPos/2-140;
	sPoint[1][1].y=nYPos/2-70;
	sPoint[1][2].x=nXPos/2-140;
	sPoint[1][2].y=nYPos/2-30;
	sPoint[1][3].x=nXPos/2-140;
	sPoint[1][3].y=nYPos/2+10;
	sPoint[1][4].x=nXPos/2-220;
	sPoint[1][4].y=nYPos/2-80;

	sPoint[2][0].x=nXPos/2-120;
	sPoint[2][0].y=nYPos/2+60;
	sPoint[2][1].x=nXPos/2-160;
	sPoint[2][1].y=nYPos/2+120;
	sPoint[2][2].x=nXPos/2-160;
	sPoint[2][2].y=nYPos/2+160;
	sPoint[2][3].x=nXPos/2-160;
	sPoint[2][3].y=nYPos/2+200;
	sPoint[2][4].x=nXPos/2+150;
	sPoint[2][4].y=(float)nYPos*(600.0f/850.0f);

	sPoint[3][0].x=nXPos/2+200;
	sPoint[3][0].y=nYPos/2-170;
	sPoint[3][1].x=nXPos/2+110;
	sPoint[3][1].y=nYPos/2-70;
	sPoint[3][2].x=nXPos/2+110;
	sPoint[3][2].y=nYPos/2-30;
	sPoint[3][3].x=nXPos/2+110;
	sPoint[3][3].y=nYPos/2+10;
	sPoint[3][4].x=nXPos/2+190;
	sPoint[3][4].y=nYPos/2-80;
	bool bDrawPlayer[4][5];
	int nPlus=0;
	int nPlus1=0;
	CopyMemory(bDrawPlayer[0],m_bDrawPlayer1,sizeof(m_bDrawPlayer1));
	CopyMemory(bDrawPlayer[1],m_bDrawPlayer2,sizeof(m_bDrawPlayer2));
	CopyMemory(bDrawPlayer[2],m_bDrawPlayer3,sizeof(m_bDrawPlayer3));
	CopyMemory(bDrawPlayer[3],m_bDrawPlayer4,sizeof(m_bDrawPlayer4));
	int ViewChairID=0;
	if(m_bDrawSpecialCard==false)
	{
		for(int i=0;i<4;i++)
		{
			ViewChairID=GetViewChairID(i);
			if(m_bPlayer[i])
			{
				for(int j=0;j<5;j++)
				{
					CPngImage *ImageSign;
					CPngImage *ImageScore;
					if(bDrawPlayer[ViewChairID][j])
					{
						if(0==j)
						{
							if(ViewChairID==2)
							{
								if(m_nSGameEnd.cbCompareResult[i][m_nDrawPlayer[ViewChairID]]>0)
								{
									nPlus=0;
									ImageSign=&m_IBigPlusSign;
									ImageScore=&m_IPlusBigNumber;
								}
								else if(m_nSGameEnd.cbCompareResult[i][m_nDrawPlayer[ViewChairID]]<0)
								{
									nPlus=10;
									ImageSign=&m_IBigSubtractionSign;
									ImageScore=&m_IPlusBigNumber;
								}
								else
								{
									ImageScore=&m_IPlusBigNumber;
								}
								nPlus1=20;
							}

						}
						else if(4==j)
						{
							//MessageBox(NULL,"4",NULL);
							int Number=0;
							if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]==false&&bDrawPlayer[ViewChairID][3]==false)
							{
								Number=m_nSGameEnd.cbCompareResult[i][0];
							}
							if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3]==false)
							{
								Number=m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1];
							}
							if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3])
							{
								Number=m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1]+m_nSGameEnd.cbCompareResult[i][2]+m_nSGameEnd.cbSpecialCompareResult[i]
								+m_nSGameEnd.cbCompareDouble[i]+m_nSGameEnd.cbThreeKillResult[i];
							}
							if(Number>0)
							{
								nPlus=0;
								ImageSign=&m_IBigPlusSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else if(Number<0)
							{
								nPlus=10;
								ImageSign=&m_IBigSubtractionSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else
							{
								ImageScore=&m_IPlusBigNumber;
							}
							nPlus1=20;
						}
						else
						{
							if(ViewChairID==2)
							{
								if(m_nSGameEnd.cbCompareResult[i][j-1]>0)
								{
									nPlus=0;
									ImageSign=&m_ISmallPlusSign;
									ImageScore=&m_IPlusSmallNumber;
								}
								else if(m_nSGameEnd.cbCompareResult[i][j-1]<0)
								{
									nPlus=10;
									ImageSign=&m_ISmallSubtractionSign;
									ImageScore=&m_IPlusSmallNumber;
								}
								else
								{
									ImageScore=&m_IPlusSmallNumber;
								}
								nPlus1=10;
							}

						}

					}
					else
					{
						continue;
					}
					
					if(0==j)
					{
						if(ViewChairID==2)
						{
							if(m_nSGameEnd.cbCompareResult[i][m_nDrawPlayer[ViewChairID]]!=0)
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][j].x,sPoint[ViewChairID][j].y);
							}
						}

					}
					else if(4==j)
					{
						int Number=0;
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]==false&&bDrawPlayer[ViewChairID][3]==false)
						{
							Number=m_nSGameEnd.cbCompareResult[i][0];
						}
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3]==false)
						{
							Number=m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1];
						}
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3])
						{
							Number=m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1]+m_nSGameEnd.cbCompareResult[i][2]
							+m_nSGameEnd.cbSpecialCompareResult[i]+m_nSGameEnd.cbCompareDouble[i]+m_nSGameEnd.cbThreeKillResult[i];
						}
						if(Number!=0)
						{
							if(ViewChairID==2)
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][j].x,sPoint[ViewChairID][j].y);
							}
							else
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][0].x,sPoint[ViewChairID][0].y);
							}
						}
					}
					else
					{
						if(ViewChairID==2&&m_bDrawMyScore)
						{
							if(m_nSGameEnd.cbCompareResult[i][j-1]!=0)
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][j].x,sPoint[ViewChairID][j].y);
							}
						}
					}
					int Number=0;
					if(j==4)
					{
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]==false&&bDrawPlayer[ViewChairID][3]==false)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][0]);
						}
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3]==false)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1]);
						}
						if(bDrawPlayer[ViewChairID][1]&&bDrawPlayer[ViewChairID][2]&&bDrawPlayer[ViewChairID][3])
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][0]+m_nSGameEnd.cbCompareResult[i][1]+m_nSGameEnd.cbCompareResult[i][2]
							+m_nSGameEnd.cbSpecialCompareResult[i]+m_nSGameEnd.cbCompareDouble[i]+m_nSGameEnd.cbThreeKillResult[i]);
						}
					}
					else if(0==j)
					{
						if(ViewChairID==2)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][m_nDrawPlayer[ViewChairID]]);
						}
					}
					else if(1==j)
					{
						if(ViewChairID==2&&m_bDrawMyScore)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][0]);
						}
					}
					else if(2==j)
					{
						if(ViewChairID==2&&m_bDrawMyScore)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][1]);
						}
					}
					else if(3==j)
					{
						if(ViewChairID==2&&m_bDrawMyScore)
						{
							Number=labs(m_nSGameEnd.cbCompareResult[i][2]);
						}
					}
					if(ViewChairID==2||j==4)
					{
						if(j==1||j==2||j==3)
						{
							if(m_bDrawMyScore)
							{
								int Num=0;
								int Num1[3]={0,0,0};
								if(Number==0)
								{
									Num=1;
								}
								while(Number>0)
								{
									Num1[Num]=Number%10;
									Number/=10;
									Num++;
								}
								int nWidth=ImageScore->GetWidth()/10;
								if(Num==1)
								{
									if(ViewChairID==2)
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}
									else
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}

								}
								else if(Num==2)
								{
									if(ViewChairID==2)
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
										if(j==1||j==2||j==3)
										{
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+28+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
										}
										else
										{
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+45+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
										}

									}
									else
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+45+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}

								}
								else if(Num==3)
								{
									if(ViewChairID==2)
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[2]*nWidth,0);
										if(j==1||j==2||j==3)
										{
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+28+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+43+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
										}
										else
										{
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+45+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
											ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+77+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
										}

									}
									else
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[2]*nWidth,0);
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+45+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+77+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}
								}
							}	

						}
						else
						{
							int Num=0;
							int Num1[3]={0,0,0};
							if(Number==0)
							{
								Num=1;
							}
							while(Number>0)
							{
								Num1[Num]=Number%10;
								Number/=10;
								Num++;
							}
							int nWidth=ImageScore->GetWidth()/10;
							if(Num==1)
							{
								if(ViewChairID==2)
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
								}
								else
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
								}

							}
							else if(Num==2)
							{
								if(ViewChairID==2)
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
									if(j==1||j==2||j==3)
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+28+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}
									else
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+45+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}
								}
								else
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+45+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
								}

							}
							else if(Num==3)
							{
								if(ViewChairID==2)
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[2]*nWidth,0);
									if(j==1||j==2||j==3)
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+28+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+43+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}
									else
									{
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+45+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
										ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+77+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
									}

								}
								else
								{
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[2]*nWidth,0);
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+45+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[1]*nWidth,0);
									ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+77+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1[0]*nWidth,0);
								}
							}
						}	
					}
				}
			}
		}

	}
	else
	{
		for(int i=0;i<4;i++)
		{
			ViewChairID=GetViewChairID(i);
			if(m_bPlayer[i])
			{
				for(int j=0;j<5;j++)
				{
					CPngImage *ImageSign;
					CPngImage *ImageScore;
					if(bDrawPlayer[ViewChairID][j])
					{
						if(0==j&&ViewChairID==2)
						{
							if(m_nSGameEnd.cbSpecialCompareResult[i]>0)
							{
								nPlus=0;
								ImageSign=&m_IBigPlusSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else if(m_nSGameEnd.cbSpecialCompareResult[i]<0)
							{
								nPlus=10;
								ImageSign=&m_IBigSubtractionSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else
							{
								ImageScore=&m_IPlusBigNumber;
							}
							nPlus1=15;
						}
						else if(4==j)
						{
							//MessageBox(NULL,"4",NULL);
							int Number=0;
							Number=m_nSGameEnd.cbSpecialCompareResult[i];
							if(Number>0)
							{
								nPlus=0;
								ImageSign=&m_IBigPlusSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else if(Number<0)
							{
								nPlus=10;
								ImageSign=&m_IBigSubtractionSign;
								ImageScore=&m_IPlusBigNumber;
							}
							else
							{
								ImageScore=&m_IPlusBigNumber;
							}
							nPlus1=15;
						}
					}
					else
					{
						continue;
					}
					if(0==j&&ViewChairID==2)
					{
						if(m_nSGameEnd.cbSpecialCompareResult[i]!=0)
						{
                            ImageSign->DrawImage(pDC,sPoint[ViewChairID][j].x,sPoint[ViewChairID][j].y+nPlus);
						}
					}
					if(4==j)
					{
						int Number=0;
						Number=m_nSGameEnd.cbSpecialCompareResult[i];
						if(Number!=0)
						{
                            if(ViewChairID==2)
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][j].x,sPoint[ViewChairID][j].y+nPlus);
							}
							else
							{
								ImageSign->DrawImage(pDC,sPoint[ViewChairID][0].x,sPoint[ViewChairID][0].y+nPlus);
							}
						}
					}
					int Number;
					if(j==4||(0==j&&ViewChairID==2))
					{
						Number=labs(m_nSGameEnd.cbSpecialCompareResult[i]);
						int Num1=Number/10;
						int Num2=Number%10;
						int nWidth=ImageScore->GetWidth()/10;
						if(Num1==0)
						{
							if(ViewChairID==2)
							{
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num2*nWidth,0);
							}
							else
							{
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num2*nWidth,0);
							}
							
						}
						else
						{
							if(ViewChairID==2)
							{
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+13+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1*nWidth,0);
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][j].x+26+nPlus1,sPoint[ViewChairID][j].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num2*nWidth,0);
							}
							else
							{
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+13+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num1*nWidth,0);
								ImageScore->DrawImage(pDC,sPoint[ViewChairID][0].x+26+nPlus1,sPoint[ViewChairID][0].y,ImageScore->GetWidth()/10,ImageScore->GetHeight(),Num2*nWidth,0);
							}
							
						}
					}
					
				}
			}
		}
	}
}
void CGameClientView::SetbDrawPlayer( bool bDrawPlayer1[],bool bDrawPlayer2[],bool bDrawPlayer3[],bool bDrawPlayer4[])
{
	CopyMemory(m_bDrawPlayer1,bDrawPlayer1,sizeof(m_bDrawPlayer1));
	CopyMemory(m_bDrawPlayer2,bDrawPlayer2,sizeof(m_bDrawPlayer2));
	CopyMemory(m_bDrawPlayer3,bDrawPlayer3,sizeof(m_bDrawPlayer3));
	CopyMemory(m_bDrawPlayer4,bDrawPlayer4,sizeof(m_bDrawPlayer4));
	// 	for(int j=0;j<5;j++)
	// 	{
	// 		printf("m_bDrawPlayer1=%d ",m_bDrawPlayer1[j]);
	// 	}
	// 	for(int j=0;j<5;j++)
	// 	{
	// 		printf("m_bDrawPlayer2=%d ",m_bDrawPlayer2[j]);
	// 	}
	// 	for(int j=0;j<5;j++)
	// 	{
	// 		printf("m_bDrawPlayer3=%d ",m_bDrawPlayer3[j]);
	// 	}
	// 	for(int j=0;j<5;j++)
	// 	{
	// 		printf("m_bDrawPlayer4=%d ",m_bDrawPlayer4[j]);
	// 	}
}

void CGameClientView::SetnDrawPlayer( int nDrawPlayer[] )
{
	CopyMemory(m_nDrawPlayer,nDrawPlayer,sizeof(m_nDrawPlayer));
}

void CGameClientView::SetbPlayer( bool bPlayer[] )
{
	CopyMemory(m_bPlayer,bPlayer,sizeof(m_bPlayer));
}

void CGameClientView::SetnCurrentUser( int nCurrentUser )
{
	m_nCurrentUser=nCurrentUser;
}

int CGameClientView::GetViewChairID( int nCurrentUser )
{
	//printf("m_nCurrentUser=%d ",m_nCurrentUser);
	//printf("nCurrentUser=%d ",nCurrentUser);
	if(m_nCurrentUser-nCurrentUser==2||m_nCurrentUser-nCurrentUser==-2)
	{
		return 0;
	}
	if(nCurrentUser-m_nCurrentUser==-1)
	{
		return 1;
	}
	if(nCurrentUser-m_nCurrentUser==1)
	{
		return 3;
	}
	if(nCurrentUser-m_nCurrentUser==-3)
	{
		return 3;
	}
	if(nCurrentUser-m_nCurrentUser==3)
	{
		return 1;
	}
	if(m_nCurrentUser==nCurrentUser)
	{
		return 2;
	}
	return -1;
}

void CGameClientView::DrawUserInfo( CDC* pDC,int nXPos,int nYPos )
{
	TCHAR szBuffer[64];
	ZeroMemory(szBuffer,sizeof(szBuffer));
	bool bMove[2]={0,0};
	for(int i=0;i<4;i++)
	{
        if(m_bPlayer[i])
		{
			if(m_szPassScoreInfo[i]<0)
			{
               bMove[0]=true;
			   break;
			}
		}
	}
	for(int i=0;i<4;i++)
	{
		if(m_bPlayer[i])
		{
			if(m_LAllScore[i]<0)
			{
				bMove[1]=true;
				break;
			}
		}
	}
	
	int nIndex = 0;
	for(int i=0;i<4;i++)
	{
		if(m_bPlayer[i])
		{
			TCHAR szBuffer[32];
			//昵称
 			if(/*!GetShowUserInfo()&&*/i!=m_wMeChairID)
 			{
				CRect rcChip(nXPos-270,50+17*nIndex,nXPos-200,80+17*nIndex);
 				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家"));
				pDC->DrawText(szBuffer,sizeof(szBuffer),rcChip,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
 			}
 			else
			{
				CString sNameInfo = m_szUserNameInfo[i];
				if (sNameInfo.GetLength() > 12)
				{
					
					sNameInfo = sNameInfo.Mid(0,9);
					sNameInfo.Format(_T("%s"),sNameInfo);
					
				}
				CRect rcChip(nXPos-285,50+17*nIndex,nXPos-200,80+17*nIndex);
				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),sNameInfo);
				pDC->DrawText(szBuffer,sizeof(szBuffer),rcChip,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			}
			//上一局
            //_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_szPassScoreInfo[i]);
			if(bMove[0]&&m_szPassScoreInfo[i]>0)
			{
               // pDC->DrawText(nXPos-205+6,50+17*nIndex,PIXEL(240,240,240),szBuffer);
			}
			else
			{
               // pDC->DrawText(nXPos-205,50+17*nIndex,PIXEL(240,240,240),szBuffer);
			}

			//水数
			//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_szbToltalWinDaoShu[i]);
			//pDC->DrawText(nXPos-128,50+17*nIndex,PIXEL(240,240,240),szBuffer);

			//总成绩
			//_snprintf(szBuffer,sizeof(szBuffer),TEXT("%d"),m_LAllScore[i]);
			if(bMove[1]&&m_LAllScore[i]>0)
			{
				//pDC->DrawText(nXPos-80+6,50+17*nIndex,PIXEL(240,240,240),szBuffer);
			}
			else
			{
				//pDC->DrawText(nXPos-80,50+17*nIndex,PIXEL(240,240,240),szBuffer);
			}

			

			nIndex++;
		}
	}
}

void CGameClientView::GetAllUserScoreInfo(LONG szPassScoreInfo[],LONG szTaxInfo[],int szbToltalWinDaoShu[],bool bUserLeft[])
{
	CopyMemory(m_szPassScoreInfo,szPassScoreInfo,sizeof(m_szPassScoreInfo));
	CopyMemory(m_szbToltalWinDaoShu,szbToltalWinDaoShu,sizeof(m_szbToltalWinDaoShu));
	for(int i=0;i<GAME_PLAYER;i++)
	{
		m_LAllScore[i]+=m_szPassScoreInfo[i];
		m_szTaxInfo[i]=szTaxInfo[i];
		m_LAllTax[i]+=m_szTaxInfo[i];
	}
     for(int i=0;i<GAME_PLAYER;i++)
 	{
 		if(bUserLeft[i])
 		{
 			m_LAllScore[i]=0;
 			m_szPassScoreInfo[i]=0;
 			m_LAllTax[i]=0;
			m_szbToltalWinDaoShu[i]=0;
 		}
 	}
}

void CGameClientView::GetUserNameInfo( TCHAR szUserNameInfo1[],TCHAR szUserNameInfo2[],TCHAR szUserNameInfo3[],TCHAR szUserNameInfo4[] )
{
	if(m_bPlayer[0])
	{
		CopyMemory((void*)m_szUserNameInfo[0],szUserNameInfo1,30);
	}
	if(m_bPlayer[1])
	{
		CopyMemory((void*)m_szUserNameInfo[1],szUserNameInfo2,30);
	}
	if(m_bPlayer[2])
	{
		CopyMemory((void*)m_szUserNameInfo[2],szUserNameInfo3,30);
	}
	if(m_bPlayer[3])
	{
		CopyMemory((void*)m_szUserNameInfo[3],szUserNameInfo4,30);
	}

}

void CGameClientView::GetbSpecialType( bool bSpecialType[] )
{
	CopyMemory(m_bSpecialTypeview,bSpecialType,sizeof(m_bSpecialTypeview));
}

void CGameClientView::GetbDrawSpecial( bool bDrawSpecialCard )
{
	m_bDrawSpecialCard=bDrawSpecialCard;
}

void CGameClientView::SetbDrawScore( bool bDrawScore )
{
	m_bDrawScore=bDrawScore;
}

void CGameClientView::SetbSendAction( bool bSendAction )
{
	m_bSendCardAction=bSendAction;
}

void CGameClientView::SetnPoint( int xPoint[],int yPoint[] )
{
	CopyMemory(m_nXpoint,xPoint,sizeof(m_nXpoint));
	CopyMemory(m_nYpoint,yPoint,sizeof(m_nYpoint));
}

void CGameClientView::SetnCardSpace( int nCardSpace[] )
{
	CopyMemory(m_nCardSpace,nCardSpace,sizeof(m_nCardSpace));
}
void CGameClientView::SetbDrawFire( bool bDrawFire)
{
	m_bDrawFire=bDrawFire;
    //MessageBox(NULL,"hua huo",NULL);
}

void CGameClientView::DrawFire( CDC* pDC,int nXPos,int nYPos )
{
	int GunPointx[4][4]={0};
 	int GunPointy[4][4]={0};
 	int AimPointx[4][4]={0};
 	int AimPointy[4][4]={0};
 	int GunPointx1=0;
 	int GunPointy1=0;
 	int AimPointx1=0;
 	int AimPointy1=0;
 
    GunPointx[0][1]=nXPos/2-95;
 	GunPointy[0][1]=nYPos/2-300;
	GunPointx[0][2]=nXPos/2-95;
	GunPointy[0][2]=nYPos/2-320;
	GunPointx[0][3]=nXPos/2-95;
	GunPointy[0][3]=nYPos/2-320;

 	GunPointx[1][0]=nXPos/2-330;
 	GunPointy[1][0]=nYPos/2-130;
	GunPointx[1][2]=nXPos/2-370;
	GunPointy[1][2]=nYPos/2-100;
	GunPointx[1][3]=nXPos/2-330;
	GunPointy[1][3]=nYPos/2-98;

 	GunPointx[2][0]=nXPos/2-90;
 	GunPointy[2][0]=nYPos/2+110;
	GunPointx[2][1]=nXPos/2-90;
	GunPointy[2][1]=nYPos/2+80;
	GunPointx[2][3]=nXPos/2-70;
	GunPointy[2][3]=nYPos/2+80;

 	GunPointx[3][0]=nXPos/2+180;
 	GunPointy[3][0]=nYPos/2-130;
	GunPointx[3][1]=nXPos/2+180;
	GunPointy[3][1]=nYPos/2-100;
	GunPointx[3][2]=nXPos/2+190;
	GunPointy[3][2]=nYPos/2-100;
 
 	AimPointx[0][1]=nXPos/2-50;
 	AimPointy[0][1]=nYPos/2-240;
	AimPointx[0][2]=nXPos/2-45;
	AimPointy[0][2]=nYPos/2-270;
	AimPointx[0][3]=nXPos/2-50;
	AimPointy[0][3]=nYPos/2-250;

 	AimPointx[1][0]=nXPos/2-330;
 	AimPointy[1][0]=nYPos/2-80;
	AimPointx[1][2]=nXPos/2-310;
	AimPointy[1][2]=nYPos/2-40;
	AimPointx[1][3]=nXPos/2-310;
	AimPointy[1][3]=nYPos/2-60;

 	AimPointx[2][0]=nXPos/2-43;
 	AimPointy[2][0]=nYPos/2+170;
	AimPointx[2][1]=nXPos/2-40;
	AimPointy[2][1]=nYPos/2+130;
	AimPointx[2][3]=nXPos/2-50;
	AimPointy[2][3]=nYPos/2+130;

 	AimPointx[3][0]=nXPos/2+260;
 	AimPointy[3][0]=nYPos/2-100;
	AimPointx[3][1]=nXPos/2+240;
	AimPointy[3][1]=nYPos/2-60;
	AimPointx[3][2]=nXPos/2+250;
	AimPointy[3][2]=nYPos/2-50;
	BYTE nType = -1;
	printf("用户有没变化%d\n",m_nShootNum);
	m_nWinPlayer  = GetViewChairID(m_nShootState[m_nShootNum][0]);
	m_nLosePlayer = GetViewChairID(m_nShootState[m_nShootNum][1]);
	printf("赢玩家为%d,输玩家为%d\n",m_nWinPlayer,m_nLosePlayer);

	GunPointx1=GunPointx[m_nWinPlayer][m_nLosePlayer];
 	GunPointy1=GunPointy[m_nWinPlayer][m_nLosePlayer];
 	if(m_nDrawFireWhichFrame==4)
 	{
 		AimPointx1=(GunPointx[m_nWinPlayer][m_nLosePlayer]+AimPointx[m_nLosePlayer][m_nWinPlayer])/2;
 		AimPointy1=(GunPointx[m_nWinPlayer][m_nLosePlayer]+AimPointy[m_nLosePlayer][m_nWinPlayer])/2;
 	}
 	else if(m_nDrawFireWhichFrame==5)
 	{
 		AimPointx1=AimPointx[m_nLosePlayer][m_nWinPlayer];
 		AimPointy1=AimPointy[m_nLosePlayer][m_nWinPlayer];
 	}
 	if(m_nWinPlayer==0)
 	{
         if(m_nLosePlayer==1)
			 nType = 7;
 		else if(m_nLosePlayer==2)
			 nType = 5;
 		else if(m_nLosePlayer==3)
			 nType = 2;
		if(m_nDrawFireWhichFrame==1)
		{
			GunPointy1-=20;
		}
		else if(m_nDrawFireWhichFrame==2)
		{
			GunPointy1-=10;  
		}
 	}
 	if(m_nWinPlayer==1)
 	{
 		if(m_nLosePlayer==0)
			 nType =1;
 		else if(m_nLosePlayer==2)
			 nType = 2;
 		else if(m_nLosePlayer==3)
			 nType = 0;
 		if(m_nDrawFireWhichFrame==1)
 		{
 			GunPointx1-=20;
 		}
 		else if(m_nDrawFireWhichFrame==2)
 		{
 			GunPointx1-=10;  
 		}
 	}
 	if(m_nWinPlayer==2)
 	{
 		if(m_nLosePlayer==0)
			 nType = 4;
 		else if(m_nLosePlayer==1)
			 nType = 6;
 		else if(m_nLosePlayer==3)
			 nType = 1;
 		if(m_nDrawFireWhichFrame==1)
 		{
 			GunPointy1+=20;
 		}
 		else if(m_nDrawFireWhichFrame==2)
 		{
 			GunPointy1+=10;  
 		}
 	}
 	if(m_nWinPlayer==3)
 	{
 		if(m_nLosePlayer==0)
			 nType = 6;
 		else if(m_nLosePlayer==1)
			 nType = 3;
 		else if(m_nLosePlayer==2)
			 nType = 7;
 		if(m_nDrawFireWhichFrame==1)
 		{
 			GunPointx1+=20;
 		}
 		else if(m_nDrawFireWhichFrame==2)
 		{
 			GunPointx1+=10; 
 		}
 	}
	int iWidth = m_ImageGun1.GetWidth()/8;
	int iHeight = m_ImageGun1.GetHeight();
	if(m_nDrawFireWhichFrame==4)
		m_ImageGun2.DrawImage(pDC,GunPointx1,GunPointy1,iWidth,iHeight,iWidth*nType,0);
	else
	{
		if(m_nDrawFireWhichFrame<=5)
			m_ImageGun1.DrawImage(pDC,GunPointx1,GunPointy1,iWidth,iHeight,iWidth*nType,0);
	}
 	if(m_nDrawFireWhichFrame>=5)
 	{
        m_ICrash.DrawImage(pDC,AimPointx1, AimPointy1);
 	}
}

void CGameClientView::SetFireGunStatus( int nFireGunStatus )
{
	m_nFireGunStatus=nFireGunStatus;
}

void CGameClientView::SetbDrawViewCard(bool bDrawViewCard,BYTE cbSegmentCard11[],BYTE cbSegmentCard12[],BYTE cbSegmentCard13[],BYTE cbSegmentCard21[],
									   BYTE cbSegmentCard22[],BYTE cbSegmentCard23[],BYTE cbSegmentCard31[],BYTE cbSegmentCard32[],BYTE cbSegmentCard33[],
									   BYTE cbSegmentCard41[],BYTE cbSegmentCard42[],BYTE cbSegmentCard43[])
{
  
   CopyMemory(m_ViewSegmentCard[0][0],cbSegmentCard11,sizeof(BYTE)*3);
   CopyMemory(m_ViewSegmentCard[0][1],cbSegmentCard12,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[0][2],cbSegmentCard13,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[1][0],cbSegmentCard21,sizeof(BYTE)*3);
   CopyMemory(m_ViewSegmentCard[1][1],cbSegmentCard22,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[1][2],cbSegmentCard23,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[2][0],cbSegmentCard31,sizeof(BYTE)*3);
   CopyMemory(m_ViewSegmentCard[2][1],cbSegmentCard32,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[2][2],cbSegmentCard33,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[3][0],cbSegmentCard41,sizeof(BYTE)*3);
   CopyMemory(m_ViewSegmentCard[3][1],cbSegmentCard42,sizeof(BYTE)*5);
   CopyMemory(m_ViewSegmentCard[3][2],cbSegmentCard43,sizeof(BYTE)*5);
   m_bDrawViewCard=bDrawViewCard;
}

void CGameClientView::SetnDrawFireWhichFrame( int nDrawFireWhichFrame )
{
   m_nDrawFireWhichFrame=nDrawFireWhichFrame;
}

void CGameClientView::SetUser2CardSpace( int nUser2CardSpace )
{
   m_nCardSpace[2]=nUser2CardSpace;
}

void CGameClientView::GetUserWinOrLoss( int &nWinUser,int &nLossUser )
{
   nWinUser=m_nChairWin;
   nLossUser=m_nChairLoss;
}

void CGameClientView::DrawReady( CDC* pDC,int nXPos,int nYPos )
{
   m_ImageReady.DrawImage(pDC,nXPos,nYPos);
}
///<画比牌
void CGameClientView::DrawThanCard( CDC* pDC,int nXPos,int nYPos )
{
	m_IThanCard.DrawImage(pDC,nXPos,nYPos);
}
///<画背景装饰物1
void CGameClientView::DrawBackAdornment1( CDC* pDC,int nXPos,int nYPos )
{
	//m_IBackAdornment1->Blit(pDC,0,0,nXPos,nYPos,m_IBackAdornment1->GetWidth(),m_IBackAdornment1->GetHeight());
}

void CGameClientView::DrawUserClock(CDC* pDC,int nXPos,int nYPos)
{
	m_ImageClock.DrawImage(pDC,nXPos,nYPos);
}


void CGameClientView::DrawUserTimer(CDC* pDC, int nXPos, int nYPos, WORD wTime, WORD wTimerArea)
{
	if ((wTime>0)&&(wTimerArea>=wTime))
	{
		int   nDrawPos=nXPos;
		int   nTimerXPos=m_ImageTimer.GetWidth()/10;
		for (WORD nArea=10000;nArea>0;nArea/=10)
		{
			if(wTimerArea<nArea)	continue;

			int nTimerPos=wTime/nArea;
			m_ImageTimer.DrawImage(pDC,nDrawPos,nYPos,nTimerXPos,m_ImageTimer.GetHeight(),nTimerPos*nTimerXPos,0);
			wTime-=nTimerPos*nArea;
			nDrawPos+=nTimerXPos+3;
		}
	}
}
void CGameClientView::SetDragon( int wViewChairID,bool bDragon )
{
    m_bDragon[wViewChairID]=bDragon;
}

void CGameClientView::CleanUserScoreInfo( int nUserChairID )
{
     m_LAllScore[nUserChairID]=0;
 	 m_szPassScoreInfo[nUserChairID]=0;
     m_LAllTax[nUserChairID]=0;
	 m_szbToltalWinDaoShu[nUserChairID]=0;
}

void CGameClientView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bStart&&m_bSpecialTypeview[m_nCurrentUser]==false)
	{
       m_UserCardControl[2].OnLButtonDown(point);
	}
    m_HuaXian = point;
	CGameFrameView::OnLButtonDown(nFlags, point);
}

VOID CGameClientView::DrawGameView(CDC * pDC, INT nWidth, INT nHeight)
{
	TCHAR szBuffer[256]=TEXT("");
  
  	int dwCenterX = nWidth / 2;
  	int dwCenterY = nHeight / 2;
  
  	//填充背景
   	if (m_ImageViewFill.IsNull() == false)
   	{
   		int width = m_ImageViewFill.GetWidth();
   		int height = m_ImageViewFill.GetHeight();
   		for (int i=0; i<nHeight; )
   		{
   			for(int j=0; j<nWidth;)
   			{
   				m_ImageViewFill.DrawImage(pDC,j,i);				
                if(j + width >=nWidth)
                   break;
				else
                   j=j + width;
   			}
            if(i + height >=nHeight)
               break;
			else
               i=i + height;
   		}		
   	}
  	//绘制背景
   	if (m_ImageCenter.IsNull() == false)
   	{
   		m_ImageCenter.DrawImage(pDC, (nWidth-m_ImageCenter.GetWidth())/2,(nHeight-m_ImageCenter.GetHeight())/2);
	}

	// 	//绘画用户
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pClientUserItem=GetClientUserItem(i);

		//绘画用户
		if (pClientUserItem!=NULL)
		{
			BYTE cbAspect = DT_CENTER;
			//显示名字
			if(0==i) cbAspect = DT_LEFT;
			//pDC->SetTextColor(RGB(240, 240, 240));
			//pDC->TextOut(m_UserName[i].x, m_UserName[i].y, pClientUserItem->GetNickName(), lstrlen(pClientUserItem->GetNickName()));
			CRect rect( m_UserName[i].x, m_UserName[i].y,m_UserName[i].x+100, m_UserName[i].y+15);
			//DrawTextString(pDC,&m_Font,pClientUserItem->GetNickName(),RGB(240, 240, 240),RGB(255, 255, 255),rect,DT_END_ELLIPSIS|cbAspect|DT_VCENTER|DT_SINGLELINE);
			m_Font.DrawText(pDC,pClientUserItem->GetNickName(),rect,RGB(240, 240, 240),DT_END_ELLIPSIS|cbAspect|DT_VCENTER|DT_SINGLELINE);
			//显示金币
			LONGLONG lMeScore = pClientUserItem->GetUserScore();
			CString strScore;
			strScore.Format(TEXT("￥%I64d"), lMeScore);

			//pDC->TextOut(m_UserName[i].x, m_UserName[i].y+14, strScore);
			//DrawTextString(pDC,strScore,RGB(240, 240, 240),RGB(255, 255, 255),m_UserName[i].x, m_UserName[i].y+14);
			// 积分
			rect.OffsetRect(0, 15);
			//DrawTextString(pDC,&m_Font,strScore,RGB(240, 240, 240),RGB(255, 255, 255),rect,DT_END_ELLIPSIS|cbAspect|DT_VCENTER|DT_SINGLELINE);
			m_Font.DrawText(pDC,strScore,rect,RGB(240, 240, 240),DT_END_ELLIPSIS|cbAspect|DT_VCENTER|DT_SINGLELINE);
			//画比牌图片
			if(m_bOnOffThanCard[i])
			{
				DrawThanCard(pDC,m_ThanCardPoint[i].x,m_ThanCardPoint[i].y);
			}

			DrawUserAvatar(pDC,m_FacePoint[i].x,m_FacePoint[i].y,pClientUserItem);
			//时间
			if (wUserTimer!=0&&m_bClockPoint==false&&m_bDrawTimer)
			{	
				DrawUserClock(pDC,nWidth/2-29,nHeight/2-44);
				DrawUserTimer(pDC,nWidth/2-29+20,nHeight/2-20,wUserTimer,99);
			}
			if(wUserTimer!=0&&m_bClockPoint==true&&m_bDrawTimer)
			{
				DrawUserClock(pDC,nWidth/2+300,nHeight/2+160);
				DrawUserTimer(pDC,nWidth/2+320,nHeight/2+185,wUserTimer,99);
			}
		}
	}
	//画底分
	if(m_lCellScore!=0)
	{
		CString strScore;
		strScore.Format(TEXT("%I64d"), m_lCellScore);
		m_ScoreFrame.DrawImage(pDC,50,30);
		//DrawTextString(pDC,strScore,RGB(255,255,255),RGB(0,0,0),92,36);
		CRect rect(92,36,92+100,36+15);
		//DrawTextString(pDC,&m_Font,strScore,RGB(240, 240, 0),RGB(255, 255, 255),rect,DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		m_Font.DrawText(pDC,strScore,rect,RGB(240, 240, 0),DT_END_ELLIPSIS|DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		//pDC->DrawText(strScore,sizeof(strScore),CRect(50,30,150,60),DT_LEFT);
		//pDC->TextOut(50,30,strScore);
	}
  	//画边花
	//m_ImagezsFlower->Blit(pDC,0,0,0,0,m_ImagezsFlower->GetWidth(),m_ImagezsFlower->GetHeight());
	//m_ImagezxFlower->Blit(pDC,0,0,0,nHeight-m_ImagezxFlower->GetHeight(),m_ImagezxFlower->GetWidth(),m_ImagezxFlower->GetHeight());
	//m_ImageysFlower->Blit(pDC,0,0,nWidth-m_ImageysFlower->GetWidth(),0,m_ImageysFlower->GetWidth(),m_ImageysFlower->GetHeight());
	//m_ImageyxFlower->Blit(pDC,0,0,nWidth-m_ImageyxFlower->GetWidth(),nHeight-m_ImageyxFlower->GetHeight(),m_ImageyxFlower->GetWidth(),m_ImageyxFlower->GetHeight());
 	//m_ImageCard.DrawImage(pDC,nWidth/2,nHeight/2,m_ImageCard->GetWidth()/13,m_ImageCard->GetHeight()/5);

	DrawBackAdornment1(pDC,20,20);
  	if(m_bDrawFrontCardType)
  	{
  		DrawFrontCardType(pDC,nWidth/2-180,nHeight/2-100);
  	}
  	if(m_bDrawMidCardType)
  	{
  		DrawMidCardType(pDC,nWidth/2-180,nHeight/2-50);
  	}
  	if(m_bDrawBackCardType)
  	{
  		DrawBackCardType(pDC,nWidth/2-180,nHeight/2);
  	}
	//m_ImageScoreFrame.DrawImage(pDC,nWidth-m_ImageScoreFrame.GetWidth(),10);

	if(m_bShowBorder)
	{
		m_ICardFrame.DrawImage(pDC,nWidth/2-82,nHeight/2-95);
	}
	
	if(m_bOnOffTishikuang)
	{
		m_ITishikuang.DrawImage(pDC,nWidth/2-270,nHeight-76);
	}
		

	//	DrawUserInfo(pDC,nWidth,nHeight);

	if(m_bDrawShuffle)
	{
		m_UserSendCard0.ShuffleCardFlash(pDC,nWidth/2-130,nHeight/2-40);
	}
	if(m_bDrawHandCard)
	{
		m_UserSendCard0.SendCardFlash0(pDC,nWidth/2-15-m_UserSendCard0.m_nCardNum,nHeight/2-50);
	}
 

  	//绘画扑克
   	for(int i=0;i<GAME_PLAYER;i++)
   	{
 		if(m_bDrawHandCard)
 		{
			m_UserSendCard[i].SendCardFlash(pDC,nWidth/2,nHeight/2);
			m_UserCardControl[i].DrawCardFlash(pDC,m_nWidth/2-dwCenterX,m_nHeight/2-dwCenterY);
		}
   		for(int j=0;j<3;j++)
   		{
			//m_AllSegCardControl[i][j].SetDrawCard(true);
   			m_AllSegCardControl[i][j].DrawCard(pDC);
           
   			m_SegCardControl[i][j].DrawCard(pDC);   ///<画中间的牌
   		}
   		 m_CompareCarControl[i].DrawCard(pDC);    ///<画大牌
		//m_CompareCarControl[i].SetDrawCard(true);			
   		m_OverTimeCardControl[i].DrawCard(pDC);   ///<
   	}
  	if(m_bDrawFire)
  	{
  		DrawFire(pDC,nWidth,nHeight);
	}
	for (int i=0;i<GAME_PLAYER;i++)
	{
		if (m_bDrawCardKuang[i])
		{
			/*m_iPaiKuangZS++;
			if (m_iPaiKuangZS < 20)
			{*/
				m_SpecCardFlash.DrawImage(pDC,m_DrawCardKuang1[i].x,m_DrawCardKuang1[i].y);
			/*}
			else
			{
				if (m_iPaiKuangZS >= 40)
				{
					m_iPaiKuangZS = 0;
				}
			}*/
		}
	}
  	for(WORD i=0;i<GAME_PLAYER;i++)
  	{
  		if(m_bDragon[i])
  		{
  			m_ImageDragon.DrawImage(pDC,m_DragonPoint[i].x,m_DragonPoint[i].y);
  		}
  	}
	if(m_bDrawScore)
	{
		DrawCompareScore(pDC,nWidth,nHeight);
	}
	if(m_bTishi)
	{
		m_TishiFrame.DrawImage(pDC,m_TishiFramePoint.x,m_TishiFramePoint.y);
		int nWidth = m_CardSpecialType.GetWidth()/14;
		int nHeight = m_CardSpecialType.GetHeight();
		BYTE Type = -1;
		if(m_nSpecialType<=CT_FOUR_THREESAME)
			Type = m_nSpecialType-CT_THREE_STRAIGHT;
		else
			Type = m_nSpecialType-CT_THREE_STRAIGHT+1;
		m_CardSpecialType.DrawImage(pDC,m_TishiFramePoint.x+218,m_TishiFramePoint.y+82,
			nWidth,nHeight,nWidth*Type,0);
	}
	if(m_bShowFourbagger)
	{
		int iWidth = m_FourbaggerFlash.GetWidth()/14;
		int iHeight = m_FourbaggerFlash.GetHeight();
		m_Fourbagger.DrawImage(pDC,m_FourbaggerPoint[m_wAllUser].x,m_FourbaggerPoint[m_wAllUser].y+47);
		m_FourbaggerFlash.DrawImage(pDC,m_FourbaggerPoint[m_wAllUser].x,m_FourbaggerPoint[m_wAllUser].y,
			iWidth,iHeight,iWidth*m_nFourbaggerFrames,0);
	}
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wUserTimer=GetUserClock(i);
		IClientUserItem * pClientUserItem=GetClientUserItem(i);
		if (pClientUserItem!=NULL)
		{
			//举手
			if (pClientUserItem->GetUserStatus()==US_READY)
			{
				DrawReady(pDC,m_ReadyPoint[i].x,m_ReadyPoint[i].y);
			}
		}
	}
 }

void CGameClientView::SetCardInfo( int xPoint[],int yPoint[],int nCardSpace[] )
{
	m_bDrawHandCard=true;
	
	CopyMemory(m_nXpoint,xPoint,sizeof(m_nXpoint));
	CopyMemory(m_nYpoint,yPoint,sizeof(m_nYpoint));
	CopyMemory(m_nCardSpace,nCardSpace,sizeof(m_nCardSpace));
	//m_UserCardControl[0].SetCardSpace(m_nCardSpace[0],0,false);
	//m_UserCardControl[1].SetCardSpace(0,m_nCardSpace[1],false);
	m_UserCardControl[2].SetCardSpace(m_nCardSpace[2],0,20);
	//m_UserCardControl[3].SetCardSpace(0,m_nCardSpace[3],false);
	m_UserCardControl[0].SetBenchmarkPos(m_nWidth/2+m_nXpoint[0] , m_nHeight/2+m_nYpoint[0]+50 , enXLeft , enYCenter);    
	m_UserCardControl[1].SetBenchmarkPos(m_nWidth/2+m_nXpoint[1], m_nHeight/2+m_nYpoint[1]-30 , enXLeft , enYTop);
	m_UserCardControl[2].SetBenchmarkPos(m_nWidth/2+m_nXpoint[2]+5, m_nHeight/2+m_nYpoint[2] , enXLeft , enYBottom);
	m_UserCardControl[3].SetBenchmarkPos(m_nWidth/2+m_nXpoint[3],m_nHeight/2+m_nYpoint[3]-30 , enXRight , enYTop);
	RefreshGameView();
}

void CGameClientView::OnLButtonUp( UINT nFlags, CPoint Point )
{
	if(m_bStart&&m_bSpecialTypeview[m_nCurrentUser]==false)
	{
       if(m_UserCardControl[2].OnLButtonUp(Point)==-1)
	   {
		   m_btFrontCard.EnableWindow(FALSE);
		   m_btMidCard.EnableWindow(FALSE);
		   m_btBackCard.EnableWindow(FALSE);
	   }
	   RefreshGameView();
	}
	/*HDC hdc;
	hdc = ::GetDC(m_hWnd);
	MoveToEx(hdc,m_HuaXian.x,m_HuaXian.y,NULL);
	LineTo(hdc,Point.x,Point.y);
	::ReleaseDC(m_hWnd,hdc);
    RefreshGameView();*/
    CGameFrameView::OnLButtonUp(nFlags, Point);
}

void CGameClientView::OnMouseMove( UINT nFlags, CPoint point )
{
	if(m_bStart)
	{
	   m_UserCardControl[2].OnMouseMove(point);
	}
   CGameFrameView::OnMouseMove(nFlags, point);
}

//开始按钮
void CGameClientView::OnStart()
{
	//PlayGameSound(TEXT("Thirteen\\sound\\common\\bkmusic\\bkmusic.wav"));

	SendEngineMessage(IDM_START,0,0);

	//更新界面
	RefreshGameView();

	return ;
}
//设置后墩
void CGameClientView::OnBackCard()
{
	SendEngineMessage(IDM_BACKCARD,0,0);
	return ;
}

//设置中墩
void CGameClientView::OnMidCard()
{
	SendEngineMessage(IDM_MIDCARD,0,0);
	return ;
}

//设置前墩
void CGameClientView::OnFrontCard()
{
	SendEngineMessage(IDM_FRONTCARD,0,0);
	return ;
}
//回收牌1
void CGameClientView::OnRecyeleCard1()
{
	SendEngineMessage(IDM_RECYCLEFRONTCARD,0,0);
	return ;
}
//回收牌2
void CGameClientView::OnRecyeleCard2()
{
	SendEngineMessage(IDM_RECYCLEMIDCARD,0,0);
	return ;
}
//回收牌3
void CGameClientView::OnRecyeleCard3()
{
	SendEngineMessage(IDM_RECYCLEBACKCARD,0,0);
	return ;
}
//回收全部牌
void CGameClientView::OnRecyeleAllCard()
{
	SendEngineMessage(IDM_RECYCLEALLCARD,0,0);
	return ;
}
void CGameClientView::OnSubmit()
{
	SendEngineMessage(IDM_SUBMIT,0,0);
	return ;
}

void CGameClientView::OnCancel()
{
	SendEngineMessage(IDM_CANCEL,0,0);
	return ;
}
void CGameClientView::OnShowCard()
{
	SendEngineMessage(IDM_OK,0,0);
	return ;
}

void CGameClientView::OnOnePare()
{
	SendEngineMessage(IDM_ONEPARE,0,0);
	return ;
}
void CGameClientView::OnTwoPare()
{
	SendEngineMessage(IDM_TWOPARE,0,0);
	return ;
}
void CGameClientView::OnThreeSame()
{
	SendEngineMessage(IDM_THREESAME,0,0);
	return ;
}
void CGameClientView::OnStraight()
{
	SendEngineMessage(IDM_STRAIGHT,0,0);
	return ;
}
void CGameClientView::OnFlush()
{
	SendEngineMessage(IDM_FLUSH,0,0);
	return ;
}
void CGameClientView::OnGord()
{
	SendEngineMessage(IDM_GOURD,0,0);
	return ;
}
void CGameClientView::OnFourSamw()
{
	SendEngineMessage(IDM_FOURSAME,0,0);
	return ;
}
void CGameClientView::OnStraightFlush()
{
	SendEngineMessage(IDM_STRAIGHTFLUSH,0,0);
	return ;
}
void CGameClientView::OnTaxis()
{
	static BYTE nCout = 1;
	nCout = nCout%3;
	if(nCout == DESCEND_SORT)
		SendEngineMessage(IDM_DESCEND_SORT,0,0);
	else if(nCout == ASCEND_SORT)
		SendEngineMessage(IDM_ASCEND_SORT,0,0);
	else if(nCout == COLOR_SORT)
		SendEngineMessage(IDM_COLOR_SORT,0,0);
	nCout++;
	return ;
}
void CGameClientView::SetbClockPoint( bool bClcokPoint )
{
    m_bClockPoint=bClcokPoint;
}

void CGameClientView::SetbDrawMyScore( bool bDrawMyScore )
{
   m_bDrawMyScore=bDrawMyScore;
}

void CGameClientView::SetbShowCard( bool bShowCard )
{
   m_bShowCard=bShowCard;
}

void CGameClientView::SetMeChairID( WORD wMeChairID )
{
    m_wMeChairID=wMeChairID;
}


//管理员控制
void CGameClientView::OpenAdminWnd()
{
	if( !m_ClientControl.IsWindowVisible() ) 
		m_ClientControl.ShowWindow(SW_SHOW);
	else 
		m_ClientControl.ShowWindow(SW_HIDE);
}

//////////////////////////////////////////////////////////////////////////
