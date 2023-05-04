#include "pch.h"
#include "CBossTrace.h"
#include "CMstMissileScript.h"
#include "CBossScript.h"

CBossTrace::CBossTrace()
	: m_fATK_Delay(0.f)
	, m_tPatternTime(7.f)
	, m_fLength(0.f)
{
}

CBossTrace::~CBossTrace()
{
}

void CBossTrace::tick()
{
	// �⺻������ �÷��̾ ���� �����ϸ�, �����ð� ���� �� ��� ���ų� �ٸ� �������� �ٲ�� ��.
	m_fLength = GetOwner()->GetScript<CBossScript>()->GetLengthtoPly();


	Vec3 CurPos = GetOwner()->Transform()->GetWorldPos();
	Vec2 vDirtoPly = GetOwner()->GetScript<CBossScript>()->GetDirtoPly();

	// ���� �Ÿ� �̻� �������ִٸ� �÷��̾� ����
	if(m_fLength > 200.f)
	{
		Vec3 Diff = Vec3(vDirtoPly.x * 210.f * DT, vDirtoPly.y * 210.f * DT, 0.f);
		GetOwner()->Transform()->SetRelativePos(CurPos + Diff);
	}

	// ���� ���� �ð��� �� ������ �������� �� �������� ChangeState
	m_tPatternTime -= DT;
	if (m_tPatternTime < 0.f)
	{
		int pattern = rand() % 3;
		switch (pattern)
		{
		case 0:
			ChangeState(L"ATK_Jump");
			break;
		case 1:
			ChangeState(L"ATK_Charge");
			break;
		case 2:
			ChangeState(L"ATK_Missile");
			break;
		}
		return;
	}

	// r
	if (m_fBreakTime > 0.f)
	{
		m_fBreakTime -= DT;
		return;
	}

	// �����ð� �� ��� �Ѿ˼��ʸ� ���� �ð�
	m_fATK_Delay += DT; 
	if (m_fATK_Delay > 0.2f)
	{
		m_iBreakCount += 1;
		if (25 == m_iBreakCount)
			m_fBreakTime = 1.f;
		m_fATK_Delay -= 0.07f;
		// �Ѿ��� ���� �ޱ� �� ���ǵ� ����
		int RandomRange = 15;
		float RandomAngle = rand() % RandomRange;
		RandomAngle /= 57.3f;
		CGameObject* MstBullet = CResMgr::GetInst()->FindRes<CPrefab>(L"MstBullet")->Instantiate();
		float X = acos(vDirtoPly.x);
		float Y = asin(vDirtoPly.y);
		X += RandomAngle - RandomRange / 2.f / 57.3f;
		Y += RandomAngle - RandomRange / 2.f / 57.3f;
		Vec2 vDir = Vec2(cos(X), sin(Y));


		float Speed = 600.f;
		float RandomSpeed = rand() % 200;
		Speed += RandomSpeed;

		// �Ѿ� ������ �� ������ �ž��ϹǷ� �߻�������� �о���.
		Vec3 SpawnPos = CurPos + (m_vBulletSpawnPos * 130.f);

		MstBullet->GetScript<CMstMissileScript>()->SetDir(vDir);
		MstBullet->GetScript<CMstMissileScript>()->SetSpeed(Speed);
		SpawnGameObject(MstBullet, SpawnPos, (int)LAYER::MONSTERPROJECTILE);
	}
}

void CBossTrace::Enter()
{
	PlayAnim();
	m_tPatternTime = 7.f;
	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Geathle")->Play(0);
}

void CBossTrace::Exit()
{
	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Geathle")->Stop();
}

void CBossTrace::PlayAnim()
{
	switch (GetDir())
	{
	case DIR::UP:
		GetOwner()->Animator2D()->Play(L"Boss_Up", true);
		m_vBulletSpawnPos = Vec2(-0.4f, 1.f);
		break;
	case DIR::DOWN:
		GetOwner()->Animator2D()->Play(L"Boss_Down", true);
		m_vBulletSpawnPos = Vec2(0.4f, -1.f);
		break;
	case DIR::LEFT_UP:
		GetOwner()->Animator2D()->Play(L"Boss_Diagonal_Left_Up", true);
		m_vBulletSpawnPos = Vec2(-1.f, 0.6f);
		break;
	case DIR::LEFT:
		GetOwner()->Animator2D()->Play(L"Boss_Left", true);
		m_vBulletSpawnPos = Vec2(-1.2f, -0.38f);
		break;
	case DIR::LEFT_DOWN:
		GetOwner()->Animator2D()->Play(L"Boss_Diagonal_Left_Down", true);
		m_vBulletSpawnPos = Vec2(-1.2f, -1.f);
		break;
	case DIR::RIGHT_UP:
		GetOwner()->Animator2D()->Play(L"Boss_Diagonal_Left_Up", true);
		m_vBulletSpawnPos = Vec2(1.f, 0.6f);
		break;
	case DIR::RIGHT_DOWN:
		GetOwner()->Animator2D()->Play(L"Boss_Diagonal_Left_Down", true);
		m_vBulletSpawnPos = Vec2(1.2f, -1.f);
		break;
	case DIR::RIGHT:
		GetOwner()->Animator2D()->Play(L"Boss_Left", true);
		m_vBulletSpawnPos = Vec2(1.2f, -0.38f);
		break;
	}
	m_vBulletSpawnPos.Normalize();
}
