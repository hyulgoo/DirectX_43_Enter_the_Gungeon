#pragma once
#include "CState.h"
class CPlyWalk :
    public CState
{
private:
    CGameObject*    m_pWalkEffect;
    wstring         m_strAnimName[(UINT)DIR::END];
    bool            m_MoveStop[4];
    map<UINT, UINT> m_OtherCollider;
    float           m_fDelay;
    float           m_fSoundDelay;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

    virtual void PlayAnim() override;
    void SetAnimName(wstring _strUp, wstring _strDown, wstring _strLeftUp
        , wstring _strLeftDown, wstring _strRightUp, wstring _strRightDown);
    void Move();
    bool IsMovable(int _iDirNum) { return m_MoveStop[_iDirNum]; }
    void Clear() {
        for (int i = 0; i < 4; ++i)
        {
            m_MoveStop[i] = true;
        }
        m_OtherCollider.clear();
    }
    

    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void OnOverlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);

    CLONE(CPlyWalk);

public:
    CPlyWalk();
    ~CPlyWalk();
};

