#include "Board.h"

Board::~Board()
{
	delete[] m_cells;
}

void Board::Init(sf::Vector2i matrixSize, sf::Vector2f graphicPosition, sf::Vector2f graphicSize, int separatorPosition)
{
	m_matrixSize = matrixSize;
	m_cells = new int[m_matrixSize.x*m_matrixSize.y];
	for (int i = 0; i < m_matrixSize.x*m_matrixSize.y; ++i)
	{
		m_cells[i] = -1;
	}
	m_graphicPosition = graphicPosition;
	m_graphicSize = graphicSize;
	m_cellSize.x = m_graphicSize.x / matrixSize.x;
	m_cellSize.y = m_graphicSize.y / matrixSize.y;

	m_separatorPosition = separatorPosition;
}

int& Board::At(sf::Vector2i position)
{
	return m_cells[m_matrixSize.x * position.y + position.x];
}

void Board::Imprint(Piece& piece)
{
	sf::Vector2i position;
	for (int i = 0; i < piece.m_size.y; ++i)
	{
		for (int j = 0; j < piece.m_size.x; ++j)
		{
			position.x = j + piece.m_position.x;
			position.y = i + piece.m_position.y;
			if(position.y >= 0 && position.x >= 0 && position.y < m_matrixSize.y && position.x < m_matrixSize.x)
				if(At(position) == -1 && piece.m_cells[i][j] == 1)
					At(position) = piece.m_type;
		}
	}
}

Board::ECollisions::Enum Board::ComputeCollision(Piece& piece, sf::Vector2i pieceSpawnPos)
{
	sf::Vector2i position;
	ECollisions::Enum collision = ECollisions::None;
	for (int i = 0; i < piece.m_size.y; ++i)
	{
		for (int j = 0; j < piece.m_size.x; ++j)
		{
			position.x = j + piece.m_position.x;
			position.y = i + piece.m_position.y;
			if (piece.m_cells[i][j] == 1)
			{
				if (pieceSpawnPos.x < m_separatorPosition)
				{
					if (!(position.y >= 0 && position.x >= 0 && position.y < m_matrixSize.y))
					{
						collision = (ECollisions::Enum)(collision | ECollisions::Border);
					}
					if (!(position.x < m_separatorPosition))
					{
						collision = (ECollisions::Enum)(collision | ECollisions::Separator);
					}
					if (position.y >= 0 && position.x >= 0 && position.y < m_matrixSize.y && position.x < m_matrixSize.x)
					{
						if (At(position) != -1)
						{
							collision = (ECollisions::Enum)(collision | ECollisions::Piece);
						}
					}
				}
				else
				{
					if (!(position.y >= 0 && position.y < m_matrixSize.y && position.x < m_matrixSize.x))
					{
						collision = (ECollisions::Enum)(collision | ECollisions::Border);
					}
					if (!(position.x >= m_separatorPosition))
					{
						collision = (ECollisions::Enum)(collision | ECollisions::Separator);
					}
					if (position.y >= 0 && position.x >= 0 && position.y < m_matrixSize.y && position.x < m_matrixSize.x)
					{
						if (At(position) != -1)
						{
							collision = (ECollisions::Enum)(collision | ECollisions::Piece);
						}
					}
				}
			}
		}
	}
	return collision;
}

void Board::Draw(sf::RenderTarget& renderTarget)
{
	DrawDesign(renderTarget, m_graphicPosition, m_graphicSize, m_separatorPosition);

	sf::Sprite sprite;
	for (int i = 0; i < m_matrixSize.y; ++i)
	{
		for (int j = 0; j < m_matrixSize.x; ++j)
		{
			if ((Piece::EType)At(sf::Vector2i(j, i)) != -1)
			{
				sprite.setTexture(Piece::GetTexture((Piece::EType)At(sf::Vector2i(j, i))));
				sprite.setScale(m_cellSize.x / sprite.getTexture()->getSize().x, m_cellSize.y / sprite.getTexture()->getSize().y);
				sprite.setPosition(m_graphicPosition.x + j* m_cellSize.y, m_graphicPosition.y + i* m_cellSize.y);
				int value = 255;
				sprite.setColor(sf::Color(value, value, value, 255));
				renderTarget.draw(sprite);
			}
		}
	}
}

void Board::DrawDesign(sf::RenderTarget& renderTarget, sf::Vector2f position, sf::Vector2f size, int separatorCellPosition)
{
	//draw border outline
	sf::VertexArray border;
	sf::Color color = sf::Color::Black;
	border.append(sf::Vertex(position, color));
	border.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color));
	border.append(sf::Vertex(position + size, color));
	border.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color));
	border.append(sf::Vertex(position, color));
	border.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	renderTarget.draw(border);
	//draw background tint
	sf::VertexArray backgroundTint;
	color = sf::Color(0, 0, 0, 100);
	backgroundTint.append(sf::Vertex(position, color));
	backgroundTint.append(sf::Vertex(position + sf::Vector2f(size.x, 0.0f), color));
	backgroundTint.append(sf::Vertex(position + size, color));
	backgroundTint.append(sf::Vertex(position + sf::Vector2f(0.0f, size.y), color));
	backgroundTint.setPrimitiveType(sf::PrimitiveType::Quads);
	renderTarget.draw(backgroundTint);
	//draw border horizontal lines
	sf::VertexArray horizontalLines;
	color = sf::Color::Black;
	for (int i = 1; i < m_matrixSize.y; ++i)
	{
		horizontalLines.append(sf::Vertex(position + sf::Vector2f(0.0f, i*m_cellSize.y), color));
		horizontalLines.append(sf::Vertex(position + sf::Vector2f(m_graphicSize.x, i*m_cellSize.y), color));
	}
	horizontalLines.setPrimitiveType(sf::PrimitiveType::Lines);
	renderTarget.draw(horizontalLines);
	//draw border vertical lines
	sf::VertexArray verticalLines;
	color = sf::Color::Black;
	for (int i = 1; i < m_matrixSize.x; ++i)
	{
		verticalLines.append(sf::Vertex(position + sf::Vector2f(i*m_cellSize.x, 0.0f), color));
		verticalLines.append(sf::Vertex(position + sf::Vector2f(i*m_cellSize.x , m_graphicSize.y), color));
	}
	verticalLines.setPrimitiveType(sf::PrimitiveType::Lines);
	renderTarget.draw(verticalLines);
	//compute vertical separator position
	sf::Vector2f vertSeparatorPosition = sf::Vector2f(position.x + separatorCellPosition*m_cellSize.x, position.y);
	//draw vertical separator;
	sf::VertexArray vertSeparator;
	color = sf::Color::Red;
	vertSeparator.append(sf::Vertex(sf::Vector2f(vertSeparatorPosition.x, vertSeparatorPosition.y - 0.5f), color));
	vertSeparator.append(sf::Vertex(sf::Vector2f(vertSeparatorPosition.x, vertSeparatorPosition.y + 0.5f + m_graphicSize.y), color));
	vertSeparator.setPrimitiveType(sf::PrimitiveType::LinesStrip);
	renderTarget.draw(vertSeparator);
}