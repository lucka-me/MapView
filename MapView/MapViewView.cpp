
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
    ON_COMMAND(ID_DATA_AFFINE, &CMapViewView::OnDataAffine)
    ON_COMMAND(ID_DATA_BUILD_INDEX, &CMapViewView::OnDataBuildIndex)
    ON_COMMAND(ID_RETRIEVE_CLICK_POINT, &CMapViewView::OnRetrieveClickPoint)
    ON_COMMAND(ID_RETRIEVE_CLICK_POLYLINE, &CMapViewView::OnRetrieveClickPolyline)
    ON_COMMAND(ID_RETRIEVE_CLICK_POLYGON, &CMapViewView::OnRetrieveClickPolygon)
    ON_WM_LBUTTONDOWN()
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

    for (int i = 0; i < pDoc->featureList.GetSize(); i++) {
        pDoc->featureList[i]->Draw(memDC, pDoc->bound);
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
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
    CancelOpr();
}

void CMapViewView::OnLButtonDown(UINT nFlags, CPoint point) {
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (oprType == OPR_NONE)
        return;

    CMapViewDoc* pDoc = GetDocument();
    MFPoint * mapPoint = pDoc->bound.ConvertToMap(point);
    int row = int((mapPoint->x - pDoc->bound.mapButtom) / pDoc->gridIndex.resolution);
    int col = int((mapPoint->y - pDoc->bound.mapLeft) / pDoc->gridIndex.resolution);
    if (row < 0 || row > pDoc->gridIndex.row ||
        col < 0 || col > pDoc->gridIndex.col)
        return;

    switch (oprType) {
        case OPR_RETRIEVE_CLICK_POINT: {
            for (int i = 0; i < pDoc->gridIndex.index[row][col].GetSize(); i++) {
                if (pDoc->gridIndex.index[row][col][i]->GetType() == FT_POINT) {
                    if (pDoc->gridIndex.index[row][col][i]->DidSelected(*mapPoint)) {
                        CString msg;
                        msg.Format(_T("序号：%d，分类ID：%d"), pDoc->gridIndex.index[row][col][i]->SN, pDoc->gridIndex.index[row][col][i]->id);
                        MessageBox(msg, _T("点击检索 - 点"), MB_OK);
                        break;
                    }
                }
            }
            break;
        }
        case OPR_RETRIEVE_CLICK_POLYLINE: {
            for (int i = 0; i < pDoc->gridIndex.index[row][col].GetSize(); i++) {
                if (pDoc->gridIndex.index[row][col][i]->GetType() == FT_POLYLINE) {
                    if (pDoc->gridIndex.index[row][col][i]->DidSelected(*mapPoint)) {
                        MFStyle style;
                        CClientDC dc(this);
                        style.lineColor = RGB(255, 219, 79);
                        style.lineWidth = 2;
                        style.penStyle = PS_SOLID;
                        pDoc->gridIndex.index[row][col][i]->Draw(dc, pDoc->bound, style);
                        CString msg;
                        msg.Format(_T("序号：%d，分类ID：%d"), pDoc->gridIndex.index[row][col][i]->SN, pDoc->gridIndex.index[row][col][i]->id);
                        MessageBox(msg, _T("点击检索 - 线"), MB_OK);
                        Invalidate();
                        break;
                    }
                }
            }
            break;
        }
        case OPR_RETRIEVE_CLICK_POLYGON: {
            for (int i = 0; i < pDoc->gridIndex.index[row][col].GetSize(); i++) {
                if (pDoc->gridIndex.index[row][col][i]->GetType() == FT_POLYGON) {
                    if (pDoc->gridIndex.index[row][col][i]->DidSelected(*mapPoint)) {
                        CString msg;
                        msg.Format(_T("序号：%d，分类ID：%d"), pDoc->gridIndex.index[row][col][i]->SN, pDoc->gridIndex.index[row][col][i]->id);
                        MessageBox(msg, _T("点击检索 - 面"), MB_OK);
                        break;
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    CView::OnLButtonDown(nFlags, point);
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
// 数据处理-坐标转换
void CMapViewView::OnDataAffine() {
    CMapViewDoc* pDoc = GetDocument();
    pDoc->DoAffine();
    Invalidate();
}

// 数据处理-建立索引
void CMapViewView::OnDataBuildIndex() {
    CMapViewDoc* pDoc = GetDocument();
    pDoc->DoBuildIndex();
    Invalidate();
}

// 数据检索-点选检索-点
void CMapViewView::OnRetrieveClickPoint() {
    CancelOpr();
    oprType = OPR_RETRIEVE_CLICK_POINT;
}

// 数据检索-点选检索-线
void CMapViewView::OnRetrieveClickPolyline() {
    // TODO: 在此添加命令处理程序代码
    CancelOpr();
    oprType = OPR_RETRIEVE_CLICK_POLYLINE;
}

// 数据检索-点选检索-面
void CMapViewView::OnRetrieveClickPolygon() {
    // TODO: 在此添加命令处理程序代码
    CancelOpr();
    oprType = OPR_RETRIEVE_CLICK_POLYGON;
}

// 取消操作
void CMapViewView::CancelOpr() {
    oprType = OPR_NONE;
    Invalidate();
}
