#include "Piece.h"
#include "Utilities.h"

void Piece::LoadFromFile()
{
	int x = -1;
	int nrPieces = 0;
	FILE* f;
	fopen_s(&f, "pieces.txt", "r");
	if (f != NULL)
	{
		fscanf_s(f, "%d", &nrPieces);
		for (int k = 0; k < nrPieces; ++k)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					fscanf_s(f, "%d", &x);
					Storage[k][i][j] = x;
				}
			}
		}
		fclose(f);
	}
}

void Piece::LoadTextures()
{
	char* fileNames[COUNT] = { "Line.png", "Square.png", "T.png", "L.png", "J.png", "Z.png", "S.png" };
	char fileLocation[] = "Resources/Pieces/";
	char fullFileLocation[200];
	for (int i = 0; i < COUNT; ++i)
	{
		m_textures[i] = new sf::Texture();
		fullFileLocation[0] = 0;
		strcat_s(fullFileLocation, fileLocation);
		strcat_s(fullFileLocation, fileNames[i]);
		m_textures[i]->loadFromFile(fullFileLocation);
	}
}

sf::Texture& Piece::GetTexture(EType type)
{
	return *m_textures[type];
}

void Piece::AssignData(EType type)
{
	m_type = type;
	m_size = sf::Vector2i(4, 4);
	for (int line = 0; line < m_size.y; ++line)
	{
		for (int column = 0; column < m_size.x; ++column)
		{
			m_cells[line][column] = Piece::Storage[m_type][line][column];
		}
	}
	ComputeGeometricCenter();
}

void Piece::Init(sf::Vector2i position, EType type, sf::Vector2f cellGraphicalSize, sf::Vector2f boardGraphicalPosition)
{
	m_position = position;
	AssignData(type);
	
	m_fallTimeAccumulator = 0.0f;
	m_timeSeated = 0.0f;
	m_maxTimeSeated = 0.75f;
	m_maxFallTime = 1.0f;

	m_cellGraphicalSize	= cellGraphicalSize;
	m_boardGraphicalPosition = boardGraphicalPosition;

	m_lower = false;
}

void Piece::Rotate(bool clockwise)
{
	if (clockwise)
	{
		Utilities::RotateSquareMatrixClockwise((int*)m_cells, m_size.x);
	}
	else
	{
		Utilities::RotateSquareMatrixCounterClockwise((int*)m_cells, m_size.x);
	}
}

void Piece::ResetFallTime()
{
	m_fallTimeAccumulator = 0.0f;
}

void Piece::AddTimeSeated(float dt)
{
	m_timeSeated += dt;
}

void Piece::ResetTimeSeated()
{
	m_timeSeated = 0.0f;
}

float Piece::GetTimeSeated()
{
	return m_timeSeated;
}

void Piece::ComputeGeometricCenter()
{
	m_geometricCenter = sf::Vector2f(0.0f, 0.0f);
	int n = 0;
	for (int i = 0; i < m_size.x; ++i)
	{
		for (int j = 0; j < m_size.y; ++j)
		{
			if (m_cells[i][j] == 1)
			{
				n++;
				m_geometricCenter += sf::Vector2f(i + 0.5f, j + 0.5f);
			}
		}
	}
	m_geometricCenter /= (float)n;
}

void Piece::Update(float dt)
{
	m_fallTimeAccumulator += dt;
	m_lower = false;
	if (m_fallTimeAccumulator >= m_maxFallTime)
	{
		m_lower = true;
		m_fallTimeAccumulator = 0.0f;
	}
}

void Piece::Draw(sf::RenderTarget& renderTarget)
{
	sf::Sprite sprite;
	for (int i = 0; i < m_size.y; ++i)
	{
		for (int j = 0; j < m_size.x; ++j)
		{
			if (m_cells[i][j] == 1)
			{
				sprite.setTexture(GetTexture(m_type));
				sprite.setScale(m_cellGraphicalSize.x / sprite.getTexture()->getSize().x,
								m_cellGraphicalSize.y / sprite.getTexture()->getSize().y);
				sprite.setPosition(m_boardGraphicalPosition + 
					sf::Vector2f(m_position.x * m_cellGraphicalSize.x + j * m_cellGraphicalSize.y, 
								 m_position.y * m_cellGraphicalSize.y + i * m_cellGraphicalSize.y));
				renderTarget.draw(sprite);
			}
		}
	}
}

sf::Texture* Piece::m_textures[Piece::EType::COUNT];
int Piece::Storage[Piece::EType::COUNT][4][4];