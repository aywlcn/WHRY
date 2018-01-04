#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							88									//游戏  ID
#define GAME_PLAYER						5									//游戏人数
#define GAME_NAME						TEXT("比鸡")					    //游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型
#define HAND_CARD_COUNT					9									//扑克数目

#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本
//游戏状态
#define GS_WK_FREE				    	GAME_STATUS_FREE						//等待开始
#define GS_WK_PLAYING			    	GAME_STATUS_PLAY+3						//游戏进行

//分段类型
enum enSegmentType
{
	enFront ,																//前墩类型
	enMid ,																	//中墩类型
	enBack,																	//后墩类型
	enAllSeg,																//所有扑克
	enErr,																	//错误类型
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START				206									//游戏开始
#define SUB_S_SEND_CARD					215									//发牌命令
#define SUB_S_SETSEGMENT				102									//设置分段
#define SUB_S_SHOW_CARD					203									//玩家摊牌
#define SUB_S_GAME_END					204									//游戏结束
#define SUB_S_GAME_COMPARE              205                                 //游戏比牌
#define SUB_S_PLAYER_LEFT               210                                 //玩家强退
#define SUB_S_SCORE_RESULT              107                                 //结算
#define SUB_S_USEREXIT                  108                                 //玩家退出
#define SUB_S_AMDIN_COMMAND				109									//管理员命令
#define SUB_S_MOBILE_PUTCARD			219									//手机托管牌


//游戏开始
struct CMD_S_GameStart 
{
	LONG								wUserToltalChip;					//总共金币
	WORD								wCurrentUser;						//当前玩家
};

struct GameRule
{
	SCORE sCellScore;
};

//玩家强退
struct CMD_S_PlayerLeft
{
	int                                 nPlayerLetfChairID;                 //强退玩家的椅子ID
};

//游戏状态
struct CMD_S_StatusFree
{
	INT									lBaseScore;
	BYTE								bGameStatus[GAME_PLAYER];
	BYTE								bHaveBanker;

	INT									nAllWinTimes;
	SCORE								lTurnScore[GAME_PLAYER];
	SCORE								lCollectScore[GAME_PLAYER];

	BYTE								cbTimeStartGame;
	BYTE								cbTimeCallBanker;
	BYTE								cbTimeSetChip;
	BYTE								cbTimeRangeCard;
	BYTE								cbTimeShowCard;
	WORD								wServerID;

	LONG								wUserToltalChip;					//总共金币
	BYTE                                btRobotCount;                       //机器人数量
};



//发送扑克
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //当前玩家
	WORD								wBanker;							 //庄家
	BYTE								bCardData[HAND_CARD_COUNT];			 //手上扑克
	BYTE								cbPlayCount;
	bool								bGameStatus[GAME_PLAYER];
	SCORE								lChipArray[GAME_PLAYER];
// 	BYTE                                bAllHandCardData[GAME_PLAYER][HAND_CARD_COUNT];//所有玩家的扑克数据
// 	LONG								lCellScore;							 //游戏底分
};

struct CMD_S_ChangeCard
{
	BYTE bCard[9];		// 换牌的数据
};

//手机托管牌
struct CMD_S_MobilePutCard
{
	//BYTE cbMobilePutCard[13];
	BYTE cbMobilePutCount;
	BYTE cbMobilePutCard[GAME_PLAYER][HAND_CARD_COUNT];
};

//玩家退出
struct CMD_S_UserExit
{
	int                                 nLeftChairID;                       //离开玩家的椅子ID        
};

struct SPECIALRESULT
{
	int									nSpecialScore;							// 喜分数
	int                                 nSpecialType;							//是否通关
	int                                 nSpecialCount;							// 喜个数
};
//游戏结束
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//游戏积分
	BYTE								bEndMode;							//结束方式
	int                                 cbCompareResult[GAME_PLAYER][3];    //每一道比较结果
	int                                 nIndexResult[GAME_PLAYER][3];		//没墩牌的大小序号
	SPECIALRESULT						nSpecialResult[GAME_PLAYER];		// 每一个玩家的特殊牌型
	int									nEveryType[GAME_PLAYER][3];			// 每墩牌型
	int								    nAllResult[GAME_PLAYER];			//最终结果
};

//玩家摊牌
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//当前玩家
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[3];						//中墩扑克
	BYTE								bBackCard[3];						//后墩扑克
};

//游戏状态
struct CMD_S_StatusPlay
{
	LONG								lCellScore;							//底分
	BYTE								bHandCardData[HAND_CARD_COUNT];		//扑克数据
	BYTE								bSegmentCard[GAME_PLAYER][3][3];	//分段扑克
	bool								bFinishSegment[GAME_PLAYER];		//完成分段
	bool								bGameStatus[GAME_PLAYER];			//游戏状态
	BYTE								bCbTimeStartGame;					//开始时间
	BYTE								bCbTimeCallBanker;					//叫庄时间
	BYTE								bCbTimeSetChip;						//下注时间
	BYTE								bCbTimeRangeCard;					//理牌时间
	BYTE								bCbTimeShowCard;					//开牌时间

};
//结算
struct CMD_S_ScoreResult
{
	bool                                bScoreResult;                       //是否显示结算
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_SEGCARD					301									//分段命令
#define SUB_C_SHOWCARD					304									//玩家摊牌
#define SUB_C_OVERTIME					303									//摊牌超时
#define SUB_C_USEREXIT                  302                                 //用户离开
#define SUB_C_AMDIN_COMMAND				305									//管理员命令

//用户离开
struct CMD_C_UserExit
{
	WORD                               wChairID;							//离开椅子ID
	WORD                               wTableID;							//离开桌子ID
	WORD                               wUserID;								//用户ID
	bool                               bEnterExit;							//是否一进入就离开
};

//分段信息
struct CMD_C_ShowCard
{
	//WORD								wCurrentUser;						//当前玩家
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[3];						//中墩扑克
	BYTE								bBackCard[3];						//后墩扑克
	//bool								bCanSeeShowCard;					//能否看牌
};

struct CMD_C_ControlData
{
	bool bWinloss;					//控制输赢（1赢，0输）
	TCHAR chUserName[MAX_PATH];		//控制玩家名字
	DWORD dwChairID;				//控制位置
	BYTE cbCount;					//控制次数
};

struct CMD_C_ChangeCard
{
	 BYTE bCard[9];		// 换牌的数据
};
//////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif