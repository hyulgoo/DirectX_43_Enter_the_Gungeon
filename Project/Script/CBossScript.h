#pragma once
#include <Engine\CScript.h>

class CStateScript;

class CBossScript :
    public CScript
{
private:
    CStateScript* m_pState;
    DIR           m_CurDir;
    DIR           m_PrevDir;
    CGameObject*  m_pPlayer;
    float         m_fLengthtoPly;
    float         m_fAngle;
    Vec2          m_vDirToPly;
    Vec3          m_vPlayerPos;
    bool          m_bCollidable;
    bool          m_bHitEffect;
    float         m_fHitEffectDelay;
    bool          m_bRotable;

public:
    virtual void begin() override;
    virtual void tick() override;
    void CheckDir();
    void SetState();
    float GetLengthtoPly() { return m_fLengthtoPly; }
    Vec2 GetDirtoPly() { return m_vDirToPly; }
    Vec3 GetPlayerPos() { return m_vPlayerPos; }
    float GetDirtoPlyDegree() { return m_fAngle; }
    bool IsCollidable() { return m_bCollidable; }
    void SetCollidable(bool _able) { m_bCollidable = _able; }
    void SetHitEffect();
    void SetRotable(bool _bRotable) { m_bRotable = _bRotable; }


    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBossScript);

    CBossScript();
    CBossScript(const CBossScript& _Other);
    ~CBossScript();
};

