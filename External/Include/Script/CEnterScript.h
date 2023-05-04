#pragma once
#include <Engine\CScript.h>
class CEnterScript :
    public CScript
{
private:
    int m_iRoomNum;
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetRoomNum(int _iRoomNum) { m_iRoomNum = _iRoomNum; }

    virtual void BeginOverlap(CCollider2D* _Other) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEnterScript);
public:
    CEnterScript();
    CEnterScript(const CEnterScript& _Other);
    ~CEnterScript();
};

