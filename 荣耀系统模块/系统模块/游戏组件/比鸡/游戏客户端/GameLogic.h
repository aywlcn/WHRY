#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////
// 基础牌型
#define CT_INVALID			0				  // 错误牌型
#define CT_SINGLE			1                 //单牌类型
#define CT_DOUBLE			2                 //对子类型
#define CT_SHUN_ZI          3                 //顺子类型
#define CT_JIN_HUA          4                 //金花类型
#define CT_SHUN_JIN         5                 //顺金类型
#define CT_BAO_ZI			6                 //豹子类型
#define CT_SPECIAL          7                 //特殊类型
#define TYPE_MAX_COUNT		3
#define DRAW                2                 //和局类型

// 特殊牌型
#define CT_ALL_TONGHUA					31								// 全同花
#define CT_ALL_SHANTIAO					32								// 全三条
#define CT_ALL_TONGSE					33								// 同色

#define CT_SPECIAL_SANQING				34								// 三清
#define CT_SPECIAL_QUANHEI				35								// 全黑
#define CT_SPECIAL_QUANHONG				36								// 全红
#define CT_SPECIAL_SHUANGSHUNQING		37								// 双顺清
#define CT_SPECIAL_SHUANGSANTIAO		38								// 双三条
#define CT_SPECIAL_QUANSANTIAO			39								// 全三条
#define CT_SPECIAL_SIGETOU				40								// 四个头
#define CT_SPECIAL_SIGETOU2				41								// 四个头
#define CT_SPECIAL_YITIAOLONG			42								// 一条龙
#define CT_SPECIAL_TONGGUAN				43								// 通关
//数值掩码
#define	LOGIC_MASK_COLOR				0xF0							//花色掩码
#define	LOGIC_MASK_VALUE				0x0F							//数值掩码

//分析结构
struct tagAnalyseData
{
	BYTE							bOneCount ;								//单张数目
	BYTE							bTwoCount ;								//两张数目 
	BYTE							bThreeCount ;							//三张数目
	BYTE							bFourCount ;							//四张数目
	BYTE							bOneFirst[13];							//单牌位置
	BYTE							bTwoFirst[13];							//对牌位置
	BYTE							bThreeFirst[13];						//三条位置
	BYTE							bFourFirst[13];							//四张位置
	bool							bStraight;								//是否同花
};

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//四张数目
	BYTE 							cbThreeCount;						//三张数目
	BYTE 							cbDoubleCount;						//两张数目
	BYTE							cbSignedCount;						//单张数目
	BYTE							cbFourCardData[13];					//四张扑克
	BYTE							cbThreeCardData[13];				//三张扑克
	BYTE							cbDoubleCardData[13];				//两张扑克
	BYTE							cbSignedCardData[13];				//单张扑克
};
//分析结构
struct tagAnalyseType
{
	bool                            bOnePare;                           //有一对
	bool                            bTwoPare;                           //有两对
	bool                            bThreeSame;                         //有三条
	bool                            bStraight;                          //有顺子
	bool                            bFlush;                             //有同花
	bool                            bGourd;                             //有葫芦
	bool                            bFourSame;                          //有铁支
	bool                            bStraightFlush;                     //有同花顺
	BYTE                            cbOnePare[100];                     //一对的序号
	BYTE                            cbTwoPare[100];                     //两对的序号
	BYTE                            cbThreeSame[100];                   //三条的序号
	BYTE                            cbStraight[100];                    //顺子的序号
	BYTE                            cbFlush[100];                       //同花的序号
	BYTE                            cbGourd[100];                       //葫芦的序号
	BYTE                            cbFourSame[100];                    //铁支的序号
	BYTE                            cbStraightFlush[100];               //同花顺的序号
	bool                            bbOnePare[20];                      //所有一对标志弹起
	bool                            bbTwoPare[20];                      //所有二对标志弹起
	bool                            bbThreeSame[20];                    //所有三条标志弹起
	bool                            bbStraight[20];                     //所有顺子标志弹起
	bool                            bbFlush[20];                        //所有同花标志弹起
	bool                            bbGourd[20];                        //所有葫芦标志弹起
	bool                            bbFourSame[20];                     //所有铁支标志弹起
	bool                            bbStraightFlush[20];                //所有同花顺标志弹起
	BYTE                            btOnePare;                          //一对的数量 单独
	BYTE                            btThreeSame;                        //三条数量   单独

    BYTE                            btTwoPare;                          //两对的数量
	BYTE                            btStraight;                         //顺子的数量
	BYTE                            btFlush;                            //同花的数量
	BYTE                            btGourd;                            //葫芦的数量
	BYTE                            btFourSame;                         //铁支的数量
	BYTE                            btStraightFlush;                    //同花顺的数量 
};
//////////////////////////////////////////////////////////////////////////
//排列类型
enum enSortCardType
{
	enDescend ,																//降序类型 
	enAscend ,																//升序类型
	enColor																	//花色类型
};


//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[52];				//扑克数据
    
	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();
    BYTE                            btCardSpecialData[9];
	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);

	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE btSpecialCard[]);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //十六进制前面四位表示牌的数值
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //十六进制后面四位表示牌的花色 


	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	void RandCardListEx(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);
	//添加扑克
	bool AppendCard(const BYTE bAppendCard[],BYTE bAppendCount,BYTE bCardData[],BYTE &bCardCount);
	//得到牌的数量
	BYTE GetCardCount(BYTE bCardData[]);

	//逻辑函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE bCardData);

	//获得8种类型
	tagAnalyseType GetType(BYTE bCardData[], BYTE bCardCount);
	//内部函数

	//分析扑克
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//分析扑克
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;

	//三敦总共的水数
	BYTE ThreeDunAllShuiShu(BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[]);
	//是否比前面的大
	//bool IsBiggerThanFront(BYTE btFrontFrontCard[],BYTE btFrontMidCard[],BYTE btFrontBackCard[],BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[]);
	//牌数据是否一样
	bool IsSameCardData(BYTE btFirstCard[],BYTE btNextCard[],BYTE btFirstCount,BYTE btNextCount);
	BYTE GetSpecialCardType(BYTE cbFirstData[], BYTE cbMidData[], BYTE cbBackDatd[], int nFirstCount, int nMidCount, int nBackCount, int nType);
	void RandCardListEx2(BYTE *pCard, BYTE bBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif