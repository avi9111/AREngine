//#include "../Render/InputClass.h"
#pragma once
#include <string>
class Component
{
protected:
	std::string layerName;

public:
	Component(const std::string& name = "layer") //:	layerName(name)
	{
	};

	virtual ~Component() = default;

	//void Render(InputClass* input);//TODO:”–“¿¿µ£¨£¨£¨£¨£¨
public:
	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void OnTick(float deltaTime) {};
	//virtual void OnEvent(Event& event) {};
	virtual void OnImguiRender() {};
	virtual void End() {};
};