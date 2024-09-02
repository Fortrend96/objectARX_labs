//-----------------------------------------------------------------------------
//----- Hourglass.h : Declaration of the Hourglass
//-----------------------------------------------------------------------------
#pragma once

#ifdef CUSTOMLABDBX_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Parallelepiped.h"
#include "Sphere.h"
#include "Pyramid.h"
#include <vector>

//-----------------------------------------------------------------------------
class DLLIMPEXP Hourglass : public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(Hourglass) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:
	Hourglass(); // конструктор по умолчанию
	Hourglass(const AcGePoint3d&, const AcGePoint3d&, double); // конструктор с параметрами
	Hourglass(const Hourglass&); // конструктор копирования

	virtual ~Hourglass();

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;

	//- Dxf Filing protocol
	virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const ;
	virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) ;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual void subViewportDraw (AcGiViewportDraw *mode) ;
	virtual Adesk::UInt32 subViewportDrawLogicalFlags (AcGiViewportDraw *vd) ;
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;

	//- Osnap points protocol
public:
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subGetOsnapPoints (
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,
		const AcGePoint3d &pickPoint,
		const AcGePoint3d &lastPoint,
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d &insertionMat) const ;

	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;
	virtual Acad::ErrorStatus subGetGripPoints (
		AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
		const AcGeVector3d &curViewDir, const int bitflags) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) ;


	// вводимые данные
	AcGePoint3d m_ptBotCenter; // точка размещения часов
	AcGePoint3d m_ptBotEdge; // край размещения окружности

	double m_fHeight; // высота часов	

	// рассчитанные данные	 
	AcGePoint3d m_ptTopCenter; // верхняя точка часов
	AcGePoint3d m_ptTopEdge;
	double m_fRadius; // радиус конусов и цилиндров

	AcGePoint3d m_ptCenter; // центр часов

	Parallelepiped m_topParallelepiped;
	Cylinder m_topCylinder;
	Cone m_topCone;

	Pyramid m_pyramid;

	Cone m_botCone;
	Cylinder m_botCylinder;
	Parallelepiped m_botParallelepiped;

	Sphere m_sphere;

	AcCmColor m_botColor; // цвет нижней части фигуры 
	AcCmColor m_topColor; // цвет верхней части фигуры


	Adesk::Boolean caldData(); // расчет данных

	// команды общего редактирования
	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	// расчленение
	Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const;
};

#ifdef CUSTOMLABDBX_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(Hourglass)
#endif
