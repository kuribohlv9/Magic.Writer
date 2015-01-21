#include "stdafx.h"
#include "Word.h"

Word::Word(std::string word, sf::Vector2f position)
{
	m_text = word;
	m_position = position;
	m_active = true;
}
Word::~Word()
{
}

sf::Vector2f Word::GetPosition()
{
	return m_position;
}
std::string Word::GetText()
{
	return m_text;
}

bool Word::IsActive()
{
	return m_active;
}
void Word::SetActive(bool state)
{
	m_active = state;
}