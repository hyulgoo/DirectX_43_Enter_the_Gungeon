#pragma once
#include <Engine/CScript.h>

class CStateScript;

struct PLAYER_DIR
{
    DIR         CurDir;
    DIR         PrevDir;
};


class CPlayerScript :
    public CScript
{
private:
    CStateScript*   m_pState;
    WEAPON_TYPE     m_tCurWeapon;

    float           m_fSpeed;
    PLAYER_DIR      m_Dir;
    float           m_Invincible_Time;
    bool            m_bHitEffect;
    float           m_fHitEffectDelay;
    COLLISION_DIR   m_CollisionDir;
    int             m_iBlankBullet;
    

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

    void SetState();
    void CheckDir();
    void ChangeWeaponType(WEAPON_TYPE _Weapon);
    void SetInvincibleTime(float _fTime) { m_Invincible_Time = _fTime; }
    bool IsInvincible() 
    { 
        bool Invincible = false;
        if (0.f < m_Invincible_Time)
            Invincible = true;
        else
            Invincible = false;
        
        return Invincible; 
    }
    void SetHitEffect();
    COLLISION_DIR GetCollisionDir() { return m_CollisionDir; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

