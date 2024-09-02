#include "stdafx.h"
#include "HourglassJig.h"


HourglassJig::~HourglassJig()
{
	//if (m_pHourglassObj)
	//{
	//	delete m_pHourglassObj;
	//	m_pHourglassObj = nullptr;
	//}
}
//----------------------------------------------------------------
HourglassJig::HourglassJig(const AcGePoint3d& pt0, 
						const AcGePoint3d& pt1)
	: AcEdJig()
{
	m_ptBotCenter = pt0;
	m_ptBotEdge = pt1;
	m_fHeight = 0.0;
	m_fRadius = 0.0;

	m_pHourglassObj = nullptr;
}
//----------------------------------------------------------------
AcDbEntity* HourglassJig::entity() const
{
	return m_pHourglassObj;
}
//----------------------------------------------------------------
Adesk::Boolean HourglassJig::update()
{
	if (!m_pHourglassObj)
		return Adesk::kFalse;

	m_pHourglassObj->m_fHeight = m_fHeight;
	m_pHourglassObj->caldData();
	
	return Adesk::kTrue;
}
//----------------------------------------------------------------
AcEdJig::DragStatus HourglassJig::sampler()
{
	DragStatus stat;
	setUserInputControls(AcEdJig::kAccept3dCoordinates);

	// �������� ������ ������
	static double height;
	stat = acquireDist(m_fHeight, m_ptBotCenter);
	// ���� ��������� m_h
	if (height != m_fHeight) 
		height = m_fHeight;
	else if (stat == AcEdJig::kNormal) 
		return AcEdJig::kNoChange;

	return stat;
}
//----------------------------------------------------------------
AcDbObjectId HourglassJig::doIt()
{
	AcDbObjectId id = nullptr;
	m_pHourglassObj = new Hourglass();
	m_pHourglassObj->m_ptBotCenter = m_ptBotCenter;
	m_pHourglassObj->m_ptBotEdge = m_ptBotEdge;

	setDispPrompt(L"\n������, ��: ");
	AcEdJig::DragStatus stat = drag();// �������� sampler

	// ���� ������ ������� Esc - ����� ������
	if (stat == AcEdJig::kCancel)// <Esc>
	{
		acutPrintf(L"������ �� ������. ");
		delete m_pHourglassObj; 
		m_pHourglassObj = nullptr;
		return id;
	}
	id = append(); // ����� AcEdJig::append
	return id; // ���������� ObjectId ������������ � ������ �������
}
//----------------------------------------------------------------

