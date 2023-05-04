#include "pch.h"
#include "CAnimator2D.h"

#include "CAnim2D.h"
#include "CMeshRender.h"

#include "CMaterial.h"


CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_mapAnim{}
	, m_pCurAnim(nullptr)
	, m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _other)
	: CComponent(_other)
	, m_bRepeat(_other.m_bRepeat)
{
	for (const auto& pair : _other.m_mapAnim)
	{
		CAnim2D* Animation = new CAnim2D(*pair.second);
		Animation->SetName(pair.first);
		m_mapAnim.insert(make_pair(Animation->GetName(), Animation));
		Animation->m_pOwner = this;
	}
	wstring CurAnimName = _other.m_pCurAnim->GetName();
	m_pCurAnim = m_mapAnim.find(CurAnimName)->second;
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (nullptr != m_pCurAnim)
	{
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->Reset();
		}

		m_pCurAnim->finaltick();
	}
}

void CAnimator2D::UpdateData()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	const tAnim2DFrm& frm = m_pCurAnim->GetCurFrame();
	Vec2 vBackSize = m_pCurAnim->GetBackSize();

	int iAnimUse = 1;
	pMtrl->SetScalarParam(INT_0, &iAnimUse);
	pMtrl->SetScalarParam(VEC2_0, &frm.LeftTopUV);
	pMtrl->SetScalarParam(VEC2_1, &frm.SliceUV);
	pMtrl->SetScalarParam(VEC2_2, &frm.Offset);
	pMtrl->SetScalarParam(VEC2_3, &vBackSize);

	pMtrl->SetTexParam(TEX_0, m_pCurAnim->GetAtlasTex());

}

void CAnimator2D::Clear()
{
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();

	int iAnimUse = 0;
	// INT_0 레지스터에 바인딩하고 있던 Anim 사용 유무를 0으로 함.
	pMtrl->SetScalarParam(INT_0, &iAnimUse);

	// 다른 Anim가 사용될 수 있으므로 현재 Anim에서 Tex 사용 이후 초기화 해줌.
	Ptr<CTexture> pTex = nullptr;
	pMtrl->SetTexParam(TEX_0, pTex);

}

void CAnimator2D::Play(const wstring& _strName, bool _bRepeat)
{
	CAnim2D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_pCurAnim = pAnim;
	m_bRepeat = _bRepeat;
}

CAnim2D* CAnimator2D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator2D::SetAnim(CAnim2D* _pAnim, bool _bRepeat)
{
	m_pCurAnim->Reset();

	m_pCurAnim = _pAnim;
	m_bRepeat = _bRepeat;
}

void CAnimator2D::DeleteAnim(const wstring& _strName)
{
	map<wstring, CAnim2D*>::iterator iter = m_mapAnim.find(_strName);

	if (iter == m_mapAnim.end())
	{
		return;
	}

	m_mapAnim.erase(iter);
}

bool CAnimator2D::IsFinish()
{
	return m_pCurAnim->IsFinish();
}

void CAnimator2D::CreateAnimation(const wstring& _strAnimName
	, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize
	, int _FrameCount, int _FPS)
{
	CAnim2D* pAnim = new CAnim2D;
	pAnim->Create(_strAnimName, _AtlasTex, _vLeftTop, _vSlice, _vBackSize, _FrameCount, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(_strAnimName, pAnim));
}

#include "CPathMgr.h"
#include "CResMgr.h"

void CAnimator2D::LoadAnimFromFile(const wstring& _strName)
{
	wstring contentPath = CPathMgr::GetInst()->GetContentPath();
	contentPath += L"animation\\";
	contentPath += _strName;
	contentPath += L"\\";
	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(contentPath + L"*.anim").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == handle)
		return;

	while (true)
	{
		FILE* pFile = nullptr;
		_wfopen_s(&pFile, wstring(contentPath + tData.cFileName).c_str(), L"rb");

		size_t iFrameCount = 0;
		wstring strAnimName;
		wstring strAtlasKey;
		Vec2 BackSize;

		while (true)
		{
			wchar_t szBuffer[256] = {};
			fwscanf_s(pFile, L"%s", szBuffer, 256);

			if (!wcscmp(szBuffer, L"[ANIMATION_NAME]"))
			{
				fwscanf_s(pFile, L"%s", szBuffer, 256);
				strAnimName = szBuffer;
				if (nullptr != FindAnim(strAnimName))
					break;
			}

			else if (!wcscmp(szBuffer, L"[ATLAS_KEY]"))
			{
				fwscanf_s(pFile, L"%s", szBuffer, 256);
				strAtlasKey = szBuffer;
			}

			else if (!wcscmp(szBuffer, L"[BACK_SIZE]"))
			{
				fwscanf_s(pFile, L"%f %f", &BackSize.x, &BackSize.y);
			}

			else if (!wcscmp(szBuffer, L"[FRAME_COUNT]"))
			{
				fwscanf_s(pFile, L"%zd", &iFrameCount);
				Ptr<CTexture> Atlas = CResMgr::GetInst()->FindRes<CTexture>(strAtlasKey);
				vector<tAnim2DFrm> AllFrm;
				CreateAnimation(strAnimName, Atlas, Vec2(0.f, 0.f), Vec2(0.f, 0.f), BackSize, (int)iFrameCount, 7);
				for (size_t i = 0; i < iFrameCount; ++i)
				{
					tAnim2DFrm frm = {};

					while (true)
					{
						wchar_t szBuffer[256] = {};
						fwscanf_s(pFile, L"%s", szBuffer, 256);

						if (!wcscmp(szBuffer, L"[LEFT_TOP]"))
						{
							fwscanf_s(pFile, L"%f %f", &frm.LeftTopUV.x, &frm.LeftTopUV.y);
						}

						else if (!wcscmp(szBuffer, L"[SIZE]"))
						{
							fwscanf_s(pFile, L"%f %f", &frm.SliceUV.x, &frm.SliceUV.y);
						}

						else if (!wcscmp(szBuffer, L"[OFFSET]"))
						{
							fwscanf_s(pFile, L"%f %f", &frm.Offset.x, &frm.Offset.y);
						}

						else if (!wcscmp(szBuffer, L"[DURATION]"))
						{
							fwscanf_s(pFile, L"%f", &frm.fDuration);
							break;
						}
					}
					FindAnim(strAnimName)->SetCurFrm(frm, (int)i);
				}
				break;
			}
		}
		FindNextFile(handle, &tData);
		if (L"ZEnd.anim" == (wstring)tData.cFileName)
			break;
	}
}

void CAnimator2D::SaveToLevelFile(FILE* _File)
{
	wstring strCurAnimName = m_pCurAnim->GetName();
	SaveWString(strCurAnimName, _File);
	SaveWString(GetOwner()->GetName(), _File);
}

void CAnimator2D::LoadFromLevelFile(FILE* _File)
{
	wstring strCurAnimName;
	LoadWString(strCurAnimName, _File);
	wstring OwnerName;
	LoadWString(OwnerName, _File);
	LoadAnimFromFile(OwnerName);

	m_pCurAnim = FindAnim(strCurAnimName);
}
