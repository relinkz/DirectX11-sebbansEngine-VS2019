#include "Models.h"
#include "ResourceBufferFactory.h"
#include "ShaderFactory.h"
#include "../OBJ_Loader.h"
#include "../ComException.h"
#include <sstream>
#include "../ErrorLogger.h"
#include <WICTextureLoader.h>


using namespace DirectX;
using namespace std;

void Model::InitializeConstantBuffers(Microsoft::WRL::ComPtr<ID3D11Device>& device)
{
	auto rFactory = ResourceBufferFactory();

	m_vsCbuff = rFactory.CreateSimpleVsConstantBuffer(device);
	if (!m_vsCbuff)
	{
		errorlogger::Log("Failed to create model constant buffer (vertex)");
	}

	m_psCbuff = rFactory.CreateMaterialPsConstantBuffer(device);
	if (!m_psCbuff)
	{
		errorlogger::Log("Failed to create model constant buffer (pixel)");
	}
}

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

void Model::AddOffsetToLocalVerticies(const DirectX::XMFLOAT3& offset)
{
	for (size_t i = 0; i < m_vertecies.size(); i++)
	{
		m_vertecies.at(i).m_pos.x += offset.x;
		m_vertecies.at(i).m_pos.y += offset.y;
		m_vertecies.at(i).m_pos.z += offset.z;
	}
}

XMMATRIX Model::GetWorldMatrix() const
{
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	auto scaleMatrix = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	auto translationMatrix = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

	return scaleMatrix * rotationMatrix * translationMatrix;
}

DirectX::XMMATRIX Model::GetRotationMatrix() const
{
	auto rotationMatrix = XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	return rotationMatrix;
}

void Model::UpdateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx, CB_VS_vertexShader newData) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResourceVs;
	auto hr = dctx->Map(m_vsCbuff->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourceVs.pData, &newData, sizeof(CB_VS_vertexShader));
	dctx->Unmap(m_vsCbuff->GetBuffer(), 1);
	dctx->VSSetConstantBuffers(1, 1, m_vsCbuff->GetBufferAddress());
}

void Model::UpdateConstantBuffer(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx, CB_PS_pixelMaterialShader newData) const
{
	D3D11_MAPPED_SUBRESOURCE mappedResourcePs;
	auto hr = dctx->Map(m_psCbuff->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePs);
	COM_ERROR_IF_FAILED(hr, "Failed to map ConstantBuffer.");

	CopyMemory(mappedResourcePs.pData, &newData, sizeof(CB_PS_pixelMaterialShader));
	dctx->Unmap(m_psCbuff->GetBuffer(), 1);
	dctx->PSSetConstantBuffers(1, 1, m_psCbuff->GetBufferAddress());

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

void Model::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dctx) const
{
	// set vertexShader
	dctx->VSSetShader(m_vShader->GetShader(), NULL, 0);

	// set pixelShader
	dctx->PSSetShader(m_pShader->GetShader(), NULL, 0);

	// update vertex constant buffer
	CB_VS_vertexShader cWorldMatrix;
	cWorldMatrix.m_matrix = DirectX::XMMatrixTranspose(GetWorldMatrix());
	cWorldMatrix.m_view = DirectX::XMMatrixTranspose(GetRotationMatrix());

	UpdateConstantBuffer(dctx, cWorldMatrix);

  // update pixel constant buffer
	CB_PS_pixelMaterialShader cPsMatData;
	cPsMatData.Ka = m_Ka;
	cPsMatData.Kd = m_Kd;
	cPsMatData.Ks = m_Ks;
	cPsMatData.Ns = m_Ns;

	UpdateConstantBuffer(dctx, cPsMatData);

	UINT offset = 0;
	UINT stride = m_vertexes->GetStride();

	dctx->IASetVertexBuffers(0, 1, m_vertexes->GetBufferAddress(), &stride, &offset);
	dctx->IASetInputLayout(m_vShader->GetInputLayout());
	dctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	dctx->PSSetShaderResources(0, 1, m_diffuseTexture.GetAddressOf());
	dctx->PSSetShaderResources(1, 1, m_normalTexture.GetAddressOf());
	dctx->PSSetShaderResources(2, 1, m_occlusionTexture.GetAddressOf());
	dctx->PSSetShaderResources(3, 1, m_specularTexture.GetAddressOf());

	// call draw
	dctx->Draw(m_vertexes->GetNrOfVerticies(), 0);
}

void Model::ReadObjFile(const std::string& file)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(file);
	objl::Mesh mesh = loader.LoadedMeshes[0];
	m_diffuseMap.emplace_back(mesh.MeshMaterial.map_Kd);

	m_Ka = DirectX::XMFLOAT4(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z, 0.0f);
	m_Kd = DirectX::XMFLOAT4(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z, 0.0f);
	m_Ks = DirectX::XMFLOAT4(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z, 0.0f);
	m_Ns = DirectX::XMFLOAT4(mesh.MeshMaterial.Ns, 0.0f, 0.0f, 0.0f);

	vector<Vertex> obj = std::vector<Vertex>(mesh.Vertices.size());

	for (size_t i = 0; i < mesh.Vertices.size(); i++)
	{
		auto pos = mesh.Vertices[i].Position;
		obj.at(i).m_pos = DirectX::XMFLOAT3(pos.X, pos.Y, pos.Z);

		auto norm = mesh.Vertices[i].Normal;
		obj.at(i).m_normal = DirectX::XMFLOAT3(norm.X, norm.Y, norm.Z);

		auto uvs = mesh.Vertices[i].TextureCoordinate;
		obj.at(i).m_texCoord = DirectX::XMFLOAT2(uvs.X, uvs.Y);
	}

	m_vertecies = vector<Vertex>(mesh.Indices.size());
	for (size_t i = 0; i < mesh.Indices.size(); i++)
	{
		m_vertecies.at(i) = obj.at(mesh.Indices[i]);
	}
}

void QuadModel::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dCtx)
{
	ResetTransformation();

	ReadObjFile("./Data/ObjFiles/QuadObj.obj");
}

void Box::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& dCtx)
{
	ResetTransformation();

	ReadObjFile("./Data/ObjFiles/box 01.obj");

	auto shaderFactory = ShaderFactory();

	const std::wstring vs1 = L"VertexShader";
	m_vShader = shaderFactory.CreateDefaultVertexShader(device, vs1);

	const std::wstring ps1 = L"PixelShader";
	m_pShader = shaderFactory.CreateDefaultPixelShader(device, ps1);

	InitializeConstantBuffers(device);

	const std::wstring pathToDiffuseMap = L"Data\\Textures\\tex_box_01_d.jpg";
	const std::wstring pathToNormalMap = L"Data\\Textures\\tex_box_01_n.jpg";
	const std::wstring pathToOcclutionMap = L"Data\\Textures\\tex_box_01_s.jpg";
	const std::wstring pathToSpecularMap = L"Data\\Textures\\tex_box_01_occ.jpg";

	auto hr = DirectX::CreateWICTextureFromFile(device.Get(), pathToDiffuseMap.c_str(), nullptr, m_diffuseTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load diffuse map.");
	hr = DirectX::CreateWICTextureFromFile(device.Get(), pathToNormalMap.c_str(), nullptr, m_normalTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load normal map.");
	hr = DirectX::CreateWICTextureFromFile(device.Get(), pathToOcclutionMap.c_str(), nullptr, m_occlusionTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load occlusion map.");
	hr = DirectX::CreateWICTextureFromFile(device.Get(), pathToSpecularMap.c_str(), nullptr, m_specularTexture.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to load specular map.");
	
	const auto offset = DirectX::XMFLOAT3(0.0f, -0.5, 0.0f);
	AddOffsetToLocalVerticies(offset);

	m_vertexes = GetResourceVertexBuffer(device);
}