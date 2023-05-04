#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Script\CCameraMoveScript.h>
#include <Script/CGameCameraScript.h>

CEditorObjMgr::CEditorObjMgr()
	: m_DebugShapeInfo{}
	, m_DebugShape()
	, m_mapClickLayer{}
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugShape);
}

void CEditorObjMgr::init()
{
	m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"));

	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"));

	// Editor Cam ����
	CGameObjectEx* pEditorCamObj = new CGameObjectEx;
	pEditorCamObj->AddComponent(new CTransform);
	pEditorCamObj->AddComponent(new CCamera);
	pEditorCamObj->AddComponent(new CCameraMoveScript);

	pEditorCamObj->Camera()->SetLayerMaskAll(true);
	pEditorCamObj->Camera()->SetLayerMask(31, false);

	pEditorCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	m_vecEditorObj.push_back(pEditorCamObj);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->Camera());
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_mapClickLayer.insert(make_pair(i, false));
	}
	m_mapClickLayer.find(1)->second = true;
}

void CEditorObjMgr::progress()
{
	// DebugShape ���� ��������
	//vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugShapeInfo();
	//m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());
	//vecInfo.clear();


	tick();

	render();
}

void CEditorObjMgr::tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	// ����Ű�� ChangeState ȣ��
	if (KEY_TAP(KEY::P))
	{
		if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
		{
			CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::STOP);
		}
		else
		{
			CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);
		}
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}
	
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}

	//if (KEY_TAP(KEY::LBTN))
	//{
	//	CGameObject* click = nullptr;
	//	click = GetClickObj();
	//	if (click != nullptr)
	//		int a = 0;
	//}
}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}

	// DebugShape Render
	CGameObjectEx* pShapeObj = nullptr;

	vector<tDebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
	for (; iter != m_DebugShapeInfo.end();)
	{
		switch (iter->eShape)
		{
		case SHAPE_TYPE::RECT:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
			break;
		case SHAPE_TYPE::CIRCLE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
			break;
		case SHAPE_TYPE::CUBE:
			break;
		case SHAPE_TYPE::SPHERE:
			break;
		}

		if (iter->matWorld != XMMatrixIdentity())
		{
			pShapeObj->Transform()->SetWorldMat(iter->matWorld);
		}
		else
		{
			pShapeObj->Transform()->SetRelativePos(iter->vWorldPos);
			pShapeObj->Transform()->SetRelativeScale(iter->vWorldScale);
			pShapeObj->Transform()->SetRelativeRot(iter->vWorldRotation);
			pShapeObj->finaltick();
		}

		pShapeObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, &iter->vColor);
		pShapeObj->render();

		iter->fCurTime += DT;
		if (iter->fMaxTime < iter->fCurTime)
		{
			iter = m_DebugShapeInfo.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
