#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

#include "value.fx"
#include "struct.fx"

// ========================
// Particle Render Shader
// mesh : RectMesh

// Parameter
// g_int_0 : Particle Index
// =========================

StructuredBuffer<tParticle>         ParticleBuffer : register(t20);
StructuredBuffer<tParticleModule>   ParticleModuleData : register(t21);
#define ModuleData                  ParticleModuleData[0]

struct VS_IN
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    uint iInstID : SV_InstanceID;
};

VS_OUT VS_ParticleRender(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    //
    //// Local Mesh의 정점에 파티클 배율을 곱하고 월드 위치로 이동시킴
    //float3 vWorldPos = _in.vPos * ParticleBuffer[_in.iInstID].vWorldScale.xyz + ParticleBuffer[_in.iInstID].vWorldPos.xyz;
    //
    //// View, Proj 행렬을 곱해 MDC 좌표계로 이동시킴
    //float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    //output.vPosition = mul(vViewPos, g_matProj);
    //
    //// vUV 전달
    //output.vUV = _in.vUV;
    
    output.vPos = _in.vPos;
    output.iInstID = _in.iInstID;
    
    return output;
}

// GS 사용으로 VS에서는 값을 그대로 return시키기만 함.
// GS 기능
// 파이프라인 제어, 빌보드 처리(mesh가 항상 카메라를 바라보게 함)
struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint   iInstID : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_ParticleRender(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outstream)
{
    uint id = _in[0].iInstID;
    
    if(0 == ParticleBuffer[id].Active)
        return;
    tParticle particle = ParticleBuffer[id];
    
    // Particle에 View 행렬 적용 (항상 z축을 바라보게 함)
    float3 vParticleViewPos = mul(float4(particle.vWorldPos.xyz, 1.f), g_matView).xyz;
    float2 vParticleScale = particle.vWorldScale.xy * particle.ScaleFactor;
    
    // 0 -- 1
    // |    |
    // 3 -- 2
    // 받은 Point 정점을 기준으로 RectMesh 생성
    float3 NewPos[4] =
    {
        // 크기 및 회전을 적용하기 위해서는 이동값을 제일 나중에 적용해야 하므로 pos값 계산을 뒤로 뺸다
        // 또한 2D로 표현될 때 자연스러워야 하기 때문에 z축을 0으로 고정함.
        float3(- vParticleScale.x / 2.f,  + vParticleScale.y / 2.f, 0.f),
        float3(+ vParticleScale.x / 2.f,  + vParticleScale.y / 2.f, 0.f),
        float3(+ vParticleScale.x / 2.f,  - vParticleScale.y / 2.f, 0.f),
        float3(- vParticleScale.x / 2.f,  - vParticleScale.y / 2.f, 0.f)
    };
    
    // RenderMoudle 활성화
    if(ModuleData.Render)
    {
        // 속도 비례 크기 모듈
        if(ModuleData.VelocityScale)
        {
            float fCurSpeed = length(particle.vVelocity);
            // 현재 속도가 최대크기 속도보다 크다면 최대크기로 비교변수를 바꿔둠
            if(ModuleData.MaxSpeed < fCurSpeed)
                fCurSpeed = ModuleData.MaxSpeed;
            
            // 최대 속도 대비 현재 속도의 비율을 구함
            float fRatio = saturate(fCurSpeed / ModuleData.MaxSpeed);
            
            // 비율에 맞는 크기 변화량을 구함
            float3 vDefaultScale = float3(1.f, 1.f, 1.f);
            // 1 ~ 최대크기 까지 비율에 따른 크기
            float3 vScale = vDefaultScale + (ModuleData.vMaxVelocityScale.xyz - vDefaultScale) * fRatio;
            
            NewPos[0] = NewPos[0] * vScale;
            NewPos[3] = NewPos[3] * vScale;
        }
        
        // 속도 정렬 모듈
        if(ModuleData.VelocityAlignment)
        {
            // 파티클 월드 기준 속도를 View 공간으로 변환함
            float3 vVelocity = normalize(particle.vVelocity);
            vVelocity = mul(float4(vVelocity, 0.f), g_matView).xyz;
            
            // 파티클 Right 방향과 이동 방향을 내적해서 둘 사이의 각도를 구함
            float3 vRight = float3(1.f, 0.f, 0.f);
            float fTheta = acos(dot(vRight, vVelocity));
            
            // 내적의 결과는 코사인 예각을 반환하기 때문에 PI를 넘어가는 경우(y값이 -일 경우)에는 2PI - fTheta해줌
            if(vVelocity.y < vRight.y)
            {
                fTheta = (2.f * 3.1415926535f) - fTheta;
            }
            
            // 구한 각도로 Z축 회전 행렬을 만든다.
            float3x3 matRotZ =
            {
                 cos(fTheta), sin(fTheta),   0,
                -sin(fTheta), cos(fTheta),   0,
                           0,           0, 1.f,
            };

            // 모든 정점에 Z축 회전 행렬을 적용해줌
            for (int i = 0; i < 4; ++i)
            {
                NewPos[i] = mul(NewPos[i], matRotZ);
            }
        }
    }
    
    // 회전까지 적용한 후 이동값을 더해줌.
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    if (ModuleData.Animation)
    {
        int CurFrm =  int(particle.NomalizedAge * ModuleData.FrmCount);
        float2 LeftTop = ModuleData.LeftTop;
        float2 Slice = ModuleData.Slice;
        int YCount = 0;
        if (CurFrm >= ModuleData.XCount)
        {
            YCount = CurFrm / ModuleData.XCount;
            while (CurFrm < ModuleData.XCount)
            {
                CurFrm -= ModuleData.XCount;
            }
        }
        LeftTop = float2(LeftTop.x + (CurFrm * Slice.x), LeftTop.y + (YCount * Slice.y));
        
        output[0].vUV = LeftTop - ModuleData.Offset;
        output[1].vUV = float2(LeftTop.x + Slice.x, LeftTop.y) - ModuleData.Offset;
        output[2].vUV = LeftTop + Slice - ModuleData.Offset;
        output[3].vUV = float2(LeftTop.x, LeftTop.y + Slice.y) - ModuleData.Offset;
    }
    else
    {    
        output[0].vUV = float2(0.f, 0.f);
        output[1].vUV = float2(1.f, 0.f);
        output[2].vUV = float2(1.f, 1.f);
        output[3].vUV = float2(0.f, 1.f);
    }
    output[0].vPosition = mul(float4(NewPos[0] + vParticleViewPos, 1.f), g_matProj);
    output[0].iInstID = id;
    
    output[1].vPosition = mul(float4(NewPos[1] + vParticleViewPos, 1.f), g_matProj);
    output[1].iInstID = id;
    
    output[2].vPosition = mul(float4(NewPos[2] + vParticleViewPos, 1.f), g_matProj);
    output[3].iInstID = id;
    
    output[3].vPosition = mul(float4(NewPos[3] + vParticleViewPos, 1.f), g_matProj);
    output[3].iInstID = id;
    
    _outstream.Append(output[0]);
    _outstream.Append(output[1]);
    _outstream.Append(output[2]);
    _outstream.RestartStrip();
    
    _outstream.Append(output[0]);
    _outstream.Append(output[2]);
    _outstream.Append(output[3]);
    _outstream.RestartStrip();
    
}

float4 PS_ParticleRender(GS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if(g_btex_0)
    {
        // 이방성 필터링 사용 안함
        vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        vOutColor.rgb *= ParticleBuffer[_in.iInstID].vColor.rgb;
        
    }
    return vOutColor;
}


#endif