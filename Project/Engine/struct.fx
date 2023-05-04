#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
    float4 vDiffuse; // ���� ����
    float4 vAmbient; // �ֺ� ��(ȯ�� ��)
};

// LightInfo
struct tLightInfo
{
    tLightColor Color; // ���� ����
    
    float4  vWorldPos; // ������ ���� �����̽� ��ġ
    float4  vWorldDir; // ���� ������ ����
    
    uint    LightType; // ���� Ÿ��(���⼺, ��, ����Ʈ)
    float   Radius; // ���� �ݰ�(��Ÿ�)
    float   Angle; // ���� ����    
    int     Padding;
};

struct tParticle
{
    float4  vLocalPos;
    float4  vWorldPos;
    float4  vWorldScale;
    float4  vColor;
    float4  vVelocity;
    float4  vForce;
    float4  vRandomForce;
    
    float   Age;
    float   PrevAge;
    float   NomalizedAge;
    float   LifeTime;
    float   Mass;
    float   ScaleFactor;
    
    int     Active;
    int     pad;
};

struct tParticleModule
{
	// ���� ���
    float4  vSpawnColor;
    float4  vSpawnScaleMin;
    float4  vSpawnScaleMax;
    float3  vBoxShapeScale;
    float   fSphereShapeRadius;
    int     SpawnShapeType; // Sphere , Box
    int     SpawnRate;
    int     Space;          // 0 : World, 1 : Local
    float   MinLifeTime;
    float   MaxLifeTime;
    int3    spawnpad;

	// Color Change ���
    float4  vStartColor; // �ʱ� ����
    float4  vEndColor; // ���� ����

	// Scale Change ���
    float  StartScale; // �ʱ� ũ��
    float  EndScale; // ���� ũ��	
    
    int     iMaxParticleCount;
    int     ipad;
    
    // Add Velocity ���
    float4  vVelocityDir;
    int     AddVelocityType;
    float   OffsfetAngle;
    float   Speed;
    int     addvpad;
    
    // Drag ���
    float   StartDrag;
    float   EndDrag;
    
    // RandomForce ���
    float   fNoiseTerm;
    float   fNoiseForce;
    
    // Render ���
    int     VelocityAlignment;      // 0 : ��� ����, 1 : �ӵ� ����(�̵��������� ȸ��)
    int     VelocityScale;          // 0 : ��� ����, 1 : �ӵ��� ���� ũ�� ��ȭ
    float   MaxSpeed;             // �ִ� ũ�⿡ �����ϴ� �ӷ�
    float4  vMaxVelocityScale;   // �ӷ¿� ���� ũ�� ��ȭ�� �ִ�ġ
    int     renderpad;

    // Animation ���
    float2  LeftTop;
    float2  Slice;
    float2  Offset;
    int     FrmCount;
    int     XCount;
    
    
	// Module Check
    int     Spawn;
    int     ColorChange;
    int     ScaleChange;
    int     AddVelocity;
    
    int     Drag;
    int     RandomForce;
    int     Render;
    int     Animation;
    
};

#endif