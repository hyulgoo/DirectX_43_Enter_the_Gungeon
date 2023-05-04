#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

class CAnimator :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;
public:

public:

public:
    CAnimator();
    ~CAnimator();
};

