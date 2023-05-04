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
	Vec4 vDiffuse; // ���� ����
	Vec4 vAmbient; // �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct tLightInfo
{
	tLightColor Color;		 // ���� ����

	Vec4		vWorldPos;   // ������ ���� �����̽� ��ġ
	Vec4		vWorldDir;	 // ���� ������ ����

	UINT		LightType;   // ���� Ÿ��(���⼺, ��, ����Ʈ)
	float		Radius;		 // ���� �ݰ�(��Ÿ�)
	float		Angle;		 // ���� ����
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

// Animator2D, 1 �����ӿ� ��ܾ� �ϴ� ����
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
	Vec4	vWorldPos;		// ��ƼŬ ��ġ
	Vec4	vWorldScale;	// ��ƼŬ ũ��
	Vec4	vColor;		// ��ƼŬ ����
	Vec4	vVelocity;		// ��ƼŬ ���� �ӵ���
	Vec4	vForce;		// ��ƼŬ�� �־��� ��
	Vec4	vRandomForce;

	float	Age;			// ���� �ð�
	float	PrevAge;
	float	NomalizeAge;	// ������ �����ð��� 0~1�� ����ȭ �� ��
	float	LifeTime;		// ����
	float	Mass;			// ����
	float	ScaleFactor;

	int		Active;
	int		pad;
};

struct tRWParticleBuffer
{
	int SpawnCount;		// ���� ��ų ��ƼŬ ����
	int padding[3];
};

struct tParticleModule
{
	// ���� ���
	Vec4    vSpawnColor;
	Vec4	vSpawnScaleMin;
	Vec4	vSpawnScaleMax;
	Vec3	vBoxShapeScale;
	float	fSphereShapeRadius;
	int		SpawnShapeType;		// 0 : BOX, 1 : Sphere
	int		SpawnRate;			// �ʴ� ���� ����
	int		Space;				// ��ƼŬ ������Ʈ ��ǥ�� ( 0 : World,  1 : Local)
	float   MinLifeTime;		// �ּ� ����
	float   MaxLifeTime;		// �ִ� ����
	int     spawnpad[3];

	// Color Change ���
	Vec4	vStartColor;		// �ʱ� ����
	Vec4	vEndColor;			// ���� ����

	// Scale Change ���
	float	StartScale;			// �ʱ� ����
	float	EndScale;			// ���� ����	

	// ���� �ִ�ũ��
	int		iMaxParticleCount;
	int		ipad;

	// Add Velocity ���
	Vec4	vVelocityDir;
	int     AddVelocityType;	// 0 : From Center, 1: To Center, 2 : Fixed Direction	
	float	OffsetAngle;
	float	Speed;
	int     addvpad;

	// Drag ��� - �ӵ� ����
	float	StartDrag;
	float	EndDrag;

	// RandomForce ���
	float	fNoiseTerm;
	float	fNoiseForce;

	// Render ���
	int		VelocityAlignment;
	int		VelocityScale;
	float	MaxSpeed;
	Vec4	vMaxVelocityScale;
	int		renderpad;

	// Animation ���
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