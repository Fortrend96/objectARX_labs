#include "StdAfx.h"
#include "utilities.h"


Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId) 
{
	Acad::ErrorStatus errorStatus; // ������ ������
	AcDbLayerTable* pLayerTable; // ������� �����

	layerId = AcDbObjectId::kNull; // �������� ID ����������� ����

	// �������� ������� ����� � ������ ������
	errorStatus = acdbHostApplicationServices()->workingDatabase()->getLayerTable(pLayerTable, AcDb::kForRead);
	
	//��������� ������ ���������� �������
	if (errorStatus == Acad::eOk) 
	{
		errorStatus = pLayerTable->getAt(layerName, layerId, Adesk::kFalse); // �������� ID ����

		//��������� ������ ���������� �������
		if (errorStatus != Acad::eOk) 
		{
			// ������� ����� ����
			AcDbLayerTableRecord* pLayerTableRecord = new AcDbLayerTableRecord;
			pLayerTableRecord->setName(layerName); // ������������� ���
			
			errorStatus = pLayerTable->upgradeOpen(); // ��������� ���������� ������� �����
			if(errorStatus == Acad::eOk)
			{
				// ��������� ����� ���� � �������
				errorStatus = pLayerTable->add(layerId, pLayerTableRecord);
				pLayerTableRecord->close(); // ��������� ����� ����
			}
			else {
				// ������� ���� � ������, ���� �� ������� �������� ��� � �������
				delete pLayerTableRecord;
				pLayerTableRecord = nullptr;
			}
		}
		pLayerTable->close(); // ��������� ������� �����
	}
	return errorStatus;
}

Acad::ErrorStatus createBlockRecord(const TCHAR* name) 
{
	AcDbBlockTable* pBlockTable; //  ������� ������
	Acad::ErrorStatus errorStatus; // ������ ������

	// ��������� ������� ������ � ������ �����
	errorStatus = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	// ��������� ������� ����� � ����� ������
	if (pBlockTable->has(name) == Adesk::kTrue) 
	{
		pBlockTable->close();
		return (Acad::eDuplicateKey);
	}

	// ������� ����� ����
	AcDbBlockTableRecord* pBlockTableRecord = new AcDbBlockTableRecord;
	pBlockTableRecord->setName(name); // ������ ���
	pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin); // ������ ������ � ������������ ���������
	
	// ��������� ���������� ������� ������
	errorStatus = pBlockTable->upgradeOpen();
	if (errorStatus != Acad::eOk) 
	{
		delete pBlockTableRecord;
		pBlockTable->close();
		return errorStatus;
	}

	// ��������� ����� ���� � �������
	errorStatus = pBlockTable->add(pBlockTableRecord);
	if (errorStatus != Acad::eOk) 
	{	
		// ������� ����� ���� � ��������� ������� ������ � ������ ������� ����������
		pBlockTable->close();
		delete pBlockTableRecord;
		pBlockTableRecord = nullptr;
		return errorStatus;
	}
	pBlockTable->close();

	const double PI = 3.141592;

	// ������� ��������, ������� ����� ������� � ���� � ������������ ������	
	AcDbCircle* pFace = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 1.0);
	AcDbCircle* pLeftEye = new AcDbCircle(AcGePoint3d(0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbCircle* pRightEye = new AcDbCircle(AcGePoint3d(-0.33, 0.25, 0.0), AcGeVector3d::kZAxis, 0.1);
	AcDbArc* pMouth = new AcDbArc(AcGePoint3d(0, 0.5, 0), 1.0, PI + (PI * 0.3), PI + (PI * 0.7));

	// ������ �������� ���������
	pFace->setColorIndex(2);
	pLeftEye->setColorIndex(5);
	pRightEye->setColorIndex(5);
	pMouth->setColorIndex(1);
	
	// ��������� �������� � ����
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
