#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CTileMap :
    public CRenderComponent
{
private:
    UINT                m_iTileCountX; // 타일 가로 개수
    UINT                m_iTileCountY; // 타일 세로 개수

    Ptr<CTexture>       m_Atlas;
    Vec2                m_vSliceSize;

    vector<tTile>       m_vecTile;
    CStructuredBuffer*  m_Buffer;

    bool                m_bShowFrame;
    float               m_Framethick;
    bool                m_bSet;

public:
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void render() override;

    void UpdateData();

    void SetAtlas(Ptr<CTexture> _AtlasKey) { m_Atlas = _AtlasKey; }
    Ptr<CTexture> GetAtlas() { return m_Atlas; }
    vector<tTile>& GetvecTile() { return m_vecTile; }

    Vec2 GetSliceSize() { return m_vSliceSize; }
    Vec2 GetTileCount() { return Vec2((float)m_iTileCountX, (float)m_iTileCountY); }
    void SetTileCount(UINT _iXCount, UINT _iYCount);
    void SetSliceSize(Vec2 _vSliceSize) { m_vSliceSize = _vSliceSize; }
    void SetDrawFrame();
    void UseFrame(float _thickness) { m_bShowFrame = true; m_Framethick = _thickness; }
    void SetTile(int _Num, tTile _Tile) { m_vecTile[_Num] = _Tile; }
    tTile GetTile(int _Num) { return m_vecTile[_Num]; }
    void SetTileState(int _Num, TILESTATE _tState) { m_vecTile[_Num].State = (UINT)_tState; }
    void SetTilePos();
    void ClearTile();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    int Save(const wstring& _strRelativePath);
    int Load(const wstring& _strFilePath);

    CLONE(CTileMap);
public:
    CTileMap();
    ~CTileMap();
};

