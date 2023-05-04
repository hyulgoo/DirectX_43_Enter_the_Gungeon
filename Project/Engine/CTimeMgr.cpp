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
	// 1�ʴ� ī���� ������
	QueryPerformanceFrequency(&m_llFrequency);
		
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{	
	QueryPerformanceCounter(&m_llCurCount);

	// tick ���� �ð�
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// �ð��� ����ٸ� DT�� 0���� ����
	if (m_bStop)
		m_fDeltaTime = 0.f;

	// ���� �ð�
	m_fTime += m_fDeltaTime;

	// �Լ� ȣ�� Ƚ��
	++m_iCallCount;

	// ���� ī��Ʈ ���� ���� ī��Ʈ�� ����
	m_llPrevCount = m_llCurCount;		

	// GlobalData ����
	GlobalData.tDT = m_fDeltaTime;
	GlobalData.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{
	// 1�ʿ� �ѹ�
	static wchar_t szBuff[256] = {};
	// 1�ʿ� �ѹ�
	if (1.f <= m_fTime)
	{
		swprintf_s(szBuff, L"FPS : %d, DT : %f", m_iCallCount, m_fDeltaTime);
		//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);

		m_fTime = 0.f;
		m_iCallCount = 0;
	}
}
