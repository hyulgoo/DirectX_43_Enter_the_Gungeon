#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"

CParticleSystem::CParticleSystem(PARTICLE _tParticle)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_RWBuffer(nullptr)
	, m_ModuleDataBuffer(nullptr)
	, m_tParticleType(_tParticle)
	, m_ModuleData{}
	, m_AccTime(0.f)
{
	// ��ƼŬ ���� �޽� ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	// ��ƼŬ ���� ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	SetModuleInfo();
	// ��ƼŬ ������Ʈ ��ǻ�� ���̴�
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Other)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_tParticleType(_Other.m_tParticleType)
	, m_ModuleData(_Other.m_ModuleData)
	, m_AccTime(0.f)
{
	// ��ƼŬ ���� �޽� ����
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	// ��ƼŬ ���� ���� ����
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	SetModuleInfo();
	// ��ƼŬ ������Ʈ ��ǻ�� ���̴�
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_ModuleData.iMaxParticleCount, SB_TYPE::READ_WRITE, false);

	m_RWBuffer = new CStructuredBuffer;
	m_RWBuffer->Create(sizeof(tRWParticleBuffer), 1, SB_TYPE::READ_WRITE, true);

	m_ModuleDataBuffer = new CStructuredBuffer;
	m_ModuleDataBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::READ_ONLY, true);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
	{
		delete m_ParticleBuffer;
		m_ParticleBuffer = nullptr;
	}

	if (nullptr != m_RWBuffer)
	{
		delete m_RWBuffer;
		m_RWBuffer = nullptr;
	}

	if (nullptr != m_ModuleDataBuffer)
	{
		delete m_ModuleDataBuffer;
		m_ModuleDataBuffer = nullptr;
	}
}

void CParticleSystem::SetModuleInfo()
{
	switch (m_tParticleType)
	{
	case PARTICLE::PLAYER_WALK:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"PLAYERWALK");
		SpawnModule(3, 2, Vec3(255.f, 255.f, 255.f), Vec3(20.f, 20.f, 1.f), Vec3(25.f, 25.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.4f, 0.4f);
		AnimationModule(8, 0, Vec2(0.f, 0.f), Vec2(21.f, 21.f), Vec2(0.f, 40.f));
	}
	break;
	case PARTICLE::PLAYER_JUMP:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"PLAYERJUMP");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(60.f, 40.f, 1.f), Vec3(60.f, 40.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.7f, 0.7f);
		AnimationModule(4, 0, Vec2(0.f, 0.f), Vec2(36.f, 28.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::BOSS_JUMP:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"PLAYERJUMP");
		SpawnModule(1, 10, Vec3(255.f, 255.f, 255.f), Vec3(360.f, 240.f, 1.f), Vec3(360.f, 240.f, 1.f), Vec3(1.f, 1.f, 1.f), 1.1f, 1.1f);
		AnimationModule(4, 0, Vec2(0.f, 0.f), Vec2(36.f, 28.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::SHADOW:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"DEFAULT_SHADOW");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(60.f, 60.f, 1.f), Vec3(60.f, 60.f, 1.f), Vec3(1.f, 1.f, 1.f), 500.f, 500.f);
		//AnimationModule(1, 1, Vec2(0.f, 0.f), Vec2(32.f, 32.f), Vec2(0.f, 40.f));
	}
	break;
	case PARTICLE::BOSS_SHADOW:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"DEFAULT_SHADOW");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(300.f, 300.f, 1.f), Vec3(300.f, 300.f, 1.f), Vec3(1.f, 1.f, 1.f), 500.f, 500.f);
	}
	break;
	case PARTICLE::SHOT_FIRE:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"SHOT_FIRE");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 40.f, 1.f), Vec3(40.f, 40.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.3f, 0.3f);
		AnimationModule(4, 0, Vec2(0.f, 0.f), Vec2(56.f, 56.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::SHOT_EFFECT:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"SHOT_EFFECT");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 40.f, 1.f), Vec3(40.f, 40.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.2f, 0.2f);
		AnimationModule(3, 0, Vec2(0.f, 0.f), Vec2(15.f, 11.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::BOMB:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"BOMB");
		SpawnModule(1, 0.7f, Vec3(255.f, 255.f, 255.f), Vec3(558.f, 438.f, 1.f), Vec3(558.f, 438.f, 1.f), Vec3(1.f, 1.f, 1.f), 2.f, 2.f);
		AnimationModule(17, 0, Vec2(0.f, 0.f), Vec2(279, 219), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::BULLET_BLANK:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"BLANKSHOT");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 40.f, 1.f), Vec3(40.f, 40.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.5f, 0.5f);
		ColorChangeModule(Vec3(255.f, 212.f, 0.f), Vec3(255.f, 0.f, 0.f));
		AnimationModule(15, 0, Vec2(0.f, 0.f), Vec2(54.f, 54.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::BULLET_DIE:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_DIE");
		SpawnModule(1, 10, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 40.f, 1.f), Vec3(35.f, 35.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.7f, 0.7f);
		AnimationModule(7, 0, Vec2(0.f, 0.f), Vec2(27.f, 27.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::BULLET_HIT:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"BULLET_HIT");
		SpawnModule(1, 10, Vec3(255.f, 255.f, 255.f), Vec3(25.f, 25.f, 1.f), Vec3(25.f, 25.f, 1.f), Vec3(1.f, 1.f, 1.f), 0.4f, 0.4f);
		AnimationModule(5, 0, Vec2(0.f, 0.f), Vec2(16.f, 16.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::ENEMY_APPEAR:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"ENEMYAPPEAR");
		SpawnModule(1, 0.7f, Vec3(255.f, 255.f, 255.f), Vec3(100.f, 100.f, 1.f), Vec3(100.f, 100.f, 1.f), Vec3(1.f, 1.f, 1.f), 2.5f, 2.5f);
		AnimationModule(22, 0, Vec2(0.f, 0.f), Vec2(78.f, 80.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::FIRE:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"FIRE");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(30.f, 30.f, 1.f), Vec3(30.f, 30.f, 1.f), Vec3(1.f, 1.f, 1.f), 3.f, 3.f);
		AnimationModule(24, 8, Vec2(0.f, 0.f), Vec2(22.f, 22.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::TORCH_SIDE:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"TORCH_SIDE");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 50.f, 1.f), Vec3(40.f, 50.f, 1.f), Vec3(1.f, 1.f, 1.f), 3.f, 3.f);
		AnimationModule(10, 0, Vec2(0.f, 0.f), Vec2(9.f, 15.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::TORCH_FRONT:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"TORCH_FRONT");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(30.f, 75.f, 1.f), Vec3(30.f, 75.f, 1.f), Vec3(1.f, 1.f, 1.f), 3.f, 3.f);
		AnimationModule(10, 0, Vec2(0.f, 0.f), Vec2(9.f, 23.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::LAMP:
	{
		m_UseTex = CResMgr::GetInst()->FindRes<CTexture>(L"LAMP");
		SpawnModule(1, 1, Vec3(255.f, 255.f, 255.f), Vec3(40.f, 64.f, 1.f), Vec3(40.f, 64.f, 1.f), Vec3(1.f, 1.f, 1.f), 1.5f, 1.5f);
		AnimationModule(5, 0, Vec2(0.f, 0.f), Vec2(11.f, 19.f), Vec2(0.f, 0.f));
	}
	break;
	case PARTICLE::END:
	{
		SpawnModule(1, 0, Vec3(0.f, 0.f, 0.f), Vec3(30.f, 30.f, 1.f), Vec3(30.f, 30.f, 1.f), Vec3(1.f, 1.f, 1.f), 200.f, 200.f);
	}
	break;
	}
}

void CParticleSystem::SetModuleData(PARTICLE _tParticle)
{
	m_tParticleType = _tParticle;
	SetModuleInfo();
}

void CParticleSystem::begin()
{
	SetModuleInfo();
}

void CParticleSystem::finaltick()
{
	// ��������Ʈ, �����ð�
	float fTimePerCount = 1.f / (float)m_ModuleData.SpawnRate;
	m_AccTime += DT;

	// �����ð��� �����ð��� �Ѿ��
	if (fTimePerCount < m_AccTime)
	{
		// �ʰ��ϴ� ���� -> ���� ����
		float fData = m_AccTime / fTimePerCount;

		// �������� �����ð�����
		m_AccTime = fTimePerCount * (fData - (float)floor(fData));

		// RW������ �������� ���������� ���ٸ� ���� ����� �÷���.
		if(m_RWBuffer->GetElementCount() < (UINT)fData)
			m_RWBuffer->Create(sizeof(tRWParticleBuffer), (UINT)fData, SB_TYPE::READ_WRITE, true);
		// ���ۿ� ����ī��Ʈ ����
		tRWParticleBuffer rwbuffer = {(int)fData};
		m_RWBuffer->SetData(&rwbuffer);
	}

	// ��ƼŬ ������Ʈ
	m_ModuleDataBuffer->SetData(&m_ModuleData);

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetRWParticleBuffer(m_RWBuffer);
	m_UpdateCS->SetModuleData(m_ModuleDataBuffer);
	m_UpdateCS->SetNoiseTexture(CResMgr::GetInst()->FindRes<CTexture>(L"noise_01"));
	m_UpdateCS->SetParticleObjectPos(Transform()->GetWorldPos());

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	// shader���� view �� proj�� ����ϱ� ���� ������Ʈ ������.
	Transform()->UpdateData();

	// ��ƼŬ ���۸� t20�� ���ε�
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// ��� ������ t21�� ���ε�
	m_ModuleDataBuffer->UpdateData(21, PIPELINE_STAGE::PS_GEOMETRY);

	// ������Ʈ
	GetMaterial()->SetTexParam(TEX_0, m_UseTex);

	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_ModuleData.iMaxParticleCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear();
	m_ModuleDataBuffer->Clear();
}

void CParticleSystem::SpawnModule(int _MaxParticle, int _SpawnRate, Vec3 _SpawnColor, Vec3 SpawnMinScale
	, Vec3 SpawnMaxScale, Vec3 _SpawnBoxScale, float _MinLifeTime, float _MaxLifeTime)
{
	for (UINT i = 0; i < (UINT)PARTICLE_MODULE::END; ++i)
	{
		m_ModuleData.ModuleCheck[i] = false;
	}
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	m_ModuleData.iMaxParticleCount = _MaxParticle;
	m_ModuleData.SpawnRate = _SpawnRate;
	m_ModuleData.vSpawnColor = _SpawnColor / 255;
	m_ModuleData.vSpawnScaleMin = SpawnMinScale;
	m_ModuleData.vSpawnScaleMax = SpawnMaxScale;
	m_ModuleData.SpawnShapeType = 0;
	m_ModuleData.vBoxShapeScale = _SpawnBoxScale;
	m_ModuleData.Space = 0; // �ùķ��̼� ��ǥ��
	m_ModuleData.MinLifeTime = _MinLifeTime;
	m_ModuleData.MaxLifeTime = _MaxLifeTime;
	
}

void CParticleSystem::ColorChangeModule(Vec3 _vStartColor, Vec3 _vEndColor)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	m_ModuleData.vStartColor = _vStartColor / 255.f;
	m_ModuleData.vEndColor = _vEndColor / 255;
}

void CParticleSystem::ScaleChangeModule(float _vStartScale, float _vEndScale)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	m_ModuleData.StartScale = _vStartScale;
	m_ModuleData.EndScale = _vEndScale;
}

void CParticleSystem::AddVelocityModule(float _fSpeed, int _iVelocityType, Vec3 _vVelocityDir, float _fAngle)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_ModuleData.AddVelocityType = _iVelocityType;
	m_ModuleData.Speed = _fSpeed;
	m_ModuleData.vVelocityDir = _vVelocityDir;
	m_ModuleData.OffsetAngle = _fAngle;
}

void CParticleSystem::DragModule(float _fStartDrag, float _fEndDrag)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	m_ModuleData.StartDrag = _fStartDrag;
	m_ModuleData.EndDrag = -_fEndDrag;
}

void CParticleSystem::RandomForceModule(float _fForceTerm, float _fForce)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RANDOM_FORCE] = true;
	m_ModuleData.fNoiseTerm = _fForceTerm;
	m_ModuleData.fNoiseForce = _fForce;
}

void CParticleSystem::VelocityAlignmentModule()
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityAlignment = true;
}

void CParticleSystem::VelocityScaleModule(float _fMaxSpeed, Vec3 _vMaxVelocityScale)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_ModuleData.VelocityScale = true;
	m_ModuleData.vMaxVelocityScale = _vMaxVelocityScale;
	m_ModuleData.MaxSpeed = _fMaxSpeed;
}

void CParticleSystem::AnimationModule(int _iFrmCount, int _iXCount, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vOffset)
{
	Vec2 Resolution = Vec2(m_UseTex->Width(), m_UseTex->Height());

	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::ANIMATION] = true;
	m_ModuleData.iFrmCount = _iFrmCount;
	m_ModuleData.iXCount = _iXCount;
	m_ModuleData.vLeftTop = _vLeftTop / Resolution;
	m_ModuleData.vSlice = _vSlice / Resolution;
	m_ModuleData.vOffset = _vOffset / Resolution;
}

void CParticleSystem::OnOff(bool _Onff)
{
	m_ModuleData.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = _Onff;
	m_AccTime = 0.f;
}

void CParticleSystem::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	fwrite(&m_tParticleType, sizeof(PARTICLE), 1, _File);
	fwrite(&m_ModuleData, sizeof(tParticleModule), 1, _File);
	SaveResRef(m_UpdateCS.Get(), _File);
	
}

void CParticleSystem::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	fread(&m_tParticleType, sizeof(PARTICLE), 1, _File);
	fread(&m_ModuleData, sizeof(tParticleModule), 1, _File);

	int i = 0;
	fread(&i, sizeof(i), 1, _File);

	// �����ϰ� �ִ� Res�̱� ������ ������ �ƴ� ã�Ƽ� ������Ŵ
	// ������Ʈ���̴��� �ڵ忡�� �����Ǳ� ������ ã�Ƽ� �߰�����.
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();
	}
	SetModuleData(m_tParticleType);
}

