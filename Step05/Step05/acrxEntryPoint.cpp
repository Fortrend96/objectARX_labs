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
				acutPrintf(L"Невозможно загрузить AsdkEmployeeDetails.dbx.\n");
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

	static void AsdkStep05_SETLAYER(void)
	{
		Acad::ErrorStatus errorStatus; // статус ошибки
		AcDbBlockTableRecord* pBlockTableRecord; // запись в таблице блоков

		// получаем таблицу блоков в режиме чтения
		AcDbBlockTable* pBlockTable = GetPBlockTable();// таблица блоков

		if (!pBlockTable) return;	

		// Получаем пространство модели в режиме чтения
		errorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		
		if (errorStatus != Acad::eOk)
		{
			acutPrintf(L"\nНе удалось получить пространство модели.\n");
			pBlockTable->close();
			return;
		}

		pBlockTable->close();

		// итератор для прохождения содержимого пространства модели
		AcDbBlockTableRecordIterator* pIter; 
		errorStatus = pBlockTableRecord->newIterator(pIter);

		if (errorStatus != Acad::eOk) {
			acutPrintf(L"\nНе удалось создать итератор пространства модели.");
			pBlockTableRecord->close();
			return;
		}

		// обертываем итератор в умный указатель
		std::unique_ptr<AcDbBlockTableRecordIterator> pBlockTableRecordIter(pIter); 

		TCHAR* blockName; // имя блока
		AcDbEntity* pEntity; // сущность
		AcDbBlockTableRecord* pCurEntityBlock; // текущий блок
		AcDbObjectId blockId; // ID блока

		for (pBlockTableRecordIter->start(); !pBlockTableRecordIter->done(); pBlockTableRecordIter->step())
		{
			// проверяем кажду сущность
			errorStatus = pBlockTableRecordIter->getEntity(pEntity, AcDb::kForRead); // получаем сущность блока в режиме чтения
			if (errorStatus != Acad::eOk) {
				acutPrintf(L"\nНе удалось получить сущность.");
				continue;
			}

			if (pEntity->isA() != AcDbBlockReference::desc()) {
				pEntity->close();
				continue;
			}

			// работа с найденным блоком
			AcDbBlockReference* pBlockRef = nullptr;
			if ((pBlockRef = AcDbBlockReference::cast(pEntity)) == nullptr)
			{
				pEntity->close();
				continue;
			}

			// получаем ID блока
			blockId = pBlockRef->blockTableRecord();

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

	// ----- AsdkStep05._LISTDETAILS command (do not rename)
	static void AsdkStep05_LISTDETAILS(void)
	{
		/*
		алгоритм функции
		1. Позвольте пользователю выбрать ссылку на блок (acedEntSel()).
			a. Преобразуйте ads_name в AcDbObjectId (acdbGetObjectId()).
			b. Откройте объект (acdbOpenAcDbObject()).
			c. Проверьте с помощью функции isKindOf(), что пользователь выбрал ссылку на блок (AcDbBlockReference::desc()).
			d. Верните, если это не ссылка на блок.
		2. Извлеките словарь расширений ссылки на блок. Если его нет, верните. (AcDbObject::extensionDictionary()).
		3. Извлеките "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()). Если его нет, верните.
		4. Извлеките объект AsdkEmployeeDetails из «ASDK_EMPLOYEE_DICTIONARY» в разделе «DETAILS».
		5. Извлеките данные объекта AsdkEmployeeDetails и распечатайте сведения.		
		*/

		ads_name employeeName;
		ads_point pt;
	
		if (acedEntSel(L"Выбирите сотрудника: ", employeeName, pt) != RTNORM)
			return;
		
		AcDbObjectId employeeID;
		if (acdbGetObjectId(employeeID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;
		if (acdbOpenAcDbObject(pEmployeeObject, employeeID, AcDb::kForRead) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) 
		{
			acutPrintf(L"\nНе ялвяется ссылкой на блок.");
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
			acutPrintf(L"\nНе удалось открыть словарь расширений.");
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
			// Открываем словарь в режиме записи, если он уже существует
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk)
			{
				pExtDict->close();
				acutPrintf(L"\nНе удалось открыть словарь сотрудника.");
				return;
			}
			
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL) {
				pEmployeeDictObject->close();
				pExtDict->close();
				acutPrintf(L"\nЗапись не является словарем.");
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
			acutPrintf(L"\nНе удалось открыть объект Detail.");
			return;
		}
		
		AsdkEmployeeDetails* pEmployeeDetails = AsdkEmployeeDetails::cast(pDetailObject);
		if (!pEmployeeDetails) 
		{
			acutPrintf(L"\nНе удалось найти объект EmployeeDetails.");
			pDetailObject->close();
			pEmployeeDict->close();
			pExtDict->close();
			return;
		}
		// Отображение информации
		Adesk::Int32 i;
		pEmployeeDetails->iD(i);
		acutPrintf(L"ID сотрудника: %d\n", i);
		pEmployeeDetails->cube(i);
		acutPrintf(L"Номер кабинки сотрудника: %d\n", i);
		TCHAR* st = nullptr;
		pEmployeeDetails->firstName(st);
		acutPrintf(L"Имя сотрудника: %s\n", st);
		delete[] st;
		pEmployeeDetails->lastName(st);
		acutPrintf(L"Фамилия сотрудника: %s\n", st);
		delete[] st;

		pDetailObject->close();
		pEmployeeDict->close();
		pExtDict->close();
	}


	// ----- AsdkStep05._REMOVEDETAIL command (do not rename)
	static void AsdkStep05_REMOVEDETAIL(void)
	{
		/*
		алгоритм функции
		1. Позвольте пользователю выбрать ссылку на блок (acedEntSel()).
			a. Преобразуйте ads_name в AcDbObjectId (acdbGetObjectId()).
			b. Откройте объект (acdbOpenAcDbObject()).
			c. Проверьте с помощью функции isKindOf(), что пользователь выбрал ссылку на блок (AcDbBlockReference::desc()).
			d. Верните, если это не ссылка на блок.
		2. Извлеките словарь расширений ссылки на блок. Если его нет, верните. (AcDbObject::extensionDictionary()).
		3. Извлеките "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()). Если его нет, верните.
		4. Извлеките объект AsdkEmployeeDetails из "ASDK_EMPLOYEE_DICTIONARY" в разделе "DETAILS".
		5. Удалите объект AsdkEmployeeDetails (AcDbObject::erase()).
		6. Удалите словари, если в них больше нет записей. (AcDbDictionary::numEntries())		
		*/

		ads_name employeeName;
		ads_point pt;
		
		if (acedEntSel(L"Выберите сотрудника: ", employeeName, pt) != RTNORM)
			return;

		AcDbObjectId employeeObjectID;
		if (acdbGetObjectId(employeeObjectID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;

		if (acdbOpenAcDbObject(pEmployeeObject, employeeObjectID, AcDb::kForRead) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) {
			acutPrintf(L"\nНе является ссылкой на блок.");
			pEmployeeObject->close();
			return;
		}

		// получаем словарь расширений
		AcDbObjectId extDicdID;
		if ((extDicdID = pEmployeeObject->extensionDictionary()) == AcDbObjectId::kNull) 
		{
			pEmployeeObject->close();
			return;
		}
		pEmployeeObject->close(); // закрываем
		
		AcDbDictionary* pExtDict; // словарь расширений
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, extDicdID, AcDb::kForWrite, Adesk::kFalse) != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось открыть словарь расширений.");
			return;
		}

		// проверяем наличие словаря сотруднкика 
		AcDbDictionary* pEmployeeDict; // словарь сотрудника
		AcDbObjectId employeeDictID; // id словаря сотрудника
		AcDbObject* pEmployeeDictObject; 
		if (pExtDict->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound) 
		{
			pExtDict->close();
			return;
		}
		else {
			// открываем словарь в режиме записи
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk)
			{
				pExtDict->close();
				acutPrintf(L"\nНе удалось открыть словарь сотрудника.");
				return;
			}
			// проверка наличия записис
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
			{
				pEmployeeDictObject->close();
				pExtDict->close();
				acutPrintf(L"\nЗапись не является словарем");
				return;
			}
		}
		// Проверяем наличие записи с таким ключом
		if (pEmployeeDict->getAt(L"DETAILS", employeeDictID) != Acad::eOk) 
		{
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(L"\nНет записи с таким ключом.");
			return;
		}

		// Открываем объект в режиме записи
		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk) 
		{
			pEmployeeDict->close();
			pExtDict->close();
			acutPrintf(L"\nНе удалось открыть объект.");
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
		Алгоритм функции:
			1. Позвольте пользователю выбрать ссылку на блок(acedEntSel()).
			a.Преобразуйте ads_name в AcDbObjectId(acdbGetObjectId()).
			b.Откройте объект(acdbOpenAcDbObject()).
			c.Проверьте с помощью функции isKindOf(), что пользователь выбрал ссылку на блок(AcDbBlockReference::desc())
			d.Верните, если это не ссылка на блок.
			2. Введите данные о сотруднике для идентификатора сотрудника; куба сотрудника; имени сотрудника; фамилии сотрудника(acedGetInt(), acedGetString()).
			3. Получите словарь расширений ссылки на блок EMPLOYEE(AcDbObject::extensionDictionary()).
			4. Если ссылка на блок EMPLOYEE не имеет словаря расширений, создайте его(AcDbObject::createExtensionDictionary()).
			5. Откройте словарь расширений.Если словарь расширения был ранее стерт, то откройте стертый словарь расширения, так как мы можем повторно использовать словарь расширения.
			6. Если словарь расширения был стерт(AcDbObject::isErased()) восстановите его стертость(AcDbObject::erase(Adesk::kFalse)).
			7. Извлеките "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
			Если AcDbDictionary "ASDK_EMLOYEE_DICTIONARY" не существует, создайте AcDbDictionary "ASDK_EMPLOYEE_DICTIONARY" и добавьте его в словарь расширения ссылки на блок EMPLOYEE.
			8. Проверьте, присутствует ли объект AsdkEmployeeDetails в "ASDK_EMPLOYEE_DICTIONARY" (мы храним объекты AsdkEmployeeDetails под ключом "DETAILS").
			a.Если объект AsdkEmployeeDetails не существует, создайте новый объект AsdkEmployeeDetails и задайте его данные.
			b.Добавьте новый объект AsdkEmployeeDetails под ключом "DETAILS" в "ASDK_EMPLOYEE_DICTIONARY"
		*/

		ads_name employeeName; // имя сотрудника
		ads_point pt; // точка

		// получаем данные от пользователя 
		if (acedEntSel(L"Выберите сотрудника: ", employeeName, pt) != RTNORM)
			return;

		// проверка данных о сотруднике
		AcDbObjectId employeeID;
		if (acdbGetObjectId(employeeID, employeeName) != Acad::eOk)
			return;

		AcDbObject* pEmployeeObject;
		if (acdbOpenAcDbObject(pEmployeeObject, employeeID, AcDb::kForWrite) != Acad::eOk)
			return;

		if (!pEmployeeObject->isKindOf(AcDbBlockReference::desc())) 
		{
			acutPrintf(L"\nОбъект не является ссылкой на блок.");
			pEmployeeObject->close();
			return;
		}

		// получаем данные от пользователя
		int id, cubeNumber;
		TCHAR strFirstName[133];
		TCHAR strLastName[133];
		if (acedGetInt(L"Введите ID сотрудника: ", &id) != RTNORM
			|| acedGetInt(L"Введите кубический номер: ", &cubeNumber) != RTNORM
			|| acedGetString(0, L"Введите имя сотрудника: ", strFirstName) != RTNORM
			|| acedGetString(0, L"Введите фамилию сотрудника: ", strLastName) != RTNORM) 
		{
			pEmployeeObject->close();
			return;
		}
		// получаем словарь расширений
		AcDbObjectId extDictID; // id словаря расширений
		if ((extDictID = pEmployeeObject->extensionDictionary()) == AcDbObjectId::kNull)
		{
			if (pEmployeeObject->createExtensionDictionary() != Acad::eOk) 
			{
				pEmployeeObject->close();
				acutPrintf(L"\nНе удалось создать словарь расширений.");
				return;
			}

			extDictID = pEmployeeObject->extensionDictionary();
		}

		// закрываем объект
		pEmployeeObject->close();

		// открываем расширенный словарь
		AcDbDictionary* pExtDict;
		if (acdbOpenAcDbObject((AcDbObject*&)pExtDict, extDictID, AcDb::kForWrite, Adesk::kTrue) != Acad::eOk) {
			acutPrintf(L"\nНе удалось открыть словарь.");
			return;
		}

		// восстанавливаем расширенный словарь, если он был стерт
		if (pExtDict->isErased())
			pExtDict->erase(Adesk::kFalse);

		// проверяем наличие словаря
		AcDbDictionary* pEmployeeDict;
		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pExtDict->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound)
		{
			// создаем словарь
			pEmployeeDict = new AcDbDictionary;
			Acad::ErrorStatus errorStatus = pExtDict->setAt(L"ASDK_EMPLOYEE_DICTIONARY", pEmployeeDict, employeeDictID);

			if (errorStatus != Acad::eOk) 
			{
				pExtDict->close();
				acutPrintf(L"\nНе удалось создать словарь сотрудника.");
				return;
			}
		}
		else {
			// Открываем словарь сотрудника
			if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk) 
			{
				pExtDict->close();
				acutPrintf(L"\nНе удалось открыть словарь.");
				return;
			}
			// проверяем, не создал ли кто-то запись с нашим именем которая не является словарем.
			if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
			{
				pEmployeeObject->close();
				pExtDict->close();
				acutPrintf(L"\nНе является словарем.");
				return;
			}
		}
		// закрываем словарь 
		pExtDict->close();
		AcDbObjectId detailsID;
		// Проверяем наличие записи с данным ключом
		if (pEmployeeDict->getAt(L"DETAILS", detailsID) == Acad::eOk) {
			pEmployeeDict->close();
			acutPrintf(L"\nDetails уже есть в объекте \"Сотрудник\".");
			return;
		}
		// Создаем EmployeeDetails
		AcDbObjectPointer<AsdkEmployeeDetails> pEmployeeDetails;
		pEmployeeDetails.create();
		pEmployeeDetails->setID(id);
		pEmployeeDetails->setCube(cubeNumber);
		pEmployeeDetails->setFirstName(strFirstName);
		pEmployeeDetails->setLastName(strLastName);
		// Добавляем EmployeeDetails в словарь
		if (pEmployeeDict->setAt(L"DETAILS", pEmployeeDetails, detailsID) != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось добавить DETAILS в объект \"Сотрудник\".");
			pEmployeeDict->close();
			return;
		}
		acutPrintf(L"\nDetails успешно добавлен!");
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