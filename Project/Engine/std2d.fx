#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

// ============================
// Std2DShader
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : AnimUse
// g_vec2_0             : AnimAtlas LeftTop
// g_vec2_1             : AnimAtlas Slice
//
// g_tex_0              : Output Texture
// ============================
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
        
    if (0.f == vOutColor.a)
        discard;

    
    return vOutColor;
}




// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : AB
// DepthStencilState    : Less
// Parameter
#define bAnimUse        g_int_0
#define LeftTop         g_vec2_0
#define Slice           g_vec2_1
#define Offset          g_vec2_2
#define BackSize        g_vec2_3
// g_tex_0              : Output Texture
// g_tex_1              : Nomal Texture
// ======================================

struct VS_Light_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Light_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};



VS_Light_OUT VS_Std2DLight(VS_Light_IN _in)
{
    VS_Light_OUT output = (VS_Light_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;
    
        
    return output;
}


float4 PS_Std2DLight(VS_Light_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
       
    if (g_btex_0)
    {
        if(bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= (BackSize - Slice) / 2.f;
            vUV -= Offset;
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_1, vUV);
                // �������� ��üȿ��
                if (g_int_3)
                    vOutColor.a *= 0.4f;
                if (1 == g_int_2)
                {
                    int Blink = (frac(g_AccTime) / 0.1f) % 2;
                    if (0 == Blink)
                    {
                        if (0.f == vOutColor.a)
                            discard;
                        else 
                        vOutColor = float4(1.f, 1.f, 1.f, 0.7f);
                    }
                }
                else if (2 == g_int_2)
                {
                    int Blink = (frac(g_AccTime) / 0.1f) % 2;
                    if (0 == Blink)
                    {
                        vOutColor.a *= 0.6f;
                    }
                }

            }
            else
            {
                discard;
            }
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    // �븻��(�ؽ����� ��������)�� �ִٸ�
    float3 vNormal = (float3)0.f;
    if(g_btex_1)
    {
        // Normal �� ����
        vNormal = g_tex_1.Sample(g_sam_1, _in.vUV);
        
        // 0 ~ 1 ������ -1 ~ 1�� ����
        vNormal = (vNormal * 2.f) - 1.f;
        
        // NormalTexture ��ǥ��� y��� z���� �ݴ�� �Ǿ�����.
        float f = vNormal.y;
        vNormal.y = vNormal.z;
        vNormal.z = f;
        
        // Texture ���� ������ Normal ������ ����� ��ȯ��Ų��.
        vNormal = normalize(mul(float4(vNormal, 0.f), g_matWorld)).xyz;

    }
        // ���İ� 0 �� ����Ÿ �� ����
    if (0.f == vOutColor.a || vOutColor.r == 1.f && vOutColor.g == 0.f && vOutColor.b == 1.f)
    {
        discard;
    }
    
    // Lighting ó��
        tLightColor vLightColor = (tLightColor) 0.f;
    // vNormal�� Zero Vector���� �Ǵ��Ͽ� �̹����� ���������� �ִ� �� Ȯ���ϰ�
    // �׿� ���� ����ó���� ��
    if (dot(vNormal, vNormal) == 0.f)
    {
        CalcLight2D(_in.vWorldPos, vLightColor);
    }
    else
    {
    // �ؽ��Ŀ� ���������� ���� �� Lighting
        CalcLight2D(_in.vWorldPos, vNormal, vLightColor);
    }
    
    
    vOutColor.rgb *= (vLightColor.vDiffuse.rgb + vLightColor.vAmbient.rgb);
        
    return vOutColor;
}

float4 PS_Std2DAnim(VS_OUT _in) : SV_Target
{
    
    float4 vOutColor = (float4) 0.f;
       
    if (g_btex_0)
    {
        if (bAnimUse)
        {
            float2 vUV = LeftTop + (BackSize * _in.vUV);
            vUV -= (BackSize - Slice) / 2.f;
            vUV -= Offset;
            
            if (LeftTop.x < vUV.x && vUV.x < LeftTop.x + Slice.x
                && LeftTop.y < vUV.y && vUV.y < LeftTop.y + Slice.y)
            {
                vOutColor = g_tex_0.Sample(g_sam_1, vUV);
                // �������� ��üȿ��
                if (g_int_3)
                    vOutColor.a *= 0.4f;
                if (1 == g_int_2)
                {
                    int Blink = (frac(g_AccTime) / 0.1f) % 2;
                    if (0 == Blink)
                    {
                        if (0.f == vOutColor.a)
                            discard;
                        else
                            vOutColor = float4(1.f, 1.f, 1.f, 0.7f);
                    }
                }
                else if (2 == g_int_2)
                {
                    int Blink = (frac(g_AccTime) / 0.1f) % 2;
                    if (0 == Blink)
                    {
                        vOutColor.a *= 0.6f;
                    }
                }

            }
            else
            {
                discard;
            }
        }
        else
        {
            vOutColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
         // ���İ� 0 �� ����Ÿ �� ����
    if (0.f == vOutColor.a || vOutColor.r == 1.f && vOutColor.g == 0.f && vOutColor.b == 1.f)
    {
        discard;
    }
    
    return vOutColor;
}


#endif