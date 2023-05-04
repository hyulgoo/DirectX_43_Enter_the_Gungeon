#pragma once
#include "ComponentUI.h"

struct tTileUIInfo
{
    CTileMap*       TileMap;
    Ptr<CTexture>   Atlas;
    UINT            XCount;
    UINT            YCount;
    ImVec2          Slice;
};

class CTileMap;

class TileMapUI :
    public ComponentUI
{
private:
    tTileUIInfo         m_CurTileInfo;
    tTileUIInfo         m_UI;
    tTileUIInfo         m_EditTileInfo;
    tTile               m_SelectTile;
    map<UINT, tTile>    m_mapTile;
    tTile               m_ClickTile;
    bool                m_bEditState;
    UINT                m_iTileState;
    bool                m_bShowFrame;
   

public:
    virtual int render_update() override;

private:
    void ShowTileEdit();
    void EditCurTile();

    void SelectAtlas(DWORD_PTR _Key);

public:
    TileMapUI();
    ~TileMapUI();
};





