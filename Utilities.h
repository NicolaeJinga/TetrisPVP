#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
namespace Utilities
{
	void RotateSquareMatrixClockwise(int* matrix, int n);
	void RotateSquareMatrixCounterClockwise(int* matrix, int n);
	float Distance(sf::Vector2f A, sf::Vector2f B);
}