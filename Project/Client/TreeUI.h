#pragma once
#include "UI.h"

// =========
// TreeNode
// =========
class TreeUI;
class TreeNode
{
private:
    TreeUI*             m_Owner;        // ��带 ������ Ʈ��
    TreeNode*           m_ParentNode;   // �θ���
    vector<TreeNode*>   m_vecChildNode; // ����� �ڽĳ��

    string              m_strName;      // ��� ��� �̸�
    UINT                m_ID;           // ����� ���� ID

    DWORD_PTR           m_Data;         // ��忡 ����� ������(�ּҰ�)

    bool                m_CategoryNode;
    bool                m_Hilight;
    
public:
    void SetCategoryNode(bool _category) { m_CategoryNode = _category; }

public:
    const string& GetName() { return m_strName; }
    DWORD_PTR GetData() { return m_Data; }

private:
    void render_update();

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};


// ======
// TreeUI
// ======
class TreeUI :
    public UI
{
private:
    TreeNode*       m_RootNode;
    UINT            g_NextId;
    bool            m_bShowRoot;

    TreeNode*       m_SelectedNode;
    TreeNode*       m_LbtDownNode;

    TreeNode*       m_DragNode;
    TreeNode*       m_DropNode;

    DWORD_PTR       m_dwPrevSelected;

    UI*             m_SelectInst;
    UI_DELEGATE_1   m_SelectFunc;

    UI*             m_DragDropInst;
    UI_DELEGATE_2   m_DragDropFunc;

    string          m_strDragDropID;

public:
    virtual int render_update() override;

public:
    void Clear();
    TreeNode* AddItem(const string& _strNodeName, DWORD_PTR _Data, TreeNode* _pParent = nullptr);
    void ShowRoot(bool _Show) { m_bShowRoot = _Show; }
    void AddDynamic_Select(UI* _UI, UI_DELEGATE_1 _MemFunc)
    {
        m_SelectInst = _UI;
        m_SelectFunc = _MemFunc;
    }
    void AddDynamic_DragDrop(UI* _UI, UI_DELEGATE_2 _MemFunc)
    {
        m_DragDropInst = _UI;
        m_DragDropFunc = _MemFunc;
    }
    void SetDragDropID(const string& _strID) { m_strDragDropID = _strID; }

private:
    void SetSelectedNode(TreeNode* _Node);
    void SetDragNode(TreeNode* _Node);
    void SetDropNode(TreeNode* _Node);

public:
    TreeNode* GetSelectedNode() { return m_SelectedNode; }
    bool GetSelectedNode(DWORD_PTR _Data);

public:
    TreeUI();
    ~TreeUI();

    friend class TreeNode;
};

