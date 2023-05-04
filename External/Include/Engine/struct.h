#pragma once

struct tVertex
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};

typedef tVertex Vtx;

struct tEvent
{
	EVENT_TYPE Type;
	DWORD_PTR  wParam;
	DWORD_PTR  lParam;
};

struct tDebugShapeInfo
{
	SHAPE_TYPE eShape;
	Matrix	   matWorld;
	Vec3	   vWorldPos;
	Vec3	   vWorldScale;
	Vec3	   vWorldRotation;
	Vec4	   vColor;
	float	   fMaxTime;
	float	   fCurTime;
};


struct tLightColor
{
	Vec4 vDiffuse; // 빛의 색상
	Vec4 vAmbient; // 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // 빛의 색상

	Vec4		vWorldPos;   // 광원의 월드 스페이스 위치
	Vec4		vWorldDir;	 // 빛을 보내는 방향

	UINT		LightType;   // 빛의 타입(방향성, 점, 스포트)
	float		Radius;		 // 빛의 반경(사거리)
	float		Angle;		 // 빛의 각도
	int			padding;
};


// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vPos;
	UINT State = 0;
	int  padding;
};

// Animator2D, 1 프레임에 담겨야 하는 정보
struct tAnim2DFrm
{
	Vec2	LeftTopUV;
	Vec2	SliceUV;
	Vec2	Offset;
	float	fDuration;
};

struct tParticle
{
	Vec4	vLocalPos;
	Vec4	vWorldPos;		// 파티클 위치
	Vec4	vWorldScale;	// 파티클 크기
	Vec4	vColor;		// 파티클 색상
	Vec4	vVelocity;		// 파티클 현재 속도도
	Vec4	vForce;		// 파티클에 주어진 힘
	Vec4	vRandomForce;

	float	Age;			// 생존 시간
	float	PrevAge;
	float	NomalizeAge;	// 수명대비 생존시간을 0~1로 정규화 한 값
	float	LifeTime;		// 수명
	float	Mass;			// 질량
	float	ScaleFactor;

	int		Active;
	int		pad;
};

struct tRWParticleBuffer
{
	int SpawnCount;		// 스폰 시킬 파티클 개수
	int padding[3];
};

struct tParticleModule
{
	// 스폰 모듈
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;
	float	fSphereShapeRadius;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int		SpawnRate;			// 초당 생성 개수
	int		Space;				// 파티클 업데이트 좌표계 ( 0 : World,  1 : Local)
	float   MinLifeTime;		// 최소 수명
	float   MaxLifeTime;		// 최대 수명
	int     spawnpad[3];

	// Color Change 모듈
	Vec4	vStartColor;		// 초기 색상
	Vec4	vEndColor;			// 최종 색상

	// Scale Change 모듈
	float	StartScale;			// 초기 배율
	float	EndScale;			// 최종 배율	

	// 버퍼 최대크기
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity 모듈
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag 모듈 - 속도 제한
	float	StartDrag;
	float	EndDrag;

	// RandomForce 모듈
	float	fNoiseTerm;
	float	fNoiseForce;

	// Render 모듈
	int		VelocityAlignment;
	int		VelocityScale;
	float	MaxSpeed;
	Vec4	vMaxVelocityScale;
	int		renderpad;

	// Animation 모듈
	Vec2	vLeftTop;
	Vec2	vSlice;
	Vec2	vOffset;
	int		iFrmCount;
	int		iXCount;


	// Module Check
	int		ModuleCheck[(UINT)PARTICLE_MODULE::END];
};

struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];

	int arrTex[(UINT)TEX_PARAM::TEX_END];
};

struct tGlobal
{
	Vec2   Resolution;
	float  tDT;
	float  tAccTime;
	UINT   iLight2DCount;
	UINT   iLight3DCount;
	int    globalpadding[2];
};

extern tGlobal GlobalData;

struct tStatus
{
	int		HP;
	int		ATK;
	int		Speed;
	float	Attack_FirstDelay;
	float	Attack_LaterDelay;
	float	Attack_Range;
	float   Detect_Range;
};

struct WaveInfo
{
	int		PrefabNum;
	Vec2    SpawnPos;
};