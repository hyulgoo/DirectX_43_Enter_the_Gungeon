#ifndef _PARTICLE_UPDATE
#define _PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

RWStructuredBuffer<tParticle>       ParticleBuffer : register(u0);
RWStructuredBuffer<int4>            ParticleSpawnCount : register(u1);
StructuredBuffer<tParticleModule>   ParticleModuleData : register(t20);
Texture2D                           NoiseTexture : register(t21);

#define ObjectPos           g_vec4_0

#define NoiseTexResolution  g_vec2_0

#define SpawnCount          ParticleSpawnCount[0].x
#define ModuleData          ParticleModuleData[0]
#define ParticleMaxCount    ParticleModuleData[0].iMaxParticleCount

#define SpawnModule         ParticleModuleData[0].Spawn
#define ColorChangeModule   ParticleModuleData[0].ColorChange
#define ScaleChangeModule   ParticleModuleData[0].ScaleChange

[numthreads(128, 1, 1)]
void CS_ParticleUpdate(int3 _ID : SV_DispatchThreadID)
{
    // 파티클의 숫자보다 많은 스레드가 연산하는 것 방지
    if (ParticleMaxCount <= _ID.x)
        return;
    
    tParticle particle = ParticleBuffer[_ID.x];
    

    if (SpawnModule)
    { 
        // 파티클이 비활성화인 경우
        if (particle.Active == 0.f)
        {
            while (0 <= SpawnCount)
            {
            // 원래 SpawnCount를 변수로 기억함
                int orgvalue = SpawnCount;
                int outvalue = 0;
            // 아래 함수는 병렬처리하는 스레드를 접근하는 순서대로 처리하게 만들어줌.
            // SpawnCount를 SpawnCount - 1로 교체하고 outvalue는 교체하기 전인 SpawnCount를 기억함.
            // 여기서 모든 스레드가 SpawnCount가 이전에 처리되어 더 낮은 숫자로 바뀌더라도 접근 순서에 따라 
            // 더 높은 숫자로 교체할 수 있기 때문에 먼저 변수로 기억한 orgvalue와 같을 때만 SpawnCount - 1로 교체함
            // 위 함수로 병렬처리하는 스레드를 순차적으로 계산하게 하고 순서에 따라 발생할 수 있는 오류를 예방함.
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
        
                // 스폰에 성공한 스레드
                if (orgvalue == outvalue)
                {
                    particle.Active = 1;
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                    
                    float fNomalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.2f, vOut3);
                    
                    // shape 형태에 따라 spawn 되는 영역의 형태 설정
                    // Box shape
                    if(ModuleData.SpawnShapeType == 0)
                    {
                        // 2차원에서 자연스럽게 보여야 하므로 z축은 0으로 고정하여 평면에서만 스폰되게 함.
                        particle.vLocalPos.xyz = float3(ModuleData.vBoxShapeScale.x * vOut1.r - ModuleData.vBoxShapeScale.x * 0.5f
                                                      , ModuleData.vBoxShapeScale.y * vOut2.r - ModuleData.vBoxShapeScale.y * 0.5f
                                                      , 0.f);
                        particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
                        
                        float4 vSpawnScale = ModuleData.vSpawnScaleMin + (ModuleData.vSpawnScaleMax - ModuleData.vSpawnScaleMin) * vOut3.x;
                        particle.vWorldScale.xyz = vSpawnScale.xyz;
                    }                  
                    
                    // sphere shape
                    else if (ModuleData.SpawnShapeType == 1)
                    {
                        float fRadius = ModuleData.fSphereShapeRadius;
                        float fAngle = vOut2.r * 2 * 3.1415926535f;
                    }
                    
                    // 파티클 질량 설정
                    particle.Mass = 1.f;
                    
                    // AddVelocity 모듈
                    if(ModuleData.AddVelocity)
                    {
                        // From Center
                        if(ModuleData.AddVelocityType == 0)
                        {
                            // obj -> 생성 위치까지의 벡터를 노말라이즈 후 스피드를 곱함
                            float3 vVelocity = normalize(particle.vLocalPos.xyz);
                            particle.vVelocity.xyz = vVelocity * ModuleData.Speed;
                        }
                        // To Center
                        else if (ModuleData.AddVelocityType == 1)
                        {                            
                            float3 vVelocity = normalize(particle.vLocalPos.xyz);
                            particle.vVelocity.xyz = -(vVelocity * ModuleData.Speed);
                        }
                        // Fixed Direction
                        else if(ModuleData.AddVelocityType == 2)
                        {
                            float3 vVelocity = normalize(ModuleData.vVelocityDir.xyz);
                            particle.vVelocity.xyz = vVelocity * ModuleData.Speed;
                        }
                    }
                    
                    particle.vColor = ModuleData.vSpawnColor;
                    
                    particle.Age = 0.f;
                    particle.LifeTime = ModuleData.MinLifeTime + (ModuleData.MaxLifeTime - ModuleData.MinLifeTime) * vOut2.r;
                    break;
                }
            }
        }
    }
    // 파티클이 이미 활성화인 경우
    if(particle.Active)
    {        
        // 활성화일 동안 Age 누적
        particle.PrevAge = particle.Age;
        particle.Age += g_DT;
        particle.NomalizedAge = saturate(particle.Age / particle.LifeTime);
        particle.vForce.xyz = (float3) 0.f;
    
        // Age가 LifeTime을 넘어가는 순간 비활성화
        if (particle.LifeTime <= particle.Age)
        {
            particle.Active = 0.f;
        }
        
        // RandomForce 모듈이 켜져있다면
        if(ModuleData.RandomForce)
        {
            // 첫 프레임이라면 (이전 프레임이 없는)
            if (particle.PrevAge == 0.f)
            {
                float3 vOut1 = (float3) 0.f;
                float3 vOut2 = (float3) 0.f;
                float3 vOut3 = (float3) 0.f;
                
                // 난수 3개 생성
                float fNomalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID, vOut1);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.1f, vOut2);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.2f, vOut3);
                // 난수를 0 ~ 1로 정규화 후 vForce를 곱해 xyz에 대한 RandomForce 적용
                float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut3.z));
                particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
            }
            // 첫 프레임이 아니라면 지정한 간격을 넘어간 순간에 새로운 RandomForce를 주도록 함.
            else
            {
                // 주기를 넘은 프레임에만 RandomForce를 적용할 수 있도록
                // 이전프레임은 주기를 넘지 못했지만 이번 프레임에 넘었을 때만 적용되게 함.
                int Age = int(particle.Age * (1.f / ModuleData.fNoiseTerm));
                int PrevAge = int(particle.PrevAge * (1.f / ModuleData.fNoiseTerm));
                
                if(Age != PrevAge)
                {
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                
                // 난수 3개 생성
                    float fNomalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.2f, vOut3);
                // xyz에 대한 RandomForce 적용, 범위를 -1 ~ 1로 (반대로 갈 수도 있어야 하므로)
                    float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut3.z) * 2.f - 1.f);
                    particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
                }
            }

            particle.vForce.xyz += particle.vRandomForce.xyz;
        }
        
        // 파티클에 힘이 적용된 경우 질량을 적용하여 가속도 계산
        float3 vAccel = particle.vForce.xyz / particle.Mass;
        particle.vVelocity.xyz += vAccel * g_DT;
        
        // 속도 제한(Drag) 모듈
        // Drag 모듈은 속도 관련 모든 모듈이 끝난 후 적용
        if(ModuleData.Drag)
        {
            // 파티클의 현재 속력
            float Speed = length(particle.vVelocity);
            float Drag = ModuleData.StartDrag + (ModuleData.EndDrag - ModuleData.StartDrag) * particle.NomalizedAge;
            
            // 현재 속력이 제한속도보다 크다면 제한속도로 낮춤
            if (Drag <= 0.f)
                Drag = 0.001f;
            if(Drag < Speed)
            {
                particle.vVelocity = normalize(particle.vVelocity) * Drag;
            }
        }
        
        // 속도에 따라 파티클 이동
        if(ModuleData.Space == 0)
        {
            particle.vWorldPos += particle.vVelocity * g_DT;
        }
        else if(ModuleData.Space == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DT;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        // 크기 변화 모듈
        if(ModuleData.ScaleChange)
            particle.ScaleFactor = ModuleData.StartScale + (ModuleData.EndScale - ModuleData.StartScale) * particle.NomalizedAge ;
        else
            particle.ScaleFactor = 1.f;
        
        // 색상 변화 모듈
        if(ModuleData.ColorChange)
        {
            particle.vColor = ModuleData.vStartColor + (ModuleData.vEndColor - ModuleData.vStartColor) * particle.NomalizedAge;
        }
    }
    
    // 계산을 통해 변경된 파티클 정보를 적용
    ParticleBuffer[_ID.x] = particle;
}

#endif
