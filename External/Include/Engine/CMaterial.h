#pragma once
#include "CRes.h"

#include "ptr.h"
#include "CGraphicsShader.h"
#include "CTexture.h"

class CMaterial :
    public CRes
{
private:
    Ptr<CGraphicsShader>    m_pShader;

    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEX_END];

public:
    void SetScalarParam(SCALAR_PARAM _Param, const void* _Src);
    void SetTexParam(TEX_PARAM _Param, const Ptr<CTexture>& _Tex);

    void GetScalarParam(SCALAR_PARAM _param, void* _pData);
    Ptr<CTexture> GetTexParam(TEX_PARAM _param) { return m_arrTex[(UINT)_param]; }

    void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }

    Ptr<CTexture>* GetArrTex() { return m_arrTex; }
    virtual void UpdateData() override;

private:
    virtual int Load(const wstring& _strFilePath);
public:
    virtual int Save(const wstring& _strRelativePath);

    CLONE(CMaterial);
public:
    CMaterial(bool _bEngine = false);
    ~CMaterial();
};

