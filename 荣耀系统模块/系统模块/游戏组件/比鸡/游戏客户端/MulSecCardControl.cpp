#include "StdAfx.h"
#include "Resource.h"
#include ".\mulseccardcontrol.h"

//////////////////////////////////////////////////////////////////////////
//静态变量
bool					CMulSecCardControl::m_bLoad=false;					//加载标志
CSize					CMulSecCardControl::m_CardSize;						//扑克大小
CBitImage				CMulSecCardControl::m_ImageCard;					//图片资源
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMulSecCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

static void MyMsgBox1(LPCSTR lpszFormat, ...)
{
//	va_list args;
//	int		nBuf;
//	TCHAR	szBuffer[512];
//
//	va_start(args, lpszFormat);
//	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
//#ifdef _DEBUG
//	AfxMessageBox(szBuffer);
//#else
//	OutputDebugString(szBuffer);
//#endif
//
//	va_end(args);
}

CMulSecCardControl::CMulSecCardControl(void)
{
	//配置变量
	m_bDisplay=false;

	//间隔变量
	m_dwCardHSpace=18;
	m_dwCardVSpace=19;

	//位置变量
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//分段变量
	m_bSecCount = MSCC_SECTION_NUM;
	m_bSecArray[0] = MSCC_SECTION_ONE;
	m_bSecArray[1] = MSCC_SECTION_TWO;
	m_bSecArray[2] = MSCC_SECTION_THREE;

	//运行变量
	m_dwCurrentIndex=0xFFFFFFFF;

	//内部数据
	m_pSinkWindow=NULL;

	//资源变量
	if (m_bLoad==false)
	{
		//加载资源
		m_bLoad=true;
		if (GetSystemMetrics(SM_CXSCREEN)>=1024) m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD);
		else m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD_800);

		//获取大小
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

CMulSecCardControl::~CMulSecCardControl(void)
{
}

//设置窗口
void CMulSecCardControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//设置变量
	m_pSinkWindow=pSinkWindow;

	return;
}

//设置显示
void CMulSecCardControl::SetDisplayFlag(bool bDisplay)
{
	//状态判断
	if (m_bDisplay==bDisplay) return;

	//设置变量
	m_bDisplay=bDisplay;

	//重画控件
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//设置间距
void CMulSecCardControl::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace)
{
	//设置变量
	m_dwCardHSpace=dwCardHSpace;
	m_dwCardVSpace=dwCardVSpace;

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//基准位置
void CMulSecCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//基准位置
void CMulSecCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//设置变量
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//获取数目
DWORD CMulSecCardControl::GetCardCount()
{
	return (DWORD)m_CardDataItem.GetCount();
}

//设置扑克
DWORD CMulSecCardControl::SetCardData(const BYTE bCardData[], DWORD dwCardCount)
{
	//数据验证
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//设置扑克
	if (bCardData == NULL)
	{
		m_CardDataItem.SetSize(0);
	}
	else
	{
		m_CardDataItem.SetSize(dwCardCount);
		for (DWORD i=0;i<dwCardCount;i++)
		{
			//数据验证
			ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

			m_CardDataItem[i].bShoot=false;
			m_CardDataItem[i].bCardData=bCardData[i];
		}
	}

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();

	return dwCardCount;
}

//设置扑克
bool CMulSecCardControl::SetCardItem(BYTE bCardData, DWORD dwIndex)
{
	ASSERT(dwIndex>=0 && dwIndex<(DWORD)m_CardDataItem.GetCount()) ;
	if(dwIndex<0 || dwIndex>=(DWORD)m_CardDataItem.GetCount()) return false ;
	m_CardDataItem[dwIndex].bShoot=false;
	m_CardDataItem[dwIndex].bCardData=bCardData;

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();
	return true ;
}

//获取扑克
DWORD CMulSecCardControl::GetCardData(BYTE bCardData[], DWORD dwMaxCount)
{
	//效验参数
	ASSERT(dwMaxCount>=(DWORD)m_CardDataItem.GetCount());

	ASSERT(dwMaxCount>0 && dwMaxCount<=HAND_CARD_COUNT) ;

	//拷贝扑克
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	//数据验证
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0 ; i<dwCardCount ; ++i)
		bCardData[i] = m_CardDataItem[i].bCardData ;

	return dwCardCount;
}

//获取扑克
DWORD CMulSecCardControl::GetCardData(BYTE bCardData)
{
	//拷贝扑克
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//数据验证
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0 ; i<dwCardCount ; ++i)
		if(bCardData==m_CardDataItem[i].bCardData) return i ;	 
	return -1 ;
}

//设置扑克
DWORD CMulSecCardControl::SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount)
{
	//数据验证
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//设置扑克
	m_CardDataItem.SetSize(dwCardCount);
	CopyMemory(m_CardDataItem.GetData(),CardItemArray,dwCardCount*sizeof(tagCardItem));

	//调整控件
	if (m_hWnd!=NULL) RectifyControl();
 
	return 0;
}

//调整位置
void CMulSecCardControl::RectifyControl()
{
	//变量定义
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	DWORD dwSecCCount[MSCC_SECTION_NUM] = {0};
	dwSecCCount[2] = (dwCardCount>8)?dwCardCount-8:0;
	dwSecCCount[1] = (dwCardCount-dwSecCCount[2]>3)?dwCardCount-dwSecCCount[2]-3:0;
	dwSecCCount[0] = dwCardCount-dwSecCCount[2]-dwSecCCount[1];

	//数据验证
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//计算大小
	CSize ControlSize;
	if (dwCardCount > 0 && dwCardCount <= MSCC_SECTION_ONE)
	{
		ControlSize.cy = m_CardSize.cy;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else if (dwCardCount > MSCC_SECTION_ONE && dwCardCount <= MSCC_SECTION_TWO+MSCC_SECTION_ONE)
	{
		ControlSize.cy = m_CardSize.cy + m_dwCardVSpace;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else if (dwCardCount > MSCC_SECTION_TWO+MSCC_SECTION_ONE && dwCardCount <= HAND_CARD_COUNT)
	{
		ControlSize.cy = m_CardSize.cy + m_dwCardVSpace*2;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else
	{
		ControlSize.SetSize(0, 0);
	}

	//横向位置
	int nXPos=0;
	int nYPos=0;

	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	//移动位置
	SetWindowPos(NULL, nXPos, nYPos, ControlSize.cx, ControlSize.cy, SWP_NOZORDER);

	//变量定义
	CRgn CardRegion,SignedRegion;
	tagCardItem * pCardItem=NULL;

	//设置区域
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i = 0; i < dwCardCount; i++)
	{
		//数据验证
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//获取扑克
		pCardItem=&m_CardDataItem[i];

		//计算位置
		if (i < MSCC_SECTION_ONE)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[0]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*i;
			nYPos = 0;
		}
		else if (i >= MSCC_SECTION_ONE && i < MSCC_SECTION_TWO+MSCC_SECTION_ONE)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[1]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE);
			nYPos = m_dwCardVSpace;
		}
		else if (i >= MSCC_SECTION_TWO+MSCC_SECTION_ONE && i < HAND_CARD_COUNT)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[2]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE - MSCC_SECTION_TWO);
			nYPos = m_dwCardVSpace*2;
		}

		//合并区域
        SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+m_CardSize.cx+1,nYPos+m_CardSize.cy+1,2,2);

		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//设置区域
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//重画界面
	Invalidate(TRUE);

	return;
}

//索引切换
DWORD CMulSecCardControl::SwitchCardPoint(const CPoint & MousePoint)
{//获取鼠标在牌控件上的第几张牌
	return DWORD(-1);
}

//重画函数
void CMulSecCardControl::OnPaint()
{
	CPaintDC dc(this);

	//变量定义
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	DWORD dwSecCCount[MSCC_SECTION_NUM] = {0};
	dwSecCCount[2] = (dwCardCount>8)?dwCardCount-8:0;
	dwSecCCount[1] = (dwCardCount-dwSecCCount[2]>3)?dwCardCount-dwSecCCount[2]-3:0;
	dwSecCCount[0] = dwCardCount-dwSecCCount[2]-dwSecCCount[1];

	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//创建位图
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//创建 DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//绘画扑克
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	//数据验证
	ASSERT(m_CardDataItem.GetCount()>=0 && m_CardDataItem.GetCount()<=HAND_CARD_COUNT) ;

	for (INT i=0;i<m_CardDataItem.GetCount();i++)
	{
		//数据验证
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//获取扑克
		tagCardItem * pCardItem=&m_CardDataItem[i];

		//图片位置
		if ((m_bDisplay==true)&&(pCardItem->bCardData!=0))
		{
			dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//屏幕位置
		if (i < MSCC_SECTION_ONE)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[0]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*i;
			dwYScreenPos = 0;
		}
		else if (i >= MSCC_SECTION_ONE && i < MSCC_SECTION_TWO+MSCC_SECTION_ONE)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[1]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE);
			dwYScreenPos = m_dwCardVSpace;
		}
		else if (i >= MSCC_SECTION_TWO+MSCC_SECTION_ONE && i < HAND_CARD_COUNT)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[2]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE - MSCC_SECTION_TWO);
			dwYScreenPos = m_dwCardVSpace*2;
		}

		//绘画扑克
		m_ImageCard.BitBlt(BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
	}

	//绘画界面
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//清理资源
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//建立消息
int CMulSecCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//设置背景
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//鼠标消息
void CMulSecCardControl::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	return;
}

//鼠标消息
void CMulSecCardControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	return;
}

//鼠标消息
void CMulSecCardControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	return;
}

//光标消息
BOOL CMulSecCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	return __super::OnSetCursor(pWnd,nHitTest,message);
}
