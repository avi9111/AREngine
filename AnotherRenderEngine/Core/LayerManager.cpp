#include "LayerManager.h"

LayerManager::~LayerManager()
{
	for (auto& layer : layerArray)
	{
		layer->OnDetach();
	}

	layerArray.clear();
}

void LayerManager::PushLayer(shared_ptr<Component> layer)
{
	layerArray.push_back(layer);
	layer->OnAttach();
}

void LayerManager::PopLayer(shared_ptr<Component> inLayer)
{
	auto iter = std::find(layerArray.begin(), layerArray.end(), inLayer);

	if (iter != layerArray.end())
	{
		(*iter)->OnDetach();
		layerArray.erase(iter);
	}
}