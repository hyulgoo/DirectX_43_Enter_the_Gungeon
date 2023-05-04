#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

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

// ===================================
// GrayShader
// Mesh	   : RectMesh
// Domain  : DOMAIN_POSTPROCESS
// g_tex_0 : RenderTarget Copy Texture
// ===================================

VS_OUT VS_GrayShader(VS_IN _in)
{
	VS_OUT output = (VS_OUT)0.f;

	// Mesh의 0.5, 0.5 까지인 Pos를 2배로 하여 전체 MDC 공간을 커버하게 해 화면 전체로 확장함.
	output.vPosition = float4(_in.vLocalPos * 2.f, 1.f);
	output.vUV = _in.vUV;

	return output;
}

float4 PS_GrayShader(VS_OUT _in) : SV_Target
{
    float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);

    float vAver = (vColor.r + vColor.g + vColor.b) / 3.f;
	
    vColor = float4(vAver, vAver, vAver, 1.f);
	
    return vColor;
}


// ============================
// Distortion Shader
// mesh : RectMesh
// Domain : DOMAIN_POSTPROCESS
// g_tex_0 : RederTarget Copy Texture
// g_tex_1 : Noise Texture
// ============================

VS_OUT VS_DistortionShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
	
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
	
    return output;
}

//float4 PS_Distortion(VS_OUT _in) : SV_Target
//{
//	float2 vUV = _in.vPosition.xy / g_Resolution;
//		
//	
//	float fChange = cos(( (vUV.x - g_AccTime * 0.05f) / 0.15f) * 2 * 3.1415926535f) * 0.05f;
//
//	vUV.y += fChange;
//
//	float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
//	//vColor.r *= 2.f;
//
//	return vColor;
//}

float4 PS_DistortionShader(VS_OUT _in) : SV_Target
{
    float2 vUV = _in.vPosition.xy / g_Resolution;
	
	// Noise Texture를 참조하고 있다면
	
	if(g_btex_1)
    {
        float2 vNoiseUV = float2(_in.vUV.x - (g_AccTime % 1 * 0.2f), _in.vUV.y);
        float4 vNoise = g_tex_1.Sample(g_sam_0, vNoiseUV);
		
		// 일렁거리는 느낌을 위해 0 ~ 1까지가 아닌 -0.5 ~ 0.5까지로 범위를 바꾸고
		// UV라 0.5도 매우 큰 값이기 때문에 2%만 반영되게 함.
        vNoise = (vNoise - 0.5f) * 0.02f;

		// 노이즈 3번 텍스쳐 기준으로 일렁거리게 함.
        vUV += vNoise.r;
    }
	
    float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
	
    return vColor;
}

float4 PS_ShockWaveShader(VS_OUT _in) : SV_Target
{
	//Sawtooth function to pulse from centre.
    float CurrentTime = 10.f * g_float_0 / 7.f;

	float3 WaveParams = float3(10.0, 1.f, 0.1f);

    float ratio = 1.f;

	//Use this if you want to place the centre with the mouse instead
	//float2 WaveCentre = float2( iMouse.xy / iResolution.xy );

	float2 WaveCentre = float2(0.5, 0.5);
    WaveCentre.y *= ratio;

    float2 texCoord = _in.vPosition.xy / g_Resolution.xy;
	texCoord.y *= ratio;
	float Dist = distance(texCoord, WaveCentre);


    float4 Color = g_tex_0.Sample(g_sam_0, texCoord);

	//Only distort the pixels within the parameter distance from the centre
	if ((Dist <= ((CurrentTime)+(WaveParams.z))) &&
	(Dist >= ((CurrentTime)-(WaveParams.z))))
	{
		//The pixel offset distance based on the input parameters
		float Diff = (Dist - CurrentTime);
		float ScaleDiff = (1.f - pow(abs(Diff * WaveParams.x), WaveParams.y));
		float DiffTime = (Diff * ScaleDiff);

		//The direction of the distortion
		float2 DiffTexCoord = normalize(texCoord - WaveCentre);

		//Perform the distortion and reduce the effect over time
		texCoord += ((DiffTexCoord * DiffTime) / (CurrentTime * Dist * 40.f));
        Color = g_tex_0.Sample(g_sam_0, texCoord);

		//Blow out the color and reduce the effect over time
		Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.f);
	}

	return Color;
}

#endif