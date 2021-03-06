﻿
// MapViewView.h: CMapViewView 类的接口
//

#pragma once


class CMapViewView : public CView
{
protected: // 仅从序列化创建
	CMapViewView();
	DECLARE_DYNCREATE(CMapViewView)

// 特性
public:
	CMapViewDoc* GetDocument() const;

// 操作
public:
    // 方法
    void CancelOpr();

public:
    // 属性
    // 操作类型
    enum Operation {
        OPR_NONE,   // 无操作
        OPR_RETRIEVE_CLICK_POINT,       // 点击检索-点
        OPR_RETRIEVE_CLICK_POLYLINE,    // 点击检索-线
        OPR_RETRIEVE_CLICK_POLYGON,     // 点击检索-面
        OPR_VIEW_MOVE,  // 视图-移动
    } oprType = OPR_NONE;
    bool isDisplayDefault = true;
    CPoint lastMovePoint;
    bool didMoveBegin = false;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMapViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    
    afx_msg void OnDataAffine();
    afx_msg void OnDataBuildIndex();
    afx_msg void OnRetrieveClickPoint();
    afx_msg void OnRetrieveClickPolyline();
    afx_msg void OnRetrieveClickPolygon();
    afx_msg void OnRetrieveId();
    afx_msg void OnViewMove();
    afx_msg void OnViewRestore();

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    
};

#ifndef _DEBUG  // MapViewView.cpp 中的调试版本
inline CMapViewDoc* CMapViewView::GetDocument() const
   { return reinterpret_cast<CMapViewDoc*>(m_pDocument); }
#endif

