#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"


CTimeMgr::CTimeMgr()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_iCallCount(0)
	, m_fDeltaTime(0.f)
	, m_fTime(0.f)
	, m_bStop(false)
{

}

CTimeMgr::~CTimeMgr()
{

}



void CTimeMgr::init()
{
	// 1초당 카운팅 증가량
	QueryPerformanceFrequency(&m_llFrequency);
		
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{	
	QueryPerformanceCounter(&m_llCurCount);

	// tick 간격 시간
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// 시간을 멈춘다면 DT를 0으로 고정
	if (m_bStop)
		m_fDeltaTime = 0.f;

	// 누적 시간
	m_fTime += m_fDeltaTime;

	// 함수 호출 횟수
	++m_iCallCount;

	// 이전 카운트 값을 현재 카운트로 갱신
	m_llPrevCount = m_llCurCount;		

	// GlobalData 갱신
	GlobalData.tDT = m_fDeltaTime;
	GlobalData.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{
	// 1초에 한번
	static wchar_t szBuff[256] = {};
	// 1초에 한번
	if (1.f <= m_fTime)
	{
		swprintf_s(szBuff, L"FPS : %d, DT : %f", m_iCallCount, m_fDeltaTime);
		//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);

		m_fTime = 0.f;
		m_iCallCount = 0;
	}
}
