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

//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep04App : public AcRxArxApp {

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

		// �������� ������� ����������� �������� Get the named object dictionary
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(L"�� ������� ������� ������� ����������� ��������.");
			return;
		}

		// ��������� ������� ������� �����������
		AcDbObjectId employeeDictID; // ID ������� �����������
		AcDbDictionary* pEmployeeDict = NULL;
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
			if ((pEmployeeDict = AcDbDictionary::cast(pEmplyeeDictObject)) == NULL)
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
		AcDbXrecord* pEmployeeEntry = new AcDbXrecord;
		if (pEmployeeDict->setAt(employeeStrID, pEmployeeEntry, employeeDictID) != Acad::eOk)
		{
			acutPrintf(L"\n�� ������� �������� ������ � ���������� � �������..");
			delete pEmployeeEntry;
			pEmployeeEntry = nullptr;
			pEmployeeDict->close();
			return;
		}
		pEmployeeEntry->close();
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

		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� ������� ����������� ��������.");
			return;
		}

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
		if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL) 
		{
			acutPrintf(L"������� ������ �  ������� ����������� ��������, �� ��� �� ������� �����������.");
			pEmployeeDictObject->close();
			pNOD->close();
			return;
		}
		pNOD->close();

		AcDbDictionaryIterator* pEmpDictIter = pEmployeeDict->newIterator(); // �������� ��� ���������� �� �������

		if (pEmpDictIter)
		{
			while (!pEmpDictIter->done())
			{
				acutPrintf(L"���������: %s.\n", pEmpDictIter->name()); // ����� ����� ����������
				pEmpDictIter->next();
			}

			delete pEmpDictIter;
			pEmpDictIter = nullptr;
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

		// �������� ������� ������������ ��������
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� ������� ����������� ��������.");
			return;
		}

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
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CStep04App class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
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
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ASMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CStep04App class.
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
	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep04App, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}	
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep04App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep04App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _ADDENTRY, ADDENTRY, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _LISTENTRIES, LISTENTRIES, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep04App, AsdkStep04, _REMOVEENTRY, REMOVEENTRY, ACRX_CMD_TRANSPARENT, NULL)





