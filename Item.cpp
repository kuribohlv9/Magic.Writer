#include "stdafx.h"
#include "Item.h"
#include "Collider.h"
#include "DrawManager.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "ServiceLocator.h"

Item::Item(sf::Texture* texture, sf::Texture* particle, ItemProperty property, const std::string& name)
{
	m_active = false;
	m_type = GAMEOBJECT_ITEM;
	m_state = ITEM_HOLDING;
	m_property = property;
	m_name = name;
	m_inGame = false;
	m_speed = 800;
	m_sprite.setTexture(*texture);
	m_collider = new Collider(0, 0);

	m_emitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(particle, 50);
	m_emitter->SetActive(false);
	m_emitter->SetPosition(-100, -100);
	m_emitter->SetStartAngle(90, 90);
	m_emitter->SetSpawnRate(0.01f);
	m_emitter->SetSize(10, 1);
	m_emitter->SetLifeTime(0.7f, 0.7f);
	m_emitter->SetRotationVelocity(5);
	m_emitter->SetAcceleration(0, -3);
}
Item::~Item()
{
}
void Item::Update(float deltaTime)
{
	m_emitter->SetPosition(GetX(), GetY());
	m_emitter->SetStartAngle(80, 100);

	//Deactivate above screen
	if (m_y <= -400)
	{
		SetActive(false);
		SetInGame(false);
	}
}
void Item::Draw(DrawManager* drawManager)
{
	drawManager->Draw(m_sprite, sf::RenderStates::Default);
}
std::string Item::GetName()
{
	return m_name;
}
ItemProperty Item::GetProperty()
{
	return m_property;
}

bool Item::IsInGame()
{
	return m_inGame;
}
void Item::SetInGame(bool state)
{
	if (state)
	{
		Reset();
	}
	else
	{
		m_emitter->SetActive(false);
	}

	m_inGame = state;
}

void Item::Reset()
{
	m_sprite.setScale(0.7f, 0.7f);
	m_sprite.setColor(sf::Color::White);

	SetState(ITEM_HOLDING);
}
void Item::SetSourceRectangle(int index, sf::IntRect rectangle)
{
	m_sourceRectangles[index] = rectangle;
	m_sprite.setTextureRect(rectangle);
	m_sprite.setOrigin(rectangle.width / 2.0f, rectangle.height / 2.0f);
	m_collider->SetParent(this);
	m_collider->SetWidthHeight((int)(rectangle.width * 0.4f), (int)(rectangle.height * 0.4f));
}
void Item::SetName(const std::string& name)
{
	m_name = name;
}
void Item::SetState(ItemState state)
{
	m_sprite.setTextureRect(m_sourceRectangles[state]);
}
void Item::Activate()
{
	SetActive(true);
	SetState(ITEM_FLYING);
	m_emitter->SetActive(true);
}
