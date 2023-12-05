#include "Models.h"
#include "ResourceBufferFactory.h"
#include "../OBJ_Loader.h"
#include <sstream>

using namespace DirectX;
using namespace std;

void Model::SetPosition(const XMFLOAT3& pos)
{
	m_pos = pos;
}

void Model::SetRotation(const XMFLOAT3& rot)
{
	m_rot = rot;
}

void Model::SetScale(const XMFLOAT3& scale)
{
	m_scale = scale;
}

void Model::ResetTransformation()
{
	m_pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

XMMATRIX Model::GetWorldMatrix() const
{
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	auto scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto translationMatrix = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	return scaleMatrix * rotationMatrix * translationMatrix;
}

unique_ptr<IResourceVertexBuffer> Model::GetResourceVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	ResourceBufferFactory resourceFactory = ResourceBufferFactory();
	auto vb = resourceFactory.CreateSimpleVertexBuffer(device, m_vertecies);
	if (!vb)
	{
		assert(false && "Failed to create vertexBuffer");
	}

	return move(vb);
}

std::vector<std::wstring> Model::GetDiffuseMaps() const
{
	auto toRet = std::vector<std::wstring>(m_textureMapD.size());
	
	for (int i = 0; i < m_textureMapD.size(); i++)
	{
		std::wstringstream wss;
		wss << "Data\\Textures\\" << m_textureMapD.at(i).c_str();
		toRet.at(i) = wss.str();
	}

	return toRet;
}

void Model::ReadObjFile(const std::string& file)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(file);

	objl::Mesh mesh = loader.LoadedMeshes[0];
	m_textureMapD.emplace_back(mesh.MeshMaterial.map_Kd);

	vector<Vertex> obj = std::vector<Vertex>(mesh.Vertices.size());

	for (size_t i = 0; i < mesh.Vertices.size(); i++)
	{
		auto pos = mesh.Vertices[i].Position;
		obj.at(i).m_pos = DirectX::XMFLOAT3(pos.X, pos.Y, pos.Z);
		auto uvs = mesh.Vertices[i].TextureCoordinate;
		obj.at(i).m_texCoord = DirectX::XMFLOAT2(uvs.X, uvs.Y);
	}

	m_vertecies = vector<Vertex>(mesh.Indices.size());
	for (size_t i = 0; i < mesh.Indices.size(); i++)
	{
		m_vertecies.at(i) = obj.at(mesh.Indices[i]);
	}
}

void QuadModel::Initialize()
{
	ResetTransformation();

	ReadObjFile("./Data/ObjFiles/QuadObj.obj");
}

void Box::Initialize()
{
	ResetTransformation();

	ReadObjFile("./Data/ObjFiles/box 01.obj");
}