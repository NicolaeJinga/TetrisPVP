#include "Game.h"
#include "Utilities.h"

void Game::Init()
{
	srand((int)time(NULL));
	m_window.create(sf::VideoMode(1600, 900), "Tetris");
	m_view.setSize(sf::Vector2f(16.0f, 9.0f));
	m_view.setCenter(sf::Vector2f(0.0f, 0.0f));
	m_view.setRotation(0);
	m_window.setView(m_view);

	m_backgroundTexture.loadFromFile("Resources/bg2.jpg");
	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_backgroundSprite.setScale(16.0f / m_backgroundSprite.getTexture()->getSize().x,
								 9.0f / m_backgroundSprite.getTexture()->getSize().y);
	m_backgroundSprite.setOrigin(sf::Vector2f(m_backgroundSprite.getTexture()->getSize())*0.5f);

	m_currentState = GAME;
	m_gameOver = false;

	m_sideMoveAccumulator  = 0.0f;
	m_timeBetweenSideMoves = 0.1f;
	m_delayFirstSideMove   = 0.2f;

	m_board.Init(sf::Vector2i(40, 10), sf::Vector2f(-7.2f, -1.8f), sf::Vector2f(14.4f, 3.6f),20);
	
	Piece::LoadFromFile();
	Piece::LoadTextures();

	m_nrPlayers = 2;
	m_players[0].Init(sf::Vector2i(0, m_board.m_matrixSize.y / 2 - 2));
	m_players[0].InitKeybindings(sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::D,
								 sf::Keyboard::Key::A, sf::Keyboard::Key::Q, sf::Keyboard::Key::E);

	m_players[1].Init(sf::Vector2i(m_board.m_matrixSize.x - 4, m_board.m_matrixSize.y / 2 - 2));
	m_players[1].InitKeybindings(sf::Keyboard::Key::Numpad8, sf::Keyboard::Key::Numpad5, sf::Keyboard::Key::Numpad4,
								 sf::Keyboard::Key::Numpad6, sf::Keyboard::Key::Numpad7, sf::Keyboard::Key::Numpad9);
	SpawnPiece(m_players[0]);
	SpawnPiece(m_players[1]);
	memset(m_keyPressed, (int)false, sf::Keyboard::Key::KeyCount * sizeof(bool));
}

void Game::SpawnPiece(Player& player)
{
	player.m_piece.Init(player.m_pieceSpawnPosition, (Piece::EType)(rand() % 7), m_board.m_cellSize, m_board.m_graphicPosition);
	if (ComputeCollisions(player, player.m_pieceSpawnPosition) != Board::ECollisions::None)
	{
		m_gameOver = true;
	}
}

bool Game::OnKeyPress(sf::Keyboard::Key key)
{
	if (!sf::Keyboard::isKeyPressed(key) && m_keyPressed[key])
	{
		m_keyPressed[key] = false;
	}
	if (sf::Keyboard::isKeyPressed(key) && !m_keyPressed[key])
	{
		m_keyPressed[key] = true;
		return true;
	}
	return false;
}

bool Game::SideMoveOnKeyHold(sf::Keyboard::Key key, EMoveDirection direction, Player& player)
{
	bool hasMoved = false;
	if (OnKeyPress(key))
	{
		m_sideMoveAccumulator = m_timeBetweenSideMoves;
		if (direction == EMoveDirection::UP)
			player.m_piece.m_position.y--;
		if (direction == EMoveDirection::DOWN)
			player.m_piece.m_position.y++;
		if (player.m_pieceSpawnPosition.x < m_board.m_separatorPosition)
		{
			if (direction == EMoveDirection::LOWER)
				player.m_piece.m_position.x++;
		}
		else
		{
			if (direction == EMoveDirection::LOWER)
				player.m_piece.m_position.x--;
		}
		hasMoved = true;
	}
	if (sf::Keyboard::isKeyPressed(key))
	{
		if (m_sideMoveAccumulator >= m_delayFirstSideMove + m_timeBetweenSideMoves)
		{
			hasMoved = true;
			if (direction == EMoveDirection::UP)
				player.m_piece.m_position.y--;
			if (direction == EMoveDirection::DOWN)
				player.m_piece.m_position.y++;
			if (player.m_pieceSpawnPosition.x < m_board.m_separatorPosition)
			{
				if (direction == EMoveDirection::LOWER)
					player.m_piece.m_position.x++;
			}
			else
			{
				if (direction == EMoveDirection::LOWER)
					player.m_piece.m_position.x--;
			}
			m_sideMoveAccumulator = m_delayFirstSideMove;
		}
		m_sideMoveAccumulator += m_dt;
	}
	return hasMoved;
}

bool Game::CanLineBeCleared(int line)
{
	bool isLineComplete = true;
	for (int j = 0; j < m_board.m_matrixSize.y; ++j)
	{
		if (m_board.At(sf::Vector2i(line, j)) == -1)
		{
			isLineComplete = false;
		}
	}
	return isLineComplete;
}

void Game::ClearLine(int line)
{
	for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		m_board.At(sf::Vector2i(line, j)) = -1;
}

void Game::DropDownTheLinesAboveClearedLine(int line)
{
	if (line < m_board.m_separatorPosition)
	{
		m_board.m_separatorPosition++;
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = line; i > 0; --i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i - 1, j));
			}
		}
	}
	else
	{
		m_board.m_separatorPosition--;
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = line; i < m_board.m_matrixSize.x - 1; ++i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i + 1, j));
			}
		}
	}
}

void Game::DropDownTheLinesAboveSeparator(int line)
{
	if (line < m_board.m_separatorPosition)
	{
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = m_board.m_separatorPosition; i > 0; --i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i - 1, j));
			}
		}
	}
	else
	{
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = m_board.m_separatorPosition; i < m_board.m_matrixSize.x - 1; ++i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i + 1, j));
			}
		}
	}
}

void Game::PushUpTheLinesAboveSeparator(int line, Player& player)
{
	if (line < m_board.m_separatorPosition)
	{
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = m_board.m_matrixSize.x-1; i >= m_board.m_separatorPosition; --i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i - 1, j));
			}
		}
		player.m_piece.m_position.x++;
	}
	else
	{
		for (int j = 0; j < m_board.m_matrixSize.y; ++j)
		{
			for (int i = 0; i < m_board.m_separatorPosition; ++i)
			{
				m_board.At(sf::Vector2i(i, j)) = m_board.At(sf::Vector2i(i + 1, j));
			}
		}
		player.m_piece.m_position.x--;
	}
}

Board::ECollisions::Enum Game::ComputeCollisions(Player& player, sf::Vector2i pieceSpawnPos)
{
	return m_board.ComputeCollision(player.m_piece, pieceSpawnPos);
}

bool Game::IsPieceSeated(Player& player)
{
	if (player.m_pieceSpawnPosition.x < m_board.m_separatorPosition)
	{
		player.m_piece.m_position.x++;
		bool isSeated = ComputeCollisions(player, player.m_pieceSpawnPosition) != Board::ECollisions::None;
		player.m_piece.m_position.x--;
		return isSeated;
	}
	else
	{
		player.m_piece.m_position.x--;
		bool isSeated = ComputeCollisions(player, player.m_pieceSpawnPosition) != Board::ECollisions::None;
		player.m_piece.m_position.x++;
		return isSeated;
	}
}

void Game::Update(float dt)
{
	m_dt = dt;
	switch (m_currentState)
	{
	case SPLASHSCREEN:
	{
			
	}

	case MENU:
	{

	}

	case GAME:
	{	
		for (int no = 0; no < m_nrPlayers; ++no)
		{
			sf::Vector2i lastPosition = m_players[no].m_piece.m_position;
			//Rotation
			bool hasRotated = false;
			bool clockwise = false;
			sf::Vector2f lastGeometricCenter = (sf::Vector2f)m_players[no].m_piece.m_position + m_players[no].m_piece.m_geometricCenter;

			if (OnKeyPress(m_players[no].m_rotateLeft))
			{
				m_players[no].m_piece.Rotate(false);
				clockwise = false;
				hasRotated = true;
			}
			if (OnKeyPress(m_players[no].m_rotateRight))
			{
				m_players[no].m_piece.Rotate(true);
				clockwise = true;
				hasRotated = true;
			}
			if (hasRotated)
			{
				bool foundGoodSpot = false;
				for (int r = 0; r <= 2; ++r)
				{
					for (int offSetX = -r; offSetX <= r; ++offSetX)
					{
						for (int offSetY = -r; offSetY <= r; ++offSetY)
						{
							m_players[no].m_piece.m_position = lastPosition + sf::Vector2i(offSetX, offSetY);
							sf::Vector2f geometricCenter = (sf::Vector2f)m_players[no].m_piece.m_position + m_players[no].m_piece.m_geometricCenter;
							if (Utilities::Distance(geometricCenter, lastGeometricCenter) < 2.6f)
							{
								if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) == Board::ECollisions::None)
								{
									foundGoodSpot = true;
									break;
								}
							}
						}
						if (foundGoodSpot)
							break;
					}
					if (foundGoodSpot)
						break;
				}

				if (!foundGoodSpot)
				{
					m_players[no].m_piece.m_position = lastPosition;
					hasRotated = false;
					if (clockwise)
						m_players[no].m_piece.Rotate(false);
					else
						m_players[no].m_piece.Rotate(true);
				}
				if (hasRotated)
				{
					m_players[no].m_piece.AddTimeSeated(-0.05f);
				}
			}

			//Movement
			lastPosition = m_players[no].m_piece.m_position;
			bool hasMoved = false;
			if (sf::Keyboard::isKeyPressed(m_players[no].m_up))
			{
				hasMoved = SideMoveOnKeyHold(m_players[no].m_up, EMoveDirection::UP, m_players[no]);
			}
			if (sf::Keyboard::isKeyPressed(m_players[no].m_down))
			{
				hasMoved = SideMoveOnKeyHold(m_players[no].m_down, EMoveDirection::DOWN, m_players[no]);
			}
			if (sf::Keyboard::isKeyPressed(m_players[no].m_lower))
			{
				hasMoved = SideMoveOnKeyHold(m_players[no].m_lower, EMoveDirection::LOWER, m_players[no]);
				m_players[no].m_piece.ResetFallTime();
			}

			//HardDrop
			bool performedHardDrop = false;
			if (OnKeyPress(m_players[no].m_hardDrop))
			{
				if (m_players[no].m_piece.m_position.x < m_board.m_separatorPosition)
				{
					for (int i = 0; i < m_board.m_matrixSize.x; ++i)
					{
						m_players[no].m_piece.m_position.x++;
						if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) != Board::ECollisions::None)
						{
							m_players[no].m_piece.m_position.x--;
						}
					}
				}
				else
				{
					for (int i = 0; i < m_board.m_matrixSize.x; ++i)
					{
						m_players[no].m_piece.m_position.x--;
						if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) != Board::ECollisions::None)
						{
							m_players[no].m_piece.m_position.x++;
						}
					}
				}
				performedHardDrop = true;
			}

			if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) != Board::ECollisions::None)
			{
				m_players[no].m_piece.m_position = lastPosition;
			}
			lastPosition = m_players[no].m_piece.m_position;

			m_players[no].m_piece.Update(dt);
			if (m_players[no].m_piece.m_lower)
			{
				if (m_players[no].m_piece.m_position.x < m_board.m_separatorPosition)
					m_players[no].m_piece.m_position.x++;
				else
					m_players[no].m_piece.m_position.x--;
			}
			if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) != Board::ECollisions::None)
			{
				m_players[no].m_piece.m_position = lastPosition;
			}

			if (!performedHardDrop)
			{
				if (IsPieceSeated(m_players[no]))
				{
					m_players[no].m_piece.AddTimeSeated(dt);
				}
			}

			if (m_players[no].m_piece.GetTimeSeated() >= m_players[no].m_piece.m_maxTimeSeated || performedHardDrop)
			{
				m_board.Imprint(m_players[no].m_piece);
				SpawnPiece(m_players[no]);
				if (ComputeCollisions(m_players[no], m_players[no].m_pieceSpawnPosition) == Board::ECollisions::Piece)
				{
					if (m_gameOver)
					{
						std::cout << "Game Over!" << std::endl;
						m_currentState = EStates::GAMEEND;
					}
				}
			}

			for (int i = 0; i < m_board.m_matrixSize.x; ++i)
			{
				if (CanLineBeCleared(i))
				{
					ClearLine(i);
					DropDownTheLinesAboveClearedLine(i);
					PushUpTheLinesAboveSeparator(i, m_players[1-no]);
					DropDownTheLinesAboveSeparator(i);
				}
			}
		}
	}

	case GAMEEND:
	{
	
	}

	}
}

void Game::Draw()
{
	switch (m_currentState)
	{
	case SPLASHSCREEN:
	{

	}

	case MENU:
	{

	}

	case GAME:
	{
		m_window.draw(m_backgroundSprite);
		m_board.Draw(m_window);
		for (int no = 0; no < m_nrPlayers; ++no)
			m_players[no].m_piece.Draw(m_window);
		
	}

	case GAMEEND:
	{

	}

	}
}
