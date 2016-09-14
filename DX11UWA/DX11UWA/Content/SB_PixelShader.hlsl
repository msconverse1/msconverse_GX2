struct PIXEL_IN
{
	float4 posH : SV_POSITION;
	float3 uv   : UV;
	float3 norm : NORMAL;
	
};

textureCUBE diffuse : register(t0);
SamplerState filter : register(s0);

float4 main(PIXEL_IN input) : SV_TARGET
{
	return diffuse.Sample(filter,input.uv.xyz);
}