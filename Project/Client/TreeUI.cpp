#include "pch.h"
#include "TreeUI.h"

// ========
// TreeNode
// ========
TreeNode::TreeNode()
	: m_Owner(nullptr)
	, m_ParentNode(nullptr)
	, m_vecChildNode{}
	, m_strName{}
	, m_ID(0)
	, m_Data(0)
	, m_CategoryNode(nullptr)
	, m_Hilight(false)
{
}

TreeNode::~TreeNode()
{
	Safe_Del_Vec(m_vecChildNode);
}

void TreeNode::render_update()
{
	// 출력 이름
	string strFinalName = m_strName;
	// ID는 가리기 위해 ## 추가
	strFinalName += "##";

	// ##이 추가된 뒤 ID 추가
	char szBuffer[100] = {};
	// 바꾸고싶은 정보, 받을 버퍼, 몇 진수인지
	_itoa_s(m_ID, szBuffer, 10);
	strFinalName += szBuffer;

	// Flag - 기본적으로 노드를 가지고 있다면 열어둠
	UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

	// 자식 노드가 없다면 화살표 제거(Leaf 표시)
	if (m_vecChildNode.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	// 클릭 되었거나, 항목을 나타내는 노드의 경우 항상 하이라이트를 준다.
	if (m_Hilight || m_CategoryNode)
		flag |= ImGuiTreeNodeFlags_Selected;

	// 이름##ID를 Name으로 지정
	if (ImGui::TreeNodeEx(strFinalName.c_str(), flag))
	{
		// 마우스가 위에 있는 상태에서 클릭됐을 때
		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			// 클릭된 노드를 선택된 노드로 지정
			m_Owner->m_LbtDownNode = this;
		}
		else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			// 클릭된 노드를 선택된 노드로 지정
			m_Owner->SetSelectedNode(this);
		}
		// 트리를 리셋하기 이전에 선택된 노드였다면 다시 트리UI에 선택노드로 알려줌.
		else if (0 != m_Owner->m_dwPrevSelected && m_Owner->m_dwPrevSelected == m_Data)
		{
			m_Owner->m_dwPrevSelected = 0;
			m_Owner->SetSelectedNode(this);
		}

		if (ImGui::BeginDragDropSource())
		{
			// 드래그 되는 데이터의 ID, 담을 데이터, 크기
			ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
			ImGui::Text(m_strName.c_str());

			// 트리에 드래그 된 노드를 알림
			m_Owner->SetDragNode(this);

			ImGui::EndDragDropSource();
		}


		// 드래그 시작 후 드랍이 가능한 후보인 경우
		if (ImGui::BeginDragDropTarget())
		{
			// 마우스를 놓았을 때, 드랍된 노드가 받아들이는 ID와 드랍한 데이터의 ID가 일치한 경우
			const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload(m_Owner->m_strDragDropID.c_str());
			if (pPayLoad)
			{
				m_Owner->SetDropNode(this);
			}

			ImGui::EndDragDropTarget();
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->render_update();
		}

		ImGui::TreePop();
	}
}




// ================
//      TreeUI
// ================
TreeUI::TreeUI()
	:UI("##Tree")
	, m_RootNode(nullptr)
	, g_NextId(0)
	, m_bShowRoot(false)
	, m_SelectedNode(nullptr)
	, m_DragNode(nullptr)
	, m_DropNode(nullptr)
	, m_dwPrevSelected()
	, m_SelectInst(nullptr)
	, m_SelectFunc(nullptr)
	, m_DragDropInst(nullptr)
	, m_DragDropFunc(nullptr)
{
	m_strDragDropID = "PayLoad";
}

TreeUI::~TreeUI()
{
	if (nullptr != m_RootNode)
		delete m_RootNode;
}

int TreeUI::render_update()
{
	if (nullptr != m_RootNode)
	{
		if (m_bShowRoot)
		{
			m_RootNode->render_update();
		}
		else
		{
			for (size_t i = 0; i < m_RootNode->m_vecChildNode.size(); ++i)
			{
				m_RootNode->m_vecChildNode[i]->render_update();
			}
		}
	}

	// Drag, Drop 노드가 둘 다 있는 경우 (드래그 드랍이 올바르게 일어난 경우)
	// 또는 Drag한 상태에서 허공에 드랍한 경우
	if (m_DragNode && m_DropNode || (m_DragNode && ImGui::IsMouseReleased(ImGuiMouseButton_Left)))
	{
		if (m_DragDropInst && m_DragDropFunc)
		{
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DragNode, (DWORD_PTR)m_DropNode);
		}

		m_DragNode = nullptr;
		m_DropNode = nullptr;
	}

	return 0;
}

void TreeUI::Clear()
{
	// 초기호 전에 선택된 노드가 있다면 기억해준다.
	if (nullptr != m_SelectedNode)
		m_dwPrevSelected = m_SelectedNode->GetData();

	if (nullptr != m_RootNode)
	{
		delete m_RootNode;
		m_RootNode = nullptr;
	}
}

TreeNode* TreeUI::AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent)
{
	TreeNode* pNewNode = new TreeNode;

	pNewNode->m_Owner = this;
	pNewNode->m_strName = _strNodeName;
	pNewNode->m_Data = _Data;
	pNewNode->m_ID = g_NextId++;

	// 루트노드가 비어있다면 루트노드로 지정한다.
	if (nullptr == m_RootNode)
	{
		// 루트노드가 null이면 아무 노드도 없으므로 부모노드를 지정할 수가 없음. 
		assert(!_pParent);

		m_RootNode = pNewNode;
	}

	else
	{
		// 루트노드가 있다면 부모를 지정했을 때 부모의 아래로 넣음
		if (_pParent)
		{
			if (_pParent->m_Owner != this)
				assert(nullptr);

			_pParent->m_vecChildNode.push_back(pNewNode);
			pNewNode->m_ParentNode = _pParent;
		}
		// 따로 지정한 부모가 없다면 루트노드의 아래로 넣음
		else
		{
			m_RootNode->m_vecChildNode.push_back(pNewNode);
			pNewNode->m_ParentNode = m_RootNode;
		}
	}
	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _Node)
{
	if (m_LbtDownNode != _Node)
	{
		m_LbtDownNode = nullptr;
		return;
	}

	if (m_SelectedNode)
		m_SelectedNode->m_Hilight = false;

	m_SelectedNode = _Node;
	m_LbtDownNode = nullptr;

	if (m_SelectedNode)
	{
		m_SelectedNode->m_Hilight = true;

		if (m_SelectInst && m_SelectFunc)
		{
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_SelectedNode);
		}
	}
}

void TreeUI::SetDragNode(TreeNode* _Node)
{
	m_DragNode = _Node;
	// 드래그가 시작된 순간이므로 드랍노드가 있을 수 없음
	// 있다면 이전 드래그 드랍 때 지정된 것이므로 초기화
	m_DropNode = nullptr;
}

void TreeUI::SetDropNode(TreeNode* _Node)
{
	m_DropNode = _Node;
}

bool TreeUI::GetSelectedNode(DWORD_PTR _Data)
{
	// queue 방식으로 탐색하기 위해 list에 넣어줌.
	static list<TreeNode*> queue;
	queue.clear();

	queue.push_back(m_RootNode);

	TreeNode* pCurNode = nullptr;
	while (!queue.empty())
	{
		// 하나씩 꺼내가며 Data가 일치하는 노드가 나올 때까지 탐색하고 
		// 찾았다면 선택된 노드로 바꿔주고 return함.
		pCurNode = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pCurNode->m_vecChildNode.size(); ++i)
		{
			queue.push_back(pCurNode->m_vecChildNode[i]);
		}

		if (pCurNode->GetData() == _Data)
		{
			SetSelectedNode(pCurNode);
			return true;
		}
	}
	return false;
}