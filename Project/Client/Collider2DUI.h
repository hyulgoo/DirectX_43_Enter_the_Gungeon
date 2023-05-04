#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:
    bool    m_bShowEdieWave;
    int     m_iPrefabNum;
    Vec2    m_vSpawnPos;

public:
    virtual int render_update() override;
    void ShowWaveEditor();

public:
    Collider2DUI();
    ~Collider2DUI();
};

