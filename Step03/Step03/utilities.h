#pragma once

#include "StdAfx.h"

// ������� ��� �������� ������ ����
Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId);



// ������� ��� �������� ����� ����
AcDbObjectPointer<AcDbCircle> createFace();
// ������� ��� �������� �����
AcDbObjectPointer<AcDbCircle> createEye(const AcGePoint3d&& point);
// ������� ��� �������� ���
AcDbObjectPointer<AcDbArc> createMouth();

// ������� ���������� �������� � ���� � ����������
template<typename T>
Acad::ErrorStatus addElementToBlockRecord(AcDbObjectPointer<T>& pElement, 
				AcDbObjectPointer<AcDbBlockTableRecord>& pBlockTableRecord);


// ������� ��� �������� ������ �����
Acad::ErrorStatus createBlockRecord(const TCHAR* name);