#pragma once

// CRetrieveIDDialog 对话框

class CRetrieveIDDialog : public CDialogEx {
	DECLARE_DYNAMIC(CRetrieveIDDialog)

public:
	CRetrieveIDDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRetrieveIDDialog();
    virtual void OnOK();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RETRIEVEIDBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    // 方法

public:
    // 属性
    int retriveID = 0;
};
