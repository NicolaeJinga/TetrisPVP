#pragma once
#include "SFML/Graphics.hpp"
#include "Piece.h"

class Board
{
public:
	int* m_cells;
	sf::Vector2i m_matrixSize;

	sf::Vector2f m_graphicSize;
	sf::Vector2f m_cellSize;
	sf::Vector2f m_graphicPosition;

	int m_separatorPosition;

	struct ECollisions
	{
		enum Enum
		{
			None = 0,
			Border = 1,
			Piece = 2,
			Separator = 4
		};
	};

	~Board();
	void Init(sf::Vector2i matrixSize, sf::Vector2f m_graphicPosition, sf::Vector2f m_graphicSize, int separatorPosition);
	int& At(sf::Vector2i position);

	void Imprint(Piece& piece);

	ECollisions::Enum ComputeCollision(Piece& piece, sf::Vector2i pieceSpawnPos);

	void Draw(sf::RenderTarget& renderTarget);
private:
	void DrawDesign(sf::RenderTarget&, sf::Vector2f position, sf::Vector2f size, int separatorPosition);
};