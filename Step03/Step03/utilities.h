#pragma once

#include "StdAfx.h"

// Функция для создания нового слоя
Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId);



// функция для создания формы лица
AcDbObjectPointer<AcDbCircle> createFace();
// функция для создания глаза
AcDbObjectPointer<AcDbCircle> createEye(const AcGePoint3d&& point);
// функция для создания рта
AcDbObjectPointer<AcDbArc> createMouth();

// функция добавления элемента в блок с проверками
template<typename T>
Acad::ErrorStatus addElementToBlockRecord(AcDbObjectPointer<T>& pElement, 
				AcDbObjectPointer<AcDbBlockTableRecord>& pBlockTableRecord);


// Функция для создания нового блока
Acad::ErrorStatus createBlockRecord(const TCHAR* name);