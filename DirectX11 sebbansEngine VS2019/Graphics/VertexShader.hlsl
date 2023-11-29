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

cbuffer CameraMatrix : register(b0)
{
	float4x4 matCamera;
};

cbuffer worldMatrix : register(b1)
{
	float4x4 matWorld;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	
	// Place object in the world
	float4 localToWorld = mul(float4(input.inPos, 1.0f), matWorld);
	// What the camera sees
	float4 cameraRelative = mul(localToWorld, matCamera);
	// Apply world space to the model
	output.outPosition = cameraRelative;
	output.outColor = input.inColor;
	output.outTexCoord = input.inTexCoord;
	
	return output;
}