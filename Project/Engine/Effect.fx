#ifndef _EFFECT
#define _EFFECT

#include "value.fx"
#include "func.fx"

// ======================================
// Std2DLightShader
// RasterizerState      : None
// BlendState           : Mask
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

struct VS_Effect_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_Effect_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};



VS_Effect_OUT VS_Effect(VS_Effect_IN _in)
{
    VS_Effect_OUT output = (VS_Effect_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    output.vWorldPos = mul(float4(_in.vLocalPos, 1.f), g_matWorld).xyz;
    
        
    return output;
}


float4 PS_Effect(VS_Effect_OUT _in) : SV_Target
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
    
        // 알파값 0 및 마젠타 색 삭제
    if (0.f == vOutColor.a || vOutColor.r == 1.f && vOutColor.g == 0.f && vOutColor.b == 1.f)
    {
        discard;
    }
    return vOutColor;
}

#endif