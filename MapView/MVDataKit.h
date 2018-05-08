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
class MFStyle;
class FeatureArray;
class MapFeature;
class MFPoint;
class MFPolyline;
class MFPolygon;
class GridIndex;
class MVDecoder;

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

// 要素样式
class MFStyle {
public:
    COLORREF lineColor = RGB(0, 0, 0);
    COLORREF fillColor = RGB(255, 255, 255);
    unsigned short lineWidth = 1;
    int penStyle = PS_SOLID;
};

// 要素类数组
class FeatureArray :
    public CTypedPtrArray<CObArray, MapFeature *> {
public:
    FeatureArray() {};
    ~FeatureArray() {};
    bool Has(MapFeature * pFeature);
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
    virtual void Draw(CDC & dc, MapBound & bound) {};
    virtual void Draw(CDC & dc, MapBound & bound, MFStyle drawStyle) {};
    virtual bool DidSelected(MFPoint & selectPoint, double buffer = 0.2) { return false; };
    
    // 属性
    int SN; // 序列号
    int id; // 要素分类ID
    MFStyle style;  // 显示样式
};

class MFPoint :
    public MapFeature {
public:
    MFPoint(double setX = 0.0, double setY = 0.0, int setId = 0, int setSN = 0);
    ~MFPoint() {};
    // 重写方法
    FeatureType GetType() { return FT_POINT; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound);
    void Draw(CDC & dc, MapBound & bound, MFStyle drawStyle);
    bool DidSelected(MFPoint & selectPoint, double buffer = 0.2);
    // 属性
    double x;
    double y;
};

class MFPolyline :
    public MapFeature {
public:
    MFPolyline(int setId = 0, int setSN = 0);
    ~MFPolyline() {};
    // 重写方法
    FeatureType GetType() { return FT_POLYLINE; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound);
    void Draw(CDC & dc, MapBound & bound, MFStyle drawStyle);
    bool DidSelected(MFPoint & selectPoint, double buffer = 0.2);
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
    // 重写方法
    FeatureType GetType() { return FT_POLYGON; };
    void Affine(double a1, double b1, double c1, double a2, double b2, double c2);
    void Draw(CDC & dc, MapBound & bound);
    void Draw(CDC & dc, MapBound & bound, MFStyle drawStyle);
    bool DidSelected(MFPoint & selectPoint, double buffer = 0.2);
    // 新方法
    void Set(MFPoint * startPoint);
    void Add(MFPoint * newPoint);
    // 属性
    FeatureArray pointList;
};

// 网格索引
class GridIndex {
public:
    // 方法
    GridIndex() {};
    ~GridIndex() {};
    void Set(double setResolution, MapBound & bound);
    // 属性
    double resolution = 5.0;
    int row = 0;
    int col = 0;
    FeatureArray ** index;
};


// 解码器
class MVDecoder {
public:
    void Decode(CString line, int & a, int & b);
    void Decode(CString line, double & a, double & b);
    double ToDouble(CString str);
    int ToInt(CString str);
};