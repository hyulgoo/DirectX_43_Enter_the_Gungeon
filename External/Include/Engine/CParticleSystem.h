#pragma once
#include "CRenderComponent.h"

#include "CParticleUpdateShader.h"

class CStructuredBuffer;
class CTexture;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*         m_ParticleBuffer;
    CStructuredBuffer*         m_RWBuffer;
    CStructuredBuffer*         m_ModuleDataBuffer;

    PARTICLE                   m_tParticleType;
    tParticleModule            m_ModuleData;
    Ptr<CParticleUpdateShader> m_UpdateCS;
    Ptr<CTexture>              m_UseTex;
    float                      m_AccTime;

private:
    void SetModuleInfo();

public:
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;

    tParticleModule GetModuleData() { return m_ModuleData; }
    void SetModuleData(tParticleModule _tModuleData) { m_ModuleData = _tModuleData; }
    void SetModuleData(PARTICLE _tParticle);

    Ptr<CTexture> GetTex() { return m_UseTex; }
    PARTICLE GetParticleType() { return m_tParticleType; }

    void SpawnModule(int _iMaxParticle, int _iSpawnRate, Vec3 _vSpawnColor, Vec3 vSpawnMinScale
        , Vec3 vSpawnMaxScale, Vec3 _vSpawnBoxScale, float _fMinLifeTime, float _fMaxLifeTime);
    void ColorChangeModule(Vec3 _vStartColor, Vec3 _vEndColor);
    void ScaleChangeModule(float _vStartScale, float _vEndScale);
    void AddVelocityModule(float _fSpeed, int _iVelocityType, Vec3 _vVelocityDir, float _fAngle);
    void DragModule(float _fStartDrag, float _fEndDrag);
    void RandomForceModule(float _fForceTerm, float _fForce);
    void VelocityAlignmentModule();
    void VelocityScaleModule(float _fMaxSpeed, Vec3 _vMaxVelocityScale);
    void AnimationModule(int _iFrmCount, int _iXCount, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vOffset);
    void SetFollow() { m_ModuleData.Space = 1; }
    void SetType(PARTICLE _tType) { m_tParticleType = _tType; SetModuleInfo(); }

    void OnOff(bool _Onoff);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CParticleSystem);

public:
    CParticleSystem(PARTICLE _tParticle);
    CParticleSystem(const CParticleSystem& _Other);
    ~CParticleSystem();
};

