// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
//----- AsdkEmployeeReactor.cpp : Implementation of AsdkEmployeeReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AsdkEmployeeReactor.h"
#include <tchar.h>

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(AsdkEmployeeReactor, AcDbObjectReactor, 1)

//-----------------------------------------------------------------------------
AsdkEmployeeReactor::AsdkEmployeeReactor () : AcDbObjectReactor() {
}

//-----------------------------------------------------------------------------
AsdkEmployeeReactor::~AsdkEmployeeReactor () 
{
}
//-----------------------------------------------------------------------------
void AsdkEmployeeReactor::openedForModify(const AcDbObject* dbObj)
{
	if (DocVars.docData().m_doRepositioning)
		return;

	// ���� �� ���� �� ������������� ������ �� �������, ������� �� �������
	if (!DocVars.docData().m_editCommand)
		return;

	AcDbBlockReference* pInsert = AcDbBlockReference::cast(dbObj);
	if (!pInsert)
		return;

	AcDbObjectId blockId = pInsert->blockTableRecord();
	AcDbBlockTableRecord* pBlockTableRecord;
	if (acdbOpenAcDbObject((AcDbObject*&)pBlockTableRecord, blockId, AcDb::kForRead) != Acad::eOk) 
	{
		acutPrintf(L"\n�� ������� ������� ������� ������!");
		return;
	}

	const TCHAR* blockName;
	pBlockTableRecord->getName(blockName);
	pBlockTableRecord->close();

	if (_tcscmp(blockName, L"EMPLOYEE"))
		return; // Not an employee

	// Store the objectID and the position
	DocVars.docData().m_changedObjects.append(pInsert->objectId());
	DocVars.docData().m_employeePositions.append(pInsert->position());
}


