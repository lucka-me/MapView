#pragma once

// MARK: Enum

typedef enum _FeatureType {
    FT_NONE,
    FT_POINT,
    FT_POLYLINE,
    FT_POLYGON
} FeatureType;

/*
typedef enum _FeatureID {
    FID_NONE = 0,
    FID_RESIDENCE = 70002,
} FeatureID;
*/

// MARK: Class

class MapBound;
class FeatureArray;
class MapFeature;
class MFPoint;
class MFPolyline;
class MFPolygon;

class MapBound {
public:
    // ����
    MapBound();
    ~MapBound() {};
    void SetMap(double left, double buttom, double right, double top);
    void SetDisplay(int left, int buttom, int right, int top);
    CPoint ConvertToDisplay(MFPoint point);
    CPoint ConvertToDisplay(double x, double y);
    CPoint * ConvertToDisplay(FeatureArray & pointList);
    MFPoint * ConvertToMap(CPoint point);    // ���ص�ͼ���ָ��
    MFPoint * ConvertToMap(int x, int y);    // ���ص�ͼ���ָ��

    // ����
    double  mapLeft,     mapButtom,     mapRight,     mapTop;    // ���ݵķ�Χ
    int displayLeft, displayButtom, displayRight, displayTop;    // ��ʾ�ķ�Χ
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
    // ����
    MapFeature() {};
    virtual ~MapFeature() {};
    virtual FeatureType GetType() { return FT_NONE; };
    virtual void Draw(CDC & dc, MapBound & bound, COLORREF color) {};
    virtual bool DidSelected(MFPoint & selectPoint, double buffer = 2) { return false; };
    
    // ����
    int id;
};

class MFPoint :
    public MapFeature {
public:
    MFPoint(double setX = 0.0, double setY = 0.0, int setId = 0);
    ~MFPoint() {};
    // ��д����
    FeatureType GetType() { return FT_POINT; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // ����
    double x;
    double y;
};

class MFPolyline :
    public MapFeature {
public:
    MFPolyline(int setId = 0);
    ~MFPolyline() {};
    // ��д����
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // �·���
    void Set(MFPoint * startPoint);
    void Add(MFPoint * newPoint);
    // ����
    FeatureArray pointList;
};

class MFPolygon :
    public MapFeature {
public:
    MFPolygon(int setId = 0);
    ~MFPolygon() {};
    // ��д����
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(MFPoint & selectPoint, double buffer = 2);
    // �·���
    void Set(MFPoint * startPoint);
    void Add(MFPoint * newPoint);
    // ����
    FeatureArray pointList;
};

// ������
class MVDecoder {
public:
    void Decode(CString line, int & a, int & b);
    void Decode(CString line, double & a, double & b);
};