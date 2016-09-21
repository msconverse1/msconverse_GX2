// constant buffer for my directional light's information
cbuffer DIRECTIONAL : register(b0)
{
	float4 DIR_direction;
	float4 DIR_color;

};
cbuffer Point : register(b1)
{
	float4 p_pos;
	float4 p_color;
	float4 p_ratio;

};
cbuffer Spot : register(b2)
{
	float4 SPOT_pos;
	float4 SurfaceRatio;
	float4 SpotFactor;
	float4 light_rad;
	float4 outerCon;
	float4 innerCon;
	float4 ConeDir;
	float4 SPOT_color;

};

struct PIXEL_IN
{
	float4 posH : SV_POSITION;
	float3 uv   : UV;
	float3 norm : NORMAL;
	float3 posW : WORLDPOS;
	float3 tanget : TANGENT;
	float3 bitanget : BITANGENT;
};

Texture2D diffuse : register(t0);
Texture2D normaldiffuse : register(t1);

SamplerState filter : register(s0);

float4 main(PIXEL_IN input) : SV_TARGET
{
	float3 nm_normal = normaldiffuse.Sample(filter,input.uv.xy);
	nm_normal = (nm_normal * 2) - 1.0f;
	//stored from the texture
	float4 texture_color = diffuse.Sample(filter,input.uv.xy);
	input.norm = normalize(input.norm);
	input.tanget = normalize(input.tanget);
	input.bitanget = normalize(input.bitanget);

	float3x3 TBN;
	TBN[0] = input.tanget.xyz;
	TBN[1] = input.bitanget.xyz;
	TBN[2] = input.norm.xyz;

	nm_normal = mul(nm_normal, TBN);
	// calculations for directional light
	float3 light_ratio = dot(-DIR_direction.xyz, nm_normal);
	float3 final_dir_color = light_ratio* DIR_color;
	//spot 
	
	float3 lightdir = normalize(SPOT_pos - input.posW);
	float3 surfaceratio = saturate(dot(-lightdir, normalize(ConeDir)));
	float3 spotfactor = (surfaceratio > outerCon.x) ? 1 : 0;

	float3 lightratio = saturate(dot(lightdir, nm_normal));
	float3 spot_result = spotfactor*lightratio*SPOT_color;
	//point 
	float3 point_lightdir = normalize(p_pos - input.posW);
	float3 point_lightratio = dot(point_lightdir, nm_normal);
	float3 point_result = point_lightratio*p_color; 
	float4 final_color = float4(final_dir_color, 0.0f) + float4(spot_result, 0.0f) + float4(point_result, 0.0f);
	// directional                  spot                         point
	if (texture_color.x != 0 && texture_color.y != 0 && texture_color.z != 0)
		final_color.xyz *= texture_color.xyz;
	return saturate(float4(final_color.xyz, texture_color.a));
}