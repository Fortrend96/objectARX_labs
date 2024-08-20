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
#include "AsdkEdEmployeeReactor.h"
#include "AsdkEmployeeReactor.h"
#include "utilities.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

AsdkEdEmployeeReactor* pEdEmployeeReactor = NULL;
AsdkEmployeeReactor* pEmployeeReactor = NULL;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep07App : public AcRxArxApp {

public:
	CStep07App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		pEdEmployeeReactor = new AsdkEdEmployeeReactor(true);
		pEmployeeReactor = new AsdkEmployeeReactor();


		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		delete pEdEmployeeReactor;
		pEdEmployeeReactor = nullptr;

		delete pEmployeeReactor;
		pEmployeeReactor = nullptr;

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static void AsdkStep07_CREATEEMPLOYEE(void)
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
			|| acedGetPoint(NULL, L"������� ��������� ����������: ", asDblArray(pt)) != RTNORM
			) 
			return;
		

		// �������� ��������� �� ������� ������ � ��������� �� ������� ������ �������. ������� �� ��� ������.
		AcDbBlockTable* pBlockTable; // ������� ������
		if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) == Acad::eOk) 
		{
			AcDbBlockTableRecord* pSpaceRecord; // ������ ������������ ������
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) 
			{
				AsdkEmployee* pemployeeEntity = new AsdkEmployee;
				pemployeeEntity->setID(id);
				pemployeeEntity->setCube(cubeNumber);
				pemployeeEntity->setFirstName(strFirstName);
				pemployeeEntity->setLastName(strLastName);
				pemployeeEntity->setCenter(pt);
				// ��������� �������� � ������������ ������
				AcDbObjectId idObj;
				if (pSpaceRecord->appendAcDbEntity(idObj, pemployeeEntity) == Acad::eOk)
					pemployeeEntity->close();
				else
					delete pemployeeEntity;
				pSpaceRecord->close();
			}
			pBlockTable->close();
		}
	}

	static void AsdkStep07_CREATE(void)
	{
		AcDbObjectId layerId;
		if (createLayer(L"USER", layerId) != Acad::eOk) {
			acutPrintf(L"\n�� ������� ������� ����.");
			return;
		}

		applyCurDwgLayerTableChanges();

		acutPrintf(L"\nLayer USER successfully created.");

		// Create a new block definition named "EMPLOYEE"
		if (createBlockRecord(L"EMPLOYEE") != Acad::eOk)
			acutPrintf(L"\n�� ������� ������� ����.");
		else
			acutPrintf(L"\n���� ������.");
	}
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CStep06App class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
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

	static void ASMyGroupMySessionCmd () {
		// Put your command code here
	}

	
	static int ads_MyLispFunction () {
		return (RTNORM) ;
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg(void* pkt)
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kLoadDwgMsg(pkt);
		DocVars.docData().m_pAsdkDbEmployeeReactor = new AsdkDbEmployeeReactor(acdbHostApplicationServices()->workingDatabase());
		return (retCode);
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep07App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep07App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, AsdkStep07, _CREATEEMPLOYEE, CREATEEMPLOYEE, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep07App, AsdkStep07, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, NULL)