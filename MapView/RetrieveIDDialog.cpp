﻿// RetrieveIDDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "MapView.h"
#include "RetrieveIDDialog.h"
#include "afxdialogex.h"
#include "MVDataKit.h"


// CRetrieveIDDialog 对话框

IMPLEMENT_DYNAMIC(CRetrieveIDDialog, CDialogEx)

CRetrieveIDDialog::CRetrieveIDDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RETRIEVEIDBOX, pParent) {

}

CRetrieveIDDialog::~CRetrieveIDDialog() {
}

void CRetrieveIDDialog::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRetrieveIDDialog, CDialogEx)
END_MESSAGE_MAP()


// CRetrieveIDDialog 消息处理程序


void CRetrieveIDDialog::OnOK() {

    MVDecoder decoder;
    CString text;

    GetDlgItemTextW(IDC_RETRIEVE_ID_EDIT, text);
    retriveID = decoder.ToInt(text);

    CDialogEx::OnOK();
}
