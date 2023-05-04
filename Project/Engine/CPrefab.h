#pragma once
#include "CRes.h"

class CGameObject;

class CPrefab :
    public CRes
{
private:
    CGameObject* m_ProtoObj;
public:
    CGameObject* Instantiate();

private:
    virtual int Load(const wstring& _strFilePath);
public:
    virtual int Save(const wstring& _strRelativePath);

private:
    virtual void UpdateData() override {}
    void SaveProtoObj(CGameObject* _Obj, FILE* _File);
    CGameObject* LoadProtoObj(FILE* _File);

public:
    void RegisterProtoObject(CGameObject* _Proto);

public:
    CPrefab();
    ~CPrefab();
    
    friend class PrefabUI;
};

