#pragma once
#include <Engine/CScript.h>

class CStateScript;
class CGameObject;

struct PET_DIR
{
    DIR     CurDir;
    DIR     PrevDir;
};
class CPetScript :
    public CScript
{
private:
    CStateScript*   m_pState;
    PET_DIR         m_Dir;
    CGameObject*    m_pPlayer;
    float           m_fLengthtoPly;
    float           m_fAngle;
    Vec2            m_vDirToPly;
    Vec3            m_vPlayerPos;

public:
    virtual void begin() override;
    virtual void tick() override;
    void CheckDir();
    void SetState();
    float GetLengthtoPly() { return m_fLengthtoPly; }
    Vec2 GetDirtoPly() { return m_vDirToPly; }
    Vec3 GetPlayerPos() { return m_vPlayerPos; }

    CLONE(CPetScript);

public:
    CPetScript();
    ~CPetScript();

};

