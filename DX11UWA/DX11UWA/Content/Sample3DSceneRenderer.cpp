#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "modelloader.h"
#include "..\Common\DirectXHelper.h"

using namespace DX11UWA;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	memset(m_kbuttons, 0, sizeof(m_kbuttons));
	m_currMousePos = nullptr;
	m_prevMousePos = nullptr;
	memset(&m_camera, 0, sizeof(XMFLOAT4X4));

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources(void)
{
	auto window = m_deviceResources->GetD3DDevice();
	

	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / (outputSize.Height/2);
	float fovAngleY = 70.0f * XM_PI / 180.0f; 

		Screens[0].Height = outputSize.Height / 2;
		Screens[0].Width = outputSize.Width ;
		Screens[0].MinDepth = 0;
		Screens[0].MaxDepth = 1;
		Screens[0].TopLeftX = 0;
		Screens[0].TopLeftY = 0;

		Screens[1].Height = outputSize.Height / 2;
		Screens[1].Width = outputSize.Width;
		Screens[1].MinDepth = 0;
		Screens[1].MaxDepth = 1;
		Screens[1].TopLeftX = 0;
		Screens[1].TopLeftY = outputSize.Height / 2;





	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 2.7f, -10.5f, 0.0f };
	static const XMVECTORF32 eye2 = { 0.0f, 1.7f, -10.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_camera[0], XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&m_camera[1], XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye2, at, up)));
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtLH(eye, at, up)));

}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
		LightRotation();
	}

	//changed moveSpeed from 1 to 2
	// Update or move camera here
	UpdateCamera(timer, 8.0f, 0.75f);

}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Sample3DSceneRenderer::UpdateCamera(DX::StepTimer const& timer, float const moveSpd, float const rotSpd)
{
	const float delta_time = (float)timer.GetElapsedSeconds();
	for (unsigned int i = 0; i < 1; i++)
	{


		if (m_kbuttons['W'])
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, moveSpd * delta_time);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}
		if (m_kbuttons['S'])
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -moveSpd * delta_time);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}
		if (m_kbuttons['A'])
		{
			XMMATRIX translation = XMMatrixTranslation(-moveSpd * delta_time, 0.0f, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}
		if (m_kbuttons['D'])
		{
			XMMATRIX translation = XMMatrixTranslation(moveSpd * delta_time, 0.0f, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}
		if (m_kbuttons['X'])
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, -moveSpd * delta_time, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}
		if (m_kbuttons[VK_SPACE])
		{
			XMMATRIX translation = XMMatrixTranslation(0.0f, moveSpd * delta_time, 0.0f);
			XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
			XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
			XMStoreFloat4x4(&m_camera[i], result);
		}

		if (m_currMousePos)
		{
			if (m_currMousePos->Properties->IsRightButtonPressed && m_prevMousePos)
			{
				float dx = m_currMousePos->Position.X - m_prevMousePos->Position.X;
				float dy = m_currMousePos->Position.Y - m_prevMousePos->Position.Y;

				XMFLOAT4 pos = XMFLOAT4(m_camera[i]._41, m_camera[i]._42, m_camera[i]._43, m_camera[i]._44);

				m_camera[i]._41 = 0;
				m_camera[i]._42 = 0;
				m_camera[i]._43 = 0;

				XMMATRIX rotX = XMMatrixRotationX(dy * rotSpd * delta_time);
				XMMATRIX rotY = XMMatrixRotationY(dx * rotSpd * delta_time);

				XMMATRIX temp_camera = XMLoadFloat4x4(&m_camera[i]);
				temp_camera = XMMatrixMultiply(rotX, temp_camera);
				temp_camera = XMMatrixMultiply(temp_camera, rotY);

				XMStoreFloat4x4(&m_camera[i], temp_camera);

				m_camera[i]._41 = pos.x;
				m_camera[i]._42 = pos.y;
				m_camera[i]._43 = pos.z;
			}
			m_prevMousePos = m_currMousePos;
		}
	}

}

void Sample3DSceneRenderer::SetKeyboardButtons(const char* list)
{
	memcpy_s(m_kbuttons, sizeof(m_kbuttons), list, sizeof(m_kbuttons));
}

void Sample3DSceneRenderer::SetMousePosition(const Windows::UI::Input::PointerPoint^ pos)
{
	m_currMousePos = const_cast<Windows::UI::Input::PointerPoint^>(pos);
}

void Sample3DSceneRenderer::SetInputDeviceData(const char* kb, const Windows::UI::Input::PointerPoint^ pos)
{
	SetKeyboardButtons(kb);
	SetMousePosition(pos);
}

void DX11UWA::Sample3DSceneRenderer::StartTracking(void)
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking(void)
{
	m_tracking = false;
}


void Sample3DSceneRenderer::LightRotation(void)
{
	//dir light
	//XMStoreFloat4(&Dlightdata.direction, XMVector3Transform(XMLoadFloat4(&(Dlightdata.direction)), XMMatrixTranspose(XMMatrixRotationX(0.02f))));
	//spot light
	/*XMStoreFloat4(&Slightdata.pos, XMVector3Transform(XMLoadFloat4(&(Slightdata.pos)), XMMatrixTranspose(XMMatrixRotationZ(0.07f))));
	XMStoreFloat4(&Slightdata.ConeDir, XMVector3TransformNormal(XMLoadFloat4(&(Slightdata.ConeDir)), XMMatrixTranspose(XMMatrixRotationZ(0.07f))));*/
	//point
	XMStoreFloat4(&Plightdata.pos, XMVector3Transform(XMLoadFloat4(&(Plightdata.pos)), XMMatrixTranspose(XMMatrixRotationX(0.02f))));
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render(void)
{
	auto context = m_deviceResources->GetD3DDeviceContext();
Rendermult(0);
	//context->ClearState();

Rendermult(1);


}
void Sample3DSceneRenderer::Rendermult(int passedscreen)
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}


	auto context = m_deviceResources->GetD3DDeviceContext();
	context->RSSetViewports(1, &Screens[passedscreen]);
	
	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_camera[passedscreen]))));

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(floor_constantBufferDlight.Get(), 0, NULL, &Dlightdata, 0, 0, 0);
	context->UpdateSubresource1(floor_constantBufferSlight.Get(), 0, NULL, &Slightdata, 0, 0, 0);
	context->UpdateSubresource1(floor_constantBufferPlight.Get(), 0, NULL, &Plightdata, 0, 0, 0);

	////////////////////////////////////////////////////////////
	//skybox 
	////////////////////////////////////////////////////////////
#pragma region Skybox
	static float rot = 0.0f;
	rot += 0.01f;
	skybox_constantBufferData = m_constantBufferData;
	XMMATRIX skybox_position = XMMatrixTranslation(m_camera[passedscreen]._41, m_camera[passedscreen]._42, m_camera[passedscreen]._43);

	XMMATRIX rotationY = XMMatrixRotationY(XMConvertToRadians(rot));
	skybox_position = XMMatrixMultiply(rotationY, skybox_position);
	XMStoreFloat4x4(&skybox_constantBufferData.model, XMMatrixTranspose(skybox_position));
	context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &skybox_constantBufferData, 0, 0, 0);
	UINT stride = sizeof(VertexPositionUVNormal);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, m_vertexBuffer_new_Cube.GetAddressOf(), &stride, &offset);
	// Each index is one 16-bit unsigned integer (short).
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_inputLayout_PUN.Get());
	// Attach our vertex shader.
	context->VSSetShader(skybox_vertexShader.Get(), nullptr, 0);
	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	// Attach our pixel shader.
	context->PSSetShader(skybox_pixelShader.Get(), nullptr, 0);
	// create black sample state on other objects till they have there own resources
	context->PSSetShaderResources(0, 1, skybox_rsv.GetAddressOf());
	context->RSSetState(raster_state_ccw.Get());
	context->Draw(m_num_verts_cube, 0);
	context->RSSetState(raster_state_cw.Get());
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
#pragma endregion

	ID3D11ShaderResourceView* nullsrv = nullptr;
	context->PSSetShaderResources(0, 1, &nullsrv);
	/////////////////////////////////////////////////////////////////
	//cube///////////////////////////////
	////////////////////////////////////////////////////////////////
#pragma region Cube
	//context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &m_constantBufferData, 0, 0, 0);
	//// Each vertex is one instance of the VertexPositionColor struct.
	//stride = sizeof(VertexPositionUVNormal);
	//offset = 0;
	//context->IASetVertexBuffers(0, 1, m_vertexBuffer_new_Cube.GetAddressOf(), &stride, &offset);
	//// Each index is one 16-bit unsigned integer (short).
	//context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//context->IASetInputLayout(m_inputLayout_PUN.Get());
	//// Attach our vertex shader.
	//context->VSSetShader(m_vertexShader_PUN.Get(), nullptr, 0);
	//// Send the constant buffer to the graphics device.
	//context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	//// Attach our pixel shader.
	//context->PSSetConstantBuffers1(0, 1, floor_constantBufferDlight.GetAddressOf(), nullptr, nullptr);
	//context->PSSetConstantBuffers1(1, 1, floor_constantBufferPlight.GetAddressOf(), nullptr, nullptr);
	//context->PSSetConstantBuffers1(2, 1, floor_constantBufferSlight.GetAddressOf(), nullptr, nullptr);


	//context->PSSetShader(m_pixelShader_PUN.Get(), nullptr, 0);
	//// Draw the objects.
	//context->Draw(m_num_verts_cube, 0);
#pragma endregion
	///////////////////////////////////////////////////////////////////////
	// Creation for the floor
	////////////////////////
	////////Lights Setup////
	////////////////////////

	/////////////////////////////////////////
	//moves the cube///////
	////////////////////////////////////////
#pragma region floor
	floor_constantBufferData.view = m_constantBufferData.view;
	floor_constantBufferData.projection = m_constantBufferData.projection;
	XMStoreFloat4x4(&floor_constantBufferData.model, XMMatrixTranspose(XMMatrixTranslation(0.0f, -3.0f, 0.0f)));

	context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &floor_constantBufferData, 0, 0, 0);
	context->UpdateSubresource1(floor_constantBufferDlight.Get(), 0, NULL, &Dlightdata, 0, 0, 0);
	context->UpdateSubresource1(floor_constantBufferSlight.Get(), 0, NULL, &Slightdata, 0, 0, 0);
	context->UpdateSubresource1(floor_constantBufferPlight.Get(), 0, NULL, &Plightdata, 0, 0, 0);
	stride = sizeof(VertexPositionUVNormal);
	offset = 0;
	context->IASetVertexBuffers(0, 1, floor_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(floor_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_inputLayout_PUN.Get());
	context->VSSetShader(m_vertexShader_PUN.Get(), nullptr, 0);
	context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);
	context->PSSetConstantBuffers1(0, 1, floor_constantBufferDlight.GetAddressOf(), nullptr, nullptr);
	context->PSSetConstantBuffers1(1, 1, floor_constantBufferPlight.GetAddressOf(), nullptr, nullptr);
	context->PSSetConstantBuffers1(2, 1, floor_constantBufferSlight.GetAddressOf(), nullptr, nullptr);
	context->PSSetShader(m_pixelShader_PUN.Get(), nullptr, 0);
	context->DrawIndexed(6, 0, 0);
#pragma endregion

}

void Sample3DSceneRenderer::CreateDeviceDependentResources(void)
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");
	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &m_vertexShader));

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &m_inputLayout));
	});
	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &m_pixelShader));

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer));
	});

	// Add code to current
	auto loadVSTaskINT = DX::ReadDataAsync(L"INTVertexShader.cso");
	auto loadVSTaskPUN = DX::ReadDataAsync(L"PUN_VertexShader.cso");
	auto loadPSTaskPUN = DX::ReadDataAsync(L"PUN_PixelShader.cso");
	auto createVSTaskPUN = loadVSTaskPUN.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &m_vertexShader_PUN));

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &m_inputLayout_PUN));

	});
	auto createVSTaskINT = loadVSTaskINT.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &Asteroid_vertexShader));
	});
	auto createPSTaskPUN = loadPSTaskPUN.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &m_pixelShader_PUN));

	});

	auto loadNMVSTask = DX::ReadDataAsync(L"NormalMapping.cso");
	auto loadNMPSTask = DX::ReadDataAsync(L"NormMapPixelShader.cso");
	auto createNMVSTaskPUN = loadVSTaskINT.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &NM_vertexShader));

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &NM_inputLayout));

	});
	auto createNM2VSTaskPUN = loadNMVSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &Tomahawk_vertexShader));

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		};

		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &Tomahawk_inputLayout));

	});
	auto createNMPSTaskPUN = loadNMPSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &NM_pixelShader));
	});
	auto createNM2PSTaskPUN = loadNMPSTask.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &Tomahawk_pixelShader));
	});

	/////////////////////////////////////////////
	////Floor Loader////
	/////////////////////////////////////////////
	auto create_FloorTask = (createPSTaskPUN && createVSTaskPUN).then([this]()
	{
#pragma region Floor Verts
		// create the verts and indicies
		const VertexPositionUVNormal floor_vertices[4] =
		{
			{ XMFLOAT3(-12.0f, 0.0f,  12.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3( 12.0f, 0.0f,  12.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(-12.0f, 0.0f, -12.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3( 12.0f, 0.0f, -12.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }
		};

		XMFLOAT3 frontTL = XMFLOAT3(-1.0f,  1.0f, -1.0f);
		XMFLOAT3 frontTR = XMFLOAT3( 1.0f,  1.0f, -1.0f);
		XMFLOAT3 frontBL = XMFLOAT3(-1.0f, -1.0f, -1.0f);
		XMFLOAT3 frontBR = XMFLOAT3( 1.0f, -1.0f, -1.0f);

		XMFLOAT3 backTL = XMFLOAT3(-1.0f,  1.0f, 1.0f);
		XMFLOAT3 backTR = XMFLOAT3( 1.0f,  1.0f, 1.0f);
		XMFLOAT3 backBL = XMFLOAT3(-1.0f, -1.0f, 1.0f);
		XMFLOAT3 backBR = XMFLOAT3( 1.0f, -1.0f, 1.0f);

		XMFLOAT3 frontN = XMFLOAT3( 0.0f, 0.0f, -1.0f);
		XMFLOAT3 rightN = XMFLOAT3( 1.0f, 0.0f,  0.0f);
		XMFLOAT3 backN =  XMFLOAT3( 0.0f, 0.0f,  1.0f);
		XMFLOAT3 leftN =  XMFLOAT3(-1.0f, 0.0f,  0.0f);
		XMFLOAT3 topN =  XMFLOAT3( 0.0f,  1.0f,  0.0f);
		XMFLOAT3 botN =  XMFLOAT3( 0.0f, -1.0f,  0.0f);

		XMFLOAT3 uvTL = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 uvTR = XMFLOAT3(1.0f, 0.0f, 0.0f);
		XMFLOAT3 uvBL = XMFLOAT3(0.0f, 1.0f, 0.0f);
		XMFLOAT3 uvBR = XMFLOAT3(1.0f, 1.0f, 0.0f);

		const VertexPositionUVNormal new_cube_verts[] =
		{
			{ frontTL, uvTL, frontN },	// 0
			{ frontTR, uvTR, frontN },	// 1
			{ frontBL, uvBL, frontN },	// 2

			{ frontTR, uvTR, frontN },	// 1
			{ frontBR, uvBR, frontN },	// 3
			{ frontBL, uvBL, frontN },	// 2

			{ frontTR, uvTL, rightN },	// 4
			{ backTR, uvTR, rightN },	// 5
			{ frontBR, uvBL, rightN },	// 6

			{ backTR, uvTR, rightN },	// 5
			{ backBR, uvBR, rightN },	// 7
			{ frontBR, uvBL, rightN },	// 6

			{ backTR, uvTL, backN },	// 8
			{ backTL, uvTR, backN },	// 9
			{ backBR, uvBL, backN },	// 10

			{ backTL, uvTR, backN },	// 9
			{ backBL, uvBR, backN },	// 11
			{ backBR, uvBL, backN },	// 10

			{ backTL, uvTL, leftN },	// 12
			{ frontTL, uvTR, leftN },	// 13
			{ backBL, uvBL, leftN },	// 14


			{ frontTL, uvTR, leftN },	// 13
			{ frontBL, uvBR, leftN },	// 15
			{ backBL, uvBL, leftN },	// 14

			{ backTL, uvTL, topN },    //17
			{ backTR, uvTR, topN },    //19
			{ frontTL, uvBL, topN },   //18

			{ backTR, uvTR, topN },   //19
			{ frontTR, uvBR, topN },  //20
			{ frontTL, uvBL, topN },  //18

			{frontBL,uvTL,botN},     //21
			{frontBR,uvTR,botN},     //23
		    { backBL,uvBL,botN},     //22


			 { frontBR,uvTR,botN },  //23
			 { backBR,uvBR,botN },   //24
			 { backBL,uvBL,botN },    //22

		};
		m_num_verts_cube = ARRAYSIZE(new_cube_verts);

#pragma endregion

		const UINT floor_indicies[] =
		{
			0,1,2,
			1,3,2
		};
		floor_indexCount = ARRAYSIZE(floor_indicies);
#pragma region Floor buffers
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = floor_vertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(floor_vertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &floor_vertexBuffer));

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = floor_indicies;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(floor_indicies), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &floor_indexBuffer));

		D3D11_SUBRESOURCE_DATA vertexBufferData2 = { 0 };
		vertexBufferData2.pSysMem = new_cube_verts;
		vertexBufferData2.SysMemPitch = 0;
		vertexBufferData2.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc2(sizeof(new_cube_verts), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc2, &vertexBufferData2, &m_vertexBuffer_new_Cube));
#pragma endregion

	});
	/////////////////////////////////////////////
	////Asteroid Loader////
	/////////////////////////////////////////////
#pragma region Create Asteroid
	auto create_AsteroidTask = (createNMVSTaskPUN && createPSTaskPUN).then([this]()
	{
		vector<VertexPositionUVNormalMap> mod_vertBuffer;
		vector<UINT> mod_IndexBuffer;
		/////////////////////////////////////////////
		////Asteroid Loader////
		/////////////////////////////////////////////
		if (Load("Assets/asteroid.obj", mod_vertBuffer, mod_IndexBuffer))
		{
			Asteroid_indexCount = mod_IndexBuffer.size();


			D3D11_SUBRESOURCE_DATA Asteroid_indexBufferData = { 0 };
			Asteroid_indexBufferData.pSysMem = mod_IndexBuffer.data();
			Asteroid_indexBufferData.SysMemPitch = 0;
			Asteroid_indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC Asteroid_indexBufferDesc(sizeof(unsigned int)*mod_IndexBuffer.size(), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&Asteroid_indexBufferDesc, &Asteroid_indexBufferData, &Asteroid_indexBuffer));

			D3D11_SUBRESOURCE_DATA Asteroid_vertexBufferData = { 0 };
			Asteroid_vertexBufferData.pSysMem = mod_vertBuffer.data();
			Asteroid_vertexBufferData.SysMemPitch = 0;
			Asteroid_vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC Asteroid_vertexBufferDesc(sizeof(VertexPositionUVNormalMap)*mod_vertBuffer.size(), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&Asteroid_vertexBufferDesc, &Asteroid_vertexBufferData, &Asteroid_vertexBuffer));

			CD3D11_BUFFER_DESC constantBufferDesc(sizeof(Asteroid_arraymodel), D3D11_BIND_CONSTANT_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &Asteroid_constantBuffer));

			DX::ThrowIfFailed(CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/rock_NRM.dds", nullptr, SDF1_rock_rsv.GetAddressOf()));
			DX::ThrowIfFailed(CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/rock_COLOR.dds", nullptr, asteriod_rock_rsv.GetAddressOf()));
		}
		mod_IndexBuffer.clear();
		mod_vertBuffer.clear();
	});
#pragma endregion
	///////////////////////////////////////////////////
	////SDF1 Loader////////////////
	//////////////////////////////////////////////////
#pragma region Create SDF1
	auto create_SDF1Task = (createPSTaskPUN && createVSTaskPUN).then([this]()
	{
		vector<VertexPositionUVNormal> mod_vertBuffer;
		vector<UINT> mod_IndexBuffer;
		if (Load("Assets/SDF1.obj", mod_vertBuffer, mod_IndexBuffer))
		{
			SDF1_indexCount = mod_IndexBuffer.size();


			D3D11_SUBRESOURCE_DATA SDF1_indexBufferData = { 0 };
			SDF1_indexBufferData.pSysMem = mod_IndexBuffer.data();
			SDF1_indexBufferData.SysMemPitch = 0;
			SDF1_indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC SDF1_indexBufferDesc(sizeof(unsigned int)*mod_IndexBuffer.size(), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&SDF1_indexBufferDesc, &SDF1_indexBufferData, &SDF1_indexBuffer));

			D3D11_SUBRESOURCE_DATA SDF1_vertexBufferData = { 0 };
			SDF1_vertexBufferData.pSysMem = mod_vertBuffer.data();
			SDF1_vertexBufferData.SysMemPitch = 0;
			SDF1_vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC SDF1_vertexBufferDesc(sizeof(Vertex)*mod_vertBuffer.size(), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&SDF1_vertexBufferDesc, &SDF1_vertexBufferData, &SDF1_vertexBuffer));

			
		}
		mod_IndexBuffer.clear();
		mod_vertBuffer.clear();
	});
#pragma endregion
	/////////////////////////////////////////////////
	/////GCrusier////////////////
	////////////////////////////////////////////////
#pragma region Create GallacticCruiser
	auto create_GallacticCruiserTask = (createPSTaskPUN && createVSTaskPUN).then([this]()
	{
		vector<VertexPositionUVNormal> mod_vertBuffer;
		vector<UINT> mod_IndexBuffer;
		if (Load("Assets/GallacticCruiser.obj", mod_vertBuffer, mod_IndexBuffer))
		{
			GCruiser_indexCount = mod_IndexBuffer.size();


			D3D11_SUBRESOURCE_DATA GCruiser_indexBufferData = { 0 };
			GCruiser_indexBufferData.pSysMem = mod_IndexBuffer.data();
			GCruiser_indexBufferData.SysMemPitch = 0;
			GCruiser_indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC GCruiser_indexBufferDesc(sizeof(unsigned int)*mod_IndexBuffer.size(), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&GCruiser_indexBufferDesc, &GCruiser_indexBufferData, &GCruiser_indexBuffer));

			D3D11_SUBRESOURCE_DATA GCruiser_vertexBufferData = { 0 };
			GCruiser_vertexBufferData.pSysMem = mod_vertBuffer.data();
			GCruiser_vertexBufferData.SysMemPitch = 0;
			GCruiser_vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC GCruiser_vertexBufferDesc(sizeof(Vertex)*mod_vertBuffer.size(), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&GCruiser_vertexBufferDesc, &GCruiser_vertexBufferData, &GCruiser_vertexBuffer));

			DX::ThrowIfFailed(CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/vba1b.dds", nullptr, GCruiser_rock_rsv.GetAddressOf()));
		}
		mod_IndexBuffer.clear();
		mod_vertBuffer.clear();
	});
#pragma endregion
	////////////
	////tomahawk/////
	////////
#pragma region Create mech

		vector<VertexPositionUVNormalMap> mod_vertBuffer;
		vector<vector<UINT>> mod_IndexBuffer;
		UINT t_count=1;
		if (LoadMulti("Assets/tomahawk2.obj",mod_vertBuffer, mod_IndexBuffer, Num0fMeshes))
		{
			for (UINT i = 0; i < mod_IndexBuffer.size(); i++)
			{
				t_count += mod_IndexBuffer[i].size();
			}
			Tomahawk_indexCount = t_count;
			Num0fMeshes = mod_IndexBuffer.size();
			Tomahawk_indexBuffer.resize(Num0fMeshes);
			Tomahawk_rsv.resize(Num0fMeshes);

			for (UINT i = 0; i < Num0fMeshes; i++)
			{

			D3D11_SUBRESOURCE_DATA Tomahawk_indexBufferData = { 0 };
			Tomahawk_indexBufferData.pSysMem = mod_IndexBuffer[i].data();
			Tomahawk_indexBufferData.SysMemPitch = 0;
			Tomahawk_indexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC Tomahawk_indexBufferDesc(sizeof(unsigned int)*mod_IndexBuffer[i].size(), D3D11_BIND_INDEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&Tomahawk_indexBufferDesc, &Tomahawk_indexBufferData, &Tomahawk_indexBuffer[i]));

			}
			D3D11_SUBRESOURCE_DATA Tomahawk_vertexBufferData = { 0 };
			Tomahawk_vertexBufferData.pSysMem = mod_vertBuffer.data();
			Tomahawk_vertexBufferData.SysMemPitch = 0;
			Tomahawk_vertexBufferData.SysMemSlicePitch = 0;
			CD3D11_BUFFER_DESC Tomahawk_vertexBufferDesc(sizeof(Vertex)*mod_vertBuffer.size(), D3D11_BIND_VERTEX_BUFFER);
			DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&Tomahawk_vertexBufferDesc, &Tomahawk_vertexBufferData, &Tomahawk_vertexBuffer));


			DX::ThrowIfFailed(CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/tomahawk_base.dds", nullptr, Tomahawk_rsv[0].GetAddressOf()));
		}
		mod_IndexBuffer.clear();
		mod_vertBuffer.clear();
	
	/////////////////////
	//teture loader////
	/////////////////////
	CD3D11_SAMPLER_DESC anisoSamplerDesc(D3D11_FILTER_ANISOTROPIC,D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,0,1,D3D11_COMPARISON_NEVER,nullptr,-FLT_MAX,FLT_MAX);
	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateSamplerState(&anisoSamplerDesc,sampler_state.GetAddressOf()));

#pragma region SkyBox Desc
	/////////////////
	///SkyBox///////
	////////////////
	auto loadVSTaskSB = DX::ReadDataAsync(L"SB_VertexShader.cso");
	auto loadPSTaskSB = DX::ReadDataAsync(L"SB_PixelShader.cso");
	auto createVSTaskSB = loadVSTaskSB.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &skybox_vertexShader));
	});
	auto createPSTaskSB = loadPSTaskSB.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &skybox_pixelShader));
	});
	auto create_SkyBoxTask = (createVSTaskSB && createPSTaskSB).then([this]()
	{
    //CD3D11_RASTERIZER_DESC raster1 = CD3D11_RASTERIZER_DESC(CD3D11_DEFAULT());
	CD3D11_RASTERIZER_DESC raster_cw_desc(D3D11_FILL_SOLID,D3D11_CULL_BACK,FALSE,D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,TRUE,FALSE,FALSE,FALSE);
	CD3D11_RASTERIZER_DESC raster_ccw_desc(D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE, D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP, D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, FALSE);
	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateRasterizerState(&raster_cw_desc, raster_state_cw.GetAddressOf()));
	DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateRasterizerState(&raster_ccw_desc, raster_state_ccw.GetAddressOf()));

	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"Assets/Skyboxtexture.dds", nullptr, skybox_rsv.GetAddressOf()));

	});
	

#pragma endregion

#pragma region GeoShader Creation
	auto loadGVSTaskGS = DX::ReadDataAsync(L"FlagGeometryShader.cso");
	auto loadGSTaskGS = DX::ReadDataAsync(L"GS_VertexShader.cso");
	auto createGVSTaskGS = loadGVSTaskGS.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateGeometryShader(&fileData[0],fileData.size(), nullptr,&GS_GeoShader));
	});
	auto createGSTaskGS = loadGSTaskGS.then([this](const std::vector<byte>& fileData)
	{
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &GS_vertShader));
	});
	auto create_GeoShaderTask = (createGVSTaskGS && createGSTaskGS).then([this]()
	{
		const VertexPositionUVNormal quad_vertices[] =
		{
			{ XMFLOAT3(-2.0f, 2.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
		};
		m_que_verts = ARRAYSIZE(quad_vertices);
		D3D11_SUBRESOURCE_DATA quadBufferData = { 0 };
		quadBufferData.pSysMem = quad_vertices;
		quadBufferData.SysMemPitch = 0;
		quadBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC GeoBufferDesc(sizeof(quad_vertices), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&GeoBufferDesc, &quadBufferData, &GS_vertexBuffer));

	});
#pragma endregion
#pragma region Lights Setup
	auto create_Lighting = (createPSTaskPUN && createVSTaskPUN).then([this]() {
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(Dlightdata), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &floor_constantBufferDlight));
		CD3D11_BUFFER_DESC constantBufferDesc_Plight(sizeof(Plightdata), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc_Plight, nullptr, &floor_constantBufferPlight));
		CD3D11_BUFFER_DESC constantBufferDesc_Slight(sizeof(Slightdata), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc_Slight, nullptr, &floor_constantBufferSlight));

		Dlightdata.color = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
		Dlightdata.direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
		//point light creation light on rightside of cube
		Plightdata.color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
		Plightdata.pos = XMFLOAT4(0.0, 10.0, -1.0f, 0.0f);
		Plightdata.rad = XMFLOAT4(2.0f, 0.0f, 0.0f, 0.0f);
		//Spot light creation
		Slightdata.color = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
		Slightdata.innerCon = XMFLOAT4(0.9f, 0.0f, 0.0f, 0.0f);
		Slightdata.outerCon = XMFLOAT4(0.8f, 0.0f, 0.0f, 0.0f);
		Slightdata.light_rad = XMFLOAT4(5.5f, 0.0f, 0.0f, 0.0f);
		Slightdata.pos = XMFLOAT4(2.0f, 1.5f, 0.0f, 0.0f);
		Slightdata.ConeDir = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);

	});
#pragma endregion

	// Once the cube is loaded, the object is ready to be rendered.
	create_FloorTask.then([this]()
	{
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources(void)
{
	//Cube
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}