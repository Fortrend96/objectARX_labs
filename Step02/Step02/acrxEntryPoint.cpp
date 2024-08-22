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

//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep02App : public AcRxArxApp {

public:
	CStep02App () : AcRxArxApp () {}

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

	static void AsdkStep02_myInput(void) 
	{
		int intValue; // вводимое целое число
		double dblValue = 12.0;; // вводимое вещественное число

		int inputResult; // результат ввода
		
		ACHAR keyWord[120]; // ключевое слово для ввода вещественных чисел

		acedInitGet(RSG_NONEG | RSG_NOZERO, L"Число должно быть больше нуля"); // функция контроля интеравктивного ввода, в первом аргументе запрещаем ввод отрицательного и нулевого значений

		if ((inputResult = acedGetInt(L"\nВведите целое число: ", &intValue)) != RTNORM)
			return;

		acedInitGet(RSG_NOZERO, L"PI A B C"); // запрет на ввод нуля
		inputResult = acedGetReal(L"\nВведите вещественное число, либо [PI/A/B/C]: ", &dblValue);
		switch (inputResult) {
		case RTCAN: 
			return;
		case RTKWORD:
			if (acedGetInput(keyWord) != RTNORM)
				return;			
			if (!wcscmp(L"A",  keyWord))
				dblValue = 10.0;
			else if (!wcscmp(L"B",  keyWord))
				dblValue = 11.0;
			else if (!wcscmp(L"C", keyWord))
				dblValue = 12.0;
			else if (!wcscmp(L"PI", keyWord))
				dblValue = 3.14;
			break;
		case RTNONE:
			acutPrintf(L"\nПринять вещественное значение по умолчанию %f", dblValue);
			break;
		case RTNORM:
			break;
		default:
			return;
		}
		acutPrintf(L"\nЦелое значение: %d", intValue);
		acutPrintf(L"\nВещественное значение: %f", dblValue);
	}

	// ----- AsdkStep02._mySelect command (do not rename)
	static void AsdkStep02_mySelect(void) 
	{		
		ads_name entity;
		ads_point point;

		if (acedEntSel(NULL, entity, point) != RTNORM)
			return;

		acutPrintf(L"\nID выбранной сущности: %d", entity[0]);
	}

	static void ASMyGroupMyCommand () {	}

	static void ASMyGroupMyPickFirst () 
	{
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM ){}
		else {}
	}

	static void ASMyGroupMySessionCmd () {	}

	static int ads_MyLispFunction () 
	{
		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep02App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, nullptr)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep02App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, AsdkStep02, _mySelect, mySelect, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep02App, AsdkStep02, _myInput, myInput, ACRX_CMD_TRANSPARENT, nullptr)


