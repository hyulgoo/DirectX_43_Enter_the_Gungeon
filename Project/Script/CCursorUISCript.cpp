#include "pch.h"
#include "CCursorUISCript.h"
#include <Engine/CRenderMgr.h>
#include <Engine/CCamera.h>

CCursorUISCript::CCursorUISCript()
	: CScript((UINT)CURSORUISCRIPT)
{
}

CCursorUISCript::~CCursorUISCript()
{
}

void CCursorUISCript::begin()
{
	MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"AIM"));
	MeshRender()->GetDynamicMaterial();
}

void CCursorUISCript::tick()
{
	Vec3 MousePos = CRenderMgr::GetInst()->GetRay();
	Vec3 MainCamPos = CRenderMgr::GetInst()->GetMainCam()->GetOwner()->Transform()->GetWorldPos();
	Transform()->SetRelativePos(MousePos.x - MainCamPos.x, MousePos.y - MainCamPos.y, 1.f);
}
