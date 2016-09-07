#pragma once
using namespace DirectX;
namespace DX11UWA
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexPositionColor
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	struct VertexPositionUVNormal
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 uv;
		DirectX::XMFLOAT3 normal;
	};

	struct Dirlight
	{
		XMFLOAT4 direction;
		XMFLOAT4 color;
	};
	struct pointlight
	{
		XMFLOAT4 pos;
		XMFLOAT4 color;
		XMFLOAT4 rad;
	};
	struct Spot
	{
		XMFLOAT4 pos;
		XMFLOAT4 SurfaceRatio;
		XMFLOAT4 SpotFactor;
		XMFLOAT4 light_rad;
		XMFLOAT4 outerCon;
		XMFLOAT4 innerCon;
		XMFLOAT4 ConeDir;
		XMFLOAT4 color;
	};
}