#include "KeyMapping.h"

std::ifstream &operator >>(std::ifstream &stream, KeyMappingBase &mapping)
{
	stream >> mapping.m_key;
	stream >> mapping.m_button;

	return stream;
}

std::ofstream &operator <<(std::ofstream &stream, KeyMappingBase &mapping)
{
	stream << mapping.m_key;
	stream << mapping.m_button;

	return stream;
}


int KeyMappingBase::key() const
{
	return m_key;
}

int KeyMappingBase::button() const
{
	return m_button;
}

void KeyMappingBase::onKeyDown() const
{
	if(m_keyDown)
	{
		m_keyDown();
	}
}

void KeyMappingBase::onKeyUp() const
{
	if(m_keyUp)
	{
		m_keyUp();
	}
}