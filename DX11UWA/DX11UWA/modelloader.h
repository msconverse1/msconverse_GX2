#pragma once
#include <DirectXMath.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace DirectX;
using namespace std;

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

bool Load(const char* fname, vector<Vertex> &Vertexs, vector<unsigned int> & Indexs)
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
		Vertex t;
		t.pos = hold.temp_vert[hold.vertexIndices[i] - 1];
		t.uv = hold.temp_uvs[hold.uvIndices[i] - 1];
		t.norm = hold.temp_normals[hold.normalIndices[i] - 1];
		Vertexs.push_back(t);
		Indexs.push_back(i);
	}
	fclose(model_file);
	return true;
}


