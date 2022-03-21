#pragma once
#include "EventBase.h"
class TesterEvent :public Event
{
public:
    int key;
    TesterEvent() = default;

    EVENT_CLASS_TYPE(Tester)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryGame)
};

