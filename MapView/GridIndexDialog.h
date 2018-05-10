#pragma once


// CGridIndexDialog 对话框

class CGridIndexDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CGridIndexDialog)

public:
	CGridIndexDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGridIndexDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRIDBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    // 方法

public:
    // 属性
    double resolution = 5.0;
    virtual BOOL OnInitDialog();
    virtual void OnOK();
};
