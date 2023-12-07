struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
  float3 inNormal : COLOR;
	float2 inTexCoord : TEXCOORD;
};

Texture2D diffuseMap : TEXTURE : register(t0);
Texture2D normalMap : TEXTURE : register(t1);
Texture2D occlusionMap : TEXTURE : register(t2);
Texture2D specularMap : TEXTURE : register(t3);

SamplerState objSamplerState : SAMPLER : register(s0);

cbuffer pixelCBuff : register(b0)
{
	float alpha;
};

cbuffer materialCBuff : register(b1)
{
	float4 Ka;
	float4 Kd;
	float4 Ks;
	float4 Ns;
}

float4 main(PS_INPUT input) : SV_TARGET
{
	float3 lightDir = float3(0.0f, 1.0f, 0.0f);
	float3 sunColor = float3(1.0f, 1.0, 1.0f);
	
	// use texture coordinates
	float3 diffuseColor = diffuseMap.Sample(objSamplerState, input.inTexCoord);
	float3 normalMapSample = normalMap.Sample(objSamplerState, input.inTexCoord);
	// float3 occlusion = occlusionMap.Sample(objSamplerState, input.inTexCoord);
	// float3 specular = specularMap.Sample(objSamplerState, input.inTexCoord);
	
	// next I need lighting
	float3 Ka3 = float3(Ks[0], Ks[1], Ks[2]);
	float3 Kd3 = float3(Kd[0], Kd[1], Kd[2]);
	float3 Ks3 = float3(Ks[0], Ks[1], Ks[2]);
	
	float3 ambient = Ka * 0.8f;
	
	float3 finalNormal = input.inNormal + normalMapSample;
	float3 diffuseFactor = dot(finalNormal, lightDir);
	float3 diffuse = Kd3 * diffuseColor * sunColor * diffuseFactor;
	
	float3 finalColor = ambient + diffuse;
	
	return float4(input.inNormal, 1);
}