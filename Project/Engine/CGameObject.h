#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CMeshRender;
class CCollider2D;
class CLight2D;
class CCamera;
class CRenderComponent;
class CTileMap;
class CScript;
class CAnimator2D;
class CParticleSystem;
class CRigidbody;

#define GET_COMPONENT(Type, TYPE) C##Type* Type() const {return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE];}

class CGameObject :
	public CEntity
{
private:
	CComponent*				m_arrCom[(UINT)COMPONENT_TYPE::END];
	CRenderComponent*		m_RenderCom;

	vector<CScript*>		m_vecScript;

	CGameObject*			m_Parent;
	vector<CGameObject*>	m_vecChild;

	int						m_iLayerIdx;
	bool					m_bDead;
	float					m_fLifeTime;
	float					m_fCurLifeTime;
	bool					m_bLifeSpan;

public:
	void begin();
	void tick();
	virtual void finaltick();
	void render();

public:
	void AddComponent(CComponent* _Component);
	void DeleteComponent(UINT _ComponentType);
	void AddChild(CGameObject* _Object);

	CComponent* GetComponent(COMPONENT_TYPE _ComType) { return m_arrCom[(UINT)_ComType]; }
	const vector<CGameObject*>& GetChild() {return m_vecChild;}

	CGameObject* GetParent() const { return m_Parent; }

	GET_COMPONENT(Transform, TRANSFORM);
	GET_COMPONENT(MeshRender, MESHRENDER);
	GET_COMPONENT(Camera, CAMERA);
	GET_COMPONENT(Collider2D, COLLIDER2D);
	GET_COMPONENT(Light2D, LIGHT2D);
	GET_COMPONENT(TileMap, TILEMAP);
	GET_COMPONENT(Animator2D, ANIMATOR2D);
	GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
	GET_COMPONENT(Rigidbody, RIGIDBODY);

	CRenderComponent* GetRenderComponent() const { return m_RenderCom; }


	int GetLayerIndex() { return m_iLayerIdx; }

	template<typename T>
	T* GetScript();

	const vector<CScript*>& GetScripts() { return m_vecScript; }

	void SetLifeSpan(float _fTime)
	{
		// 이미 제한시간 설정이 되어있다면 재설정하지 않음.
		if (m_bLifeSpan)
			return;

		m_fLifeTime = _fTime;
		m_bLifeSpan = true;
	}

	bool IsDead() { return m_bDead; }
	bool IsAncestor(CGameObject* _Target);

private:
	void DisconnectFromParent();
	void ChangeToChildType();
	void AddParentList();

	CLONE(CGameObject)
public:
	CGameObject();
	CGameObject(const CGameObject& _Other);
	~CGameObject();

	friend class CLayer;
	friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		T* pScript = dynamic_cast<T*> (m_vecScript[i]);
		if (nullptr != pScript)
			return pScript;
	}

	return nullptr;
}
