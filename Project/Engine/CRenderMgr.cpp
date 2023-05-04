#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CCamera.h"
#include "CLight2D.h"

#include "CResMgr.h"
#include "CKeyMgr.h"


CRenderMgr::CRenderMgr()
	: m_Light2DBuffer(nullptr)
	, m_pEditorCam(nullptr)
	, RENDER_FUNC(nullptr)
	, m_bUseEditCam(false)
	, m_pRay(nullptr)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
													, (UINT)vResolution.x, (UINT)vResolution.y
													, DXGI_FORMAT_R8G8B8A8_UNORM
													, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

	// ����Ʈ ���μ����� ����ϴ� Mtrl�� RTCopyTex ����
	CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
	CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);

	m_pRay = new CRay;
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;

	delete m_pRay;
}

void CRenderMgr::init()
{
	// Light2DBuffer ����ȭ ���� ����
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);
}

void CRenderMgr::render()
{
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();
	m_pRay->RayAtWorldSpace((int)MousePos.x, (int)MousePos.y);

	// ������ ����, ȭ�� Ŭ����
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	CDevice::GetInst()->ClearTarget(arrColor);
	
	// ��� Ÿ�� ����
	CDevice::GetInst()->OMSet();

	// ���� �� ���� ������ ������Ʈ �� ���ε�
	UpdateData();
	// ���� �Լ� ȣ��
	(this->*RENDER_FUNC)();

	// ���� ����
	Clear();
}

void CRenderMgr::render_play()
{
	// ī�޶� ���� ������
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
	//render_editor();
}

void CRenderMgr::render_editor()
{
	// ������ ī�޶� ������
	m_pEditorCam->SortObject();
	m_pEditorCam->render();
}

int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
	if (m_vecCam.size() <= _idx)
	{
		m_vecCam.resize(_idx + 1);
	}

	m_vecCam[_idx] = _Cam;
	return _idx;
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
	if (_IsPlay)
	{
		RENDER_FUNC = &CRenderMgr::render_play;
		m_bUseEditCam = false;
	}
	else
	{
		RENDER_FUNC = &CRenderMgr::render_editor;
		m_bUseEditCam = true;
	}
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::UpdateData()
{
	// GlobalData�� ���� ���� ����
	GlobalData.iLight2DCount = (UINT)m_vecLight2D.size();

	// ����ȭ������ ũ�Ⱑ ���ڶ�� ���� ������ �°� resize
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)m_vecLight2D.size(), SB_TYPE::READ_ONLY, true);
	}

	// ����ȭ ���۷� �����͸� �ű�
	m_Light2DBuffer->SetData(m_vecLight2D.data(), sizeof(tLightInfo) * (UINT)m_vecLight2D.size());
	m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);

	// ���� ��� ������ ���ε�
	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
	pGlobalBuffer->UpdateData();
	pGlobalBuffer->UpdateData_CS();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

#include "CTransform.h"
void CRay::RayAtViewSpace(int _ScreenX, int _ScreenY)
{
	D3D11_VIEWPORT Viewport = CDevice::GetInst()->GetViewport();

	CCamera* Cam = nullptr;
	if (CRenderMgr::GetInst()->IsEditCamUse())
		Cam = CRenderMgr::GetInst()->GetEditCam();
	else
		Cam = CRenderMgr::GetInst()->GetMainCam();
	assert(Cam);

	Matrix matProj = Cam->GetProjMat();

	m_vDirection.x = ((2.f * _ScreenX) / Viewport.Width - 1.f) / matProj._11;
	m_vDirection.y = ((-2.f * _ScreenY) / Viewport.Height + 1.f) / matProj._22;
	m_vDirection.z = 1.f;
}

void CRay::RayAtWorldSpace(int _ScreenX, int _ScreenY)
{
	RayAtViewSpace(_ScreenX, _ScreenY);

	CCamera* Cam = nullptr;
	if (CRenderMgr::GetInst()->IsEditCamUse())
		Cam = CRenderMgr::GetInst()->GetEditCam();
	else
		Cam = CRenderMgr::GetInst()->GetMainCam();

	Matrix matView, matInvView;
	matView = Cam->GetViewMat();

	matInvView = matView.Invert();

	m_vOriginal = XMVector3TransformCoord(m_vOriginal, matInvView);
	m_vDirection = XMVector3TransformNormal(m_vDirection, matInvView);

	Matrix CamWorldmat = Cam->Transform()->GetWorldMat();
	Matrix CamWorldinvmat = CamWorldmat.Invert();

	m_vOriginal = XMVector3TransformCoord(m_vOriginal, CamWorldmat);
	m_vDirection = XMVector3TransformCoord(m_vDirection, CamWorldmat);
}
