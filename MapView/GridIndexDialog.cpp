// GridIndexDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "MapView.h"
#include "GridIndexDialog.h"
#include "afxdialogex.h"
#include "MVDataKit.h"

// CGridIndexDialog 对话框

IMPLEMENT_DYNAMIC(CGridIndexDialog, CDialogEx)

CGridIndexDialog::CGridIndexDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRIDBOX, pParent) {

}

CGridIndexDialog::~CGridIndexDialog() {
}

BOOL CGridIndexDialog::OnInitDialog() {
    CDialogEx::OnInitDialog();

    SetDlgItemTextW(IDC_GRID_RESOLUTION_EDIT, _T("5.0"));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CGridIndexDialog::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGridIndexDialog, CDialogEx)
END_MESSAGE_MAP()


// CGridIndexDialog 消息处理程序
void CGridIndexDialog::OnOK() {
    
    MVDecoder decoder;
    CString text;

    GetDlgItemTextW(IDC_GRID_RESOLUTION_EDIT, text);
    resolution = decoder.ToDouble(text);

    CDialogEx::OnOK();
}
