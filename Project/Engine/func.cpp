#include "pch.h"

#include "CEventMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "ptr.h"
#include "CResMgr.h"

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _iLayerIdx)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = _iLayerIdx;

	CEventMgr::GetInst()->AddEvent(evn);
}

void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _strName)
{
	_NewObject->Transform()->SetRelativePos(_vWorldPos);

	tEvent evn = {};

	evn.Type = EVENT_TYPE::CREATE_OBJECT;
	evn.wParam = (DWORD_PTR)_NewObject;
	evn.lParam = CLevelMgr::GetInst()->GetCurLevel()->FindLayerByName(_strName)->GetLayerIdx();

	CEventMgr::GetInst()->AddEvent(evn);
}

void DestroyObject(CGameObject* _DeleteObject)
{
	if (_DeleteObject->IsDead())
		return;

	tEvent evn = {};

	evn.Type = EVENT_TYPE::DELETE_OBJECT;
	evn.wParam = (DWORD_PTR)_DeleteObject;

	CEventMgr::GetInst()->AddEvent(evn);
}

void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_vWorldScale.x, _vWorldScale.y, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::RECT;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = XMMatrixIdentity();
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius, _fRadius, 1.f);
	info.vWorldRotation = _vRotation;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime)
{
	tDebugShapeInfo info = {};

	info.matWorld = _matWorld;
	info.eShape = SHAPE_TYPE::CIRCLE;
	info.fMaxTime = _fTime;
	info.vColor = _vColor;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

const char* ToString(RES_TYPE type)
{
	return RES_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(RES_TYPE type)
{
	return RES_TYPE_WSTR[(UINT)type];
}

const char* ToString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_STR[(UINT)type];
}

const wchar_t* ToWString(COMPONENT_TYPE type)
{
	return COMPONENT_TYPE_WSTR[(UINT)type];
}

void SaveWString(const wstring& _str, FILE* _File)
{
	UINT iLen = (UINT)_str.length();
	// 문자열 길이 ( 읽을 때 길이를 알아야 정확히 읽을 수 있기 때문)
	fwrite(&iLen, sizeof(UINT), 1, _File);
	// 문자열
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _File)
{
	wchar_t szBuffer[256] = {};

	UINT iLen = 0;
	fread(&iLen, sizeof(UINT), 1, _File);
	fread(szBuffer, sizeof(wchar_t), iLen, _File);

	_str = szBuffer;
}

float GetMouseDir(Vec3 _TargetPos)
{
	// Player가 마우스 바라보게 하기
	Vec3 TargetPos = _TargetPos;
	Vec2 vDefault = Vec2(0.f, -1.f);
	Vec3 Cursor = CRenderMgr::GetInst()->GetRay();
	Vec2 vDir = Vec2(Cursor.x - TargetPos.x, Cursor.y - TargetPos.y);
	vDir = Vec2(vDir.x, vDir.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(vDefault));

	if (vDir.x < 0.f)
		angle = (360 / 180 * XM_PI) - angle;

	return angle;
}

float GetDirfloat(Vec3 _CurPos, Vec3 _TargetPos, bool _degree)
{
	// 아래축을 기준으로 CurPos에서 TargetPos를 바라보는 angle 반환
	Vec3 CurPos = _CurPos;
	Vec2 vDefault = Vec2(0.f, -1.f);
	Vec3 TargetPos = _TargetPos;
	Vec2 vDir = Vec2(TargetPos.x - CurPos.x, TargetPos.y - CurPos.y);
	vDir.Normalize();
	float angle = (float)acos(vDir.Dot(vDefault));

	if (vDir.x < 0.f)
		angle = (360.f / 180.f * XM_PI) - angle;

	if(_degree)
		angle *= (180.f / XM_PI);
	return angle;
}

Vec2 GetDirVec2(Vec3 _CurPos, Vec3 _TargetPos)
{
	Vec2 vDir = Vec2(_TargetPos.x - _CurPos.x, _TargetPos.y - _CurPos.y);
	vDir.Normalize();
	return vDir;
}

void SaveResRef(Ptr<CRes> _Res, FILE* _File)
{
	int i = 0;
	if (nullptr == _Res)
	{
		fwrite(&i, sizeof(i), 1, _File);
	}
	else
	{
		i = 1;
		fwrite(&i, sizeof(i), 1, _File);
		SaveWString(_Res->GetKey(), _File);
		SaveWString(_Res->GetRelativePath(), _File);
	}
}

