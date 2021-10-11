struct PS_INPUT
{
	float4 inPosition : SV_POSITION;
    float3 inColor : COLOR;
	float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	// use texture coordinates
    float3 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
	return float4(pixelColor, 1.0f);
	
	// use colors
    // return float4(input.inColor, 1.0f);
}