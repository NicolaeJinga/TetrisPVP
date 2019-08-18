#pragma once
#include "SFML/Graphics.hpp"

class Piece
{
public:
	enum EType
	{
		LINE = 0,
		SQUARE,
		T,
		L,
		J,
		Z,
		S,
		COUNT
	};

	static int Storage[COUNT][4][4];

	EType m_type;
	sf::Vector2i m_position;
	sf::Vector2i m_size;
	int m_cells[4][4];

	sf::Vector2f m_cellGraphicalSize;
	sf::Vector2f m_boardGraphicalPosition;

	sf::Vector2f m_geometricCenter;
	
	float m_fallTimeAccumulator;
	float m_timeSeated;

	float m_maxFallTime;
	float m_maxTimeSeated;

	bool m_lower;

	static sf::Texture* m_textures[COUNT];

	static void LoadFromFile();
	static void LoadTextures();
	static sf::Texture& GetTexture(EType type);
	void AssignData(EType type);
	void Init(sf::Vector2i position, EType type, sf::Vector2f cellGraphicalSize, sf::Vector2f boardGraphicalPosition);
	void Rotate(bool clockwise);
	void ResetFallTime();
	void AddTimeSeated(float dt);
	void ResetTimeSeated();
	float GetTimeSeated();
	void ComputeGeometricCenter();
	void Update(float dt);
	void Draw(sf::RenderTarget& renderTarget);
};
