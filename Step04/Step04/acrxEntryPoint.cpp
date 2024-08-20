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
		Алгоритм функции 
		1 Запросить у пользователя имя сотрудника (acedGetString()).
		2 Получить словарь именованных объектов из текущей рабочей базы данных (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		3 Проверить, есть ли уже "ASDK_EMPLOYEE_DICTIONARY" в NOD (AcDbDictionary::getAt()).
		4 Если базовый уровень "ASDK_EMPLOYEE_DICTIONARY" отсутствует в NOD, создать новый AcDbDictionary с ключом "ASDK_EMPLOYEE_DICTIONARY" и добавить его в словарь именованных объектов (new AcDbDictionary, AcDbDictionary::setAt()).
		5 Проверить, есть ли уже имя сотрудника в словаре "ASDK_EMPLOYEE_DICTIONARY".
		6 Если словарь сотрудников отсутствует, создайте новую запись AcDbXrecord и добавьте ее в "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::setAt()).
		7 Не забудьте закрыть словарь именованных объектов, "ASDK_EMPLOYEE_DICTIONARY" и запись Xrecord, если она была создана.
		*/

		TCHAR employeeStrID[133];
		if (acedGetString(0, L"Введите имя сотрудника: ", employeeStrID) != RTNORM)
			return;

		// получаем словарь именованныз объектов Get the named object dictionary
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(L"Не удалось открыть словарь именованных объектов.");
			return;
		}

		// проверяем наличие словаря сотрудников
		AcDbObjectId employeeDictID; // ID словаря сотрудников
		AcDbDictionary* pEmployeeDict = NULL;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) == Acad::eKeyNotFound)
		{			
			if (pNOD->upgradeOpen() != Acad::eOk) 
			{
				acutPrintf(L"\nНе удалось открыть словарь именованных объектов в режиме записи!");
				pNOD->close();
				return;
			}
			pEmployeeDict = new AcDbDictionary; // создаем  словарь
			// добавляем словарь сотрудников в словарь именованных объектов
			if (pNOD->setAt(L"ASDK_EMPLOYEE_DICTIONARY", pEmployeeDict, employeeDictID) != Acad::eOk) 
			{
				acutPrintf(L"\nНе удалось добавить словарь сотрудников в словарь именованных объектов AutoCAD!");
				delete pEmployeeDict;
				pEmployeeDict = nullptr;
				pNOD->close();
				return;
			}
		}
		else {
			AcDbObject* pEmplyeeDictObject; // наш словарь
			if (acdbOpenAcDbObject(pEmplyeeDictObject, employeeDictID, AcDb::kForWrite) != Acad::eOk)
			{
				acutPrintf(L"\nНе удалось открыть объект в режиме записи.");
				pNOD->close();
				return;
			}
			// Проверка на наличие записи с тем же именем, что и у словаря сотрудников.
			if ((pEmployeeDict = AcDbDictionary::cast(pEmplyeeDictObject)) == NULL)
			{
				acutPrintf(L"\nНайдена запись в  словаре именованных объектов, но это не словарь сотрудников.");
				pEmplyeeDictObject->close();
				pNOD->close();
				return;
			}
		}
		pNOD->close();
		// Проверяем на наличие запись с данным ключом
		if (pEmployeeDict->getAt(employeeStrID, employeeDictID) == Acad::eOk) {
			acutPrintf(L"\nДанный сотрудник уже зарегистрирован.");
			pEmployeeDict->close();
			return;
		}
		// Добавляем новую запись о сотруднике
		AcDbXrecord* pEmployeeEntry = new AcDbXrecord;
		if (pEmployeeDict->setAt(employeeStrID, pEmployeeEntry, employeeDictID) != Acad::eOk)
		{
			acutPrintf(L"\nНе удалось добавить запись о сотруднике в словарь..");
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
		Алгоритм функции
		1. Получите словарь именованных объектов из текущей рабочей базы данных (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		2. Получите словарь "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
		3. Выполните итерацию по "ASDK_EMPLOYEE_DICTIONARY" и выведите ключи словаря (имена сотрудников).
			1. Создайте новый итератор (AcDbDictionary::newIterator(), AcDbDictionaryIterator).
			2. Выполните итерацию по "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionaryIterator::done(), AcDbDictionaryIterator::next()).
			3. Распечатайте ключ словаря (AcDbDictionaryIterator::name()).
		4. Удалите итератор и не забудьте закрыть открытые объекты.
		*/

		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\nНе удалось открыть словарь именованных объектов.");
			return;
		}

		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) != Acad::eOk) 
		{
			acutPrintf(L"\nСловарь сотрудников отсутствует.");
			pNOD->close();
			return;
		}

		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(L"\nНе удалось открыть словарь для чтения.");
			pNOD->close();
			return;
		}

		// Проверка на наличие записи с тем же именем, что и у словаря сотрудников.
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL) 
		{
			acutPrintf(L"Найдена запись в  словаре именованных объектов, но это не словарь сотрудников.");
			pEmployeeDictObject->close();
			pNOD->close();
			return;
		}
		pNOD->close();

		AcDbDictionaryIterator* pEmpDictIter = pEmployeeDict->newIterator(); // итератор для пробегания по словарю

		if (pEmpDictIter)
		{
			while (!pEmpDictIter->done())
			{
				acutPrintf(L"Сотрудник: %s.\n", pEmpDictIter->name()); // вывод имени сотрудника
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
		Алгоритм функции
		1. Получите имя сотрудника от пользователя (acedGetString()) для удаления.
		2. Получите словарь именованных объектов из текущей рабочей базы данных (AcDbDictionary, AcDbDatabase::getNamedObjectsDictionary()).
		3. Получите словарь "ASDK_EMPLOYEE_DICTIONARY" (AcDbDictionary::getAt()).
		4. Получите запись AcDbXrecord с указанным именем сотрудника.
		Если запись сотрудника существует, откройте ее для записи, а затем сотрите (AcDbObject::erase()).
		*/

		// запрашиваем у пользователя имя сотрудника
		TCHAR employeeStrID[133];
		if (acedGetString(0, L"Введите имя сотрудника: ", employeeStrID) != RTNORM)
			return;

		// получаем словарь именнованных объектов
		AcDbDictionary* pNOD;
		if (acdbHostApplicationServices()->workingDatabase()->getNamedObjectsDictionary(pNOD, AcDb::kForRead) != Acad::eOk) {
			acutPrintf(L"\nНе удалось открыть словарь именованных объектов.");
			return;
		}

		// проверяем наличие словаря сотрудников
		AcDbObjectId employeeDictID;
		AcDbObject* pEmployeeDictObject;
		if (pNOD->getAt(L"ASDK_EMPLOYEE_DICTIONARY", employeeDictID) != Acad::eOk)
		{
			acutPrintf(L"\nСловарь сотрудников отсутствует.");
			pNOD->close();
			return;
		}

		// открываем словарь сотрудников в режиме чтения
		if (acdbOpenAcDbObject(pEmployeeDictObject, employeeDictID, AcDb::kForRead) != Acad::eOk)
		{
			acutPrintf(L"\nНе удалось открыть словарь для чтения.");
			pNOD->close();
			return;
		}

		// Проверка на наличие записи с тем же именем, что и у словаря сотрудников.
		AcDbDictionary* pEmployeeDict;
		if ((pEmployeeDict = AcDbDictionary::cast(pEmployeeDictObject)) == NULL)
		{
			acutPrintf(L"Найдена запись в  словаре именованных объектов, но это не словарь сотрудников.");
			pEmployeeDictObject->close();
			pNOD->close();
			return;
		}
		pNOD->close();

		AcDbObjectId employeeObjectID;
		// Проверяем наличие записи о сотруднике в словаре
		if (pEmployeeDict->getAt(employeeStrID, employeeObjectID) != Acad::eOk)
		{
			acutPrintf(L"\nЗапись не найдена.");
			pEmployeeDict->close();
			return;
		}
		pEmployeeDict->close();
		
		AcDbObject* pEmployeeObject;
		// открываем запись о сотруднике
		if (acdbOpenAcDbObject(pEmployeeObject, employeeObjectID, AcDb::kForWrite) != Acad::eOk) {
			acutPrintf(L"\nEntry cannot be opened for write.");
			return;
		}
		// удаляем запись
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





