#pragma once
#include "UI.h"

class TreeUI;
class TreeNode;

class OutlinerUI :
    public UI
{
private:
    TreeUI*     m_Tree;
    DWORD_PTR   m_dwSelectedData;

public:
    virtual void tick() override;
    virtual int render_update() override;

public:
    void ResetOutliner();
    void SetTargetToInspector(DWORD_PTR _SelectedNode);
    CGameObject* GetSelectedObject();

    void SetSelectedNodeData(DWORD_PTR _data)
    {
        m_dwSelectedData = _data;
    }

private:
    void AddGameObject(CGameObject* _obj, TreeNode* _ParentNode);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode);

public:
    OutlinerUI();
    ~OutlinerUI();
};

