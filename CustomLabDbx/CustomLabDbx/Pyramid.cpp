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
//----- Pyramid.cpp : Implementation of Pyramid
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Pyramid.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 Pyramid::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	Pyramid, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, PYRAMID,
CUSTOMLABDBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
Pyramid::Pyramid () : 
	AcDbEntity (),
	//m_ptTop(AcGePoint3d(0,0,20)),
	//m_ptBaseCenter(AcGePoint3d::kOrigin),
	//m_fBaseRadius(10.0),
	m_nBaseSides(4)
{
}
//-----------------------------------------------------------------------------
Pyramid::Pyramid(const AcGePoint3d& ptTop,
				const AcGePoint3d& ptBaseCenter,
				double fBaseRadius,
				int nBaseSides) 
	//:
	//m_ptTop(ptTop),
	//m_ptBaseCenter(ptBaseCenter),
	//m_fBaseRadius(fBaseRadius),
	//m_nBaseSides(nBaseSides)
{
}
//-----------------------------------------------------------------------------
Pyramid::~Pyramid () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus Pyramid::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (Pyramid::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	//es = pFiler->writePoint3d(m_ptTop); if (es != Acad::eOk) return (es);
	//es = pFiler->writePoint3d(m_ptBaseCenter); if (es != Acad::eOk) return (es);

	//es = pFiler->writeDouble(m_fHeight); if (es != Acad::eOk) return (es);
	//es = pFiler->writeInt32(m_nBaseSides); if (es != Acad::eOk) return (es);
	//es = pFiler->writeDouble(m_fBaseRadius); if (es != Acad::eOk) return (es);

	// запись вершин
	int nVertexCount = m_arVertices.length();
	pFiler->writeInt32(nVertexCount);

	for (int i = 0; i < nVertexCount; ++i)
	{
		es = pFiler->writePoint3d(m_arVertices[i]);
		if (es != Acad::eOk) return (es);

	}
	return Acad::eOk;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > Pyramid::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Pyramid::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....

	//es = pFiler->readPoint3d(&m_ptTop); if (es != Acad::eOk) return (es);
	//es = pFiler->readPoint3d(&m_ptBaseCenter); if (es != Acad::eOk) return (es);

	//es = pFiler->readDouble(&m_fHeight); if (es != Acad::eOk) return (es);
	//es = pFiler->readInt32(&m_nBaseSides); if (es != Acad::eOk) return (es);
	//es = pFiler->readDouble(&m_fBaseRadius); if (es != Acad::eOk) return (es);

	//// чтение вершин
	//int nVertexCount;
	//pFiler->readInt32(&nVertexCount);

	//m_arVertices.setLogicalLength(nVertexCount);
	//for (int i = 0; i < nVertexCount; ++i)
	//{
	//	es = pFiler->readPoint3d(&m_arVertices[i]);
	//	if (es != Acad::eOk) return (es);
	//}

	return Acad::eOk;
}
//-----------------------------------------------------------------------------
//- Dxf Filing protocol
Acad::ErrorStatus Pyramid::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("Pyramid")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, Pyramid::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....





}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("Pyramid")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > Pyramid::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Pyramid::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch ( rb.restype ) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....

			default:
				//----- An unrecognized group. Push it back so that the subclass can read it again.
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus ()) ;
}
//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean Pyramid::subWorldDraw (AcGiWorldDraw *mode) 
{
	assertReadEnabled();
	return draw(mode);
}
//-----------------------------------------------------------------------------
void Pyramid::subViewportDraw (AcGiViewportDraw *mode) {
	assertReadEnabled () ;
	AcDbEntity::subViewportDraw (mode) ;
}
//-----------------------------------------------------------------------------
Adesk::UInt32 Pyramid::subViewportDrawLogicalFlags (AcGiViewportDraw *vd) {
	assertReadEnabled () ;
	return (AcDbEntity::subViewportDrawLogicalFlags (vd)) ;
}
//-----------------------------------------------------------------------------
Adesk::UInt32 Pyramid::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}
//-----------------------------------------------------------------------------
	//- Osnap points protocol
Acad::ErrorStatus Pyramid::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds,
	const AcGeMatrix3d &insertionMat) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
}
//-----------------------------------------------------------------------------
//- Grip points protocol
Acad::ErrorStatus Pyramid::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds)) ;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::subGetGripPoints (
	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	const AcGeVector3d &curViewDir, const int bitflags
) const {
	assertReadEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Pyramid::subMoveGripPointsAt (
	const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
	const int bitflags
) {
	assertWriteEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
}
//-----------------------------------------------------------------------------
Adesk::Boolean Pyramid::draw(AcGiWorldDraw* pWorldDraw)
{
	try
	{
		// задаем цвет фигуре
		pWorldDraw->subEntityTraits().setColor(m_color.colorIndex());

		// –исуем грани пирамиды
		for (int i = 0; i < m_nBaseSides; ++i)
		{
			AcGePoint3d arPoint3d[]{ m_arVertices [i],  m_arVertices[m_nBaseSides] };
			pWorldDraw->geometry().polyline(2, arPoint3d);
		}

		// –исуем основание пирамиды
		AcGePoint3dArray arBaseVertices;

		for (int i = 0; i < m_nBaseSides; ++i)
		{
			arBaseVertices.append(m_arVertices[i]);
		}
		pWorldDraw->geometry().polygon(m_nBaseSides, arBaseVertices.asArrayPtr());
	}
	catch (...)
	{
		return Adesk::kFalse;
	}

	return Adesk::kTrue;
}
//-----------------------------------------------------------------------------
Adesk::Boolean Pyramid::calcData()
{
	try
	{
		
		//m_arVertices.setLogicalLength(m_nBaseSides + 1);

		// расчет высоты пирамиды
		m_fHeight = m_ptApex.distanceTo(m_ptBotCenter);

		// –асчет вершин основани€ 
		const double PI = 3.141592653589793;
		double angleStep = 2.0 * PI / m_nBaseSides; // угловой шаг в радианах

		m_arVertices.removeAll();

		for (int i = 0; i < m_nBaseSides; ++i)
		{
			double angle = i * angleStep;
			m_arVertices.append(AcGePoint3d(m_ptBotCenter.x + m_fRadius * cos(angle),
				m_ptBotCenter.y + m_fRadius * sin(angle),
				m_ptBotCenter.z));
		}

		// верхушка пирамиды
		m_arVertices.append(m_ptApex);
	}
	catch (...)
	{
		return Adesk::kFalse;
	}

	return Adesk::kTrue;
}
