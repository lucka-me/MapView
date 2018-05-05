﻿
// MapViewView.cpp: CMapViewView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapView.h"
#endif

#include "MapViewDoc.h"
#include "MapViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapViewView

IMPLEMENT_DYNCREATE(CMapViewView, CView)

BEGIN_MESSAGE_MAP(CMapViewView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMapViewView 构造/析构

CMapViewView::CMapViewView()
{
	// TODO: 在此处添加构造代码

}

CMapViewView::~CMapViewView()
{
}

BOOL CMapViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapViewView 绘图

void CMapViewView::OnDraw(CDC* /*pDC*/)
{
	CMapViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMapViewView 打印


void CMapViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMapViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMapViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMapViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMapViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	
#endif
}


// CMapViewView 诊断

#ifdef _DEBUG
void CMapViewView::AssertValid() const
{
	CView::AssertValid();
}

void CMapViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapViewDoc* CMapViewView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapViewDoc)));
	return (CMapViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapViewView 消息处理程序
