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
	float4 alpha;
	float4 lightPos;
	float4 lightColor;
	float4 cameraPos;
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
	float3 viewDir = normalize(cameraPos);
	float3 sunColor = lightColor;
	
	// use texture coordinates
	float3 diffuseColor = diffuseMap.Sample(objSamplerState, input.inTexCoord);
	float3 normalMapSample = normalMap.Sample(objSamplerState, input.inTexCoord);
	float3 occlusion = occlusionMap.Sample(objSamplerState, input.inTexCoord);
	float3 specularSample = specularMap.Sample(objSamplerState, input.inTexCoord);
	
	// next I need lighting
	float3 Ka3 = float3(Ks[0], Ks[1], Ks[2]);
	float3 Kd3 = float3(Kd[0], Kd[1], Kd[2]);
	float3 Ks3 = float3(Ks[0], Ks[1], Ks[2]);
	
	float3 ambient = Ka * 0.2f;
	
	float3 finalNormal = normalize(input.inNormal + normalMapSample);
	float3 pixelPos = input.inPosition;
	float3 lightPos3 = lightPos;
	float3 lightDir = normalize(pixelPos - lightPos3);
	//float3 lightDir = normalize(lightPos);
	float3 diffuseFactor = max(0.0f, dot(finalNormal, lightDir));
	float3 diffuse = Kd3 * diffuseColor * sunColor * diffuseFactor;
	
	float shininess = 2.0f;
	float3 reflectionDir = reflect(-lightDir, finalNormal);
	float specularFactor = pow(max(0.0f, dot(reflectionDir, viewDir)), shininess);
	float3 specular = Ks3 * specularSample * sunColor * specularFactor;
	
	float3 finalColor = ambient + diffuse + specular;
	
	return float4(lightDir, 1);
}