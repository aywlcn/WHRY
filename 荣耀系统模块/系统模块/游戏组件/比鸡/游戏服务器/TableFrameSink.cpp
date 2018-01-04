#include "StdAfx.h"
#include "TableFrameSink.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//开始模式
CAndroidUserItemSink     *  pAndroidUserItemSink = new CAndroidUserItemSink();							//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//AllocConsole();
	//freopen("CON","wt",stdout);
	srand((unsigned)time(NULL));
	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	m_nLeftAndroidchairID=-1;
	m_bNomalEnd=false;

	//客户端控制
	m_bControl = false;
	m_bControlLossWin = false;
	m_dwControlID = INVALID_CHAIR;
	m_nControlCount = 0;
	//游戏变量
	memset(&m_sGameEnd,0,sizeof(m_sGameEnd));
	ZeroMemory(m_bCompareResult , sizeof(m_bCompareResult)) ;
	ZeroMemory(m_bShootState , sizeof(m_bShootState)) ;
	ZeroMemory(m_bThreeKillResult , sizeof(m_bThreeKillResult)) ;
	ZeroMemory(m_bToltalWinDaoShu , sizeof(m_bToltalWinDaoShu)) ;
	ZeroMemory(m_bCompareDouble , sizeof(m_bCompareDouble)) ;
	ZeroMemory(m_bSpecialCompareResult , sizeof(m_bSpecialCompareResult)) ;
	ZeroMemory(m_lGameScore , sizeof(m_lGameScore)) ;
	m_nXShoot = 0;

	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	ZeroMemory(m_bSpecialTypeTable,sizeof(m_bSpecialTypeTable));
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;
	m_nPlayerCount=0;
	m_bOnePlayer=false;
	ZeroMemory(m_lpLeftszName,sizeof(m_lpLeftszName));
	ZeroMemory(m_nLeftChairID,sizeof(m_nLeftChairID));
	ZeroMemory(m_lLeftScore,sizeof(m_lLeftScore));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	ZeroMemory(m_tagScoreInfo,sizeof(m_tagScoreInfo));
	ZeroMemory(m_lLefUserScore,sizeof(m_lLefUserScore));

	ZeroMemory(m_nResult, sizeof(m_nResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));//计算道数
	ZeroMemory(m_nAllResult, sizeof(m_nAllResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));
	ZeroMemory(m_lGameScoreEx, sizeof(m_lGameScoreEx));
	ZeroMemory(m_nSpecialType, sizeof(m_nSpecialType));
	ZeroMemory(m_bShowCardChairId, sizeof(m_bShowCardChairId));
	ZeroMemory(m_nIndexResult, sizeof(m_nIndexResult));
	ZeroMemory(m_bTongGuan, sizeof(m_bTongGuan));
	ZeroMemory(m_nEveryType, sizeof(m_nEveryType));
	ZeroMemory(m_nSpecialCount, sizeof(m_nSpecialCount));
	ZeroMemory(m_cbCard, sizeof(m_cbCard));
	ZeroMemory(m_nSpecialTongGuan, sizeof(m_nSpecialTongGuan));
	m_nGamePlays = 0;
	ZeroMemory(m_rgArea1, sizeof(m_rgArea1));
	ZeroMemory(m_rgArea2, sizeof(m_rgArea2));
	ZeroMemory(m_rgArea3, sizeof(m_rgArea3));



	m_lGameEndTax=0;
	m_bGameEnd=false;
	m_nAndroidCount=0;
	m_lCellScore=0;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		m_bOverTime[i] = false ;
		m_bUserLeft[i] = false ;
	}
	//配置变量
	m_bIsCheat = false;
	m_WinnerScore = 0;
	m_lRobotloseMax = 0;
	m_lRobotLoseMaxChange = 0;
	m_btRobotLoseChangePercent = 0;
	m_lRealBankerRobotLoseMax = 0;
	m_lRobotProbabilityTanking = 0;
	m_lRobotDefineTanking = 0;
	m_btRobotTankingPercent = 0;
	m_lRealBankerRobotLoseMaxChange = 0;
	m_btRealBankerRobotLoseChangePercent = 0;
	m_btUsuallyChangePercent = 0;
	m_btRobotWinScorePercent = 0;
	m_lRobotWinChiefScore = 0;
	m_lRobotResultScore = 0;

	m_IsCheatUser = FALSE;
	m_dwUserID = 0;
	m_dwUserLoseWinPercent = 0;
	m_bLoseWin = FALSE;
	m_lWinMaxGold = 0;
	m_lLoseMaxGold = 0;
	m_lAllUserGold = 0;
	m_lUserGold = 0;
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);


	//获取配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\GameControl\\biji.ini"),szPath);
	_sntprintf(m_szConfigFilePath,sizeof(m_szConfigFilePath),TEXT("%s\\GameControl\\User.ini"),szPath);

	ASSERT(m_pGameServiceOption!=NULL);
	_sntprintf(m_szRoomName, sizeof(m_szRoomName), TEXT("%d"),m_pGameServiceOption->wServerID);
	return true;
}

//复位桌子
VOID CTableFrameSink::RepositionSink()
{
	//写入配置
	WriteGameINI();

	//游戏变量
	memset(&m_sGameEnd,0,sizeof(m_sGameEnd));
	ZeroMemory(m_bCompareResult , sizeof(m_bCompareResult)) ;
	ZeroMemory(m_bShootState , sizeof(m_bShootState)) ;
	ZeroMemory(m_bThreeKillResult , sizeof(m_bThreeKillResult)) ;
	ZeroMemory(m_bToltalWinDaoShu , sizeof(m_bToltalWinDaoShu)) ;
	ZeroMemory(m_bCompareDouble , sizeof(m_bCompareDouble)) ;
	ZeroMemory(m_bSpecialCompareResult , sizeof(m_bSpecialCompareResult)) ;
	ZeroMemory(m_lGameScore , sizeof(m_lGameScore)) ;
	m_nXShoot = 0;
	m_lUserGold = 0;
	m_WinnerScore = 0;

	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData));
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment));
	ZeroMemory(m_bSpecialTypeTable,sizeof(m_bSpecialTypeTable));
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;
	m_nPlayerCount=0;
	m_bOnePlayer=false;
	m_bGameEnd=false;
	ZeroMemory(m_lpLeftszName,sizeof(m_lpLeftszName));
	ZeroMemory(m_nLeftChairID,sizeof(m_nLeftChairID));
	ZeroMemory(m_lLeftScore,sizeof(m_lLeftScore));
	ZeroMemory(m_bPlayer,sizeof(m_bPlayer));
	ZeroMemory(m_bDragon,sizeof(m_bDragon));
	ZeroMemory(m_lLefUserScore,sizeof(m_lLefUserScore));
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		m_bOverTime[i] = false ;
		m_bUserLeft[i] = false ;
	}
	return;
}

//消费能力
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//效验状态
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetTableID()==m_pITableFrame->GetTableID()));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetTableID()!=m_pITableFrame->GetTableID())) return 0;

	//用户判断
	switch (pIServerUserItem->GetUserStatus())
	{
	case US_SIT:		//坐下状态
	case US_READY:		//准备状态
		{
			return pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore;
		}
	case US_PLAYING:	//游戏状态
	case US_OFFLINE:	//断线状态
		{
			WORD wMaxTimes=m_pGameCustomRule->wMaxScoreTimes;
			return pIServerUserItem->GetUserScore()-m_lCellScore*wMaxTimes;
		}
	case US_LOOKON:		//旁观状态
		{
			return pIServerUserItem->GetUserScore();
		}
	}

	return 0L;
}

//最少积分
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0L;
}
//开始模式
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}
//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	IServerUserItem * pIServerUserItem=NULL;
	memset(m_bShowCardChairId, 0, sizeof(m_bShowCardChairId));

	m_bControl = false;
	LONGLONG lMixScore=0;
	BYTE nCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;
		nCount++;
		if(nCount==1)
			lMixScore=pIServerUserItem->GetUserScore();
		else
		{
			lMixScore=__min(lMixScore,pIServerUserItem->GetUserScore());
		}
	}
	//m_lCellScore=__max(lMixScore/36L,m_pGameServiceOption->lCellScore);
	m_lCellScore = m_pGameServiceOption->lCellScore;
	//游戏变量
	ZeroMemory(m_bUserCardData , sizeof(m_bUserCardData)) ;
	ZeroMemory(m_bSegmentCard , sizeof(m_bSegmentCard)) ;
	ZeroMemory(m_bFinishSegment , sizeof(m_bFinishSegment)) ;
	m_bShowCardCount=0 ; 
	m_bCompleteCompareCount=0 ;

	int Number=0;
	for(WORD i=0; i<GAME_PLAYER; i++)
	{
		pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem!=NULL)
		{
			m_bPlayer[i]=true;
			Number++;
		}
	}
	m_nPlayerCount=Number;
	for(BYTE i=0; i<GAME_PLAYER; ++i)
	{
		m_bOverTime[i] = false ;
		m_bUserLeft[i] = false ;
	}
	//设置状态
	m_pITableFrame->SetGameStatus(GS_WK_PLAYING);
	BYTE bCardData[52];
	m_GameLogic.RandCardList(bCardData , 52);
	CopyMemory(m_cbCard, bCardData, sizeof(bCardData));
	


	for(int i=0;i<GAME_PLAYER;i++)
	{
		CopyMemory(m_bUserCardData[i] , bCardData+i*HAND_CARD_COUNT,HAND_CARD_COUNT);
	}
	//------控制换牌------
// 	LoadGameINI();
// 
// 	//客户端控制换牌
// 	if(m_nControlCount>0 && m_dwControlID!=INVALID_CHAIR)
// 	{
// 		ClientControl();
// 	}
// 	else
// 	{
// 		bool IsUser = false;
// 		if(m_IsCheatUser)
// 		{
// 			IsUser = UserControl();
// 		}
// 		if(!IsUser)
// 		{
// 			if(m_bIsCheat)
// 				ControlCheat();
// 		}
// 	}
	///////////////////////

	CMD_S_SendCard SendCard ;
	memset(&SendCard , 0 , sizeof(SendCard));
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem)
			SendCard.bGameStatus[i] = true;
		SendCard.lChipArray[i] = 0;
	}

	CMD_S_MobilePutCard MobilePutCard;

//	SendCard.lCellScore = m_lCellScore;


	//保存扑克
	for(WORD i=0 ; i<GAME_PLAYER ; i++)
	{
		CopyMemory(SendCard.bCardData , m_bUserCardData[i] , HAND_CARD_COUNT);
		CopyMemory(MobilePutCard.cbMobilePutCard, m_bUserCardData, sizeof(MobilePutCard.cbMobilePutCard));
		MobilePutCard.cbMobilePutCount = 3;
		SendCard.wCurrentUser = i ;
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		CMD_S_GameStart GameStart;
		GameStart.wCurrentUser = 0;
		GameStart.wUserToltalChip = 0;
		if(pIServerUserItem)
		{
			m_pITableFrame->SendTableData(i, SUB_S_GAME_START, &GameStart, sizeof(CMD_S_GameStart));
 			m_pITableFrame->SendTableData(i, SUB_S_SEND_CARD, &SendCard, sizeof(CMD_S_SendCard));
 			//m_pITableFrame->SendTableData(i, SUB_S_MOBILE_PUTCARD, &MobilePutCard, sizeof(CMD_S_MobilePutCard));
			m_GameLogic.RemoveCard(m_bUserCardData[i], HAND_CARD_COUNT, m_cbCard, _countof(m_cbCard));

			m_pITableFrame->SendLookonData(i, SUB_S_SEND_CARD, &SendCard, sizeof(CMD_S_SendCard));
		}
	}

	//DWORD m_dwJettonTime = (DWORD)time(NULL);
	m_pITableFrame->SetGameTimer(IDI_GAME_END, 60 * 1000, 1, 0L);
	time(&m_tNowTimeStamp);

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_DISMISS:
	{
						ASSERT(pIServerUserItem != NULL);
						ASSERT(wChairID < m_wPlayerCount);

						CMD_S_GameEnd GameEnd;
						memset(&GameEnd, 0, sizeof(GameEnd));

						GameEnd.bEndMode = GER_DISMISS;

						//发送信息
						m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

						//结束游戏
						m_pITableFrame->ConcludeGame(GS_WK_FREE);

						return true;
	}
	case GER_NORMAL:
	{
					   CMD_S_GameEnd GameEnd;
					   memset(&GameEnd, 0, sizeof(GameEnd));

					   GameEnd.bEndMode = GER_NORMAL;

					   ComputeChoutEx();

					   CopyMemory(GameEnd.cbCompareResult, m_nResult, sizeof(m_nResult));
					   CopyMemory(GameEnd.nAllResult, m_nAllResult, sizeof(m_nAllResult));
					   CopyMemory(GameEnd.lGameScore, m_lGameScoreEx, sizeof(m_lGameScoreEx));
// 					   CopyMemory(GameEnd.nSpecialResult->nSpecialType, m_nSpecialType, sizeof(m_nSpecialType));
// 					   CopyMemory(GameEnd.nSpecialResult->nSpecialResult, m_nSpecialResult, sizeof(m_nSpecialResult));

					   for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
					   {
						   //玩家判断;
						   if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
						   {
							   continue;
						   }
						   GameEnd.nSpecialResult[wUser].nSpecialScore = m_nSpecialScore[wUser];
						   GameEnd.nSpecialResult[wUser].nSpecialType = m_nSpecialType[wUser];
						   GameEnd.nSpecialResult[wUser].nSpecialCount = m_nSpecialCount[wUser];
					   }
					   CopyMemory(GameEnd.nEveryType, m_nEveryType, sizeof(m_nEveryType));
					   CopyMemory(GameEnd.nIndexResult, m_nIndexResult, sizeof(m_nIndexResult));



					   // 积分变量
					   tagScoreInfo ScoreInfoArray[GAME_PLAYER];
					   ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));

					   // 统计积分
					   for (WORD i = 0; i < GAME_PLAYER; ++i)
					   {
						   if (!IsUserPlaying(i))
							   continue;
						   IServerUserItem * pIUserItem = m_pITableFrame->GetTableUserItem(i);
						   // 计算积分
						   ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];

						   ScoreInfoArray[i].cbType = (ScoreInfoArray[i].lScore >= 0L) ? SCORE_TYPE_WIN : SCORE_TYPE_LOSE;
						   // 计算税收
						   ScoreInfoArray[i].lRevenue = m_pITableFrame->CalculateRevenue(i, ScoreInfoArray[i].lScore);
						   if (ScoreInfoArray[i].lRevenue > 0L) ScoreInfoArray[i].lScore -= ScoreInfoArray[i].lRevenue;

						   // 设置积分
						   GameEnd.lGameScore[i] = ScoreInfoArray[i].lScore;

					   }

					   m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));

					   //发送信息
					   m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
					   m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

					   //结束游戏
					  m_pITableFrame->ConcludeGame(GS_WK_FREE);

					   return true;
	}
	case GER_USER_LEAVE:
	{
						   CMD_S_GameEnd GameEnd;
						   memset(&GameEnd, 0, sizeof(GameEnd));

						   GameEnd.bEndMode = GER_USER_LEAVE;

						   // 积分变量
						   tagScoreInfo ScoreInfoArray[GAME_PLAYER];
						   ZeroMemory(&ScoreInfoArray, sizeof(ScoreInfoArray));
						   GameEnd.lGameScore[wChairID] -= max(m_lCellScore * 5, pIServerUserItem->GetUserScore());

						   // 统计积分
						   for (WORD i = 0; i < GAME_PLAYER; ++i)
						   {
							   // 计算积分
							   ScoreInfoArray[i].lScore = GameEnd.lGameScore[i];

							   // 计算税收
							   ScoreInfoArray[i].lRevenue = m_pITableFrame->CalculateRevenue(i, ScoreInfoArray[i].lScore);
							   if (ScoreInfoArray[i].lRevenue > 0L) ScoreInfoArray[i].lScore -= ScoreInfoArray[i].lRevenue;

							   // 设置积分
							   GameEnd.lGameScore[i] = ScoreInfoArray[i].lScore;

							   //历史积分
							   //m_HistoryScore.OnEventUserScore(i, GameEnd.lGameScore[i]);
						   }

						   // 发送数据
						   m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));
						   m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_GAME_END, &GameEnd, sizeof(GameEnd));

						   // 写入积分
						   m_pITableFrame->WriteTableScore(ScoreInfoArray, CountArray(ScoreInfoArray));



						   //结束游戏
						   m_pITableFrame->ConcludeGame(GS_WK_FREE);

						   return true;
	}
	case GER_NETWORK_ERROR:
	{
								   m_pITableFrame->ConcludeGame(GS_WK_FREE);
								   return true;
	}
	}

	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret)
{
	m_pITableFrame->GetGameRule();
	switch (bGameStatus)
	{
	case GS_WK_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));
			//StatusFree.cbTimeStartGame = m_pGameCustomRule->cbTimeStartGame;
			StatusFree.cbTimeStartGame = 30;

			//printf("%d",pIServerUserItem->GetUserID());
			if(pIServerUserItem->IsAndroidUser())
			{
				pIServerUserItem->SetAndroidUser(true);
			}
			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_WK_PLAYING:		//游戏状态
		{
			time_t ttNow = time(NULL);
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//StatusPlay.wCurrentUser = wChiarID;
			//StatusPlay.wUserToltalChip=pIServerUserItem->GetUserScore();
			StatusPlay.lCellScore=m_lCellScore;

			CopyMemory(StatusPlay.bHandCardData , m_bUserCardData[wChiarID] , HAND_CARD_COUNT) ;
			CopyMemory(StatusPlay.bSegmentCard , m_bSegmentCard , sizeof(m_bSegmentCard)) ;
			CopyMemory(StatusPlay.bFinishSegment , m_bFinishSegment , sizeof(m_bFinishSegment)) ;

			CopyMemory(StatusPlay.bGameStatus, m_bPlayer, sizeof(m_bPlayer));
			//CopyMemory(StatusPlay.bOverTime, m_bOverTime, sizeof(StatusPlay.bOverTime)) ;

			//CopyMemory(StatusPlay.bSpecialTypeTable1, m_bSpecialTypeTable, sizeof(StatusPlay.bSpecialTypeTable1)) ;
			//CopyMemory(StatusPlay.bDragon1, m_bDragon, sizeof(StatusPlay.bDragon1)) ;
			//CopyMemory(StatusPlay.bAllHandCardData,m_bUserCardData,sizeof(StatusPlay.bAllHandCardData));
			//StatusPlay.sGameEnd=m_sGameEnd;

			int nTime = 0;
			nTime = ttNow - m_tNowTimeStamp;
			if (nTime >= 60 || nTime < 0 )
			{
				StatusPlay.bCbTimeRangeCard = 0;
			}
			else
			{
				StatusPlay.bCbTimeRangeCard = 60 - nTime;
			}

			StatusPlay.bCbTimeStartGame = 0;
			StatusPlay.bCbTimeCallBanker = 0;
			StatusPlay.bCbTimeSetChip = 0;
			StatusPlay.bCbTimeShowCard = 0;

			//设置变量

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_WRITE_SCORE:
	{
							printf("***************************************************9\n");
							//MessageBox(NULL,"shijian",NULL,NULL);
							for (int i = 0; i < GAME_PLAYER; i++)
							{
								if (m_bPlayer[i] && m_bUserLeft[i] == false)
								{

									m_pITableFrame->WriteUserScore(i, m_tagScoreInfo[i]);

								}
							}
							m_pITableFrame->KillGameTimer(IDI_WRITE_SCORE);
							CMD_S_ScoreResult ScoreResult;
							ScoreResult.bScoreResult = true;
							m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_SCORE_RESULT, &ScoreResult, sizeof(ScoreResult));
							m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_SCORE_RESULT, &ScoreResult, sizeof(ScoreResult));
							m_pITableFrame->ConcludeGame(GS_WK_FREE);
							return true;
	}
	case IDI_GAME_END:
	{
						 for (INT nChairId = 0; nChairId < _countof(m_bShowCardChairId); nChairId++)
						 {
							 if (!m_bShowCardChairId[nChairId])
							 {
								 IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(nChairId);
								 if (pIServerUserItem == NULL)
								 {
									 continue;
								 }

								 if (pIServerUserItem->GetUserStatus() == US_PLAYING)
								 {
									 SetSegCard sscCard;
									 ZeroMemory(&sscCard, sizeof(sscCard));


									 pAndroidUserItemSink->OnSetSegment(m_bUserCardData[nChairId], sizeof(m_bUserCardData[nChairId]), sscCard);

									 OnShowCard(nChairId, &sscCard, sizeof(sscCard));

								 }
							 }
						 }

						 m_pITableFrame->KillGameTimer(IDI_GAME_END);
						 return true;

	}
	}


	return false;
}

//游戏消息处理
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{

	switch (wSubCmdID)
	{
	case SUB_C_SEGCARD:				//设置分段
		{	
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			return OnSetSegment(pIServerUserItem->GetChairID() , pDataBuffer, wDataSize) ;
		}
	case SUB_C_SHOWCARD:			//玩家摊牌
		{
			// 用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus != US_PLAYING)
			{
				return true;
			}

			return OnShowCard(pIServerUserItem->GetChairID() , pDataBuffer, wDataSize) ;
		}
	case SUB_C_OVERTIME:			//玩家超时
		{
			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//设置变量
			m_bOverTime[pIServerUserItem->GetChairID()] = true;
			m_bShowCardCount++ ;

			//计算人数
			int nPlayerCount=0 ; 
			for(WORD i=0 ; i<GAME_PLAYER ; ++i)
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
				if(pIServerUserItem) ++nPlayerCount ;
			}

			//结束判断
			if(nPlayerCount==m_bShowCardCount)
			{
				OnEventGameConclude(INVALID_CHAIR, NULL, 0) ;
			}

			return true ;
		}
	case SUB_C_USEREXIT:
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_UserExit));
			if (wDataSize != sizeof(CMD_C_UserExit))
			{
				return false;
			}

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus != US_PLAYING)
			{
				return true;
			}
			
			CMD_C_UserExit *pUserExit=(CMD_C_UserExit*)pDataBuffer;
			WORD wUserID=pUserExit->wUserID;
			WORD wChairID=pUserExit->wChairID;
			WORD wTableID=pUserExit->wTableID;
			bool bEnterExit=pUserExit->bEnterExit;
			
			return OnUserExit(wChairID,wTableID,wUserID);
		}	
	case SUB_C_AMDIN_COMMAND:
	{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus != US_PLAYING)
			{
				return true;
			}

			ASSERT(wDataSize == sizeof(CMD_C_ChangeCard));
			if (wDataSize != sizeof(CMD_C_ChangeCard))
			{
				return true;
			}

			if (!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				return true;
			}
			else
			{
				return OnChangeCard(pIServerUserItem->GetChairID(), pDataBuffer, wDataSize);
			}

			return true;
		}
	}

	return false;
}

//框架消息处理
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//数据事件
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
//积分事件
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return true;
}

//设置分段
bool CTableFrameSink::OnSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) 
{
	//数据验证
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize);
	if (sizeof(CMD_C_ShowCard) != wDataSize) return false;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*)pDataBuffer;

	//发送数据
	CMD_S_ShowCard SetSegment;
	memset(&SetSegment , 0 , sizeof(SetSegment));

//	SetSegment.wCurrentUser = pSetSeg->wCurrentUser;
	CopyMemory(SetSegment.bFrontCard, pSetSeg->bFrontCard, sizeof(SetSegment.bFrontCard));
	CopyMemory(SetSegment.bMidCard, pSetSeg->bMidCard, sizeof(SetSegment.bMidCard));
	CopyMemory(SetSegment.bBackCard, pSetSeg->bBackCard, sizeof(SetSegment.bBackCard));
//	SetSegment.bCanSeeShowCard = pSetSeg->bCanSeeShowCard;

	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SETSEGMENT,&SetSegment,sizeof(SetSegment)); 
	return true ;
}
//退出事件
bool CTableFrameSink::OnUserExit(WORD wChairID,WORD wTableID,WORD wUserID)
{
	IServerUserItem * pIServerUserItem=NULL;
	pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (pIServerUserItem!=NULL)
	{
		//pIServerUserItem->SetUserStatus(US_LEAVEGAME,wTableID,wChairID);
		//printf("=====退出用户椅子为%d,ID为:%d\n",wChairID,wUserID);

	}
	
	m_pITableFrame->ConcludeGame(GS_WK_FREE);

	return true;
}
//玩家摊牌
bool CTableFrameSink::OnShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize)
{

	//数据验证 
	ASSERT(INVALID_CHAIR!=wChairID) ;
	if(INVALID_CHAIR==wChairID) return false ;

	ASSERT(sizeof(CMD_C_ShowCard)==wDataSize) ;
	if(sizeof(CMD_C_ShowCard)!=wDataSize) return false ;

	m_bShowCardChairId[wChairID] = TRUE;

	CMD_C_ShowCard *pSetSeg = (CMD_C_ShowCard*) pDataBuffer ;

	// 验证客户端发过来的牌和服务端的手牌是否一致
	BYTE bTmpCard[9] = {0};
	CopyMemory(bTmpCard, pSetSeg->bFrontCard, 3);
	CopyMemory(bTmpCard + 3, pSetSeg->bMidCard, 3);
	CopyMemory(bTmpCard+6, pSetSeg->bBackCard, 3);
	m_GameLogic.SortCardList(bTmpCard, 9);

	BYTE bTmpServerCard[9] = { 0 };
	CopyMemory(bTmpServerCard, m_bUserCardData[wChairID], sizeof(bTmpServerCard));
	m_GameLogic.SortCardList(bTmpServerCard, 9);

	bool bRet = true;
	for (int i = 0; i < 9; i++)
	{
		if (bTmpCard[i] != bTmpServerCard[i])
		{
			bRet = false;
			break;
		}
	}

	if (!bRet)
	{
		return false;
	}

	// 验证客户端发过来的牌的顺序是否符合要求
	BYTE bFront[3] = { 0 };
	BYTE bMid[3] = { 0 };
	BYTE bBack[3] = { 0 };
	BYTE bTmp[3] = { 0 };
	bool bFlag = false;
	bool bFlag2 = false;

	CopyMemory(bFront, pSetSeg->bFrontCard, 3);
	CopyMemory(bMid, pSetSeg->bMidCard, 3);
	CopyMemory(bBack, pSetSeg->bBackCard, 3);

	bool bRetEx = m_GameLogic.CompareCard(bFront, bMid, 3);
	if (bRetEx)
	{
		bFlag2 = true;
		CopyMemory(bTmp, bFront, 3);
		CopyMemory(bFront, bMid, 3);
		CopyMemory(bMid, bTmp, 3);

	}

	bRetEx = m_GameLogic.CompareCard(bMid, bBack, 3);
	if (bRetEx)
	{
		bFlag2 = true;
		bFlag = true;
		CopyMemory(bTmp, bMid, 3);
		CopyMemory(bMid, bBack, 3);
		CopyMemory(bBack, bTmp, 3);
	}

	if (bFlag)
	{
		bRetEx = m_GameLogic.CompareCard(bFront, bMid, 3);
		if (bRetEx)
		{
			bFlag2 = true;
			CopyMemory(bTmp, bFront, 3);
			CopyMemory(bFront, bMid, 3);
			CopyMemory(bMid, bTmp, 3);
		}
	}


	if (bFlag2)
	{
		CopyMemory(m_bSegmentCard[wChairID][0], bFront, 3);
		CopyMemory(m_bSegmentCard[wChairID][1], bMid, 3);
		CopyMemory(m_bSegmentCard[wChairID][2], bBack, 3);
	}
	else
	{
		CopyMemory(m_bSegmentCard[wChairID][0], pSetSeg->bFrontCard, 3);
		CopyMemory(m_bSegmentCard[wChairID][1], pSetSeg->bMidCard, 3);
		CopyMemory(m_bSegmentCard[wChairID][2], pSetSeg->bBackCard, 3);

	}

	//设置数据
	m_bFinishSegment[wChairID] = true ;
	//发送数据
	CMD_S_ShowCard  ShowCard ;
	memset(&ShowCard , 0 , sizeof(ShowCard)) ;

	//计算人数
	WORD wPlayerCount=0 ; 
	for(WORD wID=0 ; wID<GAME_PLAYER ; ++wID)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wID);
		if(pIServerUserItem && !m_bOverTime[wID] && m_bPlayer[wID]) ++wPlayerCount ;
	}

	m_bShowCardCount++ ;

	//ShowCard.bCanSeeShowCard = m_bShowCardCount == wPlayerCount ? true : false;
	ShowCard.wCurrentUser =wChairID;
	CopyMemory(ShowCard.bFrontCard , m_bSegmentCard[wChairID][0] , 3);
	CopyMemory(ShowCard.bMidCard ,   m_bSegmentCard[wChairID][1] , 3);
	CopyMemory(ShowCard.bBackCard ,  m_bSegmentCard[wChairID][2] , 3);
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if(pIServerUserItem)
		{
			m_pITableFrame->SendTableData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
			m_pITableFrame->SendLookonData(i,SUB_S_SHOW_CARD,&ShowCard,sizeof(ShowCard));
		}
	}

	int nPlayerCount=0 ; 
	for(WORD i=0 ; i<GAME_PLAYER ; ++i)
	{
		IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem && m_bPlayer[i])
		{
			nPlayerCount++;
		}	
	}

	if(nPlayerCount==m_bShowCardCount)
	{
		OnEventGameConclude(INVALID_CHAIR, NULL, 0) ;
	}

	return true ;
}

//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

bool  CTableFrameSink::OnActionUserStandUp( WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser )
{
	CMD_S_UserExit UserExit;
	ZeroMemory(&UserExit,sizeof(UserExit));
	UserExit.nLeftChairID=wChairID;
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USEREXIT,&UserExit,sizeof(UserExit));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USEREXIT,&UserExit,sizeof(UserExit)); 
	return true;
}

bool CTableFrameSink::BinarySearch(const DWORD array[], int wCount, DWORD wUserID) 
{
	int begin,center,end;
	begin=0;  //首部 
	end=wCount-1; //尾部 
	while(begin<=end && begin>=0)
	{
		center=(begin+end+1)/2;

		if(array[center]==wUserID)    
		{
			return true;				 
		}
		else
			if(array[center]<wUserID)
				begin=center+1;   //先前找 
			else
				end=center-1;    //先后找 
	}
	return false;
}

void CTableFrameSink::ComputeChout()
{
	ZeroMemory(m_nResult, sizeof(m_nResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));//计算道数
	ZeroMemory(m_nAllResult, sizeof(m_nAllResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));
	ZeroMemory(m_lGameScoreEx, sizeof(m_lGameScoreEx));
	ZeroMemory(m_nSpecialType, sizeof(m_nSpecialType));
	ZeroMemory(m_bTongGuan, sizeof(m_bTongGuan));
	ZeroMemory(m_nSpecialCount, sizeof(m_nSpecialCount));
	ZeroMemory(m_nSpecialTongGuan, sizeof(m_nSpecialTongGuan));
	for (int i = 0; i< _countof(m_nIndexResult); i++)
	{
		m_nIndexResult[i][0] = 1;
		m_nIndexResult[i][1] = 1;
		m_nIndexResult[i][2] = 1;
	}

	// 每墩
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			m_nIndexResult[wUser][0] = -1;
			m_nIndexResult[wUser][1] = -1;
			m_nIndexResult[wUser][2] = -1;
			continue;
		}

		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				m_nIndexResult[wUserEx][0] = -1;
				m_nIndexResult[wUserEx][1] = -1;
				m_nIndexResult[wUserEx][2] = -1;
				continue;;
			}

			BYTE bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][0], m_bSegmentCard[wUserEx][0], sizeof(m_bSegmentCard[wUser][0]));
			if (bRet)
			{
				m_nIndexResult[wUser][0]++;

				m_nResult[wUser][0] += 1;
				m_nResult[wUserEx][0] -= 1;

// 				m_bTongGuan[wUser][0] = TRUE;
// 				m_bTongGuan[wUserEx][0] = FALSE;
			}
			else
			{
				m_nIndexResult[wUserEx][0]++;

				m_nResult[wUser][0] -= 1;
				m_nResult[wUserEx][0] += 1;

// 				m_bTongGuan[wUser][0] = FALSE;
// 				m_bTongGuan[wUserEx][0] = TRUE;
			}
			
			bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][1], m_bSegmentCard[wUserEx][1], sizeof(m_bSegmentCard[wUser][1]));
			if (bRet)
			{
				m_nIndexResult[wUser][1]++;

				m_nResult[wUser][1] += 1;
				m_nResult[wUserEx][1] -= 1;

// 				m_bTongGuan[wUser][1] = TRUE;
// 				m_bTongGuan[wUserEx][1] = FALSE;
			}
			else
			{
				m_nIndexResult[wUserEx][1]++;

				m_nResult[wUser][1] -= 1;
				m_nResult[wUserEx][1] += 1;

// 				m_bTongGuan[wUser][1] = FALSE;
// 				m_bTongGuan[wUserEx][1] = TRUE;
			}

			bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][2], m_bSegmentCard[wUserEx][2], sizeof(m_bSegmentCard[wUser][2]));
			if (bRet)
			{
				m_nIndexResult[wUser][2]++;

				m_nResult[wUser][2] += 1;
				m_nResult[wUserEx][2] -= 1;

// 				m_bTongGuan[wUser][2] = TRUE;
// 				m_bTongGuan[wUserEx][2] = FALSE;
			}
			else
			{
				m_nIndexResult[wUserEx][2]++;

				m_nResult[wUser][2] -= 1;
				m_nResult[wUserEx][2] += 1;

// 				m_bTongGuan[wUser][2] = FALSE;
// 				m_bTongGuan[wUserEx][2] = TRUE;
			}
		}
	}



	// 喜个数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 1) == CT_SPECIAL_SANQING)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
		{
			m_nSpecialCount[wUser]++;

		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
		{
			m_nSpecialCount[wUser]++;


		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
		{
			m_nSpecialCount[wUser]++;
		}
	}

	// 喜分数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				continue;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]),3,3,3,1) == CT_SPECIAL_SANQING)
			{
				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 1) == CT_SPECIAL_SANQING)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
			{
			

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
			{
			

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
			{
			

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
			{
				

				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
			{
				

				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

// 			if (m_bTongGuan[wUser][0] == TRUE
// 				&& m_bTongGuan[wUser][1] == TRUE
// 				&& m_bTongGuan[wUser][2] == TRUE)
// 			{
// 				m_nSpecialScore[wUser] += 1;
// 				m_nSpecialScore[wUserEx] -= 1;
// 				m_nSpecialType[wUser] = 1;
// 			}
// 
// 			if (m_bTongGuan[wUserEx][0] == TRUE
// 				&& m_bTongGuan[wUserEx][1] == TRUE
// 				&& m_bTongGuan[wUserEx][2] == TRUE)
// 			{
// 				m_nSpecialScore[wUser] -= 1;
// 				m_nSpecialScore[wUserEx] += 1;
// 				m_nSpecialType[wUserEx] = 1;
// 			}
		}
	}

	// 是否通关
	int nIndexFirst = 0;
	int nIndexSecond = 0;
	int nIndexThird = 0;

	for (WORD wUser = 1; wUser < GAME_PLAYER; wUser++)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}

		nIndexFirst = m_nResult[wUser][0] > m_nResult[nIndexFirst][0] ? wUser : nIndexFirst;
		nIndexSecond = m_nResult[wUser][1] > m_nResult[nIndexSecond][1] ? wUser : nIndexSecond;
		nIndexThird = m_nResult[wUser][2] > m_nResult[nIndexThird][2] ? wUser : nIndexThird;
	}

	m_bTongGuan[nIndexFirst][0] = TRUE;
	m_bTongGuan[nIndexSecond][1] = TRUE;
	m_bTongGuan[nIndexThird][2] = TRUE;

	for (int i = 0; i < _countof(m_bTongGuan); i++)
	{
		//玩家判断;
		if (!m_bPlayer[i] || m_bUserLeft[i])
		{
			continue;
		}

		if (m_bTongGuan[i][0] == TRUE
			&& m_bTongGuan[i][1] == TRUE
			&& m_bTongGuan[i][2] == TRUE)
		{
			m_nSpecialType[i] = 1;
		}
	}

	// 通关分数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				continue;
			}

			if (m_nSpecialType[wUser] == 1)
			{
				m_nSpecialTongGuan[wUser]++;
				m_nSpecialTongGuan[wUserEx]--;
			}
			if (m_nSpecialType[wUserEx] == 1)
			{
				m_nSpecialTongGuan[wUserEx]++;
				m_nSpecialTongGuan[wUser]--;
			}
		}
	}


	// 每一墩牌型
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (int i = 0; i < 3; i++)
		{
			int nType = 0;
			nType = m_GameLogic.GetCardType(m_bSegmentCard[wUser][i], 3);
			switch (nType)
			{
			case 1:
				m_nEveryType[wUser][i] = 1;
				break;
			case 2:
				m_nEveryType[wUser][i] = 2;
				break;
			case 3:
				m_nEveryType[wUser][i] = 3;
				break;
			case 4:
				m_nEveryType[wUser][i] = 4;
				break;
			case 5:
				m_nEveryType[wUser][i] = 5;
				break;
			case 6:
				m_nEveryType[wUser][i] = 6;
				break;
			default:
				break;
			}
		}
	}


// 	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
// 	{
// 		//玩家判断;
// 		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
// 		{
// 			continue;
// 		}
// 
// 		BYTE bSpecial[9] = { 0 };
// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_TONGHUA)
// 		{
// 			m_nSpecialType[wUser] = 1;
// 
// 		}
// 
// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_SHANTIAO)
// 		{
// 			m_nSpecialType[wUser] = 2;
// 		}
// 		
// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_TONGSE)
// 		{
// 			m_nSpecialType[wUser] = 3;
// 		}	
// 	}


// 	for (int i = 0; i < GAME_PLAYER; i++)
// 	{
// 		if (!m_bPlayer[i] || m_bUserLeft[i])
// 		{
// 			continue;
// 		}
// 
// 		m_nAllResult[i] = m_nResult[i][0] + m_nResult[i][1] + m_nResult[i][2] + m_nSpecialScore[i] + m_nSpecialTongGuan[i];
// 	}
// 
// 
// 	for (int i = 0; i < GAME_PLAYER; i++)
// 	{
// 		if (!m_bPlayer[i] || m_bUserLeft[i])
// 		{
// 			continue;
// 		}
// 
// 		m_lGameScoreEx[i] = m_nAllResult[i] * m_lCellScore;
// 	}

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i] || m_bUserLeft[i])
		{
			continue;
		}

		m_nResult[i][0] = m_nResult[i][0] * m_lCellScore;
		m_nResult[i][1] = m_nResult[i][1] * m_lCellScore;
		m_nResult[i][2] = m_nResult[i][2] * m_lCellScore;
		m_nSpecialScore[i] = m_nSpecialScore[i] * m_lCellScore;
		m_nSpecialTongGuan[i] = m_nSpecialTongGuan[i] * m_lCellScore;

		m_nAllResult[i] = m_nResult[i][0] + m_nResult[i][1] + m_nResult[i][2] + m_nSpecialScore[i] + m_nSpecialTongGuan[i];
	}


	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i] || m_bUserLeft[i])
		{
			continue;
		}

		m_lGameScoreEx[i] = m_nAllResult[i];
	}


}
void CTableFrameSink::ComputeChoutEx()
{
	ZeroMemory(m_nResult, sizeof(m_nResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));//计算道数
	ZeroMemory(m_nAllResult, sizeof(m_nAllResult));
	ZeroMemory(m_nSpecialScore, sizeof(m_nSpecialScore));
	ZeroMemory(m_lGameScoreEx, sizeof(m_lGameScoreEx));
	ZeroMemory(m_nSpecialType, sizeof(m_nSpecialType));
	ZeroMemory(m_bTongGuan, sizeof(m_bTongGuan));
	ZeroMemory(m_nSpecialCount, sizeof(m_nSpecialCount));
	ZeroMemory(m_nSpecialTongGuan, sizeof(m_nSpecialTongGuan));
	m_nGamePlays = 0;
	ZeroMemory(m_rgArea1, sizeof(m_rgArea1));
	ZeroMemory(m_rgArea2, sizeof(m_rgArea2));
	ZeroMemory(m_rgArea3, sizeof(m_rgArea3));


	for (int i = 0; i < _countof(m_nIndexResult); i++)
	{
		m_nIndexResult[i][0] = 1;
		m_nIndexResult[i][1] = 1;
		m_nIndexResult[i][2] = 1;
	}

	// 人数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		else
		{
			m_nGamePlays++;
		}
	}


	// 每墩
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			m_nIndexResult[wUser][0] = -1;
			m_nIndexResult[wUser][1] = -1;
			m_nIndexResult[wUser][2] = -1;
			continue;
		}
	
		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				m_nIndexResult[wUserEx][0] = -1;
				m_nIndexResult[wUserEx][1] = -1;
				m_nIndexResult[wUserEx][2] = -1;
				continue;;
			}

			BYTE bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][0], m_bSegmentCard[wUserEx][0], sizeof(m_bSegmentCard[wUser][0]));
			if (bRet)
			{
				m_nIndexResult[wUser][0]++;
			}
			else
			{
				m_nIndexResult[wUserEx][0]++;
			}

			bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][1], m_bSegmentCard[wUserEx][1], sizeof(m_bSegmentCard[wUser][1]));
			if (bRet)
			{
				m_nIndexResult[wUser][1]++;
			}
			else
			{
				m_nIndexResult[wUserEx][1]++;	
			}

			bRet = m_GameLogic.CompareCard(m_bSegmentCard[wUser][2], m_bSegmentCard[wUserEx][2], sizeof(m_bSegmentCard[wUser][2]));
			if (bRet)
			{
				m_nIndexResult[wUser][2]++;
			}
			else
			{
				m_nIndexResult[wUserEx][2]++;
			}
		}
	}

	CalculateEveryScore();


	// 喜个数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 1) == CT_SPECIAL_SANQING)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
		{
			m_nSpecialCount[wUser]++;

		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
		{
			m_nSpecialCount[wUser]++;


		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
		{
			m_nSpecialCount[wUser]++;
		}

		if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
		{
			m_nSpecialCount[wUser]++;
		}
	}


	// 喜分数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				continue;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 1) == CT_SPECIAL_SANQING)
			{
				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 1) == CT_SPECIAL_SANQING)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 2) == CT_SPECIAL_QUANHEI)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 3) == CT_SPECIAL_QUANHONG)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 4) == CT_SPECIAL_SHUANGSHUNQING)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 5) == CT_SPECIAL_SHUANGSANTIAO)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 6) == CT_SPECIAL_QUANSANTIAO)
			{
				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
			{
				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 7) == CT_SPECIAL_SIGETOU)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 8) == CT_SPECIAL_SIGETOU2)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUser][0], m_bSegmentCard[wUser][1], (m_bSegmentCard[wUser][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
			{


				m_nSpecialScore[wUser] += 1;
				m_nSpecialScore[wUserEx] -= 1;
			}

			if (m_GameLogic.GetSpecialCardType(m_bSegmentCard[wUserEx][0], m_bSegmentCard[wUserEx][1], (m_bSegmentCard[wUserEx][2]), 3, 3, 3, 9) == CT_SPECIAL_YITIAOLONG)
			{


				m_nSpecialScore[wUser] -= 1;
				m_nSpecialScore[wUserEx] += 1;
			}
		}
	}

	// 是否通关
	if (m_rgArea1[0].nIndex == m_rgArea2[0].nIndex
		&& m_rgArea2[0].nIndex == m_rgArea3[0].nIndex)
	{
		m_nSpecialType[m_rgArea1[0].nIndex] = 1;
		m_nSpecialCount[m_rgArea1[0].nIndex]++;
	}
	
	// 通关分数
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (WORD wUserEx = wUser + 1; wUserEx < GAME_PLAYER; wUserEx++)
		{
			if (!m_bPlayer[wUserEx] || m_bUserLeft[wUserEx])
			{
				continue;
			}

			if (m_nSpecialType[wUser] == 1)
			{
				m_nSpecialTongGuan[wUser]++;
				m_nSpecialTongGuan[wUserEx]--;
			}
			if (m_nSpecialType[wUserEx] == 1)
			{
				m_nSpecialTongGuan[wUserEx]++;
				m_nSpecialTongGuan[wUser]--;
			}
		}
	}


	// 每一墩牌型
	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	{
		//玩家判断;
		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
		{
			continue;
		}
		for (int i = 0; i < 3; i++)
		{
			int nType = 0;
			nType = m_GameLogic.GetCardType(m_bSegmentCard[wUser][i], 3);
			switch (nType)
			{
			case 1:
				m_nEveryType[wUser][i] = 1;
				break;
			case 2:
				m_nEveryType[wUser][i] = 2;
				break;
			case 3:
				m_nEveryType[wUser][i] = 3;
				break;
			case 4:
				m_nEveryType[wUser][i] = 4;
				break;
			case 5:
				m_nEveryType[wUser][i] = 5;
				break;
			case 6:
				m_nEveryType[wUser][i] = 6;
				break;
			default:
				break;
			}
		}
	}


	// 	for (WORD wUser = 0; wUser < GAME_PLAYER; ++wUser)
	// 	{
	// 		//玩家判断;
	// 		if (!m_bPlayer[wUser] || m_bUserLeft[wUser])
	// 		{
	// 			continue;
	// 		}
	// 
	// 		BYTE bSpecial[9] = { 0 };
	// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_TONGHUA)
	// 		{
	// 			m_nSpecialType[wUser] = 1;
	// 
	// 		}
	// 
	// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_SHANTIAO)
	// 		{
	// 			m_nSpecialType[wUser] = 2;
	// 		}
	// 		
	// 		if (m_GameLogic.GetCardType(m_bSegmentCard[wUser][0], 9, bSpecial) == CT_ALL_TONGSE)
	// 		{
	// 			m_nSpecialType[wUser] = 3;
	// 		}	
	// 	}


	// 	for (int i = 0; i < GAME_PLAYER; i++)
	// 	{
	// 		if (!m_bPlayer[i] || m_bUserLeft[i])
	// 		{
	// 			continue;
	// 		}
	// 
	// 		m_nAllResult[i] = m_nResult[i][0] + m_nResult[i][1] + m_nResult[i][2] + m_nSpecialScore[i] + m_nSpecialTongGuan[i];
	// 	}
	// 
	// 
	// 	for (int i = 0; i < GAME_PLAYER; i++)
	// 	{
	// 		if (!m_bPlayer[i] || m_bUserLeft[i])
	// 		{
	// 			continue;
	// 		}
	// 
	// 		m_lGameScoreEx[i] = m_nAllResult[i] * m_lCellScore;
	// 	}

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i] || m_bUserLeft[i])
		{
			continue;
		}

		m_nResult[i][0] = m_nResult[i][0] * m_lCellScore;
		m_nResult[i][1] = m_nResult[i][1] * m_lCellScore;
		m_nResult[i][2] = m_nResult[i][2] * m_lCellScore;

		if (m_nGamePlays == 2)
		{
			m_nSpecialScore[i] = m_nSpecialScore[i] * m_lCellScore * 1;
			m_nSpecialTongGuan[i] = m_nSpecialTongGuan[i] * m_lCellScore * 1;
		}

		if (m_nGamePlays == 3)
		{
			m_nSpecialScore[i] = m_nSpecialScore[i] * m_lCellScore * 2;
			m_nSpecialTongGuan[i] = m_nSpecialTongGuan[i] * m_lCellScore * 2;
		}

		if (m_nGamePlays == 4)
		{
			m_nSpecialScore[i] = m_nSpecialScore[i] * m_lCellScore * 3;
			m_nSpecialTongGuan[i] = m_nSpecialTongGuan[i] * m_lCellScore * 3;
		}

		if (m_nGamePlays == 5)
		{
			m_nSpecialScore[i] = m_nSpecialScore[i] * m_lCellScore * 4;
			m_nSpecialTongGuan[i] = m_nSpecialTongGuan[i] * m_lCellScore * 4;
		}
		

		m_nAllResult[i] = m_nResult[i][0] + m_nResult[i][1] + m_nResult[i][2] + m_nSpecialScore[i] + m_nSpecialTongGuan[i];
	}


	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (!m_bPlayer[i] || m_bUserLeft[i])
		{
			continue;
		}

		m_lGameScoreEx[i] = m_nAllResult[i];
	}


}

void CTableFrameSink::ComputeResult()
{
	CopyMemory(m_bSegmentCard, m_nResult, sizeof(m_bSegmentCard));
	CopyMemory(m_bSpecialCompareResult, m_nSpecialScore, sizeof(m_bSpecialCompareResult));

}

void CTableFrameSink::ShowCard(BYTE ShowPos)
{
	if(!m_bPlayer[ShowPos]) return;

	BYTE j=ShowPos;
	BYTE m_bSpecialCardData[HAND_CARD_COUNT];//特殊牌的数据

	if(m_GameLogic.GetCardType(m_bUserCardData[j],HAND_CARD_COUNT,m_GameLogic.btCardSpecialData)!=CT_INVALID)
	{
		m_GameLogic.SortCardList(m_bUserCardData[j],HAND_CARD_COUNT);

		BYTE m_bHandCardCount=HAND_CARD_COUNT;
		BYTE btSpecialCard[9] = { 0 };
		tagAnalyseData AnalyseData ;
		m_GameLogic.AnalyseCard(m_bUserCardData[j] , m_bHandCardCount , AnalyseData) ;

		for(int i=0;i<3;i++)
		{
			m_bSegmentCard[j][0][i]=m_bSpecialCardData[6+i];
		}

		for(int i=0;i<3;i++)
		{
			m_bSegmentCard[j][1][i]=m_bSpecialCardData[3+i];
		}

		for(int i=0;i<3;i++)
		{
			m_bSegmentCard[j][2][i]=m_bSpecialCardData[i];
		}
		m_bSpecialTypeTable[j]=true;
		m_GameLogic.SortCardList(m_bSegmentCard[j][0],3);
		m_GameLogic.SortCardList(m_bSegmentCard[j][1],3);
		m_GameLogic.SortCardList(m_bSegmentCard[j][2],3);
	}
	else
	{
		m_GameLogic.SortCardList(m_bUserCardData[j],HAND_CARD_COUNT);
		/*for(int i=0;i<13;i++)
		{
			printf("m_bUserCardData[j][i]=%d ",m_bUserCardData[j][i]);
		}*/

		BYTE m_bHandCardCount=HAND_CARD_COUNT;
		tagAnalyseType tagCardType;                            //类型及组合
		ZeroMemory(&tagCardType,sizeof(tagCardType));
		tagCardType=m_GameLogic.GetType(m_bUserCardData[j],m_bHandCardCount);
		//m_GameLogic.TheBestCard(tagCardType,m_bUserCardData[j],m_bHandCardCount,m_bSegmentCard[j][0],m_bSegmentCard[j][1],m_bSegmentCard[j][2]);

		//乌龙判断
		bool bDragon = false ;
// 		if((true==m_GameLogic.CompareCard(m_bSegmentCard[j][0] , m_bSegmentCard[j][1], 3 , 5 , false)) &&
// 			(true==m_GameLogic.CompareCard(m_bSegmentCard[j][1], m_bSegmentCard[j][2] , 5 , 5 , false)))
// 			bDragon = false ;
// 		else
// 			bDragon = true ;
		if(bDragon)
		{
			//恢复扑克
			BYTE bCardCount=HAND_CARD_COUNT;

			const BYTE SEARCHE_COUNT = 10 ;
			ASSERT(bCardCount==9) ;
			if(bCardCount!=9) return ;
			BYTE bAllSegmentCardIndex[SEARCHE_COUNT][3][3] ;			//分段扑克

			BYTE bSegmentScore[SEARCHE_COUNT] ;							//分段权重
			BYTE bCardDataIndex[13] = {0,1,2,3,4,5,6,7,8} ;	//扑克下标
			BYTE bFrontCardType ,										//前墩类型
				bMidCardType,											//中墩类型
				bBackCardType;											//后墩类型
			BYTE bCardData[3] ;
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
				bCardIndex = rand()%(9-bFrontCard) ;
				bAllSegmentCardIndex[bSegCount][0][bFrontCard] = bCardDataIndex[bCardIndex] ;
				bCardData[bFrontCard] = m_bUserCardData[j][bCardDataIndex[bCardIndex]] ;
				bCardDataIndex[bCardIndex] = bCardDataIndex[9-bFrontCard-1] ;
			}

			m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;
			bFrontCardType = m_GameLogic.GetCardType(bCardData , 3,m_GameLogic.btCardSpecialData) ;

			//中墩扑克
			for(BYTE bMidCard=0 ; bMidCard<3 ; ++bMidCard)
			{
				bCardIndex = rand()%(6-bMidCard) ;
				bAllSegmentCardIndex[bSegCount][1][bMidCard] = bCardDataIndex[bCardIndex] ;
				bCardData[bMidCard] = m_bUserCardData[j][bCardDataIndex[bCardIndex]] ;
				bCardDataIndex[bCardIndex] = bCardDataIndex[6-bMidCard-1] ;
			}

			m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;
			bMidCardType = m_GameLogic.GetCardType(bCardData , 3,m_GameLogic.btCardSpecialData) ;

			//后墩扑克
			for(BYTE bBackCard=0 ; bBackCard<3 ; ++bBackCard)
			{
				bAllSegmentCardIndex[bSegCount][2][bBackCard] = bCardDataIndex[bBackCard] ;
				bCardData[bBackCard] = m_bUserCardData[j][bCardDataIndex[bBackCard]] ;
			}
			m_GameLogic.SortCardList(bCardData , 3 , enDescend) ;


			bBackCardType = m_GameLogic.GetCardType(bCardData , 3,m_GameLogic.btCardSpecialData) ;


			if(bBackCardType>bMidCardType && bMidCardType>bFrontCardType) 
			{
				bSegmentScore[bSegCount] = bFrontCardType+bMidCardType+bBackCardType ;
				bSegCount++;				
			}

			//恢复数据
			for(BYTE i=0 ; i<9 ; ++i)
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
					for(BYTE j=0 ; j<3 ; ++j)
						bAllSegmentCardIndex[0][i][j]=bIndex++ ;
				bStop = true ;
				break ;
			}
			}

			//最大权重
			BYTE bMaxScore=bSegmentScore[0] ;
			BYTE bIndex=0 ;
			for(BYTE i=0 ; i<bSegCount ; ++i)
			{
				if(bMaxScore<bSegmentScore[i])
			{
				bIndex=i ;
				bMaxScore=bSegmentScore[i] ;
			}
			//设置前墩
			for(int i=0;i<3;i++)
			{
				m_bSegmentCard[j][0][i]=m_bUserCardData[j][bAllSegmentCardIndex[bIndex][0][i]];
			}
			//设置中敦，后敦
			for(int i=0;i<3;i++)
			{
				m_bSegmentCard[j][1][i]=m_bUserCardData[j][bAllSegmentCardIndex[bIndex][1][i]];
				m_bSegmentCard[j][2][i]=m_bUserCardData[j][bAllSegmentCardIndex[bIndex][2][i]];
			}
			}
		}
		m_bSpecialTypeTable[j]=false;
		m_GameLogic.SortCardList(m_bSegmentCard[j][0],3);
		m_GameLogic.SortCardList(m_bSegmentCard[j][1],3);
		m_GameLogic.SortCardList(m_bSegmentCard[j][2],3);
	}
}
void CTableFrameSink::LoadGameINI()
{
	//是否作弊换牌
	m_bIsCheat = GetPrivateProfileInt(m_szRoomName,TEXT("Ischeat"),0,m_szConfigFileName);
	
	//有概率换牌的起始输分分数
	m_lRealBankerRobotLoseMax = GetPrivateProfileInt(m_szRoomName,TEXT("RealBankerRobotLoseMax"),10000000,m_szConfigFileName);
	//百分百换牌，机器人输的起始分数
	m_lRealBankerRobotLoseMaxChange = GetPrivateProfileInt(m_szRoomName,TEXT("RealBankerRobotLoseMaxChange"),30000000,m_szConfigFileName);
	//换牌百分比
	m_btRealBankerRobotLoseChangePercent = GetPrivateProfileInt(m_szRoomName,TEXT("RealBankerRobotLoseChangePercent"),30,m_szConfigFileName);

	//赢的金额数，机器人有概率放水
	m_lRobotProbabilityTanking = GetPrivateProfileInt(m_szRoomName,TEXT("RobotProbabilityTanking "),80000000,m_szConfigFileName);
	//赢的金额数，机器人百分百放水
	m_lRobotDefineTanking = GetPrivateProfileInt(m_szRoomName,TEXT("RobotDefineTanking "),100000000,m_szConfigFileName);
	//机器人放水的百分比
	m_btRobotTankingPercent = GetPrivateProfileInt(m_szRoomName,TEXT("RobotTankingPercent "),20,m_szConfigFileName);
}

void CTableFrameSink::WriteGameINI()
{
	//写入配置
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szFileName[MAX_PATH];

	//机器人累计的输赢分数
	m_lRobotResultScore = GetPrivateProfileInt(m_szRoomName,TEXT("RobotResultScore"),0,m_szConfigFileName);
	//扣除的机器人赢的分数总和
	m_lRobotWinChiefScore = GetPrivateProfileInt(m_szRoomName,TEXT("RobotWinChiefScore"),0,m_szConfigFileName);
	//机器人累计的输赢分数
	if (GetCurrentDirectory(sizeof(szPath), szPath))
	{
		TCHAR szRobotResultScore[MAX_PATH]=TEXT("");
		//玩家控制
		if(m_IsCheatUser && m_lUserGold!=0)
		{
			_sntprintf(szRobotResultScore,sizeof(szRobotResultScore),TEXT("%d"),m_lAllUserGold);
			WritePrivateProfileString(TEXT("1"),TEXT("UserGold"),szRobotResultScore,m_szConfigFilePath);
		}
		if(((m_bLoseWin && m_lAllUserGold>m_lWinMaxGold) || (!m_bLoseWin && m_lAllUserGold<m_lLoseMaxGold)) && m_IsCheatUser)
			WritePrivateProfileString(TEXT("1"),TEXT("IsCheatUser"),TEXT("0"),m_szConfigFilePath);

		//机器人控制
		if(m_WinnerScore != 0)
		{
			m_lRobotResultScore+=m_WinnerScore;
			if(m_lRobotResultScore>=m_lRobotDefineTanking)
			{
				long lMoney = m_lRobotResultScore*( m_btRobotWinScorePercent*0.01);
				m_lRobotWinChiefScore += lMoney;
				m_lRobotResultScore -= lMoney;
				//扣除的机器人赢的分数总和
				_sntprintf(szRobotResultScore,sizeof(szRobotResultScore),TEXT("%d"),m_lRobotWinChiefScore);
				WritePrivateProfileString(m_szRoomName,TEXT("RobotWinChiefScore"),szRobotResultScore,m_szConfigFileName);
			}
			//机器人累计的输赢分数
			_sntprintf(szRobotResultScore,sizeof(szRobotResultScore),TEXT("%d"),m_lRobotResultScore);
			WritePrivateProfileString(m_szRoomName,TEXT("RobotResultScore"),szRobotResultScore,m_szConfigFileName);
		}

		static bool bControl = true;
		TCHAR szBunko[MAX_PATH]=TEXT("");
		CString str;
		CTime tm;
		tm = CTime::GetCurrentTime();
		if(tm.GetHour() == 11)
		{
			bControl = true;
		}
		if(tm.GetHour() == 12)
		{
			if(bControl)
			{
				bControl = false;
				_sntprintf(szFileName,sizeof(szFileName),TEXT("%s\\GameLog\\Thirteen.log"),szPath);
				str = tm.Format("%Y-%m-%d %X");
				str+= "\t胜负";
				_sntprintf(szBunko,sizeof(szBunko),TEXT("%d"),m_lRobotResultScore+m_lRobotWinChiefScore);
				WritePrivateProfileString(m_szRoomName,str,szBunko,szFileName);
				//bControl = false;
				m_lRobotWinChiefScore = 0;
				m_lRobotResultScore = 0;
				//扣除的机器人赢的分数总和
				_sntprintf(szRobotResultScore,sizeof(szRobotResultScore),TEXT("%d"),m_lRobotWinChiefScore);
				WritePrivateProfileString(m_szRoomName,TEXT("RobotWinChiefScore"),szRobotResultScore,m_szConfigFileName);
				//机器人累计的输赢分数
				_sntprintf(szRobotResultScore,sizeof(szRobotResultScore),TEXT("%d"),m_lRobotResultScore);
				WritePrivateProfileString(m_szRoomName,TEXT("RobotResultScore"),szRobotResultScore,m_szConfigFileName);
			}
		}
	}
}
void CTableFrameSink::ControlCheat()
{
	//验证玩家
	bool IsAndroid = false;
	bool IsPlayer = false;
	for(BYTE j=0; j<m_wPlayerCount; j++)
	{
		IServerUserItem *pServerBankerUser = m_pITableFrame->GetTableUserItem(j);
		if(pServerBankerUser!=NULL && m_bPlayer[j])
		{
			if(pServerBankerUser->IsAndroidUser())
				IsAndroid = true;
			else
				IsPlayer = true;
		}
	}
	if(IsAndroid && IsPlayer)
	{
		for (BYTE i=0; i<m_wPlayerCount; i++)
		{
			ShowCard(i);
		}
		ComputeChoutEx();
		//计算输赢
		//ComputeResult();

		//放水
		if((m_lRobotResultScore>=m_lRobotDefineTanking) ||
			(m_lRobotResultScore>=m_lRobotProbabilityTanking && rand()%100 < m_btRobotTankingPercent))
		{
			if(m_WinnerScore>0)
			{
				WORD nWinUser=0,nLoseUser=0,nCount=0;
				LONG lMaxScore=0;
				for (BYTE i=0; i<m_wPlayerCount; i++)
				{
					IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
					if(pServerUser!=NULL && m_bPlayer[i])
					{
						if(pServerUser->IsAndroidUser() && m_lGameScore[i]>0)
						{
							if(nCount==0)
							{
								lMaxScore=m_lGameScore[i];
								nWinUser=i;
							}
							else
							{
								if(lMaxScore<m_lGameScore[i])
								{
									lMaxScore=m_lGameScore[i];
									nWinUser=i;
								}
							}
							nCount++;
						}
						if(!pServerUser->IsAndroidUser() && m_lGameScore[i]<0)
						{
							nLoseUser=i;
						}
					}
				}
				/*for (BYTE i=0; i<13; i++)
				{
					printf("\n 赢 m_bUserCardData[%d][%d] = %d",nWinUser,i,m_bUserCardData[nWinUser][i]);
				}
				for (BYTE i=0; i<13; i++)
				{
					printf("\n 输 m_bUserCardData[%d][%d] = %d",nLoseUser,i,m_bUserCardData[nLoseUser][i]);
				}*/
				//交换手牌
				BYTE bTempCardData[13] = {};
				memcpy(bTempCardData,	m_bUserCardData[nWinUser],	sizeof(bTempCardData));
				memcpy(m_bUserCardData[nWinUser],	m_bUserCardData[nLoseUser],	sizeof(bTempCardData));
				memcpy(m_bUserCardData[nLoseUser],	bTempCardData,	sizeof(bTempCardData));
				printf("\n 赢：%d，输：%d",nWinUser,nLoseUser);
				printf("\n 机器人放水成功！！");
			}
		}//赢
		else if((m_lRobotResultScore<=m_lRobotLoseMaxChange) || (m_lRobotResultScore<=m_lRobotloseMax && rand()%100 < m_btRobotLoseChangePercent) ||
			(rand()%100 < m_btUsuallyChangePercent) || (m_WinnerScore<=(-m_lRealBankerRobotLoseMaxChange)) || 
			(m_WinnerScore<=(-m_lRealBankerRobotLoseMax) && rand()%100 < m_btRealBankerRobotLoseChangePercent))
		{
			if(m_WinnerScore<0)
			{
				WORD nWinUser=0,nLoseUser=0,nCount=0;
				LONG lMaxScore=0;
				for (BYTE i=0; i<m_wPlayerCount; i++)
				{
					IServerUserItem *pServerUser = m_pITableFrame->GetTableUserItem(i);
					if(pServerUser!=NULL && m_bPlayer[i])
					{
						if(!pServerUser->IsAndroidUser() && m_lGameScore[i]>0)
						{
							if(nCount==0)
							{
								lMaxScore=m_lGameScore[i];
								nWinUser=i;
							}
							else
							{
								if(lMaxScore<m_lGameScore[i])
								{
									lMaxScore=m_lGameScore[i];
									nWinUser=i;
								}
							}
							nCount++;
						}
						if(pServerUser->IsAndroidUser() && m_lGameScore[i]<0)
						{
							nLoseUser=i;
						}
					}
				}
				//交换手牌
				BYTE bTempCardData[13] = {};
				memcpy(bTempCardData,	m_bUserCardData[nWinUser],	sizeof(bTempCardData));
				memcpy(m_bUserCardData[nWinUser],	m_bUserCardData[nLoseUser],	sizeof(bTempCardData));
				memcpy(m_bUserCardData[nLoseUser],	bTempCardData,	sizeof(bTempCardData));
				printf("\n 赢：%d，输：%d",nWinUser,nLoseUser);
				printf("\n 机器人换牌成功！！");
			}
		}
	}
}

bool CTableFrameSink::UserControl()
{
	WORD wUserChairIndex = INVALID_CHAIR;
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem != NULL)
		{
			if(pIServerUserItem->GetGameID() == m_dwUserID)
			{
				wUserChairIndex = wChairID;
				break;
			}
		}
	}
	if(wUserChairIndex != INVALID_CHAIR && m_bPlayer[wUserChairIndex])
	{
		for (BYTE i=0; i<m_wPlayerCount; i++)
		{
			ShowCard(i);
		}
		ComputeChoutEx();
		//计算输赢
		//ComputeResult();
		m_lUserGold = m_lGameScore[wUserChairIndex];
		//判断换牌
		if(m_bLoseWin)
		{
			if((rand()%100<m_dwUserLoseWinPercent && m_lUserGold<0) /*|| ((m_lUserScore+m_lAllUserGold) > (m_lWinMaxGold+m_lWinMaxGold/10))*/)
			{
				for (BYTE j=0; j<m_wPlayerCount; j++)
				{
					if(m_bPlayer[j] && m_lGameScore[j]>0)
					{
						//交换手牌
						BYTE bTempCardData[13] = {};
						memcpy(bTempCardData,	m_bUserCardData[wUserChairIndex],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[wUserChairIndex],	m_bUserCardData[j],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[j],	bTempCardData,	sizeof(bTempCardData));
					}
				}
			}
		}
		else
		{
			if((rand()%100<m_dwUserLoseWinPercent && m_lUserGold>0) /*|| ((m_lUserScore+m_lAllUserGold) < (m_lLoseMaxGold+m_lLoseMaxGold/10))*/)
			{
				for (BYTE j=0; j<m_wPlayerCount; j++)
				{
					if(m_bPlayer[j] && m_lGameScore[j]<0)
					{
						//交换手牌
						BYTE bTempCardData[13] = {};
						memcpy(bTempCardData,	m_bUserCardData[wUserChairIndex],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[wUserChairIndex],	m_bUserCardData[j],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[j],	bTempCardData,	sizeof(bTempCardData));
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool CTableFrameSink::ClientControl()
{

	if(m_bPlayer[m_dwControlID])
	{
		for (BYTE i=0; i<m_wPlayerCount; i++)
		{
			ShowCard(i);
		}
		ComputeChoutEx();
		//计算输赢
		//ComputeResult();
		m_lUserGold = m_lGameScore[m_dwControlID];
		//判断换牌
		if(m_bControlLossWin)
		{
			if( m_lUserGold<0)
			{
				for (BYTE j=0; j<m_wPlayerCount; j++)
				{
					if(m_lGameScore[j]>0)
					{
						//交换手牌
						BYTE bTempCardData[13] = {};
						memcpy(bTempCardData,	m_bUserCardData[m_dwControlID],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[m_dwControlID],	m_bUserCardData[j],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[j],	bTempCardData,	sizeof(bTempCardData));
					}
				}
			}
		}
		else
		{
			if(m_lUserGold>0)
			{
				for (BYTE j=0; j<m_wPlayerCount; j++)
				{
					if(m_lGameScore[j]<0)
					{
						//交换手牌
						BYTE bTempCardData[13] = {};
						memcpy(bTempCardData,	m_bUserCardData[m_dwControlID],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[m_dwControlID],	m_bUserCardData[j],	sizeof(bTempCardData));
						memcpy(m_bUserCardData[j],	bTempCardData,	sizeof(bTempCardData));
					}
				}
			}
		}
		m_nControlCount--;
		m_bControl = true;

		CMD_C_ControlData ControlData;
		ZeroMemory(&ControlData,sizeof(ControlData));
		ControlData.cbCount = m_nControlCount;
		ControlData.bWinloss = m_bControlLossWin;
		ControlData.dwChairID = m_dwControlID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_AMDIN_COMMAND,&ControlData,sizeof(ControlData));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_AMDIN_COMMAND,&ControlData,sizeof(ControlData));
	}
	return true;
}

bool CTableFrameSink::SetGameRule(byte *cbRule, int len)
{
	GameRule * pGameRule = (GameRule *)cbRule;
	m_lCellScore = pGameRule->sCellScore;

	return true;
}

bool CTableFrameSink::OnChangeCard(const WORD wChairID, const void * pDataBuffer, WORD wDataSize)
{
	static LONG dwRandCount = 0L;
	srand((unsigned)time(NULL) + dwRandCount++);

	CMD_C_ChangeCard * pcccChangeCard = (CMD_C_ChangeCard *)pDataBuffer;
	int nCardLength = strlen((const char*)m_cbCard);
	int nChangeCard = strlen((const char*)pcccChangeCard->bCard);

	CMD_S_ChangeCard cscCard;
	memset(&cscCard, 0, sizeof(cscCard));

	if (nChangeCard <= nCardLength)
	{
		int nIndex = 0;
		BYTE bTmp = 0;
		CopyMemory(cscCard.bCard, pcccChangeCard->bCard, sizeof(pcccChangeCard->bCard));

		for (int i = 0; i < nChangeCard; i++)
		{
			nIndex = rand() % (nCardLength - 1);

			bTmp = cscCard.bCard[i];

			cscCard.bCard[i] = m_cbCard[nIndex];
			m_cbCard[nIndex] = bTmp;

		}

		m_GameLogic.RemoveCard(pcccChangeCard->bCard, nChangeCard, m_bUserCardData[wChairID], _countof(m_bUserCardData[wChairID]));
		CopyMemory(&m_bUserCardData[wChairID][HAND_CARD_COUNT - nChangeCard], cscCard.bCard, strlen((const char *)cscCard.bCard));
	}
	else
	{
		BYTE bLastCard[52] = { 0 };
		CopyMemory(bLastCard, m_cbCard, nCardLength);

		int nIndex = 0;
		BYTE bTmp = 0;
		CopyMemory(cscCard.bCard, pcccChangeCard->bCard, sizeof(pcccChangeCard->bCard));

		for (int i = 0; i < nCardLength; i++)
		{

			nIndex = rand() % (nChangeCard - 1);

			bTmp = m_cbCard[i];

			m_cbCard[i] = cscCard.bCard[nIndex];
			cscCard.bCard[nIndex] = bTmp;
		}

		CopyMemory(m_bUserCardData[wChairID], cscCard.bCard, sizeof(cscCard.bCard));
	}

	m_pITableFrame->SendTableData(wChairID, SUB_S_AMDIN_COMMAND, &cscCard, sizeof(cscCard));

	return true;
}
void CTableFrameSink::CalculateEveryScore()
{
	// 赋值
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		m_rgArea1[i].nIndex = i;
		m_rgArea1[i].nCardIndex = m_nIndexResult[i][0];

		m_rgArea2[i].nIndex = i;
		m_rgArea2[i].nCardIndex = m_nIndexResult[i][1];

		m_rgArea3[i].nIndex = i;
		m_rgArea3[i].nCardIndex = m_nIndexResult[i][2];
	}

	// 大到小排序
	Region rg;
	ZeroMemory(&rg, sizeof(rg));
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		for (int j = i + 1; j < GAME_PLAYER; j++)
		{
			if (m_rgArea1[i].nCardIndex < m_rgArea1[j].nCardIndex)
			{
				rg.nIndex = m_rgArea1[i].nIndex;
				rg.nCardIndex = m_rgArea1[i].nCardIndex;

				m_rgArea1[i].nIndex = m_rgArea1[j].nIndex;
				m_rgArea1[i].nCardIndex = m_rgArea1[j].nCardIndex;

				m_rgArea1[j].nIndex = rg.nIndex;
				m_rgArea1[j].nCardIndex = rg.nCardIndex;
			}

			if (m_rgArea2[i].nCardIndex < m_rgArea2[j].nCardIndex)
			{
				rg.nIndex = m_rgArea2[i].nIndex;
				rg.nCardIndex = m_rgArea2[i].nCardIndex;

				m_rgArea2[i].nIndex = m_rgArea2[j].nIndex;
				m_rgArea2[i].nCardIndex = m_rgArea2[j].nCardIndex;

				m_rgArea2[j].nIndex = rg.nIndex;
				m_rgArea2[j].nCardIndex = rg.nCardIndex;
			}

			if (m_rgArea3[i].nCardIndex < m_rgArea3[j].nCardIndex)
			{
				rg.nIndex = m_rgArea3[i].nIndex;
				rg.nCardIndex = m_rgArea3[i].nCardIndex;

				m_rgArea3[i].nIndex = m_rgArea3[j].nIndex;
				m_rgArea3[i].nCardIndex = m_rgArea3[j].nCardIndex;

				m_rgArea3[j].nIndex = rg.nIndex;
				m_rgArea3[j].nCardIndex = rg.nCardIndex;
			}

		}
	}

	// 算分
	if (m_nGamePlays == 2)
	{
		m_nResult[m_rgArea1[0].nIndex][0] = 1;
		m_nResult[m_rgArea1[1].nIndex][0] = -1;

		m_nResult[m_rgArea2[0].nIndex][1] = 1;
		m_nResult[m_rgArea2[1].nIndex][1] = -1;

		m_nResult[m_rgArea3[0].nIndex][2] = 1;
		m_nResult[m_rgArea3[1].nIndex][2] = -1;
	}

	if (m_nGamePlays == 3)
	{
		m_nResult[m_rgArea1[0].nIndex][0] = 3;
		m_nResult[m_rgArea1[1].nIndex][0] = -1;
		m_nResult[m_rgArea1[2].nIndex][0] = -2;

		m_nResult[m_rgArea2[0].nIndex][1] = 3;
		m_nResult[m_rgArea2[1].nIndex][1] = -1;
		m_nResult[m_rgArea2[2].nIndex][1] = -2;

		m_nResult[m_rgArea3[0].nIndex][2] = 3;
		m_nResult[m_rgArea3[1].nIndex][2] = -1;
		m_nResult[m_rgArea3[2].nIndex][2] = -2;

	}

	if (m_nGamePlays == 4)
	{
		m_nResult[m_rgArea1[0].nIndex][0] = 6;
		m_nResult[m_rgArea1[1].nIndex][0] = -1;
		m_nResult[m_rgArea1[2].nIndex][0] = -2;
		m_nResult[m_rgArea1[3].nIndex][0] = -3;


		m_nResult[m_rgArea2[0].nIndex][1] = 6;
		m_nResult[m_rgArea2[1].nIndex][1] = -1;
		m_nResult[m_rgArea2[2].nIndex][1] = -2;
		m_nResult[m_rgArea2[3].nIndex][1] = -3;

		m_nResult[m_rgArea3[0].nIndex][2] = 6;
		m_nResult[m_rgArea3[1].nIndex][2] = -1;
		m_nResult[m_rgArea3[2].nIndex][2] = -2;
		m_nResult[m_rgArea3[3].nIndex][2] = -3;

	}

	if (m_nGamePlays == 5)
	{
		m_nResult[m_rgArea1[0].nIndex][0] = 10;
		m_nResult[m_rgArea1[1].nIndex][0] = -1;
		m_nResult[m_rgArea1[2].nIndex][0] = -2;
		m_nResult[m_rgArea1[3].nIndex][0] = -3;
		m_nResult[m_rgArea1[4].nIndex][0] = -4;

		m_nResult[m_rgArea2[0].nIndex][1] = 10;
		m_nResult[m_rgArea2[1].nIndex][1] = -1;
		m_nResult[m_rgArea2[2].nIndex][1] = -2;
		m_nResult[m_rgArea2[3].nIndex][1] = -3;
		m_nResult[m_rgArea2[4].nIndex][1] = -4;

		m_nResult[m_rgArea3[0].nIndex][2] = 10;
		m_nResult[m_rgArea3[1].nIndex][2] = -1;
		m_nResult[m_rgArea3[2].nIndex][2] = -2;
		m_nResult[m_rgArea3[3].nIndex][2] = -3;
		m_nResult[m_rgArea3[4].nIndex][2] = -4;
	}

}
//////////////////////////////////////////////////////////////////////////
