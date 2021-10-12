struct VS_INPUT
{
	float3 inPos : POSITION;
	float3 inColor : COLOR;
	float2 inTexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float3 outColor : COLOR;
	float2 outTexCoord : TEXCOORD;
};

cbuffer myCBuff : register(b0)
{
	float4x4 mat;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	output.outPosition = mul(float4(input.inPos, 1.0f), mat);
	output.outColor = input.inColor;
	output.outTexCoord = input.inTexCoord;
	
	return output;
}