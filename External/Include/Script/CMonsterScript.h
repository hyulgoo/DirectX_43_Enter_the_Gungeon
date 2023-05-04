#pragma once
#include <Engine/CScript.h>

class CStateScript;
class CGameObject;

struct MONSTER_DIR
{
    DIR         CurDir;
    DIR         PrevDir;
};
class CMonsterScript :
    public CScript
{
private:
    UINT          m_MstType;
    int           m_iRoomNum;
    CStateScript* m_pState;
    MONSTER_DIR   m_Dir;
    CGameObject*  m_pPlayer;
    float         m_fLengthtoPly;
    float         m_fAngle;
    Vec2          m_vDirToPly;
    Vec3          m_vPlayerPos;
    bool          m_bHitEffect;
    float         m_fHitEffectDelay;
    Vec3          m_vOwnerPos;

public:

    virtual void begin() override;
    virtual void tick() override;
    void CheckDir();
    void SetState();
    UINT GetType() { return m_MstType; }
    float GetLengthtoPly() { return m_fLengthtoPly; }
    Vec2 GetDirtoPly() { return m_vDirToPly; }
    Vec3 GetPlayerPos() { return m_vPlayerPos; }
    float GetDirtoPlyDegree() { return m_fAngle; }
    void SetHitEffect();
    

    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CMonsterScript);

    CMonsterScript();
    CMonsterScript(const CMonsterScript& _Other);
    ~CMonsterScript();
};

