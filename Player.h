#pragma once
#include<SFML/Graphics.hpp>
#include "Piece.h"
class Player
{
public:
	sf::Keyboard::Key m_up;
	sf::Keyboard::Key m_down;
	sf::Keyboard::Key m_lower;
	sf::Keyboard::Key m_hardDrop;
	sf::Keyboard::Key m_rotateLeft;
	sf::Keyboard::Key m_rotateRight;

	Piece m_piece;
	sf::Vector2i m_pieceSpawnPosition;
	int m_linesCleared;

	void InitKeybindings(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key lower,
		sf::Keyboard::Key hardDrop, sf::Keyboard::Key rotateLeft, sf::Keyboard::Key rotateRight);

	void Init(sf::Vector2i position);

};