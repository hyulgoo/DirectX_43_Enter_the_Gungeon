#pragma once
#include "ComponentUI.h"

class CParticleSystem;
class TreeUI;

class ParticleSystemUI :
    public ComponentUI
{
private:
    TreeUI*             m_Tree;
    CParticleSystem*    m_pParticleSystem;
    tParticleModule     m_tModuleData;
    int                 m_DragNum;

public:
    virtual int render_update() override;
    void ONOFF(string _strName, string _strID, int &_ModuleCheck);
    void SpawnModule();
    void ScaleModule();
    void ColorModule();
    void AddVelocityModule();
    void DragModule();
    void RandomForceModule();
    void VelocityScaleModule();
    void AnimationModule();

    void Clear() { m_pParticleSystem = nullptr; }

public:
    ParticleSystemUI();
    ~ParticleSystemUI();

};

