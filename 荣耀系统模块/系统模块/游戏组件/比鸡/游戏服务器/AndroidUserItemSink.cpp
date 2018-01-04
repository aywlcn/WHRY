#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////
#define IDI_READY                  100
#define IDI_SHOW_CARD              101

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//AllocConsole();
	//freopen("CON","wt",stdout);
   ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
   ZeroMemory(m_bSegmentCard,sizeof(m_bSegmentCard));
   ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
   ZeroMemory(&m_SetSegCard, sizeof(m_SetSegCard));
   m_bHandCardCount=0;
   static LONG dwRandCount=0L;
   srand((unsigned)time(NULL)+dwRandCount++);

   m_nRobotPlayMaxCount=2+rand()%3;
   m_nRobotPlayCount=0; 

}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	ZeroMemory(m_bSegmentCard,sizeof(m_bSegmentCard));
	ZeroMemory(m_bSpecialCardData,sizeof(m_bSpecialCardData));
	m_bHandCardCount=0;
	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{   
	switch(nTimerID)
	{
	case IDI_READY:
		{
			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);
            m_pIAndroidUserItem->KillGameTimer(IDI_READY);
			return true;
		}
	case IDI_SHOW_CARD:
		{
            ShowCard();
            m_pIAndroidUserItem->KillGameTimer(IDI_SHOW_CARD);
			return true;
		}
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:
	case SUB_S_MOBILE_PUTCARD:
		return true;
	case SUB_S_SEND_CARD:
	{
									return OnSubSendCard(pData, wDataSize);
	}
	case SUB_S_GAME_END:
	{
									return OnSubGameEnd(pData, wDataSize);
	}
	case SUB_S_SETSEGMENT:		//设置分段
	{
									return true;
	}
	case SUB_S_SHOW_CARD:      //玩家摊牌
	{
								   //ShowCard();
								   return true;
	}
	case SUB_S_PLAYER_LEFT:     //玩家强退
	{
									return true;
	}
	case SUB_S_SCORE_RESULT:     //游戏结算
	{
									 return true;
	}
	case SUB_S_USEREXIT:         //玩家退出
	{
									 return true;
	}
	case SUB_S_AMDIN_COMMAND:
	{
								return true;
	}
	}
	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GS_WK_FREE:			//空闲状态
		{
			//开始时间
			UINT nElapse=rand()%(3)+2;
			if(rand()%100 > 90)
				nElapse=rand()%(15)+5;
			m_pIAndroidUserItem->SetGameTimer(IDI_READY,nElapse);  
			return true;
		}
	case GS_WK_PLAYING:
		{
			return true;

		}
	}

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}


//游戏开始
bool CAndroidUserItemSink::OnSubSendCard(void * pData, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

    ZeroMemory(m_bHandCardData,sizeof(m_bHandCardData));
	//变量定义
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pData;
	CopyMemory(m_bHandCardData,pSendCard->bCardData,sizeof(m_bHandCardData));
	//if(m_GameLogic.GetCardType(m_bHandCardData,13,m_GameLogic.btCardSpecialData)!=CT_INVALID)
	//if (m_GameLogic.GetCardType(m_bHandCardData, 13) != CT_INVALID)
	{
        m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_CARD,(10));
	}
// 	else
// 	{ 
// 		int Number=rand()%3;
// 		if(Number==0)
// 		{
//             m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_CARD,(8+rand()%10));
// 		}
// 		else if(Number==1)
// 		{
//             m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_CARD,(8+rand()%20));
// 		}
// 		else if(Number==2)
// 		{
//             m_pIAndroidUserItem->SetGameTimer(IDI_SHOW_CARD,(8+rand()%25));
// 		}
// 	}
	return true;
}


//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(void * pData, WORD wDataSize)
{ 
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pData;
	
	m_nRobotPlayCount += 1;

	//开始时间
	UINT nElapse=rand()%(3)+2;
	if(rand()%100 > 90)
		nElapse=rand()%(15)+5;
	m_pIAndroidUserItem->SetGameTimer(IDI_READY,10+nElapse); //设置准备时间

	return true;
}

void CAndroidUserItemSink::GetSpecialCard( BYTE CardData[],BYTE CardCount )
{

	BYTE btSpecialCard[9];
	ZeroMemory(btSpecialCard,sizeof(btSpecialCard));
	tagAnalyseData AnalyseData ;
	m_GameLogic.AnalyseCard(CardData , CardCount , AnalyseData) ;


	if (m_GameLogic.GetCardType(CardData, CardCount, btSpecialCard) == CT_ALL_TONGHUA)
	{
		m_GameLogic.SortCardList(CardData, CardCount);
		CopyMemory(m_bSpecialCardData, CardData, _countof(m_bSpecialCardData));
		
	}

	if (m_GameLogic.GetCardType(CardData, CardCount, btSpecialCard) == CT_ALL_SHANTIAO)
	{
		m_GameLogic.SortCardList(CardData, CardCount);
		CopyMemory(m_bSpecialCardData, CardData, _countof(m_bSpecialCardData));
	}
	
}

void CAndroidUserItemSink::ShowCard()
{
	CMD_C_ShowCard AllSegmentCard;
	memset(&AllSegmentCard, 0, sizeof(AllSegmentCard));

// 	if (m_GameLogic.GetCardType(m_bHandCardData, HAND_CARD_COUNT, m_bSpecialCardData) != CT_INVALID)
// 	{
// 		m_GameLogic.SortCardList(m_bHandCardData, HAND_CARD_COUNT);
// 		GetSpecialCard(m_bHandCardData, HAND_CARD_COUNT);
// 
// 		for (int i = 0; i < 3; i++)
// 		{
// 			m_bSegmentCard[0][i] = m_bSpecialCardData[6 + i];
// 		}
// 
// 		for (int i = 0; i < 3; i++)
// 		{
// 			m_bSegmentCard[1][i] = m_bSpecialCardData[3 + i];
// 		}
// 
// 		for (int i = 0; i < 3; i++)
// 		{
// 			m_bSegmentCard[2][i] = m_bSpecialCardData[i];
// 		}
// 		m_GameLogic.SortCardList(m_bSegmentCard[0], 3);
// 		m_GameLogic.SortCardList(m_bSegmentCard[1], 3);
// 		m_GameLogic.SortCardList(m_bSegmentCard[2], 3);
// 		CopyMemory(AllSegmentCard.bFrontCard, m_bSegmentCard[0], sizeof(AllSegmentCard.bFrontCard));
// 		CopyMemory(AllSegmentCard.bMidCard, m_bSegmentCard[1], sizeof(AllSegmentCard.bMidCard));
// 		CopyMemory(AllSegmentCard.bBackCard, m_bSegmentCard[2], sizeof(AllSegmentCard.bBackCard));
// 
// 		//AllSegmentCard.bSpecialType = true;
// 		//CopyMemory(AllSegmentCard.btSpecialData, m_bSpecialCardData, sizeof(AllSegmentCard.btSpecialData));
// 		m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));
// 	}
// 	else
	{
		BYTE bTmpCard[9] = {0 };
		CopyMemory(bTmpCard, m_bHandCardData, sizeof(bTmpCard));
		m_GameLogic.SortCardList(bTmpCard, HAND_CARD_COUNT);

// 		BYTE btmp[3] = { 0x31, 0x3b, 0x3a };
// 		BYTE btmp2[3] = { 0x21, 0x2b, 0x2a };
// 		bool bRet = m_GameLogic.CompareCard(btmp, btmp2, 3);

		// 设置分段
		OnSetSegment(bTmpCard, sizeof(bTmpCard), m_SetSegCard);

		for (int i = 0; i < 3; i++)
		{
			m_bSegmentCard[0][i] = m_SetSegCard.bFrontCard[i];
		}

		for (int i = 0; i < 3; i++)
		{
			m_bSegmentCard[1][i] = m_SetSegCard.bMidCard[i];
		}

		for (int i = 0; i < 3; i++)
		{
			m_bSegmentCard[2][i] = m_SetSegCard.bBackCard[i];
		}

		CopyMemory(AllSegmentCard.bFrontCard, m_bSegmentCard[0], sizeof(AllSegmentCard.bFrontCard));
		CopyMemory(AllSegmentCard.bMidCard, m_bSegmentCard[1], sizeof(AllSegmentCard.bMidCard));
		CopyMemory(AllSegmentCard.bBackCard, m_bSegmentCard[2], sizeof(AllSegmentCard.bBackCard));

		m_GameLogic.SortCardList(AllSegmentCard.bFrontCard, 3);
		m_GameLogic.SortCardList(AllSegmentCard.bMidCard, 3);
		m_GameLogic.SortCardList(AllSegmentCard.bBackCard, 3);
		m_pIAndroidUserItem->SendSocketData(SUB_C_SHOWCARD, &AllSegmentCard, sizeof(AllSegmentCard));
	}
}

void CAndroidUserItemSink::OnSetSegment(BYTE bCardData[], BYTE CardCount, SetSegCard &sscCard)
{
	ASSERT(CardCount == 9);
	if (CardCount != 9)
	{
		return;
	}
	BYTE bCardDataTmp[9] = { 0 };
	CopyMemory(bCardDataTmp, bCardData, sizeof(bCardDataTmp));
	m_GameLogic.SortCardList(bCardDataTmp, sizeof(bCardDataTmp));

	BYTE bMaXData[3] = { 0 };
	CopyMemory(bMaXData, bCardDataTmp, sizeof(bMaXData));

	BYTE bTmp[3] = { 0 };
	BYTE bSpecialCard[3] = { 0 };
	for (int i = 0; i < _countof(bCardDataTmp); i++)
	{
		bTmp[0] = bCardDataTmp[i];
		for (int j = i + 1; j < _countof(bCardDataTmp); j++)
		{
			bTmp[1] = bCardDataTmp[j];
 			for (int x = j +1; x<_countof(bCardDataTmp); x++)
			{
				bTmp[2] = bCardDataTmp[x];

				bool bRet = m_GameLogic.CompareCard(bTmp, bMaXData, 3);
				if (bRet)
				{
					CopyMemory(bMaXData, bTmp, 3);
					continue;
				}
			}
		}
	}
	CopyMemory(sscCard.bBackCard, bMaXData, sizeof(sscCard.bBackCard));
	m_GameLogic.RemoveCard(bMaXData, sizeof(bMaXData), bCardDataTmp, sizeof(bCardDataTmp));
	memset(bMaXData, 0, sizeof(bMaXData));

	
	BYTE bCardDataTmp2[6] = { 0 };
	CopyMemory(bCardDataTmp2, bCardDataTmp, sizeof(bCardDataTmp2));
	CopyMemory(bMaXData, bCardDataTmp2, sizeof(bMaXData));
	for (int i = 0; i < _countof(bCardDataTmp2); i++)
	{
		bTmp[0] = bCardDataTmp2[i];
		for (int j = i + 1; j < _countof(bCardDataTmp2); j++)
		{
			bTmp[1] = bCardDataTmp2[j];
			for (int x = j + 1; x < _countof(bCardDataTmp2); x++)
			{
				bTmp[2] = bCardDataTmp2[x];

				bool bRet = m_GameLogic.CompareCard(bTmp, bMaXData, 3);
				if (bRet)
				{
					CopyMemory(bMaXData, bTmp, 3);
					continue;
				}
			}
		}
	}

	CopyMemory(sscCard.bMidCard, bMaXData, sizeof(sscCard.bMidCard));
	m_GameLogic.RemoveCard(bMaXData, sizeof(bMaXData), bCardDataTmp2, sizeof(bCardDataTmp2));
	memset(bMaXData, 0, sizeof(bMaXData));

	CopyMemory(sscCard.bFrontCard, bCardDataTmp2, sizeof(sscCard.bFrontCard));
}