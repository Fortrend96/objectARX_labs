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
#include <memory>

//-----------------------------------------------------------------------------
#define szRDS _RXST("AS")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CStep03App : public AcRxArxApp {

private:


public:
	CStep03App () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {	}

	static void AsdkStep03_CREATE(void)
	{
		// Создаем слой под названием "USER"
		AcDbObjectId layerId;
		if (createLayer(L"USER", layerId) != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось создать слой.");
			return;
		}

		// для синхрониззации созданного слоя с базой данных чертежа
		applyCurDwgLayerTableChanges();

		acutPrintf(L"\nСлой USER создан.");

		// создаем новый блок под названием "EMPLOYEE"		
		createBlockRecord(L"EMPLOYEE") != Acad::eOk ?
			acutPrintf(L"\nНе удалось создать новый блок.") :
			acutPrintf(L"\nБлок EMPLOYEE создан.");
	}

	static void AsdkStep03_SETLAYER(void)
	{
		Acad::ErrorStatus errorStatus; // статус ошибки
		AcDbBlockTable* pBlockTable; // таблица блоков
		AcDbBlockTableRecord* pBlockTableRecord; // запись в таблице блоков

		AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();
		
		if (!pHostAppServices)
		{
			acutPrintf(L"\nНе удалось получить доступ к объекту HostAppServices.");
			return;
		}

		AcDbDatabase* pWorkingDataBase = pHostAppServices->workingDatabase();

		if (!pWorkingDataBase)
		{
			acutPrintf(L"\nНе удалось получить доступ к объекту WorkingDataBase.");
			return;
		}

		// получаем таблицу блоков в режиме чтения
		errorStatus = pWorkingDataBase->getBlockTable(pBlockTable, AcDb::kForRead);

		if (errorStatus != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось открыть таблицу блоков!");
			return;
		}

		// Получаем пространство модели в режиме чтения
		errorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		if (errorStatus != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось получить пространство модели.\n");
			pBlockTable->close();
			return;
		}
		pBlockTable->close();

		AcDbBlockTableRecordIterator* pIter; // итератор для прохождения содержимого пространства модели
		errorStatus = pBlockTableRecord->newIterator(pIter);

		if (errorStatus != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось создать итератор пространства модели.");
			pBlockTableRecord->close();
			return;
		}

		// уникальный указатель для автоматический очистики памяти после выполнения функции
		std::unique_ptr<AcDbBlockTableRecordIterator> pBlockTableIterator(pIter);

		TCHAR* blockName; // имя блока
		AcDbEntity* pEntity; // сущность
		AcDbBlockTableRecord* pCurEntityBlock; // текущий блок
		AcDbObjectId blockId; // ID блока

		for (pBlockTableIterator->start(); !pBlockTableIterator->done(); pBlockTableIterator->step())
		{
			// проверяем кажду сущность
			errorStatus = pBlockTableIterator->getEntity(pEntity, AcDb::kForRead); // получаем сущность блока в режиме чтения
			
			if (errorStatus != Acad::eOk)
			{
				acutPrintf(L"\nНе удалось получить сущность.");
				continue;
			}

			if (pEntity->isA() != AcDbBlockReference::desc()) 
			{
				pEntity->close();
				continue;
			}

			// работа с найденным блоком
			blockId = (AcDbBlockReference::cast(pEntity))->blockTableRecord(); // получаем ID блока

			if (acdbOpenObject((AcDbObject*&)pCurEntityBlock, blockId, AcDb::kForRead) == Acad::eOk) 
			{
				pCurEntityBlock->getName(blockName);
				if (wcscmp(blockName, L"EMPLOYEE") == 0) 
				{
					if (pEntity->upgradeOpen() == Acad::eOk)
						pEntity->setLayer(L"USER"); // устанавливаем слой
				}
				pCurEntityBlock->close();
				acdbFree(blockName);
			}
			pEntity->close();
		}

		pBlockTableRecord->close();
	}
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ASMyGroupMyCommand () {	}

	static void ASMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM ) {}
		else {}
	}

	// ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ASMyGroupMySessionCmd () {	}

	// ACED_ADSFUNCTION_ENTRY_AUTO(CStep03App, MyLispFunction, false)
	static int ads_MyLispFunction () 
	{
		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CStep03App)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ASMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ASMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, ASMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, nullptr)
ACED_ADSSYMBOL_ENTRY_AUTO(CStep03App, MyLispFunction, false)

ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AsdkStep03, _CREATE, CREATE, ACRX_CMD_TRANSPARENT, nullptr)
ACED_ARXCOMMAND_ENTRY_AUTO(CStep03App, AsdkStep03, _SETLAYER, SETLAYER, ACRX_CMD_TRANSPARENT, nullptr)

