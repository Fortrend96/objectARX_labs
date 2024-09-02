//-----------------------------------------------------------------------------
//----- Hourglass.cpp : Implementation of Hourglass
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "Hourglass.h"
//-----------------------------------------------------------------------------
Adesk::UInt32 Hourglass::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	Hourglass, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, HOURGLASS,
CUSTOMLABDBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
Hourglass::Hourglass () : 
	AcDbEntity () 
{
	m_ptBotCenter = AcGePoint3d(0, 25, 0);
	m_ptBotEdge = AcGePoint3d(0, 50, 0);
	m_fHeight = 50;

	m_botColor.setRGB(255, 0, 0);
	m_topColor.setRGB(255, 255, 255);

}
//-----------------------------------------------------------------------------
Hourglass::Hourglass(const AcGePoint3d& ptTopCenter, 
					const AcGePoint3d& ptBottomCenter,
					double fHalfWidth) : AcDbEntity()
{
	//m_ptTopCenter = ptTopCenter;
	//m_ptBottomCenter = ptBottomCenter;
	//m_fHalfWidth = fHalfWidth;
	//caldData();
}
//-----------------------------------------------------------------------------
Hourglass::Hourglass(const Hourglass& obj)
{
	//m_ptTopCenter = obj.m_ptTopCenter; // верхняя точка часов
	//m_ptBottomCenter = obj.m_ptBottomCenter;  // нижняя точка часов
	//m_fHalfWidth = obj.m_fHalfWidth; // полуширина часов

	//// рассчетные данные	
	//m_ptCenter = obj.m_ptCenter; // точка центра часов
	//m_fHeight = obj.m_fHeight; // высота часов
	//m_fBaseWidth = obj.m_fBaseWidth; // ширина основания часов
	//m_arVerices = obj.m_arVerices; // вершины конусов и цилиндров
}
//-----------------------------------------------------------------------------
Hourglass::~Hourglass () 
{

}
//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus Hourglass::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (Hourglass::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params

	//es = pFiler->writePoint3d(m_ptTopCenter); if (es != Acad::eOk) return (es);
	//es = pFiler->writePoint3d(m_ptBottomCenter);  if (es != Acad::eOk) return (es);
	//es = pFiler->writeDouble(m_fHalfWidth); if (es != Acad::eOk) return (es);

	//// рассчетные данные	
	//es = pFiler->writePoint3d(m_ptCenter); if (es != Acad::eOk) return (es);
	//es = pFiler->writeDouble(m_fHeight); if (es != Acad::eOk) return (es);
	//es = pFiler->writeDouble(m_fBaseWidth); if (es != Acad::eOk) return (es);

	//for (int i = 0; i < m_arVerices.length(); ++i)
	//{
	//	es = pFiler->writePoint3d(m_arVerices[i]);
	//	if (es != Acad::eOk) return (es);
	//}

	return Acad::eOk;
}

Acad::ErrorStatus Hourglass::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > Hourglass::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;


	//es = pFiler->readPoint3d(&m_ptTopCenter); if (es != Acad::eOk) return (es);
	//es = pFiler->readPoint3d(&m_ptBottomCenter);  if (es != Acad::eOk) return (es);
	//es = pFiler->readDouble(&m_fHalfWidth); if (es != Acad::eOk) return (es);

	//// рассчетные данные	
	//es = pFiler->readPoint3d(&m_ptCenter); if (es != Acad::eOk) return (es);
	//es = pFiler->readDouble(&m_fHeight); if (es != Acad::eOk) return (es);
	//es = pFiler->readDouble(&m_fBaseWidth); if (es != Acad::eOk) return (es);

	//for (int i = 0; i < m_arVerices.length(); ++i)
	//{
	//	es = pFiler->readPoint3d(&m_arVerices[i]);
	//	if (es != Acad::eOk) return (es);
	//}

	return Acad::eOk;
}

//- Dxf Filing protocol
Acad::ErrorStatus Hourglass::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("Hourglass")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, Hourglass::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus Hourglass::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("Hourglass")) )
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
	if ( version > Hourglass::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < Hourglass::kCurrentVersionNumber )
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
Adesk::Boolean Hourglass::subWorldDraw (AcGiWorldDraw *mode) 
{
	try
	{
		assertReadEnabled();
		m_topParallelepiped.subWorldDraw(mode);
		m_topCylinder.subWorldDraw(mode);
		m_topCone.subWorldDraw(mode);
		m_pyramid.subWorldDraw(mode);

		m_botCone.subWorldDraw(mode);
		m_botCylinder.subWorldDraw(mode);
		m_botParallelepiped.subWorldDraw(mode);
		m_sphere.subWorldDraw(mode);
		
	}
	catch (...)
	{
		return Adesk::kFalse;
	}

	return Adesk::kTrue;
}

void Hourglass::subViewportDraw (AcGiViewportDraw *mode) {
	assertReadEnabled () ;
	AcDbEntity::subViewportDraw (mode) ;
}

Adesk::UInt32 Hourglass::subViewportDrawLogicalFlags (AcGiViewportDraw *vd) {
	assertReadEnabled () ;
	return (AcDbEntity::subViewportDrawLogicalFlags (vd)) ;
}

Adesk::UInt32 Hourglass::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

	//- Osnap points protocol
Acad::ErrorStatus Hourglass::subGetOsnapPoints (
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

Acad::ErrorStatus Hourglass::subGetOsnapPoints (
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

//- Grip points protocol
Acad::ErrorStatus Hourglass::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds)) ;
}

Acad::ErrorStatus Hourglass::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

Acad::ErrorStatus Hourglass::subGetGripPoints (
	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	const AcGeVector3d &curViewDir, const int bitflags
) const {
	assertReadEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}

Acad::ErrorStatus Hourglass::subMoveGripPointsAt (
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
Acad::ErrorStatus Hourglass::subTransformBy(const AcGeMatrix3d& xform)
{
	try
	{
		
	}
	catch (...)
	{
		return Acad::eCreateFailed;
	}
	
	return Acad::eOk;
}
//-----------------------------------------------------------------------------
Acad::ErrorStatus Hourglass::subExplode(AcDbVoidPtrArray& entitySet) const
{
	try
	{
		assertReadEnabled();
		

	}
	catch (...)
	{
		return Acad::eCreateFailed;
	}
	
	
	return Acad::eOk;
}
//-----------------------------------------------------------------------------
Adesk::Boolean Hourglass::caldData()
{
	try
	{
		m_fRadius = m_ptBotCenter.distanceTo(m_ptBotEdge); // радиус элементов часов
		
		// центр верхушки часов
		m_ptTopCenter = AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.y, m_ptBotCenter.z + m_fHeight);

		// край верхушки часов
		m_ptTopEdge = AcGePoint3d(m_ptBotEdge.x, m_ptBotEdge.y, m_ptBotEdge.z + m_fHeight);

		// центр часов
		m_ptCenter = AcGePoint3d((m_ptBotCenter.x + m_ptTopCenter.x) / 2.0,
			(m_ptBotCenter.y + m_ptTopCenter.y) / 2.0,
			(m_ptBotCenter.z + m_ptTopCenter.z) / 2.0);


		double fParalHeight =		(m_fHeight / 2.0) * 0.1; // высота параллелепипеда
		double fCylinderHeight =	(m_fHeight / 2.0) * 0.45; // высота цилиндра
		double fConeHeight =		(m_fHeight / 2.0) * 0.45; // высота конуса
	

		// нижняя часть часов
		m_botParallelepiped.SetInputData(AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.y, m_ptBotCenter.z+ fParalHeight), 
									m_ptBotCenter, 
									m_ptBotEdge, m_botColor);

		m_botCylinder.SetInputData(
			AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.y, m_ptBotCenter.z + fParalHeight + fCylinderHeight),
			AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.y, m_ptBotCenter.z + fParalHeight),
			AcGePoint3d(m_ptBotEdge.x, m_ptBotEdge.y,	m_ptBotEdge.z + fParalHeight), m_botColor);

		m_botCone.SetInputData(
			AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.y, m_ptBotCenter.z + fParalHeight + fCylinderHeight),
			AcGePoint3d(m_ptBotEdge.x, m_ptBotEdge.y, m_ptBotEdge.z + fParalHeight + fCylinderHeight),
			m_ptCenter, m_botColor);


		m_pyramid.setInputData(
			AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.x, m_ptBotCenter.z + fParalHeight + fCylinderHeight),
			AcGePoint3d(m_ptBotCenter.x, m_ptBotCenter.x, m_ptBotCenter.z+ fParalHeight),
			m_fRadius, 4, m_botColor
		);


		// верхняя часть часов
		m_topCone.SetInputData(
			AcGePoint3d(m_ptTopCenter.x, m_ptTopCenter.y, m_ptTopCenter.z - fParalHeight - fCylinderHeight),
			AcGePoint3d(m_ptTopEdge.x, m_ptTopEdge.y,	m_ptTopEdge.z - fParalHeight - fCylinderHeight),
			m_ptCenter, m_topColor);

		m_topCylinder.SetInputData(
			AcGePoint3d(m_ptTopCenter.x, m_ptTopCenter.y, m_ptTopCenter.z - fParalHeight),
			AcGePoint3d(m_ptTopCenter.x, m_ptTopCenter.y, m_ptTopCenter.z - fParalHeight - fCylinderHeight),
			AcGePoint3d(m_ptTopEdge.x, m_ptTopEdge.y, m_ptTopEdge.z - fParalHeight - fCylinderHeight), m_topColor);

		m_topParallelepiped.SetInputData(m_ptTopCenter,
										AcGePoint3d(m_ptTopCenter.x, m_ptTopCenter.y, m_ptTopCenter.z - fParalHeight),
										AcGePoint3d(m_ptTopEdge.x, m_ptTopEdge.y, m_ptTopEdge.z - fParalHeight), m_topColor);

		m_sphere.setInputData(AcGePoint3d(m_ptTopCenter.x, m_ptTopCenter.y, m_ptTopCenter.z - fParalHeight - fCylinderHeight / 2.0),
			m_fRadius, m_topColor);

		
		// расчет составляющих часов
		m_topParallelepiped.caldData();
		m_topCylinder.caldData();
		m_topCone.caldData();
		m_pyramid.calcData();

		m_botCone.caldData();
		m_botCylinder.caldData();
		m_botParallelepiped.caldData();
		m_sphere.calcData();
	}
	catch (...)
	{
		return Adesk::kFalse;
	}

	return Adesk::kTrue;
}
//-----------------------------------------------------------------------------

