#pragma once

#include "..\Common\DeviceResources.h"
#include "ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "Common\DDSTextureLoader.h"


namespace DX11UWA
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer
	{
	public:
		Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources(void);
		void CreateWindowSizeDependentResources(void);
		void ReleaseDeviceDependentResources(void);
		void Update(DX::StepTimer const& timer);
		void Render(void);
		void StartTracking(void);
		void TrackingUpdate(float positionX);
		void StopTracking(void);
		inline bool IsTracking(void) { return m_tracking; }

		// Helper functions for keyboard and mouse input
		void SetKeyboardButtons(const char* list);
		void SetMousePosition(const Windows::UI::Input::PointerPoint^ pos);
		void SetInputDeviceData(const char* kb, const Windows::UI::Input::PointerPoint^ pos);


	private:
		void Rotate(float radians);
		void UpdateCamera(DX::StepTimer const& timer, float const moveSpd, float const rotSpd);
		void LightRotation(void);
	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_constantBuffer;

		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// Direct3D resources for floor geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout_PUN;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader_PUN;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader_PUN;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_constantBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_constantBufferDlight;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_constantBufferPlight;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		floor_constantBufferSlight;
		Dirlight                                    Dlightdata;
		pointlight                                  Plightdata;
		Spot                                        Slightdata;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer_new_Cube;
		UINT										m_num_verts_cube;
		// System resources for floor geometry.
		ModelViewProjectionConstantBuffer	floor_constantBufferData;
		uint32	floor_indexCount;
		/////////////////////
		//Model loading
		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	SDF1_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		SDF1_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		SDF1_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	SDF1_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	SDF1_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		SDF1_constantBuffer;
		// System resources for cube geometry.
		ModelViewProjectionConstantBuffer	SDF1_constantBufferData;
		uint32	SDF1_indexCount;
		/////
		//Asteroid 
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	Asteroid_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		Asteroid_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		Asteroid_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	Asteroid_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	Asteroid_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		Asteroid_constantBuffer;
		// System resources for cube geometry.		
		INSTANCED_MATRIX                            Asteroid_arraymodel;
		ModelViewProjectionConstantBuffer	        Asteroid_constantBufferData;
		uint32	                                    Asteroid_indexCount;
		uint32                                        modelin;
		////////
		//////Gallactic Cruiser
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	GCruiser_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		GCruiser_vertexBuffer;
		//Microsoft::WRL::ComPtr<ID3D11Buffer*>		GCruiser_vertexBuffer2;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		GCruiser_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	GCruiser_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	GCruiser_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		GCruiser_constantBuffer;
		// System resources for cube geometry.		
		INSTANCED_MATRIX                            GCruiser_arraymodel;
		ModelViewProjectionConstantBuffer	        GCruiser_constantBufferData;
		uint32	                                    GCruiser_indexCount;
		//vector<ID3D11Buffer*>                       Gcruiser_indexBuffer2;
		///////////////////////////
		//texture creation
		///////////////////////////
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GCruiser_rock_rsv;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SDF1_rock_rsv;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>       sampler_state;

		/////////////////////////////////////////////
		//creating the skybox
		/////////////////////////////////////////////
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skybox_rsv;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	     skybox_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	     skybox_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> raster_state_cw;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> raster_state_ccw;

		ModelViewProjectionConstantBuffer	skybox_constantBufferData;
		/////////////
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	     GS_inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		     GS_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		     GS_indexBuffer;
		
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	 GS_GeoShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	     GS_vertShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	     GS_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		     GS_constantBuffer;
		ModelViewProjectionConstantBuffer	             GS_constantBufferData;
		uint32                                       	 GS_indexCount;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> GS_state_cw;
		UINT										m_que_verts;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;
		bool	floor_loadingComplete;
		float	floor_degreesPerSecond;
		bool	floor_tracking;
		// Data members for keyboard and mouse input
		char	m_kbuttons[256];
		Windows::UI::Input::PointerPoint^ m_currMousePos;
		Windows::UI::Input::PointerPoint^ m_prevMousePos;

		// Matrix data member for the camera
		DirectX::XMFLOAT4X4 m_camera;
	};
}

