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
#include "..\Employee\AsdkEmployee.h" 

//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep06App : public AcRxArxApp {

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
	CStep06App () : AcRxArxApp () {}

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

	static void AsdkStep06_CREATEEMPLOYEE(void)
	{
		/*	
		1. ������� ������ � ���������� ��� �������������� ����������: 
			������� ����������; 
			����� ����������; 
			������� ���������� (acedGetInt(), acedGetString()) � ��������� ���������� (acedGetPoint()).
			����������. ������� acedGetPoint() ���������� ads_point, �������������� �����, ��������� �������������. 
			����� ������������� ads_point ��������������� � ����� AcGePoint3d, ����������� ���������� ������� asDblArray(). 
			��� ��������� �������������� ���������� �� asDblArray() ���������� � ������-������� ObjectARX.
		2. �������� ����� ��������� �������� AsdkEmployee.
		3. ������� ������ AsdkEmployee.
		4. �������� �������� AsdkEmployee � ������ ������������ ������.		
		*/


		int id;
		int cubeNumber;
		const int STR_LENGHT = 133;
		TCHAR strFirstName[STR_LENGHT];
		TCHAR strLastName[STR_LENGHT];
		AcGePoint3d pt;
		// ���� ������ �������������
		if (acedGetInt(L"������� ID ����������: ", &id) != RTNORM
			|| acedGetInt(L"������� ����� �������: ", &cubeNumber) != RTNORM
			|| acedGetString(0, L"������� ��� ����������: ", strFirstName) != RTNORM
			|| acedGetString(0, L"������� ������� ����������: ", strLastName) != RTNORM
			|| acedGetPoint(NULL, L"������� ��������� ����������: ", asDblArray(pt)) != RTNORM) 
			return;
		

		// �������� ��������� �� ������� ������ � ��������� �� ������� ������ �������. ������� �� ��� ������.
		AcDbBlockTable* pBlockTable = GetPBlockTable(); // ������� ������
		if(pBlockTable)
		{
			AcDbBlockTableRecord* pSpaceRecord; // ������ ������������ ������
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) 
			{
				AcDbObjectPointer<AsdkEmployee> pEmployeeEntity;
				pEmployeeEntity.create();
				pEmployeeEntity->setID(id);
				pEmployeeEntity->setCube(cubeNumber);
				pEmployeeEntity->setFirstName(strFirstName);
				pEmployeeEntity->setLastName(strLastName);
				pEmployeeEntity->setCenter(pt);
				// ��������� �������� � ������������ ������
				AcDbObjectId idObj;
				if (pSpaceRecord->appendAcDbEntity(idObj, pEmployeeEntity) == Acad::eOk)
					pEmployeeEntity->close();
				
				pSpaceRecord->close();
			}
			pBlockTable->close();
		}
	}
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void ASMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if (iRet == RTNORM) {}
		else {}
		
	}

	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ASMyGroupMySessionCmd () {
		// Put your command code here
	}

	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep06App, MyLispFunction, false)
	static int ads_MyLispFunction () {

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep06App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, nullptr)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep06App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, AsdkStep06, _CREATEEMPLOYEE, CREATEEMPLOYEE, ACRX_CMD_TRANSPARENT, nullptr)