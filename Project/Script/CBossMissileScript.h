#pragma once
#include <Engine\CScript.h>
enum class MISSILE_STATE
{
    START,
    UP,
    AIMING,
    DOWN,
    EXPLOSE,
};
class CBossMissileScript :
    public CScript
{
private:
    UINT          m_MissileState;
    float         m_fDelay;
    bool          m_bLoad;
    bool          m_bAddCollider;

public:
    virtual void begin() override;
    virtual void tick() override;


    virtual void BeginOverlap(CCollider2D* _Other) override;
    void PlayAnim();

    CLONE(CBossMissileScript);

    CBossMissileScript();
    ~CBossMissileScript();
};

