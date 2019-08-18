#pragma once
#include<iostream>
#include "SFML/Graphics.hpp"
#include "Board.h"
#include "Piece.h"
#include "Player.h"

class Game
{
public:
	sf::RenderWindow m_window;
	sf::View m_view;
	int m_currentState;

	float m_dt;

	sf::Texture m_backgroundTexture;
	sf::Sprite  m_backgroundSprite;

	Board m_board;
	int m_nrPlayers;
	Player m_players[2];

	bool m_keyPressed[sf::Keyboard::Key::KeyCount];

	float m_sideMoveAccumulator;
	float m_timeBetweenSideMoves;
	float m_delayFirstSideMove;

	enum EMoveDirection
	{
		UP = 0,
		DOWN,
		LOWER,
	};

	bool m_gameOver;

	enum EStates
	{
		SPLASHSCREEN = 0,
		MENU,
		GAME,
		GAMEEND
	};

	void Init();
	void SpawnPiece(Player& player);
	bool OnKeyPress(sf::Keyboard::Key key);
	bool SideMoveOnKeyHold(sf::Keyboard::Key key, EMoveDirection direction, Player& player);
	bool CanLineBeCleared(int line);
	void ClearLine(int line);
	void DropDownTheLinesAboveClearedLine(int line);
	void DropDownTheLinesAboveSeparator(int line);
	void PushUpTheLinesAboveSeparator(int line, Player& player);
	Board::ECollisions::Enum ComputeCollisions(Player& player, sf::Vector2i pieceSpawnPos);
	bool IsPieceSeated(Player& player);
	void Update(float dt);
	void Draw();
};
