#pragma once


class CTimeMgr
	: public CSingleton<CTimeMgr>
{	
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	float			m_fDeltaTime;
	float			m_fTime;
	bool			m_bStop;

public:
	void init();
	void tick();
	void render();
	void Stop() { m_bStop = true; };
	void Play() { m_bStop = false; }

public:
	float GetDeltaTime() { return m_fDeltaTime; }
};

