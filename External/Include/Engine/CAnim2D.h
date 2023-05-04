#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CAnimator2D.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;

    int                 m_iCurFrm;
    float               m_fTime;

    bool                m_bFinish;
    bool                m_bStop;
     
public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);

    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm]; }
    void SetCurFrm(tAnim2DFrm _FrmInfo, int _iFrmNum) { m_vecFrm[_iFrmNum] = _FrmInfo; }
    void SetCurFrmNum(int _iFrmNum) { m_iCurFrm = _iFrmNum; }

    void AddFrm(tAnim2DFrm _NewFrmInfo) { m_vecFrm.push_back(_NewFrmInfo); }

    const vector<tAnim2DFrm>& GetAllFrm() { return m_vecFrm; }
    void SetAllFrm(vector<tAnim2DFrm> _vAllFrm) { m_vecFrm = _vAllFrm; }
    const int GetCurFrmNum() { return m_iCurFrm; }

    Vec2 GetBackSize() { return m_vBackSize; }
    void SetBackSize(Vec2 _vSize) { m_vBackSize = _vSize; }

    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    void SetAtlasTex(Ptr<CTexture> _Atlas) { m_AtlasTex = _Atlas; }
    void PlayStop() {
        if (m_bStop)
        {
            m_bStop = false;
            return;
        }
        m_bStop = true;
    }
    void Stop(int _iFrmNum) { m_bStop = true; m_iCurFrm = _iFrmNum; }
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    CLONE(CAnim2D);
public:
    CAnim2D();
    CAnim2D(const CAnim2D& _Other);
    ~CAnim2D();

    friend class CAnimator2D;
};

