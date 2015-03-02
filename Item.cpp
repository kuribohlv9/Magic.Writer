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

	m_emitter = ServiceLocator<ParticleManager>::GetService()->CreateEmitter(particle, 70);
	m_emitter->SetActive(false);
	m_emitter->SetPosition(-100, -100);
	m_emitter->SetStartAngle(70, 110);
	m_emitter->SetSpawnRate(0.01f);
	m_emitter->SetSize(10, 1);
	m_emitter->SetLifeTime(1, 1);
}
Item::~Item()
{
}
void Item::Update(float deltaTime)
{
	//Move upwards
	Move(0, -m_speed * deltaTime);

	m_emitter->SetPosition(GetX(), GetY());

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
		Reset();
	else
	{
		m_emitter->SetActive(false);
	}

	m_inGame = state;
}

void Item::Reset()
{
	m_sprite.setScale(0.5, 0.5);
	m_sprite.setColor(sf::Color::White);

	SetState(ITEM_HOLDING);
}
void Item::SetSourceRectangle(int index, sf::IntRect rectangle)
{
	m_sourceRectangles[index] = rectangle;
	m_sprite.setTextureRect(rectangle);
	m_sprite.setOrigin(rectangle.width / 2, rectangle.height / 2);
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
