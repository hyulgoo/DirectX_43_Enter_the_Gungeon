#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CMeshRender.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_fLifeTime(0.f)
	, m_fCurLifeTime(0.f)
	, m_bLifeSpan(false)
{
}

CGameObject::CGameObject(const CGameObject& _Other)
	: CEntity(_Other)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_fLifeTime(0.f)
	, m_fCurLifeTime(0.f)
	, m_bLifeSpan(false)
{
	// Component ����
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i])
		{
			AddComponent(_Other.m_arrCom[i]->Clone());
		}
	}

	// Script ����
	for (size_t i = 0; i < _Other.m_vecScript.size(); ++i)
	{
		AddComponent(_Other.m_vecScript[i]->Clone());
	}

	// Child Obj ����
	for (size_t i = 0; i < _Other.m_vecChild.size(); ++i)
	{
		AddChild(_Other.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Array(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::begin()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->begin();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->begin();
	}
}

void CGameObject::tick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->tick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->tick();
	}
}

void CGameObject::finaltick()
{
	if (m_bLifeSpan)
	{
		m_fCurLifeTime += DT;
		if (m_fLifeTime < m_fCurLifeTime)
		{
			DestroyObject(this);
		}
	}
	// COMPONENT_TYPE END �տ� �ݵ�� SCRIPT�� �ΰ� ��ũ��Ʈ �������� finaltick�� �����Ŵ
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}

	// ���̾� ��ȣ�� ���µ� finaltick�� ȣ��� ���
	assert(-1 != m_iLayerIdx);

	// ���̾ ������Ʈ ���
	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObject(this);
}

void CGameObject::render()
{
	// RenderComponent�� ���� ��츸 render ȣ��
	if (nullptr != m_RenderCom)
		m_RenderCom->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	_Component->m_pOwner = this;

	// ������Ʈ�� �ߺ����� �������� �� �ֵ��� �ߺ��˻縦 ���� ����.
	if (COMPONENT_TYPE::SCRIPT == _Component->GetType())
	{
		m_vecScript.push_back((CScript*)_Component);
	}

	else
	{
		// �̹� �����ϰ� �ִ� ������Ʈ�� ��� assert
		assert(!m_arrCom[(UINT)_Component->GetType()]);

		m_arrCom[(UINT)_Component->GetType()] = _Component;

		// Component�� render ����� ���� ������Ʈ���
		// �̹� render ����� ���� ������Ʈ�� �ִٸ� assert ���ٸ� RenderCom���� 
		if (COMPONENT_TYPE::MESHRENDER <= _Component->GetType()
			&& _Component->GetType() <= COMPONENT_TYPE::DECAL)
		{
			assert(!m_RenderCom);
			m_RenderCom = (CRenderComponent*)_Component;
		}
	}
	
}

void CGameObject::DeleteComponent(UINT _ComponentType)
{
	delete m_arrCom[_ComponentType];
	m_arrCom[_ComponentType] = nullptr;
}

void CGameObject::AddChild(CGameObject* _Object)
{
	if(_Object->m_Parent)
	{
		// �θ� �ִٸ� �θ���� ������ ���� �� �ڽ����� �־���.
		_Object->DisconnectFromParent();
	}
	else
	{
		// �θ� ���ٸ� �θ�Ÿ���̹Ƿ� �ڽ�Ÿ������ �ٲ��� �� �ڽ����� �־���
		_Object->ChangeToChildType();
	}
	_Object->m_Parent = this;
	m_vecChild.push_back(_Object);
}

bool CGameObject::IsAncestor(CGameObject* _Target)
{
	// �θ� ���� ������ ���� �ö󰡸� ���� Target�� �ִ��� Ȯ���Ѵ�.
	CGameObject* pParent = m_Parent;
	while(pParent)
	{
		if (pParent == _Target)
			return true;
		pParent = pParent->m_Parent;
	}

	return false;
}

void CGameObject::DisconnectFromParent()
{
	if (!m_Parent)
		return;

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (this == *iter)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}
	assert(nullptr);
}

void CGameObject::ChangeToChildType()
{
	// ���̾ �ʱ� �Ǵ� MAX LAYER �ȿ� ������� �ʴٸ� ����
	assert(-1 <= m_iLayerIdx && m_iLayerIdx < MAX_LAYER);

	// ���̾ �����ִ� ��� ���̾�� �����Ѵ�.
	if (-1 != m_iLayerIdx)
	{
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
		pLayer->RemoveFromParentList(this);
	}
}

void CGameObject::AddParentList()
{
	// �θ� ������Ʈ�� �ٲ���.
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pLayer->AddParentList(this);
}


