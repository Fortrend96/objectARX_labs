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
		
		1. Введите данные о сотруднике для идентификатора сотрудника: 
			кабинки сотрудника; 
			имени сотрудника; 
			фамилии сотрудника (acedGetInt(), acedGetString()) и должности сотрудника (acedGetPoint()).
			ПРИМЕЧАНИЕ. Функция acedGetPoint() возвращает ads_point, представляющую точку, выбранную пользователем. 
			Чтобы преобразовать ads_point непосредственно в точку AcGePoint3d, используйте глобальную функцию asDblArray(). 
			Для получения дополнительной информации об asDblArray() обратитесь к онлайн-справке ObjectARX.
		2. Создайте новый экземпляр сущности AsdkEmployee.
		3. Задайте данные AsdkEmployee.
		4. Добавьте сущность AsdkEmployee в запись пространства модели.		
		*/


		int id;
		int cubeNumber;
		const int STR_LENGHT = 133;
		TCHAR strFirstName[STR_LENGHT];
		TCHAR strLastName[STR_LENGHT];
		AcGePoint3d pt;
		// Ввод данных пользователем
		if (acedGetInt(L"Введите ID сотрудника: ", &id) != RTNORM
			|| acedGetInt(L"Введите номер кабинки: ", &cubeNumber) != RTNORM
			|| acedGetString(0, L"Введите имя сотрудника: ", strFirstName) != RTNORM
			|| acedGetString(0, L"Введите фамилию сотрудника: ", strLastName) != RTNORM
			|| acedGetPoint(NULL, L"Введите должность сотрудника: ", asDblArray(pt)) != RTNORM
			) 
			return;
		

		// получаем указатель на текущий чертеж и указатель на таблицу блоков чертежа. Открыть ее для чтения.
		AcDbBlockTable* pBlockTable; // таблица блоков
		if (acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead) == Acad::eOk) 
		{
			AcDbBlockTableRecord* pSpaceRecord; // запись пространства модели
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) 
			{
				AsdkEmployee* pemployeeEntity = new AsdkEmployee;
				pemployeeEntity->setID(id);
				pemployeeEntity->setCube(cubeNumber);
				pemployeeEntity->setFirstName(strFirstName);
				pemployeeEntity->setLastName(strLastName);
				pemployeeEntity->setCenter(pt);
				// добавляем сущность в пространство модели
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

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ASMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CStep06App class.
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
	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep06App, MyLispFunction, false)
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
IMPLEMENT_ARX_ENTRYPOINT(CStep06App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep06App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep06App, AsdkStep06, _CREATEEMPLOYEE, CREATEEMPLOYEE, ACRX_CMD_TRANSPARENT, NULL)