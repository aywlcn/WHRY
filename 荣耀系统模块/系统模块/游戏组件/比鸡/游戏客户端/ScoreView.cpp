#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"

//////////////////////////////////////////////////////////////////////////

#define SCORE_VIEW_EXIT             102
#define IDC_STARTAGAIN                      112                             //重新开始
#define IDI_ENDAGAIN                        113                             //结束
BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(SCORE_VIEW_EXIT,OnClose)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CScoreView::CScoreView(void)
{
	//设置数据
	m_nChairWin=0;
	m_nChairLoss=0;
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_nCompare,0,sizeof(m_nCompare));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));
	m_bEnd=0;
    
	//加载资源
	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_GAME_END);
}

//析构函数
CScoreView::~CScoreView(void)
{
}
////控件绑定
//void CScoreView::DoDataExchange(CDataExchange* pDX)
//{
//	__super::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_CLOSE, m_pClose);
//}
//初始化函数
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//MessageBox(NULL,"you",NULL);
	//设置数据
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));
    
	//设置界面
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	CRgn ImageRgn;
	m_ImageBack.CreateImageRegion(ImageRgn,RGB(255,0,255));
	SetWindowRgn(ImageRgn, FALSE);
	ImageRgn.DeleteObject();
	//移动窗口
	MoveWindow(300,200,m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	//SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER); 

	//设置按钮
	/*m_pClose.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(20,30,10,10),this,SCORE_VIEW_EXIT);
	m_btStartAgain.Create(TEXT(""),WS_CHILD,CRect(0,0,0,0),this,IDC_STARTAGAIN);
	m_btStartAgain.ShowWindow(SW_SHOW);*/
	//CImageHandle ImageHandle1(&m_ImageStart);
	//m_btStartAgain.MoveWindow(70,284,m_ImageStart.GetWidth()/5,m_ImageStart.GetHeight());

	//创建资源管理器
	m_pSkinResourceManager= (CSkinResourceManager *)CSkinResourceManager::GetInstance();
	ASSERT(m_pSkinResourceManager!=NULL);
	
	return TRUE;
}

//重画函数
void CScoreView::OnPaint() 
{
	//if(m_bEnd)
	{
		CPaintDC dc(this); 
		//设置 DC
		dc.SetBkMode(TRANSPARENT);
		
		CFont DrawFont;
		DrawFont.CreateFont(15,0,0,0,600,0,0,0,0,0,0,0,0,TEXT("粗体"));
		dc.SelectObject(&DrawFont);
		//dc.SelectObject(&CSkinAttribute::m_DefaultFont);


		//绘画背景
		m_ImageBack.BitBlt(dc,0,0);

		//显示用户
		CRect rcDraw;
		CRect rcDrawDun;
		CString strScore ;

		for (WORD i=0;i<CountArray(m_szUserName);i++)
		{
			int ndaoshu=0;
			LONG lScore=0;
			if(lstrcmp(m_szUserName[i],_T("")))
			{
				
				DrawFont.CreateFont(20,0,0,0,600,0,0,0,0,0,0,0,0,TEXT("粗体"));
				dc.SelectObject(&DrawFont);
				dc.SetTextColor(RGB(10,243,108));
				//用户名字
				rcDraw.top=15;
				rcDraw.bottom=rcDraw.top+40;
			/*	rcDrawDun.left = 30;
				rcDrawDun.right= 108;*/
				if(0==i)
				{
					rcDraw.left=90;
					rcDraw.right=rcDraw.left+78;
				}
				if(1==i)
				{
					rcDraw.left=170;
					rcDraw.right=rcDraw.left+78;
				}
				if(2==i)
				{
					rcDraw.left=250;
					rcDraw.right=rcDraw.left+78;
				}
				if(3==i)
				{
					rcDraw.left=330;
					rcDraw.right=rcDraw.left+78;
				}
				TCHAR UserName[256]=TEXT("womendeai");
				dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				//用户每敦的总道数
 				
				dc.SetTextColor(RGB(166,158,255));
				rcDraw.top=49;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][0]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 
				rcDraw.top=86;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][1]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 
				rcDraw.top=122;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][2]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

				rcDraw.top=154;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_nCompareDouble[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

				rcDraw.top=185;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_cCompareKillThree[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
                
				rcDraw.top=222;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_nCompareSpecial[i]); 
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				rcDraw.top=147;
// 				rcDraw.bottom=181;
// 				if(m_nChairWin==i)
// 				{
//                    strScore.Format("%s",TEXT("打"));
// 				   dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				}
// 				if(m_nChairLoss==i)
// 				{
// 					strScore.Format("%s",TEXT("被打"));
//                     dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				}
//				dc.SetTextColor(RGB(0,255,100));
				//用户的分数
				rcDraw.top=253;
				rcDraw.bottom=rcDraw.top+40;
				//ndaoshu=m_nCompare[i][0]+m_nCompare[i][1]+m_nCompare[i][2]+m_nCompareDouble[i]+m_nCompareSpecial[i];
				strScore.Format(_T("%d"),m_nTotalDaoShu[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				
				//dc.SetTextColor(RGB(255,0,255));
				//rcDraw.top=253;
				//rcDraw.bottom=rcDraw.top+40;
				//strScore.Format("%ld",m_lGameTax[i]/*111111*/);
				//dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				dc.SetTextColor(RGB(255,255,0));
				rcDraw.top=290;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%ld"),m_lGameScore[i]/*111111*/);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
			}
		}
	}
	return;
}

//绘画背景
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}



//鼠标消息
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//消息模拟
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//重置积分
void CScoreView::ResetScore()
{
	//设置数据
	m_bEnd=0;
	m_nChairWin=0;
	m_nChairLoss=0;
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_nCompare,0,sizeof(m_nCompare));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));

	//绘画视图
	Invalidate(NULL);
	
	return;
}

//设置税收
void CScoreView::SetGameTax(LONG lGameEveryTax[])
{
	//设置变量
	CopyMemory(m_lGameTax,lGameEveryTax,sizeof(m_lGameTax));
	Invalidate(FALSE);
	return;
}

//设置积分
void CScoreView::SetGameScore(WORD wChairID,LPCTSTR pszUserName, LONG lScore,int nTotalDaoShu )
{
	//设置变量
	if (wChairID<CountArray(m_lGameScore))
	{
		m_lGameScore[wChairID]=lScore;
		m_nTotalDaoShu[wChairID]=nTotalDaoShu;
		
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}

	return;
}


//////////////////////////////////////////////////////////////////////////

void CScoreView::OnKillFocus(CWnd* pNewWnd)
{
	//隐藏窗体
	//ShowWindow(SW_HIDE) ;

	CDialog::OnKillFocus(pNewWnd);

}
void CScoreView::OnClose()
{
	ShowWindow(SW_SHOW);
	return;
}

void CScoreView::SetEnd( bool bEnd )
{
   m_bEnd=bEnd;
}