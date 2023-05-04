#pragma once
#include "CComponent.h"
class CLight2D :
    public CComponent
{
private:
    tLightInfo      m_LightInfo;

public:
    void SetLightType(LIGHT_TYPE _Type) { m_LightInfo.LightType = (UINT)_Type; }
    void SetLightDirection(Vec3 _vDir) { m_LightInfo.vWorldDir = _vDir; }
    void SetLightDiffuse(Vec3 _vDiffuse) { m_LightInfo.Color.vDiffuse = _vDiffuse; }
    void SetLightAmbient(Vec3 _vAmbient) { m_LightInfo.Color.vAmbient = _vAmbient; }
    void SetRadius(float _fRadius) { m_LightInfo.Radius = _fRadius; }
    void SetAngle(float _fAngle) { m_LightInfo.Angle = _fAngle; }

    tLightInfo GetLightInfo() const { return m_LightInfo; }

public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLight2D);
public:
    CLight2D();
    ~CLight2D();
};

