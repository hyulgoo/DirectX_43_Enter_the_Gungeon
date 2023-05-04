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
	// Component 복사
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (_Other.m_arrCom[i])
		{
			AddComponent(_Other.m_arrCom[i]->Clone());
		}
	}

	// Script 복사
	for (size_t i = 0; i < _Other.m_vecScript.size(); ++i)
	{
		AddComponent(_Other.m_vecScript[i]->Clone());
	}

	// Child Obj 복사
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
	// COMPONENT_TYPE END 앞에 반드시 SCRIPT를 두고 스크립트 전까지만 finaltick을 실행시킴
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::SCRIPT; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finaltick();
	}

	// 레이어 번호가 없는데 finaltick이 호출된 경우
	assert(-1 != m_iLayerIdx);

	// 레이어에 오브젝트 등록
	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObject(this);
}

void CGameObject::render()
{
	// RenderComponent를 가진 경우만 render 호출
	if (nullptr != m_RenderCom)
		m_RenderCom->render();
}

void CGameObject::AddComponent(CComponent* _Component)
{
	_Component->m_pOwner = this;

	// 컴포넌트는 중복으로 갖고있을 수 있도록 중복검사를 하지 않음.
	if (COMPONENT_TYPE::SCRIPT == _Component->GetType())
	{
		m_vecScript.push_back((CScript*)_Component);
	}

	else
	{
		// 이미 보유하고 있는 컴포넌트인 경우 assert
		assert(!m_arrCom[(UINT)_Component->GetType()]);

		m_arrCom[(UINT)_Component->GetType()] = _Component;

		// Component가 render 기능을 가진 컴포넌트라면
		// 이미 render 기능을 가진 컴포넌트가 있다면 assert 없다면 RenderCom으로 
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
		// 부모가 있다면 부모와의 연결을 끊은 후 자식으로 넣어줌.
		_Object->DisconnectFromParent();
	}
	else
	{
		// 부모가 없다면 부모타입이므로 자식타입으로 바꿔준 후 자식으로 넣어줌
		_Object->ChangeToChildType();
	}
	_Object->m_Parent = this;
	m_vecChild.push_back(_Object);
}

bool CGameObject::IsAncestor(CGameObject* _Target)
{
	// 부모가 없을 때까지 위로 올라가며 위에 Target이 있는지 확인한다.
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
	// 레이어가 초기 또는 MAX LAYER 안에 들어있지 않다면 종료
	assert(-1 <= m_iLayerIdx && m_iLayerIdx < MAX_LAYER);

	// 레이어에 속해있는 경우 레이어에서 삭제한다.
	if (-1 != m_iLayerIdx)
	{
		CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
		pLayer->RemoveFromParentList(this);
	}
}

void CGameObject::AddParentList()
{
	// 부모 오브젝트로 바꿔줌.
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(m_iLayerIdx);
	pLayer->AddParentList(this);
}


