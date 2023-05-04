#include "pch.h"
#include "CTileMap.h"
#include "CResMgr.h"
#include "CTransform.h"
#include "CEngine.h"
#include "CAstarMgr.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(100)
	, m_iTileCountY(100)
	, m_Atlas(nullptr)
	, m_vSliceSize{}
	, m_vecTile{}
	, m_Buffer(nullptr)
	, m_bShowFrame(false)
	, m_Framethick(1)
	, m_bSet(false)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TileMapMtrl"));
	m_Buffer = new CStructuredBuffer;
	m_Buffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, SB_TYPE::READ_ONLY, true);
	m_vSliceSize = Vec2(0.1f, 0.1f);
	m_vecTile.resize(m_iTileCountX * m_iTileCountY);
}

CTileMap::~CTileMap()
{
	if (nullptr != m_Buffer)
		delete m_Buffer;
}
void CTileMap::begin()
{
	m_Atlas = CResMgr::GetInst()->FindRes<CTexture>(L"TILE");
	GetMaterial()->SetTexParam(TEX_0, m_Atlas); 
	if(!m_bSet)
	{
		SetTilePos();
		m_bSet = true;
	}
	wstring Name = GetOwner()->GetName();
	if(L"Tile" == Name)
	{
		CAstarMgr::GetInst()->LoadTile(this);
		m_bShowFrame = true;
	}
}
void CTileMap::finaltick()
{}

void CTileMap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;
	// 위치 업데이트
	Transform()->UpdateData();

	// 가로 세로 개수 전달 및 재질 업데이트
	GetMaterial()->SetScalarParam(INT_0, &m_iTileCountX);
	GetMaterial()->SetScalarParam(INT_1, &m_iTileCountY);
	GetMaterial()->UpdateData();

	// 구조화 버퍼 업데이트(사이즈, 개수)
	UpdateData();

	// 렌더
	GetMesh()->render();
}

void CTileMap::UpdateData()
{
	// 구조화 버퍼를 전용레지스터(t20)에 바인딩
	m_Buffer->SetData(m_vecTile.data(), sizeof(tTile) * (UINT)m_vecTile.size());
	m_Buffer->UpdateData(20, PIPELINE_STAGE::PS_PIXEL);
}

void CTileMap::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	m_iTileCountX = _iXCount;
	m_iTileCountY = _iYCount;

	//m_vecTile.clear();
	//m_vecTile.resize(m_iTileCountX * m_iTileCountY);

	// 버퍼의 크기가 타일 개수보다 작다면 resize
	if (m_Buffer->GetElementCount() != m_vecTile.size())
	{
		m_Buffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), SB_TYPE::READ_ONLY, true);
	}
	// 타일 세팅 테스트용 코드
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{			
			m_vecTile[(int)i * m_iTileCountX + (int)j].vLeftTop.x = m_vSliceSize.x * (int)j;
			m_vecTile[(int)i * m_iTileCountX + (int)j].vLeftTop.y = m_vSliceSize.y * (int)i;
			m_vecTile[(int)i * m_iTileCountX + (int)j].vSlice = m_vSliceSize;
			
		}
	}
}

void CTileMap::SetDrawFrame()
{
	if (m_bShowFrame)
	{
		int a = 1;
		GetMaterial()->SetScalarParam(INT_2, &a);
		GetMaterial()->SetScalarParam(FLOAT_0, &m_Framethick);
	}
	else
	{
		int a = 0;
		GetMaterial()->SetScalarParam(INT_2, &a);
	}
}

void CTileMap::SetTilePos()
{
	//m_vecTile.clear();
	//m_vecTile.resize(m_iTileCountX * m_iTileCountY);
	Vec3 TileCenterPos = Transform()->GetRelativePos();
	Vec3 TileScale = Transform()->GetRelativeScale();
	Vec3 TileLeftTopPos = Vec3(TileCenterPos.x - TileScale.x / 2, TileCenterPos.y + TileScale.y / 2, TileCenterPos.z);
	Vec2 ScalePerTile = Vec2(TileScale.x / m_iTileCountX, TileScale.y / m_iTileCountY);
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{

			m_vecTile[i * m_iTileCountX + j].vPos = Vec3(TileLeftTopPos.x + ScalePerTile.x * j + ScalePerTile.x / 2.f,
				TileLeftTopPos.y - ScalePerTile.y * i - ScalePerTile.y / 2.f,
				TileCenterPos.z);
		}
	}
}

void CTileMap::ClearTile()
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		m_vecTile[i].vSlice = Vec2(0.f, 0.f);
		m_vecTile[i].vLeftTop = Vec2(0.f, 0.f);
	}
}

void CTileMap::SaveToLevelFile(FILE* _File)
{
	CRenderComponent::SaveToLevelFile(_File);
	
	wstring AtlasName = m_Atlas->GetKey();
	SaveWString(AtlasName, _File);
	
	fwrite(&m_iTileCountX, sizeof(UINT), 1, _File);
	fwrite(&m_iTileCountY, sizeof(UINT), 1, _File);
	fwrite(&m_vSliceSize, sizeof(Vec2), 1, _File);
	fwrite(m_vecTile.data(), sizeof(tTile), m_vecTile.size(), _File);
}

void CTileMap::LoadFromLevelFile(FILE* _File)
{
	CRenderComponent::LoadFromLevelFile(_File);
	
	wstring AtlasName;
	LoadWString(AtlasName, _File);
	m_Atlas = CResMgr::GetInst()->FindRes<CTexture>(AtlasName);
	GetMaterial()->SetTexParam(TEX_0, m_Atlas);
	fread(&m_iTileCountX, sizeof(UINT), 1, _File);
	fread(&m_iTileCountY, sizeof(UINT), 1, _File);
	fread(&m_vSliceSize, sizeof(Vec2), 1, _File);
	SetTileCount(m_iTileCountX, m_iTileCountY);
	
	fread(m_vecTile.data(), sizeof(tTile), m_iTileCountX * m_iTileCountY, _File);
}

int CTileMap::Save(const wstring& _strRelativePath)
{
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strRelativePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"wb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Tile Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return E_FAIL;
	}

	// TileMap 이름 저장
	fwprintf_s(pFile, L"\n");

	fwprintf_s(pFile, L"[TILEMAP_NAME]\n");
	fwprintf_s(pFile, GetName().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 아틀라스 텍스쳐 키값 저장   
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, m_Atlas->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_PATH]\n");
	fwprintf_s(pFile, m_Atlas->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_SIZE]\n");
	fwprintf_s(pFile, L"%.1f %.1f\n", m_Atlas->Width(), m_Atlas->Height());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[TileCountX]\n");
	fwprintf_s(pFile, L"%d\n", (int)m_iTileCountX);
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[TileCountY]\n");
	fwprintf_s(pFile, L"%d\n", (int)m_iTileCountY);
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[Slice_Size]\n");
	fwprintf_s(pFile, L"%f %f\n", m_vSliceSize.x, m_vSliceSize.y);
	fwprintf_s(pFile, L"\n\n");


	// 프레임 정보 저장
	wchar_t szNum[50] = {};

	fwprintf_s(pFile, L"[FRAME_COUNT]\n");

	size_t iTileCount = m_vecTile.size();
	_itow_s((int)iTileCount, szNum, 50, 10);

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	for (size_t i = 0; i < iTileCount; ++i)
	{
		fwprintf_s(pFile, L"[%zd_FRAME]\n", i);

		// LEFT_TOP
		fwprintf_s(pFile, L"[LEFT_TOP]\n");
		fwprintf_s(pFile, L"%.1f %.1f\n", m_vecTile[i].vLeftTop.x * m_Atlas->Width(), m_vecTile[i].vLeftTop.y * m_Atlas->Height());

		// SIZE
		fwprintf_s(pFile, L"[SIZE]\n");
		fwprintf_s(pFile, L"%.1f %.1f\n", m_vecTile[i].vSlice.x * m_Atlas->Width(), m_vecTile[i].vSlice.y * m_Atlas->Height());

		// TileProperty
		fwprintf_s(pFile, L"[Property]\n");
		fwprintf_s(pFile, L"%d\n", m_vecTile[i].State);
		fwprintf_s(pFile, L"\n");
	}

	fclose(pFile);


	return S_OK;
}

int CTileMap::Load(const wstring& _strFilePath)
{

	//m_vecTile.clear();
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strFilePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"TileMap Load 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 부르기 실패", MB_OK);
		return E_FAIL;
	}

	size_t iTileCount = 0;
	Vec2 AtlasSize = Vec2(0.f, 0.f);
	wstring strAtlasKey;
	wstring strAltasRelativePath;

	while (true)
	{
		wchar_t szBuffer[256] = {};
		fwscanf_s(pFile, L"%s", szBuffer, 256);

		if (!wcscmp(szBuffer, L"[TILEMAP_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			SetName(szBuffer);
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAtlasKey = szBuffer;
		}

		else if (!wcscmp(szBuffer, L"[ATLAS_PATH]"))
		{
			fwscanf_s(pFile, L"%s", szBuffer, 256);
			strAltasRelativePath = szBuffer;
		}
		else if (!wcscmp(szBuffer, L"[ATLAS_SIZE]"))
		{
			fwscanf_s(pFile, L"%f %f", &AtlasSize.x, &AtlasSize.y);
		}
		else if (!wcscmp(szBuffer, L"[TileCountX]"))
		{
			fwscanf_s(pFile, L"%d", &m_iTileCountX);
		}
		else if (!wcscmp(szBuffer, L"[TileCountY]"))
		{
			fwscanf_s(pFile, L"%d", &m_iTileCountY);
		}
		else if (!wcscmp(szBuffer, L"[Slice_Size]"))
		{
			fwscanf_s(pFile, L"%f %f", &m_vSliceSize.x, &m_vSliceSize.y);
			SetTileCount(m_iTileCountX, m_iTileCountY);
			//m_vecTile.clear();
		}
		else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
		{
			fwscanf_s(pFile, L"%zd", &iTileCount);

			for (size_t i = 0; i < iTileCount; ++i)
			{
				tTile tile = m_vecTile[i];

				while (true)
				{
					wchar_t szBuffer[256] = {};
					fwscanf_s(pFile, L"%s", szBuffer, 256);

					if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
					{
						fwscanf_s(pFile, L"%f %f", &tile.vLeftTop.x, &tile.vLeftTop.y);
						tile.vLeftTop /= AtlasSize;
					}

					else if (!wcscmp(szBuffer, L"[SIZE]"))
					{
						fwscanf_s(pFile, L"%f %f", &tile.vSlice.x, &tile.vSlice.y);
						tile.vSlice /= AtlasSize;
					}

					else if (!wcscmp(szBuffer, L"[Property]"))
					{
						UINT i = 0;
						fwscanf_s(pFile, L"%d", &i);
						tile.State = i;
						break;
					}
				}

				m_vecTile[i] = tile;
			}

			break;
		}
	}

	m_Atlas = CResMgr::GetInst()->Load<CTexture>(strAtlasKey, strAltasRelativePath);
	GetMaterial()->SetTexParam(TEX_0, m_Atlas);

	fclose(pFile);

	return S_OK;
}