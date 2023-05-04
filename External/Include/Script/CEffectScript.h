#pragma once
#include <Engine\CScript.h>      
class CEffectScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;
    Vec3         m_vPlayerPos;
    float        m_fDir;
    float        m_fDelay;
    bool         m_bOnOff;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetEffect(bool _Onoff);

    void SetRelativePos();
    void SetRelativeRot();

    CLONE(CEffectScript);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CEffectScript();
    CEffectScript(const CEffectScript& _Other);
    ~CEffectScript();
};

