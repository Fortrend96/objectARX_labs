// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "utilities.h"
#include "..\EmployeeDetails\AsdkEmployeeDetails.h" 
#include <memory>


//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep05App : 
	public AcRxArxApp 
{
private:
	// ������� ��������� ������� ������
	static AcDbBlockTable* GetPBlockTable()
	{
		AcDbBlockTable* pBlockTable = nullptr; // ������� ������

		AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
		if (!pHostAppServices) {
			acutPrintf(L"\n�� ������� �������� ������ HostAppServices!");
			return pBlockTable;
		}
			

		AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();
		if (!pWorkingDatabase) { 
			acutPrintf(L"\n�� ������� �������� ������ WorkingDatabase!");
			return pBlockTable; 
		}

		// �������� ������� ������ � ������ ������
		if (pWorkingDatabase->getBlockTable(pBlockTable, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� ������� ������!");
			return pBlockTable;
		}

		return pBlockTable;
	}


public:
	CStep05App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		AcRxObject* pSvc;
		if (!(pSvc = acrxServiceDictionary->at(ASDKEMPLOYEEDETAILS_DBXSERVICE)))
		{
			// Try to load the module, if it is not yet present 
			if (!acrxDynamicLinker->loadModule(L"AsdkEmployeeDetails.dbx", 0))
			{
				acutPrintf(L"���������� ��������� AsdkEmployeeDetails.dbx.\n");
				return AcRx::kRetError;
			}
		}

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void AsdkStep05_CREATE(void)
	{
		// ������� ���� ��� ��������� "USER"
		AcDbObjectId layerId;
		if (createLayer(L"USER", layerId) != Acad::eOk)
		{
			acutPrintf(L"\n�� ������� ������� ����.");
			return;
		}

		// ��� �������������� ���������� ���� � ����� ������ �������
		applyCurDwgLayerTableChanges();

		acutPrintf(L"\n���� USER ������.");

		// ������� ����� ���� ��� ��������� "EMPLOYEE"		
		createBlockRecord(L"EMPLOYEE") != Acad::eOk ?
			acutPrintf(L"\n�� ������� ������� ����� ����.") :
			acutPrintf(L"\n���� EMPLOYEE ������.");
	}

	static void AsdkStep05_SETLAYER(void)
	{
		Acad::ErrorStatus errorStatus; // ������ ������
		AcDbBlockTableRecord* pBlockTableRecord; // ������ � ������� ������

		// �������� ������� ������ � ������ ������
		AcDbBlockTable* pBlockTable = GetPBlockTable();// ������� ������

		if (!pBlockTable) return;	

		// �������� ������������ ������ � ������ ������
		errorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		
		if (errorStatus != Acad::eOk)
		{
			acutPrintf(L"\n�� ������� �������� ������������ ������.\n");
			pBlockTable->close();
			return;
		}

		pBlockTable->close();

		// �������� ��� ����������� ����������� ������������ ������
		AcDbBlockTableRecordIterator* pIter; 
		errorStatus = pBlockTableRecord->newIterator(pIter);

		if (errorStatus != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� �������� ������������ ������.");
			pBlockTableRecord->close();
			return;
		}

		// ���������� �������� � ����� ���������
		std::unique_ptr<AcDbBlockTableRecordIterator> pBlockTableRecordIter(pIter); 

		TCHAR* blockName; // ��� �����
		AcDbEntity* pEntity; // ��������
		AcDbBlockTableRecord* pCurEntityBlock; // ������� ����
		AcDbObjectId blockId; // ID �����

		for (pBlockTableRecordIter->start(); !pBlockTableRecordIter->done(); pBlockTableRecordIter->step())
		{
			// ��������� ����� ��������
			errorStatus = pBlockTableRecordIter->getEntity(pEntity, AcDb::kForRead); // �������� �������� ����� � ������ ������
			if (errorStatus != Acad::eOk) {
				acutPrintf(L"\n�� ������� �������� ��������.");
				continue;
			}

			if (pEntity->isA() != AcDbBlockReference::desc()) {
				pEntity->close();
				continue;
			}

			// ������ � ��������� ������
			AcDbBlockReference* pBlockRef = nullptr;
			if ((pBlockRef = AcDbBlockReference::cast(pEntity)) == nullptr)
			{
				pEntity->close();
				continue;
			}

			// �������� ID �����
			blockId = pBlockRef->blockTableRecord();

			if (acdbOpenObject((AcDbObject*&)pCurEntityBlock, blockId, AcDb::kForRead) == Acad::eOk)
			{
				pCurEntityBlock->getName(blockName);
				if (wcscmp(blockName, L"EMPLOYEE") == 0)
				{
					if (pEntity->upgradeOpen() == Acad::eOk)
						pEntity->setLayer(L"USER"); // ������������� ����
				}
				pCurEntityBlock->close();
				acdbFree(blockName);
			}
			pEntity->close();
		}

		pBlockTableRecord->close();
	}

	// ----- AsdkStep05._LISTDETAILS command (do not rename)
	static void AsdkStep05_LISTDETAILS(void)
	{
		/*
		�������� �������
		1. ��������� ������������ ������� ������ �� ���� (acedEntSel()).
			a. ������������ ads_name � AcDbObjectId (acdbGetObjectId()).
			b. �������� ������ (acdbOpenAcDbObject()).
			c. ��������� � ������� ������� isKindOf(), ��� ������������ ������ ������ �� ���� (AcDbBlockReference::desc()).
			d. �������, ���� ��� �� ������ �� ����.
		2. ��������� ������� ���������� ������ �� ����. ���� ��� ���, �������. (AcDbObject::extensionDictionary()).
		3. ��������� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()). ���� ��� ���, �������.
		4. ��������� ������ AsdkEmployeeDetails �� �ASDK_EMPLOYEE_DICTIONARY� � ������� �DETAILS�.
		5. ��������� ������ ������� AsdkEmployeeDetails � ������������ ��������.		
		*/

		ads_name employeeName;
		ads_point pt;
	
		if (acedEntSel(L"�������� ����������: ", employeeName, pt) != RTNORM)
			return;
		
		AcDbObjectId employeeID;
		if (acdbGetObjectId(employeeID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;
		if (acdbOpenAcDbObject(pEmployeeObject, employeeID, AcDb::kForRead) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) 
		{
			acutPrintf(L"\n�� �������� ������� �� ����.");
			pEmployeeObject->close();
			return;
		}
		
		AcDbObjectId extDictID;
		if ((extDictID = pEmployeeObject->extensionDictionary()) == AcDbObjectId::kNull) 
		{
			pEmployeeObject->close();
			return;
		}
		pEmployeeObject->close();
		
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, extDictID, AcDb::kForRead, Adesk::kFalse) != Acad::eOk) 
		{
			acutPrintf(L"\n�� ������� ������� ������� ����������.");
			return;
		}
		
		AcDbDictionary* pEmployeeDict;
		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;

		if (pExtDict->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound) 
		{
			pExtDict->close();
			return;
		}
		else 
		{
			// ��������� ������� � ������ ������, ���� �� ��� ����������
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk)
			{
				pExtDict->close();
				acutPrintf(L"\n�� ������� ������� ������� ����������.");
				return;
			}
			
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL) {
				pEmployeeDictObject->close();
				pExtDict->close();
				acutPrintf(L"\n������ �� �������� ��������.");
				return;
			}
		}
		
		AcDbObjectId detailsID;
		if (pEmployeeDict->getAt(L"DETAILS", detailsID) != Acad::eOk) 
		{
			pEmployeeDict->close();
			pExtDict->close();
			return;
		}
		
		AcDbObject* pDetailObject;
		if (acdbOpenAcDbObject(pDetailObject, detailsID, AcDb::kForRead) != Acad::eOk)
		{
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(L"\n�� ������� ������� ������ Detail.");
			return;
		}
		
		AsdkEmployeeDetails* pEmployeeDetails = AsdkEmployeeDetails::cast(pDetailObject);
		if (!pEmployeeDetails) 
		{
			acutPrintf(L"\n�� ������� ����� ������ EmployeeDetails.");
			pDetailObject->close();
			pEmployeeDict->close();
			pExtDict->close();
			return;
		}
		// ����������� ����������
		Adesk::Int32 i;
		pEmployeeDetails->iD(i);
		acutPrintf(L"ID ����������: %d\n", i);
		pEmployeeDetails->cube(i);
		acutPrintf(L"����� ������� ����������: %d\n", i);
		TCHAR* st = nullptr;
		pEmployeeDetails->firstName(st);
		acutPrintf(L"��� ����������: %s\n", st);
		delete[] st;
		pEmployeeDetails->lastName(st);
		acutPrintf(L"������� ����������: %s\n", st);
		delete[] st;

		pDetailObject->close();
		pEmployeeDict->close();
		pExtDict->close();
	}


	// ----- AsdkStep05._REMOVEDETAIL command (do not rename)
	static void AsdkStep05_REMOVEDETAIL(void)
	{
		/*
		�������� �������
		1. ��������� ������������ ������� ������ �� ���� (acedEntSel()).
			a. ������������ ads_name � AcDbObjectId (acdbGetObjectId()).
			b. �������� ������ (acdbOpenAcDbObject()).
			c. ��������� � ������� ������� isKindOf(), ��� ������������ ������ ������ �� ���� (AcDbBlockReference::desc()).
			d. �������, ���� ��� �� ������ �� ����.
		2. ��������� ������� ���������� ������ �� ����. ���� ��� ���, �������. (AcDbObject::extensionDictionary()).
		3. ��������� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()). ���� ��� ���, �������.
		4. ��������� ������ AsdkEmployeeDetails �� "ASDK_EMPLOYEE_DICTIONARY" � ������� "DETAILS".
		5. ������� ������ AsdkEmployeeDetails (AcDbObject::erase()).
		6. ������� �������, ���� � ��� ������ ��� �������. (AcDbDictionary::numEntries())		
		*/

		ads_name employeeName;
		ads_point pt;
		
		if (acedEntSel(L"�������� ����������: ", employeeName, pt) != RTNORM)
			return;

		AcDbObjectId employeeObjectID;
		if (acdbGetObjectId(employeeObjectID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;

		if (acdbOpenAcDbObject(pEmployeeObject, employeeObjectID, AcDb::kForRead) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) {
			acutPrintf(L"\n�� �������� ������� �� ����.");
			pEmployeeObject->close();
			return;
		}

		// �������� ������� ����������
		AcDbObjectId extDicdID;
		if ((extDicdID = pEmployeeObject->extensionDictionary()) == AcDbObjectId::kNull) 
		{
			pEmployeeObject->close();
			return;
		}
		pEmployeeObject->close(); // ���������
		
		AcDbDictionary* pExtDict; // ������� ����������
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, extDicdID, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk) 
		{
			acutPrintf(L"\n�� ������� ������� ������� ����������.");
			return;
		}

		// ��������� ������� ������� ����������� 
		AcDbDictionary* pEmployeeDict; // ������� ����������
		AcDbObjectId employeeDictID; // id ������� ����������
		AcDbObject* pEmployeeDictObject; 
		if (pExtDict->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound) 
		{
			pExtDict->close();
			return;
		}
		else {
			// ��������� ������� � ������ ������
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk)
			{
				pExtDict->close();
				acutPrintf(L"\n�� ������� ������� ������� ����������.");
				return;
			}
			// �������� ������� �������
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
			{
				pEmployeeDictObject->close();
				pExtDict->close();
				acutPrintf(L"\n������ �� �������� ��������");
				return;
			}
		}
		// ��������� ������� ������ � ����� ������
		if (pEmployeeDict->getAt(L"DETAILS", employeeDictID) != Acad::eOk) 
		{
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(L"\n��� ������ � ����� ������.");
			return;
		}

		// ��������� ������ � ������ ������
		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk) 
		{
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(L"\n�� ������� ������� ������.");
			return;
		}

		pEmployeeDictObject->erase();
		pEmployeeDictObject->close();
		
		if (pEmployeeDict->numEntries() == 0)
			pEmployeeDict->erase();
		pEmployeeDict->close();
		
		if (pExtDict->numEntries() == 0)
			pExtDict->erase();
		pExtDict->close();
	}

	// ----- AsdkStep05._ADDDETAIL command (do not rename)
	static void AsdkStep05_ADDDETAIL(void)
	{
		/*
		�������� �������:
			1. ��������� ������������ ������� ������ �� ����(acedEntSel()).
			a.������������ ads_name � AcDbObjectId(acdbGetObjectId()).
			b.�������� ������(acdbOpenAcDbObject()).
			c.��������� � ������� ������� isKindOf(), ��� ������������ ������ ������ �� ����(AcDbBlockReference::desc())
			d.�������, ���� ��� �� ������ �� ����.
			2. ������� ������ � ���������� ��� �������������� ����������; ���� ����������; ����� ����������; ������� ����������(acedGetInt(), acedGetString()).
			3. �������� ������� ���������� ������ �� ���� EMPLOYEE(AcDbObject::extensionDictionary()).
			4. ���� ������ �� ���� EMPLOYEE �� ����� ������� ����������, �������� ���(AcDbObject::createExtensionDictionary()).
			5. �������� ������� ����������.���� ������� ���������� ��� ����� �����, �� �������� ������� ������� ����������, ��� ��� �� ����� �������� ������������ ������� ����������.
			6. ���� ������� ���������� ��� �����(AcDbObject::isErased()) ������������ ��� ���������(AcDbObject::erase(Adesk::kFalse)).
			7. ��������� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
			���� AcDbDictionary "ASDK_EMLOYEE_DICTIONARY" �� ����������, �������� AcDbDictionary "ASDK_EMPLOYEE_DICTIONARY" � �������� ��� � ������� ���������� ������ �� ���� EMPLOYEE.
			8. ���������, ������������ �� ������ AsdkEmployeeDetails � "ASDK_EMPLOYEE_DICTIONARY" (�� ������ ������� AsdkEmployeeDetails ��� ������ "DETAILS").
			a.���� ������ AsdkEmployeeDetails �� ����������, �������� ����� ������ AsdkEmployeeDetails � ������� ��� ������.
			b.�������� ����� ������ AsdkEmployeeDetails ��� ������ "DETAILS" � "ASDK_EMPLOYEE_DICTIONARY"
		*/

		ads_name employeeName; // ��� ����������
		ads_point pt; // �����

		// �������� ������ �� ������������ 
		if (acedEntSel(L"�������� ����������: ", employeeName, pt) != RTNORM)
			return;

		// �������� ������ � ����������
		AcDbObjectId employeeID;
		if (acdbGetObjectId(employeeID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;
		if (acdbOpenAcDbObject(pEmployeeObject, employeeID, AcDb::kForWrite) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) 
		{
			acutPrintf(L"\n������ �� �������� ������� �� ����.");
			pEmployeeObject->close();
			return;
		}

		// �������� ������ �� ������������
		int id, cubeNumber;
		TCHAR strFirstName[133];
		TCHAR strLastName[133];
		if (acedGetInt(L"������� ID ����������: ", &id) != RTNORM
			|| acedGetInt(L"������� ���������� �����: ", &cubeNumber) != RTNORM
			|| acedGetString(0, L"������� ��� ����������: ", strFirstName) != RTNORM
			|| acedGetString(0, L"������� ������� ����������: ", strLastName) != RTNORM) 
		{
			pEmployeeObject->close();
			return;
		}
		// �������� ������� ����������
		AcDbObjectId extDictID; // id ������� ����������
		if ((extDictID = pEmployeeObject->extensionDictionary()) == AcDbObjectId::kNull)
		{
			if (pEmployeeObject->createExtensionDictionary() != Acad::eOk) 
			{
				pEmployeeObject->close();
				acutPrintf(L"\n�� ������� ������� ������� ����������.");
				return;
			}

			extDictID = pEmployeeObject->extensionDictionary();
		}

		// ��������� ������
		pEmployeeObject->close();

		// ��������� ����������� �������
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, extDictID, AcDb::kForWrite, Adesk::kTrue) != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� �������.");
			return;
		}

		// ��������������� ����������� �������, ���� �� ��� �����
		if (pExtDict->isErased())
			pExtDict->erase(Adesk::kFalse);

		// ��������� ������� �������
		AcDbDictionary* pEmployeeDict;
		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pExtDict->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound)
		{
			// ������� �������
			pEmployeeDict = new AcDbDictionary;
			Acad::ErrorStatus errorStatus = pExtDict->setAt(L"ASDK_EMPLOYEE_DICTIONARY", pEmployeeDict, employeeDictID);

			if (errorStatus != Acad::eOk) 
			{
				pExtDict->close();
				acutPrintf(L"\n�� ������� ������� ������� ����������.");
				return;
			}
		}
		else {
			// ��������� ������� ����������
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk) 
			{
				pExtDict->close();
				acutPrintf(L"\n�� ������� ������� �������.");
				return;
			}
			// ���������, �� ������ �� ���-�� ������ � ����� ������ ������� �� �������� ��������.
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
			{
				pEmployeeObject->close();
				pExtDict->close();
				acutPrintf(L"\n�� �������� ��������.");
				return;
			}
		}
		// ��������� ������� 
		pExtDict->close();
		AcDbObjectId detailsID;
		// ��������� ������� ������ � ������ ������
		if (pEmployeeDict->getAt(L"DETAILS", detailsID) == Acad::eOk) {
			pEmployeeDict->close();
			acutPrintf(L"\nDetails ��� ���� � ������� \"���������\".");
			return;
		}
		// ������� EmployeeDetails
		AcDbObjectPointer<AsdkEmployeeDetails> pEmployeeDetails;
		pEmployeeDetails.create();
		pEmployeeDetails->setID(id);
		pEmployeeDetails->setCube(cubeNumber);
		pEmployeeDetails->setFirstName(strFirstName);
		pEmployeeDetails->setLastName(strLastName);
		// ��������� EmployeeDetails � �������
		if (pEmployeeDict->setAt(L"DETAILS", pEmployeeDetails, detailsID) != Acad::eOk) 
		{
			acutPrintf(L"\n�� ������� �������� DETAILS � ������ \"���������\".");
			pEmployeeDict->close();
			return;
		}
		acutPrintf(L"\nDetails ������� ��������!");
		pEmployeeDict->close();
		pEmployeeDetails->close();
	}

	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CStep05App class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void ASMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ASMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CStep05App class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep05App, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep05App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, nullptr)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep05App, MyLispFunction, false)


ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _SETLAYER, SETLAYER, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _LISTDETAILS, LISTDETAILS, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _REMOVEDETAIL, REMOVEDETAIL, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep05App, AsdkStep05, _ADDDETAIL, ADDDETAIL, ACRX_CMD_TRANSPARENT, nullptr)