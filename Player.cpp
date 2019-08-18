#include "Player.h"

void Player::InitKeybindings(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key lower,
			sf::Keyboard::Key hardDrop, sf::Keyboard::Key rotateLeft, sf::Keyboard::Key rotateRight)
{
	m_up = up;
	m_down = down;
	m_lower = lower;
	m_hardDrop = hardDrop;
	m_rotateLeft = rotateLeft;
	m_rotateRight = rotateRight;
}

void Player::Init(sf::Vector2i position)
{
	m_pieceSpawnPosition = position;
	m_linesCleared = 0;
}