#pragma once
//#include "Layer.h"
#include <vector>;
#include <memory>
#include "Component.h"
using std::shared_ptr;
class  LayerManager
{
private:
	std::vector<shared_ptr<Component>> layerArray;

public:
	LayerManager() = default;
	~LayerManager();

public:
	void PushLayer(shared_ptr<Component> layer);
	void PopLayer(shared_ptr<Component> layer);

public:
	std::vector<shared_ptr<Component>>::iterator Begin() { return layerArray.begin(); }
	std::vector<shared_ptr<Component>>::iterator End() { return layerArray.end(); }
};