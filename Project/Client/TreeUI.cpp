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
	// ��� �̸�
	string strFinalName = m_strName;
	// ID�� ������ ���� ## �߰�
	strFinalName += "##";

	// ##�� �߰��� �� ID �߰�
	char szBuffer[100] = {};
	// �ٲٰ���� ����, ���� ����, �� ��������
	_itoa_s(m_ID, szBuffer, 10);
	strFinalName += szBuffer;

	// Flag - �⺻������ ��带 ������ �ִٸ� �����
	UINT flag = ImGuiTreeNodeFlags_DefaultOpen;

	// �ڽ� ��尡 ���ٸ� ȭ��ǥ ����(Leaf ǥ��)
	if (m_vecChildNode.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	// Ŭ�� �Ǿ��ų�, �׸��� ��Ÿ���� ����� ��� �׻� ���̶���Ʈ�� �ش�.
	if (m_Hilight || m_CategoryNode)
		flag |= ImGuiTreeNodeFlags_Selected;

	// �̸�##ID�� Name���� ����
	if (ImGui::TreeNodeEx(strFinalName.c_str(), flag))
	{
		// ���콺�� ���� �ִ� ���¿��� Ŭ������ ��
		if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left))
		{
			// Ŭ���� ��带 ���õ� ���� ����
			m_Owner->m_LbtDownNode = this;
		}
		else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			// Ŭ���� ��带 ���õ� ���� ����
			m_Owner->SetSelectedNode(this);
		}
		// Ʈ���� �����ϱ� ������ ���õ� ��忴�ٸ� �ٽ� Ʈ��UI�� ���ó��� �˷���.
		else if (0 != m_Owner->m_dwPrevSelected && m_Owner->m_dwPrevSelected == m_Data)
		{
			m_Owner->m_dwPrevSelected = 0;
			m_Owner->SetSelectedNode(this);
		}

		if (ImGui::BeginDragDropSource())
		{
			// �巡�� �Ǵ� �������� ID, ���� ������, ũ��
			ImGui::SetDragDropPayload(m_Owner->m_strDragDropID.c_str(), this, sizeof(TreeNode));
			ImGui::Text(m_strName.c_str());

			// Ʈ���� �巡�� �� ��带 �˸�
			m_Owner->SetDragNode(this);

			ImGui::EndDragDropSource();
		}


		// �巡�� ���� �� ����� ������ �ĺ��� ���
		if (ImGui::BeginDragDropTarget())
		{
			// ���콺�� ������ ��, ����� ��尡 �޾Ƶ��̴� ID�� ����� �������� ID�� ��ġ�� ���
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

	// Drag, Drop ��尡 �� �� �ִ� ��� (�巡�� ����� �ùٸ��� �Ͼ ���)
	// �Ǵ� Drag�� ���¿��� ����� ����� ���
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
	// �ʱ�ȣ ���� ���õ� ��尡 �ִٸ� ������ش�.
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

	// ��Ʈ��尡 ����ִٸ� ��Ʈ���� �����Ѵ�.
	if (nullptr == m_RootNode)
	{
		// ��Ʈ��尡 null�̸� �ƹ� ��嵵 �����Ƿ� �θ��带 ������ ���� ����. 
		assert(!_pParent);

		m_RootNode = pNewNode;
	}

	else
	{
		// ��Ʈ��尡 �ִٸ� �θ� �������� �� �θ��� �Ʒ��� ����
		if (_pParent)
		{
			if (_pParent->m_Owner != this)
				assert(nullptr);

			_pParent->m_vecChildNode.push_back(pNewNode);
			pNewNode->m_ParentNode = _pParent;
		}
		// ���� ������ �θ� ���ٸ� ��Ʈ����� �Ʒ��� ����
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
	// �巡�װ� ���۵� �����̹Ƿ� �����尡 ���� �� ����
	// �ִٸ� ���� �巡�� ��� �� ������ ���̹Ƿ� �ʱ�ȭ
	m_DropNode = nullptr;
}

void TreeUI::SetDropNode(TreeNode* _Node)
{
	m_DropNode = _Node;
}

bool TreeUI::GetSelectedNode(DWORD_PTR _Data)
{
	// queue ������� Ž���ϱ� ���� list�� �־���.
	static list<TreeNode*> queue;
	queue.clear();

	queue.push_back(m_RootNode);

	TreeNode* pCurNode = nullptr;
	while (!queue.empty())
	{
		// �ϳ��� �������� Data�� ��ġ�ϴ� ��尡 ���� ������ Ž���ϰ� 
		// ã�Ҵٸ� ���õ� ���� �ٲ��ְ� return��.
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