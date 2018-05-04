#pragma once

// MARK: Enum

typedef enum _FeatureType {
    FT_NONE,
    FT_POINT,
    FT_POLYLINE,
    FT_POLYGON
} FeatureType;

typedef enum _FeatureID {
    FID_RESIDENCE = 70002
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
    // ����
    MapBound();
    ~MapBound() {};
    CPoint convert(MFPoint point);
    MFPoint convert(CPoint point);

    // ����
    double     mapLeft,     mapButtom,     mapRight,     mapTop;    // ���ݵķ�Χ
    double displayLeft, displayButtom, displayRight, displayTop;    // ��ʾ�ķ�Χ
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
    virtual bool DidSelected(unsigned short buffer = 2) { return false; };
    
    // ����
    FeatureID id;
};

class MFPoint :
    public MapFeature {
public:
    MFPoint();
    ~MFPoint() {};
    // ��д����
    FeatureType GetType() { return FT_POINT; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(unsigned short buffer = 2);
    // ����
    double x;
    double y;
};

class MFPolyline :
    public MapFeature {
public:
    MFPolyline();
    ~MFPolyline() {};
    // ��д����
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(unsigned short buffer = 2);
    // �·���
    void Set(MFPoint startPoint);
    void Append(MFPoint newPoint);
    // ����
    FeatureArray pointList;
};

class MFPolygon :
    public MapFeature {
public:
    MFPolygon();
    ~MFPolygon() {};
    // ��д����
    FeatureType GetType() { return FT_POLYLINE; };
    void Draw(CDC & dc, MapBound & bound, COLORREF color);
    bool DidSelected(unsigned short buffer = 2);
    // �·���
    void Set(MFPoint startPoint);
    void Append(MFPoint newPoint);
    // ����
    FeatureArray pointList;
};