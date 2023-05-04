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
    // ��ƼŬ�� ���ں��� ���� �����尡 �����ϴ� �� ����
    if (ParticleMaxCount <= _ID.x)
        return;
    
    tParticle particle = ParticleBuffer[_ID.x];
    

    if (SpawnModule)
    { 
        // ��ƼŬ�� ��Ȱ��ȭ�� ���
        if (particle.Active == 0.f)
        {
            while (0 <= SpawnCount)
            {
            // ���� SpawnCount�� ������ �����
                int orgvalue = SpawnCount;
                int outvalue = 0;
            // �Ʒ� �Լ��� ����ó���ϴ� �����带 �����ϴ� ������� ó���ϰ� �������.
            // SpawnCount�� SpawnCount - 1�� ��ü�ϰ� outvalue�� ��ü�ϱ� ���� SpawnCount�� �����.
            // ���⼭ ��� �����尡 SpawnCount�� ������ ó���Ǿ� �� ���� ���ڷ� �ٲ���� ���� ������ ���� 
            // �� ���� ���ڷ� ��ü�� �� �ֱ� ������ ���� ������ ����� orgvalue�� ���� ���� SpawnCount - 1�� ��ü��
            // �� �Լ��� ����ó���ϴ� �����带 ���������� ����ϰ� �ϰ� ������ ���� �߻��� �� �ִ� ������ ������.
                InterlockedCompareExchange(SpawnCount, orgvalue, SpawnCount - 1, outvalue);
        
                // ������ ������ ������
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
                    
                    // shape ���¿� ���� spawn �Ǵ� ������ ���� ����
                    // Box shape
                    if(ModuleData.SpawnShapeType == 0)
                    {
                        // 2�������� �ڿ������� ������ �ϹǷ� z���� 0���� �����Ͽ� ��鿡���� �����ǰ� ��.
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
                    
                    // ��ƼŬ ���� ����
                    particle.Mass = 1.f;
                    
                    // AddVelocity ���
                    if(ModuleData.AddVelocity)
                    {
                        // From Center
                        if(ModuleData.AddVelocityType == 0)
                        {
                            // obj -> ���� ��ġ������ ���͸� �븻������ �� ���ǵ带 ����
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
    // ��ƼŬ�� �̹� Ȱ��ȭ�� ���
    if(particle.Active)
    {        
        // Ȱ��ȭ�� ���� Age ����
        particle.PrevAge = particle.Age;
        particle.Age += g_DT;
        particle.NomalizedAge = saturate(particle.Age / particle.LifeTime);
        particle.vForce.xyz = (float3) 0.f;
    
        // Age�� LifeTime�� �Ѿ�� ���� ��Ȱ��ȭ
        if (particle.LifeTime <= particle.Age)
        {
            particle.Active = 0.f;
        }
        
        // RandomForce ����� �����ִٸ�
        if(ModuleData.RandomForce)
        {
            // ù �������̶�� (���� �������� ����)
            if (particle.PrevAge == 0.f)
            {
                float3 vOut1 = (float3) 0.f;
                float3 vOut2 = (float3) 0.f;
                float3 vOut3 = (float3) 0.f;
                
                // ���� 3�� ����
                float fNomalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID, vOut1);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.1f, vOut2);
                GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.2f, vOut3);
                // ������ 0 ~ 1�� ����ȭ �� vForce�� ���� xyz�� ���� RandomForce ����
                float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut3.z));
                particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
            }
            // ù �������� �ƴ϶�� ������ ������ �Ѿ ������ ���ο� RandomForce�� �ֵ��� ��.
            else
            {
                // �ֱ⸦ ���� �����ӿ��� RandomForce�� ������ �� �ֵ���
                // ������������ �ֱ⸦ ���� �������� �̹� �����ӿ� �Ѿ��� ���� ����ǰ� ��.
                int Age = int(particle.Age * (1.f / ModuleData.fNoiseTerm));
                int PrevAge = int(particle.PrevAge * (1.f / ModuleData.fNoiseTerm));
                
                if(Age != PrevAge)
                {
                    float3 vOut1 = (float3) 0.f;
                    float3 vOut2 = (float3) 0.f;
                    float3 vOut3 = (float3) 0.f;
                
                // ���� 3�� ����
                    float fNomalizeThreadID = (float) _ID.x / (float) ParticleMaxCount;
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID, vOut1);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.1f, vOut2);
                    GaussianSample(NoiseTexture, NoiseTexResolution, fNomalizeThreadID + 0.2f, vOut3);
                // xyz�� ���� RandomForce ����, ������ -1 ~ 1�� (�ݴ�� �� ���� �־�� �ϹǷ�)
                    float3 vForce = normalize(float3(vOut1.x, vOut2.x, vOut3.z) * 2.f - 1.f);
                    particle.vRandomForce.xyz = vForce * ModuleData.fNoiseForce;
                }
            }

            particle.vForce.xyz += particle.vRandomForce.xyz;
        }
        
        // ��ƼŬ�� ���� ����� ��� ������ �����Ͽ� ���ӵ� ���
        float3 vAccel = particle.vForce.xyz / particle.Mass;
        particle.vVelocity.xyz += vAccel * g_DT;
        
        // �ӵ� ����(Drag) ���
        // Drag ����� �ӵ� ���� ��� ����� ���� �� ����
        if(ModuleData.Drag)
        {
            // ��ƼŬ�� ���� �ӷ�
            float Speed = length(particle.vVelocity);
            float Drag = ModuleData.StartDrag + (ModuleData.EndDrag - ModuleData.StartDrag) * particle.NomalizedAge;
            
            // ���� �ӷ��� ���Ѽӵ����� ũ�ٸ� ���Ѽӵ��� ����
            if (Drag <= 0.f)
                Drag = 0.001f;
            if(Drag < Speed)
            {
                particle.vVelocity = normalize(particle.vVelocity) * Drag;
            }
        }
        
        // �ӵ��� ���� ��ƼŬ �̵�
        if(ModuleData.Space == 0)
        {
            particle.vWorldPos += particle.vVelocity * g_DT;
        }
        else if(ModuleData.Space == 1)
        {
            particle.vLocalPos += particle.vVelocity * g_DT;
            particle.vWorldPos.xyz = particle.vLocalPos.xyz + ObjectPos.xyz;
        }
        
        // ũ�� ��ȭ ���
        if(ModuleData.ScaleChange)
            particle.ScaleFactor = ModuleData.StartScale + (ModuleData.EndScale - ModuleData.StartScale) * particle.NomalizedAge ;
        else
            particle.ScaleFactor = 1.f;
        
        // ���� ��ȭ ���
        if(ModuleData.ColorChange)
        {
            particle.vColor = ModuleData.vStartColor + (ModuleData.vEndColor - ModuleData.vStartColor) * particle.NomalizedAge;
        }
    }
    
    // ����� ���� ����� ��ƼŬ ������ ����
    ParticleBuffer[_ID.x] = particle;
}

#endif