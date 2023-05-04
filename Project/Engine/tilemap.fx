#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "func.fx"

#define UseFrame   g_int_2
#define Framethick g_float_0

struct tTile
{
    float2 vLeftTop;
    float2 vSlice;
    float2 vPos;
    uint   tState;
    int    padding;
    
};

// ============================
// TileMap Shader
// 
// RS_TYPE : CULL_NONE
// DS_TYPE : LESS
// BS_TYPE : MASK

// Parameter
// g_tex_0 : Tile Atlas Texture
// g_int_0 : Tile X Count
// g_int_1 : Tile Y Count
StructuredBuffer<tTile> TileBuffer : register(t20);
// ============================
struct VTX_TILEMAP_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_TILEMAP_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vWorlsPos : POSITIONT;
};

VTX_TILEMAP_OUT VS_TileMap(VTX_TILEMAP_IN _in)
{
    VTX_TILEMAP_OUT output = (VTX_TILEMAP_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV * float2(g_int_0, g_int_1);
    output.vWorlsPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    
    return output;
}

float4 PS_TileMap(VTX_TILEMAP_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    // �Ҽ���Ʈ�� ���ϱ�, frac(_in.vUV) : Ÿ�� �� ĭ ������ �ȼ��� ������� ��ġ (0~1)
    // ������Ʈ�� ���ϱ�, floor(_in.vUV) : ��ü Ÿ�� �߿��� �ε���(��, ��)
    int2 TileIdx = floor(_in.vUV);
    int BufferIdx = g_int_0 * TileIdx.y + TileIdx.x;
    float2 vUV = TileBuffer[BufferIdx].vLeftTop + (TileBuffer[BufferIdx].vSlice * frac(_in.vUV));
    
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_1, vUV);
    }
    if (UseFrame)
    {
        float4 FrameColor;
        if (0 == TileBuffer[BufferIdx].tState)
            FrameColor = float4(1.f, 1.f, 1.f, 1.f);
        else if (1 == TileBuffer[BufferIdx].tState)
            FrameColor = float4(0.f, 1.f, 0.f, 1.f);
        else if (2 == TileBuffer[BufferIdx].tState)
            FrameColor = float4(1.f, 0.f, 0.f, 1.f);
    
        if (TileIdx.x == g_int_0 - 1)
        {
            if (frac(_in.vUV.x) > 1 - (2.f * Framethick) / 100)
                vOutColor = FrameColor;
        }      
        if (TileIdx.y == g_int_1 - 1)
        {
             if (frac(_in.vUV.y) > 1 - (2.f * Framethick) / 100)
                vOutColor = FrameColor;
        }
        if (TileIdx.x == 0)
        {
            if (frac(_in.vUV.x) < (2.f * Framethick) / 100)
                vOutColor = FrameColor;
        }
        if (TileIdx.y == 0)
        {
            if (frac(_in.vUV.y) < (2.f * Framethick) / 100)
                vOutColor = FrameColor;
        }
    
        if (frac(_in.vUV.x) > 1 - Framethick / 100 || frac(_in.vUV.x) < Framethick / 100)
            vOutColor = FrameColor;
        if (frac(_in.vUV.y) > 1 - Framethick / 100 || frac(_in.vUV.y) < Framethick / 100)
            vOutColor = FrameColor;
    }
    
    // Lighting ó��
    float3 vNormal = (float3) 0.f;
    tLightColor vLightColor = (tLightColor) 0.f;
    // vNormal�� Zero Vector���� �Ǵ��Ͽ� �̹����� ���������� �ִ� �� Ȯ���ϰ�
    // �׿� ���� ����ó���� ��
    CalcLight2D(_in.vWorlsPos, vLightColor);
    
    
    
    vOutColor.rgb *= (vLightColor.vDiffuse.rgb + vLightColor.vAmbient.rgb);
        
    
    return vOutColor;
}
#endif