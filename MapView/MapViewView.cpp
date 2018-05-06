
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

void CMapViewView::OnDraw(CDC* pDC)
{
	CMapViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    if (pDoc->featureList.IsEmpty()) {
        return;
    }

    // 确定显示范围
    CRect displayRect;
    GetClientRect(&displayRect);
    // 对比纵横比
    double displayRatio = double(displayRect.Height()) / displayRect.Width();
    double mapRatio = pDoc->bound.MapHeight() / pDoc->bound.MapWidth();
    if (displayRatio > mapRatio) {
        int displayHeightHalf = int(displayRect.Width() * 0.9 * mapRatio / 2);
        pDoc->bound.SetDisplay(int(displayRect.Width() * 0.05),
                               int(displayRect.Height() / 2 + displayHeightHalf),
                               int(displayRect.Width() * 0.95),
                               int(displayRect.Height() / 2 - displayHeightHalf));
    } else {
        int displayWidthHalf = int(displayRect.Height() * 0.9 / mapRatio / 2);
        pDoc->bound.SetDisplay(int(displayRect.Width() / 2 - displayWidthHalf),
                               int(displayRect.Height() * 0.95),
                               int(displayRect.Width() / 2 + displayWidthHalf),
                               int(displayRect.Height() * 0.05));
    }

    // 双缓存
    CDC memDC;
    CBitmap memBitmap;
    memDC.CreateCompatibleDC(NULL);
    memBitmap.CreateCompatibleBitmap(pDC, displayRect.right, displayRect.bottom);
    CBitmap *pOldBit = memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(0, 0, displayRect.right, displayRect.bottom, RGB(255, 255, 255));

    // 绘制 Bound
    /*
    CBrush brush;
    brush.CreateSolidBrush(RGB(0, 0, 0));
    CGdiObject *pOldBrush = memDC.SelectObject(&brush);
    CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen *pOldPen = memDC.SelectObject(&pen);

    memDC.Rectangle(pDoc->bound.displayLeft, pDoc->bound.displayTop, pDoc->bound.displayRight, pDoc->bound.displayButtom);

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);
    */

    for (int i = 0; i < pDoc->featureList.GetSize(); i++) {
        MFStyle style;
        switch (pDoc->featureList[i]->id) {
            case 10000: {   // 图廓
                style.lineColor = RGB(0, 0, 0);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 10001: {   // 铁路
                style.lineColor = RGB(0, 0, 0);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 10003: {   // 汽渡
                style.lineColor = RGB(72, 92, 194);
                style.lineWidth = 1;
                style.penStyle = PS_DASH;
                break;
            }
            case 10004: {   // 主要道路
                style.lineColor = RGB(255, 0, 0);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 20001: {   // 码头
                style.lineColor = RGB(72, 92, 194);
                style.fillColor = RGB(90, 152, 245);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 20002: {   // 铁路中转站
                style.lineColor = RGB(148, 147, 145);
                style.fillColor = RGB(171, 169, 164);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 20003: {   // 河流、湖泊
                style.lineColor = RGB(83, 143, 189);
                style.fillColor = RGB(102, 189, 204);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 20004: {   // 居民地
                style.lineColor = RGB(0, 103, 167);
                style.fillColor = RGB(168, 185, 189);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
            case 30000: {   // 控制点
                style.lineColor = RGB(0, 0, 255);
                style.penStyle = PS_SOLID;
                break;
            }
            default: {
                style.lineColor = RGB(255, 255, 255);
                style.fillColor = RGB(255, 255, 255);
                style.lineWidth = 1;
                style.penStyle = PS_SOLID;
                break;
            }
        }
        pDoc->featureList[i]->Draw(memDC, pDoc->bound, style);
    }

    pDC->BitBlt(0, 0, displayRect.right, displayRect.bottom, &memDC, 0, 0, SRCCOPY);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
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

