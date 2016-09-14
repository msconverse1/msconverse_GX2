
struct VERTEX_IN
{
	float3 pos: Position;
	float3 uv : UV;
	float3 norm : NORMAL;

};

struct VERTEX_OUT
{
	float3 pos: Position;
	float3 uv : UV;
	float3 norm : NORMAL;
};

VERTEX_OUT main(VERTEX_IN input)
{
	VERTEX_OUT output = (VERTEX_OUT)0;

	output.pos = input.pos;
	output.uv = input.uv;
	output.norm = input.norm;

	return output;
}