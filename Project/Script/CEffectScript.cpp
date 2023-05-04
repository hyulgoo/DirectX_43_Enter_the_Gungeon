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

	// �߻� ����Ʈ�� ���������� tick�� ������ ����
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
		// weaponscript���� shoot �Լ� ���� �� ȣ����.
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
	// ���� ��ġ ����, �÷��̾ �������� ���콺���� ���� * RelativeScale / 2
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
	// ���� ���� ����
	m_fDir = GetMouseDir(m_vPlayerPos) * (180.f / XM_PI);

	// Player�� ���콺 ��ġ�� ���� �¿� ������ �ϱ� ������ �׿� �°� ȸ������ ����
	if (m_fDir > 180)
	{
		// �Ʒ��� �������� ȸ����Ű�� ������ �̹����� �Ʒ��� �ٶ󺸰� -90��
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
