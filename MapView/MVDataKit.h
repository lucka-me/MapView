#pragma once

// MARK: Enum

typedef enum _FeatureType {
    FT_NONE,
    FT_POINT,
    FT_POLYLINE,
    FT_POLYGON
} FeatureType;

typedef enum _FeatureID {
    FID_NONE = 0,
    FID_RESIDENCE = 70002,
} FeatureID;

// MARK: Class

class MapBound;
class FeatureArray;
class MapFeature;
class MFPoint;
class MFPolyline;
class MFPolygon;

class MapBound {
public:
    // 方法
    MapBound();
    ~MapBound() {};
    void SetMap(double left, double buttom, double right, double top);
    void SetDisplay(int left, int buttom, int right, int top);
    CPoint ConvertToDisplay(MFPoint point);
    CPoint ConvertToDisplay(double x, double y);
    CPoint * ConvertToDisplay(FeatureArray & pointList);
    MFPoint * ConvertToMap(CPoint point);    // 返回地图点的指针
    MFPoint * ConvertToMap(int x, int y);    // 返回地图点的指针

    // 属性
    double  mapLeft,     mapButtom,     mapRight,     mapTop;    // 数据的范围
    int displayLeft, displayButtom, displayRight, displayTop;    // 显示的范围
};

class FeatureArray :
    public CTypedPtrArray<CObArray, MapFeature *> {
public:
    FeatureArray() {};
    ~FeatureArray() {};
};

class MapFeature :
    public CObject {
public:
    // 方法
    MapFeature() {};
    virtual ~MapFeature() {};
    virtual FeatureType GetType() { return FT_NONE; };
    virtual void Draw(CDC & dc, MapBound & bound, COLORREF color) {};
    virtual bool DidSelected(MFPoint & selectPoint, double buffer = 2) { return false; };
    
    // 属性
    FeatureID id;
};

class MFPoint :
    public MapFeature {
public:
    MFPoint(double setX = 0.0, double setY = 0.0, FeatureID setId = FID_NONE);
    ~MFPoint() {};
    // 重写功能
    FeatureType GetType() { return FT_POINT; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 属性
    double x;
    double y;
};

class MFPolyline :
    public MapFeature {
public:
    MFPolyline(FeatureID setId = FID_NONE);
    ~MFPolyline() {};
    // 重写功能
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 新方法
    void Set(MFPoint * startPoint);
    void Append(MFPoint * newPoint);
    // 属性
    FeatureArray pointList;
};

class MFPolygon :
    public MapFeature {
public:
    MFPolygon(FeatureID setId = FID_NONE);
    ~MFPolygon() {};
    // 重写功能
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 新方法
    void Set(MFPoint * startPoint);
    void Append(MFPoint * newPoint);
    // 属性
    FeatureArray pointList;
};