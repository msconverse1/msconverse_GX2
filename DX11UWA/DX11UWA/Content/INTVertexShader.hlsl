struct VERTEX_IN
{
	float3 pos : POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
	float4 tanget : TANGENT;
};

struct VERTEX_OUT
{
	float4 posH : SV_POSITION;
	float3 uv : UV;
	float3 norm : NORMAL;
	float3 posW : WORLDPOS;
	float3 tanget : TANGENT;
	float3 bitanget : BITANGENT;
};



cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model[10];
	matrix view;
	matrix projection;
};


VERTEX_OUT main(VERTEX_IN input, uint ID : SV_InstanceID)
{
	VERTEX_OUT output = (VERTEX_OUT)0;

	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, model[ID]);

	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.posH = pos;
	output.uv = input.uv;
	output.norm = mul(input.norm, (float3x3)model[ID]);
	output.tanget = mul(float3(1.0f,1.0f,0.0f), (float3x3)model[ID]);
	output.bitanget = mul(cross(input.norm.xyz, input.tanget.xyz), (float3x3)model[ID]);
return output;
}