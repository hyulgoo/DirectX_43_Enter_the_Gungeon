#ifndef _FUNC
#define _FUNC

#include "value.fx"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        // DIRECTION 광원
        if(g_Light2DBuffer[i].LightType == 0)
        {
            //전체에 같은 색과 주변광을 적용시킴
            _Light.vDiffuse += g_Light2DBuffer[i].Color.vDiffuse;
            _Light.vAmbient += g_Light2DBuffer[i].Color.vAmbient;
        }
        // POINT 광원
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            // 광원의 위치와 픽셀의 위치를 가져옴
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // 광원과 픽셀의 거리 비율에 따라 빛의 세기를 조절함
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
            
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
        }
        // SPOT 광원
        else if (g_Light2DBuffer[i].LightType == 2)
        { 
            // 광원의 위치와 픽셀의 위치를 가져옴
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // 광원의 우벡터 방향정보를 가져옴
            float vLightDir = normalize(g_Light2DBuffer[i].vWorldDir);
            
            float2 lightDir = normalize(vWorldPos.xy - vLightWorldPos.xy);
            float2 fWorldDir = normalize(g_Light2DBuffer[i].vWorldDir.xy);
            float fAngle = cos(g_Light2DBuffer[i].Angle / 2.f);
            if (dot(normalize(lightDir), normalize(g_Light2DBuffer[i].vWorldDir.xy)) < cos(g_Light2DBuffer[i].Angle / 2.f))
            {
                _Light.vDiffuse.rgba = float4(0.f, 0.f, 0.f, 1.f); //outside of radius make it black
            }
            
            else
            {
                float fAnglePow = saturate(1.f -(fAngle / dot(lightDir, fWorldDir)) * 0.9f);
          
                // 광원과 픽셀의 거리 비율에 따라 빛의 세기를 조절함
                float fDistance = abs(distance(vWorldPos, vLightWorldPos));
                float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
            
                _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow * fAnglePow;
            }
            }
        }
}

void CalcLight2D(float3 _vWorldPos, float3 _vWorldDir, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        if (g_Light2DBuffer[i].LightType == 0)
        {   
            // 광원의 방향값과 픽셀에 있는 방향정보의 스칼라 내적을 구하여 비율로 나타냄
            float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, _vWorldDir));
            // 빛에 각도에 따른 값을 곱해 원래 픽셀에 적용시킴
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            // 광원의 위치와 픽셀의 위치를 가져옴
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // 광원의 위치에서 픽셀에 위치로 가는 방향정보를 구함.
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            // dot : 두 점의 스칼라 내적을 반환
            // 광원에서 픽셀로 가는 방향과, 픽셀의 방향정보의 내적을 구함.
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            // 광원과 픽셀의 거리에 따른 비율을 구함.
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
            
            // 최종 출력은 원래 픽셀의 색에 빛의 거리 및 각도에 따른 비율을 합산값을 더함.
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        { 
            //광원의 x축으로 타원형 빛을 발사한다.
            float3 vLightDir = float3(g_Light2DBuffer[i].vWorldDir.xy, 0.f);
            //광원의 중심의 WorldPosition
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            //광원의 빛을 받는 물체의 Position
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
           // 광원 중심에서 물체를 향하는 방향
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            
            
            if (acos(dot(vLight, vLightDir)) < g_Light2DBuffer[i].Angle / 2.f)
            {
                float fDistance = abs(distance(vWorldPos, vLightWorldPos));
                //광원에서 멀리 있는 물체는 흐려지게 함
                float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
                //양옆으로 갈수록 흐려지게 함
                float fAnglePow = saturate(1.f - acos(dot(vLight, vLightDir)) / (g_Light2DBuffer[i].Angle / 2.f));
        
                _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow * fAnglePow;
            }
            else
            {
                _Light.vDiffuse.rgb = float3(0.f, 0.f, 0.f);
            }

        }
    }

}

// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizeThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizeThreadID, 0.5f);
    
    vUV.x += g_AccTime * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 계산
    vUV.y -= (sin((_NomalizeThreadID - (g_AccTime) /*시간에 따라 그래프를 우측으로 이동*/)
                * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    // 주변에 참조하는 UV가 1을 넘어갈 경우 정수를 버리고 소수점만 남김.
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    // 0 아래일 경우 1 - frac(UV)로 초과한 만큼 반복된 것 처럼 UV를 갖도록 함.
    else if (vUV.x < 0.f)
        vUV.x = 1.f - frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f - frac(vUV.y);
    
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            // 가우시안 필터 적용 offset으로 좌상단부터 차례대로 비율값을 곱해 적용함.
            vOut += _NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

#endif