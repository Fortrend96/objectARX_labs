#pragma once

#include "StdAfx.h"

// �㭪�� ��� ᮧ����� ������ ᫮�
Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId);



// �㭪�� ��� ᮧ����� ��� ���
AcDbObjectPointer<AcDbCircle> createFace();
// �㭪�� ��� ᮧ����� �����
AcDbObjectPointer<AcDbCircle> createEye(const AcGePoint3d&& point);
// �㭪�� ��� ᮧ����� ��
AcDbObjectPointer<AcDbArc> createMouth();

// �㭪�� ���������� ����� � ���� � �஢�ઠ��
template<typename T>
Acad::ErrorStatus addElementToBlockRecord(AcDbObjectPointer<T>& pElement, 
				AcDbObjectPointer<AcDbBlockTableRecord>& pBlockTableRecord);


// �㭪�� ��� ᮧ����� ������ �����
Acad::ErrorStatus createBlockRecord(const TCHAR* name);