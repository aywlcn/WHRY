#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE
/////
#pragma once

#include "Stdafx.h"
#include "DlgCustomRule.h"
#include "..\游戏客户端\GameLogic.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_ROBOTCOUNT		1000

//////////////////////////////////////////////////////////////////////////
#define IDI_WRITE_SCORE       10			//比牌消息ID
#define IDI_GAME_END		  11			//结束时间

#define IDT_WRITE_SCORE1      16           //比牌时间
#define IDT_WRITE_SCORE2      5            //全部特殊牌或者倒水时间

struct Region
{
	int nIndex;
	int nCardIndex;
};
//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//组件变量
protected:
	CGameLogic							m_GameLogic;										//游戏逻辑
	ITableFrame							* m_pITableFrame;									//框架接口
	const tagGameServiceOption			* m_pGameServiceOption;								//配置参数
	tagCustomRule *						m_pGameCustomRule;									//自定规则
	

	//属性变量
protected:
	static const WORD					m_wPlayerCount;										//游戏人数
	static const BYTE					m_GameStartMode;									//开始模式
	BYTE                                m_nAndroidCount;                                    //本张桌子机器人的数量

	int									m_nLeftAndroidchairID;								//离开机器人ID    
	BOOL								m_bNomalEnd;										//是否正常结束

	//游戏变量
protected:
	CMD_S_GameEnd						m_sGameEnd;											//游戏结束数据
	BYTE								m_bUserCardData[GAME_PLAYER][HAND_CARD_COUNT];		//玩家扑克	7
	BYTE								m_bSegmentCard[GAME_PLAYER][3][3];					//分段扑克	7
	bool								m_bFinishSegment[GAME_PLAYER];						//完成分段
	BYTE								m_bShowCardCount;									//摊牌数目
	BYTE								m_bCompleteCompareCount ;							//完成比较
	BYTE								m_bOverTime[GAME_PLAYER];							//摊牌超时
	bool								m_bUserLeft[GAME_PLAYER];							//玩家强退
	bool                                m_bSpecialTypeTable[GAME_PLAYER];                   //是否特殊牌型	7
	int                                 m_nPlayerCount;                                     //实际玩家人数
	TCHAR                               m_lpLeftszName[GAME_PLAYER][MAX_PATH];              //逃跑玩家的名字
	int                                 m_nLeftChairID[GAME_PLAYER];                        //逃跑玩家的椅子号
	LONG                                m_lLeftScore[GAME_PLAYER];                          //逃跑玩家扣得分数
	bool                                m_bPlayer[GAME_PLAYER];                             //玩家存在
	bool                                m_bOnePlayer;                                       //是否只剩一个玩家
	bool                                m_bDragon[GAME_PLAYER];                             //是否倒水	7
	tagScoreInfo                        m_tagScoreInfo[GAME_PLAYER];                        //写分
	LONG                                m_lGameEndTax;                                      //游戏结束的税收
	bool                                m_bGameEnd;                                         //游戏是否结束
	LONG                                m_lLefUserScore[GAME_PLAYER];                       //强退玩家的分数

	int									m_nResult[GAME_PLAYER][3];							//每一道的道数
	int									m_nSpecialScore[GAME_PLAYER];						//喜分
	int									m_nSpecialTongGuan[GAME_PLAYER];						// 通关
	int                                 m_nSpecialType[GAME_PLAYER];							//是否通关
	int									m_nSpecialCount[GAME_PLAYER];							//喜个数
	int									m_nAllResult[GAME_PLAYER];							//最终结果
	SCORE								m_lGameScoreEx[GAME_PLAYER];							// 分数
	SCORE								m_lCellScore;										//单元底分
	BOOL								m_bShowCardChairId[GAME_PLAYER];					//摊牌的椅子id
	time_t								m_tNowTimeStamp;									//当前时间时间戳
	int									m_nIndexResult[GAME_PLAYER][3];						//每墩牌大小序号
	BOOL								m_bTongGuan[GAME_PLAYER][3];							//是否通关
	int									m_nEveryType[GAME_PLAYER][3];						//每墩牌型
	int									m_nGamePlays;										// 玩家人数
	Region								m_rgArea1[5];										//记录序号
	Region								m_rgArea2[5];					
	Region								m_rgArea3[5];

	//
private:
	int                                 m_bCompareResult[GAME_PLAYER][3];		//每一道比较结果
	int									m_bShootState[6][2];					//打枪(0赢的玩家,1输的玩家)
	int									m_bThreeKillResult[GAME_PLAYER];		//全垒打加减分
	int								    m_bToltalWinDaoShu[GAME_PLAYER];		//总共道数
	int                                 m_bCompareDouble[GAME_PLAYER];			//打枪的道数
	int                                 m_bSpecialCompareResult[GAME_PLAYER];	//特殊牌型比较结果 
	LONG								m_lGameScore[GAME_PLAYER];				//游戏积分
	BYTE								m_nXShoot;								//几家打枪
	byte								m_cbGameRule[RULE_LEN];
	BYTE								m_cbCard[52];							//记录牌

	//控制变量
protected:
	TCHAR								m_szRoomName[32];						//配置房间
	TCHAR								m_szConfigFileName[MAX_PATH];			//配置文件
	TCHAR								m_szConfigFilePath[MAX_PATH];			//玩家配置文件路径

	//机器人换牌配置信息
	bool								m_bIsCheat;						//是否作弊（换牌）
	LONG                                m_lRobotloseMax;				//机器人输的最大分数有概率换牌
	LONG                                m_lRobotLoseMaxChange;			//机器人输的最大分数百分百换牌
	BYTE                                m_btRobotLoseChangePercent;		//机器人输分，换牌百分比

	LONG                                m_lRealBankerRobotLoseMax;				//有概率换牌的起始输分分数
	LONG                                m_lRealBankerRobotLoseMaxChange;		//百分百换牌的起始输分分数
	BYTE                                m_btRealBankerRobotLoseChangePercent;	//起始输分，换牌百分比

	LONG                                m_lRobotProbabilityTanking;		//赢的金额数，机器人有概率放水
	LONG                                m_lRobotDefineTanking;			//赢的金额数，机器人百分百放水
	BYTE                                m_btRobotTankingPercent;		//机器人放水的百分比

	BYTE                                m_btUsuallyChangePercent;		//通常情况下，换牌百分比
	BYTE                                m_btRobotWinScorePercent ;		//扣除机器人赢的分数的百分比
	LONG								m_lRobotWinChiefScore;			//扣除的机器人赢的分数总和
	LONG                                m_lRobotResultScore;			//所有机器人输赢的总分
	LONG                                m_WinnerScore;					//本局机器人输赢结果分数

	BOOL								m_IsCheatUser;					//是否控制玩家
	DWORD								m_dwUserID;						//控制输赢的玩家ID
	DWORD								m_dwUserLoseWinPercent;			//控制增加玩家输赢的百分比概率
	BOOL								m_bLoseWin;						//控制玩家是赢还是输（1表示赢 0表示输）
	LONG								m_lWinMaxGold;					//控制玩家赢的金额最大上限
	LONG								m_lLoseMaxGold;					//控制玩家输的金额的最大下限
	LONG								m_lAllUserGold;					//控制玩家的输赢情况
	LONG								m_lUserGold;					//本局用户成绩

	//客户端控制
protected:
	bool							m_bControl;								//是否控制
	bool							m_bControlLossWin;						//控制输赢
	DWORD							m_dwControlID;							//控制的玩家ID
	int								m_nControlCount;						//控制次数

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//是否有效
	virtual bool  IsValid() { return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual VOID RepositionSink();

	virtual bool SetGameRule(byte *cbRule, int len);

	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem);
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID){return true;}
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){};

	//信息接口
public:
	//开始模式
	virtual BYTE GetGameStartMode();
	//游戏状态
	virtual bool IsUserPlaying(WORD wChairID);

	//消息处理
protected:

	//退出事件
	bool OnUserExit(WORD wChairID,WORD wTableID,WORD wUserID);
	//设置分段
	bool OnSetSegment(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
	//玩家摊牌
	bool OnShowCard(const WORD wChairID , const void * pDataBuffer, WORD wDataSize) ;
    bool BinarySearch(const DWORD array[], int wCount, DWORD wUserID); 

	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize);
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);

	//////////////////////////////////////////////////////////////////////////

	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	//功能函数
private:
	//设置摆牌
	void ShowCard(BYTE ShowPos);
	//计算结果
	void ComputeResult();
	//计算每一道的道数
	void ComputeChout();
	//加载配置文件
	void LoadGameINI();
	//写入配置
	void WriteGameINI();
	//控制作弊
	void ControlCheat();
	//玩家控制
	bool UserControl();
	//客户端控制
	bool ClientControl();
	// 换牌
	bool OnChangeCard(const WORD wChairID, const void * pDataBuffer, WORD wDataSize);
	void ComputeChoutEx();
	void CalculateEveryScore();

};

//////////////////////////////////////////////////////////////////////////

#endif