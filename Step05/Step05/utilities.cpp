#include "StdAfx.h"
#include "utilities.h"


Acad::ErrorStatus createLayer(const TCHAR* layerName, AcDbObjectId& layerId) 
{
	Acad::ErrorStatus errorStatus; // ������ ������
	AcDbLayerTable* pLayerTable; // ������� �����

	layerId = AcDbObjectId::kNull; // �������� ID ����������� ����

	errorStatus = Acad::eNullObjectPointer; // ������ �������� ���������

	AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
	if (!pHostAppServices)
		return errorStatus;

	AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();
	if (!pWorkingDatabase)
		return errorStatus;

	// �������� ������� ����� � ������ ������
	errorStatus = pWorkingDatabase->getLayerTable(pLayerTable, AcDb::kForRead);
	//��������� ������ ���������� �������
	if (errorStatus != Acad::eOk)
		return errorStatus;

	errorStatus = pLayerTable->getAt(layerName, layerId, Adesk::kFalse); // �������� ID ����

	//��������� ������ ���������� �������
	if (errorStatus != Acad::eOk) 
	{
		// ������� ����� ����
		AcDbObjectPointer<AcDbLayerTableRecord> pLayerTableRecord;
		pLayerTableRecord.create();
		pLayerTableRecord->setName(layerName); // ������������� ���
			
		errorStatus = pLayerTable->upgradeOpen(); // ��������� ���������� ������� �����
		if(errorStatus == Acad::eOk)
		{
			// ��������� ����� ���� � �������
			errorStatus = pLayerTable->add(layerId, pLayerTableRecord);
			pLayerTableRecord->close(); // ��������� ����� ����
		}
	}
	pLayerTable->close(); // ��������� ������� �����	
	return errorStatus;
}
//-------------------------------------------------------------------------------------------
AcDbObjectPointer<AcDbCircle> createFace()
{
	AcDbObjectPointer<AcDbCircle> pFace;

	if (pFace.create() != Acad::eOk)
		return nullptr;

	pFace->setCenter(AcGePoint3d::kOrigin);
	pFace->setNormal(AcGeVector3d::kZAxis);
	pFace->setRadius(1.0);
	pFace->setColorIndex(2);

	return pFace;
}
//-------------------------------------------------------------------------------------------
AcDbObjectPointer<AcDbCircle> createEye(const AcGePoint3d&& point)
{
	AcDbObjectPointer<AcDbCircle> pEye;

	if (pEye.create() != Acad::eOk)
		return nullptr;

	pEye.create();
	pEye->setCenter(point);
	pEye->setNormal(AcGeVector3d::kZAxis);
	pEye->setRadius(0.1);
	pEye->setColorIndex(5);

	return pEye;
}
//-------------------------------------------------------------------------------------------
AcDbObjectPointer<AcDbArc> createMouth()
{
	AcDbObjectPointer<AcDbArc> pMouth;

	if (pMouth.create() != Acad::eOk)
		return nullptr;

	const double PI = 3.14;

	pMouth->setCenter(AcGePoint3d(0.0, 0.5, 0));
	pMouth->setRadius(1.0);
	pMouth->setStartAngle(PI + (PI * 0.3));
	pMouth->setEndAngle(PI + (PI * 0.7));
	pMouth->setColorIndex(1);

	return pMouth;
}
//-------------------------------------------------------------------------------------------
template<typename T>
Acad::ErrorStatus addElementToBlockRecord(AcDbObjectPointer<T>& pElement,
					AcDbObjectPointer<AcDbBlockTableRecord>& pBlockTableRecord)
{
	Acad::ErrorStatus errorStatus = Acad::eNullEntityPointer;

	if (pElement && pBlockTableRecord)
	{
		errorStatus = pBlockTableRecord->appendAcDbEntity(pElement);
		if (errorStatus != Acad::eOk)
		{
			pBlockTableRecord->erase();
			pBlockTableRecord->close();
			return errorStatus;
		}
		pElement->close();
	}
	return errorStatus;
}
//-------------------------------------------------------------------------------------------
Acad::ErrorStatus createBlockRecord(const TCHAR* name) 
{
	AcDbBlockTable* pBlockTable; //  ������� ������
	Acad::ErrorStatus errorStatus; // ������ ������

	errorStatus = Acad::eNullObjectPointer; // ������ �������� ���������

	AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
	if (!pHostAppServices)
		return errorStatus;

	AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();
	if (!pWorkingDatabase)
		return errorStatus;

	// ��������� ������� ������ � ������ �����
	errorStatus = pWorkingDatabase->getBlockTable(pBlockTable, AcDb::kForRead);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	// ��������� ������� ����� � ����� ������
	if (pBlockTable->has(name) == Adesk::kTrue) 
	{
		pBlockTable->close();
		return (Acad::eDuplicateKey);
	}

	// ������� ����� ����
	AcDbObjectPointer<AcDbBlockTableRecord> pBlockTableRecord;
	pBlockTableRecord.create();
	pBlockTableRecord->setName(name); // ������ ���
	pBlockTableRecord->setOrigin(AcGePoint3d::kOrigin); // ������ ������ � ������������ ���������
	
	// ��������� ���������� ������� ������
	errorStatus = pBlockTable->upgradeOpen();
	if (errorStatus != Acad::eOk) 
	{
		pBlockTable->close();
		return errorStatus;
	}

	// ��������� ����� ���� � �������
	errorStatus = pBlockTable->add(pBlockTableRecord);
	if (errorStatus != Acad::eOk) 
	{	
		// ������� ����� ���� � ��������� ������� ������ � ������ ������� ����������
		pBlockTable->close();
		return errorStatus;
	}
	pBlockTable->close();

	// ������� ��������, ������� ����� ������� � ���� � ������������ ������, � ��������� � ����
	AcDbObjectPointer<AcDbCircle> pFace = createFace();
	
	errorStatus = addElementToBlockRecord(pFace, pBlockTableRecord);
	
	if (errorStatus != Acad::eOk)
		return errorStatus;
	
	AcDbObjectPointer<AcDbCircle> pLeftEye = createEye(AcGePoint3d(0.33, 0.25, 0.0));
	
	errorStatus = addElementToBlockRecord(pLeftEye, pBlockTableRecord);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	AcDbObjectPointer<AcDbCircle> pRightEye = createEye(AcGePoint3d(-0.33, 0.25, 0.0));

	errorStatus = addElementToBlockRecord(pRightEye, pBlockTableRecord);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	AcDbObjectPointer<AcDbArc> pMouth = createMouth();

	errorStatus = addElementToBlockRecord(pMouth, pBlockTableRecord);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	//pBlockTableRecord->close();
	return Acad::eOk;
}
