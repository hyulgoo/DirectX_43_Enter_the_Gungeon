#pragma once

class CEngine 
    : public CSingleton<CEngine>
{
private:
    HWND m_hWnd;

public:
    void init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
    void progerss();

public:
    CEngine();
    ~CEngine();
};

