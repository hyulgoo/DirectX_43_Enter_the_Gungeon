#include "pch.h"
#include "CSetColorShader.h"

CSetColorShader::CSetColorShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CSetColorShader::~CSetColorShader()
{
}

void CSetColorShader::UpdateData()
{
	// UA에 UpdateData
	m_OutTex->UpdateData_CS(0, true);

	// 해상도에 맞춰 필요한 스레드 그룹 개수를 구함
	m_iGroupX = (UINT)(m_OutTex->Width() / m_iGroupPerThreadX);
	m_iGroupY = (UINT)(m_OutTex->Height() / m_iGroupPerThreadY);
	m_iGroupZ = 1;
}

void CSetColorShader::Clear()
{
	m_OutTex->Clear_CS(true);
}
