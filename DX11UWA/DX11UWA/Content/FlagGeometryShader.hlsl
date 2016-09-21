struct GSOutput
{
	float4 pos: SV_Position;
	float3 uv : UV;
	float3 norm : NORMAL;
	float3 posW : WORLDPOS;
};

struct GS_Input
{
	float3 pos: Position;
	float3 uv : UV;
	float3 norm : NORMAL;
};

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

[maxvertexcount(8)]
void main(point GS_Input input[1], inout TriangleStream<GSOutput> output)
{

		GSOutput element[8] = { (GSOutput)0,(GSOutput)0,(GSOutput)0,(GSOutput)0, (GSOutput)0,(GSOutput)0,(GSOutput)0,(GSOutput)0};
		element[0].pos= float4(input[0].pos,1.0f);
		element[1].pos = float4(input[0].pos, 1.0f);
		element[2].pos = float4(input[0].pos, 1.0f);
		element[3].pos = float4(input[0].pos, 1.0f);
		element[4].pos = float4(input[0].pos, 1.0f);
		element[5].pos = float4(input[0].pos, 1.0f);
		element[6].pos = float4(input[0].pos, 1.0f);
		element[7].pos = float4(input[0].pos, 1.0f);
		//front face
		element[0].pos.x -= 1.0f;
		element[0].pos.y += 1.0f;
		element[1].pos.x += 1.0f;
		element[1].pos.y += 1.0f;
		element[2].pos.x -= 1.0f;
		element[2].pos.y -= 1.0f;
		element[3].pos.x += 1.0f;
		element[3].pos.y -= 1.0f;
		element[0].uv = float3(0,0,0);
		element[1].uv = float3(1,0,0);
		element[2].uv = float3(0,1,0);
		element[3].uv = float3(1,1,0);
		//back face

		element[4].pos.x += 1.0f;
		element[4].pos.y += 1.0f;
		element[5].pos.x -= 1.0f;
		element[5].pos.y += 1.0f;
		element[6].pos.x += 1.0f;
		element[6].pos.y -= 1.0f;
		element[7].pos.x -= 1.0f;
		element[7].pos.y -= 1.0f;
		element[4].uv = float3(0, 0, 0);
		element[5].uv = float3(1, 0, 0);
		element[6].uv = float3(0, 1, 0);
		element[7].uv = float3(1, 1, 0);

		for (uint i = 0; i < 4; i++)
		{
			element[i].pos = mul(element[i].pos, model);
			element[i].posW = element[i].pos;
			element[i].pos = mul(element[i].pos, view);
			element[i].pos = mul(element[i].pos, projection);
			element[i].norm = mul(input[0].norm, (float3x3)model);
			
		output.Append(element[i]);
			
		}
		output.RestartStrip();
		for (uint i = 4; i < 8; i++)
		{
			element[i].pos = mul(element[i].pos, model);
			element[i].pos = mul(element[i].pos, view);
			element[i].pos = mul(element[i].pos, projection);
			element[i].posW = element[i].pos;
			element[i].norm = mul(-input[0].norm, (float3x3)model);

			output.Append(element[i]);

		}

}