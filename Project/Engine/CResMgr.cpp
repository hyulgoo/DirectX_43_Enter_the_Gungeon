#include "pch.h"
#include "CResMgr.h"

CResMgr::CResMgr()
	: m_arrRes{}
	, m_Changed(false)
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	CreateDefaultComputeShader();
	CreateDefaultMaterial();

	Reload();
}

void CResMgr::tick()
{
	m_Changed = false;
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	Ptr<CMesh> pMesh = nullptr;


	// ==============
	// PointMesh 생성
	// ==============
	// 파티클이 사용할 Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	UINT idx = 0;
	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes(L"PointMesh", pMesh);

	// =============
	// RectMesh 생성
	// =============	
	// 0 --- 1 
	// |  \  |
	// 3 --- 2
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh", pMesh);

	vecIdx.clear();
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"RectMesh_Debug", pMesh);

	// Circle Mesh를 만들어야 하므로 초기화
	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Mesh
	// ===========

	// 반지름
	float fRadius = 0.5f;

	// 각도
	UINT Slice = 40;
	float fTheta = XM_2PI / (float)Slice;

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	// 정점 위치 지정
	for (UINT i = 0; i < Slice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}

	// 인덱스 설정
	for (UINT i = 0; i < Slice - 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 마지막 삼각형
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(Slice);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh", pMesh);

	vecIdx.clear();
	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes(L"CircleMesh_Debug", pMesh);

	vecVtx.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultGraphicsShader()
{
	Ptr<CGraphicsShader> pShader = nullptr;

	// ============================
	// Std2DShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddTexParam(TEX_0, "Output Texture");


	AddRes(pShader->GetKey(), pShader);

	// ======================================
	// Std2DLightShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// g_tex_1              : Nomal Texture
	// ======================================

	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DLightShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2DLight");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DLight");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	pShader->AddScalarParam(INT_1, "Test Int");
	pShader->AddScalarParam(FLOAT_0, "Test Float");
	pShader->AddScalarParam(VEC2_0, "Test Vec2");
	pShader->AddScalarParam(VEC4_0, "Test Vec4");

	pShader->AddTexParam(TEX_0, "Output Texture 1");
	pShader->AddTexParam(TEX_1, "Output Texture 2");
	pShader->AddTexParam(TEX_2, "Output Texture 3");;

	AddRes(pShader->GetKey(), pShader);

	// ===========================
	// UI
	// =============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"UIShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DAnim");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	AddRes(pShader->GetKey(), pShader);

	// =================
	// DebugShape Shader
	// Topology : LineStrip
	// RS_TYPE  : CULL_NONE
	// DS_TYPE  : NO_TEST_NO_WRITE
	// BS_TYPE  : Default
	// g_vec4_0 : OutColor
	// ==================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DebugShapeShader");
	pShader->CreateVertexShader(L"shader\\debugshape.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debugshape.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// TileMap Shader
	// 
	// RS_TYPE : CULL_NONE
	// DS_TYPE : LESS
	// BS_TYPE : MASK

	// Parameter
	// g_tex_0 : Tile Atlas Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TileMapShader");
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexParam(TEX_0, "Output Texture 1");

	AddRes(pShader->GetKey(), pShader);


	// ============================
	// ParticleRender
	// 
	// RS_TYPE : CULL_NONE	 항상 정면을 봐야함
	// DS_TYPE : NO_WRITE    가려져야 하지만 가리지는 않아야 함.
	// BS_TYPE : ALPHA_BLEND 

	// Parameter
	// g_int_0 : Particle Index
	// 
	// Domain : TRANSPARENT  반투명도 그려질 수 있어야 함.
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"ParticleRenderShader");
	pShader->CreateVertexShader(L"shader\\particle_render.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particle_render.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particle_render.fx", "PS_ParticleRender");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes(pShader->GetKey(), pShader);

	// ============================
	// GrayShader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"GrayShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayShader");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddRes(pShader->GetKey(), pShader);

	// ============================
	// Distortion Shader
	// RS_TYPE : CULL_NONE
	// DS_TYPE : NO_TEST_NO_WRITE
	// BS_TYPE : DEFAULT	 
	// Domain : POSTPROCESS
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"DistortionShader");
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_DistortionShader");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_ShockWaveShader");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	// Parameter
	pShader->AddTexParam(TEX_1, "Noise Texture");

	AddRes(pShader->GetKey(), pShader);

	pShader = new CGraphicsShader;
	pShader->SetKey(L"EffectShader");
	pShader->CreateVertexShader(L"shader\\Effect.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\Effect.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	// Param
	AddRes(pShader->GetKey(), pShader);
}

#include "CSetColorShader.h"
#include "CParticleUpdateShader.h"

void CResMgr::CreateDefaultComputeShader()
{
	Ptr<CComputeShader> pCS = nullptr;

	// Texture 색상 변경 쉐이더
	pCS = new CSetColorShader(32, 32, 1);
	pCS->SetKey(L"SetColorCS");
	pCS->CreateComputeShader(L"shader\\setcolor.fx", "CS_SetColor");
	AddRes(pCS->GetKey(), pCS);

	// Particle Update 쉐이더
	pCS = new CParticleUpdateShader(128, 1, 1);
	pCS->SetKey(L"ParticleUpdateCS");
	pCS->CreateComputeShader(L"shader\\particle_update.fx", "CS_ParticleUpdate");
	AddRes(pCS->GetKey(), pCS);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// std2d Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	// std2d Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DAnimMtrl", pMtrl);

	// Light Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DLightMtrl", pMtrl);

	// AnimLight Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"Std2DAnimLightMtrl", pMtrl);

	// Debug Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugShapeShader"));
	AddRes(L"DebugShapeMtrl", pMtrl);

	// Tile Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes(L"TileMapMtrl", pMtrl);

	// Particle Render Mtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes(L"ParticleRenderMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"BulletMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"CrossbowMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"ShadowMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DLightShader"));
	AddRes(L"EffectMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GrayShader"));
	AddRes(L"GrayMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DistortionShader"));
	AddRes(L"DistortionMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes(L"UIMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"UIShader"));
	AddRes(L"CursorMtrl", pMtrl);
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
	, DXGI_FORMAT _pixelformat, UINT _BingFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _pixelformat, _BingFlag, _Usage)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	assert(nullptr == pTex);

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);

	return pTex;
}

#include "CEventMgr.h"

void CResMgr::Reload()
{// Content 폴더에 있는 파일 이름들을 확인
	m_vecResPath.clear();
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	FindFileName(strContentPath);

	// 파일명으로 리소스 로딩
	for (size_t i = 0; i < m_vecResPath.size(); ++i)
	{
		RES_TYPE type = GetResTypeByExt(m_vecResPath[i]);

		wchar_t szName[50] = {};
		wstring FullPath = m_vecResPath[i];
		_wsplitpath_s(FullPath.c_str(), 0, 0, 0, 0, szName, 50, 0, 0);
		wstring strFileName = szName;

		if (type == RES_TYPE::END)
			continue;
		switch (type)
		{
		case RES_TYPE::MESHDATA:
			break;
		case RES_TYPE::MATERIAL:
			Load<CMaterial>(strFileName, m_vecResPath[i]);
			break;
		case RES_TYPE::PREFAB:
			Load<CPrefab>(strFileName, m_vecResPath[i]);
			break;
		case RES_TYPE::MESH:
			break;
		case RES_TYPE::TEXTURE:
			Load<CTexture>(strFileName, m_vecResPath[i]);
			break;
		case RES_TYPE::SOUND:
			Load<CSound>(strFileName, m_vecResPath[i]);
			break;
		}
	}

	for (UINT i = 0; i < UINT(RES_TYPE::END); ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		for (const auto& pair : mapRes)
		{
			if (pair.second->IsEngineRes())
				continue;

			// 모든 Res를 돌면서 Res의 경로에 원본 파일이 있는 지 확인하고 없다면 Delete
			wstring strFilePath = strContentPath + pair.second->GetRelativePath();
			if (!filesystem::exists(strFilePath))
			{
				tEvent evn = {};
				evn.Type = EVENT_TYPE::DELETE_RESOURCE;
				evn.wParam = (DWORD_PTR)i;
				evn.lParam = (DWORD_PTR)pair.second.Get();
				CEventMgr::GetInst()->AddEvent(evn);
			}
		}
	}
}


void CResMgr::FindFileName(const wstring& _FolderPath)
{
	WIN32_FIND_DATA FindData = {};

	// 모든 타입의 확장자를 찾음.
	wstring FolderPath = _FolderPath + L"*.*";

	// 첫 파일을 찾은 핸들
	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		// 찾은 파일의 타입을 알려주는 플래그를 통해 파일과 폴더를 구분
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// 상위 폴더로 이동하는 ..폴더의 경우 탐색 건너뛰기
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			// 폴더라면 탐색한 폴더이름을 경로에 포함시켜 다시 탐색(재귀호출)
			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;

		// Content까지의 경로를 전체 경로에서 지워 상대경로로 만들어준다
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		// 상대경로를 넣어준다.
		m_vecResPath.push_back(RelativePath);
	}
	FindClose(hFindHandle);
}

RES_TYPE CResMgr::GetResTypeByExt(const wstring& _RelativePath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_RelativePath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
	wstring strExt = szExt;

	if (L".mdat" == strExt)
		return RES_TYPE::MESHDATA;
	else if (L".pref" == strExt)
		return RES_TYPE::PREFAB;
	else if (L".mesh" == strExt)
		return RES_TYPE::MESH;
	else if (L".mtrl" == strExt)
		return RES_TYPE::MATERIAL;
	else if (L".png" == strExt || L".jpg" == strExt
		|| L".jpeg" == strExt || L".bmp" == strExt
		|| L".tga" == strExt || L".dds" == strExt)
		return RES_TYPE::TEXTURE;
	else if (L".mp3" == strExt || L".wav" == strExt || L".ogg" == strExt)
		return RES_TYPE::SOUND;
	else
		return RES_TYPE::END;
}

void CResMgr::DeleteRes(RES_TYPE _type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_type].find(_strKey);

	assert(!(iter == m_arrRes[(UINT)_type].end()));

	m_arrRes[(UINT)_type].erase(iter);

	m_Changed = true;
}
