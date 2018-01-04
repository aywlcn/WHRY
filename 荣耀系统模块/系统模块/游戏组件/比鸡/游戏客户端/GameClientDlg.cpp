#include "Stdafx.h"
#include "GameClient.h"
#include "GameClientDlg.h"
#include "GameOption.h"

#include <mmsystem.h>
//#include <Digitalv.h>
#pragma comment(lib, "Winmm.lib")
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
 	ON_MESSAGE(IDM_START,OnStart)
// 	ON_MESSAGE(IDM_END,OnEnd)
 	ON_MESSAGE(IDM_LEFT_HIT_CARD,OnLeftHitCard)
 	ON_MESSAGE(IDM_FRONTCARD , OnFrontCard)
 	ON_MESSAGE(IDM_BACKCARD , OnBackCard)
 	ON_MESSAGE(IDM_MIDCARD , OnMidCard)
	ON_MESSAGE(IDM_OK , OnShowCard)
	ON_MESSAGE(IDM_SUBMIT , OnSubmit)
	ON_MESSAGE(IDM_CANCEL , OnCancel)
 	ON_MESSAGE(IDM_DESCEND_SORT , OnDescendSort)
 	ON_MESSAGE(IDM_ASCEND_SORT , OnAscendSort)
 	ON_MESSAGE(IDM_COLOR_SORT , OnColorSort)
// 	ON_MESSAGE(IDM_AUTO_OUT_CARD,OnAutoOutCard)
// 	ON_MESSAGE(IDM_RESTORE_CARD, OnRestoreCard)
 	ON_MESSAGE(IDM_ONEPARE,OnOnePareCard)
 	ON_MESSAGE(IDM_TWOPARE,OnTwoPareCard)
 	ON_MESSAGE(IDM_THREESAME,OnThreeSameCard)
 	ON_MESSAGE(IDM_STRAIGHT,OnStraightCard)
 	ON_MESSAGE(IDM_FLUSH,OnFlushCard)
 	ON_MESSAGE(IDM_GOURD,OnGourdCard)
 	ON_MESSAGE(IDM_FOURSAME,OnFourSameCard)
 	ON_MESSAGE(IDM_STRAIGHTFLUSH,OnStraightFlushCard)
 	ON_MESSAGE(IDM_RECYCLEFRONTCARD,OnRecycleFrontCard)
 	ON_MESSAGE(IDM_RECYCLEMIDCARD,OnRecycleMidCard)
 	ON_MESSAGE(IDM_RECYCLEBACKCARD,OnRecycleBackCard)
	ON_MESSAGE(IDM_RECYCLEALLCARD,OnRecycleAllCard)
	ON_MESSAGE(IDM_ADMIN_COMMDN,OnAdminCommand)
	ON_WM_TIMER()
//	ON_MESSAGE(WM_GAME_BASE_CTRL_BUTTON_CLICK_DOWN,OnOperateMessage)
END_MESSAGE_MAP()



//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
const int				g_nGameTime = 90 ;
bool					g_bNewStart = true ;
#endif
/////////////////////////////////////////////////////////////////////////////////
//宏定义

//结束原因
#define GER_NORMAL						0x00							//常规结束
#define GER_DISMISS						0x01							//游戏解散
#define GER_USER_LEFT					0x02							//用户离开

//游戏定时器
#define IDI_START_GAME					200								//开始定时器
#define IDI_SHOW_CARD					201								//摊牌定时器
#define IDI_COMPLETE_COMPARE			205								//完成定时器
#define IDI_COMPARE_CARD                206                             //比牌定时器
#define IDI_FIRE                        207                             //打枪定时器

#define IDT_START_TIME					30								//开始时间
#define IDT_START_TIME0					5000							//开始时间
#define IDT_COMPARE_TIME                2                               //比牌时间

#define IDT_GUN_TIME					30                              //绘制打枪消息
#define IDT_AYAKA_TIME					60                              //绘制彩花时间消息
#define IDT_FOURRBAGGETR_TIME			70								//绘制彩花时间消息

#ifdef _DEBUG
#define IDT_SHOW_TIME					15								//摊牌时间
#else
#define IDT_SHOW_TIME					40								//摊牌时间
#endif
////////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg()
{
	/*AllocConsole();
	freopen("CON","wt",stdout);*/
	//MessageBox(NULL,"Dlg gouzao",NULL);
	//游戏变量

	m_bBackGroundSound = false;
	m_bSound = false;
	m_bOnTimeEvent = false;
	m_dwCardHSpace=24;
	m_bHandCardCount=0;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard));
	m_wUserToltalChip=0;
	m_bOnOffStart = false;              ///<是否发完牌开始
	m_bFinishSegment=false;
	m_bReady = false;
	m_wDataSize=0;
	m_nPlayer=0;
	m_nWhichCard=0;
	m_bDraw=true;
	m_nSpecialType=0;
	m_bShowCard=false;
	m_bStrat=false;
	m_bCompareCard=false;
	bFirst=true;
	ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
	ZeroMemory(m_bSpecialType,sizeof(m_bSpecialType));
	ZeroMemory(m_bOverTime, sizeof(m_bOverTime));
	ZeroMemory(&m_ppBuffer,sizeof(m_ppBuffer));
	ZeroMemory(&m_Buffer,sizeof(m_Buffer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	/*m_iGameSerial=KIND_ID;*/
	ZeroMemory(m_bbThreeCard,sizeof(m_bbThreeCard));
	ZeroMemory(m_btCardType,sizeof(m_btCardType));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bAllHandCardData,sizeof(m_bAllHandCardData));
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
        m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
        m_GameClientView.m_UserCardControl[i].SetDrawCard(false);
	}
		
   
	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::OnInitGameEngine()
{
	//设置标题
	SetWindowText(TEXT("GG娱乐城欢迎您进入临武撇撇夫房间"));

	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//配置控件
	m_GameClientView.m_UserCardControl[2].SetCardSpace(m_dwCardHSpace,20,20);
	
	for(int i=0;i<GAME_PLAYER;i++)
	{
		switch(i)
		{
		case 0:m_GameClientView.m_UserSendCard[i].m_NumCard=-1;break;
		case 1:m_GameClientView.m_UserSendCard[i].m_NumCard=-3;break;
		case 2:m_GameClientView.m_UserSendCard[i].m_NumCard=-5;break;
		case 3:m_GameClientView.m_UserSendCard[i].m_NumCard=-7;break;
		}
			
	}
	m_GameClientView.m_bOnOffTishikuang = false;
	//播放声音
	CGlobalUnits *pGlobalUnits=CGlobalUnits::GetInstance();
	if ( pGlobalUnits->m_bAllowBackGroundSound )
	{
		m_bBackGroundSound = true;
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BK_MUSIC"));
	}

	return true;
}

//重置框架
bool CGameClientDlg::OnResetGameEngine()
{
	//游戏变量
 	m_bHandCardCount=0 ;
 	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
 	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
 	ZeroMemory(m_bTmpHandCardData , sizeof(m_bTmpHandCardData)) ;
 	ZeroMemory(m_btCardType,sizeof(m_btCardType));
 	m_wUserToltalChip=0;
 	m_bReady = false ;
 	ZeroMemory(&m_ppBuffer,sizeof(m_ppBuffer));
 	ZeroMemory(m_bDragon,sizeof(m_bDragon));
 	m_wDataSize=0;
 	m_nPlayer=0;
 	m_nWhichCard=0;
 	m_bDraw=true;
 	m_nSpecialType=0;
 	m_bShowCard=false;
 	m_bStrat=false;
 	m_bCompareCard=false;
 	ZeroMemory(&m_Buffer,sizeof(m_Buffer));
 	ZeroMemory(m_bSpecialType,sizeof(m_bSpecialType));
 	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
 	ZeroMemory(m_bAllHandCardData,sizeof(m_bAllHandCardData));
 	ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
 	//设置扑克
 	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
 	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
 	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
 	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
 	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;
 
 	//清空扑克
 	for (WORD i=0;i<GAME_PLAYER;i++)
 	{
 		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0) ;
 	    m_GameClientView.m_UserCardControl[i].SetDrawCard(false);
 	}

	return true;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{	//构造数据

	return;
}
//时钟删除
bool CGameClientDlg::OnEventGameClockKill(WORD wChairID)
{
	return true;
}
//时间消息
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID)
{
	static int nTime0=0;
	static int nTime1=0;
	nTime1 = GetTickCount();
	if((nTime1-nTime0)>=62000)
	{
		//PlayFmodSound(TEXT("Thirteen\\sound\\common\\bkmusic\\bkmusic.wav"));
		nTime0 = GetTickCount();
	}
	switch (wClockID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			printf("\n 开始时间 = %d",nElapse);
			if(m_bSound==false)
			{
				m_bSound = true;
				nTime0=GetTickCount();
			}
			if (nElapse==0)
			{
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
					AfxGetMainWnd()->PostMessage( WM_CLOSE, 0, 0 ) ;//时间到没有开始就退出
				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
				PlayGameSound( AfxGetInstanceHandle(), TEXT( "GAME_WARN" ) ) ;  ///<倒计时声音
            m_GameClientView.RefreshGameView();
			return true;
		}
	case IDI_SHOW_CARD:        //玩家亮牌
		{
			printf("\n 亮牌时间 = %d",nElapse);
			//printf("%d号玩家准备好比牌\n",SwitchViewChairID(wChairID));
			if (nElapse==0)
			{
				
				if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
				{
					m_GameClientView.m_btOk.EnableWindow(FALSE) ;
					m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

					if(m_bShowCard==false)
					{
						AutomaticOutCard();			
						////摊牌
						OnShowCard(0 , 0) ;
						//摊牌超时
						//SendData(SUB_C_OVERTIME) ;
					}
				}

				return false;
			}
			if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
				PlayGameSound( AfxGetInstanceHandle(), TEXT( "GAME_WARN" ) ) ;
            m_GameClientView.RefreshGameView();
			return true ;
		}
	case IDI_COMPARE_CARD:         //玩家比牌
		{
			printf("\n 比牌时间 = %d",nElapse);
			for(int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_bOnOffThanCard[i] = false;   ///<是否比牌不显示了 
			}
			
			static int Number=1;
			static int ViewChairID=0;
			static int nWhichCard=0;
			static bool bDrawPlayer1[5]={0};
			static bool bDrawPlayer2[5]={0};
			static bool bDrawPlayer3[5]={0};
			static bool bDrawPlayer4[5]={0};
			static int  nDrawPlayer[4]={0};
			static bool bDrawSpecialType[4]={0};
			static bool bDrawSpecial=false;
			static bool bDisPlay=false;
			if(bFirst)
			{
				int nDragon=0;
				for(int i=0;i<GAME_PLAYER;i++)
				{
                   if(m_bDragon[i])
				   {
                       nDragon++;
				   }
				}
				if(m_GameClientView.m_bCompareAllCard)
					bDrawSpecial=true;
				else
					bDrawSpecial=false;
			   if(bDrawSpecial)
                  bDisPlay=true;
			}
			static int  Count=0;
			static bool bDrawScore=false;
			static bool bHideCard=false;
			static int Index[4]={0};

			if(bDrawScore==false)
			{
				if(1==nElapse)
				{
					if(bHideCard)
					{
						for(int i=0;i<GAME_PLAYER;i++)
						{
							m_GameClientView.m_CompareCarControl[i].SetDrawCard(false);  
							m_GameClientView.RefreshGameView();
							m_GameClientView.m_CompareCarControl[i].SetCardData(NULL,0);
						}
						if(!bFirst)
						{
 							bDrawPlayer1[0]=0;
 							bDrawPlayer2[0]=0;
 							bDrawPlayer3[0]=0;
 							bDrawPlayer4[0]=0;

							m_GameClientView.SetbDrawPlayer(bDrawPlayer1,bDrawPlayer2,bDrawPlayer3,bDrawPlayer4);
							if(bDrawSpecial==false)
							{
								for(int i=0;i<GAME_PLAYER;i++)
								{
// 									if(m_bPlayer[i]==false)
// 									{
// 										continue;
// 									}
									int ViewChairID = SwitchViewChairID(i) ;
									if(m_bSpecialType[i]==false)
									{
                                       m_GameClientView.m_AllSegCardControl[ViewChairID][nWhichCard].SetDisplayFlag(TRUE);
									}
									
								}
							}
							else
							{
								if(bDisPlay)
								{
									for(int i=0;i<GAME_PLAYER;i++)
									{
										int ViewChairID = SwitchViewChairID(i) ;
										if(m_bSpecialType[i])
										{
											//MessageBox(NULL,"xianshi",NULL);
											m_GameClientView.m_AllSegCardControl[ViewChairID][0].SetDisplayFlag(TRUE);
											m_GameClientView.m_AllSegCardControl[ViewChairID][1].SetDisplayFlag(TRUE);
											m_GameClientView.m_AllSegCardControl[ViewChairID][2].SetDisplayFlag(TRUE);
										}
										m_GameClientView.m_AllSegCardControl[i][0].SetDisplayFlag(TRUE);
										m_GameClientView.m_AllSegCardControl[i][1].SetDisplayFlag(TRUE);
										m_GameClientView.m_AllSegCardControl[i][2].SetDisplayFlag(TRUE);
									}
								}
								else
								{
									for(int i=0;i<GAME_PLAYER;i++)
									{
										int ViewChairID = SwitchViewChairID(i) ;
										if(m_bSpecialType[i]==false)
										{
											m_GameClientView.m_AllSegCardControl[ViewChairID][nWhichCard].SetDisplayFlag(TRUE);
										}
									}
								}
							}
						}
					}
				}
				if(0==nElapse)
				{
					 
					if(!m_bDraw)
					{
						Number=1;
						ViewChairID=0;
						nWhichCard=0;
						bFirst=true;
						bDrawSpecial=false;
						Count=0;
						bDrawScore=false;
						bHideCard=false;
						bDisPlay=false;
						ZeroMemory(bDrawPlayer1,sizeof(bDrawPlayer1));
						ZeroMemory(bDrawPlayer2,sizeof(bDrawPlayer2));
						ZeroMemory(bDrawPlayer3,sizeof(bDrawPlayer3));
						ZeroMemory(bDrawPlayer4,sizeof(bDrawPlayer4));
						ZeroMemory(nDrawPlayer,sizeof(nDrawPlayer));
						ZeroMemory(bDrawSpecialType,sizeof(bDrawSpecialType));
						ZeroMemory(Index,sizeof(Index));
						KillGameClock(IDI_COMPARE_CARD);
						
                        if(m_ppBuffer.m_nXShoot>0)
						{
                           SetTimer(IDT_GUN_TIME,200,NULL);
						}
						return true;
					}
					if(m_nCurrentUser==GetMeChairID()&&m_bDraw)
					{

						if(bDrawSpecial==false)
						{
							//if(Number<=m_nPlayer)
							{
								for(int i=0;i<4;i++)
								{
									ViewChairID = SwitchViewChairID(i) ;
									if(m_bPlayer[i]==false)
									{
										continue;
									}
									//if(m_nPlayerCard[i][m_nWhichCard]==m_nPlayer-Number)
									{
										//Index[Number-1]=ViewChairID;
										if(0==m_nWhichCard)
										{
											// m_GameClientView.m_CompareCarControl[wChairID].SetbBigCard(true);
											if(0==ViewChairID)
											{
												if(m_bSpecialType[i]==false)
												{
													bDrawPlayer1[0]=1;
													bDrawPlayer1[1]=1;
													bDrawPlayer1[2]=0;
													bDrawPlayer1[3]=0;
													bDrawPlayer1[4]=1;
												}
												else
												{
													bDrawPlayer1[0]=0;
													bDrawPlayer1[1]=0;
													bDrawPlayer1[2]=0;
													bDrawPlayer1[3]=0;
													bDrawPlayer1[4]=1;
												}

											}
											if(1==ViewChairID)
											{
												if(m_bSpecialType[i]==false)
												{
													bDrawPlayer2[0]=1;
													bDrawPlayer2[1]=1;
													bDrawPlayer2[2]=0;
													bDrawPlayer2[3]=0;
													bDrawPlayer2[4]=1;
												}
												else
												{
													bDrawPlayer2[0]=0;
													bDrawPlayer2[1]=0;
													bDrawPlayer2[2]=0;
													bDrawPlayer2[3]=0;
													bDrawPlayer2[4]=1;
												}

											}
											if(2==ViewChairID)
											{
												if(m_bSpecialType[i]==false)
												{
													bDrawPlayer3[0]=1;
													bDrawPlayer3[1]=1;
													bDrawPlayer3[2]=0;
													bDrawPlayer3[3]=0;
													bDrawPlayer3[4]=1;
												}
												else
												{
													bDrawPlayer3[0]=1;
													bDrawPlayer3[1]=0;
													bDrawPlayer3[2]=0;
													bDrawPlayer3[3]=0;
													bDrawPlayer3[4]=1;
												}

											}
											if(3==ViewChairID)
											{
												if(m_bSpecialType[i]==false)
												{
													bDrawPlayer4[0]=1;
													bDrawPlayer4[1]=1;
													bDrawPlayer4[2]=0;
													bDrawPlayer4[3]=0;
													bDrawPlayer4[4]=1;
												}
												else
												{
													bDrawPlayer4[0]=0;
													bDrawPlayer4[1]=0;
													bDrawPlayer4[2]=0;
													bDrawPlayer4[3]=0;
													bDrawPlayer4[4]=1;
												}

											}
											nDrawPlayer[ViewChairID]=0;

											if(m_bDragon[i])
											{
												if(m_bSpecialType[i]==false)
												{
													m_GameClientView.SetDragon(ViewChairID,true);
													m_GameClientView.m_AllSegCardControl[ViewChairID][0].SetDisplayFlag(TRUE);
													m_GameClientView.m_AllSegCardControl[ViewChairID][1].SetDisplayFlag(TRUE);
													m_GameClientView.m_AllSegCardControl[ViewChairID][2].SetDisplayFlag(TRUE);
													bHideCard=false;
												}
											}
											else
											{
												if(m_bSpecialType[i]==false)
												{
													m_GameClientView.m_CompareCarControl[ViewChairID].SetCardData(m_bSegmentCard[i][m_nWhichCard],3);
													m_GameClientView.m_CompareCarControl[ViewChairID].SetCardType(GetCardType(i,m_nWhichCard,true,false));
													m_GameClientView.m_CompareCarControl[ViewChairID].SetDrawCard(true);
													m_GameClientView.RefreshGameView();
													bHideCard=false;
													
												}
											}
										}
										else
										{
											if(1==m_nWhichCard)
											{
												if(0==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer1[0]=1;
														bDrawPlayer1[1]=1;
														bDrawPlayer1[2]=1;
														bDrawPlayer1[3]=0;
														bDrawPlayer1[4]=1;
													}
													else
													{
														bDrawPlayer1[0]=0;
														bDrawPlayer1[1]=0;
														bDrawPlayer1[2]=0;
														bDrawPlayer1[3]=0;
														bDrawPlayer1[4]=1;
													}

												}
												if(1==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer2[0]=1;
														bDrawPlayer2[1]=1;
														bDrawPlayer2[2]=1;
														bDrawPlayer2[3]=0;
														bDrawPlayer2[4]=1;
													}
													else
													{
														bDrawPlayer2[0]=0;
														bDrawPlayer2[1]=0;
														bDrawPlayer2[2]=0;
														bDrawPlayer2[3]=0;
														bDrawPlayer2[4]=1;
													}

												}
												if(2==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer3[0]=1;
														bDrawPlayer3[1]=1;
														bDrawPlayer3[2]=1;
														bDrawPlayer3[3]=0;
														bDrawPlayer3[4]=1;
													}
													else
													{
														bDrawPlayer3[0]=1;
														bDrawPlayer3[1]=0;
														bDrawPlayer3[2]=0;
														bDrawPlayer3[3]=0;
														bDrawPlayer3[4]=1;
													}

												}
												if(3==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer4[0]=1;
														bDrawPlayer4[1]=1;
														bDrawPlayer4[2]=1;
														bDrawPlayer4[3]=0;
														bDrawPlayer4[4]=1;
													}
													else
													{
														bDrawPlayer4[0]=0;
														bDrawPlayer4[1]=0;
														bDrawPlayer4[2]=0;
														bDrawPlayer4[3]=0;
														bDrawPlayer4[4]=1;
													}

												}
												nDrawPlayer[ViewChairID]=1;
											}
											if(2==m_nWhichCard)
											{
												if(0==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer1[0]=1;
														bDrawPlayer1[1]=1;
														bDrawPlayer1[2]=1;
														bDrawPlayer1[3]=1;
														bDrawPlayer1[4]=1;
													}
													else
													{
														bDrawPlayer1[0]=0;
														bDrawPlayer1[1]=0;
														bDrawPlayer1[2]=0;
														bDrawPlayer1[3]=0;
														bDrawPlayer1[4]=1;
													}

												}
												if(1==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer2[0]=1;
														bDrawPlayer2[1]=1;
														bDrawPlayer2[2]=1;
														bDrawPlayer2[3]=1;
														bDrawPlayer2[4]=1;
													}
													else
													{
														bDrawPlayer2[0]=0;
														bDrawPlayer2[1]=0;
														bDrawPlayer2[2]=0;
														bDrawPlayer2[3]=0;
														bDrawPlayer2[4]=1;
													}

												}
												if(2==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer3[0]=1;
														bDrawPlayer3[1]=1;
														bDrawPlayer3[2]=1;
														bDrawPlayer3[3]=1;
														bDrawPlayer3[4]=1;
													}
													else
													{
														bDrawPlayer3[0]=1;
														bDrawPlayer3[1]=0;
														bDrawPlayer3[2]=0;
														bDrawPlayer3[3]=0;
														bDrawPlayer3[4]=1;
													}

												}
												if(3==ViewChairID)
												{
													if(m_bSpecialType[i]==false)
													{
														bDrawPlayer4[0]=1;
														bDrawPlayer4[1]=1;
														bDrawPlayer4[2]=1;
														bDrawPlayer4[3]=1;
														bDrawPlayer4[4]=1;
													}
													else
													{
														bDrawPlayer4[0]=0;
														bDrawPlayer4[1]=0;
														bDrawPlayer4[2]=0;
														bDrawPlayer4[3]=0;
														bDrawPlayer4[4]=1;
													}

												}
												nDrawPlayer[ViewChairID]=2;
											}
                                            if(m_bDragon[i])
											{
                                                if(m_bSpecialType[i]==false)
												{
                                                    bHideCard=false;
												}
											}
											else
											{
												if(m_bSpecialType[i]==false)
												{
													m_GameClientView.m_CompareCarControl[ViewChairID].SetCardData(m_bSegmentCard[i][m_nWhichCard],5);
													bool bMidCard=false;
													if(m_nWhichCard==1)
													{
														bMidCard=true;
													}
													m_GameClientView.m_CompareCarControl[ViewChairID].SetCardType(GetCardType(i,m_nWhichCard,false,bMidCard));
													m_GameClientView.m_CompareCarControl[ViewChairID].SetDrawCard(true);
													m_GameClientView.RefreshGameView();
													bHideCard=false;
													
												}
											}
											
										} 
										bFirst=false;
										nWhichCard=m_nWhichCard;
									}
								}
								m_nWhichCard++;
								Number=1;	
								bDrawScore=true;
							}
						}
						else
						{
							//MessageBox(NULL,"ta",NULL);
							for(int i=0;i<4;i++)
							{
								ViewChairID = SwitchViewChairID(i) ;

								if(m_bPlayer[i]==false)
								{
									continue;
								}
								if(m_bSpecialType[i]||m_bDragon[i])
								{
									//if(m_nSpecialCard[i]==Count)
									{
										//Index[Count]=ViewChairID;
										if(0==ViewChairID)
										{

											bDrawPlayer1[0]=0;
											bDrawPlayer1[1]=1;
											bDrawPlayer1[2]=1;
											bDrawPlayer1[3]=1;
											bDrawPlayer1[4]=1;

										}
										if(1==ViewChairID)
										{
											bDrawPlayer2[0]=0;
											bDrawPlayer2[1]=1;
											bDrawPlayer2[2]=1;
											bDrawPlayer2[3]=1;
											bDrawPlayer2[4]=1;
										}
										if(2==ViewChairID)
										{
											bDrawPlayer3[0]=1;
											bDrawPlayer3[1]=1;
											bDrawPlayer3[2]=1;
											bDrawPlayer3[3]=1;
											bDrawPlayer3[4]=1;
										}
										if(3==ViewChairID)
										{
											bDrawPlayer4[0]=0;
											bDrawPlayer4[1]=1;
											bDrawPlayer4[2]=1;
											bDrawPlayer4[3]=1;
											bDrawPlayer4[4]=1;
										}
										m_GameClientView.GetbDrawSpecial(false);
										m_GameClientView.SetbDrawPlayer(bDrawPlayer1,bDrawPlayer2,bDrawPlayer3,bDrawPlayer4);
										if(m_bDragon[i])
										{
											if(m_bSpecialType[i]==false)
											{
												m_GameClientView.SetDragon(ViewChairID,true);
												m_GameClientView.m_AllSegCardControl[ViewChairID][0].SetDisplayFlag(TRUE);
												m_GameClientView.m_AllSegCardControl[ViewChairID][1].SetDisplayFlag(TRUE);
												m_GameClientView.m_AllSegCardControl[ViewChairID][2].SetDisplayFlag(TRUE);
											}
										}
										if(m_bSpecialType[i])
										{
											ZeroMemory(m_GameClientView.m_bDrawCardKuang,sizeof(m_GameClientView.m_bDrawCardKuang));
											m_GameClientView.m_CompareCarControl[ViewChairID].SetCardData(m_bSpecialCardData[i],13);
											m_GameClientView.m_CompareCarControl[ViewChairID].SetCardType(GetCardType(i,m_nWhichCard,true,false));
											m_GameClientView.m_CompareCarControl[ViewChairID].SetDrawCard(true);
											m_GameClientView.m_CompareCarControl[ViewChairID].m_bAyaka=true;
											SetTimer(IDT_AYAKA_TIME,160,NULL);
											m_GameClientView.RefreshGameView();
											
										}
										Count++;
										bFirst=false;
									}
								}	
							}
							bDisPlay=true;
							bDrawScore=true;
						}
						if(bDrawSpecial==false)
						{
							if(m_nWhichCard<=3)
							{
								if(m_nWhichCard==3)
								{
									if(m_nSpecialType==0)
									{
										m_bDraw=false;
									}
									else
									{
										bDrawSpecial=true;
										m_bDraw=true;
									}
								}
								SetGameClock(GetMeChairID(),IDI_COMPARE_CARD,IDT_COMPARE_TIME); 
								printf("进入时间1\n");
							}
						}
						else 
						{
							m_bDraw=false;
							SetGameClock(GetMeChairID(),IDI_COMPARE_CARD,IDT_COMPARE_TIME); 
							printf("进入时间2\n");
						}
					}
				}
			}
			else
			{
				if(0==nElapse)
				{
					m_GameClientView.GetbDrawSpecial(false);
					m_GameClientView.SetbDrawPlayer(bDrawPlayer1,bDrawPlayer2,bDrawPlayer3,bDrawPlayer4);
					m_GameClientView.SetnDrawPlayer(nDrawPlayer);
					
					bDrawScore=false;
					bHideCard=true;
					SetGameClock(GetMeChairID(),IDI_COMPARE_CARD,IDT_COMPARE_TIME); 
					printf("进入时间3\n");
				}
			}
		}
	}
	return true;
}


bool CGameClientDlg::OnEventLookonMode(VOID * pData, WORD wDataSize)
{
	return true;
}

//网络消息
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{	
	switch (wSubCmdID)
	{
	case SUB_S_SEND_CARD:		//发送扑克
		{
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_SETSEGMENT:		//设置分段
		{
			return OnSubSetSegment(pData , wDataSize) ;
		}
	case SUB_S_SHOW_CARD:      //玩家摊牌
		{
			//MessageBox(NULL,"tanpai",NULL);
			return OnSubShowCard(pData , wDataSize) ;
		}
	case SUB_S_PLAYER_LEFT:     //玩家强退
		{
            return OnSubPlayerLeft(pData , wDataSize) ;
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			CMD_S_GameEnd GameEnd=*(CMD_S_GameEnd*)pData;
            if(GameEnd.bLeft)
			{
                return OnSubGameEnd(pData,wDataSize);
			}
			else
			{
				m_bCompareCard=true;
                return ComparePlayerCard(pData , wDataSize);
			}
			
		}	
	case SUB_S_SCORE_RESULT:     //游戏结算
		{
            CMD_S_ScoreResult *ScoreResult = (CMD_S_ScoreResult *)pData;
			if(ScoreResult->bScoreResult)
			{
               OnSubGameEnd(&m_ppBuffer,m_wDataSize);
			}
			return true;
		}
	case SUB_S_USEREXIT:         //玩家退出
		{
			CMD_S_UserExit *UserExit=(CMD_S_UserExit *)pData;
            m_GameClientView.CleanUserScoreInfo(UserExit->nLeftChairID);
			return true;
		}
	case SUB_S_AMDIN_COMMAND:
		{
			return OnControlPrompt(pData,wDataSize);
		}
	}

	return false;
}

//游戏场景
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:	//空闲状态
		{
			//设置界面

			CMD_S_StatusFree *pStatusFree=(CMD_S_StatusFree*)pData ;
			m_wUserToltalChip = pStatusFree->wUserToltalChip ;
			

			// 控制按钮
			if( CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}
			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.EnableWindow(TRUE);

				SetGameClock(GetMeChairID(),IDI_START_GAME,IDT_START_TIME);

				printf("\n 本人ID = %d",GetMeChairID());
				m_GameClientView.SetnCurrentUser(GetMeChairID());
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
			}
			m_GameClientView.SetMeChairID(GetMeChairID());
			KillTimer(40);
			return true;
		}

	case GS_WK_PLAYING:		//游戏状态
		{

			// 控制按钮
			if( CUserRight::IsGameCheatUser(m_pIClientKernel->GetUserAttribute()->dwUserRight))
			{
				m_GameClientView.m_btOpenAdmin.ShowWindow(SW_SHOW);
			}

			CMD_S_StatusPlay *pStatusPlay = (CMD_S_StatusPlay *)pData;

			int xPoint[4]={0};
			int yPoint[4]={0};
			int nCardSpace[4]={0};

			xPoint[0]=-174;
			yPoint[0]=-310;
			xPoint[1]=-295;
			yPoint[1]=-157;
			xPoint[2]=-174;
			yPoint[2]=100;
			xPoint[3]=245;
			yPoint[3]=-157;

			nCardSpace[0]=24;
			nCardSpace[1]=18;
			nCardSpace[2]=24;
			nCardSpace[3]=18;
			//清理
			m_GameClientView.SetCardInfo(xPoint,yPoint,nCardSpace);
			m_GameClientView.HideButton();
			m_GameClientView.m_bOnOffTishikuang=false;
			m_GameClientView.m_bShowBorder=false;
			m_GameClientView.m_UserSendCard0.m_nCardNum = 0;
			ZeroMemory(m_GameClientView.m_bOnOffThanCard,sizeof(m_GameClientView.m_bOnOffThanCard));

			BYTE bCardData[13] ;
			memset(bCardData , 0 , 13) ;
			//其他玩家
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IClientUserItem * pUserData=GetTableUserItem(i);
				WORD wChairID = SwitchViewChairID(i);
				
				//设置背景
				if(NULL!=pUserData && pUserData->GetUserStatus()==US_PLAYING)
				{
					printf("\n >>>>>>>>>用户数 %d ？",i);
					if (false==pStatusPlay->bFinishSegment[i])
					{
						printf("\n >>>>>>>>>没摊牌的 %d ？",i);
						m_GameClientView.SetCardData(wChairID , bCardData , HAND_CARD_COUNT , enHandCard);
						m_GameClientView.m_UserCardControl[wChairID].m_nI=HAND_CARD_COUNT;  ///<几个玩家一共发多少牌
						m_GameClientView.m_UserCardControl[wChairID].SetDrawCard(true);
						m_GameClientView.m_UserCardControl[wChairID].SetDisplayFlag(true);
					}
					//m_nPlayer++;
					m_bPlayer[i]=true;
				}
				else
				{
					m_bPlayer[i]=false;
				}
			}
			m_GameClientView.SetbPlayer(m_bPlayer);
			m_GameClientView.m_bDrawScore = true;

			m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
			m_GameClientView.m_btStart.EnableWindow(FALSE);
			//m_GameClientView.m_btEnd.ShowWindow(SW_HIDE);
			//m_GameClientView.m_btEnd.EnableWindow(FALSE);

			//最高下注
			m_wUserToltalChip=pStatusPlay->wUserToltalChip ;
			//单元积分
			m_GameClientView.m_lCellScore=pStatusPlay->lCellScore;

			bool bFinishiShowCard = true ;
			//完成摊牌
			for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
			{
				IClientUserItem * pClientUserItem=GetTableUserItem(wID);
				if(pClientUserItem && m_bPlayer[wID]) 
				{
					if(false==pStatusPlay->bFinishSegment[wID])
					{
						bFinishiShowCard = false ;
						break ;
					}
				}
			}
			AfxGetApp()->m_hInstance;
			//超时状态
			CopyMemory(m_bOverTime, pStatusPlay->bOverTime, sizeof(m_bOverTime)) ;
			CopyMemory(m_bHandCardData , pStatusPlay->bHandCardData , sizeof(m_bHandCardData)) ;
			CopyMemory(m_bTmpHandCardData , m_bHandCardData , sizeof(m_bHandCardData)) ;
			CopyMemory(m_bAllHandCardData,pStatusPlay->bAllHandCardData,sizeof(m_bAllHandCardData));
			for(int i=0;i<4;i++)
			{
				m_GameLogic.SortCardList(m_bAllHandCardData[i],13,enDescend);
			}
			for(int i=0;i<4;i++)
			{
				if(m_bPlayer[i])
				{
					if(m_GameLogic.GetCardType(m_bAllHandCardData[i],13,m_GameLogic.btCardSpecialData)!=CT_INVALID && pStatusPlay->bSpecialTypeTable1[i])
					{
						m_bSpecialType[i]=true;
						CopyMemory(m_bSpecialCardData[i],m_bAllHandCardData[i],sizeof(m_bSpecialCardData[i]));
					}
					else
					{
						m_bSpecialType[i]=false;
					}
				}
			}
			bool bSpecialType[GAME_PLAYER];
			ZeroMemory(bSpecialType,sizeof(bSpecialType));
			for(int i=0;i<4;i++)
			{
				if(m_bPlayer[i])
				{
					if(m_GameLogic.GetCardType(m_bAllHandCardData[i],13,m_GameLogic.btCardSpecialData)!=CT_INVALID)
					{
						bSpecialType[i]=true;
					}
					else
					{
						bSpecialType[i]=false;
					}
				}
			}
			m_GameClientView.GetbSpecialType(bSpecialType);
			m_bHandCardCount = HAND_CARD_COUNT ;
			WORD wMeID = pStatusPlay->wCurrentUser;
			printf("\n >>>>>>>>>>>> 场景当前ID = %d",wMeID);
			/*if(false==pStatusPlay->bFinishSegment[wMeID] && !IsLookonMode())
			{
				m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,enDescend);
				m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard);
			}*/
			if(false==IsLookonMode())
			{
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(true);
				m_GameClientView.m_UserCardControl[2].SetPositively(true);

				if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
				{
					m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
				}
				else
				{
					//SetGameClock(wMeID , IDI_SHOW_CARD , IDT_SHOW_TIME);
					m_nCurrentUser=wMeID;
					m_GameClientView.SetnCurrentUser(wMeID);
					/*m_GameClientView.SetStart(true);
					m_GameClientView.m_bShowBorder = true;
					m_GameClientView.m_bOnOffTishikuang = TRUE;
					m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
					m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
					m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;*/

					/*m_GameClientView.m_btOnePare.ShowWindow(TRUE);
					m_GameClientView.m_btOnePare.EnableWindow(FALSE);
					m_GameClientView.m_btTwoPare.ShowWindow(TRUE);
					m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
					m_GameClientView.m_btThreeSame.ShowWindow(TRUE);
					m_GameClientView.m_btThreeSame.EnableWindow(FALSE);
					m_GameClientView.m_btStraight.ShowWindow(TRUE);
					m_GameClientView.m_btStraight.EnableWindow(FALSE);
					m_GameClientView.m_btFlush.ShowWindow(TRUE);
					m_GameClientView.m_btFlush.EnableWindow(FALSE);
					m_GameClientView.m_btGourd.ShowWindow(TRUE);
					m_GameClientView.m_btGourd.EnableWindow(FALSE);
					m_GameClientView.m_btFourSame.ShowWindow(TRUE);
					m_GameClientView.m_btFourSame.EnableWindow(FALSE);
					m_GameClientView.m_btStraightFlush.ShowWindow(TRUE);
					m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
					m_GameClientView.m_btRecycleCard1.ShowWindow(TRUE);
					m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
					m_GameClientView.m_btRecycleCard2.ShowWindow(TRUE);
					m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
					m_GameClientView.m_btRecycleCard3.ShowWindow(TRUE);
					m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
					m_GameClientView.m_btRecycleAllCard.ShowWindow(TRUE);
					m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
					SetTimer(20,20,NULL);*/
				}
			}
			else	
			{
				m_nCurrentUser=wMeID;
				m_GameClientView.SetnCurrentUser(wMeID);
				m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false);
				m_GameClientView.m_UserCardControl[2].SetPositively(false);
				m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
				m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
				m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
			}

			CopyMemory(m_bSegmentCard , pStatusPlay->bSegmentCard , sizeof(m_bSegmentCard)) ;

			//没有摊牌

			//if(true==pStatusPlay->bFinishSegment[GetMeChairID()])
			//{
				for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
				{
					IClientUserItem * pUserData=GetTableUserItem(wID);
					WORD wChairID = SwitchViewChairID(wID) ;
					if(NULL!=pUserData)
					{
						//摊牌扑克
						if(true==pStatusPlay->bFinishSegment[wID])
						{
							m_GameClientView.m_bOnOffThanCard[wChairID] = true;
							for(BYTE j=0 ; j<3 ; ++j)
							{
								if(0==j)
									m_GameClientView.SetCardData(wChairID , m_bSegmentCard[wID][j] , 3 , enAllSegmentCard , j);
								else
									m_GameClientView.SetCardData(wChairID , m_bSegmentCard[wID][j] , 5 , enAllSegmentCard , j);
								//if(true==IsLookonMode())
								m_GameClientView.m_AllSegCardControl[wChairID][j].SetDrawCard(true);
								m_GameClientView.m_AllSegCardControl[wChairID][j].SetDisplayFlag(false) ;
							}
						}
					}
				}
			/*}
			else
			{
				for(WORD wID = 0 ; wID<GAME_PLAYER ; ++wID)
				{
					IClientUserItem * pClientUserItem=GetTableUserItem(wID);

					if(NULL!=pClientUserItem)
					{
						if(wID!=GetMeChairID())
						{
							BYTE bCardData[13] ;
							memset(bCardData , 0 , 13) ;
							m_GameClientView.SetCardData(SwitchViewChairID(wID) , bCardData , 13 , enHandCard) ;
						}
					}
				}
				}*/
			memset(&m_ppBuffer,0,sizeof(m_ppBuffer));
			m_GameClientView.m_ScoreView.ResetScore();
			m_wDataSize	= 0;
			m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
			if(true==bFinishiShowCard)
			{
				//结束框
				m_ppBuffer=pStatusPlay->sGameEnd;
				m_wDataSize=sizeof(m_ppBuffer);
				m_GameClientView.SetSGameEnd(m_ppBuffer);//设置积分
				bool IsLeft=false;
				TCHAR szBuffer[32];
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					IClientUserItem *pClientUserItem = GetTableUserItem(i);
					if(NULL!=pClientUserItem && m_bPlayer[i])
					{
						if(i!=GetMeChairID())
						{
							_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),pClientUserItem->GetNickName());
							m_GameClientView.m_ScoreView.SetGameScore(i,szBuffer,m_ppBuffer.lGameScore[i],m_ppBuffer.bToltalWinDaoShu[i]);
						}
						else
						{
							m_GameClientView.m_ScoreView.SetGameScore(i,pClientUserItem->GetNickName(),m_ppBuffer.lGameScore[i],m_ppBuffer.bToltalWinDaoShu[i]);
						}
					}
					if(m_ppBuffer.bUserLeft[i])
					{
						IsLeft=true;
						m_GameClientView.m_ScoreView.SetGameScore(i,m_ppBuffer.LeftszName[i],m_ppBuffer.lGameScore[i],m_ppBuffer.bToltalWinDaoShu[i]);
					}
				}
				if(IsLeft)
				{
					m_GameClientView.SetSGameEnd(m_ppBuffer);
				}
				m_GameClientView.m_ScoreView.SetGameTax(m_ppBuffer.lGameEveryTax);
				m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
				//显示牌
				m_GameClientView.m_UserCardControl[2].SetPositively(false);
				for (int ii=0;ii<GAME_PLAYER;ii++)
				{
					WORD wChairID = SwitchViewChairID(ii) ;
					m_GameClientView.m_bOnOffThanCard[wChairID] = false;
					for (int jj=0;jj<3;jj++)
					{
						m_GameClientView.m_AllSegCardControl[wChairID][jj].SetDisplayFlag(true) ;
					}
				}
				KillGameClock(IDI_SHOW_CARD);
				if (!IsLookonMode())
				{
					SetGameStatus(GS_WK_FREE);
				}
			}
			//设置按钮
			//if(false==IsLookonMode())
			//{
			//	if(false==pStatusPlay->bFinishSegment[GetMeChairID()])
			//	{
			//		m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
			//		m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
			//		m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
			//		m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
			//		m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
			//		m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

			//		//m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
			//		//m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE) ;

			//		//设置时间
			//		SetGameClock(GetMeChairID() , IDI_SHOW_CARD , IDT_SHOW_TIME);
			//		//		SetGameClock(GetMeChairID() , IDI_SHOW_CARD , 5) ;
			//	}
			//}
			m_GameClientView.RefreshGameView();
		}
	}	
	return true;
}
//声音控制
bool CGameClientDlg::AllowBackGroundSound(bool bAllowSound)
{
	if(bAllowSound && !m_bBackGroundSound )
	{
		PlayBackGroundSound(AfxGetInstanceHandle(), TEXT("BK_MUSIC"));
	}
	else if ( !bAllowSound )
	{
		StopSound();
		m_bBackGroundSound = false;
	}

	return true;
}

//用户进入
VOID CGameClientDlg::OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_GameClientView.m_ClientControl.AddUserBx(pIClientUserItem->GetNickName()); 
}
//用户离开
VOID CGameClientDlg::OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser)
{
	m_GameClientView.m_ClientControl.DeleteUserBx(pIClientUserItem->GetNickName()); 
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//删除定时器
	if(!IsLookonMode())
		KillGameClock(IDI_START_GAME);
	m_bStrat=true;
	m_GameClientView.m_lCellScore = 0;
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;
	//发送消息
	if(!IsLookonMode())
		SendUserReady(NULL,0);
	SetTimer(40,50,NULL);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btEnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btEnd.EnableWindow(FALSE);

	m_bReady = true ;
	m_GameClientView.SetEnd(false);
	ZeroMemory(m_bOverTime, sizeof(m_bOverTime)) ;
	ZeroMemory(&m_bType,sizeof(m_bType));

	//设置界面
	m_GameClientView.SetbDrawFire(false);
	KillTimer(IDT_GUN_TIME);
	KillTimer(IDI_START_GAME);
	m_GameClientView.ShowSegInfo(enErr, NULL) ;

	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);
	for(BYTE i=0; i<GAME_PLAYER; ++i) m_GameClientView.m_OverTimeCardControl[i].SetCardData(NULL, 0) ;

	//设置按钮
	//m_GameClientView.m_btStartAgain.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btStartAgain.EnableWindow(FALSE);
	
	//m_GameClientView.m_btOnePare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOnePare.EnableWindow(FALSE);
	//m_GameClientView.m_btTwoPare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
	//m_GameClientView.m_btThreeSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeSame.EnableWindow(FALSE);  
	//m_GameClientView.m_btStraight.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraight.EnableWindow(FALSE);
	//m_GameClientView.m_btFlush.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFlush.EnableWindow(FALSE);
	//m_GameClientView.m_btGourd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGourd.EnableWindow(FALSE);
	//m_GameClientView.m_btFourSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFourSame.EnableWindow(FALSE);
	//m_GameClientView.m_btStraightFlush.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard1.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard2.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard3.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
	
	//m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	//m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	//m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	//m_GameClientView.m_btOk.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOk.EnableWindow(FALSE);
	
	
	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0) ;
		m_GameClientView.m_UserCardControl[i].SetDrawCard(false);
	}
	return 0;
}

//发送扑克
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);
	m_GameClientView.m_DrawGun.SetCardData(NULL , 0 );
	m_GameClientView.m_ScoreView.ResetScore();
	m_GameClientView.ResetViewData(IsLookonMode());
	//m_GameClientView.ResetGameView();
	//ResetGameFrame();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStart.EnableWindow(FALSE);
	m_GameClientView.m_btEnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btEnd.EnableWindow(FALSE);
	ZeroMemory(m_bbThreeCard,sizeof(m_bbThreeCard));
	ZeroMemory(m_bSegmentCard,sizeof(m_bSegmentCard));
	//初始化
	//设置界面
	KillTimer(IDI_START_GAME);

	//m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	//m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE);
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<3;j++)
		{
			m_GameClientView.m_SegCardControl[i][j].SetDrawCard(false);
            m_GameClientView.m_AllSegCardControl[i][j].SetDrawCard(false);
		}
		m_GameClientView.m_CompareCarControl[i].SetDrawCard(false);
	    m_GameClientView.m_OverTimeCardControl[i].SetDrawCard(false);
	}

	//	m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW);
	//	m_GameClientView.m_btRestoreCard.EnableWindow(FALSE);

	m_GameClientView.m_ScoreView.ShowWindow(SW_HIDE);



	//设置扑克
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enAllSegmentCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enCompareCard) ;
	m_GameClientView.SetCardData(INVALID_CHAIR , NULL , 0 , enBankCard) ;

	//清空扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_UserCardControl[i].SetCardData(NULL,0);
		m_GameClientView.m_UserCardControl[i].SetDrawCard(false);
	}



	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//设置状态
	SetGameStatus(GS_WK_PLAYING);

	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;
	CopyMemory(m_bAllHandCardData,pSendCard->bAllHandCardData,sizeof(m_bAllHandCardData));
	for(int i=0;i<4;i++)
	{
		m_GameLogic.SortCardList(m_bAllHandCardData[i],13);
	}

	m_bHandCardCount = HAND_CARD_COUNT ;

	m_nCurrentUser=pSendCard->wCurrentUser;
	m_GameClientView.m_lCellScore=pSendCard->lCellScore;

	m_GameClientView.SetnCurrentUser(m_nCurrentUser);
	// 	AllocConsole();
	// 	freopen("CON","wt",stdout);
	//设置数据
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(pSendCard->bCardData));
	CopyMemory(m_bTmpHandCardData , m_bHandCardData , sizeof(m_bHandCardData)) ;

	m_bFinishSegment=false ; 
	m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount,enDescend);

	BYTE bCardData[13] ;
	memset(bCardData , 0 , 13) ;

	//手上扑克
	m_GameClientView.SetCardData(2, m_bHandCardData , HAND_CARD_COUNT , enHandCard);
	m_GameClientView.m_UserCardControl[2].SetDrawCard(true);

	//其他玩家
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem * pBankerData = GetTableUserItem(i);

		WORD wChairID = SwitchViewChairID(i) ;

		//设置背景
		if(NULL!=pBankerData && GetMeChairID()!=i)
		{
			m_GameClientView.SetCardData(wChairID , bCardData , HAND_CARD_COUNT , enHandCard);
			m_GameClientView.m_UserCardControl[wChairID].SetDrawCard(true);
		}
	}
	if (IsLookonMode()==true) 
	{
		m_GameClientView.m_UserCardControl[2].SetDisplayFlag(false);
	}
	CopyMemory(&m_Buffer,pBuffer,sizeof(m_Buffer));
	//m_GameClientView.m_DrawGun.SetCardData(bCardData,HAND_CARD_COUNT);
	//m_GameClientView.m_DrawGun.ShowWindow(SW_SHOW);

	//设置按钮
 	if (pSendCard->wCurrentUser==GetMeChairID())
 	{
		m_GameClientView.m_UserCardControl[2].SetPositively(true);
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false);

		for(int i=0;i<4;i++)
		{
			IClientUserItem * pBankerData = GetTableUserItem(i);
			if(NULL!=pBankerData)
			{
				m_nPlayer++;
				m_bPlayer[i]=true;
			}
			else
			{
				m_bPlayer[i]=false;
			}
		}
		m_GameClientView.m_UserSendCard0.m_nCardNum=m_nPlayer*13;  ///<几个玩家一共发多少牌
		printf("\n 玩家数量 ： %d",m_nPlayer);
		m_GameClientView.SetbPlayer(m_bPlayer);
		bool bSpecialType[GAME_PLAYER]={0};
		for(int i=0;i<4;i++)
		{
			if(m_bPlayer[i])
			{
				if(m_GameLogic.GetCardType(m_bAllHandCardData[i],13,m_GameLogic.btCardSpecialData)!=CT_INVALID)
				{
					bSpecialType[i]=true;
				}
				else
				{
					bSpecialType[i]=false;
				}
			}
		}
		m_GameClientView.GetbSpecialType(bSpecialType);
 	}
	IClientUserItem * pUserData1 = GetTableUserItem(0);
	IClientUserItem * pUserData2 = GetTableUserItem(1);
	IClientUserItem * pUserData3 = GetTableUserItem(2);
	IClientUserItem * pUserData4 = GetTableUserItem(3);
	TCHAR ch1[64] = TEXT("");
	TCHAR ch2[64] = TEXT("");
	TCHAR ch3[64] = TEXT("");
	TCHAR ch4[64] = TEXT("");
	if(pUserData1 != NULL)
		_sntprintf(ch1,sizeof(ch1),_T("%s"),pUserData1->GetNickName());
	if(pUserData2 != NULL)
		_sntprintf(ch2,sizeof(ch1),_T("%s"),pUserData2->GetNickName());
	if(pUserData3 != NULL)
		_sntprintf(ch3,sizeof(ch1),_T("%s"),pUserData3->GetNickName());
	if(pUserData4 != NULL)
		_sntprintf(ch4,sizeof(ch1),_T("%s"),pUserData4->GetNickName());
	m_GameClientView.GetUserNameInfo(ch1,ch2,ch3,ch4);
	//播放声音
	PlayGameSound( AfxGetInstanceHandle(), TEXT( "GAME_SHUFFLE" ) ) ;
	m_GameClientView.SetbClockPoint(true);
	//printf("执行了多少次\n");
	SetTimer(50,80,NULL);
	SetTimer(10,20,NULL);


	//设置时间
	SetGameClock(GetMeChairID() , IDI_SHOW_CARD , IDT_SHOW_TIME) ;
	m_GameClientView.RefreshGameView();
	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	
	//InsertSystemString("CGameClientDlg::OnSubGameEnd");
	//效验数据
	m_GameClientView.SetbDrawTimer(true);
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	m_GameClientView.SetStart(false);
	//清理界面
	m_GameClientView.SetCardData(INVALID_CHAIR, NULL, 0, enSegmentCard) ;
	for(int i=0;i<3;i++)
	{
		m_GameClientView.m_SegCardControl[2][i].SetCardData(NULL, 0) ;
	}

	if(m_bFinishSegment)
	{
		m_GameClientView.m_UserCardControl[2].SetCardData(NULL, 0) ;
		m_GameClientView.m_UserCardControl[2].SetDrawCard(false);
	}
	//	else CopyMemory(m_bHandCardData, m_bTmpHandCardData, sizeof(m_bHandCardData)) ;

	//m_GameClientView.m_HandSegCardControl[2].SetCardData(NULL, 0) ;
	//m_GameClientView.m_HandSegCardControl[1].SetCardData(NULL, 0) ;
	//m_GameClientView.m_HandSegCardControl[0].SetCardData(NULL, 0) ;

	//消息处理
	m_bReady = false ; 

	//CString str ;
	//str.Format(TEXT("底注：%ld"), pGameEnd->lUnderScore) ;
	//InsertShtickString(str, RGB(255,0,255), true) ;

	//设置扑克
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		//玩家判断
		IClientUserItem *pClientUserItem = GetTableUserItem(i);
		if(!pClientUserItem) continue ;

		//玩家逃跑
		if(pGameEnd->bUserLeft[i]) continue ;

		WORD wViewID = SwitchViewChairID(i) ;
		if(pGameEnd->bOverTime[i])
		{
			if(wViewID==2)
			{
				m_GameClientView.m_UserCardControl[2].SetCardData(NULL, 0) ;
				m_GameClientView.m_UserCardControl[2].SetDrawCard(false);
			}
			m_GameClientView.m_OverTimeCardControl[wViewID].SetCardData(pGameEnd->cbCardData[i], 13) ;
		}
		else
		{
			if(m_GameClientView.m_AllSegCardControl[wViewID][0].GetCardCount()>0 && m_GameClientView.m_AllSegCardControl[wViewID][1].GetCardCount() &&
				m_GameClientView.m_AllSegCardControl[wViewID][2].GetCardCount()>0)
			{
				m_GameClientView.m_AllSegCardControl[wViewID][0].SetDisplayFlag(true) ;
				m_GameClientView.m_AllSegCardControl[wViewID][1].SetDisplayFlag(true) ;
				m_GameClientView.m_AllSegCardControl[wViewID][2].SetDisplayFlag(true) ;
			}
		}
	}

	//删除定时器
	//KillGameTimer(IDI_SHOW_CARD);
	KillTimer(m_nTimer) ;

	//设置按钮
	//m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	//m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	//m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.m_btOk.EnableWindow(FALSE);
	//m_GameClientView.m_btOnePare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btOnePare.EnableWindow(FALSE);
	//m_GameClientView.m_btTwoPare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
	//m_GameClientView.m_btThreeSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeSame.EnableWindow(FALSE);
	//m_GameClientView.m_btStraight.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraight.EnableWindow(FALSE);
	//m_GameClientView.m_btFlush.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFlush.EnableWindow(FALSE);
	//m_GameClientView.m_btGourd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGourd.EnableWindow(FALSE);
	//m_GameClientView.m_btFourSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFourSame.EnableWindow(FALSE);
	//m_GameClientView.m_btStraightFlush.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard1.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard2.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard3.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
	//m_bSound = false;
	//设置状态
	SetGameStatus(GS_WK_FREE);

	//设置积分
	bool IsLeft=false;
	TCHAR szBuffer[32];
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IClientUserItem *pClientUserItem = GetTableUserItem(i);
		int nChairWin=0,nChairLoss=0;
		m_GameClientView.GetUserWinOrLoss(nChairWin,nChairLoss);
		if(NULL!=pClientUserItem && m_bPlayer[i])
		{
			if(i!=GetMeChairID())
			{
				_sntprintf(szBuffer,sizeof(szBuffer),TEXT("%s"),pClientUserItem->GetNickName());
                m_GameClientView.m_ScoreView.SetGameScore(i,szBuffer,pGameEnd->lGameScore[i],pGameEnd->bToltalWinDaoShu[i]);
			}
			else
			{
                m_GameClientView.m_ScoreView.SetGameScore(i,pClientUserItem->GetNickName(),pGameEnd->lGameScore[i],pGameEnd->bToltalWinDaoShu[i]);
			}
		}
		if(pGameEnd->bUserLeft[i])
		{
			IsLeft=true;
			m_GameClientView.m_ScoreView.SetGameScore(i,pGameEnd->LeftszName[i],pGameEnd->lGameScore[i],pGameEnd->bToltalWinDaoShu[i]);
		}
	}
	if(IsLeft)
	{
		m_ppBuffer=*(CMD_S_GameEnd*)pBuffer;
		m_GameClientView.SetSGameEnd(m_ppBuffer);
	}

	m_GameClientView.GetAllUserScoreInfo(pGameEnd->lGameScore,pGameEnd->lGameEveryTax,pGameEnd->bToltalWinDaoShu,pGameEnd->bUserLeft);
	m_GameClientView.SetbDrawScore(false);
	m_GameClientView.SetEnd(true);
	m_GameClientView.m_ScoreView.SetEnd(true);
	m_GameClientView.m_ScoreView.SetGameTax(pGameEnd->lGameEveryTax);
	//m_GameClientView.m_ScoreView.CenterWindow(&m_GameClientView) ;
	m_GameClientView.m_ScoreView.ShowWindow(SW_SHOW);


	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(TRUE);
		m_GameClientView.m_btStart.EnableWindow(TRUE);
		SetGameClock(GetMeChairID(),IDI_START_GAME,IDT_START_TIME);
	}
	ResetDlgData();
    m_GameClientView.RefreshGameView();
	
	return true;
}

//左键扑克
LRESULT CGameClientDlg::OnLeftHitCard(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	if(false==m_bFinishSegment)
	{
		//获取扑克
		BYTE bCardData[HAND_CARD_COUNT];
		BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);

		//设置按钮
		if(5==bShootCount)
		{
			if((!m_bbThreeCard[1]))
				m_GameClientView.m_btMidCard.EnableWindow(TRUE) ;
			if((!m_bbThreeCard[2]))
				m_GameClientView.m_btBackCard.EnableWindow(TRUE) ;
		}
		else if(3==bShootCount)
		{
			if((!m_bbThreeCard[0]))
				m_GameClientView.m_btFrontCard.EnableWindow(TRUE) ;
		}
		else
		{
			m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
		}
	}
	return 0;
}
//设置前墩
LRESULT CGameClientDlg::OnFrontCard(WPARAM wParam, LPARAM lParam) 
{
	ZeroMemory(&m_bType,sizeof(m_bType));
	m_bbThreeCard[0]=true;
	if(m_bbThreeCard[0]&&m_bbThreeCard[1]&&m_bbThreeCard[2])
	{
		m_bFinishSegment=true ; 
		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);
        m_GameClientView.m_btOk.EnableWindow(TRUE);
	}
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(3==bShootCount) ;
	if(3!=bShootCount) 
	{
		//AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	CopyMemory(m_bSegmentCard[m_nCurrentUser][0],bCardData,3);
	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;


	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	m_GameClientView.m_SegCardControl[2][0].SetCardData(bCardData,bShootCount);
	m_GameClientView.m_SegCardControl[2][0].SetDrawCard(true);
	BYTE CardType=m_GameLogic.GetCardType(bCardData,bShootCount,m_GameLogic.btCardSpecialData);
	m_GameClientView.SetbDrawFrontCardType(true,GetCardType(CardType,true,false));

	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][0].SetDrawCard(false);
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][0].SetCardData(bCardData , bShootCount) ;

	SegCard.SegmentType=enFront ;

	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btRecycleCard1.EnableWindow(TRUE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
		//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(m_bbThreeCard[1])
		{
			//		m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][2].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][2].SetDrawCard(true);
			m_bbThreeCard[2]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawBackCardType(true,GetCardType(CardType,false,false));
			m_GameClientView.m_btRecycleCard3.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][2],m_bHandCardData,5);
			//MessageBox(NULL,"1 3",NULL);
		}
		else if(m_bbThreeCard[2])
		{
			//		m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][1].SetDrawCard(true);
			m_bbThreeCard[1]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawMidCardType(true,GetCardType(CardType,false,true));
			m_GameClientView.m_btRecycleCard2.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][1],m_bHandCardData,5);
			//MessageBox(NULL,"1 2",NULL);
		}
		//else AfxMessageBox("OnFrontCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
		m_bHandCardCount=0;
		m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;

		m_GameClientView.m_btOk.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btOk.EnableWindow(TRUE);
		
		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);
	}

	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	if(m_nCurrentUser==GetMeChairID())
	{
		SetTimer(20,20,NULL);
	}

	m_GameClientView.RefreshGameView();
	return true ;
}
//设置中墩
LRESULT CGameClientDlg::OnMidCard(WPARAM wParam, LPARAM lParam)
{
	ZeroMemory(&m_bType,sizeof(m_bType));
	m_bbThreeCard[1]=true;
	if(m_bbThreeCard[0]&&m_bbThreeCard[1]&&m_bbThreeCard[2])
	{
		m_bFinishSegment=true ; 
		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);
		m_GameClientView.m_btOk.EnableWindow(TRUE);
	}
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		//AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	CopyMemory(m_bSegmentCard[m_nCurrentUser][1],bCardData,5);
	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount ;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;

	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][1].SetDrawCard(false);
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][1].SetCardData(bCardData , bShootCount) ;

	//设置界面
	//m_GameClientView.SetCardData(2 , bCardData , bShootCount , enSegmentCard) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	m_GameClientView.m_SegCardControl[2][1].SetCardData(bCardData,bShootCount);
	m_GameClientView.m_SegCardControl[2][1].SetDrawCard(true);
	BYTE CardType=m_GameLogic.GetCardType(bCardData,bShootCount,m_GameLogic.btCardSpecialData);
	m_GameClientView.SetbDrawMidCardType(true,GetCardType(CardType,false,true));

	SegCard.SegmentType=enMid ;

	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btRecycleCard2.EnableWindow(TRUE);
	//m_GameClientView.m_btRestoreCard.EnableWindow();

	//m_GameClientView.m_btAutoOutCard.ShowWindow(SW_HIDE) ;
	//m_GameClientView.m_btAutoOutCard.EnableWindow(FALSE) ;

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
		//		m_GameClientView.m_AllSegCardControl[2][1].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(m_bbThreeCard[0])
		{
			//		m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][2].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][2].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][2].SetDrawCard(true);
			m_bbThreeCard[2]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawBackCardType(true,GetCardType(CardType,false,false));
			m_GameClientView.m_btRecycleCard3.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][2],m_bHandCardData,5);
			//MessageBox(NULL,"2 3",NULL);
		}
		else if(m_bbThreeCard[2])
		{
			//		m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][0].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][0].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][0].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][0].SetDrawCard(true);
            m_bbThreeCard[0]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawFrontCardType(true,GetCardType(CardType,true,false));
			m_GameClientView.m_btRecycleCard1.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][0],m_bHandCardData,3);
			//MessageBox(NULL,"2 1",NULL);
		}
		//else AfxMessageBox("OnMidCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
		m_bHandCardCount=0;
		m_GameClientView.m_btOk.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btOk.EnableWindow(TRUE);
		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);
	}
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	if(m_nCurrentUser==GetMeChairID())
	{
		SetTimer(20,20,NULL);
	}

	m_GameClientView.RefreshGameView();
	return true ;
}
//玩家摊牌
LRESULT CGameClientDlg::OnShowCard(WPARAM wParam, LPARAM lParam) 
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_ShowCard AllSegmentCard ;
	memset(&AllSegmentCard , 0 , sizeof(AllSegmentCard)) ;

	BYTE bCardCount ;

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][0].GetCardData(AllSegmentCard.bFrontCard , 3)) ;
	ASSERT(bCardCount==3) ;
	if(3!=bCardCount) return false ;
	m_GameLogic.SortCardList(AllSegmentCard.bFrontCard , bCardCount);

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][1].GetCardData(AllSegmentCard.bMidCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	m_GameLogic.SortCardList(AllSegmentCard.bMidCard , bCardCount) ;

	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][2].GetCardData(AllSegmentCard.bBackCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	
	m_GameLogic.SortCardList(AllSegmentCard.bBackCard , bCardCount) ;

	//乌龙判断
	bool bDragon = false ;
	if((true==m_GameLogic.CompareCard(AllSegmentCard.bFrontCard , AllSegmentCard.bMidCard , 3 , 5 , false)) &&
		(true==m_GameLogic.CompareCard(AllSegmentCard.bMidCard, AllSegmentCard.bBackCard , 5 , 5 , false))&&!m_bSpecialType[2])
		bDragon = false ;
	else
		bDragon = true ;

	//乌龙扑克
	/*if(bDragon)
	{
		MessageBox(TEXT("对不起，你摆的是乌龙牌，不可以摊牌，必须：前段<中段<后段"), TEXT("温馨提示")) ;
		return 0 ;
	}*/
    
    if(m_bSpecialType[m_nCurrentUser])
	{
		m_GameClientView.SetbDrawMyScore(false);
	   AllSegmentCard.bSpecialType=true;
	   CopyMemory(AllSegmentCard.btSpecialData,m_bSpecialCardData[m_nCurrentUser],sizeof(AllSegmentCard.btSpecialData));
	}
	else 
	{
	   AllSegmentCard.bDragon=bDragon;
       AllSegmentCard.bSpecialType=false;
	}
	if(false==IsLookonMode())
		SendSocketData(SUB_C_SHOWCARD , &AllSegmentCard , sizeof(AllSegmentCard)) ;

	for(BYTE i=0 ; i<3 ; ++i)
	{
		m_GameClientView.m_UserCardControl[2].SetDrawCard(false);
		m_GameClientView.m_SegCardControl[2][i].SetDrawCard(false);
        m_GameClientView.m_AllSegCardControl[2][i].SetDrawCard(true);
	}
	m_GameClientView.SetbDrawFrontCardType(false,11);
	m_GameClientView.SetbDrawMidCardType(false,11);
	m_GameClientView.SetbDrawBackCardType(false,11);
	m_GameClientView.SetbShowCard(true);
	m_bShowCard=true;
	
	m_GameClientView.HideButton();
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));

	m_GameClientView.RefreshGameView();
	return true ;
}
//设置后墩
LRESULT CGameClientDlg::OnBackCard(WPARAM wParam, LPARAM lParam)
{
	ZeroMemory(&m_bType,sizeof(m_bType));
	m_bbThreeCard[2]=true;
	if(m_bbThreeCard[0]&&m_bbThreeCard[1]&&m_bbThreeCard[2])
	{
		m_bFinishSegment=true ; 
		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);
		m_GameClientView.m_btOk.EnableWindow(TRUE);
	}
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	CMD_C_SegCard  SegCard ;
	memset(&SegCard , 0 , sizeof(SegCard)) ;

	//获取扑克
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);
	//排列扑克
	m_GameLogic.SortCardList(bCardData , bShootCount) ;

	ASSERT(5==bShootCount) ;
	if(5!=bShootCount) 
	{
		//AfxMessageBox("错误扑克,请重新选择!") ;
		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;
		return true ;
	}

	CopyMemory(m_bSegmentCard[m_nCurrentUser][2],bCardData,5);
	//删除扑克
	m_GameLogic.RemoveCard(bCardData , bShootCount , m_bHandCardData , 	m_bHandCardCount) ;
	m_bHandCardCount -= bShootCount;
	//排列扑克
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount);


	//隐藏扑克
	m_GameClientView.m_AllSegCardControl[2][2].SetDrawCard(false);
	//保存扑克
	m_GameClientView.m_AllSegCardControl[2][2].SetCardData(bCardData , bShootCount) ;

	//设置界面
	//m_GameClientView.SetCardData(2 , bCardData , bShootCount , enSegmentCard) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	m_GameClientView.m_SegCardControl[2][2].SetCardData(bCardData,bShootCount);
	m_GameClientView.m_SegCardControl[2][2].SetDrawCard(true);
	BYTE CardType=m_GameLogic.GetCardType(bCardData,bShootCount,m_GameLogic.btCardSpecialData);
	m_GameClientView.SetbDrawBackCardType(true,GetCardType(CardType,false,false));

	SegCard.SegmentType=enBack;

	SendSocketData(SUB_C_SEGCARD , &SegCard , sizeof(SegCard)) ;

	//设置按钮
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btRecycleCard3.EnableWindow(TRUE);

	if(3==m_bHandCardCount || 5==m_bHandCardCount)
	{
		//显示扑克
		//m_GameClientView.m_AllSegCardControl[2][2].ShowWindow(SW_SHOW) ;

		m_bFinishSegment=true ; 

		//设置扑克
		if(m_bbThreeCard[0])
		{
			//		m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_SHOW) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][1].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][1].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][1].SetDrawCard(true);
			m_bbThreeCard[1]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawMidCardType(true,GetCardType(CardType,false,true));
			m_GameClientView.m_btRecycleCard2.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][1],m_bHandCardData,5);
			//MessageBox(NULL,"3 2",NULL);
		}
		else if(m_bbThreeCard[1])
		{
			m_GameClientView.m_AllSegCardControl[2][0].SetCardData(m_bHandCardData , m_bHandCardCount) ;
			m_GameClientView.m_AllSegCardControl[2][0].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[2][0].SetCardData(m_bHandCardData,m_bHandCardCount);
			m_GameClientView.m_SegCardControl[2][0].SetDrawCard(true);
            m_bbThreeCard[0]=true;
			BYTE CardType=m_GameLogic.GetCardType(m_bHandCardData,m_bHandCardCount,m_GameLogic.btCardSpecialData);
			m_GameClientView.SetbDrawFrontCardType(true,GetCardType(CardType,true,false));
			m_GameClientView.m_btRecycleCard1.EnableWindow(TRUE);
			CopyMemory(m_bSegmentCard[m_nCurrentUser][0],m_bHandCardData,3);

			//MessageBox(NULL,"3 1",NULL);
		}
		//else AfxMessageBox("OnBackCard：错误扑克！") ;

		//设置扑克
		m_GameClientView.SetCardData(2 , NULL , 0 , enHandCard) ;
		ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
		m_bHandCardCount=0;

		m_GameClientView.m_btOk.ShowWindow(SW_SHOW) ;
		m_GameClientView.m_btOk.EnableWindow(TRUE);

		m_GameClientView.m_btRecycleAllCard.EnableWindow(TRUE);

		//分段信息
		//ShowSegInfo() ;
	}
	//播放声音
	PlayGameSound(AfxGetInstanceHandle(),TEXT("OUT_CARD"));
	if(m_nCurrentUser==GetMeChairID())
	{
		SetTimer(20,20,NULL);
	}

	m_GameClientView.RefreshGameView();
	return true ;
}

//设置分段
bool CGameClientDlg::OnSubSetSegment(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_SetSegment)==wDataSize) ;
	if(sizeof(CMD_S_SetSegment)!=wDataSize) return false ;

	CMD_S_SetSegment *pSetSegment = (CMD_S_SetSegment*)pBuffer ;

	//其他玩家只是设置背面
// 	if(GetMeChairID()!=pSetSegment->wCurrentUser)
// 	{
// 		BYTE bCardData[5] ;
// 		memset(bCardData , 0 , sizeof(bCardData)) ;
// 		BYTE bCardCount = pSetSegment->SegmentType==enFront ? 3 : 5 ;
// 		m_GameClientView.SetCardData(SwitchViewChairID(pSetSegment->wCurrentUser) , bCardData , bCardCount , enSegmentCard) ;
// 	}
	return true ;
}

//玩家摊牌
bool CGameClientDlg::OnSubShowCard(const void * pBuffer , WORD wDataSize)
{
	//数据验证
	ASSERT(sizeof(CMD_S_ShowCard)==wDataSize) ;
	if(sizeof(CMD_S_ShowCard)!=wDataSize) return false;

	CMD_S_ShowCard *pShowCard = (CMD_S_ShowCard*)pBuffer ;

	WORD wChiarID = SwitchViewChairID(pShowCard->wShowUser) ;
	if(pShowCard->bSpecialType)
	{
		CopyMemory(m_bSpecialCardData[pShowCard->wShowUser],pShowCard->btSpecialData,sizeof(m_bSpecialCardData[pShowCard->wShowUser]));
	}
    m_bDragon[pShowCard->wShowUser]=pShowCard->bDragon;
	//WORD wViewChairID=SwitchViewChairID(pShowCard->wShowUser);
	//m_GameClientView.SetDragon(wViewChairID,pShowCard->bDragon);
	m_GameClientView.SetCardData(wChiarID , NULL , 0 , enHandCard) ;
	m_GameClientView.SetCardData(wChiarID , NULL , 0 , enSegmentCard) ;

	BYTE nCount = rand()%3;
	IClientUserItem* pUserData = GetTableUserItem(pShowCard->wShowUser);
	if(pUserData!=NULL && m_GameClientView.m_bOnOffThanCard[wChiarID]==false)
	{
		if(pUserData->GetGender()==1)
		{
			if(nCount==0)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD1"));
			else if(nCount==1)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD2"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD3"));
		}
		else
		{
			if(nCount==0)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD4"));
			else if(nCount==1)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD5"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("COMPARE_CARD6"));
		}
	}

	m_GameClientView.m_bOnOffThanCard[wChiarID] = true;   ///<是否比牌显示了
	if(1)//(pShowCard->bCanSeeShowCard)
	{
		//设置扑克
		m_GameClientView.SetCardData(wChiarID , pShowCard->bFrontCard , 3 , enAllSegmentCard , 0) ;
		m_GameClientView.SetCardData(wChiarID , pShowCard->bMidCard , 3 , enAllSegmentCard , 1) ;
		m_GameClientView.SetCardData(wChiarID , pShowCard->bBackCard , 3 , enAllSegmentCard , 2) ;
        m_GameClientView.m_AllSegCardControl[wChiarID][0].SetDrawCard(true);
        m_GameClientView.m_AllSegCardControl[wChiarID][1].SetDrawCard(true);
		m_GameClientView.m_AllSegCardControl[wChiarID][2].SetDrawCard(true);
		if(!IsLookonMode() && GetMeChairID()==pShowCard->wShowUser)
		{
			m_GameClientView.m_AllSegCardControl[wChiarID][0].SetDisplayFlag(true) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][1].SetDisplayFlag(true) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][2].SetDisplayFlag(true) ;
		}
		else 
		{
			m_GameClientView.m_AllSegCardControl[wChiarID][0].SetDisplayFlag(false) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][1].SetDisplayFlag(false) ;
			m_GameClientView.m_AllSegCardControl[wChiarID][2].SetDisplayFlag(false) ;
		}
		//保存数据
		CopyMemory(m_bSegmentCard[pShowCard->wShowUser][0] , pShowCard->bFrontCard , 3) ;
		CopyMemory(m_bSegmentCard[pShowCard->wShowUser][1] , pShowCard->bMidCard , 3) ;
		CopyMemory(m_bSegmentCard[pShowCard->wShowUser][2] , pShowCard->bBackCard , 3) ;
	}

    m_GameClientView.RefreshGameView();
	return true ;
}


/*
*	算法描述：随机选出正确的十种分段（不摆乌龙），然后计算每种分段的权重，最后选权重最大者
*/
//自动代打
void CGameClientDlg::AutomaticOutCard() 
{
	if(m_bSpecialType[m_nCurrentUser])
	{
		GetSpecialCard(m_bHandCardData,13);
		BYTE cbCardData[5];
		BYTE cbCardCount=0;
		for(int i=0;i<3;i++)
		{
			cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][10+i];
		}
		cbCardCount=3;
		//保存扑克
		m_GameClientView.m_AllSegCardControl[2][0].SetCardData(cbCardData , cbCardCount) ;
		//隐藏扑克
		m_GameClientView.m_AllSegCardControl[2][0].SetDrawCard(false);
		ZeroMemory(cbCardData,sizeof(cbCardData));

		for(int i=0;i<5;i++)
		{
			cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][5+i];
		}
		cbCardCount=5;
		//保存扑克
		m_GameClientView.m_AllSegCardControl[2][1].SetCardData(cbCardData , cbCardCount) ;
		//隐藏扑克
		m_GameClientView.m_AllSegCardControl[2][1].SetDrawCard(false);
		ZeroMemory(cbCardData,sizeof(cbCardData));

		for(int i=0;i<5;i++)
		{
			cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][i];
		}
		cbCardCount=5;
		//保存扑克
		m_GameClientView.m_AllSegCardControl[2][2].SetCardData(cbCardData , cbCardCount) ;
		//隐藏扑克
		m_GameClientView.m_AllSegCardControl[2][2].SetDrawCard(false);
		ZeroMemory(cbCardData,sizeof(cbCardData));
		//m_GameClientView.m_btOk.ShowWindow(SW_HIDE) ;
		//m_GameClientView.m_btOk.EnableWindow(TRUE);
		//CopyMemory(m_bHandCardData , m_bTmpHandCardData , sizeof(m_bHandCardData)) ;
		//m_GameLogic.SortCardList(m_bHandCardData,13);
	}
	else
	{
		//恢复扑克
		CopyMemory(m_bHandCardData , m_bTmpHandCardData , sizeof(m_bHandCardData)) ; //防止玩家分了一段或两段后超时调用此函数
		m_bHandCardCount = sizeof(m_bHandCardData) ;
		m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData , m_bHandCardCount) ;
        m_GameClientView.m_UserCardControl[2].SetDrawCard(true);
		BYTE bCardCount = (BYTE)(m_GameClientView.m_UserCardControl[2].GetCardCount()) ;

		m_GameClientView.m_UserCardControl[2].ShootAllCard(false) ;

		const BYTE SEARCHE_COUNT = 10 ;
		ASSERT(bCardCount==13) ;
		if(bCardCount!=13) return ;
		BYTE bAllSegmentCardIndex[SEARCHE_COUNT][3][5] ;			//分段扑克

		BYTE bSegmentScore[SEARCHE_COUNT] ;							//分段权重
		BYTE bCardDataIndex[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12} ;	//扑克下标
		BYTE bFrontCardType ,										//前墩类型
			bMidCardType,											//中墩类型
			bBackCardType;											//后墩类型
		BYTE bCardData[5] ;
		ZeroMemory(bAllSegmentCardIndex , sizeof(bAllSegmentCardIndex)) ;
		ZeroMemory(bSegmentScore , sizeof(bSegmentScore)) ;

		srand((unsigned)time( NULL ));

		int  bSegCount=0 ;
		LONG bSearchCount = 0 ;
		bool bStop = false ;
		while(false==bStop)
		{
			++bSearchCount ;
			BYTE bCardIndex=0 ;										//扑克下标
			//前墩扑克


			for(BYTE bFrontCard=0 ; bFrontCard<3 ; ++bFrontCard)
			{
				bCardIndex = rand()%(13-bFrontCard) ;
				bAllSegmentCardIndex[bSegCount][0][bFrontCard] = bCardDataIndex[bCardIndex] ;
				bCardData[bFrontCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
				bCardDataIndex[bCardIndex] = bCardDataIndex[13-bFrontCard-1] ;
			}

			m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;
			bFrontCardType = m_GameLogic.GetCardType(bCardData , 3,m_GameLogic.btCardSpecialData) ;

			//中墩扑克
			for(BYTE bMidCard=0 ; bMidCard<5 ; ++bMidCard)
			{
				bCardIndex = rand()%(10-bMidCard) ;
				bAllSegmentCardIndex[bSegCount][1][bMidCard] = bCardDataIndex[bCardIndex] ;
				bCardData[bMidCard] = m_bHandCardData[bCardDataIndex[bCardIndex]] ;
				bCardDataIndex[bCardIndex] = bCardDataIndex[10-bMidCard-1] ;
			}

			m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;
			bMidCardType = m_GameLogic.GetCardType(bCardData , 5,m_GameLogic.btCardSpecialData) ;

			//后墩扑克
			for(BYTE bBackCard=0 ; bBackCard<5 ; ++bBackCard)
			{
				bAllSegmentCardIndex[bSegCount][2][bBackCard] = bCardDataIndex[bBackCard] ;
				bCardData[bBackCard] = m_bHandCardData[bCardDataIndex[bBackCard]] ;
			}
			m_GameLogic.SortCardList(bCardData , 5 , enDescend) ;


			bBackCardType = m_GameLogic.GetCardType(bCardData , 5,m_GameLogic.btCardSpecialData) ;


			if(bBackCardType>bMidCardType && bMidCardType>bFrontCardType) 
			{
				bSegmentScore[bSegCount] = bFrontCardType+bMidCardType+bBackCardType ;
				bSegCount++;				
			}

			//恢复数据
			for(BYTE i=0 ; i<13 ; ++i)
				bCardDataIndex[i] = i ;

			//停止搜索
			if(bSegCount>=SEARCHE_COUNT || bSearchCount>=10000) bStop = true ;
			//搜到一个
			if(true==bStop && 0==bSegCount) bStop = false ; 

			//搜索不到
			if(bSearchCount>=100000)
			{
				BYTE bIndex=0 ; 
				for(BYTE i=0 ; i<3 ; ++i)
					for(BYTE j=0 ; j<5 ; ++j)
						bAllSegmentCardIndex[0][i][j]=bIndex++ ;
				bStop = true ;
				break ;
			}
		}

		//最大权重
		BYTE bMaxScore=bSegmentScore[0] ;
		BYTE bIndex=0 ;
		for(BYTE i=0 ; i<bSegCount ; ++i)
			if(bMaxScore<bSegmentScore[i])
			{
				bIndex=i ;
				bMaxScore=bSegmentScore[i] ;
			}


			//保留扑克
			BYTE bHandCardData[13] ;
			CopyMemory(bHandCardData , m_bHandCardData , 13) ;
			//设置前墩
			m_GameClientView.m_UserCardControl[2].SetShootCard(bAllSegmentCardIndex[bIndex][0] , 3) ;
			OnFrontCard(0 , 0) ;

			BYTE bNewIndex[5] ;
			for(BYTE i=0 ; i<5 ; ++i)
			{
				for(BYTE j=0 ; j<10 ; ++j)
					if(m_bHandCardData[j]==bHandCardData[bAllSegmentCardIndex[bIndex][1][i]])
					{
						bNewIndex[i] = j ;
						break ;
					}
			}
			//设置中墩
			m_GameClientView.m_UserCardControl[2].SetShootCard(bNewIndex , 5) ;
			OnMidCard(0 , 0) ;

			//摊牌
			//	OnShowCard(0 , 0) ;
	}
	
}

//降序排列
LRESULT	CGameClientDlg::OnDescendSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enDescend) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.RefreshGameView();
	return true ;
}
//升序排列
LRESULT	CGameClientDlg::OnAscendSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enAscend) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;

	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.RefreshGameView();
	return true ;
}
//花色排列
LRESULT	CGameClientDlg::OnColorSort(WPARAM wParam, LPARAM lParam) 
{
	if(m_bHandCardCount<1) return false ;

	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount , enColor) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	//按钮设置
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

	m_GameClientView.RefreshGameView();
	return true ;
}

//自动代打
LRESULT CGameClientDlg::OnAutoOutCard(WPARAM wParam, LPARAM lParam)
{
	//无用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//设置按钮
	m_GameClientView.m_btFrontCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btMidCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
	m_GameClientView.m_btBackCard.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
	//m_GameClientView.m_btRestoreCard.ShowWindow(SW_SHOW) ;
	//	m_GameClientView.m_btRestoreCard.EnableWindow(FALSE);
	AutomaticOutCard() ;

	return true ;
}

//分段信息
bool CGameClientDlg::ShowSegInfo() 
{
	if(!m_bFinishSegment) return true ;

	//分段扑克
	BYTE bCardCount ;
	BYTE cbFrontCard[3], cbMinCard[5], cbBackCard[5] ;

	//前段扑克
	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][0].GetCardData(cbFrontCard , 3)) ;
	ASSERT(bCardCount==3) ;
	if(3!=bCardCount) return false ;
	m_GameLogic.SortCardList(cbFrontCard , bCardCount) ;

	//中段扑克
	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][1].GetCardData(cbMinCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	m_GameLogic.SortCardList(cbMinCard , bCardCount) ;

	//尾段扑克
	bCardCount = (BYTE)(m_GameClientView.m_AllSegCardControl[2][2].GetCardData(cbBackCard , 5)) ;
	ASSERT(bCardCount==5) ;
	if(5!=bCardCount) return false ;
	m_GameLogic.SortCardList(cbBackCard , bCardCount) ;

	//底牌信息
	bool bUnderInfo = false ;
	//m_GameClientView.ShowSegInfo(enAllSeg, TEXT("底牌：0 水")) ;

	//同色牌型
	bool bSameColor = true ;
	BYTE cbFirstCardColor = m_GameLogic.GetCardColor(m_bTmpHandCardData[0]) ;
	BYTE cbRedCont=0, cbBlackCount=0 ;
	if(0x00==cbFirstCardColor || 0x20==cbFirstCardColor) cbRedCont=1 ;
	else cbBlackCount=1 ;
	for(BYTE i=1; i<13; ++i)
	{
		if(0x00==cbFirstCardColor || 0x20==cbFirstCardColor)
		{
			if(m_GameLogic.GetCardColor(m_bTmpHandCardData[i])!=0x00 && m_GameLogic.GetCardColor(m_bTmpHandCardData[i])!=0x20)
			{
				bSameColor = false ;
				continue ;
			}

			//红色扑克
			++cbRedCont ;
		}
		else
		{
			if(m_GameLogic.GetCardColor(m_bTmpHandCardData[i])!=0x10 && m_GameLogic.GetCardColor(m_bTmpHandCardData[i])!=0x30)
			{
				bSameColor = false ;
				continue ;
			}

			//黑色扑克
			++cbBlackCount ;
		}
	}
	if(bSameColor)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌同花：26 水") ;
			bUnderInfo = true ;
		}
	}

	//十三单牌
	BYTE cbTmpCardData[13] ;
	CopyMemory(cbTmpCardData, m_bTmpHandCardData, 13) ;
	m_GameLogic.SortCardList(cbTmpCardData, 13) ;
	BYTE j = 0;
	for(j=0; j<12; ++j)
	{
		if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[j])!=(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[j+1])+1))
			break ;
	}
	if(j==12)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌十三单：13 水") ;
			bUnderInfo = true ;
		}
	}

	//全红一点黑
	if(12==cbRedCont && 1==cbBlackCount)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌全红一点黑：13 水") ;
			bUnderInfo = true ;
		}
	}
	//全黑一点红
	if(12==cbBlackCount && 1==cbRedCont)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌全黑一点红：13 水") ;
			bUnderInfo = true ;
		}
	}

	//J~A的牌
	BYTE i = 0;
	for(i=1; i<13; ++i) if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])>14 || m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])<11) break ;
	if(13==i)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌J至A的牌：10 水") ;
			bUnderInfo = true ;
		}
	}

	//10~A的牌
	for(BYTE i=1; i<13; ++i) if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])>14 || m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])<10) break ;
	if(13==i)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌10至A的牌：11 水") ;
			bUnderInfo = true ;
		}
	}

	//9~A的牌
	for(BYTE i=1; i<13; ++i) if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])>14 || m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])<9) break ;
	if(13==i)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌9至A的牌：9 水") ;
			bUnderInfo = true ;
		}
	}

	//8~A的牌
	for(BYTE i=1; i<13; ++i) if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])>14 || m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])<8) break ;
	if(13==i)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌8至A的牌：8 水") ;
			bUnderInfo = true ;
		}
	}

	//7~A的牌
	for(BYTE i=1; i<13; ++i) if(m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])>14 || m_GameLogic.GetCardLogicValue(m_bTmpHandCardData[i])<7) break ;
	if(13==i)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌7至A的牌：7 水") ;
			bUnderInfo = true ;
		}	
	}

	//有5对和一个三个
	tagAnalyseResult AnalyseData ;
	memset(&AnalyseData , 0 , sizeof(AnalyseData)) ;
	m_GameLogic.AnalysebCardData(m_bTmpHandCardData, 13 , AnalyseData) ;
	if(5==AnalyseData.cbDoubleCount && 1==AnalyseData.cbThreeCount)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌5对加一个3条：7 水") ;
			bUnderInfo = true ;
		}		
	}

	//全小扑克
	bool bSmallCard=true ;
	for(BYTE i=0; i<13; ++i) if(m_GameLogic.GetCardValue(m_bTmpHandCardData[i])>10 || m_GameLogic.GetCardValue(m_bTmpHandCardData[i])==1) { bSmallCard=false; break ;}
	if(bSmallCard)
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌全小扑克：6 水") ;
			bUnderInfo = true ;
		}	
	}

	//3 个同花
	if(m_GameLogic.GetCardColor(cbFrontCard[0])==m_GameLogic.GetCardColor(cbFrontCard[1]) &&
		m_GameLogic.GetCardColor(cbFrontCard[0])==m_GameLogic.GetCardColor(cbFrontCard[2]) &&
		CT_FIVE_FLUSH==m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData) &&
		CT_FIVE_FLUSH==m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData))
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌3 个同花：4 水") ;
			bUnderInfo = true ;
		}	
	}

	//A至10
	bool bHalfSmallCard=true ;
	for(BYTE i=0; i<13; ++i) if(m_GameLogic.GetCardValue(m_bTmpHandCardData[i])>10) { bSmallCard=false; break ;}
	if(bSmallCard)  
	{
		if(!bUnderInfo) 
		{
			m_GameClientView.ShowSegInfo(enAllSeg, "底牌A至10：3 水") ;
			bUnderInfo = true ;
		}	
	}

	//3个顺子
	bool bFrontLine = false ;
	if(m_GameLogic.GetCardLogicValue(cbFrontCard[0])==m_GameLogic.GetCardLogicValue(cbFrontCard[1]+1) &&
		m_GameLogic.GetCardLogicValue(cbFrontCard[0])==m_GameLogic.GetCardLogicValue(cbFrontCard[2]+2)) bFrontLine = true ;
	if(14==m_GameLogic.GetCardLogicValue(cbFrontCard[0]) &&
		2==m_GameLogic.GetCardLogicValue(cbFrontCard[1])	&&
		3==m_GameLogic.GetCardLogicValue(cbFrontCard[2])) bFrontLine = true ;
	BYTE cbMinCardType = m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData) ;
	BYTE cbBackCardType = m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData) ;
// 
// 	if(bFrontLine && (cbMinCardType>=CT_FIVE_MIXED_FLUSH_NO_A&& cbMinCardType<=CT_FIVE_MIXED_FLUSH_BACK_A || cbMinCardType>=CT_FIVE_STRAIGHT_FLUSH_FIRST_A&&cbMinCardType<=CT_FIVE_STRAIGHT_FLUSH) &&
// 		(cbBackCardType>=CT_FIVE_MIXED_FLUSH_NO_A&& cbBackCardType<=CT_FIVE_MIXED_FLUSH_BACK_A || cbBackCardType>=CT_FIVE_STRAIGHT_FLUSH_FIRST_A&&cbBackCardType<=CT_FIVE_STRAIGHT_FLUSH))
// 	{
// 		if(!bUnderInfo) 
// 		{
// 			m_GameClientView.ShowSegInfo(enAllSeg, TEXT("底牌3个顺子：3 水"));
// 			bUnderInfo = true ;
// 		}	
// 	}

	//设置信息
	m_GameClientView.ShowSegInfo(enFront, "前段：0 水") ;
	m_GameClientView.ShowSegInfo(enMid, "中段：0 水") ;
	m_GameClientView.ShowSegInfo(enBack, "后段：0 水") ;

	//后墩同花
	if(CT_FIVE_FLUSH==m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack,"后段同花：5 水") ;
	}
	//四带一牌
	else if(CT_FIVE_FOUR_ONE==m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack, "后段四带一牌：4 水") ;

	}
	//3+2牌
	else if(CT_FIVE_THREE_DEOUBLE==m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack, "后段三带二牌：4 水") ;
	}

	//中墩同花
	if(CT_FIVE_FLUSH==m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack, "中段同花：10 水") ;
	}

	//四带一牌
	if(CT_FIVE_FOUR_ONE==m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack, "中段四带一牌：8 水") ;

	}

	//3+2牌
	else if(CT_FIVE_THREE_DEOUBLE==m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enBack, "中段三带二牌：2 水") ;
	}

	//所有散牌
	if(CT_SINGLE==m_GameLogic.GetCardType(cbFrontCard, 3,m_GameLogic.btCardSpecialData) && CT_SINGLE==m_GameLogic.GetCardType(cbMinCard, 5,m_GameLogic.btCardSpecialData) &&
		CT_SINGLE==m_GameLogic.GetCardType(cbBackCard, 5,m_GameLogic.btCardSpecialData))
	{
		m_GameClientView.ShowSegInfo(enFront, "前段散牌") ;
		m_GameClientView.ShowSegInfo(enMid, "中段散牌") ;
		m_GameClientView.ShowSegInfo(enBack, "后段散牌") ;
	}

	return true ;
}
//重排扑克
LRESULT CGameClientDlg::OnRestoreCard(WPARAM wParam, LPARAM lParam) 
{
	//不用参数
	UNREFERENCED_PARAMETER(wParam) ;
	UNREFERENCED_PARAMETER(lParam) ;

	//恢复数据
	CopyMemory(m_bHandCardData, m_bTmpHandCardData, sizeof(m_bHandCardData));
	m_bHandCardCount = 13 ;

	//设置按钮
	//m_GameClientView.m_btColor.ShowWindow(SW_SHOW) ;
	//m_GameClientView.m_btColor.EnableWindow(TRUE);

	//m_GameClientView.m_btAscend.ShowWindow(SW_SHOW) ;
	//	m_GameClientView.m_btAscend.EnableWindow(TRUE);

	//m_GameClientView.m_btDescend.ShowWindow(SW_SHOW) ;
	//m_GameClientView.m_btDescend.EnableWindow(TRUE);

	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;

	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;

	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;

	//m_GameClientView.m_btAutoOutCard.ShowWindow(SW_SHOW) ;
	//m_GameClientView.m_btAutoOutCard.EnableWindow(TRUE);

	//m_GameClientView.m_btOk.ShowWindow(SW_HIDE) ;
	m_GameClientView.m_btOk.EnableWindow(FALSE);

	//m_GameClientView.m_btRestoreCard.ShowWindow(SW_HIDE);
	//	m_GameClientView.m_btRestoreCard.EnableWindow(FALSE);
	//设置扑克
	m_GameClientView.m_UserCardControl[2].SetCardData(m_bHandCardData, 13) ;
	m_GameClientView.m_UserCardControl[2].SetDrawCard(true);
	m_GameClientView.m_AllSegCardControl[2][0].SetCardData(NULL, 0) ;
	m_GameClientView.m_AllSegCardControl[2][1].SetCardData(NULL, 0) ;
	m_GameClientView.m_AllSegCardControl[2][2].SetCardData(NULL, 0) ;

	//分段扑克
	m_GameClientView.SetCardData(2, NULL, 0, enSegmentCard) ;
	//m_GameClientView.m_HandSegCardControl[2].SetCardData(NULL, 0) ;
	//m_GameClientView.m_HandSegCardControl[1].SetCardData(NULL, 0) ;
	//m_GameClientView.m_HandSegCardControl[0].SetCardData(NULL, 0) ;

	//设置变量
	m_bFinishSegment = false ;

	//道数信息
	m_GameClientView.ShowSegInfo(enFront, "前段：0 水") ;
	m_GameClientView.ShowSegInfo(enMid, "中段：0 水") ;
	m_GameClientView.ShowSegInfo(enBack, "后段：0 水") ;
	m_GameClientView.ShowSegInfo(enAllSeg, "底牌：0 水") ;

	return 0 ;
}
// void CGameClientDlg::RectifyVideo(bool resize,int ReSizeL,int Serial,bool ower)
// {
// 	/*  CRect rc;
// 	::GetClientRect(AfxGetMainWnd()->m_hWnd,&rc);
// 	CPoint postion[GAME_PLAYER];
// 	UINT  width=rc.Width();
// 	if (resize)
// 	{
// 	width=ReSizeL;
// 	}
// 
// 	if (GetMeChairID()==INVALID_CHAIR)
// 	return;
// 	int nTemp;
// 	if (GetMeChairID()==0||GetMeChairID()==2)
// 	nTemp=2;
// 	else
// 	nTemp=0;
// 	int nNewChainID=(GetMeChairID()+Serial+nTemp)%GAME_PLAYER;
// 	CRect rcClient=m_GameVideo.GetWinRect(Serial);
// 	postion[0].x=rc.left+width-110;
// 	postion[0].y=rc.top+236;
// 	postion[1].x=rc.left+9;
// 	postion[1].y=rc.top+rc.Height()/2+36;
// 	postion[2].x=rc.left+9;
// 	postion[2].y=rc.top+rc.Height()-11;
// 	postion[3].x=rc.left+width+236;
// 	postion[3].y=rc.top+rc.Height()/2+126;
// 	m_GameVideo.SetWindowPos(Serial,postion[nNewChainID].x,postion[nNewChainID].y-rc.Height(),156,174);
// 	m_GameVideo.ShowWindow(TRUE,Serial);
// 	return;*/
// 	CRect rc;
// 	::GetClientRect(AfxGetMainWnd()->m_hWnd,&rc);
// 	CPoint postion[GAME_PLAYER];
// 	UINT width=rc.Width();
// 	if(resize)
// 	{
// 		width = ReSizeL;
// 	}
// 
// 	if(GetMeChairID()==INVALID_CHAIR)
// 		return;
// 
// 	int nTemp;
// 	if(GetMeChairID()==0||GetMeChairID()==2)
// 		nTemp=2;
// 	else
// 		nTemp=0;
// 	int nNewChairID=(GetMeChairID()+Serial+nTemp)%GAME_PLAYER;
// 
// 	CRect rcClient=m_GameVideo.GetWinRect(Serial);
// 
// 	postion[0].x = rc.left+width-90-150;
// 	postion[0].y = rc.top+175;
// 	postion[1].x = rc.left+6;
// 	postion[1].y = rc.top+rc.Height()/2-36;
// 	postion[2].x = rc.left+9;
// 	postion[2].y = rc.top+rc.Height()-6-5;
// 	postion[3].x = rc.left+width-90-15+2;
// 	postion[3].y = rc.top+rc.Height()/2-36;
// 
// 	m_GameVideo.SetWindowPos( Serial, postion[nNewChairID].x,postion[nNewChairID].y-rcClient.Height(),156,174);
// 	m_GameVideo.ShowWindow(TRUE,Serial);
// 
// 	return;
// 
// 
// 
// 
// }
//退出
LRESULT CGameClientDlg::OnEnd( WPARAM wParam, LPARAM lParam )
{
	PostMessage(WM_CLOSE);
	return 0;
}
//一对的操作
LRESULT CGameClientDlg::OnOnePareCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[2];
	BYTE CardCount=2;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbOnePare[i]==FALSE)
		{
			if(m_bType.cbOnePare[i*2]==0&&m_bType.cbOnePare[i*2+1]==0)
			{
				ZeroMemory(m_bType.bbOnePare,sizeof(m_bType.bbOnePare));
				i=0;
			}
			ShootIndex[0]=m_bType.cbOnePare[i*2];
			ShootIndex[1]=m_bType.cbOnePare[i*2+1];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbOnePare[i]=TRUE;
		
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}
	}
	m_GameClientView.RefreshGameView();
	return true;
}
//二对的操作
LRESULT CGameClientDlg::OnTwoPareCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[4];
	BYTE CardCount=4;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbTwoPare[i]==FALSE)
		{
			if(m_bType.cbTwoPare[i*4]==0&&m_bType.cbTwoPare[i*4+1]==0)
			{
				ZeroMemory(m_bType.bbTwoPare,sizeof(m_bType.bbTwoPare));
				i=0;
			}
			ShootIndex[0]=m_bType.cbTwoPare[i*4];
			ShootIndex[1]=m_bType.cbTwoPare[i*4+1];
			ShootIndex[2]=m_bType.cbTwoPare[i*4+2];
			ShootIndex[3]=m_bType.cbTwoPare[i*4+3];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbTwoPare[i]=TRUE;
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}


	}
	m_GameClientView.RefreshGameView();
	return true;
}
//三条的操作
LRESULT CGameClientDlg::OnThreeSameCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[3];
	BYTE CardCount=3;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbThreeSame[i]==FALSE)
		{
			if(m_bType.cbThreeSame[i*3]==0&&m_bType.cbThreeSame[i*3+1]==0)
			{
				ZeroMemory(m_bType.bbThreeSame,sizeof(m_bType.bbThreeSame));
				i=0;
			}
			ShootIndex[0]=m_bType.cbThreeSame[i*3];
			ShootIndex[1]=m_bType.cbThreeSame[i*3+1];
			ShootIndex[2]=m_bType.cbThreeSame[i*3+2];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbThreeSame[i]=TRUE;
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}

	}
	m_GameClientView.RefreshGameView();
	return true;
}
//顺子的操作
LRESULT CGameClientDlg::OnStraightCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[5];
	BYTE CardCount=5;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbStraight[i]==FALSE)
		{
			if(m_bType.cbStraight[i*5]==0&&m_bType.cbStraight[i*5+1]==0)
			{
				ZeroMemory(m_bType.bbStraight,sizeof(m_bType.bbStraight));
				i=0;
			}
			ShootIndex[0]=m_bType.cbStraight[i*5];
			ShootIndex[1]=m_bType.cbStraight[i*5+1];
			ShootIndex[2]=m_bType.cbStraight[i*5+2];
			ShootIndex[3]=m_bType.cbStraight[i*5+3];
			ShootIndex[4]=m_bType.cbStraight[i*5+4];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbStraight[i]=TRUE;
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}

	}
	SetThreeCard();
	m_GameClientView.RefreshGameView();
	return true;
}
//同花的操作
LRESULT CGameClientDlg::OnFlushCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[5];
	BYTE CardCount=5;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbFlush[i]==FALSE)
		{
			if(m_bType.cbFlush[i*5]==0&&m_bType.cbFlush[i*5+1]==0)
			{
				ZeroMemory(m_bType.bbFlush,sizeof(m_bType.bbFlush));
				i=0;
			}
			ShootIndex[0]=m_bType.cbFlush[i*5];
			ShootIndex[1]=m_bType.cbFlush[i*5+1];
			ShootIndex[2]=m_bType.cbFlush[i*5+2];
			ShootIndex[3]=m_bType.cbFlush[i*5+3];
			ShootIndex[4]=m_bType.cbFlush[i*5+4];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbFlush[i]=TRUE;
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}

	}
	SetThreeCard();
	m_GameClientView.RefreshGameView();
	return true;
}
//葫芦的操作
LRESULT CGameClientDlg::OnGourdCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[5];
	BYTE CardCount=5;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbGourd[i]==FALSE)
		{
			if(m_bType.cbGourd[i*5]==0&&m_bType.cbGourd[i*5+1]==0)
			{
				ZeroMemory(m_bType.bbGourd,sizeof(m_bType.bbGourd));
				i=0;
			}
			ShootIndex[0]=m_bType.cbGourd[i*5];
			ShootIndex[1]=m_bType.cbGourd[i*5+1];
			ShootIndex[2]=m_bType.cbGourd[i*5+2];
			ShootIndex[3]=m_bType.cbGourd[i*5+3];
			ShootIndex[4]=m_bType.cbGourd[i*5+4];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbGourd[i]=TRUE;
			break;
		}

	}
	SetThreeCard();
	m_GameClientView.RefreshGameView();
	return true;
}
//铁支的操作
LRESULT CGameClientDlg::OnFourSameCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[4];
	BYTE CardCount=4;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbFourSame[i]==FALSE)
		{
			if(m_bType.cbFourSame[i*4]==0&&m_bType.cbFourSame[i*4+1]==0)
			{
				ZeroMemory(m_bType.bbFourSame,sizeof(m_bType.bbFourSame));
				i=0;
			}
			ShootIndex[0]=m_bType.cbFourSame[i*4];
			ShootIndex[1]=m_bType.cbFourSame[i*4+1];
			ShootIndex[2]=m_bType.cbFourSame[i*4+2];
			ShootIndex[3]=m_bType.cbFourSame[i*4+3];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbFourSame[i]=TRUE;
			
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}
	}
	m_GameClientView.RefreshGameView();
	return true;
}
//同花顺的操作
LRESULT CGameClientDlg::OnStraightFlushCard(WPARAM wParam, LPARAM lParam )
{
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_UserCardControl[2].SetRegainCard(m_bHandCardCount);
	BYTE ShootIndex[5];
	BYTE CardCount=5;
	for(BYTE i=0;i<20;i++)
	{
		if(m_bType.bbStraightFlush[i]==FALSE)
		{
			if(m_bType.cbStraightFlush[i*5]==0&&m_bType.cbStraightFlush[i*5+1]==0)
			{
				ZeroMemory(m_bType.bbStraightFlush,sizeof(m_bType.bbStraightFlush));
				i=0;
			}
			ShootIndex[0]=m_bType.cbStraightFlush[i*5];
			ShootIndex[1]=m_bType.cbStraightFlush[i*5+1];
			ShootIndex[2]=m_bType.cbStraightFlush[i*5+2];
			ShootIndex[3]=m_bType.cbStraightFlush[i*5+3];
			ShootIndex[4]=m_bType.cbStraightFlush[i*5+4];
			m_GameClientView.m_UserCardControl[2].SetShootCard(ShootIndex,CardCount);
			m_bType.bbStraightFlush[i]=TRUE;
			//printf("\n%d,%d",ShootIndex[0],ShootIndex[1]);
			break;
		}

	}
	SetThreeCard();
	m_GameClientView.RefreshGameView();
	return true;
}
//定时器的操作
void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==40)
	{   
		static BYTE Number=0;
	    Number++;
		if(Number==4)
		{
			Number=0;
			KillTimer(40);
		}
		m_GameClientView.RefreshGameView();
	}
    if(nIDEvent==IDT_GUN_TIME)
	{
		if(m_nCurrentUser==GetMeChairID())
		{
			static int Number=0;
			Number++;
			if(Number%20==1)
			{
				//MessageBox(NULL,"1",NULL);
				m_GameClientView.SetnDrawFireWhichFrame(1);
				//m_GameClientView.RefreshGameView(NULL) ;
				m_GameClientView.SetbDrawFire(true);
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_GUNREADY"));
				m_GameClientView.RefreshGameView();
				//Invalidate();
			}
			if(Number%20==2)
			{
				m_GameClientView.SetnDrawFireWhichFrame(2);
				//Invalidate();
				m_GameClientView.RefreshGameView();
			}
			if(Number%20==3)
			{
				m_GameClientView.SetnDrawFireWhichFrame(3);
				//Invalidate();
				m_GameClientView.RefreshGameView();
			}
			if(Number%20==4)
			{
				m_GameClientView.SetnDrawFireWhichFrame(4);
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_GUN"));
				//Invalidate();
				m_GameClientView.RefreshGameView();
			}
			if(Number%20==5)
			{
				m_GameClientView.SetnDrawFireWhichFrame(5);
				//Invalidate();	
				m_GameClientView.RefreshGameView();
			}
			if(Number%20==12)
			{
				Number=0;
				m_GameClientView.m_nShootNum++;
				printf("获得几对客户打枪%d\n",m_ppBuffer.m_nXShoot);
				if(m_GameClientView.m_nShootNum == m_ppBuffer.m_nXShoot)
				{
					m_GameClientView.SetbDrawFire(false);
					KillTimer(IDT_GUN_TIME);
					m_GameClientView.RefreshGameView();
					Number=0;
					if(m_GameClientView.m_bFourbagger)
					{
						m_GameClientView.m_bShowFourbagger=true;
						SetTimer(IDT_FOURRBAGGETR_TIME,150,NULL);
						PlayGameSound(AfxGetInstanceHandle(),TEXT("QUAN_LEI_DA"));
					}
				}
				//m_GameClientView.RefreshGameView();
			}
		}
	}
	if(nIDEvent==20)
	{
		if(m_nCurrentUser==GetMeChairID())
		{
			//MessageBox(NULL,"shijian",NULL);
			m_bType=m_GameLogic.GetType(m_bHandCardData,m_bHandCardCount);
			if(m_bType.bOnePare)
			{
				m_GameClientView.m_btOnePare.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btOnePare.EnableWindow(FALSE);
			}
			if(m_bType.bTwoPare)
			{
				m_GameClientView.m_btTwoPare.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
			}
			if(m_bType.bThreeSame)
			{
				m_GameClientView.m_btThreeSame.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btThreeSame.EnableWindow(FALSE);
			}
			if(m_bType.bStraight)
			{
				m_GameClientView.m_btStraight.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btStraight.EnableWindow(FALSE);
			}
			if(m_bType.bFlush)
			{
				m_GameClientView.m_btFlush.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btFlush.EnableWindow(FALSE);
			}
			if(m_bType.bGourd)
			{
				m_GameClientView.m_btGourd.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btGourd.EnableWindow(FALSE);
			}
			if(m_bType.bFourSame)
			{
				m_GameClientView.m_btFourSame.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btFourSame.EnableWindow(FALSE);
			}
			if(m_bType.bStraightFlush)
			{
				m_GameClientView.m_btStraightFlush.EnableWindow(TRUE);
			}
			else
			{
				m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
			}
			KillTimer(20);
		}

	}
	if(nIDEvent==50)
	{
		
		if(m_GameClientView.m_UserSendCard0.m_nShuffleNum==9)
		{
			m_GameClientView.m_UserSendCard0.m_nShuffleNum=-1;
			m_bOnTimeEvent = true;
			KillTimer(50);
		}	
		if(m_bOnTimeEvent == false)
		{
			m_GameClientView.m_UserSendCard0.m_nShuffleNum++;
			printf("洗牌次数%d\n",m_GameClientView.m_UserSendCard0.m_nShuffleNum);

			m_GameClientView.m_bDrawShuffle=true;
			m_GameClientView.RefreshGameView();
		}
		
	}
	if(nIDEvent==IDT_AYAKA_TIME)
	{
		WORD wId=0;
		for(int i=0;i<GAME_PLAYER;i++)
		{
			wId=SwitchViewChairID(i);
			if(m_bSpecialType[i])
			{
				m_GameClientView.m_CompareCarControl[wId].m_nAyakaFrames++;
			}
			if(m_GameClientView.m_CompareCarControl[wId].m_nAyakaFrames==7)
			{
				m_GameClientView.m_CompareCarControl[wId].m_bAyaka=false;
				m_GameClientView.m_CompareCarControl[wId].m_nAyakaFrames=0;
				KillTimer(IDT_AYAKA_TIME);
			}
		}
		m_GameClientView.RefreshGameView();
	}
	if(nIDEvent==IDT_FOURRBAGGETR_TIME)
	{
		m_GameClientView.m_nFourbaggerFrames++;
		if(m_GameClientView.m_nFourbaggerFrames==14)
		{
			m_GameClientView.m_bShowFourbagger=false;
			m_GameClientView.m_nFourbaggerFrames=0;
			KillTimer(IDT_FOURRBAGGETR_TIME);
		}
		m_GameClientView.RefreshGameView();
	}
	if(nIDEvent==10)
	{
		
		if(m_bOnTimeEvent==true)
		{
			if(m_nCurrentUser==GetMeChairID())
			{
				//-30,-160,-130,+23,-30,+160,+130,+23
				static int xPoint[4]={0};
				static int yPoint[4]={0};
				  
				static int nCardSpace[4]={0};
				static int Number=0;
				      
				if(Number<150)
				{
					xPoint[0]=-174;
					yPoint[0]=-310;
					xPoint[1]=-295;
					yPoint[1]=-157;
					xPoint[2]=-174;
					yPoint[2]=100;
					xPoint[3]=245;
					yPoint[3]=-157;

					nCardSpace[0]=24;
					nCardSpace[1]=18;
					nCardSpace[2]=24;
					nCardSpace[3]=18;
					for(int i=0;i<GAME_PLAYER;i++)
					{
						if(m_GameClientView.m_UserCardControl[i].m_CardDataItem.GetCount()>0)
						{
							if(i==0)
							{  
								m_GameClientView.m_UserSendCard[i].m_PosCard.cx-=(15-2*m_GameClientView.m_UserCardControl[i].m_nI);

								m_GameClientView.m_UserSendCard[i].m_PosCard.cy-=295.0f/5.0f;
							}
							if(i==1)
							{
								m_GameClientView.m_UserSendCard[i].m_PosCard.cx-=295.0f/5.0f;

								m_GameClientView.m_UserSendCard[i].m_PosCard.cy-=(15-2*m_GameClientView.m_UserCardControl[i].m_nI);
							}
							if(i==2)
							{
								m_GameClientView.m_UserSendCard[i].m_PosCard.cx-=(15-2*m_GameClientView.m_UserCardControl[i].m_nI);

								m_GameClientView.m_UserSendCard[i].m_PosCard.cy+=100.0f/5.0f;
							}
							if(i==3)
							{
								m_GameClientView.m_UserSendCard[i].m_PosCard.cx+=200.0f/5.0f;

								m_GameClientView.m_UserSendCard[i].m_PosCard.cy-=(15-2*m_GameClientView.m_UserCardControl[i].m_nI);
							}
								
								m_GameClientView.m_UserSendCard[i].m_NumCard++;

								if(m_GameClientView.m_UserCardControl[i].m_nI>=13)
								{
									m_GameClientView.m_UserSendCard[i].m_NumCard=-1;
								}
								if(m_GameClientView.m_UserSendCard[i].m_NumCard>=5)
								{
									PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_CARD"));
									m_GameClientView.m_UserCardControl[i].m_nI++;
									m_GameClientView.m_UserSendCard[i].m_NumCard=-1;	
									m_GameClientView.m_UserSendCard[i].m_PosCard.cx=0;
									m_GameClientView.m_UserSendCard[i].m_PosCard.cy=0;
									m_GameClientView.m_UserSendCard0.m_nCardNum--;
									
								}
							
							}
						}
						
					m_GameClientView.SetCardInfo(xPoint,yPoint,nCardSpace);
				}
				
				Number++;
				if(Number>91)
				{
					if((m_GameClientView.m_UserCardControl[0].m_nI>=13||m_GameClientView.m_UserCardControl[0].m_nI<=0)&&
						(m_GameClientView.m_UserCardControl[1].m_nI>=13||m_GameClientView.m_UserCardControl[1].m_nI<=0)&&
						(m_GameClientView.m_UserCardControl[2].m_nI>=13||m_GameClientView.m_UserCardControl[2].m_nI<=0)&&
						(m_GameClientView.m_UserCardControl[3].m_nI>=13||m_GameClientView.m_UserCardControl[3].m_nI<=0))
					{
						  
						Number=200;  
						m_GameClientView.m_UserSendCard0.m_nCardNum=0;
						for(int i=0;i<GAME_PLAYER;i++)
						{
							m_GameClientView.m_UserSendCard[i].m_PosCard.cx=0;
							m_GameClientView.m_UserSendCard[i].m_PosCard.cy=0;
						}
					}
				}
				   
				  
				if(Number==200)
				{
					printf("发牌完毕\n");
					for(int i=0;i<GAME_PLAYER;i++)
					{
						
						printf("玩家%d,坐标X%d,坐标Y%d\n",i,m_GameClientView.m_UserSendCard[i].m_PosCard.cx,m_GameClientView.m_UserSendCard[i].m_PosCard.cy);
					}

					m_bOnTimeEvent = false;   
					ZeroMemory(xPoint,sizeof(xPoint));
					ZeroMemory(yPoint,sizeof(yPoint));
					ZeroMemory(nCardSpace,sizeof(nCardSpace));
					Number=0;
					m_GameClientView.SetStart(true);
					m_GameClientView.RefreshGameView();
					if(IsLookonMode()==false)
					{
						m_GameClientView.m_bShowBorder = true;
						m_GameClientView.m_btOk.ShowWindow(SW_SHOW) ;
						m_GameClientView.m_btOk.EnableWindow(FALSE);
						
						m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW) ;
						m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
						m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW) ;
						m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
						m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW) ;
						m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;

						//m_GameClientView.m_btOnePare.ShowWindow(SW_SHOW);
						m_GameClientView.m_btOnePare.EnableWindow(FALSE);
						//m_GameClientView.m_btTwoPare.ShowWindow(SW_SHOW);
						m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
						//m_GameClientView.m_btThreeSame.ShowWindow(SW_SHOW);
						m_GameClientView.m_btThreeSame.EnableWindow(FALSE);
						//m_GameClientView.m_btStraight.ShowWindow(SW_SHOW);
						m_GameClientView.m_btStraight.EnableWindow(FALSE);
						//m_GameClientView.m_btFlush.ShowWindow(SW_SHOW);
						m_GameClientView.m_btFlush.EnableWindow(FALSE);
						//m_GameClientView.m_btGourd.ShowWindow(SW_SHOW);
						m_GameClientView.m_btGourd.EnableWindow(FALSE);
						//m_GameClientView.m_btFourSame.ShowWindow(SW_SHOW);
						m_GameClientView.m_btFourSame.EnableWindow(FALSE);
						//m_GameClientView.m_btStraightFlush.ShowWindow(SW_SHOW);
						m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
						//m_GameClientView.m_btPaiXu.ShowWindow(SW_HIDE);
						m_GameClientView.m_btPaiXu.EnableWindow(TRUE);

						if(m_GameClientView.m_bSpecialTypeview[m_nCurrentUser]==false)
						{
							SetTimer(20,200,NULL);
							m_GameClientView.m_btRecycleCard1.ShowWindow(SW_SHOW);
							m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
							m_GameClientView.m_btRecycleCard2.ShowWindow(SW_SHOW);
							m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
							m_GameClientView.m_btRecycleCard3.ShowWindow(SW_SHOW);
							m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
							m_GameClientView.m_btRecycleAllCard.ShowWindow(SW_SHOW);
							m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
						}
						else
						{
							//MessageBox(NULL,"teshu",NULL);
							GetSpecialCard(m_bHandCardData,13);
							BYTE cbCardData[5];
							for(int i=0;i<3;i++)
							{
								cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][10+i];
							}
							//保存扑克
							m_GameClientView.m_AllSegCardControl[2][0].SetCardData(cbCardData , 3) ;
							//隐藏扑克
							m_GameClientView.m_AllSegCardControl[2][0].SetDrawCard(false);
							ZeroMemory(cbCardData,sizeof(cbCardData));

							for(int i=0;i<5;i++)
							{
								cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][5+i];
							}
							//保存扑克
							m_GameClientView.m_AllSegCardControl[2][1].SetCardData(cbCardData , 5) ;
							//隐藏扑克
							m_GameClientView.m_AllSegCardControl[2][1].SetDrawCard(false);
							ZeroMemory(cbCardData,sizeof(cbCardData));

							for(int i=0;i<5;i++)
							{
								cbCardData[i]=m_bSpecialCardData[m_nCurrentUser][i];
							}
							//保存扑克
							m_GameClientView.m_AllSegCardControl[2][2].SetCardData(cbCardData , 5) ;
							//隐藏扑克
							m_GameClientView.m_AllSegCardControl[2][2].SetDrawCard(false);
							ZeroMemory(cbCardData,sizeof(cbCardData));

							CopyMemory(m_bHandCardData , m_bTmpHandCardData , sizeof(m_bHandCardData)) ;
							m_GameLogic.SortCardList(m_bHandCardData,13);
							//是否特殊牌型
							m_GameClientView.m_btSubmit.ShowWindow(TRUE);
							m_GameClientView.m_btSubmit.EnableWindow(TRUE);
							m_GameClientView.m_btCancel.ShowWindow(TRUE);
							m_GameClientView.m_btCancel.EnableWindow(TRUE);
							m_GameClientView.m_nSpecialType=m_GameLogic.GetCardType(m_bHandCardData,13,m_GameLogic.btCardSpecialData);
							m_GameClientView.m_bTishi=true;
						}
					}
					m_GameClientView.SetbSendAction(false);
					KillTimer(10);
				}
			}
		}
	}
	__super::OnTimer(nIDEvent);
	
}
//回收第一墩
LRESULT CGameClientDlg::OnRecycleFrontCard( WPARAM wParam, LPARAM lParam )
{
	BYTE CardCount=3;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][0],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][0],sizeof(m_bSegmentCard[m_nCurrentUser][0]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][0],sizeof(m_GameClientView.m_AllSegCardControl[2][0]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][0].SetDrawCard(false);
	m_GameClientView.SetbDrawFrontCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
	m_bbThreeCard[0]=false;
	SetTimer(20,20,NULL);
	if(m_bFinishSegment)
	{
		m_bFinishSegment=false;
	}
    m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_btOk.EnableWindow(FALSE);
	
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
	m_GameClientView.RefreshGameView();
	return true;
}
//回收第二墩
LRESULT CGameClientDlg::OnRecycleMidCard( WPARAM wParam, LPARAM lParam )
{
	BYTE CardCount=5;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][1],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][1],sizeof(m_bSegmentCard[m_nCurrentUser][1]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][1],sizeof(m_GameClientView.m_AllSegCardControl[2][1]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][1].SetDrawCard(false);
	m_GameClientView.SetbDrawMidCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
	if(m_bFinishSegment)
	{
		m_bFinishSegment=false;
	}
	m_bbThreeCard[1]=false;

	SetTimer(20,20,NULL);
	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_btOk.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
	m_GameClientView.RefreshGameView();
	return true;
}
//回收第三墩
LRESULT CGameClientDlg::OnRecycleBackCard( WPARAM wParam, LPARAM lParam )
{
	BYTE CardCount=5;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][2],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][2],sizeof(m_bSegmentCard[m_nCurrentUser][2]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][2],sizeof(m_GameClientView.m_AllSegCardControl[2][2]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][2].SetDrawCard(false);
	m_GameClientView.SetbDrawBackCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
	if(m_bFinishSegment)
	{
		m_bFinishSegment=false;
	} 
	m_bbThreeCard[2]=false;
	
	SetTimer(20,20,NULL);
	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_btOk.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);

	m_GameClientView.RefreshGameView();
	return true;
}
//回收全部
LRESULT CGameClientDlg::OnRecycleAllCard( WPARAM wParam, LPARAM lParam )
{
	BYTE CardCount=3;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][0],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][0],sizeof(m_bSegmentCard[m_nCurrentUser][0]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][2],sizeof(m_GameClientView.m_AllSegCardControl[2][2]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][0].SetDrawCard(false);
	m_GameClientView.SetbDrawFrontCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
	m_bbThreeCard[0]=false;
	CardCount=5;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][1],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][1],sizeof(m_bSegmentCard[m_nCurrentUser][1]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][2],sizeof(m_GameClientView.m_AllSegCardControl[2][2]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][1].SetDrawCard(false);
	m_GameClientView.SetbDrawMidCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
	m_bbThreeCard[1]=false;
	m_GameLogic.AppendCard(m_bSegmentCard[m_nCurrentUser][2],CardCount,m_bHandCardData,m_bHandCardCount);
	ZeroMemory(m_bSegmentCard[m_nCurrentUser][2],sizeof(m_bSegmentCard[m_nCurrentUser][2]));
	//ZeroMemory(m_GameClientView.m_AllSegCardControl[2][2],sizeof(m_GameClientView.m_AllSegCardControl[2][2]));
	m_GameLogic.SortCardList(m_bHandCardData , m_bHandCardCount) ;
	m_GameClientView.SetCardData(2 , m_bHandCardData , m_bHandCardCount , enHandCard) ;
	m_GameClientView.m_SegCardControl[2][2].SetDrawCard(false);
	m_GameClientView.SetbDrawBackCardType(false,11);
	//m_GameClientView.m_AllSegCardControl[2][0].ShowWindow(SW_HIDE);
	m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
	m_bbThreeCard[2]=false;
	if(m_bFinishSegment)
	{
		m_bFinishSegment=false;
	} 
	SetTimer(20,20,NULL);
	m_GameClientView.m_btFrontCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btFrontCard.EnableWindow(FALSE);
	m_GameClientView.m_btMidCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btMidCard.EnableWindow(FALSE);
	m_GameClientView.m_btBackCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btBackCard.EnableWindow(FALSE);
	m_GameClientView.m_btOk.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);

	m_GameClientView.RefreshGameView();
	return true;
}

LRESULT CGameClientDlg::OnSubmit(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_bTishi=false;
	m_GameClientView.m_btSubmit.ShowWindow(FALSE);
	m_GameClientView.m_btCancel.ShowWindow(FALSE);
	m_bSpecialType[m_nCurrentUser]=true;
	OnShowCard(0,0);
	m_GameClientView.RefreshGameView();
	return true;
}

LRESULT CGameClientDlg::OnCancel(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_bTishi=false;
	m_GameClientView.m_btRecycleCard1.ShowWindow(SW_SHOW);
	m_GameClientView.m_btRecycleCard1.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard2.ShowWindow(SW_SHOW);
	m_GameClientView.m_btRecycleCard2.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleCard3.ShowWindow(SW_SHOW);
	m_GameClientView.m_btRecycleCard3.EnableWindow(FALSE);
	m_GameClientView.m_btRecycleAllCard.ShowWindow(SW_SHOW);
	m_GameClientView.m_btRecycleAllCard.EnableWindow(FALSE);
	m_GameClientView.m_btSubmit.ShowWindow(FALSE);
	m_GameClientView.m_btCancel.ShowWindow(FALSE);
	SetTimer(20,200,NULL);
	m_GameClientView.RefreshGameView();
	return true;
}

LRESULT CGameClientDlg::OnAdminCommand( WPARAM wParam, LPARAM lParam )
{
	SendSocketData(SUB_C_AMDIN_COMMAND,(CMD_C_ControlData*)wParam,sizeof(CMD_C_ControlData));
	return true;
}
void CGameClientDlg::SetThreeCard()
{
	BYTE bCardData[HAND_CARD_COUNT];
	BYTE bShootCount=(BYTE)m_GameClientView.m_UserCardControl[2].GetShootCard(bCardData,HAND_CARD_COUNT);

	//设置按钮
	if(5==bShootCount)
	{
		if((!m_bbThreeCard[1]))
			m_GameClientView.m_btMidCard.EnableWindow(TRUE) ;
		if((!m_bbThreeCard[2]))
			m_GameClientView.m_btBackCard.EnableWindow(TRUE) ;
	}
	else if(3==bShootCount)
	{
		if((!m_bbThreeCard[0]))
			m_GameClientView.m_btFrontCard.EnableWindow(TRUE) ;
	}
	else
	{
			m_GameClientView.m_btMidCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btBackCard.EnableWindow(FALSE) ;
			m_GameClientView.m_btFrontCard.EnableWindow(FALSE) ;
	}
}

bool CGameClientDlg::ComparePlayerCard(const void * pBuffer, WORD wDataSize)
{
	for(int i=0;i<4;i++)
	{
		if(m_bPlayer[i])
		{
			for(int j=0;j<3;j++)
			{
                 int WhichChair=SwitchViewChairID(i);
                 m_GameClientView.m_AllSegCardControl[WhichChair][j].SetDrawCard(true);
			}
		}
	}
    m_GameClientView.m_AllSegCardControl[2][0].SetDisplayFlag(FALSE);
	m_GameClientView.m_AllSegCardControl[2][1].SetDisplayFlag(FALSE);   
	m_GameClientView.m_AllSegCardControl[2][2].SetDisplayFlag(FALSE);
	
	m_GameClientView.SetStart(false);
	m_GameClientView.m_btOnePare.EnableWindow(FALSE);
	//m_GameClientView.m_btOnePare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btTwoPare.EnableWindow(FALSE);
	//m_GameClientView.m_btTwoPare.ShowWindow(SW_HIDE);
	m_GameClientView.m_btThreeSame.EnableWindow(FALSE);
	//m_GameClientView.m_btThreeSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraight.EnableWindow(FALSE);
	//m_GameClientView.m_btStraight.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFlush.EnableWindow(FALSE);
	//m_GameClientView.m_btFlush.ShowWindow(SW_HIDE);
	m_GameClientView.m_btGourd.EnableWindow(FALSE);
	//m_GameClientView.m_btGourd.ShowWindow(SW_HIDE);
	m_GameClientView.m_btFourSame.EnableWindow(FALSE);
	//m_GameClientView.m_btFourSame.ShowWindow(SW_HIDE);
	m_GameClientView.m_btStraightFlush.EnableWindow(FALSE);
	//m_GameClientView.m_btStraightFlush.ShowWindow(SW_HIDE);
	if(m_nCurrentUser==GetMeChairID())
	{
		m_ppBuffer=*(CMD_S_GameEnd*)pBuffer;
		m_wDataSize=wDataSize;
		m_GameClientView.SetSGameEnd(m_ppBuffer);
		CopyMemory(m_bSpecialType,m_ppBuffer.bSpecialCard , sizeof(m_bSpecialType));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			if(m_ppBuffer.cbThreeKillResult[i]>0)
			{
				m_GameClientView.m_wAllUser=SwitchViewChairID(i);
				m_GameClientView.m_bFourbagger=true;
			}
		}
		for(int i=0;i<GAME_PLAYER;i++)
		{
			WORD WhichChair=SwitchViewChairID(i);
			if(m_bPlayer[i])
			{
				if(m_bSpecialType[i])
				{
					m_GameClientView.m_bDrawCardKuang[WhichChair]=true;
					m_nSpecialType++;
				}
			}
			if(m_bPlayer[i]&&m_bSpecialType[i]==false)
			{
				for(int j=0;j<3;j++)
				{
					if(0==j)
					{
						m_btCardType[i][j]=m_GameLogic.GetCardType(m_bSegmentCard[i][j],3,m_GameLogic.btCardSpecialData);
					}
					else
					{
						m_btCardType[i][j]=m_GameLogic.GetCardType(m_bSegmentCard[i][j],5,m_GameLogic.btCardSpecialData);
					}
				}
			}
		}

		m_GameClientView.SetbDrawTimer(false);
		m_GameClientView.SetbDrawScore(true);
		SetGameClock(GetMeChairID() , IDI_COMPARE_CARD , IDT_COMPARE_TIME);
		printf("进入时间4\n");
	}
	m_GameClientView.RefreshGameView();
	return true;
}

BYTE CGameClientDlg::GetCardType( int Player,int WhichCard ,bool bFrontCard,bool bMidCard)
{
	if(m_bSpecialType[Player]==false)
	{
		if(m_btCardType[Player][WhichCard]==CT_SINGLE)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_PAI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\sanpai\\128.wav"));
			}*/
			return 0;
		}
		if(m_btCardType[Player][WhichCard]==CT_ONE_DOUBLE)
		{
				/*if(pUserData->cbGender)
				{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("DUI_ZI"));
				/*}
				else
				{
					PlayGameSound(TEXT("Thirteen\\sound\\女1\\duizi\\129.wav"));
				}*/
			return 1;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_TWO_DOUBLE)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("LIANG_DUI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\liangdui\\130.wav"));
			}*/
			return 2;
		}
		if(m_btCardType[Player][WhichCard]==CT_THREE&&!bFrontCard)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_TIAO"));
		/*	}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\santiao\\131.wav"));
			}*/
			return 3;
		}
		if(m_btCardType[Player][WhichCard]==CT_THREE&&bFrontCard)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_TIAO"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\santiao\\131.wav"));
			}*/
			return 3;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_MIXED_FLUSH_NO_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHUN_ZI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\sunzi\\132.wav"));
			}*/
			return 4;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_MIXED_FLUSH_FIRST_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHUN_ZI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\sunzi\\132.wav"));
			}*/
			return 4;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_MIXED_FLUSH_BACK_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SHUN_ZI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\sunzi\\132.wav"));
			}*/
			return 4;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_FLUSH)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TONG_HUA"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\tonghua\\133.wav"));
			}*/
			return 5;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_THREE_DEOUBLE&&bMidCard)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ZHONG_DUN_HU_LU"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\zhongdunhulu\\74.wav"));
			}*/
			return CT_FIVE_MID_THREE_DEOUBLE;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_THREE_DEOUBLE&&!bMidCard)
		{/*
			if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("HU_LU"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\hulu\\134.wav"));
			}*/
			return 6;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_FOUR_ONE)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TIE_ZI"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\tiezi\\135.wav"));
			}*/
			return 7;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_STRAIGHT_FLUSH_FIRST_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TONG_HUA_SHUN"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\tonghuasun\\136.wav"));
			}*/
			return 8;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_STRAIGHT_FLUSH_NO_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TONG_HUA_SHUN"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\tonghuasun\\136.wav"));
			}*/
			return 8;
		}
		if(m_btCardType[Player][WhichCard]==CT_FIVE_STRAIGHT_FLUSH_BACK_A)
		{
			/*if(pUserData->cbGender)
			{*/
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("TONG_HUA_SHUN"));
			/*}
			else
			{
				PlayGameSound(TEXT("Thirteen\\sound\\女1\\tonghuasun\\136.wav"));
			}*/
			return 8;
		}
	}
	else
	{
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THIRTEEN_FLUSH)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("ZHI_ZUN_QING_LONG"));
			return CT_THIRTEEN_FLUSH;
		}
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THIRTEEN)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("YI_TIAO_LONG"));
			return CT_THIRTEEN;
		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_TWELVE_KING)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("12HUANG_ZU"));
 			return CT_TWELVE_KING;
 		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THREE_STRAIGHTFLUSH)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_TONG_HUA_SHUN"));
 			return CT_THREE_STRAIGHTFLUSH;
 		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THREE_BOMB)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_FEN_TIAN_XIA"));
 			return CT_THREE_BOMB;
		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_ALL_BIG)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("QUAN_DA"));
 			return CT_ALL_BIG;
 		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_ALL_SMALL)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("QUAN_XIAO"));
 			return CT_ALL_SMALL;
 		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_SAME_COLOR)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("CHOU_YI_SE"));
 			return CT_SAME_COLOR;
 		}
 		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_FOUR_THREESAME)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("4TAO_3TIAO"));
 			return CT_FOUR_THREESAME;
 		}
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_FIVEPAIR_THREE)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("5DUI_3TIAO"));
			return CT_FIVEPAIR_THREE;
		}
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_SIXPAIR)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("LIU_DUI_BAN"));
			return CT_SIXPAIR;
		}
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THREE_FLUSH)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_TONG_HUA"));
			return CT_THREE_FLUSH;
		}
		if(m_GameLogic.GetCardType(m_bAllHandCardData[Player],13,m_GameLogic.btCardSpecialData)==CT_THREE_STRAIGHT)
		{
			if(GetMeChairID() == Player)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("SAN_SHUN_ZI"));
			return CT_THREE_STRAIGHT;
		}
	}
	return 0;
}

int CGameClientDlg::GetCardType( BYTE BCardType,bool bFrontCard,bool bMidCard )
{
	if(BCardType==CT_SINGLE)
	{
		return 0;
	}
	if(BCardType==CT_ONE_DOUBLE)
	{
		return 1;
	}
	if(BCardType==CT_FIVE_TWO_DOUBLE)
	{
		return 2;
	}
	if(BCardType==CT_THREE&&!bFrontCard)
	{
		return 3;
	}
	if(BCardType==CT_THREE&&bFrontCard)
	{
		return 4;
	}
	if(BCardType==CT_FIVE_MIXED_FLUSH_NO_A)
	{
		return 5;
	}
	if(BCardType==CT_FIVE_MIXED_FLUSH_FIRST_A)
	{
		return 5;
	}
	if(BCardType==CT_FIVE_MIXED_FLUSH_BACK_A)
	{
		return 5;
	}
	if(BCardType==CT_FIVE_FLUSH)
	{
		return 6;
	}
	if(BCardType==CT_FIVE_THREE_DEOUBLE&&bMidCard)
	{
		return 7;
	}
	if(BCardType==CT_FIVE_THREE_DEOUBLE&&!bMidCard)
	{
		return 8;
	}
	if(BCardType==CT_FIVE_FOUR_ONE)
	{
		return 9;
	}
	if(BCardType==CT_FIVE_STRAIGHT_FLUSH_FIRST_A)
	{
		return 10;
	}
	if(BCardType==CT_FIVE_STRAIGHT_FLUSH_NO_A)
	{
		return 10;
	}
	if(BCardType==CT_FIVE_STRAIGHT_FLUSH_BACK_A)
	{
		return 10;
	}
	return 11;
}

void CGameClientDlg::GetSpecialCard( BYTE CardData[],BYTE CardCount )
{
    BYTE btSpecialCard[13];
    ZeroMemory(btSpecialCard,sizeof(btSpecialCard));
	tagAnalyseData AnalyseData ;
	m_GameLogic.AnalyseCard(CardData , CardCount , AnalyseData);

	if(m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THIRTEEN_FLUSH||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THIRTEEN||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_TWELVE_KING||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THREE_STRAIGHTFLUSH||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THREE_BOMB||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_ALL_BIG||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_ALL_SMALL||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_SAME_COLOR||
		m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_FOUR_THREESAME)
	{
		CopyMemory(m_bSpecialCardData[m_nCurrentUser],CardData,sizeof(m_bSpecialCardData[m_nCurrentUser]));
		return;
	}
	if(m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THREE_FLUSH)
	{
        CopyMemory(m_bSpecialCardData[m_nCurrentUser],btSpecialCard,sizeof(btSpecialCard));
		return;
	}
	if(m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_THREE_STRAIGHT)
	{
		CopyMemory(m_bSpecialCardData[m_nCurrentUser],btSpecialCard,sizeof(btSpecialCard));
		return;
	}
	if(m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_FIVEPAIR_THREE)
	{
		BYTE SaveData[5];
		ZeroMemory(SaveData,sizeof(SaveData));
		SaveData[0]=CardData[AnalyseData.bThreeFirst[0]];
		SaveData[1]=CardData[AnalyseData.bThreeFirst[0]+1];
		SaveData[2]=CardData[AnalyseData.bThreeFirst[0]+2];
		m_GameLogic.RemoveCard(SaveData,3,CardData,CardCount);
		CardCount-=3;
		m_GameLogic.SortCardList(CardData,CardCount);
		for(int i=0;i<CardCount;i++)
		{
			m_bSpecialCardData[m_nCurrentUser][i]=CardData[i];
		}
		m_bSpecialCardData[m_nCurrentUser][10]=SaveData[0];
		m_bSpecialCardData[m_nCurrentUser][11]=SaveData[1];
		m_bSpecialCardData[m_nCurrentUser][12]=SaveData[2];

		return;
	}
	if(m_GameLogic.GetCardType(CardData,CardCount,btSpecialCard)==CT_SIXPAIR)
	{
		BYTE SaveData[5];
		ZeroMemory(SaveData,sizeof(SaveData));
		SaveData[0]=CardData[AnalyseData.bOneFirst[0]];
		m_GameLogic.RemoveCard(SaveData,1,CardData,CardCount);
		CardCount-=1;
		m_GameLogic.SortCardList(CardData,CardCount);
		for(int i=0;i<CardCount;i++)
		{
			m_bSpecialCardData[m_nCurrentUser][i]=CardData[i];
		}
		m_bSpecialCardData[m_nCurrentUser][12]=SaveData[0];
		return;
	}
}

bool CGameClientDlg::OnSubPlayerLeft( const void * pBuffer , WORD wDataSize )
{
	//MessageBox(NULL,"tui",NULL);
	ASSERT(wDataSize==sizeof(CMD_S_PlayerLeft));
	if (wDataSize!=sizeof(CMD_S_PlayerLeft)) return false;
	if(m_bCompareCard==false)
	{
		CMD_S_PlayerLeft *pPlayerLeft=(CMD_S_PlayerLeft*)pBuffer;
		m_bPlayer[pPlayerLeft->nPlayerLetfChairID]=false;
		m_GameClientView.SetbPlayer(m_bPlayer);
		int PlayerLeftChairID=pPlayerLeft->nPlayerLetfChairID;
		int PlayerLeftViewID=SwitchViewChairID(PlayerLeftChairID);
     
		m_GameClientView.m_UserCardControl[PlayerLeftViewID].SetDrawCard(false);
        for(int i=0;i<3;i++)
		{
			m_GameClientView.m_AllSegCardControl[PlayerLeftViewID][i].SetDrawCard(false);
			m_GameClientView.m_SegCardControl[PlayerLeftViewID][i].SetDrawCard(false);
		}
		m_nPlayer--;
	}
	
    return true;
}

LRESULT CGameClientDlg::OnOperateMessage( WPARAM wParam, LPARAM lParam )
{
	//int id= lParam;
	//WORD wMeChairID=GetMeChairID();
	////BYTE cardData[4]; 
	//switch(id)
	//{
	//case IDM_START:
	//	{
	//		OnStart(0,0);
	//		m_GameClientView.RefreshGameView();
 //          break;
	//	}
	//case IDM_END:
	//	{
	//		OnEnd(0,0);
	//		m_GameClientView.RefreshGameView();
 //          break;
	//	}
	//case IDM_FRONTCARD:
	//	{
	//		OnFrontCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_BACKCARD:
	//	{
	//		OnBackCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_MIDCARD:
	//	{
	//		OnMidCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_OK:
	//	{
	//		
	//		OnShowCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_ONEPARE:
	//	{
	//		OnOnePareCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_TWOPARE:
	//	{
	//		OnTwoPareCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_THREESAME:
	//	{
	//		OnThreeSameCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_STRAIGHT:
	//	{
	//		OnStraightCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_FLUSH:
	//	{
	//		OnFlushCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_GOURD:
	//	{
	//		OnGourdCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_FOURSAME:
	//	{
	//		OnFourSameCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_STRAIGHTFLUSH:
	//	{
	//		OnStraightFlushCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_RECYCLEFRONTCARD:
	//	{
	//		OnRecycleFrontCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_RECYCLEMIDCARD:
	//	{
	//		OnRecycleMidCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_RECYCLEBACKCARD:
	//	{
	//		OnRecycleBackCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//case IDM_RECYCLEALLCARD:
	//	{
	//		OnRecycleAllCard(0,0);
	//		m_GameClientView.RefreshGameView();
	//		break;
	//	}
	//}
   return true;
}

void CGameClientDlg::ResetDlgData()
{
	//游戏变量
	//MessageBox(NULL,"women",NULL);
	m_bHandCardCount=0 ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bHandCardData , sizeof(m_bHandCardData)) ;
	ZeroMemory(m_bTmpHandCardData , sizeof(m_bTmpHandCardData)) ;
	ZeroMemory(m_btCardType,sizeof(m_btCardType));
	m_wUserToltalChip=0;
	m_bReady = false ;
	ZeroMemory(&m_ppBuffer,sizeof(m_ppBuffer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	m_wDataSize=0;
	m_nPlayer=0;
	m_nWhichCard=0;
	m_bDraw=true;
	m_nSpecialType=0;
	m_bShowCard=false;
	m_bStrat=false;
	m_bCompareCard=false;
	bFirst=true;
	ZeroMemory(&m_Buffer,sizeof(m_Buffer));
	ZeroMemory(m_bSpecialType,sizeof(m_bSpecialType));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bAllHandCardData,sizeof(m_bAllHandCardData));
	ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));

}

bool CGameClientDlg::OnControlPrompt( const void * pBuffer, WORD wDataSize )
{
	if(wDataSize != sizeof(CMD_C_ControlData)) return false;
	CMD_C_ControlData* pControlData = (CMD_C_ControlData*)pBuffer;
	IClientUserItem *pUser = GetTableUserItem(pControlData->dwChairID);
	if(pUser == NULL) return false;

	m_GameClientView.m_ClientControl.PromptMessage(pUser->GetNickName(),pControlData->bWinloss,pControlData->cbCount);
	return true;
}

//////////////////////////////////////////////////////////////////////////
