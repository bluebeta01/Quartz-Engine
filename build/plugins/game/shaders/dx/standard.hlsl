Texture2D shaderTexture;
SamplerState SampleType;

cbuffer matracies : register(b0)
{
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

struct VOut
{
    float4 position : SV_POSITION;
	float3 pixelPos : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

VOut VShader(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VOut output;
	
	//position.z += 5;
	
	position.w = 1.0f;
    output.position = mul(position, modelMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.normal = normal.xyz;
	output.tex = texcoord;

	output.pixelPos = mul(modelMatrix, position).xyz;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float3 pixelPos : POSITION, float3 normal : NORMAL, float2 tex : TEXCOORD0) : SV_TARGET
{
	
	
	float3 ambientLight = { 0.3f, 0.3f, 0.3f };
	float3 lightPos = { 0,0,20 };
	normal = normalize(normal);
	float3 lightDir = normalize(lightPos - pixelPos);
	float diff = max(dot(normal, lightDir), 0.0);
	float3 lightColor = {1,1,1};
	float3 diffuse = diff * lightColor;
	
	float3 c = { 1.0f, 1.0f, 1.0f };
	float3 result = (ambientLight + diffuse) * c;
    //return float4(result, 1.0f);
	
	
	
	
	float4 textureColor;
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, tex);
	return textureColor;
    //return textureColor * float4((ambientLight + diffuse) * c, 1.0f);
}