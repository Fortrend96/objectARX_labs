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
	// функция получения таблицы блоков
	static AcDbBlockTable* GetPBlockTable()
	{
		AcDbBlockTable* pBlockTable = nullptr; // таблица блоков

		AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
		if (!pHostAppServices) {
			acutPrintf(L"\nНе удалось получить объект HostAppServices!");
			return pBlockTable;
		}


		AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();
		if (!pWorkingDatabase) {
			acutPrintf(L"\nНе удалось получить объект WorkingDatabase!");
			return pBlockTable;
		}

		// получаем таблицу блоков в режиме чтения
		if (pWorkingDatabase->getBlockTable(pBlockTable, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\nНе удалось открыть таблицу блоков!");
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
			|| acedGetPoint(NULL, L"Введите должность сотрудника: ", asDblArray(pt)) != RTNORM) 
			return;
		

		// получаем указатель на текущий чертеж и указатель на таблицу блоков чертежа. Открыть ее для чтения.
		AcDbBlockTable* pBlockTable = GetPBlockTable(); // таблица блоков
		if(pBlockTable)
		{
			AcDbBlockTableRecord* pSpaceRecord; // запись пространства модели
			if (pBlockTable->getAt(ACDB_MODEL_SPACE, pSpaceRecord, AcDb::kForWrite) == Acad::eOk) 
			{
				AcDbObjectPointer<AsdkEmployee> pEmployeeEntity;
				pEmployeeEntity.create();
				pEmployeeEntity->setID(id);
				pEmployeeEntity->setCube(cubeNumber);
				pEmployeeEntity->setFirstName(strFirstName);
				pEmployeeEntity->setLastName(strLastName);
				pEmployeeEntity->setCenter(pt);
				// добавляем сущность в пространство модели
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