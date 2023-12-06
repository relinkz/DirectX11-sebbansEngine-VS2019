struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
  float3 inColor : COLOR;
	float2 inTexCoord : TEXCOORD;
};

cbuffer pixelCBuff : register(b0)
{
	float alpha;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// use texture coordinates
  float3 diffuseColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	return float4(diffuseColor, alpha);
}