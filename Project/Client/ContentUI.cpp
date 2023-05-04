#include "pch.h"
#include "ContentUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CPathMgr.h>
#include "TreeUI.h"
#include "InspectorUI.h"


ContentUI::ContentUI()
	: UI("##Content")
{
	SetName("Content");

	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	m_Tree->SetActive(true);
	m_Tree->ShowRoot(false);

	m_Tree->AddDynamic_Select(this, (UI_DELEGATE_1)&ContentUI::SetTargetToInspector);
	m_Tree->SetDragDropID("Resource");
	AddChildUI(m_Tree);
}

ContentUI::~ContentUI()
{
}

void ContentUI::init()
{
	ResetContent();

	Reload();
}

void ContentUI::tick()
{
	UI::tick();

	if (CResMgr::GetInst()->IsResourceChanged())
	{
		ResetContent();
	}
}

int ContentUI::render_update()
{
	return 0;
}

void ContentUI::Reload()
{
	CResMgr::GetInst()->Reload();

	ResetContent();
}

void ContentUI::ResetContent()
{
	// 전체 트리 초기화
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// 모든 타입의 리소스를 받아옴
	for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		// RES_TYPE을 string으로 Category 이름으로 써야하기 때문에 가져옴
		TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
		pCategory->SetCategoryNode(true);

		for (const auto& pair : mapRes)
		{
			// 리소스를 가져와 이름인 first는 제목으로 주소인 second는 노드 안의 주소값으로 넣어줌.
			m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
		}
	}
}

void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	// 선택된 노드를 TargetRes로 지정
	TreeNode* pSelectedNode = (TreeNode*)_SelectedNode;
	CRes* pSelectObject = (CRes*)pSelectedNode->GetData();

	if (nullptr == pSelectObject)
		return;

	InspectorUI* pInspector = (InspectorUI*)ImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetResource(pSelectObject);
}

void ContentUI::FindFileName(const wstring& _FolderPath)
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

RES_TYPE ContentUI::GetResTypeByExt(const wstring& _RelativePath)
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


