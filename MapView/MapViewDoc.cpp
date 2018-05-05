
// MapViewDoc.cpp: CMapViewDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapView.h"
#endif

#include "MapViewDoc.h"

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
    CTypedPtrArray<CObArray, MFPoint *> controlPointList;// = CTypedPtrArray<CObArray, MFPoint *>();

    int index, featureID;
    int blank;  // 用于处理无用输入

    while (true) {

        file.ReadString(line);
        if (line.Find(_T("-999999.0,-999999.0")) != -1)
            break;
        decoder.Decode(line, index, blank);
        file.ReadString(line);
        decoder.Decode(line, featureID, blank);

        switch (featureID) {
            case 10000:
            case 10001:
            case 10003:
            case 10004: {
                MFPolyline * newPolyline = new MFPolyline(featureID);
                double x, y;
                do {
                    file.ReadString(line);
                    if (line.Find(_T("-666666.0,-666666.0")) != -1)
                        break;
                    decoder.Decode(line, x, y);
                    newPolyline->Add(new MFPoint(x, y));
                } while (true);
                featureList.Add(newPolyline);
                break;
            }
            case 20000:
            case 20001:
            case 20002:
            case 20003:
            case 20004: {
                MFPolygon * newPolyline = new MFPolygon(featureID);
                double x, y;
                do {
                    file.ReadString(line);
                    if (line.Find(_T("-666666.0,-666666.0")) != -1)
                        break;
                    decoder.Decode(line, x, y);
                    newPolyline->Add(new MFPoint(x, y));
                } while (true);
                featureList.Add(newPolyline);
                break;
            }
            case 30000: {
                double x, y;
                file.ReadString(line);
                decoder.Decode(line, x, y);
                MFPoint * point = new MFPoint(x, y, featureID);
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
    }

    // 以控制点确定图幅
    double left, buttom, right, top;
    buttom = controlPointList[0]->x;
    top    = controlPointList[0]->x;
    left   = controlPointList[0]->y;
    right  = controlPointList[0]->y;
    for (int i = 1; i < controlPointList.GetSize(); i++) {
        buttom = controlPointList[i]->x < buttom ? controlPointList[i]->x : buttom;
        top    = controlPointList[i]->x >    top ? controlPointList[i]->x :    top;
        left   = controlPointList[i]->y <   left ? controlPointList[i]->x :   left;
        right  = controlPointList[i]->y >  right ? controlPointList[i]->x :  right;
    }
    bound.SetMap(left, buttom, right, top);

    CString message;
    message.Format(_T("数据读取成功，共%d条数据。"), featureList.GetSize() - 1);
    AfxMessageBox(message, MB_OK);

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
