#pragma once
//#define _XM_NO_INTRINSICS_
#include <DirectXMath.h>
#include "Content\ShaderStructures.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace DirectX;
using namespace std;
using namespace DX11UWA;
struct Model
{
	vector<unsigned int> vertexIndices, normalIndices, uvIndices;
	vector<XMFLOAT3> temp_vert, temp_normals;
	vector<XMFLOAT3>temp_uvs;
};
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 uv;
	XMFLOAT3 norm;
};
struct MultiModel
{
	vector<vector<unsigned int>> vertexIndices, normalIndices, uvIndices;
	vector<XMFLOAT3> temp_vert, temp_normals;
	vector<XMFLOAT3>temp_uvs;
};

bool Load(const char* fname, vector<VertexPositionUVNormal> &Vertexs, vector<unsigned int> & Indexs)
{
	Model hold;
	//fstream model;
	FILE * model_file;
	fopen_s(&model_file,fname, "r");
	
	if (model_file == NULL)
	{
		printf("can not open File !\n");
		return false;
	}
	while (1)
	{
		char line[128];
		int RES = fscanf_s(model_file, "%s", line,128);
		if (RES == EOF)
		{
			break;
		}
		if (strcmp(line,"v")==0)
		{
			XMFLOAT3 verts;
			fscanf_s(model_file, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			hold.temp_vert.push_back(verts);
			
		}
		else if (strcmp(line, "vt") == 0)
		{
			XMFLOAT3 uv;
			fscanf_s(model_file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			hold.temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			XMFLOAT3 norms;
			fscanf_s(model_file, "%f %f %f\n", &norms.x, &norms.y,&norms.z);
			norms.x*=-1.0f;
			hold.temp_normals.push_back(norms);
		}
		else if (strcmp(line, "f") == 0)
		{
			string vert1, vert2, vert3;
			unsigned int vertindex[3], uvindex[3], normalindex[3];
			int match = fscanf_s(model_file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertindex[0], &uvindex[0], &normalindex[0],
				&vertindex[1], &uvindex[1], &normalindex[1],
				&vertindex[2], &uvindex[2], &normalindex[2]);
			if (match != 9)
			{
				printf("file can not be read");
				return false;
			}
			hold.vertexIndices.push_back(vertindex[0]);
			hold.vertexIndices.push_back(vertindex[1]);
			hold.vertexIndices.push_back(vertindex[2]);
			
			hold.uvIndices.push_back(uvindex[0]);
			hold.uvIndices.push_back(uvindex[1]);
			hold.uvIndices.push_back(uvindex[2]);
			

			hold.normalIndices.push_back(normalindex[0]);
			hold.normalIndices.push_back(normalindex[1]);
			hold.normalIndices.push_back(normalindex[2]);
			

		}
	}
	for (unsigned int i = 0; i < hold.vertexIndices.size(); i++)
	{
		VertexPositionUVNormal t;
		t.pos = hold.temp_vert[hold.vertexIndices[i] - 1];
		t.uv = hold.temp_uvs[hold.uvIndices[i] - 1];
		t.normal = hold.temp_normals[hold.normalIndices[i] - 1];
		Vertexs.push_back(t);
		Indexs.push_back(i);
	}
	fclose(model_file);
	return true;
}


//Overloaded for NormalMapping
bool Load(const char* fname, vector<VertexPositionUVNormalMap> &Vertexs, vector<unsigned int> & Indexs)
{
	Model hold;
	//fstream model;
	FILE * model_file;
	fopen_s(&model_file, fname, "r");

	if (model_file == NULL)
	{
		printf("can not open File !\n");
		return false;
	}
	while (1)
	{
		char line[128];
		int RES = fscanf_s(model_file, "%s", line, 128);
		if (RES == EOF)
		{
			break;
		}
		if (strcmp(line, "v") == 0)
		{
			XMFLOAT3 verts;
			fscanf_s(model_file, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			hold.temp_vert.push_back(verts);

		}
		else if (strcmp(line, "vt") == 0)
		{
			XMFLOAT3 uv;
			fscanf_s(model_file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			hold.temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			XMFLOAT3 norms;
			fscanf_s(model_file, "%f %f %f\n", &norms.x, &norms.y, &norms.z);
			norms.x *= -1.0f;
			hold.temp_normals.push_back(norms);
		}
		else if (strcmp(line, "f") == 0)
		{
			string vert1, vert2, vert3;
			unsigned int vertindex[3], uvindex[3], normalindex[3];
			int match = fscanf_s(model_file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertindex[0], &uvindex[0], &normalindex[0],
				&vertindex[1], &uvindex[1], &normalindex[1],
				&vertindex[2], &uvindex[2], &normalindex[2]);
			if (match != 9)
			{
				printf("file can not be read");
				return false;
			}
			hold.vertexIndices.push_back(vertindex[0]);
			hold.vertexIndices.push_back(vertindex[2]);
			hold.vertexIndices.push_back(vertindex[1]);

			hold.uvIndices.push_back(uvindex[0]);
			hold.uvIndices.push_back(uvindex[2]);
			hold.uvIndices.push_back(uvindex[1]);

			hold.normalIndices.push_back(normalindex[0]);
			hold.normalIndices.push_back(normalindex[2]);
			hold.normalIndices.push_back(normalindex[1]);
		}
	}
		for (unsigned int i = 0; i < hold.vertexIndices.size(); i++)
		{
			VertexPositionUVNormalMap t;
			t.pos = hold.temp_vert[hold.vertexIndices[i] - 1];
			t.uv = hold.temp_uvs[hold.uvIndices[i] - 1];
			t.normal = hold.temp_normals[hold.normalIndices[i] - 1];
			//t.tangent = XMFLOAT4(rand() , rand(), rand(), rand());
			Vertexs.push_back(t);
			Indexs.push_back(i);

		}
//#pragma region cut
		for (size_t i = 0; i < Indexs.size(); i+=3)
		{
			VertexPositionUVNormalMap &t = Vertexs[Indexs[i]];
			VertexPositionUVNormalMap &t1 = Vertexs[Indexs[i + 1]];
			VertexPositionUVNormalMap &t2 = Vertexs[Indexs[i + 2]];
			
				XMVECTOR vertEdge0, vertEdge1, uvEdge0, uvEdge1;
				vertEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.pos), XMLoadFloat3(&t.pos));
				vertEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.pos), XMLoadFloat3(&t.pos));
				uvEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.uv), XMLoadFloat3(&t.uv));
				uvEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.uv), XMLoadFloat3(&t.uv));

				float uv_x0 =      XMVectorGetX(uvEdge0);
				float uv_y0 =      XMVectorGetY(uvEdge0);
				float uv_z0 =      XMVectorGetZ(uvEdge0);
				float uv_x1 =      XMVectorGetX(uvEdge1);
				float uv_y1 =      XMVectorGetY(uvEdge1);
				float uv_z1 =      XMVectorGetZ(uvEdge1);
				float vert_x0 =  XMVectorGetX(vertEdge0);
				float vert_y0 =  XMVectorGetY(vertEdge0);
				float vert_z0 =  XMVectorGetZ(vertEdge0);
				float vert_x1 =  XMVectorGetX(vertEdge1);
				float vert_y1 =  XMVectorGetY(vertEdge1);
				float vert_z1 = XMVectorGetZ(vertEdge1);


				float ratio = 1.0f / (uv_x0* uv_y1 - uv_x1*uv_y0);

				XMFLOAT3A uDirection = (XMFLOAT3A((uv_y1*vert_x0)-  (uv_y0*vert_x1)*ratio,
					                           (uv_y1*vert_y0) - (uv_y0*vert_y1)*ratio,
					                           (uv_y1*vert_z0) - (uv_y0*vert_z1)*ratio));
				XMStoreFloat3(&uDirection, XMVector3Normalize(XMLoadFloat3(&uDirection)));
				
				XMVECTOR dotResult = XMVector3Dot(XMLoadFloat3(&t.normal), XMLoadFloat3(&uDirection));
				XMStoreFloat4(&t.tangent,XMVectorSubtract(XMLoadFloat3(&uDirection),XMVectorMultiply(XMLoadFloat3(&t.normal), dotResult)));
				XMStoreFloat4(&t.tangent, XMVector3Normalize(XMLoadFloat4(&t.tangent)));

				dotResult = XMVector3Dot(XMLoadFloat3(&t1.normal), XMLoadFloat3(&uDirection));
				XMStoreFloat4(&t1.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t1.normal), dotResult)));
				XMStoreFloat4(&t1.tangent, XMVector3Normalize(XMLoadFloat4(&t1.tangent)));

				dotResult = XMVector3Dot(XMLoadFloat3(&t2.normal), XMLoadFloat3(&uDirection));
				XMStoreFloat4(&t2.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t2.normal), dotResult)));
				XMStoreFloat4(&t2.tangent, XMVector3Normalize(XMLoadFloat4(&t2.tangent)));


		}

	
#pragma endregion
	fclose(model_file);
	return true;
}

///overload for Multitexturing
bool LoadMulti(const char* fname, vector<VertexPositionUVNormalMap> &Vertexs, vector<vector<unsigned int>> & Indexs, unsigned int& NumofMeshes)
{
	MultiModel hold;
	unsigned int NumofG=0,curr=0;
	//fstream model;
	FILE * model_file;
	fopen_s(&model_file, fname, "r");

	if (model_file == NULL)
	{
		printf("can not open File !\n");
		return false;
	}
	while (1)
	{
		char line[128];
		int RES = fscanf_s(model_file, "%s", line, 128);
		if (RES == EOF)
		{
			break;
		}
		if (strcmp(line, "v") == 0)
		{
			XMFLOAT3 verts;
			fscanf_s(model_file, "%f %f %f\n", &verts.x, &verts.y, &verts.z);
			hold.temp_vert.push_back(verts);

		}
		else if (strcmp(line, "vt") == 0)
		{
			XMFLOAT3 uv;
			fscanf_s(model_file, "%f %f\n", &uv.x, &uv.y);
			uv.y = 1 - uv.y;
			hold.temp_uvs.push_back(uv);
		}
		else if (strcmp(line, "vn") == 0)
		{
			XMFLOAT3 norms;
			fscanf_s(model_file, "%f %f %f\n", &norms.x, &norms.y, &norms.z);
			norms.x *= -1.0f;
			hold.temp_normals.push_back(norms);
		}
		else if (strcmp(line, "f") == 0)
		{
			string vert1, vert2, vert3;
			unsigned int vertindex[3], uvindex[3], normalindex[3];
			int match = fscanf_s(model_file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertindex[0], &uvindex[0], &normalindex[0],
				&vertindex[1], &uvindex[1], &normalindex[1],
				&vertindex[2], &uvindex[2], &normalindex[2]);
			if (match != 9)
			{
				printf("file can not be read");
				return false;
			}
			hold.vertexIndices[NumofG-1].push_back(vertindex[0]);
			hold.vertexIndices[NumofG-1].push_back(vertindex[2]);
			hold.vertexIndices[NumofG-1].push_back(vertindex[1]);

			hold.uvIndices[NumofG-1].push_back(uvindex[0]);
			hold.uvIndices[NumofG-1].push_back(uvindex[2]);
			hold.uvIndices[NumofG-1].push_back(uvindex[1]);

			hold.normalIndices[NumofG-1].push_back(normalindex[0]);
			hold.normalIndices[NumofG-1].push_back(normalindex[2]);
			hold.normalIndices[NumofG-1].push_back(normalindex[1]);
		}
	// Dealing with multi mesh texturing
	else if (strcmp(line, "g") == 0)
	{
		NumofG += 1;
		hold.vertexIndices.resize(NumofG);
		hold.uvIndices.resize(NumofG);
		hold.normalIndices.resize(NumofG);
		Indexs.resize(NumofG);
	}
	}

	for (int j = 0; j < NumofG; ++j)
	{
		for (unsigned int i = 0; i < hold.vertexIndices[j].size(); i++)
		{
			VertexPositionUVNormalMap t;
			t.pos = hold.temp_vert[hold.vertexIndices[j][i] - 1];
			t.uv = hold.temp_uvs[hold.uvIndices[j][i] - 1];
			t.normal = hold.temp_normals[hold.normalIndices[j][i] - 1];
			t.tangent = XMFLOAT4(rand(), rand(), rand(), rand());
			Vertexs.push_back(t);
			Indexs[j].push_back(Vertexs.size()-1);

		}
	}
#pragma region cut
	//for (size_t i = 0; i < Indexs.size(); i+=3)
	//{
	//	VertexPositionUVNormalMap &t = Vertexs[Indexs[i]];
	//	VertexPositionUVNormalMap &t1 = Vertexs[Indexs[i + 1]];
	//	VertexPositionUVNormalMap &t2 = Vertexs[Indexs[i + 2]];
	//	
	//		XMVECTOR vertEdge0, vertEdge1, uvEdge0, uvEdge1;
	//		vertEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.pos), XMLoadFloat3(&t.pos));
	//		vertEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.pos), XMLoadFloat3(&t.pos));
	//		uvEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.uv), XMLoadFloat3(&t.uv));
	//		uvEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.uv), XMLoadFloat3(&t.uv));

	//		float uv_x0 =      XMVectorGetX(uvEdge0);
	//		float uv_y0 =      XMVectorGetY(uvEdge0);
	//		float uv_z0 =      XMVectorGetZ(uvEdge0);
	//		float uv_x1 =      XMVectorGetX(uvEdge1);
	//		float uv_y1 =      XMVectorGetY(uvEdge1);
	//		float uv_z1 =      XMVectorGetZ(uvEdge1);
	//		float vert_x0 =  XMVectorGetX(vertEdge0);
	//		float vert_y0 =  XMVectorGetY(vertEdge0);
	//		float vert_z0 =  XMVectorGetZ(vertEdge0);
	//		float vert_x1 =  XMVectorGetX(vertEdge1);
	//		float vert_y1 =  XMVectorGetY(vertEdge1);
	//		float vert_z1 = XMVectorGetZ(vertEdge1);


	//		float ratio = 1.0f / (uv_x0* uv_y1 - uv_x1*uv_y0);

	//		XMFLOAT3 uDirection = (XMFLOAT3((uv_y1*vert_x0)-  (uv_y0*vert_x1)*ratio,
	//			                           (uv_y1*vert_y0) - (uv_y0*vert_y1)*ratio,
	//			                           (uv_y1*vert_z0) - (uv_y0*vert_z1)*ratio));
	//		XMStoreFloat3(&uDirection, XMVector3Normalize(XMLoadFloat3(&uDirection)));

	//		XMVECTOR dotResult = XMVector3Dot(XMLoadFloat3(&t.normal), XMLoadFloat3(&uDirection));
	//		XMStoreFloat4(&t.tangent,XMVectorSubtract(XMLoadFloat3(&uDirection),XMVectorMultiply(XMLoadFloat3(&t.normal), dotResult)));
	//		XMStoreFloat4(&t.tangent, XMVector3Normalize(XMLoadFloat4(&t.tangent)));

	//		dotResult = XMVector3Dot(XMLoadFloat3(&t1.normal), XMLoadFloat3(&uDirection));
	//		XMStoreFloat4(&t1.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t1.normal), dotResult)));
	//		XMStoreFloat4(&t1.tangent, XMVector3Normalize(XMLoadFloat4(&t1.tangent)));

	//		dotResult = XMVector3Dot(XMLoadFloat3(&t2.normal), XMLoadFloat3(&uDirection));
	//		XMStoreFloat4(&t2.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t2.normal), dotResult)));
	//		XMStoreFloat4(&t2.tangent, XMVector3Normalize(XMLoadFloat4(&t2.tangent)));


	//}

	//for (unsigned int i = 0; i < hold.vertexIndices.size(); i+=3)
	//{
	//	VertexPositionUVNormalMap t,t1,t2;
	//	ZeroMemory(&t, sizeof(VertexPositionUVNormalMap));
	//	t.pos = hold.temp_vert[hold.vertexIndices[i] - 1];		
	//	t1.pos = hold.temp_vert[hold.vertexIndices[i + 1] - 1];
	//	t2.pos = hold.temp_vert[hold.vertexIndices[i + 2] - 1];
	//	t.uv = hold.temp_uvs[hold.uvIndices[i] - 1];
	//	t1.uv = hold.temp_uvs[hold.uvIndices[i+1] - 1];
	//	t2.uv = hold.temp_uvs[hold.uvIndices[i+2] - 1];

	//	t.normal = hold.temp_normals[hold.normalIndices[i] - 1];
	//	t1.normal = hold.temp_normals[hold.normalIndices[i+1] - 1];
	//	t2.normal = hold.temp_normals[hold.normalIndices[i+2] - 1];

	//	XMVECTOR vertEdge0, vertEdge1, uvEdge0, uvEdge1;
	//	vertEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.pos), XMLoadFloat3(&t.pos));
	//	vertEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.pos), XMLoadFloat3(&t.pos));
	//	uvEdge0 = XMVectorSubtract(XMLoadFloat3(&t1.uv), XMLoadFloat3(&t.uv));
	//	uvEdge1 = XMVectorSubtract(XMLoadFloat3(&t2.uv), XMLoadFloat3(&t.uv));

	//	float uv_x0 =      XMVectorGetX(uvEdge0);
	//	float uv_y0 =      XMVectorGetY(uvEdge0);
	//	float uv_z0 =      XMVectorGetZ(uvEdge0);
	//	float uv_x1 =      XMVectorGetX(uvEdge1);
	//	float uv_y1 =      XMVectorGetY(uvEdge1);
	//	float uv_z1 =      XMVectorGetZ(uvEdge1);
	//	float vert_x0 =  XMVectorGetX(vertEdge0);
	//	float vert_y0 =  XMVectorGetY(vertEdge0);
	//	float vert_z0 =  XMVectorGetZ(vertEdge0);
	//	float vert_x1 =  XMVectorGetX(vertEdge1);
	//	float vert_y1 =  XMVectorGetY(vertEdge1);
	//	float vert_z1 = XMVectorGetZ(vertEdge1);


	//	float ratio = 1.0f / (uv_x0* uv_y1 - uv_x1*uv_y0);

	//	XMFLOAT3 uDirection = (XMFLOAT3((uv_y1*vert_x0)-  (uv_y0*vert_x1)*ratio,
	//		                           (uv_y1*vert_y0) - (uv_y0*vert_y1)*ratio,
	//		                           (uv_y1*vert_z0) - (uv_y0*vert_z1)*ratio));
	//	XMStoreFloat3(&uDirection, XMVector3Normalize(XMLoadFloat3(&uDirection)));

	//	XMVECTOR dotResult = XMVector3Dot(XMLoadFloat3(&t.normal), XMLoadFloat3(&uDirection));
	//	XMStoreFloat4(&t.tangent,XMVectorSubtract(XMLoadFloat3(&uDirection),XMVectorMultiply(XMLoadFloat3(&t.normal), dotResult)));
	//	XMStoreFloat4(&t.tangent, XMVector3Normalize(XMLoadFloat4(&t.tangent)));

	//	dotResult = XMVector3Dot(XMLoadFloat3(&t1.normal), XMLoadFloat3(&uDirection));
	//	XMStoreFloat4(&t1.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t1.normal), dotResult)));
	//	XMStoreFloat4(&t1.tangent, XMVector3Normalize(XMLoadFloat4(&t1.tangent)));

	//	dotResult = XMVector3Dot(XMLoadFloat3(&t2.normal), XMLoadFloat3(&uDirection));
	//	XMStoreFloat4(&t2.tangent, XMVectorSubtract(XMLoadFloat3(&uDirection), XMVectorMultiply(XMLoadFloat3(&t2.normal), dotResult)));
	//	XMStoreFloat4(&t2.tangent, XMVector3Normalize(XMLoadFloat4(&t2.tangent)));

	//	//XMFLOAT3 vDirection = XMFLOAT3((uv_x0*vert_x1) - (uv_y1*vert_x0)*ratio,
	//	//	                           (uv_y0*vert_y1) - (uv_y1*vert_y0)*ratio,
	//	//	                           (uv_z0*vert_z1) - (uv_y1*vert_z0)*ratio);

	//	//XMStoreFloat3(&vDirection, XMVector3Normalize(XMLoadFloat3(&vDirection)));
	//	//XMVECTOR cross = XMVector3Cross(XMLoadFloat3(&t.normal), XMLoadFloat3(&uDirection));
	//	//


	//	Vertexs.push_back(t);
	//	Indexs.push_back(i);
	//	Vertexs.push_back(t1);
	//	Indexs.push_back(i+1);
	//	Vertexs.push_back(t2);
	//	Indexs.push_back(i+2);
	//}
#pragma endregion
	fclose(model_file);
	return true;
}

