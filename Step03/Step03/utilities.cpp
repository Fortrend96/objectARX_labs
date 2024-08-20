#include "StdAfx.h"
#include "utilities.h"


Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId) 
{
	Acad::ErrorStatus errorStatus; // статус ошибки
	AcDbLayerTable* pLayerTable; // таблица слоев

	layerId = AcDbObjectId::kNull; // зануляем ID переданного слоя

	// получаем таблицу слоев в режиме чтения
	errorStatus = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	
	//проверяем статус выполнения функции
	if (errorStatus == Acad::eOk) 
	{
		errorStatus = pLayerTable->getAt(layerName, layerId, Adesk::kFalse); // получаем ID слоя

		//проверяем статус выполнения функции
		if (errorStatus != Acad::eOk) 
		{
			// Создаем новый слой
			AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
			pLayerTableRecord->setName(layerName); // устанавливаем имя
			
			errorStatus = pLayerTable->upgradeOpen(); // запускаем обновление таблицы слоев
			if(errorStatus == Acad::eOk)
			{
				// добавляем новый слой в таблицу
				errorStatus = pLayerTable->add(layerId, pLayerTableRecord);
				pLayerTableRecord->close(); // закрываем новый слой
			}
			else {
				// удаляем слой в случае, если не удалось добавить его в таблицу
				delete pLayerTableRecord;
				pLayerTableRecord = nullptr;
			}
		}
		pLayerTable->close(); // закрываем таблицу слоев
	}
	return errorStatus;
}

Acad::ErrorStatus createBlockRecord(const TCHAR* name) 
{
	AcDbBlockTable* pBlockTable; //  таблица блоков
	Acad::ErrorStatus errorStatus; // статус ошибки

	// открываем таблицу блоков в режиме чтени
	errorStatus = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	// проверяем наличие блока с таким именем
	if (pBlockTable->has(name) == Adesk::kTrue) 
	{
		pBlockTable->close();
		return (Acad::eDuplicateKey);
	}

	// создаем новый блок
	AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord;
	pBlockTableRecord->setName(name); // задаем имя
	pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin); // задаем начало в координатной плоскости
	
	// запускаем обновление таблицы блоков
	errorStatus = pBlockTable->upgradeOpen();
	if (errorStatus != Acad::eOk) 
	{
		delete pBlockTableRecord;
		pBlockTable->close();
		return errorStatus;
	}

	// добавляем новый блок в таблицу
	errorStatus = pBlockTable->add(pBlockTableRecord);
	if (errorStatus != Acad::eOk) 
	{	
		// удаляем новый блок и закрываем таблицу блоков в случае провала добавления
		pBlockTable->close();
		delete pBlockTableRecord;
		pBlockTableRecord = nullptr;
		return errorStatus;
	}
	pBlockTable->close();

	const double PI = 3.141592;

	// создаем элементы, которые будут входить в блок и образовывать улыбку	
	AcDbCircle* pFace = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0);
	AcDbCircle* pLeftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbCircle* pRightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbArc* pMouth = new AcDbArc(AcGePoint3d(0, 0.5, 0), 1.0, PI + (PI * 0.3), PI + (PI * 0.7));

	// задаем свойства элементов
	pFace->setColorIndex(2);
	pLeftEye->setColorIndex(5);
	pRightEye->setColorIndex(5);
	pMouth->setColorIndex(1);
	
	// добавляем элементы в блок
	errorStatus = pBlockTableRecord->appendAcDbEntity(pFace);
	if (errorStatus != Acad::eOk) 
	{
		delete pFace; pFace = nullptr;
		delete pLeftEye; pLeftEye = nullptr;
		delete pRightEye; pRightEye = nullptr;
		delete pMouth; pMouth = nullptr;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return errorStatus;
	}
	pFace->close();

	errorStatus = pBlockTableRecord->appendAcDbEntity(pLeftEye);
	if (errorStatus != Acad::eOk) 
	{
		delete pLeftEye; pLeftEye = nullptr;
		delete pRightEye; pRightEye = nullptr;
		delete pMouth; pMouth = nullptr;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return errorStatus;
	}
	pLeftEye->close();

	errorStatus = pBlockTableRecord->appendAcDbEntity(pRightEye);
	if (errorStatus != Acad::eOk) 
	{
		delete pRightEye; pRightEye = nullptr;
		delete pMouth; pMouth = nullptr;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return errorStatus;
	}
	pRightEye->close();

	errorStatus = pBlockTableRecord->appendAcDbEntity(pMouth);
	if (errorStatus != Acad::eOk) {
		delete pMouth; pMouth = nullptr;
		pBlockTableRecord->erase();
		pBlockTableRecord->close();
		return errorStatus;
	}
	pMouth->close();

	pBlockTableRecord->close();
	return Acad::eOk;
}
