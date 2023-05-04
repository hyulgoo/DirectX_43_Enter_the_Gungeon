#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;
    CAnim2D*                m_pCurAnim;
    bool                    m_bRepeat;

public:
    virtual void finaltick() override;
    void UpdateData();
    void Clear();

public:
    void Play(const wstring& _strName, bool _bRepeat);
    CAnim2D* FindAnim(const wstring& _strName);
    CAnim2D* GetCurAnim() { return m_pCurAnim; }
    map<wstring, CAnim2D*> GetAllAnim() { return m_mapAnim; }
    void SetAnim(CAnim2D* _pAnim, bool _bRepeat);
    void DeleteAnim(const wstring& _strName);
    bool IsFinish();

    // ��Ʋ�󽺸� �ʿ��� ũ��� �ڸ��� ���� �μ� �ִϸ��̼� �̸�, �ؽ��� �»��, ������, ������ ����, �ʴ� ��� ����
    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);
    void LoadAnimFromFile(const wstring& _strRelativePath);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CAnimator2D);
public:
    CAnimator2D(); 
    CAnimator2D(const CAnimator2D& _other);
    ~CAnimator2D();
};

