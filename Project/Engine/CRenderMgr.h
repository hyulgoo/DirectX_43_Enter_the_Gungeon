#pragma once
#include "CSingleton.h"

class CCamera;
class CLight2D;
class CStructuredBuffer;

class CRay
{
private:
    Vec3 m_vOriginal;
    Vec3 m_vDirection;

public:
    void RayAtViewSpace(int ScreenX, int ScreenY);
    void RayAtWorldSpace(int ScreenX, int ScreenY);
public:
    CRay()
        : m_vOriginal(0.f, 0.f, 0.f)
        , m_vDirection(0.f, 0.f, 0.f)
    {

    }
    ~CRay()
    {

    }

    friend class CRenderMgr;
};
class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>            m_vecCam;       // Game �ȿ��� ���� Camera ���
    CCamera*                    m_pEditorCam;     // Editor��� Camera

    vector<tDebugShapeInfo>     m_vecShapeInfo; // �׵θ��� �׷��� ��ü������ ����(EditorObjMgr���� ������)

    vector<tLightInfo>          m_vecLight2D;   // ���� ������ ����, ī�޶�� �ٸ��� �ٲ� �� �־� ������ �� �־�� ��.
    CStructuredBuffer*          m_Light2DBuffer;

    void (CRenderMgr::* RENDER_FUNC)(void);

    Ptr<CTexture>               m_RTCopyTex;
    bool                        m_bUseEditCam;
    CRay*                       m_pRay;

public:
    void init();
    void render();

public:
    int RegisterCamera(CCamera* _Cam, int _idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_pEditorCam = _Cam; }
    void SetRenderFunc(bool _IsPlay);
    void RegisterLight2D(const tLightInfo& _Light2D) { m_vecLight2D.push_back(_Light2D); }
    void ClearCamera() { m_vecCam.clear(); }

    void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_vecShapeInfo.push_back(_info); }
    vector<tDebugShapeInfo>& GetDebugShapeInfo() { return m_vecShapeInfo; }

    bool IsEditCamUse() { return m_bUseEditCam; }
    Vec3 GetRay() { return m_pRay->m_vDirection; }

    CCamera* GetMainCam()
    {
        if (m_vecCam.empty())
            return nullptr;
        return m_vecCam[0];
    }

    CCamera* GetEditCam()
    {
        if (nullptr == m_pEditorCam)
            return nullptr;
        return m_pEditorCam;
    }

    void CopyRenderTarget();

private:
    void UpdateData();
    void render_play();
    void render_editor();
    void Clear();
};
