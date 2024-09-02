#pragma once
#include "..\CustomLabDbx\Hourglass.h"

class HourglassJig :
    public AcEdJig
{
public:
    HourglassJig(const AcGePoint3d&, const AcGePoint3d&);

    ~HourglassJig();
private:
    // ��������� ����� ����������� ������� �� �������
    AcGePoint3d m_ptBotCenter; // ����� ���������� �����
    AcGePoint3d m_ptBotEdge; // ���� ���������� ����������
    double m_fHeight; // ������ �����
    double m_fRadius; // ������ ������� � ���������


    //AcGePoint3d m_ptDiameterLeft; //  ����� ������� ��������
    //AcGePoint3d m_ptDiameterRight; // ������ ������� ��������

    //// ������ ����������� ������� �� �������
    //double m_fHeight;

    // ��������� ����������� ������������� � ������������� �� ������� ���������� �������
    Hourglass* m_pHourglassObj;

public:
    AcDbObjectId doIt();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbEntity* entity() const;
};

