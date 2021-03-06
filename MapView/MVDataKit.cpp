﻿#include "stdafx.h"
#include "MVDataKit.h"
#include <sstream>
# define M_PI 3.14159265358979323846  /* pi */
// MARK: MapBound

MapBound::MapBound() {
        mapLeft = 0.0;   mapBottom = 0.0;   mapRight = 0.0;   mapTop = 0.0;
    displayLeft = 0; displayBottom = 0; displayRight = 0; displayTop = 0;
}

void MapBound::SetMap(double left, double bottom, double right, double top) {
    mapLeft   = left;
    mapBottom = bottom;
    mapRight  = right;
    mapTop    = top;
}

void MapBound::SetDisplay(int left, int bottom, int right, int top) {
    displayLeft   = left;
    displayBottom = bottom;
    displayRight  = right;
    displayTop    = top;
}

double MapBound::MapWidth() {
    return mapRight - mapLeft;
}

double MapBound::MapHeight() {
    return mapTop - mapBottom;
}

CPoint MapBound::ConvertToDisplay(MFPoint point) {
    return ConvertToDisplay(point.x, point.y);
}

CPoint MapBound::ConvertToDisplay(double x, double y) {
    int displayX = int(round(displayLeft + (y - mapLeft) / (mapRight - mapLeft) * double(displayRight - displayLeft)));
    int displayY = int(round(displayTop + (mapTop - x) / (mapTop - mapBottom) * double(displayBottom - displayTop)));
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
    double mapX = mapBottom + double(displayBottom - y) / double(displayBottom - displayTop) * (mapTop - mapBottom);
    double mapY = mapLeft + double(x - displayLeft) / double(displayRight - displayLeft) * (mapRight - mapLeft);
    return new MFPoint(mapX, mapY);
}

// MARK: FeatureArray

bool FeatureArray::Has(MapFeature * pFeature) {
    for (int i = 0; i < GetSize(); i++)
        if (GetAt(i) == pFeature)
            return true;
    
    return false;
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

void MFPoint::Draw(CDC & dc, MapBound & bound) {
    Draw(dc, bound, style);
}

void MFPoint::Draw(CDC & dc, MapBound & bound, MFStyle drawStyle) {
    CPoint displayPoint = bound.ConvertToDisplay(x, y);

    CBrush brush;
    brush.CreateSolidBrush(drawStyle.fillColor);
    CGdiObject *pOldBrush = dc.SelectObject(&brush);
    CPen pen;
    pen.CreatePen(drawStyle.bottomPenStyle, drawStyle.bottomLineWidth, drawStyle.bottomLineColor);
    CPen *pOldPen = dc.SelectObject(&pen);

    dc.Ellipse(displayPoint.x - 2, displayPoint.y - 2, displayPoint.x + 2, displayPoint.y + 2);

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
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

void MFPolyline::Draw(CDC & dc, MapBound & bound) {
    Draw(dc, bound, style);
}

void MFPolyline::Draw(CDC & dc, MapBound & bound, MFStyle drawStyle) {
    CPoint * pList = bound.ConvertToDisplay(pointList);

    CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
    CPen bottomPen;
    bottomPen.CreatePen(drawStyle.bottomPenStyle, drawStyle.bottomLineWidth, drawStyle.bottomLineColor);
    CPen *pOldPen = dc.SelectObject(&bottomPen);

    dc.Polyline(pList, pointList.GetSize());
    
    CPen topPen;
    topPen.CreatePen(drawStyle.topPenStyle, drawStyle.topLineWidth, drawStyle.topLineColor);
    dc.SelectObject(&topPen);
    //int drawMode = dc.SetROP2(R2_NOP);
    
    dc.Polyline(pList, pointList.GetSize());

    //dc.SetROP2(drawMode);
    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

bool MFPolyline::DidSelected(MFPoint & selectPoint, double buffer) {
    for (int i = 1; i < pointList.GetSize(); i++) {
        MFPoint * pointA = (MFPoint *)pointList[i - 1];
        MFPoint * pointB = (MFPoint *)pointList[i];
        double left   = pointA->y < pointB->y ? pointA->y : pointB->y;
        double bottom = pointA->x < pointB->x ? pointA->x : pointB->x;
        double right  = pointA->y > pointB->y ? pointA->y : pointB->y;
        double top    = pointA->x > pointB->x ? pointA->x : pointB->x;
        if ((selectPoint.y > left   - buffer) &&
            (selectPoint.x > bottom - buffer) &&
            (selectPoint.y < right  + buffer) &&
            (selectPoint.x < top    + buffer)) {
            double A = pointA->y - pointB->y;
            double B = pointB->x - pointA->x;
            double C = pointA->x * pointB->y - pointB->x * pointA->y;
            if (fabs((A * selectPoint.x + B * selectPoint.y + C) / sqrt(A * A + B * B)) <= buffer) {
                return true;
                break;
            }
        }
    }
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

void MFPolygon::Draw(CDC & dc, MapBound & bound) {
    Draw(dc, bound, style);
}

void MFPolygon::Draw(CDC & dc, MapBound & bound, MFStyle drawStyle) {
    CBrush brush;
    brush.CreateSolidBrush(drawStyle.fillColor);
    CGdiObject *pOldBrush = dc.SelectObject(&brush);
    CPen pen;
    pen.CreatePen(drawStyle.bottomPenStyle, drawStyle.bottomLineWidth, drawStyle.bottomLineColor);
    CPen *pOldPen = dc.SelectObject(&pen);

    CPoint * pList = bound.ConvertToDisplay(pointList);
    dc.Polygon(pList, pointList.GetSize());

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}

bool MFPolygon::DidSelected(MFPoint & selectPoint, double buffer) {
    // 先判断在不在边界上
    for (int i = 1; i < pointList.GetSize(); i++) {
        MFPoint * pointA = (MFPoint *)pointList[i - 1];
        MFPoint * pointB = (MFPoint *)pointList[i];
        double left = pointA->y < pointB->y ? pointA->y : pointB->y;
        double bottom = pointA->x < pointB->x ? pointA->x : pointB->x;
        double right = pointA->y > pointB->y ? pointA->y : pointB->y;
        double top = pointA->x > pointB->x ? pointA->x : pointB->x;
        if ((selectPoint.y > left - buffer) &&
            (selectPoint.x > bottom - buffer) &&
            (selectPoint.y < right + buffer) &&
            (selectPoint.x < top + buffer)) {
            double A = pointA->y - pointB->y;
            double B = pointB->x - pointA->x;
            double C = pointA->x * pointB->y - pointB->x * pointA->y;
            if (fabs((A * selectPoint.x + B * selectPoint.y + C) / sqrt(A * A + B * B)) <= buffer)
                return true;

        }
    }

    // 再判断在不在多边形内
    MFPoint * lastPoint = (MFPoint *)pointList[pointList.GetSize() - 1];
    MFPoint * firstPoint = (MFPoint *)pointList[0];
    double dxA = lastPoint->x - selectPoint.x;
    double dyA = lastPoint->y - selectPoint.y;
    double dxB = firstPoint->x - selectPoint.x;
    double dyB = firstPoint->y - selectPoint.y;
    double radianA = atan2(dxA, dyA);
    double radianB = atan2(dxB, dyB);
    double angle = radianB - radianA;
    if (angle > M_PI) {
        angle = angle - 2 * M_PI;
    } else if (angle < -M_PI) {
        angle = angle + 2 * M_PI;
    }
    double angleSum = angle;
    for (int i = 1; i < pointList.GetSize(); i++) {
        MFPoint * pointA = (MFPoint *)pointList[i - 1];
        MFPoint * pointB = (MFPoint *)pointList[i];
        dxA = pointA->x - selectPoint.x;
        dyA = pointA->y - selectPoint.y;
        dxB = pointB->x - selectPoint.x;
        dyB = pointB->y - selectPoint.y;
        radianA = atan2(dxA, dyA);
        radianB = atan2(dxB, dyB);
        angle = radianB - radianA;
        if (angle > M_PI) {
            angle = angle - 2 * M_PI;
        } else if (angle < -M_PI) {
            angle = angle + 2 * M_PI;
        }
        angleSum += angle;
    }
    
    return !(fabs(angleSum) < REAL_EPSILON);
}

void MFPolygon::Set(MFPoint * startPoint) {
    pointList.RemoveAll();
    Add(startPoint);
}

void MFPolygon::Add(MFPoint * newPoint) {
    pointList.Add(newPoint);
}

// MARK: GridIndex
void GridIndex::Set(double setResolution, MapBound & bound) {
    resolution = setResolution;
    row = int(bound.MapHeight() / resolution) + 1;
    col = int(bound.MapWidth()  / resolution) + 1;
    // 动态建立二维数组
    //   Reference: StackOverflow 936687
    index = new FeatureArray *[row];
    for (int i = 0; i < row; i++) {
        index[i] = new FeatureArray[col];
    }
}

// MARK: MVDecoder

void MVDecoder::Decode(CString line, int & a, int & b) {
    // 解码整数对数据
    std::string str;
    std::stringstream ss;
    line.Replace(TEXT(","), TEXT(" "));
    str = CT2A(line.GetBuffer());
    ss << str;
    ss >> a >> b;
}

void MVDecoder::Decode(CString line, double & a, double & b) {
    // 解码浮点数对数据
    std::string str;
    std::stringstream ss;
    line.Replace(TEXT(","), TEXT(" "));
    str = CT2A(line.GetBuffer());
    ss << str;
    ss >> a >> b;
}

double MVDecoder::ToDouble(CString str) {
    // 解码浮点数
    std::string temp;
    std::stringstream ss;
    temp = CT2A(str.GetBuffer());
    ss << temp;
    double result;
    if(!(ss >> result))
        result = 0.0;
    return result;
}

int MVDecoder::ToInt(CString str) {
    // 解码整数
    std::string temp;
    std::stringstream ss;
    temp = CT2A(str.GetBuffer());
    ss << temp;
    int result;
    if (!(ss >> result))
        result = 0;
    return result;
}
