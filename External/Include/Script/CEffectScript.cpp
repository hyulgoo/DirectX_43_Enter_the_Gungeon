#include "pch.h"
#include "CEffectScript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CAnim2D.h>

void CEffectScript::SaveToLevelFile(FILE* _File)
{
	
}

void CEffectScript::LoadFromLevelFile(FILE* _File)
{
	//m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
	//Animator2D()->LoadAnimFromFile(L"Effect");
	//Animator2D()->Play(L"Shot_Effect", true);
	//Transform()->SetAbsolute(true);
	//Transform()->SetRelativeScale(0.f, 0.f, 0.f);
}

CEffectScript::CEffectScript()
	: CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_pPlayer(nullptr)
	, m_vPlayerPos{}
    , m_fDir(0.f)
    , m_fDelay(0.f)
    , m_bOnOff(false)
{
}

CEffectScript::CEffectScript(const CEffectScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT)
	, m_pPlayer(nullptr)
	, m_vPlayerPos{}
	, m_fDir(0.f)
	, m_fDelay(0.f)
	, m_bOnOff(false)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	if(nullptr == m_pPlayer)
	{
		m_pPlayer = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"Player");
		Animator2D()->LoadAnimFromFile(L"Effect");
		Animator2D()->Play(L"Shot_Effect", true);
		Transform()->SetAbsolute(true);
		Transform()->SetRelativeScale(0.f, 0.f, 0.f);
	}
}

void CEffectScript::tick()
{
	SetRelativePos();
	SetRelativeRot();

	// 발사 이펙트가 꺼져있으면 tick을 돌리지 않음
	if (!m_bOnOff)
		return;
	else
	{
		if (m_fDelay > 0.f)
			m_fDelay -= DT;
		else
		{
			Transform()->SetRelativeScale(0.f, 0.f, 0.f);
			m_bOnOff = true;
		}
	}
}

void CEffectScript::SetEffect(bool _Onoff)
{
	if (_Onoff)
	{
		// weaponscript에서 shoot 함수 실행 시 호출함.
		Transform()->SetRelativeScale(30.f, 30.f, 1.f);
		Animator2D()->GetCurAnim()->Reset();
		m_fDelay = 0.19f;
		m_bOnOff = true;
	}
	else
		Transform()->SetRelativeScale(0.f, 0.f, 0.f);
}

void CEffectScript::SetRelativePos()
{
	m_vPlayerPos = GetOwner()->GetParent()->Transform()->GetWorldPos();
	// 총의 위치 설정, 플레이어를 기준으로 마우스까지 방향 * RelativeScale / 2
	Vec3 TargetPos = m_vPlayerPos;
	Vec3 Cursor = CRenderMgr::GetInst()->GetRay();
	Vec2 vDir = Vec2(Cursor.x - TargetPos.x, (Cursor.y + 30.f) - TargetPos.y);
	vDir.Normalize();

	float angle = (float)acos(vDir.Dot(Vec2(0.f, -1.f)));
	if (vDir.x < 0.f)
		angle = (360.f / 180.f * XM_PI) - angle;

	if (angle * (180.f / XM_PI) > 180.f)
		Transform()->SetRelativePos(Vec3(-vDir.x * 100.f, vDir.y * 100.f, 0.1f));
	else
		Transform()->SetRelativePos(Vec3(vDir.x * 100.f, vDir.y * 100.f, 0.1f));
}

void CEffectScript::SetRelativeRot()
{
	// 무기 방향 설정
	m_fDir = GetMouseDir(m_vPlayerPos) * (180.f / XM_PI);

	// Player가 마우스 위치에 따라 좌우 반전을 하기 떄문에 그에 맞게 회전각도 조절
	if (m_fDir > 180)
	{
		// 아래를 기준으로 회전시키기 떄문에 이미지를 아래를 바라보게 -90도
		m_fDir -= 90.f;
		m_fDir = 180.f - m_fDir;
		m_fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, 0.f, m_fDir));
	}
	else
	{
		m_fDir -= 90.f;
		m_fDir *= (XM_PI / 180.f);
		Transform()->SetRelativeRot(Vec3(0.f, 0.f, m_fDir));
	}
}
