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
	float xOffset;
	float yOffset;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	input.inPos.x += xOffset;
	input.inPos.y += yOffset;
	
	output.outPosition = float4(input.inPos, 1.0f);
	output.outColor = input.inColor;
	output.outTexCoord = input.inTexCoord;
	
	return output;
}