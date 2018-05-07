#include "stdafx.h"
#include "MVDataKit.h"
#include <sstream>

// MARK: MapBound

MapBound::MapBound() {
        mapLeft = 0.0;   mapButtom = 0.0;   mapRight = 0.0;   mapTop = 0.0;
    displayLeft = 0; displayButtom = 0; displayRight = 0; displayTop = 0;
}

void MapBound::SetMap(double left, double buttom, double right, double top) {
    mapLeft   = left;
    mapButtom = buttom;
    mapRight  = right;
    mapTop    = top;
}

void MapBound::SetDisplay(int left, int buttom, int right, int top) {
    displayLeft   = left;
    displayButtom = buttom;
    displayRight  = right;
    displayTop    = top;
}

double MapBound::MapWidth() {
    return mapRight - mapLeft;
}

double MapBound::MapHeight() {
    return mapTop - mapButtom;
}

CPoint MapBound::ConvertToDisplay(MFPoint point) {
    return ConvertToDisplay(point.x, point.y);
}

CPoint MapBound::ConvertToDisplay(double x, double y) {
    int displayX = int(round(displayLeft + (y - mapLeft) / (mapRight - mapLeft) * double(displayRight - displayLeft)));
    int displayY = int(round(displayTop + (mapTop - x) / (mapTop - mapButtom) * double(displayButtom - displayTop)));
    return CPoint(displayX, displayY);
}

CPoint * MapBound::ConvertToDisplay(FeatureArray & pointList) {
    CPoint * pList = new CPoint[pointList.GetSize()];
    for (int i = 0; i < pointList.GetSize(); i++) {
        MFPoint * point = (MFPoint *)pointList[i];
        pList[i] = ConvertToDisplay(point->x, point->y);
    }
    return pList;
}

MFPoint * MapBound::ConvertToMap(CPoint point) {
    return ConvertToMap(point.x, point.y);
}

MFPoint * MapBound::ConvertToMap(int x, int y) {
    double mapX = mapButtom + double(displayButtom - y) / double(displayButtom - displayTop) * (mapTop - mapButtom);
    double mapY = mapLeft + double(x - displayLeft) / double(displayRight - displayLeft) * (mapRight - mapLeft);
    return new MFPoint(mapX, mapY);
}

// MARK: MapFeature

// MARK: MFPoint

MFPoint::MFPoint(double setX, double setY, int setId, int setSN) {
    SN = setSN;
    id = setId;
    x = setX;
    y = setY;
}

void MFPoint::Affine(double a1, double b1, double c1, double a2, double b2, double c2) {
    double tempX = x;
    double tempY = y;
    x = a1 * tempX + b1 * tempY + c1;
    y = a2 * tempX + b2 * tempY + c2;
}

void MFPoint::Draw(CDC & dc, MapBound & bound, MFStyle style) {
    CPoint displayPoint = bound.ConvertToDisplay(x, y);
    //CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
    //CPen pen;
    //pen.CreatePen(PS_SOLID, 1, style.lineColor);
    //CPen *pOldPen = dc.SelectObject(&pen);

    dc.SetPixel(displayPoint.x, displayPoint.y, style.lineColor);

    //dc.SelectObject(pOldPen);
    //dc.SelectObject(pOldBrush);
}

bool MFPoint::DidSelected(MFPoint & selectPoint, double buffer) {
    return (selectPoint.x >= x - buffer) &&
           (selectPoint.x <= x + buffer) &&
           (selectPoint.y >= y - buffer) &&
           (selectPoint.y <= y + buffer);
}

// MARK: MFPolyline

MFPolyline::MFPolyline(int setId, int setSN) {
    SN = setSN;
    id = setId;
}

void MFPolyline::Affine(double a1, double b1, double c1, double a2, double b2, double c2) {
    for (int i = 0; i < pointList.GetSize(); i++)
        pointList[i]->Affine(a1, b1, c1, a2, b2, c2);
}

void MFPolyline::Draw(CDC & dc, MapBound & bound, MFStyle style) {
    CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
    CPen pen;
    pen.CreatePen(style.penStyle, style.lineWidth, style.lineColor);
    CPen *pOldPen = dc.SelectObject(&pen);

    CPoint * pList = bound.ConvertToDisplay(pointList);
    dc.Polyline(pList, pointList.GetSize());

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

bool MFPolyline::DidSelected(MFPoint & selectPoint, double buffer) {
    return false;
}

void MFPolyline::Set(MFPoint * startPoint) {
    pointList.RemoveAll();
    Add(startPoint);
}

void MFPolyline::Add(MFPoint * newPoint) {
    pointList.Add(newPoint);
}

// MARK: MFPolygon

MFPolygon::MFPolygon(int setId, int setSN) {
    SN = setSN;
    id = setId;
}

void MFPolygon::Affine(double a1, double b1, double c1, double a2, double b2, double c2) {
    for (int i = 0; i < pointList.GetSize(); i++)
        pointList[i]->Affine(a1, b1, c1, a2, b2, c2);
}

void MFPolygon::Draw(CDC & dc, MapBound & bound, MFStyle style) {
    CBrush brush;
    brush.CreateSolidBrush(style.fillColor);
    CGdiObject *pOldBrush = dc.SelectObject(&brush);
    CPen pen;
    pen.CreatePen(style.penStyle, style.lineWidth, style.lineColor);
    CPen *pOldPen = dc.SelectObject(&pen);

    CPoint * pList = bound.ConvertToDisplay(pointList);
    dc.Polygon(pList, pointList.GetSize());

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

bool MFPolygon::DidSelected(MFPoint & selectPoint, double buffer) {
    return false;
}

void MFPolygon::Set(MFPoint * startPoint) {
    pointList.RemoveAll();
    Add(startPoint);
}

void MFPolygon::Add(MFPoint * newPoint) {
    pointList.Add(newPoint);
}

// MARK: MVDecoder

void MVDecoder::Decode(CString line, int & a, int & b) {
    std::string str;
    std::stringstream ss;
    line.Replace(TEXT(","), TEXT(" "));
    str = CT2A(line.GetBuffer());
    ss << str;
    ss >> a >> b;
}

void MVDecoder::Decode(CString line, double & a, double & b) {
    std::string str;
    std::stringstream ss;
    line.Replace(TEXT(","), TEXT(" "));
    str = CT2A(line.GetBuffer());
    ss << str;
    ss >> a >> b;
}

double MVDecoder::ToDouble(CString str) {
    std::string temp;
    std::stringstream ss;
    temp = CT2A(str.GetBuffer());
    ss << temp;
    double result;
    ss >> result;
    return result;
}
