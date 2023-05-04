#include "pch.h"
#include "CBossCharge.h"
#include "CBossScript.h"
#include <Engine/CAnim2D.h>
#include "CMstMissileScript.h"

CBossCharge::CBossCharge()
	: m_fDelay(4.f)
	, m_fAttackDelay(2.f)
	, m_iBulletCount(20)
{
}

CBossCharge::~CBossCharge()
{
}

void CBossCharge::tick()
{
	m_fDelay -= DT;
	m_fAttackDelay -= DT;

	if (m_fAttackDelay < 0.f)
	{
		CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Charge")->Stop();

		CResMgr::GetInst()->FindRes<CSound>(L"Galmea_ShotGun")->Play(1, 1.f, true);
		m_iBulletCount += 10;
		Ptr<CPrefab> pBullet = CResMgr::GetInst()->FindRes<CPrefab>(L"MstBullet");

		vector<Vec3> vecBulletPos = {};
		UINT Slice = m_iBulletCount;
		float fRadius = 250.f;
		float fTheta = XM_2PI / (float)m_iBulletCount;
		for (int i = 0; i < m_iBulletCount; ++i)
		{
			Vec3 BulletPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 1.f);
			vecBulletPos.push_back(BulletPos);
		}

		float Dir = GetOwner()->GetScript<CBossScript>()->GetDirtoPlyDegree();

		float Distance = Dir < 180 ? 120.f : -120;
		Vec3 vBossPos = GetOwner()->Transform()->GetWorldPos();
		for (int i = 0; i < m_iBulletCount; ++i)
		{
			Vec3 vDir = vecBulletPos[i].Normalize();
			CGameObject* Bullet = pBullet->Instantiate();
			Bullet->GetScript<CMstMissileScript>()->SetDir(vDir);
			SpawnGameObject(Bullet, Vec3(vBossPos.x + Distance, vBossPos.y - 50.f, vBossPos.z), (int)LAYER::MONSTERPROJECTILE);
		}
		m_fAttackDelay = 0.5;
	}

	if(m_fDelay < 0.f)
		ChangeState(L"Trace");
}

void CBossCharge::Enter()
{
	PlayAnim();
	GetOwner()->Animator2D()->GetCurAnim()->PlayStop();
	CGameObject* pCharge = new CGameObject;
	pCharge->AddComponent(new CTransform);
	pCharge->AddComponent(new CMeshRender);
	pCharge->AddComponent(new CAnimator2D);

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> Std2DMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl");

	pCharge->Transform()->SetRelativeScale(300.f, 300.f, 1.f);
	pCharge->MeshRender()->SetMesh(pMesh);
	pCharge->MeshRender()->SetMaterial(Std2DMtrl);
	pCharge->Animator2D()->LoadAnimFromFile(L"Charge");
	pCharge->Animator2D()->Play(L"Boss_Compression", true);
	pCharge->SetLifeSpan(2.f);

	float Dir = GetOwner()->GetScript<CBossScript>()->GetDirtoPlyDegree();

	float Distance = Dir < 180 ? 160.f : -160;
	Vec3 BossPos = GetOwner()->Transform()->GetWorldPos();
	SpawnGameObject(pCharge, Vec3(BossPos.x + Distance, BossPos.y - 50.f, BossPos.z - 100.f), (int)LAYER::MONSTERPROJECTILE);

	CResMgr::GetInst()->FindRes<CSound>(L"Galmea_Charge")->Play(0);
	GetOwner()->GetScript<CBossScript>()->SetRotable(false);
}

void CBossCharge::Exit()
{
	m_fDelay = 4.f;
	m_iBulletCount = 10;
	m_fAttackDelay = 2.f;
	GetOwner()->GetScript<CBossScript>()->SetRotable(true);
}

void CBossCharge::PlayAnim()
{
	GetOwner()->Animator2D()->Play(L"Boss_Left", false);
}
