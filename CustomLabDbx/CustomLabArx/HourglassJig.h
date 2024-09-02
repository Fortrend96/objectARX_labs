#pragma once
#include "..\CustomLabDbx\Hourglass.h"

class HourglassJig :
    public AcEdJig
{
public:
    HourglassJig(const AcGePoint3d&, const AcGePoint3d&);

    ~HourglassJig();
private:
    // граничные точки изображения объекта на курсоре
    AcGePoint3d m_ptBotCenter; // точка размещения часов
    AcGePoint3d m_ptBotEdge; // край размещения окружности
    double m_fHeight; // высота часов
    double m_fRadius; // радиус конусов и цилиндров


    //AcGePoint3d m_ptDiameterLeft; //  левая граница диаметра
    //AcGePoint3d m_ptDiameterRight; // правая граница диаметра

    //// высота изображения объекта на курсоре
    //double m_fHeight;

    // указатель динамически созздаваемого и отображаемого на курсоре временного объекта
    Hourglass* m_pHourglassObj;

public:
    AcDbObjectId doIt();
    virtual DragStatus sampler();
    virtual Adesk::Boolean update();
    virtual AcDbEntity* entity() const;
};

