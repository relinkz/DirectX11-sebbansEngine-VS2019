#include "ModelFactory.h"
#include "QuadModel.h"

using namespace std;
using namespace Microsoft::WRL;

unique_ptr<IModel> ModelFactory::CreateQuadModel(ComPtr<ID3D11Device>& device)
{
	auto object = std::make_unique<QuadModel>();
	object->Initialize(device);

	return object;
}