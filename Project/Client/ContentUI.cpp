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
	// ��ü Ʈ�� �ʱ�ȭ
	m_Tree->Clear();
	m_Tree->AddItem("Root", 0);

	// ��� Ÿ���� ���ҽ��� �޾ƿ�
	for (size_t i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResources((RES_TYPE)i);

		// RES_TYPE�� string���� Category �̸����� ����ϱ� ������ ������
		TreeNode* pCategory = m_Tree->AddItem(ToString((RES_TYPE)i), 0);
		pCategory->SetCategoryNode(true);

		for (const auto& pair : mapRes)
		{
			// ���ҽ��� ������ �̸��� first�� �������� �ּ��� second�� ��� ���� �ּҰ����� �־���.
			m_Tree->AddItem(string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get(), pCategory);
		}
	}
}

void ContentUI::SetTargetToInspector(DWORD_PTR _SelectedNode)
{
	// ���õ� ��带 TargetRes�� ����
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

	// ��� Ÿ���� Ȯ���ڸ� ã��.
	wstring FolderPath = _FolderPath + L"*.*";

	// ù ������ ã�� �ڵ�
	HANDLE hFindHandle = FindFirstFile(FolderPath.c_str(), &FindData);

	while (FindNextFile(hFindHandle, &FindData))
	{
		// ã�� ������ Ÿ���� �˷��ִ� �÷��׸� ���� ���ϰ� ������ ����
		if (FindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// ���� ������ �̵��ϴ� ..������ ��� Ž�� �ǳʶٱ�
			if (!wcscmp(FindData.cFileName, L".."))
			{
				continue;
			}

			// ������� Ž���� �����̸��� ��ο� ���Խ��� �ٽ� Ž��(���ȣ��)
			FindFileName(_FolderPath + FindData.cFileName + L"\\");
			continue;
		}

		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
		wstring RelativePath = _FolderPath + FindData.cFileName;

		// Content������ ��θ� ��ü ��ο��� ���� ����η� ������ش�
		RelativePath = RelativePath.substr(strContentPath.length(), RelativePath.length() - strContentPath.length());

		// ����θ� �־��ش�.
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


