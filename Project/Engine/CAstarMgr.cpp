#include "pch.h"
#include "CAstarMgr.h"
#include "CTileMap.h"
#include "CTransform.h"

CAstarMgr::CAstarMgr()
: m_iXCount(0)
, m_iYCount(0)
, m_iCurPosX(0)
, m_iCurPosY(0)
, m_iDestPosX(0)
, m_iDestPosY(0)
, m_vTileSize{}
, m_vTileScale{}
, m_OpenList{}
{
}
CAstarMgr::~CAstarMgr()
{
}

// 매 틱마다 경로를 계산하므로 가야할 다음 타일의 위치만 반환함.
Vec2 CAstarMgr::SetFind(Vec3 _CurPos, Vec3 _DestPos)
{
	Vec2 CurTilePos = Vec2(_CurPos.x + m_vTileScale.x / 2, _CurPos.y + m_vTileScale.y / 2);
	Vec2 DestTilePos = Vec2(_DestPos.x + m_vTileScale.x / 2, _DestPos.y + m_vTileScale.y / 2);

	int CurPosX = (int)CurTilePos.x / m_vTileSize.x;
	int CurPosY = (int)CurTilePos.y / m_vTileSize.y;
	
	m_iCurPosX = CurPosX;
	m_iCurPosY = 99 - CurPosY;

	m_iDestPosX = (int)DestTilePos.x / m_vTileSize.x;
	m_iDestPosY = 100 - (int)DestTilePos.y / m_vTileSize.y;

	Vec2 Pos = FindPath();
	return Pos;
}

void CAstarMgr::Rebuild(priority_queue<tNode*, vector<tNode*>, Compare>& _queue)
{
	priority_queue<tNode*, vector<tNode*>, Compare> tempQueue;

	while (!_queue.empty())
	{
		tempQueue.push(_queue.top());
		_queue.pop();
	}
	_queue.swap(tempQueue);
}

void CAstarMgr::CalculateCost(tNode* _pCurNode, tNode* _pOrigin)
{
	_pCurNode->pPrevNode = _pOrigin;
	_pCurNode->fFromParent = (m_vTileSize.x + m_vTileSize.y) / 2 + _pOrigin->fFromParent;
	_pCurNode->fToDest = abs(m_iDestPosX - _pCurNode->iIdxX) * m_vTileSize.x + abs(m_iDestPosY - _pCurNode->iIdxY) * m_vTileSize.y;
	_pCurNode->fFinal = _pCurNode->fFromParent + _pCurNode->fToDest;
}

void CAstarMgr::AddOpenList(int _iXIdx, int _iYIdx, tNode* _pOrigin)
{
	// 현재 지점에서 갈 수 있는 곳을 OpenList에 넣는다.
	// 노드 범위를 벗어난 경우
	if (_iXIdx < 0 || _iXIdx >= m_iXCount || _iYIdx < 0 || _iYIdx >= m_iYCount
		|| !m_arrNode[_iYIdx][_iXIdx].bMove)
		return;


	// 해당 길이 닫힌 리스트에 있는 경우 넣지 않는다
	if (m_arrNode[_iYIdx][_iXIdx].bClosed)
		return;

	// Open List에 비용을 계산해서 넣는다.
	if (false == m_arrNode[_iYIdx][_iXIdx].bOpen)
	{
		CalculateCost(&m_arrNode[_iYIdx][_iXIdx], _pOrigin);
		// Open List 에 넣는다.
		m_arrNode[_iYIdx][_iXIdx].bOpen = true;
		m_OpenList.push(&m_arrNode[_iYIdx][_iXIdx]);
	}
	else // 이미 OpenList에 있는 경우
	{
		//비용을 계산해서 더 효율이 좋은 것으로 대체한다.
		tNode node = m_arrNode[_iYIdx][_iXIdx];
		CalculateCost(&node, _pOrigin);

		if (m_arrNode[_iYIdx][_iXIdx].fFinal > node.fFinal)
		{
			m_arrNode[_iYIdx][_iXIdx] = node;

			// 오픈리스트(우선순위큐) 재설정
			Rebuild(m_OpenList);
		}
	}
}
Vec2 CAstarMgr::FindPath()
{
	// 닫힌리스트 또는 이전에 OpenList에 있었다면 초기화
	for (int i = 0; i < 100; ++i)
	{
		for (int j = 0; j < 100; ++j)
		{
			if(true == m_arrNode[i][j].bClosed || true == m_arrNode[i][j].bOpen)
			{
				m_arrNode[i][j].bClosed = false;
				m_arrNode[i][j].bOpen = false;
				m_arrNode[i][j].bPath = false;
				m_arrNode[i][j].fFinal = 0.f;
				m_arrNode[i][j].fFromParent = 0.f;
				m_arrNode[i][j].fToDest = 0.f;
				m_arrNode[i][j].pPrevNode = nullptr;
			}
		}
	}
	// OpenList 초기화
	while (!m_OpenList.empty())
	{
		tNode* node = m_OpenList.top();
		m_OpenList.pop();
	}

	Vec2 ToDestPos;
	if (false == m_arrNode[m_iDestPosY][m_iDestPosX].bMove
		|| m_iCurPosX == m_iDestPosX && m_iCurPosY == m_iDestPosY)
	{
		return ToDestPos;
	}
		// 현재 위치 노드를 찾고 이미 그 위치에 있으므로 첫 노드를 닫힌리스트에 넣음.
	int iCurX = m_iCurPosX;
	int iCurY = m_iCurPosY;

	tNode* pCurNode = &m_arrNode[iCurY][iCurX];
	pCurNode->bClosed = true;

	while (true)
	{
		// 현재 지점에서 4방향, 대각선의 노드를 OpenList 에 넣는다.

		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY + 1, pCurNode);

		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY, pCurNode);

		AddOpenList(pCurNode->iIdxX
			, pCurNode->iIdxY - 1, pCurNode);

		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY, pCurNode);

		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY - 1, pCurNode);
		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY + 1, pCurNode);
		
		AddOpenList(pCurNode->iIdxX + 1
			, pCurNode->iIdxY + 1, pCurNode);
		
		AddOpenList(pCurNode->iIdxX - 1
			, pCurNode->iIdxY + 1, pCurNode);

		// 2. Open List 에서 가장 효율이 좋은 노드를 꺼낸다.
		//  - 해당 노드는 ClostList 에 넣는다.
		if (m_OpenList.empty())
			return ToDestPos;
		pCurNode = m_OpenList.top();
		pCurNode->bClosed = true; 

		m_OpenList.pop();

		// 목적지에 도착한 경우 Astar 종료
		if (pCurNode->iIdxX == m_iDestPosX && pCurNode->iIdxY == m_iDestPosY)
		{
			break;
		}
	}

	// 목적지까지 탐색이 끝났다면 왔던 노드를 되돌아가며 길 표시를 해줌.
	while (true)
	{
		m_arrNode[pCurNode->iIdxY][pCurNode->iIdxX].bPath = true;
		pCurNode = pCurNode->pPrevNode;

		if (pCurNode->iIdxX == m_iCurPosX && pCurNode->iIdxY == m_iCurPosY)
		{
			pCurNode->bPath = true;
			break;
		}
		// 현재 위치 바로 다음 타일 pos를 반환한다.
		if (pCurNode->pPrevNode->iIdxX == m_iCurPosX && pCurNode->pPrevNode->iIdxY == m_iCurPosY)
		{
			ToDestPos = pCurNode->vPos;
		}
	}
		
	 //Dest가 갈 수 없는 곳이면 0, 0을 반환하므로 예외처리 필요
	return ToDestPos;
}

void CAstarMgr::LoadTile(CTileMap* _Tile)
{
	m_vTileSize = _Tile->GetSliceSize() * Vec2(_Tile->GetAtlas()->Width(), _Tile->GetAtlas()->Height());
	m_vTileScale = _Tile->GetOwner()->Transform()->GetRelativeScale();
	vector<tTile> vecTile = _Tile->GetvecTile();
	Vec2 TileCount = _Tile->GetTileCount();
	m_iXCount = (int)TileCount.x;
	m_iYCount = (int)TileCount.y;
	for (int i = 0; i < (int)TileCount.y; ++i)
	{
		for (int j = 0; j < (int)TileCount.x; ++j)
		{
			m_arrNode[i][j].iIdxX = (int)j;
			m_arrNode[i][j].iIdxY = (int)i;

			m_arrNode[i][j].vPos = vecTile[j + i * TileCount.x].vPos;
			
			// 타일의 이동가능 상태 옮겨오기
			if ((UINT)TILESTATE::ABLE == vecTile[j + i * TileCount.x].State)
				m_arrNode[i][j].bMove = true;
			else
				m_arrNode[i][j].bMove = false;
			m_arrNode[i][j].bClosed = false;
			m_arrNode[i][j].bOpen = false;
			m_arrNode[i][j].bPath = false;
			m_arrNode[i][j].fFinal = 0.f;
			m_arrNode[i][j].fFromParent = 0.f;
			m_arrNode[i][j].fToDest = 0.f;
			m_arrNode[i][j].pPrevNode = nullptr;
		}
	}
}
