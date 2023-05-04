#include "pch.h"
#include "CAnim2D.h"

#include "CTimeMgr.h"
#include "CResMgr.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_bStop(false)
{
}

CAnim2D::CAnim2D(const CAnim2D& _Other)
	: m_pOwner(nullptr)
	, m_vecFrm(_Other.m_vecFrm)
	, m_vBackSize(_Other.m_vBackSize)
	, m_iCurFrm(0)
	, m_bFinish(_Other.m_bFinish)
	, m_fTime(0.f)
	, m_bStop(false)
	, m_AtlasTex(_Other.m_AtlasTex)
{

}

CAnim2D::~CAnim2D()
{
}

void CAnim2D::finaltick()
{
	if (m_bFinish)
		return;

	if (!m_bStop)
		m_fTime += DT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;
		++m_iCurFrm;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

	m_AtlasTex = _AtlasTex;

	Vec2 vResolution = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_vBackSize = _vBackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
		frm.SliceUV = _vSlice / vResolution;

		m_vecFrm.push_back(frm);
	}
}
