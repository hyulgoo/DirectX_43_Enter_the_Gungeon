#ifndef _STRUCT
#define _STRUCT


struct tLightColor
{
    float4 vDiffuse; // 빛의 색상
    float4 vAmbient; // 주변 광(환경 광)
};

// LightInfo
struct tLightInfo
{
    tLightColor Color; // 빛의 색상
    
    float4  vWorldPos; // 광원의 월드 스페이스 위치
    float4  vWorldDir; // 빛을 보내는 방향
    
    uint    LightType; // 빛의 타입(방향성, 점, 스포트)
    float   Radius; // 빛의 반경(사거리)
    float   Angle; // 빛의 각도    
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
	// 스폰 모듈
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

	// Color Change 모듈
    float4  vStartColor; // 초기 색상
    float4  vEndColor; // 최종 색상

	// Scale Change 모듈
    float  StartScale; // 초기 크기
    float  EndScale; // 최종 크기	
    
    int     iMaxParticleCount;
    int     ipad;
    
    // Add Velocity 모듈
    float4  vVelocityDir;
    int     AddVelocityType;
    float   OffsfetAngle;
    float   Speed;
    int     addvpad;
    
    // Drag 모듈
    float   StartDrag;
    float   EndDrag;
    
    // RandomForce 모듈
    float   fNoiseTerm;
    float   fNoiseForce;
    
    // Render 모듈
    int     VelocityAlignment;      // 0 : 사용 안함, 1 : 속도 정렬(이동방향으로 회전)
    int     VelocityScale;          // 0 : 사용 안함, 1 : 속도에 따른 크기 변화
    float   MaxSpeed;             // 최대 크기에 도달하는 속력
    float4  vMaxVelocityScale;   // 속력에 따른 크기 변화량 최대치
    int     renderpad;

    // Animation 모듈
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