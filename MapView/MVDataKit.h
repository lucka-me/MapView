#pragma once

// MARK: Enum

typedef enum _FeatureType {
    FT_NONE,
    FT_POINT,
    FT_POLYLINE,
    FT_POLYGON
} FeatureType;

// MARK: Class

class MapBound;
class MVDecoder;
class MFStyle;
class FeatureArray;
class MapFeature;
class MFPoint;
class MFPolyline;
class MFPolygon;

// 图廓类
class MapBound {
public:
    // 方法
    MapBound();
    ~MapBound() {};
    void SetMap(double left, double buttom, double right, double top);
    void SetDisplay(int left, int buttom, int right, int top);
    double MapWidth();
    double MapHeight();
    CPoint ConvertToDisplay(MFPoint point);
    CPoint ConvertToDisplay(double x, double y);
    CPoint * ConvertToDisplay(FeatureArray & pointList);
    MFPoint * ConvertToMap(CPoint point);    // 返回地图点的指针
    MFPoint * ConvertToMap(int x, int y);    // 返回地图点的指针

    // 属性
    double  mapLeft,     mapButtom,     mapRight,     mapTop;    // 数据的范围
    int displayLeft, displayButtom, displayRight, displayTop;    // 显示的范围
};

// 解码器
class MVDecoder {
public:
    void Decode(CString line, int & a, int & b);
    void Decode(CString line, double & a, double & b);
};

// 要素样式
class MFStyle {
public:
    COLORREF lineColor;
    COLORREF fillColor;
    unsigned short lineWidth;
    int penStyle;
};

// 要素类数组
class FeatureArray :
    public CTypedPtrArray<CObArray, MapFeature *> {
public:
    FeatureArray() {};
    ~FeatureArray() {};
};

// 要素类
class MapFeature :
    public CObject {
public:
    // 方法
    MapFeature() {};
    virtual ~MapFeature() {};
    virtual FeatureType GetType() { return FT_NONE; };
    virtual void Affine(double a1, double b1, double c1, double a2, double b2, double c2) {};
    virtual void Draw(CDC & dc, MapBound & bound, MFStyle style) {};
    virtual bool DidSelected(MFPoint & selectPoint, double buffer = 2) { return false; };
    
    // 属性
    int SN; // 序列号
    int id; // 要素分类ID
};

class MFPoint :
    public MapFeature {
public:
    MFPoint(double setX = 0.0, double setY = 0.0, int setId = 0, int setSN = 0);
    ~MFPoint() {};
    // 重写功能
    FeatureType GetType() { return FT_POINT; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound, MFStyle style);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 属性
    double x;
    double y;
};

class MFPolyline :
    public MapFeature {
public:
    MFPolyline(int setId = 0, int setSN = 0);
    ~MFPolyline() {};
    // 重写功能
    FeatureType GetType() { return FT_POLYLINE; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound, MFStyle style);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 新方法
    void Set(MFPoint * startPoint);
    void Add(MFPoint * newPoint);
    // 属性
    FeatureArray pointList;
};

class MFPolygon :
    public MapFeature {
public:
    MFPolygon(int setId = 0, int setSN = 0);
    ~MFPolygon() {};
    // 重写功能
    FeatureType GetType() { return FT_POLYLINE; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound, MFStyle style);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // 新方法
    void Set(MFPoint * startPoint);
    void Add(MFPoint * newPoint);
    // 属性
    FeatureArray pointList;
};