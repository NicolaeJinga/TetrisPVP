#include "Utilities.h"

namespace Utilities
{
	void RotateSquareMatrixClockwise(int* matrix, int n)
	{
		int* a = new int[n*n];
		memcpy(a, matrix, n*n*sizeof(int));
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				matrix[i*n + j] = a[(n - j - 1)*n + i];
		delete[] a;
	}
	void RotateSquareMatrixCounterClockwise(int* matrix, int n)
	{
		int* a = new int[n*n];
		memcpy(a, matrix, n*n*sizeof(int));
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				matrix[i*n + j] = a[j*n + (n - i - 1)];
		delete[] a;
	}
	float Distance(sf::Vector2f A, sf::Vector2f B)
	{
		return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
	}
}