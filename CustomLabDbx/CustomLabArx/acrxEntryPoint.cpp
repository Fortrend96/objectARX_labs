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
#include "..\CustomLabDbx\Hourglass.h"
#include "HourglassJig.h"
#include <memory>

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CCustomLabArxApp : public AcRxArxApp {

public:
	CCustomLabArxApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) 
	{
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {	}


	static void CustomlabarxCHOURGLASS(void)
	{		
		// TODO: команда создания песочных часов
		int retval;
		ads_point ptTop;
		ads_point ptBottom;

		/*retval = acedGetPoint(nullptr,
			L"\nВерхняя точка часов <0,0,0>: ", ptTop);

		if (retval == RTNONE)
		{
			ptTop[X] = 0;
			ptTop[Y] = 0;
			ptTop[Z] = 10;
		}

		acedInitGet(RSG_NONULL, nullptr);

		retval = acedGetPoint(nullptr,
			L"\nНижняя точка часов <0,0,0>: ", ptBottom);

		AcGePoint3d p0(ptTop[X], ptTop[Y], ptTop[Z]);
		AcGePoint3d p1(ptBottom[X], ptBottom[Y], ptBottom[Z]);*/

		AcGePoint3d p0(25, 25, 0);
		AcGePoint3d p1(25, 50, 0);

		std::unique_ptr<HourglassJig> pHgJig(new HourglassJig(p0, p1));
		pHgJig->doIt();
	}

	static void CustomlabarxCTURNHG(void)
	{
		// TODO: команда переворачивания созданных часов
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCustomLabArxApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CCustomLabArxApp,
	Customlabarx, CHOURGLASS, CHOURGLASS, ACRX_CMD_MODAL, nullptr)

ACED_ARXCOMMAND_ENTRY_AUTO(CCustomLabArxApp,
	Customlabarx, CTURNHG, CTURNHG, ACRX_CMD_MODAL, nullptr)
