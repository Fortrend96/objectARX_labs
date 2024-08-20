#pragma once

#include "StdAfx.h"

// Функция для создания нового слоя
Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId);
// Функция для создания нового блока
Acad::ErrorStatus createBlockRecord(const TCHAR* name);