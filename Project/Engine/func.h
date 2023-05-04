#pragma once

class CGameObject;
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, int _iLayerIdx);
void SpawnGameObject(CGameObject* _NewObject, Vec3 _vWorldPos, const wstring& _strName);

void DestroyObject(CGameObject* _DeleteObject);


// DrawDebugShape
void DrawDebugRect(Vec3 _vWorldPos, Vec2 _vWorldScale, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);


void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec4 _vColor, Vec3 _vRotation, float _fTime = 0.f);
void DrawDebugCircle(const Matrix& _matWorld, Vec4 _vColor, float _fTime = 0.f);

// void DrawDebugSphere(); 
// void DrawDebugCube();

const char* ToString(RES_TYPE type);
const wchar_t* ToWString(RES_TYPE type);

const char* ToString(COMPONENT_TYPE type);
const wchar_t* ToWString(COMPONENT_TYPE type);

void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _File);

float GetMouseDir(Vec3 _TargetPos);
float GetDirfloat(Vec3 _CurPos, Vec3 _TargetPos, bool _degree);
Vec2  GetDirVec2(Vec3 _CurPos, Vec3 _TargetPos);


class CRes;
template<typename T>
class Ptr;

void SaveResRef(Ptr<CRes> _Res, FILE* _File);

class CResMgr;
template<typename T>

void LoadResRef(Ptr<T>& _Res, FILE* _File)
{
	int i = 0;
	// Res 참조 유무를 받아옴
	fread(&i, sizeof(int), 1, _File);

	// 참조하고 있다면 ResKey와 경로를 받아옴
	if (i)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		// 받아온 Key, 경로를 통해 Res Load
		_Res = CResMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}




template<typename T, UINT Size>
void Safe_Del_Array(T* (&arr)[Size])
{
	for (UINT i = 0; i < Size; ++i)
	{
		if (nullptr != arr[i])
		{
			delete arr[i];
		}
	}
}

template<typename T>
void Safe_Del_Vec(vector<T>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr != _vec[i])
		{
			delete _vec[i];
		}
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{
		if (nullptr != pair.second)
		{
			delete pair.second;
		}
	}
	_map.clear();
}
