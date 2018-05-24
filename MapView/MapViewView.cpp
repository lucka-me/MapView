
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

#include "RetrieveIDDialog.h"

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
    ON_COMMAND(ID_RETRIEVE_ID, &CMapViewView::OnRetrieveId)
    ON_COMMAND(ID_VIEW_ZOOM, &CMapViewView::OnViewZoom)
    ON_WM_MOUSEWHEEL()
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
    if (isDisplayDefault) {
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
    }
    

    // 双缓存
    CDC memDC;
    CBitmap memBitmap;
    memDC.CreateCompatibleDC(NULL);
    memBitmap.CreateCompatibleBitmap(pDC, displayRect.right, displayRect.bottom);
    CBitmap *pOldBit = memDC.SelectObject(&memBitmap);
    memDC.FillSolidRect(0, 0, displayRect.right, displayRect.bottom, RGB(255, 255, 255));

    // 绘图
    for (int i = 0; i < pDoc->featureList.GetSize(); i++) {
        pDoc->featureList[i]->Draw(memDC, pDoc->bound);
    }

    // 绘制格网
    CGdiObject *pOldBrush = memDC.SelectStockObject(NULL_BRUSH);
    CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
    CPen *pOldPen = memDC.SelectObject(&pen);
    
    double displayGridSize = double(pDoc->bound.displayButtom - pDoc->bound.displayTop) / pDoc->gridIndex.row;
    int gridRight = pDoc->bound.displayLeft + int(floor(pDoc->gridIndex.col * displayGridSize));
    for (int i = 0; i <= pDoc->gridIndex.row; i++) {
        memDC.MoveTo(pDoc->bound.displayLeft, pDoc->bound.displayTop + int(floor(i * displayGridSize)));
        memDC.LineTo(gridRight, pDoc->bound.displayTop + int(floor(i * displayGridSize)));
    }
    int gridTop = pDoc->bound.displayButtom - int(floor(pDoc->gridIndex.row * displayGridSize));
    for (int i = pDoc->gridIndex.col; i >= 0; i--) {
        memDC.MoveTo(pDoc->bound.displayLeft + int(floor(i * displayGridSize)), gridTop);
        memDC.LineTo(pDoc->bound.displayLeft + int(floor(i * displayGridSize)), pDoc->bound.displayButtom);
    }

    memDC.SelectObject(pOldPen);
    memDC.SelectObject(pOldBrush);

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

    MFStyle style;
    style.lineColor = RGB(255, 219, 79);
    style.fillColor = RGB(254, 242, 99);
    style.lineWidth = 2;
    style.penStyle = PS_SOLID;

    switch (oprType) {
        case OPR_RETRIEVE_CLICK_POINT: {
            for (int i = pDoc->gridIndex.index[row][col].GetSize() - 1; i >= 0; i--) {
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
            for (int i = pDoc->gridIndex.index[row][col].GetSize() - 1; i >= 0; i--) {
                if (pDoc->gridIndex.index[row][col][i]->GetType() == FT_POLYLINE) {
                    if (pDoc->gridIndex.index[row][col][i]->DidSelected(*mapPoint)) {
                        CClientDC dc(this);
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
            for (int i = pDoc->gridIndex.index[row][col].GetSize() - 1; i >= 0; i--) {
                if (pDoc->gridIndex.index[row][col][i]->GetType() == FT_POLYGON) {
                    if (pDoc->gridIndex.index[row][col][i]->DidSelected(*mapPoint)) {
                        CClientDC dc(this);
                        pDoc->gridIndex.index[row][col][i]->Draw(dc, pDoc->bound, style);
                        CString msg;
                        msg.Format(_T("序号：%d，分类ID：%d"), pDoc->gridIndex.index[row][col][i]->SN, pDoc->gridIndex.index[row][col][i]->id);
                        MessageBox(msg, _T("点击检索 - 面"), MB_OK);
                        Invalidate();
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

BOOL CMapViewView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
    switch (oprType) {
    case OPR_VIEW_ZOOM: {
        CMapViewDoc* pDoc = GetDocument();
        double level = 1.0;
        if (zDelta > 0) {
            // 缩小
            level = 0.9;
        } else if (zDelta < 0) {
            // 放大
            level = 1.1;
        }
        pDoc->bound.displayLeft   = pt.x + int((pDoc->bound.displayLeft   - pt.x) * level);
        pDoc->bound.displayButtom = pt.y + int((pDoc->bound.displayButtom - pt.y) * level);
        pDoc->bound.displayRight  = pt.x + int((pDoc->bound.displayRight  - pt.x) * level);
        pDoc->bound.displayTop    = pt.y + int((pDoc->bound.displayTop    - pt.y) * level);
        isDisplayDefault = false;
        Invalidate();
        break;
    }
    default:
        break;
    }


    return CView::OnMouseWheel(nFlags, zDelta, pt);
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
    CancelOpr();
    oprType = OPR_RETRIEVE_CLICK_POLYLINE;
}

// 数据检索-点选检索-面
void CMapViewView::OnRetrieveClickPolygon() {
    CancelOpr();
    oprType = OPR_RETRIEVE_CLICK_POLYGON;
}

// 数据检索-检索分类ID
void CMapViewView::OnRetrieveId() {
    CancelOpr();
    CRetrieveIDDialog retrieveIDDlg;
    if (retrieveIDDlg.DoModal() == IDOK) {
        int retrieveID = retrieveIDDlg.retriveID;
        FeatureArray & featureList = GetDocument()->featureList;
        CMapViewDoc* pDoc = GetDocument();
        MFStyle style;
        style.lineColor = RGB(255, 219, 79);
        style.fillColor = RGB(254, 242, 99);
        style.lineWidth = 2;
        style.penStyle = PS_SOLID;
        CClientDC dc(this);
        int count = 0;
        for (int i = 0; i < featureList.GetSize(); i++) {
            if (featureList[i]->id == retrieveID) {
                count++;
                featureList[i]->Draw(dc, pDoc->bound, style);
            }
        }
        CString msg;
        msg.Format(_T("分类ID：%d，共查到%d个。"), retrieveID, count);
        MessageBox(msg, _T("检索分类ID"), MB_OK);
        Invalidate();
    }
}

// 取消操作
void CMapViewView::CancelOpr() {
    oprType = OPR_NONE;
    Invalidate();
}

void CMapViewView::OnViewZoom() {
    // TODO: 在此添加命令处理程序代码
    CancelOpr();
    oprType = OPR_VIEW_ZOOM;
}
