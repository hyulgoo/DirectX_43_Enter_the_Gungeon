#ifndef _FUNC
#define _FUNC

#include "value.fx"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        // DIRECTION ����
        if(g_Light2DBuffer[i].LightType == 0)
        {
            //��ü�� ���� ���� �ֺ����� �����Ŵ
            _Light.vDiffuse += g_Light2DBuffer[i].Color.vDiffuse;
            _Light.vAmbient += g_Light2DBuffer[i].Color.vAmbient;
        }
        // POINT ����
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            // ������ ��ġ�� �ȼ��� ��ġ�� ������
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // ������ �ȼ��� �Ÿ� ������ ���� ���� ���⸦ ������
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
            
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
        }
        // SPOT ����
        else if (g_Light2DBuffer[i].LightType == 2)
        { 
            // ������ ��ġ�� �ȼ��� ��ġ�� ������
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // ������ �캤�� ���������� ������
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
          
                // ������ �ȼ��� �Ÿ� ������ ���� ���� ���⸦ ������
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
            // ������ ���Ⱚ�� �ȼ��� �ִ� ���������� ��Į�� ������ ���Ͽ� ������ ��Ÿ��
            float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, _vWorldDir));
            // ���� ������ ���� ���� ���� ���� �ȼ��� �����Ŵ
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            // ������ ��ġ�� �ȼ��� ��ġ�� ������
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
            // ������ ��ġ���� �ȼ��� ��ġ�� ���� ���������� ����.
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            // dot : �� ���� ��Į�� ������ ��ȯ
            // �������� �ȼ��� ���� �����, �ȼ��� ���������� ������ ����.
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            // ������ �ȼ��� �Ÿ��� ���� ������ ����.
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
            
            // ���� ����� ���� �ȼ��� ���� ���� �Ÿ� �� ������ ���� ������ �ջ갪�� ����.
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        { 
            //������ x������ Ÿ���� ���� �߻��Ѵ�.
            float3 vLightDir = float3(g_Light2DBuffer[i].vWorldDir.xy, 0.f);
            //������ �߽��� WorldPosition
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            //������ ���� �޴� ��ü�� Position
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);
            
           // ���� �߽ɿ��� ��ü�� ���ϴ� ����
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            
            
            if (acos(dot(vLight, vLightDir)) < g_Light2DBuffer[i].Angle / 2.f)
            {
                float fDistance = abs(distance(vWorldPos, vLightWorldPos));
                //�������� �ָ� �ִ� ��ü�� ������� ��
                float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
                //�翷���� ������ ������� ��
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
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV ���
    vUV.y -= (sin((_NomalizeThreadID - (g_AccTime) /*�ð��� ���� �׷����� �������� �̵�*/)
                * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
    // �ֺ��� �����ϴ� UV�� 1�� �Ѿ ��� ������ ������ �Ҽ����� ����.
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    // 0 �Ʒ��� ��� 1 - frac(UV)�� �ʰ��� ��ŭ �ݺ��� �� ó�� UV�� ������ ��.
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
            // ����þ� ���� ���� offset���� �»�ܺ��� ���ʴ�� �������� ���� ������.
            vOut += _NoiseTex[pixel + offset + int2(j, i)] * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}

#endif