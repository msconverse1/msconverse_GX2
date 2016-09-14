
struct VERTEX_IN
{
	float3 pos : POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
};

struct VERTEX_OUT
{
	float4 posH : SV_POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
};

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};


VERTEX_OUT main(VERTEX_IN input)
{
	VERTEX_OUT output = (VERTEX_OUT)0;

	float4 pos = float4(input.pos, 1.0f);
	output.uv = pos.xyz;

	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.posH = pos;
	output.norm = mul(input.norm, (float3x3)model);

	return output;
}