#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
  //设置变量
  ZeroMemory(&m_CustomRule,sizeof(m_CustomRule));

  m_CustomRule.lRobotScoreMin = 100000;
  m_CustomRule.lRobotScoreMax = 1000000;
  m_CustomRule.lRobotBankGet = 1000000;
  m_CustomRule.lRobotBankGetBanker = 10000000;
  m_CustomRule.lRobotBankStoMul = 10;

  //库存控制
  m_CustomRule.lStorageStart = 1000000;
  m_CustomRule.lStorageDeduct = 0;
  m_CustomRule.lStorageMax = 5000000;
  m_CustomRule.lStorageMul = 80;

  m_CustomRule.wMaxOpenTurn = 15;
  m_CustomRule.wMinLookTurn = 0;
  m_CustomRule.wMinCompareTurn = 0;

  m_CustomRule.bCompareSmall = 0;

  m_CustomRule.cbCompareWay = 0;

  return;
}

//析构函数
CDlgCustomRule::~CDlgCustomRule()
{
}

//配置函数
BOOL CDlgCustomRule::OnInitDialog()
{
  __super::OnInitDialog();

  //设置控件
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMIN))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTSCOREMAX))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGET))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKGETBANKER))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_ROBOTBANKSTOMUL))->LimitText(2);

  ((CEdit*)GetDlgItem(IDC_EDIT_STORAGE_START))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_STORAGE_DEDUCT))->LimitText(3);
  ((CEdit*)GetDlgItem(IDC_EDIT_STORAGE_MAX))->LimitText(10);
  ((CEdit*)GetDlgItem(IDC_EDIT_STORAGE_MUL))->LimitText(2);
  
  ((CEdit*)GetDlgItem(IDC_EDIT_MIN_LOOK_TURN))->LimitText(1);
  ((CEdit*)GetDlgItem(IDC_EDIT_MIN_COMPARE_TURN))->LimitText(1);
  ((CEdit*)GetDlgItem(IDC_EDIT_MAX_OPEN_TURN))->LimitText(3);

  //更新参数
  FillDataToControl();

  return FALSE;
}

//确定函数
VOID CDlgCustomRule::OnOK()
{
  //投递消息
  GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDOK, 0), 0);

  return;
}

//取消消息
VOID CDlgCustomRule::OnCancel()
{
  //投递消息
  GetParent()->PostMessage(WM_COMMAND, MAKELONG(IDCANCEL, 0), 0);

  return;
}

//更新控件
bool CDlgCustomRule::FillDataToControl()
{
  //机器人存储款
  SetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN, m_CustomRule.lRobotScoreMin);
  SetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX, m_CustomRule.lRobotScoreMax);
  SetDlgItemInt(IDC_EDIT_ROBOTBANKGET, m_CustomRule.lRobotBankGet);
  SetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER, m_CustomRule.lRobotBankGetBanker);
  SetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL, m_CustomRule.lRobotBankStoMul);

  //库存控制
  SetDlgItemInt(IDC_EDIT_STORAGE_START, m_CustomRule.lStorageStart);
  SetDlgItemInt(IDC_EDIT_STORAGE_DEDUCT, m_CustomRule.lStorageDeduct);
  SetDlgItemInt(IDC_EDIT_STORAGE_MAX, m_CustomRule.lStorageMax);
  SetDlgItemInt(IDC_EDIT_STORAGE_MUL, m_CustomRule.lStorageMul);

  SetDlgItemInt(IDC_EDIT_MAX_OPEN_TURN, m_CustomRule.wMaxOpenTurn);
  SetDlgItemInt(IDC_EDIT_MIN_COMPARE_TURN, m_CustomRule.wMinCompareTurn);
  SetDlgItemInt(IDC_EDIT_MIN_LOOK_TURN, m_CustomRule.wMinLookTurn);

  CButton *rudio1 = (CButton *)GetDlgItem(IDC_RADIO1);
  CButton *rudio2 = (CButton *)GetDlgItem(IDC_RADIO2);
  CButton *rudio3 = (CButton *)GetDlgItem(IDC_RADIO3);
  rudio3->SetCheck(0);
  rudio2->SetCheck(0);
  rudio1->SetCheck(0);
  if (m_CustomRule.cbCompareWay == 0)
	  rudio1->SetCheck(1);
  else if (m_CustomRule.cbCompareWay == 1)
	  rudio2->SetCheck(1);
  else
	  rudio3->SetCheck(1);


  return true;
}

//更新数据
bool CDlgCustomRule::FillControlToData()
{
  //机器人存储款
  m_CustomRule.lRobotScoreMin = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMIN);
  m_CustomRule.lRobotScoreMax = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTSCOREMAX);
  m_CustomRule.lRobotBankGet = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGET);
  m_CustomRule.lRobotBankGetBanker = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKGETBANKER);
  m_CustomRule.lRobotBankStoMul = (LONGLONG)GetDlgItemInt(IDC_EDIT_ROBOTBANKSTOMUL);

  //库存控制
  m_CustomRule.lStorageStart = (LONGLONG)GetDlgItemInt(IDC_EDIT_STORAGE_START);
  m_CustomRule.lStorageDeduct = (LONGLONG)GetDlgItemInt(IDC_EDIT_STORAGE_DEDUCT);
  m_CustomRule.lStorageMax = (LONGLONG)GetDlgItemInt(IDC_EDIT_STORAGE_MAX);
  m_CustomRule.lStorageMul = (LONGLONG)GetDlgItemInt(IDC_EDIT_STORAGE_MUL);

  m_CustomRule.wMaxOpenTurn = (LONGLONG)GetDlgItemInt(IDC_EDIT_MAX_OPEN_TURN);
  m_CustomRule.wMinCompareTurn = (LONGLONG)GetDlgItemInt(IDC_EDIT_MIN_COMPARE_TURN);
  m_CustomRule.wMinLookTurn = (LONGLONG)GetDlgItemInt(IDC_EDIT_MIN_LOOK_TURN);

  CButton *rudio1 = (CButton *)GetDlgItem(IDC_RADIO1);
  CButton *rudio2 = (CButton *)GetDlgItem(IDC_RADIO2);
  CButton *rudio3 = (CButton *)GetDlgItem(IDC_RADIO3);
  if (rudio1->GetCheck())
  {
	  m_CustomRule.bCompareSmall = FALSE;
	  m_CustomRule.cbCompareWay = 0;
  }
  else if (rudio2->GetCheck())
  {
	  m_CustomRule.bCompareSmall = TRUE;
	  m_CustomRule.cbCompareWay = 1;
  }
  else if (rudio3->GetCheck())
  {
	  m_CustomRule.bCompareSmall = FALSE;
	  m_CustomRule.cbCompareWay = 2;
  }

  if(!(m_CustomRule.lRobotScoreMax > m_CustomRule.lRobotScoreMin))
  {
    AfxMessageBox(TEXT("存款最大值应大于取款最小值"), MB_ICONSTOP);
    return false;
  }

  if(!(m_CustomRule.lRobotBankGetBanker > m_CustomRule.lRobotBankGet))
  {
    AfxMessageBox(TEXT("取款最大数额应大于取款最小数额"), MB_ICONSTOP);
    return false;
  }

  return true;
}

//读取配置
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
  //读取参数
  if(FillControlToData() == true)
  {
    CustomRule = m_CustomRule;
    return true;
  }

  return false;
}

//设置配置
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
  //设置变量
  m_CustomRule = CustomRule;

  //更新参数
  if(m_hWnd != NULL)
  {
    FillDataToControl();
  }

  return true;
}

//////////////////////////////////////////////////////////////////////////////////
