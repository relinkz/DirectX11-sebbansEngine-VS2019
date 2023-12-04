#include "ModelFactory.h"
#include "Models.h"

using namespace std;
using namespace Microsoft::WRL;

unique_ptr<IModel> ModelFactory::CreateQuadModel()
{
	auto object = std::make_unique<QuadModel>();
	object->Initialize();

	return move(object);
}

std::unique_ptr<IModel> ModelFactory::CreateBox()
{
	return std::unique_ptr<IModel>();
}
