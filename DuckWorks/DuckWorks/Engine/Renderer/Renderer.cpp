#include <Precomp.h>
#include <Engine/Renderer/Renderer.h>

// Engine includes
#include <Engine/Renderer/WindowEvents/WindowEventManager.h>

// External includes
#include <External/SFML/Graphics.hpp>

Renderer::Renderer()
{
	PROFILE_SCOPE(Renderer::Renderer)


	sf::VideoMode video_mode;
	video_mode.width = 1280;
	video_mode.height = 720;
	mRenderWindow = std::make_unique<sf::RenderWindow>(video_mode, "DuckRenderer");
	//mRenderWindow->setVerticalSyncEnabled(true);
	mRenderWindow->setFramerateLimit(60);
	uint32 pixels[8 * 8] =
	{
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF00FF00, 0xFF00FF00, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0xFFFF00FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFF0000FF, 0xFFFF00FF, 0x00000000,
		0x00000000, 0x00000000, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
	};
	mRenderWindow->setIcon(8, 8, reinterpret_cast<uint8*>(pixels));

	mWindowEventManager = new WindowEventManager(*this);
	mWindowEventManager->RegisterMessageListener(this, &Renderer::OnMouseWheelScrolled);
	mWindowEventManager->RegisterMessageListener(this, &Renderer::OnMouseMoved);
	mWindowEventManager->RegisterMessageListener(this, &Renderer::OnWindowResized);
}

Renderer::~Renderer()
{
	mWindowEventManager->UnregisterMessageListener(this, &Renderer::OnMouseWheelScrolled);
	mWindowEventManager->UnregisterMessageListener(this, &Renderer::OnMouseMoved);
	mWindowEventManager->UnregisterMessageListener(this, &Renderer::OnWindowResized);

	mRenderWindow->close();
}

Json Renderer::Serialize() const
{
	Json json = RTTIClass::Serialize();

	json["mRenderWindow_Position"] = IVec2(mRenderWindow->getPosition());
	json["mRenderWindow_Size"] = IVec2(mRenderWindow->getSize());

	return json;
}

void Renderer::Deserialize(const Json& inJson)
{
	RTTIClass::Deserialize(inJson);
	
	IVec2 position = inJson["mRenderWindow_Position"];
	mRenderWindow->setPosition({ position.mX, position.mY });
	IVec2 size = inJson["mRenderWindow_Size"];
	mRenderWindow->setSize({ static_cast<uint32>(size.mX), static_cast<uint32>(size.mY) });
}

void Renderer::Update(float inDeltaTime)
{
	PROFILE_SCOPE(Renderer::Update)

	

	mWindowEventManager->Update(*this, inDeltaTime);
}

void Renderer::BeginFrame()
{
	PROFILE_SCOPE(Renderer::BeginFrame)

	BroadcastMessage<MsgPreBeginFrame>();

	mRenderWindow->clear();

	BroadcastMessage<MsgPostBeginFrame>();
}

void Renderer::EndFrame()
{
	PROFILE_SCOPE(Renderer::EndFrame)

	BroadcastMessage<MsgPreEndFrame>();

	mRenderWindow->display();

	BroadcastMessage<MsgPostEndFrame>();
}

void Renderer::Draw(const sf::Drawable& inDrawable)
{
	PROFILE_SCOPE(Renderer::Draw)

	mRenderWindow->draw(inDrawable);
}

void Renderer::Draw(const Array<sf::Drawable*>& inDrawables)
{
	PROFILE_SCOPE(Renderer::Draw::Array)

	for (const sf::Drawable* drawable : inDrawables)
		mRenderWindow->draw(*drawable);
}

void Renderer::OnMouseWheelScrolled(const MsgWindowMouseWheelScrolled& inMsg)
{
	PROFILE_SCOPE(Renderer::OnMouseWheelScrolled)

	sf::View view = mRenderWindow->getView();
	view.zoom(1.0f - inMsg.mDelta * 0.1f);
	mRenderWindow->setView(view);
}

void Renderer::OnMouseMoved(const MsgWindowMouseMoved& inMsg)
{
	PROFILE_SCOPE(Renderer::OnMouseMoved)

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
	{
		sf::View view = mRenderWindow->getView();
		view.move(-mWindowEventManager->GetMouseDelta() * view.getSize() / Vec2(1920, 1080));
		mRenderWindow->setView(view);
	}
}

void Renderer::OnWindowResized(const MsgWindowResized& inMsg)
{
	sf::View view;
	view.setSize(1920, 1080);
	view.zoom(1.0f);
	view.setCenter({0.0f, 0.0f});

	float target_width = 1920.f;
	float target_height = 1080.f;

	float window_width = static_cast<float>(inMsg.mWidth);
	float window_height = static_cast<float>(inMsg.mHeight);

	// Calculate aspect ratios
	float window_aspect = window_width / window_height;
	float view_aspect = target_width / target_height;

	if (window_aspect > view_aspect) 
	{
		// Window is wider than the view, add black bars on the sides
		float scale = window_height / target_height;
		float new_width = target_width * scale / window_width;
		view.setViewport(sf::FloatRect((1.f - new_width) / 2.f, 0.f, new_width, 1.f));
	}
	else 
	{
		// Window is taller than the view, add black bars on the top and bottom
		float scale = window_width / target_width;
		float new_height = target_height * scale / window_height;
		view.setViewport(sf::FloatRect(0.f, (1.f - new_height) / 2.f, 1.f, new_height));
	}

	mRenderWindow->setView(view);
}
