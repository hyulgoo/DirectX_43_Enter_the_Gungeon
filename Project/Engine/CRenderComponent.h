#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>      m_pMesh;
    Ptr<CMaterial>  m_pSharedMtrl;
    Ptr<CMaterial>  m_pDynamicMtrl;
    Ptr<CMaterial>  m_pCurrentMtrl;

public:
    virtual void render() = 0;

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pCurrentMtrl; }
    Ptr<CMaterial> GetSharedMaterial() { return m_pSharedMtrl; }
    Ptr<CMaterial> GetDynamicMaterial();

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};

