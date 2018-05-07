#pragma once

// CAffineDialg 对话框

class CAffineDialg : public CDialogEx
{
	DECLARE_DYNAMIC(CAffineDialg)

public:
	CAffineDialg(double setOrgX1, double setOrgY1,
                 double setOrgX2, double setOrgY2,
                 double setOrgX3, double setOrgY3,
                 CWnd* pParent = nullptr);   // 构造函数
	virtual ~CAffineDialg();
    virtual BOOL OnInitDialog();
    virtual void OnOK();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AFFINEBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
    // 方法
    void Get(double & targetSetX1, double & targetSetY1,
             double & targetSetX2, double & targetSetY2,
             double & targetSetX3, double & targetSetY3);

public:
    // 属性
    double orgX1, orgY1, orgX2, orgY2, orgX3, orgY3;
    double setX1, setY1, setX2, setY2, setX3, setY3;
};
