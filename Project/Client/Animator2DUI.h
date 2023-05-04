#pragma once
#include "ComponentUI.h"

class CTexture;
class CAnim2D;

class Animator2DUI :
    public ComponentUI
{
private:  
    map<string, bool>             m_mapStrData;
    vector<tAnim2DFrm>            m_vecAllFrm;
    map<wstring, CAnim2D*>        m_mapAllAnim;
    vector<tAnim2DFrm>            m_PrevAllAnim;
    float                         m_AtlasRatio;
    vector<tAnim2DFrm>            m_vecNewAnim;
    int                           m_iNewAnimFrmNum;
    Vec2                          m_NewAnimLtp;
    Vec2                          m_NewAnimSlice;
    int                           m_NewAnimFPS;
    Vec2                          m_NewAnimBackSize;

    bool                          m_ShowEditor;
    bool                          m_ShowCreator;

    CAnim2D*                      m_CurAnim;
    tAnim2DFrm                    m_CurFrm;

    Ptr<CTexture>                 m_AnimAtlas;

    ImVec2                        m_DragLtp;
    ImVec2                        m_DragBtm;
    ImVec2                        m_CreateTexSize;

    tAnim2DFrm                    m_CreateFrmInfo;
    ImVec2                        m_CreateFrmLtp;
    ImVec2                        m_CreateFrmBtm;
    ImVec2                        m_CreateFrmSize;

public:
    virtual int render_update() override;
    void SetAnimInfo();

    void ShowAnimList(bool& _ShowEditor);
    void ShowCurAnim();
    void ShowAnimEditor(bool& _bShow);
    void ShowCreateAnimation();

    void EditCurFrm();

    void SelectAnim(DWORD_PTR _Key);
    void SelectAtlas(DWORD_PTR _Key);

    void Save();
    void Load();
public:
    Animator2DUI();
    ~Animator2DUI();
};


