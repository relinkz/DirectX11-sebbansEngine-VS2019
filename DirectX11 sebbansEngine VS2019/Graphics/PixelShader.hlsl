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

Texture2D diffuseMap : TEXTURE : register(t0);
Texture2D normalMap : TEXTURE : register(t1);
Texture2D occlusionMap : TEXTURE : register(t2);
Texture2D specularMap : TEXTURE : register(t3);

SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// use texture coordinates
	float3 diffuseColor = diffuseMap.Sample(objSamplerState, input.inTexCoord);
	// float3 normal = normalMap.Sample(objSamplerState, input.inTexCoord);
	// float3 occlusion = occlusionMap.Sample(objSamplerState, input.inTexCoord);
	// float3 specular = specularMap.Sample(objSamplerState, input.inTexCoord);
	
	// next I need lighting
	float3 finalColor = diffuseColor;
	
	return float4(finalColor, alpha);
}