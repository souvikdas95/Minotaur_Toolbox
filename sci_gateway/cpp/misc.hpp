#ifndef MISCHEADER
#define MISCHEADER

#include <cstdlib>

template <class T> T* ArrConvertMajor(T* src, unsigned int rows, unsigned int cols)
{
	unsigned int i, j;
	T* dest = (T*)malloc(sizeof(T) * rows * cols);
	for(i = 0; i < rows; ++i)
	{
		for(j = 0; j < cols; ++j)
		{
			dest[i * cols + j] = src[j * rows + i];
		}
	}
	return dest;
}

#endif //MISCHEADER
