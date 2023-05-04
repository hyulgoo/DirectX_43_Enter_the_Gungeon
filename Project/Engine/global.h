#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <queue>
#include <stack>

using std::vector;
using std::list;
using std::map;
using std::make_pair;
using std::wstring;
using std::string;
using std::queue;
using std::priority_queue;
using std::stack;
using std::pair;

#include <typeinfo>
#include <assert.h>

// FileSystem
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
using namespace std::experimental;
using namespace std::experimental::filesystem;

#include <wrl.h>
using namespace Microsoft::WRL;


#include <d3d11.h> // 다이렉트 헤더
#include <d3dcompiler.h> // 쉐이더 컴파일러 헤더
#include <DirectXMath.h> // DX 수학관련 헤더
#include <DirectXPackedVector.h> // 다이렉트에서 자주 사용되는 벡터 관련

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
// math와 vector는 알아서 생김

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#include "define.h"
#include "struct.h"
#include "func.h"

#include "CSingleton.h"