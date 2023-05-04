#pragma once
#include "CSingleton.h"

class CTileMap;

typedef struct _tagNode
{
	_tagNode*	pPrevNode;   // 이전 노드
	float		fFromParent; // 이전 노드에서 현재 노드까지의 거리
	float		fToDest;	 // 현재 노드에서 목적지 까지의 거리
	float		fFinal;		 // 위에 두 값을 합친 값(우선순위 기준)
	Vec2		vPos;

	int			iIdxX;
	int			iIdxY;
	bool		bMove;   // 이동 가능 불가능
	bool		bOpen;   // OpenList 에 들어갔는지
	bool		bClosed; // ClostList 에 들어있는지

	bool		bPath;
}tNode;

class Compare
{
public:
	bool operator() (tNode* _pLeft, tNode* _pRight)
	{
		return _pLeft->fFinal > _pRight->fFinal;
	}
};

class CAstarMgr :
    public CSingleton<CAstarMgr>
{
	SINGLE(CAstarMgr);
private:
	int		m_iXCount;
	int		m_iYCount;
	int		m_iCurPosX;
	int		m_iCurPosY;
	int		m_iDestPosX;
	int		m_iDestPosY;
	tNode	m_arrNode[200][200]; // 타일 개수 맞게
	Vec2	m_vTileSize;
	Vec2	m_vTileScale;
	priority_queue<tNode*, vector<tNode*>, Compare>	m_OpenList;

private:
	void Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue);
	Vec2 FindPath();
	void CalculateCost(tNode* _pCurNode, tNode* _pOrigin);
	void AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin);

public:
	Vec2 SetFind(Vec3 _CurPos, Vec3 _DestPos);
	void LoadTile(CTileMap* _Tile);
};

