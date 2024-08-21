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
//----- DocData.cpp : Implementation file
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include <memory>
#include <tchar.h>

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars ;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData () {
	m_editCommand = false;
	m_doRepositioning = false;
	m_pAsdkDbEmployeeReactor = nullptr;
	attachEmployeeReactorToAllEmployee(true);
}

//-----------------------------------------------------------------------------
CDocData::CDocData (const CDocData &data) {
}

//-----------------------------------------------------------------------------
CDocData::~CDocData () {
}




Acad::ErrorStatus attachEmployeeReactorToAllEmployee(bool attach)
{
	// Ёта функци€ ищет все ссылки на блок "employee" в пространстве модели.
	// ≈сли вставка не была произведена к нашему объекту-реактору, мы присоедин€ем ее.
	Acad::ErrorStatus errorStatus = Acad::eNullEntityPointer;

	// получаем таблицу блоков текущей базы данных
	AcDbBlockTable* pBlockTable;

	AcDbHostApplicationServices* pHostAppServices = acdbHostApplicationServices();

	if (!pHostAppServices) return errorStatus;

	AcDbDatabase* pWorkingDatabase = pHostAppServices->workingDatabase();

	if (!pWorkingDatabase) return errorStatus;

	errorStatus = pWorkingDatabase->getBlockTable(pBlockTable, AcDb::kForRead);

	if (errorStatus != Acad::eOk)
		return errorStatus;

	// получаем пространство моделей
	AcDbBlockTableRecord* pModelSpace;
	errorStatus = pBlockTable->getAt(ACDB_MODEL_SPACE, pModelSpace, AcDb::kForRead);
	pBlockTable->close();
	if (errorStatus != Acad::eOk)
		return errorStatus;

	// создаем итератор
	AcDbBlockTableRecordIterator* pIterator;
	if ((errorStatus = pModelSpace->newIterator(pIterator)) != Acad::eOk) {
		pModelSpace->close();
		return errorStatus;
	}

	std::unique_ptr<AcDbBlockTableRecordIterator> pBlockTableRecordIterator(pIterator);

	// проходим по запис€м
	for (; !pBlockTableRecordIterator->done(); pBlockTableRecordIterator->step()) {
		AcDbEntity* pEnt;
		errorStatus = pBlockTableRecordIterator->getEntity(pEnt, AcDb::kForRead);

		AcDbBlockReference* pInsert = AcDbBlockReference::cast(pEnt);

		if (!pInsert) {
			pEnt->close();
			continue;
		}

		// получаем ID BlockTableRecord, где определена ссылка
		AcDbObjectId blockId = pInsert->blockTableRecord();
		AcDbBlockTableRecord* pBlockTableRecord;
		if (acdbOpenAcDbObject((AcDbObject*&)pBlockTableRecord, blockId, AcDb::kForRead) != Acad::eOk) 
		{
			acutPrintf(L"\nЌе удалось открыть таблицу блоков!");
			pEnt->close();
			break;
		}

		const TCHAR* blockName;
		pBlockTableRecord->getName(blockName);
		pBlockTableRecord->close();

		if (_tcscmp(blockName, _T("EMPLOYEE"))) {
			pEnt->close();
			continue; 
		}

		if (attach) {
			pEnt->addReactor(pEmployeeReactor);
		}
		else {
			pEnt->removeReactor(pEmployeeReactor);
		}
		pEnt->close();
	}
	pModelSpace->close();
	return (Acad::eOk);
}

void detachAllEmployeeReactors()
{
	std::unique_ptr<AcApDocumentIterator> pIterator(acDocManager->newAcApDocumentIterator());
	if (!pIterator)
		return;

	AcApDocument* pOldDoc = acDocManager->curDocument();

	while (!pIterator->done()) 
	{
		AcApDocument* pDoc = pIterator->document();
		if (pDoc->lockMode() == AcAp::kNone) {
			if (acDocManager->setCurDocument(pDoc, AcAp::kAutoWrite, Adesk::kFalse) == Acad::eOk) 
			{
				attachEmployeeReactorToAllEmployee(false);
				acDocManager->unlockDocument(pDoc);
			}
		}
		else {
			acDocManager->setCurDocument(pDoc);
			attachEmployeeReactorToAllEmployee(false);
		}
		pIterator->step();
	}

	acDocManager->setCurDocument(pOldDoc, AcAp::kNone, Adesk::kFalse);
}