#include "pch.h"
#include "CMeshRender.h"
#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
}

void CMeshRender::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	Transform()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	GetMaterial()->UpdateData();

	GetMesh()->render();

	if (Animator2D())
		Animator2D()->Clear();
}
