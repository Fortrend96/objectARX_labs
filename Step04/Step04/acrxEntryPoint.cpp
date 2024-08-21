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
#include <dbxrecrd.h>
#include <memory>

//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep04App : public AcRxArxApp {

private:
	static AcDbDictionary* getPNOD()
	{
		AcDbDictionary* pNOD = nullptr;
		
		AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
		if (!pHostAppServices) return pNOD;

		AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();
		if (!pWorkingDatabase) return pNOD;

		// �������� ������� ����������� �������� 		
		if (pWorkingDatabase->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk)
		{
			acutPrintf(L"�� ������� ������� ������� ����������� ��������.");
			return pNOD;
		}

		return pNOD;
	}

	

public:
	CStep04App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

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

	// ----- AsdkStep04._ADDENTRY command (do not rename)
	static void AsdkStep04_ADDENTRY(void)
	{
		/*
		�������� ������� 
		1 ��������� � ������������ ��� ���������� (acedGetString()).
		2 �������� ������� ����������� �������� �� ������� ������� ���� ������ (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		3 ���������, ���� �� ��� "ASDK_EMPLOYEE_DICTIONARY" � NOD (AcDbDictionary::getAt()).
		4 ���� ������� ������� "ASDK_EMPLOYEE_DICTIONARY" ����������� � NOD, ������� ����� AcDbDictionary � ������ "ASDK_EMPLOYEE_DICTIONARY" � �������� ��� � ������� ����������� �������� (new AcDbDictionary, AcDbDictionary::setAt()).
		5 ���������, ���� �� ��� ��� ���������� � ������� "ASDK_EMPLOYEE_DICTIONARY".
		6 ���� ������� ����������� �����������, �������� ����� ������ AcDbXrecord � �������� �� � "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::setAt()).
		7 �� �������� ������� ������� ����������� ��������, "ASDK_EMPLOYEE_DICTIONARY" � ������ Xrecord, ���� ��� ���� �������.
		*/

		TCHAR employeeStrID[133];

		if (acedGetString(0, L"������� ��� ����������: ", employeeStrID) != RTNORM)
			return;

		// �������� ������� ����������� �������� 
		AcDbDictionary* pNOD = getPNOD();
		if (!pNOD) return;

		// ��������� ������� ������� �����������
		AcDbObjectId employeeDictID; // ID ������� �����������
		AcDbDictionary* pEmployeeDict = nullptr;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound)
		{			
			if (pNOD->upgradeOpen() != Acad::eOk) 
			{
				acutPrintf(L"\n�� ������� ������� ������� ����������� �������� � ������ ������!");
				pNOD->close();
				return;
			}
			pEmployeeDict = new AcDbDictionary; // �������  �������
			// ��������� ������� ����������� � ������� ����������� ��������
			if (pNOD->setAt(L"ASDK_EMPLOYEE_DICTIONARY", pEmployeeDict, employeeDictID) != Acad::eOk) 
			{
				acutPrintf(L"\n�� ������� �������� ������� ����������� � ������� ����������� �������� AutoCAD!");
				delete pEmployeeDict;
				pEmployeeDict = nullptr;
				pNOD->close();
				return;
			}
		}
		else {
			AcDbObject* pEmplyeeDictObject; // ��� �������
			if (acdbOpenAcDbObject(pEmplyeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk)
			{
				acutPrintf(L"\n�� ������� ������� ������ � ������ ������.");
				pNOD->close();
				return;
			}
			// �������� �� ������� ������ � ��� �� ������, ��� � � ������� �����������.
			if ((pEmployeeDict = AcDbDictionary::cast(pEmplyeeDictObject)) == nullptr)
			{
				acutPrintf(L"\n������� ������ �  ������� ����������� ��������, �� ��� �� ������� �����������.");
				pEmplyeeDictObject->close();
				pNOD->close();
				return;
			}
		}
		pNOD->close();
		// ��������� �� ������� ������ � ������ ������
		if (pEmployeeDict->getAt(employeeStrID, employeeDictID) == Acad::eOk) {
			acutPrintf(L"\n������ ��������� ��� ���������������.");
			pEmployeeDict->close();
			return;
		}
		// ��������� ����� ������ � ����������
		AcDbObjectPointer<AcDbXrecord> pEmployeeEntry;
		pEmployeeEntry.create();
		if (pEmployeeDict->setAt(employeeStrID, pEmployeeEntry, employeeDictID) != Acad::eOk)
		{
			acutPrintf(L"\n�� ������� �������� ������ � ���������� � �������..");
			pEmployeeDict->close();
			return;
		}
		//pEmployeeEntry->close();
		pEmployeeDict->close();
	}

	// ----- AsdkStep04._LISTENTRIES command (do not rename)
	static void AsdkStep04_LISTENTRIES(void)
	{
		/*
		�������� �������
		1. �������� ������� ����������� �������� �� ������� ������� ���� ������ (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		2. �������� ������� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
		3. ��������� �������� �� "ASDK_EMPLOYEE_DICTIONARY" � �������� ����� ������� (����� �����������).
			1. �������� ����� �������� (AcDbDictionary::newIterator(), AcDbDictionaryIterator).
			2. ��������� �������� �� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionaryIterator::done(), AcDbDictionaryIterator::next()).
			3. ������������ ���� ������� (AcDbDictionaryIterator::name()).
		4. ������� �������� � �� �������� ������� �������� �������.
		*/

		// �������� ������� ����������� �������� 
		AcDbDictionary* pNOD = getPNOD();
		if (!pNOD) return;

		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) != Acad::eOk) 
		{
			acutPrintf(L"\n������� ����������� �����������.");
			pNOD->close();
			return;
		}

		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(L"\n�� ������� ������� ������� ��� ������.");
			pNOD->close();
			return;
		}

		// �������� �� ������� ������ � ��� �� ������, ��� � � ������� �����������.
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == nullptr) 
		{
			acutPrintf(L"������� ������ �  ������� ����������� ��������, �� ��� �� ������� �����������.");
			pEmployeeDictObject->close();
			pNOD->close();
			return;
		}
		pNOD->close();

		std::unique_ptr<AcDbDictionaryIterator> pEmpDictIter(pEmployeeDict->newIterator()); // �������� ��� ���������� �� �������

		if (pEmpDictIter)
		{
			while (!pEmpDictIter->done())
			{
				acutPrintf(L"���������: %s.\n", pEmpDictIter->name()); // ����� ����� ����������
				pEmpDictIter->next();
			}
		}
		pEmployeeDict->close();
	}


	// ----- AsdkStep04._REMOVEENTRY command (do not rename)
	static void AsdkStep04_REMOVEENTRY(void)
	{
		/*
		�������� �������
		1. �������� ��� ���������� �� ������������ (acedGetString()) ��� ��������.
		2. �������� ������� ����������� �������� �� ������� ������� ���� ������ (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		3. �������� ������� "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
		4. �������� ������ AcDbXrecord � ��������� ������ ����������.
		���� ������ ���������� ����������, �������� �� ��� ������, � ����� ������� (AcDbObject::erase()).
		*/

		// ����������� � ������������ ��� ����������
		TCHAR employeeStrID[133];
		if (acedGetString(0, L"������� ��� ����������: ", employeeStrID) != RTNORM)
			return;

		// �������� ������� ����������� �������� 
		AcDbDictionary* pNOD = getPNOD();
		if (!pNOD) return;

		// ��������� ������� ������� �����������
		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) != Acad::eOk)
		{
			acutPrintf(L"\n������� ����������� �����������.");
			pNOD->close();
			return;
		}

		// ��������� ������� ����������� � ������ ������
		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk)
		{
			acutPrintf(L"\n�� ������� ������� ������� ��� ������.");
			pNOD->close();
			return;
		}

		// �������� �� ������� ������ � ��� �� ������, ��� � � ������� �����������.
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
		{
			acutPrintf(L"������� ������ �  ������� ����������� ��������, �� ��� �� ������� �����������.");
			pEmployeeDictObject->close();
			pNOD->close();
			return;
		}
		pNOD->close();

		AcDbObjectId employeeObjectID;
		// ��������� ������� ������ � ���������� � �������
		if (pEmployeeDict->getAt(employeeStrID, employeeObjectID) != Acad::eOk)
		{
			acutPrintf(L"\n������ �� �������.");
			pEmployeeDict->close();
			return;
		}
		pEmployeeDict->close();
		
		AcDbObject* pEmployeeObject;
		// ��������� ������ � ����������
		if (acdbOpenAcDbObject(pEmployeeObject, employeeObjectID, AcDb::kForWrite) != Acad::eOk) {
			acutPrintf(L"\nEntry cannot be opened for write.");
			return;
		}
		// ������� ������
		pEmployeeObject->erase();
		pEmployeeObject->close();
	}
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void ASMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM ) {		}
		else	{		}
	}

	static void ASMyGroupMySessionCmd () {	}

	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep04App, MyLispFunction, false)
	static int ads_MyLispFunction () {
		return (RTNORM) ;
	}	
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep04App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, nullptr)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep04App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _ADDENTRY, ADDENTRY, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _LISTENTRIES, LISTENTRIES, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _REMOVEENTRY, REMOVEENTRY, ACRX_CMD_TRANSPARENT, nullptr)





