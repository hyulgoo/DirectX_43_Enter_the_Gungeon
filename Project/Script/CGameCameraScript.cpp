#include "pch.h"
#include "CGameCameraScript.h"

#include <Engine/CRenderMgr.h>

CGameCameraScript::CGameCameraScript()
	: CScript((UINT)SCRIPT_TYPE::GAMECAMERASCRIPT)
	, m_fMoveDistance(150.f)
	, m_fMaxLength(800.f)
	, m_vPlayerPos{}
	, m_vDir{}
	, m_Player(nullptr)
	, m_vOffset{}
	, m_fAccTime(0.f)
	, m_fMaxTime(0.f)
	, m_fRange(0.f)
	, m_fShakeSpeed(0.f)
	, m_fShakeDir(0.f)
	, m_bCamShake(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Length, "Length");
}

CGameCameraScript::~CGameCameraScript()
{
}
void CGameCameraScript::begin()
{
}

void CGameCameraScript::tick()
{
	if(nullptr == m_Player )
		m_Player = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	CameraMove();
	ShackCamera();
}

void CGameCameraScript::CameraShake(float _fRange, float _fShakeSpeed, float _fTerm)
{
	m_fAccTime = 0.f;
	m_fMaxTime = _fTerm;
	m_fRange = _fRange;
	m_fShakeSpeed = _fShakeSpeed;
	m_fShakeDir = 1.f;
	m_bCamShake = true;
}

void CGameCameraScript::ShackCamera()
{
	if (!m_bCamShake)
		return;

	m_fAccTime += DT;

	if (m_fMaxTime <= m_fAccTime)
	{
		m_bCamShake = false;
		m_vOffset = Vec2(0.f, 0.f);
	}

	m_vOffset.x += DT * m_fShakeSpeed * m_fShakeDir;
	m_fShakeSpeed -= m_fShakeSpeed * m_fMaxTime * DT;
	if (m_fRange < fabsf(m_vOffset.x))
	{
		m_vOffset.x = m_fRange * m_fShakeDir;
		m_fShakeDir *= -1;
	}
}

void CGameCameraScript::CameraMove()
{
	m_vPlayerPos = m_Player->Transform()->GetWorldPos();

	m_vDir = CRenderMgr::GetInst()->GetRay();
	Vec2 vDiff = Vec2(m_vDir.x - m_vPlayerPos.x, -m_vDir.y + m_vPlayerPos.y);
	float Length = abs(vDiff.Length());
	Length = Length > 200.f ? Length - 200.f : 0.f;
	float Ratio = Length > m_fMaxLength ? 1.f : Length / m_fMaxLength;
	float Distance = Ratio * m_fMoveDistance;

	vDiff.Normalize();


	Transform()->SetRelativePos(Vec3(m_vPlayerPos.x + vDiff.x * Distance + m_vOffset.x, m_vPlayerPos.y - vDiff.y * Distance + m_vOffset.y, 1.f));
}

void CGameCameraScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fMoveDistance, sizeof(float), 1, _File);
}

void CGameCameraScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fMoveDistance, sizeof(float), 1, _File);
}

