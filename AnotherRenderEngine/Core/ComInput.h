#include "Component.h"
#include <functional>
#include "Event/EventBase.h"
class ComInput: public Component
{
public:
	using EventCallback = std::function<void(Event&)>;
	EventCallback eventCallback;
	void SetEventCallback(const EventCallback& callBack);
	ComInput();
	void OnImguiRender() override;
};