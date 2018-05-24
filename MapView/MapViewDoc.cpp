
// MapViewDoc.cpp: CMapViewDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapView.h"
#endif

#include "MapViewDoc.h"
#include "MainFrm.h"
#include "AffineDialg.h"
#include "GridIndexDialog.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMapViewDoc

IMPLEMENT_DYNCREATE(CMapViewDoc, CDocument)

BEGIN_MESSAGE_MAP(CMapViewDoc, CDocument)
    
END_MESSAGE_MAP()


// CMapViewDoc 构造/析构

CMapViewDoc::CMapViewDoc() {
	// TODO: 在此添加一次性构造代码

}

CMapViewDoc::~CMapViewDoc() {
}

BOOL CMapViewDoc::OnNewDocument() {
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

BOOL CMapViewDoc::OnOpenDocument(LPCTSTR lpszPathName) {
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // 读取数据
    CStdioFile file;
    if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyNone | CFile::typeText))
        return FALSE;

    MVDecoder decoder = MVDecoder();
    CString line;

    featureList.RemoveAll();
    featureList.Add(new MapFeature());  // 占位用，因为数据序号从1开始
    controlPointList.RemoveAll();

    int SN, featureID;
    int blank;  // 用于处理无用输入

    // 修改状态栏，添加进度条
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    pFrame->m_wndStatusBar.EnablePaneProgressBar(1, file.GetLength(), TRUE);
    pFrame->m_wndStatusBar.SetPaneProgress(1, 0, TRUE);
    pFrame->m_wndStatusBar.SetPaneText(0, _T("读取数据中"));
    
    while (true) {

        file.ReadString(line);
        if (line.Find(_T("-999999.0,-999999.0")) != -1)
            break;
        decoder.Decode(line, SN, blank);
        file.ReadString(line);
        decoder.Decode(line, featureID, blank);

        switch (featureID) {
            case 10000:
            case 10001:
            case 10003:
            case 10004: {
                MFPolyline * newPolyline = new MFPolyline(featureID, SN);
                double x, y;
                do {
                    file.ReadString(line);
                    if (line.Find(_T("-666666.0,-666666.0")) != -1)
                        break;
                    decoder.Decode(line, x, y);
                    newPolyline->Add(new MFPoint(x, y));
                } while (true);
                switch (featureID) {
                    case 10000: {   // 图廓
                        newPolyline->style.bottomLineColor = RGB(0, 0, 0);
                        newPolyline->style.topLineColor = RGB(0, 0, 0);
                        newPolyline->style.bottomLineWidth = 1;
                        newPolyline->style.topLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        newPolyline->style.topPenStyle = PS_SOLID;
                        break;
                    }
                    case 10001: {   // 铁路
                        newPolyline->style.bottomLineColor = RGB(56, 60, 60);
                        newPolyline->style.topLineColor = RGB(56, 60, 60);
                        newPolyline->style.bottomLineWidth = 3;
                        newPolyline->style.topLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        newPolyline->style.topPenStyle = PS_DASH;
                        break;
                    }
                    case 10003: {   // 汽渡
                        newPolyline->style.bottomLineColor = RGB(72, 92, 194);
                        newPolyline->style.topLineColor = RGB(72, 92, 194);
                        newPolyline->style.bottomLineWidth = 1;
                        newPolyline->style.topLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_DASH;
                        newPolyline->style.topPenStyle = PS_DASH;
                        break;
                    }
                    case 10004: {   // 主要道路
                        newPolyline->style.bottomLineColor = RGB(167, 56, 54);
                        newPolyline->style.topLineColor = RGB(230, 0, 51);
                        newPolyline->style.bottomLineWidth = 4;
                        newPolyline->style.topLineWidth = 2;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        newPolyline->style.topPenStyle = PS_SOLID;
                        break;
                    }
                    default:
                        break;
                }
                featureList.Add(newPolyline);
                break;
            }
            case 20000:
            case 20001:
            case 20002:
            case 20003:
            case 20004: {
                MFPolygon * newPolyline = new MFPolygon(featureID, SN);
                double x, y;
                do {
                    file.ReadString(line);
                    if (line.Find(_T("-666666.0,-666666.0")) != -1)
                        break;
                    decoder.Decode(line, x, y);
                    newPolyline->Add(new MFPoint(x, y));
                } while (true);
                switch (featureID) {
                    case 20002: {   // 铁路中转站
                        newPolyline->style.bottomLineColor = RGB(148, 147, 145);
                        newPolyline->style.fillColor = RGB(171, 169, 164);
                        newPolyline->style.bottomLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        break;
                    }
                    case 20003: {   // 河流、湖泊
                        newPolyline->style.bottomLineColor = RGB(83, 143, 189);
                        newPolyline->style.fillColor = RGB(102, 189, 204);
                        newPolyline->style.bottomLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        break;
                    }
                    case 20004: {   // 居民地
                        newPolyline->style.bottomLineColor = RGB(0, 103, 167);
                        newPolyline->style.fillColor = RGB(168, 185, 189);
                        newPolyline->style.bottomLineWidth = 1;
                        newPolyline->style.bottomPenStyle = PS_SOLID;
                        break;
                    }
                    default:
                        break;
                }
                featureList.Add(newPolyline);
                break;
            }
            case 30000: {   // 控制点
                double x, y;
                file.ReadString(line);
                decoder.Decode(line, x, y);
                MFPoint * point = new MFPoint(x, y, featureID, SN);
                point->style.bottomLineColor = RGB(230, 0, 51);
                point->style.fillColor = RGB(201, 23, 30);
                featureList.Add(point);
                controlPointList.Add(point);
                file.ReadString(line);
                break;
            }
            default: {
                CString message;
                message.Format(_T("未识别的要素代码：%d\n已读取%d个数据，终止读取。"),
                    featureID, featureList.GetSize() - 1);
                AfxMessageBox(message, MB_OK | MB_ICONERROR);
                featureList.RemoveAll();
                return FALSE;
            }
        }

        pFrame->m_wndStatusBar.SetPaneProgress(1, file.GetPosition(), TRUE);
    }

    // 仿射变换
    DoAffine();
    // 建立索引
    DoBuildIndex();
    
    CString message;
    message.Format(_T("共读取%d条数据。"), featureList.GetSize() - 1);
    pFrame->m_wndStatusBar.SetPaneText(0, message);

    return TRUE;
}

// CMapViewDoc 序列化

void CMapViewDoc::Serialize(CArchive& ar) {
	if (ar.IsStoring()) {
		// TODO: 在此添加存储代码
	} else {
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMapViewDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds) {
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CMapViewDoc::InitializeSearchContent() {
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMapViewDoc::SetSearchContent(const CString& value) {
	if (value.IsEmpty()) {
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	} else {
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr) {
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMapViewDoc 诊断

#ifdef _DEBUG
void CMapViewDoc::AssertValid() const {
	CDocument::AssertValid();
}

void CMapViewDoc::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMapViewDoc 命令

// 坐标变换（仿射变换）
bool CMapViewDoc::DoAffine() {
    
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    pFrame->m_wndStatusBar.EnablePaneProgressBar(1, featureList.GetSize() - 1, TRUE);
    pFrame->m_wndStatusBar.SetPaneProgress(1, 0, TRUE);
    pFrame->m_wndStatusBar.SetPaneText(0, _T("仿射变换处理中"));

    if (controlPointList.GetSize() < 3) {
        AfxMessageBox(_T("控制点不足3个，无法进行仿射变换"), MB_OK | MB_ICONWARNING);
    } else {
        double orgX1 = controlPointList[0]->x, orgY1 = controlPointList[0]->y;
        double orgX2 = controlPointList[1]->x, orgY2 = controlPointList[1]->y;
        double orgX3 = controlPointList[2]->x, orgY3 = controlPointList[2]->y;

        CAffineDialg affineDlg(orgX1, orgY1, orgX2, orgY2, orgX3, orgY3);
        if (affineDlg.DoModal() == IDOK) {
            double setX1, setY1;
            double setX2, setY2;
            double setX3, setY3;
            affineDlg.Get(setX1, setY1, setX2, setY2, setX3, setY3);

            if ((orgY2 - orgY1) == 0.0 ||
                (orgY2 - orgY1) == 0.0 ||
                ((orgX1 - orgX2) * (orgY3 - orgY1) - (orgX1 - orgX3) * (orgY2 - orgY1)) == 0.0 ||
                ((orgX1 - orgX2) * (orgY3 - orgY1) - (orgX1 - orgX3) * (orgY2 - orgY1)) == 0.0) {
                AfxMessageBox(_T("无法计算仿射变换参数。"), MB_OK | MB_ICONWARNING);
            } else {
                double a1 = ((setX3 - setX1) * (orgY2 - orgY1) - (setX2 - setX1) * (orgY3 - orgY1))
                    / ((orgX1 - orgX2) * (orgY3 - orgY1) - (orgX1 - orgX3) * (orgY2 - orgY1));
                double b1 = (setX2 - setX1 + a1 * (orgX1 - orgX2)) / (orgY2 - orgY1);
                double c1 = setX1 - a1 * orgX1 - b1 * orgY1;

                double a2 = ((setY3 - setY1) * (orgY2 - orgY1) - (setY2 - setY1) * (orgY3 - orgY1))
                    / ((orgX1 - orgX2) * (orgY3 - orgY1) - (orgX1 - orgX3) * (orgY2 - orgY1));
                double b2 = (setY2 - setY1 + a2 * (orgX1 - orgX2)) / (orgY2 - orgY1);
                double c2 = setY1 - a2 * orgX1 - b2 * orgY1;

                for (int i = 1; i < featureList.GetSize(); i++) {
                    featureList[i]->Affine(a1, b1, c1, a2, b2, c2);
                    pFrame->m_wndStatusBar.SetPaneProgress(1, i, TRUE);
                }
            }
            
        }
    }

    // 确定图幅
    pFrame->m_wndStatusBar.EnablePaneProgressBar(1, featureList.GetSize() - 1, TRUE);
    pFrame->m_wndStatusBar.SetPaneProgress(1, 0, TRUE);
    pFrame->m_wndStatusBar.SetPaneText(0, _T("确定图幅中"));

    double left, bottom, right, top;
    bottom = controlPointList[0]->x;
    top    = controlPointList[0]->x;
    left   = controlPointList[0]->y;
    right  = controlPointList[0]->y;
    for (int i = 1; i < featureList.GetSize(); i++) {
        switch (featureList[i]->GetType()) {
            case FT_POINT: {
                MFPoint * point = (MFPoint *)featureList[i];
                bottom = point->x < bottom ? point->x : bottom;
                top    = point->x >    top ? point->x : top;
                left   = point->y <   left ? point->y : left;
                right  = point->y >  right ? point->y : right;
                break;
            }
            case FT_POLYLINE: {
                MFPolyline * polyline = (MFPolyline *)featureList[i];
                for (int j = 0; j < polyline->pointList.GetSize(); j++) {
                    MFPoint * point = (MFPoint *)polyline->pointList[j];
                    bottom = point->x < bottom ? point->x : bottom;
                    top    = point->x >    top ? point->x : top;
                    left   = point->y <   left ? point->y : left;
                    right  = point->y >  right ? point->y : right;
                }
                break;
            }
            case FT_POLYGON: {
                MFPolygon * polygon = (MFPolygon *)featureList[i];
                for (int j = 0; j < polygon->pointList.GetSize(); j++) {
                    MFPoint * point = (MFPoint *)polygon->pointList[j];
                    bottom = point->x < bottom ? point->x : bottom;
                    top    = point->x >    top ? point->x : top;
                    left   = point->y <   left ? point->y : left;
                    right  = point->y >  right ? point->y : right;
                }
                break;
            }
            default:
                break;
        }
        pFrame->m_wndStatusBar.SetPaneProgress(1, i, TRUE);
    }
    bound.SetMap(left, bottom, right, top);
    
    return true;
}

// 建立索引
bool CMapViewDoc::DoBuildIndex() {

    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    pFrame->m_wndStatusBar.EnablePaneProgressBar(1, featureList.GetSize() - 1, TRUE);
    pFrame->m_wndStatusBar.SetPaneProgress(1, 0, TRUE);
    pFrame->m_wndStatusBar.SetPaneText(0, _T("建立索引中"));

    double gridResolution = 5.0; // 网格分辨率
    CGridIndexDialog gridIndexDlg;
    if (gridIndexDlg.DoModal() == IDOK)
        gridResolution = gridIndexDlg.resolution;
    else
        return false;

    gridIndex.Set(gridResolution, bound);
    
    for (int i = 0; i < featureList.GetSize(); i++) {
        switch (featureList[i]->GetType()) {
            case FT_POINT: {
                MFPoint * point = (MFPoint *)featureList[i];
                int row = int((point->x - bound.mapBottom) / gridIndex.resolution);
                int col = int((point->y - bound.mapLeft)   / gridIndex.resolution);
                gridIndex.index[row][col].Add(point);
                break;
            }
            case FT_POLYLINE: {
                MFPolyline * polyline = (MFPolyline *)featureList[i];

                for (int scan = 1; scan < polyline->pointList.GetSize(); scan++) {
                    MFPoint * pointA = (MFPoint *)polyline->pointList[scan - 1];
                    MFPoint * pointB = (MFPoint *)polyline->pointList[scan];
                    double left   = pointA->y < pointB->y ? pointA->y : pointB->y;
                    double bottom = pointA->x < pointB->x ? pointA->x : pointB->x;
                    double right  = pointA->y > pointB->y ? pointA->y : pointB->y;
                    double top    = pointA->x > pointB->x ? pointA->x : pointB->x;
                    int leftCol   = int((left   - bound.mapLeft)   / gridIndex.resolution);
                    int bottomRow = int((bottom - bound.mapBottom) / gridIndex.resolution);
                    int rightCol  = int((right  - bound.mapLeft)   / gridIndex.resolution);
                    int topRow    = int((top    - bound.mapBottom) / gridIndex.resolution);
                    for (int scanRow = bottomRow; scanRow <= topRow; scanRow++)
                        for (int scanCol = leftCol; scanCol <= rightCol; scanCol++)
                            if (!gridIndex.index[scanRow][scanCol].Has(polyline))
                                gridIndex.index[scanRow][scanCol].Add(polyline);

                }
                break;
            }
            case FT_POLYGON: {
                MFPolygon * polygon = (MFPolygon *)featureList[i];
                MFPoint * firstPoint = (MFPoint *)polygon->pointList[0];
                double left   = firstPoint->y;
                double bottom = firstPoint->x;
                double right  = firstPoint->y;
                double top    = firstPoint->x;
                for (int scan = 1; scan < polygon->pointList.GetSize(); scan++) {
                    MFPoint * point = (MFPoint *)polygon->pointList[scan];
                    left   = point->y < left   ? point->y : left;
                    bottom = point->x < bottom ? point->x : bottom;
                    right  = point->y > right  ? point->y : right;
                    top    = point->x > top    ? point->x : top;
                }
                int leftCol   = int((left   - bound.mapLeft)   / gridIndex.resolution);
                int bottomRow = int((bottom - bound.mapBottom) / gridIndex.resolution);
                int rightCol  = int((right  - bound.mapLeft)   / gridIndex.resolution);
                int topRow    = int((top    - bound.mapBottom) / gridIndex.resolution);
                for (int scanRow = bottomRow; scanRow <= topRow; scanRow++)
                    for (int scanCol = leftCol; scanCol <= rightCol; scanCol++)
                        gridIndex.index[scanRow][scanCol].Add(polygon);
                break;
            }
            default:
                break;
        }
        
        pFrame->m_wndStatusBar.SetPaneProgress(1, i, TRUE);
    }

    return true;
}
