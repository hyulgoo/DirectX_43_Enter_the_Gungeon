#pragma once

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() {return nullptr; assert(nullptr);}

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32

// 모든 싱글톤 클래스가 생성자 소멸자를 호출할 수 있어야하므로 싱글톤 클래스를 friend 해야함.
#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>

enum class COMPONENT_TYPE
{
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation 
	ANIMATOR3D,		// Bone Sknning Animation
	RIGIDBODY,
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM,	// 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	END,
	// custom
	SCRIPT,			// 엔진에서 제공해줄 수 없는 기능 
	//(게임이면 공통적으로 가져야하는 기능이 아닌 해당 게임만 필요한 기능)
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];

enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
	END,
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};


enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,	// 뒤에 있는 것은 가림
	CULL_FRONT,	// 앞에 있는 것을 가림
	CULL_NONE,	// 아무것도 가리지 않음
	WIRE_FRAME,	// 정점을 잇는 선만 그림
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// 깊이 테스트는 하지만 깊이값을 남기지는 않음
	NO_TEST_NO_WRITE,	// 테스트도 하지않고 깊이값도 남기지 않음.
	END,
};

enum class BS_TYPE
{
	DEFAULT,
	MASK,
	ALPHA_BLEND,
	ONE_ONE,
	END,
};

enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명 오브젝트
	DOMAIN_MASK,		// 불투명, 투명으로 나눠진 오브젝트
	DOMAIN_TRANSPARENT, // 반투명 오브젝트
	DOMAIN_POSTPROCESS,	// 후처리
	DOMAIN_UI,
	DOMAIN_UNDEFINED,	// 미정의
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	ADD_CHILD,
	DELETE_RESOURCE,
	LEVEL_CHANGE,
	CHANGE_STATE,
};

enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	END,
};

enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};

enum class COLLISION_DIR
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL, // 위치에 상관없이 모든 화면을 비추면서 방향성을 가진 빛
	POINT,		 // 점으로 특정 위치만 비추는 빛
	SPOT,		 // 광원이 원근투영 같이 투영각을 가져 일부 각도에만 빛을 비치는 것.
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	RANDOM_FORCE,
	RENDER,
	ANIMATION,

	END,
};

enum class PARTICLE
{
	PLAYER_WALK,
	PLAYER_JUMP,
	SHADOW,
	SHOT_FIRE,
	SHOT_EFFECT,
	BOMB,
	BULLET_BLANK,
	BULLET_DIE,
	BULLET_HIT,
	ENEMY_APPEAR,
	FIRE,
	TORCH_SIDE,
	TORCH_FRONT,
	LAMP,
	BOSS_SHADOW,
	BOSS_JUMP,
	END,
};

enum class DIR
{
	UP,
	DOWN,
	LEFT_UP,
	LEFT_DOWN,
	LEFT,
	RIGHT_UP,
	RIGHT_DOWN,
	RIGHT,
	END,
};

enum class TILESTATE
{
	NONE,
	ABLE,
	DISABLE,
	END,
};

enum class WEAPON_TYPE
{
	OLDGUN,
	CROSSBOW,
	MAGNUM,
};

enum class LAYER
{
	DEFAULT,
	TILE,
	PLAYER,
	MONSTER,
	PLAYERPROJECTILE,
	MONSTERPROJECTILE,
	WALL,
	ITEM,
	FALLAREA,
	SPAWNBOX,
	BOMB,
	BLANK,

	UI = 31,
};

enum class MONSTER_TYPE
{
	REDBOOK,
	GREENBOOK,
	BULLETKIN,
	SHOTGUNKIN_BLUE,
	SHOTGUNKIN_RED,
	CUBULON,
	GIGI,
	SHOTBAT,
	BULLETBAT,
	MAGE,
	GATLING_GULL
};
enum class WEAPON_STATES
{
	IDLE,
	SHOT,
	RELOAD,
	END,
};