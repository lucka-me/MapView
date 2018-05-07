// AffineDialg.cpp: 实现文件
//

#include "stdafx.h"
#include "MapView.h"
#include "AffineDialg.h"
#include "afxdialogex.h"
#include "MVDataKit.h"


// CAffineDialg 对话框

IMPLEMENT_DYNAMIC(CAffineDialg, CDialogEx)

CAffineDialg::CAffineDialg(double setOrgX1, double setOrgY1,
                           double setOrgX2, double setOrgY2,
                           double setOrgX3, double setOrgY3,
                           CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AFFINEBOX, pParent) {
    orgX1 = setOrgX1;
    orgY1 = setOrgY1;
    orgX2 = setOrgX2;
    orgY2 = setOrgY2;
    orgX3 = setOrgX3;
    orgY3 = setOrgY3;
}

CAffineDialg::~CAffineDialg() {
}

BOOL CAffineDialg::OnInitDialog() {
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    CString str;
    str.Format(_T("%.3f"), orgX1);
    SetDlgItemTextW(IDC_AFFINE_ORG_X1, str);
    str.Format(_T("%.3f"), orgY1);
    SetDlgItemTextW(IDC_AFFINE_ORG_Y1, str);
    str.Format(_T("%.3f"), orgX2);
    SetDlgItemTextW(IDC_AFFINE_ORG_X2, str);
    str.Format(_T("%.3f"), orgY2);
    SetDlgItemTextW(IDC_AFFINE_ORG_Y2, str);
    str.Format(_T("%.3f"), orgX3);
    SetDlgItemTextW(IDC_AFFINE_ORG_X3, str);
    str.Format(_T("%.3f"), orgY3);
    SetDlgItemTextW(IDC_AFFINE_ORG_Y3, str);
    
    setX1 = 12.5, setY1 = 37.4;
    setX2 = 62.5, setY2 = 37.4;
    setX3 = 62.5, setY3 = 82.4;

    str.Format(_T("%.1f"), setX1);
    SetDlgItemTextW(IDC_AFFINE_SET_X1, str);
    str.Format(_T("%.1f"), setY1);
    SetDlgItemTextW(IDC_AFFINE_SET_Y1, str);
    str.Format(_T("%.1f"), setX2);
    SetDlgItemTextW(IDC_AFFINE_SET_X2, str);
    str.Format(_T("%.1f"), setY2);
    SetDlgItemTextW(IDC_AFFINE_SET_Y2, str);
    str.Format(_T("%.1f"), setX3);
    SetDlgItemTextW(IDC_AFFINE_SET_X3, str);
    str.Format(_T("%.1f"), setY3);
    SetDlgItemTextW(IDC_AFFINE_SET_Y3, str);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CAffineDialg::OnOK() {
    // TODO: 在此添加专用代码和/或调用基类
    MVDecoder decoder;
    CString text;

    GetDlgItemTextW(IDC_AFFINE_SET_X1, text);
    setX1 = decoder.ToDouble(text);
    GetDlgItemTextW(IDC_AFFINE_SET_Y1, text);
    setY1 = decoder.ToDouble(text);

    GetDlgItemTextW(IDC_AFFINE_SET_X2, text);
    setX2 = decoder.ToDouble(text);
    GetDlgItemTextW(IDC_AFFINE_SET_Y2, text);
    setY2 = decoder.ToDouble(text);

    GetDlgItemTextW(IDC_AFFINE_SET_X3, text);
    setX3 = decoder.ToDouble(text);
    GetDlgItemTextW(IDC_AFFINE_SET_Y3, text);
    setY3 = decoder.ToDouble(text);

    CDialogEx::OnOK();
}

void CAffineDialg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAffineDialg, CDialogEx)
END_MESSAGE_MAP()


// CAffineDialg 消息处理程序


void CAffineDialg::Get(double & targetSetX1, double & targetSetY1,
                       double & targetSetX2, double & targetSetY2,
                       double & targetSetX3, double & targetSetY3) {
    targetSetX1 = setX1;
    targetSetY1 = setY1;
    targetSetX2 = setX2;
    targetSetY2 = setY2;
    targetSetX3 = setX3;
    targetSetY3 = setY3;
}
