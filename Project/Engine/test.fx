#ifndef _TEST
#define _TEST

#include "value.fx"

// 입력받는 구조체 : 좌표와 컬러값을 갖고있음.
struct VS_IN
{
	float3 vPos   : POSITION; // semantic 정점 구조체 안에서 내가 원하는 정보를 지칭하는
	float4 vColor : COLOR;
    float2 vUV    : TEXCOORD;
};

// 반환하는 구조체 : 좌표값만 반환함(알파값을 더함)
struct VS_OUT
{
	float4 vPosition : SV_Position;
    float4 vOutColor : COLOR;
    float2 vOutUV    : TEXCOORD;
};

// Vertex shader 
// Local Space에 있는 물체를 NDC 좌표계로 이동시킴
// 들어온 좌표값에 알파값을 더해서 반환시킴
VS_OUT VS_Test(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
    
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
	
    output.vPosition = vProjPos;
    output.vOutColor = _in.vColor;
    output.vOutUV = _in.vUV;

	return output;
}

// Pixel shader
// 반환시킨 값에 지정한 색을 덧씌워 적용시킴
float4 PS_Test(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
  
    if (g_int_0 == 0)
        vColor = g_tex_0.Sample(g_sam_0, _in.vOutUV);
    else if (g_int_0 == 1)
        vColor = g_tex_0.Sample(g_sam_1, _in.vOutUV);
    
    return vColor;
}

#endif