
//struct VS_INPUT
//{
//	float4 Position : POSITION;
//	float3 Normal : NORMAL;
//	float2 TexCoord : TEXCOORD;
//};
//
//struct VS_OUTPUT
//{
//	float4 Position : SV_POSITION;
//	float4 WorldPosition : POSITION;
//	float3 Normal : NORMAL;
//	float2 TexCoord : TEXCOORD;
//};
//
//float4 PS(VS_OUTPUT Input) : SV_Target
//{
//
//	// Get light direction for this fragment
//	float3 lightDir = normalize(light.Position - Input.WorldPosition);
//
//	// Note: Non-uniform scaling not supported
//	float diffuseLighting = saturate(dot(Input.Normal, -lightDir)); // per pixel diffuse lighting
//
//																	// Introduce fall-off of light intensity
//	diffuseLighting *= ((length(lightDir) * length(lightDir)) / dot(light.Position - Input.WorldPosition, light.Position - Input.WorldPosition));
//
//	// Using Blinn half angle modification for perofrmance over correctness
//	float3 h = normalize(normalize(CameraPosition.xyz - Input.WorldPosition) - lightDir);
//	float specLighting = pow(saturate(dot(h, Input.Normal)), 2.0f);
//
//	return saturate(light.Ambient + (light.Diffuse * diffuseLighting * 0.6f) + (specLighting * 0.5f));
//}