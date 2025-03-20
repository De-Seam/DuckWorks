#pragma once
#include <DuckCore/Events/Event.h>

class PreRendererBeginFrameEvent : public DC::Event
{
	RTTI_CLASS(RendererBeginFrameEvent, Event)
};

class PostRendererBeginFrameEvent : public DC::Event
{
	RTTI_CLASS(RendererBeginFrameEvent, Event)
};

class PreRendererEndFrameEvent : public DC::Event
{
	RTTI_CLASS(RendererEndFrameEvent, Event)
};

class PostRendererEndFrameEvent : public DC::Event
{
	RTTI_CLASS(RendererEndFrameEvent, Event)
};