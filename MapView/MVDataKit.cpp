#include "stdafx.h"
#include "MVDataKit.h"

// MARK: MapBound

MapBound::MapBound() {
}

// MARK: MapFeature

// MARK: MFPoint

MFPoint::MFPoint() {
}

void MFPoint::Draw(CDC & dc, MapBound & bound, COLORREF color) {
}

bool MFPoint::DidSelected(unsigned short buffer) {
    return false;
}

// MARK: MFPolyline

MFPolyline::MFPolyline() {
}

void MFPolyline::Draw(CDC & dc, MapBound & bound, COLORREF color) {
}

bool MFPolyline::DidSelected(unsigned short buffer) {
    return false;
}

void MFPolyline::Set(MFPoint startPoint) {
}

void MFPolyline::Append(MFPoint newPoint) {
}

// MARK: MFPolygon

MFPolygon::MFPolygon() {
}

void MFPolygon::Draw(CDC & dc, MapBound & bound, COLORREF color) {
}

bool MFPolygon::DidSelected(unsigned short buffer) {
    return false;
}

void MFPolygon::Set(MFPoint startPoint) {
}

void MFPolygon::Append(MFPoint newPoint) {
}
