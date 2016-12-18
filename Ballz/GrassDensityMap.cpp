#include "stdafx.h"
#include "GrassDensityMap.h"
#include "GeometryManager.h"

GrassDensityMap::GrassDensityMap()
{
}

void GrassDensityMap::applyPaint(float x, float y, float w, float size)
{
	/*for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			data[rows*w + h] += w;
		}
	}*/

	if (x < minX || y > maxY || x > maxX || y < minY)
		return;

	auto& f = read(x, y);
	f = std::max(0.f,std::min(1.f, f+w));
}

void GrassDensityMap::fill(float value)
{
	for (int i = 0; i < rows*cols; i++)
	{
		data[i] = value;
	}
}

void GrassDensityMap::scale(float x, float y)
{
	float centerX = (minX + maxX) / 2;
	float centerY = (minY + maxY) / 2;

	float sizeX = (centerX - minX)*x;
	float sizeY = (centerY - minY)*y;

	allocate(centerX - sizeX, centerX + sizeX, centerY - sizeY, centerY + sizeY);
}

void GrassDensityMap::allocate(float _minX, float _maxX, float _minY, float _maxY)
{
	minX = _minX;
	maxX = _maxX;
	minY = _minY;
	maxY = _maxY;

	rows = ((int)maxX - (int)minX)*pixelsPerUnit;
	cols = ((int)maxY - (int)minY)*pixelsPerUnit;

	rows = std::max(rows, 1);
	cols = std::max(cols, 1);

	data = new float[rows*cols];
}

void GrassDensityMap::preserveOriginal(bool enable)
{
	preserve = enable;
}

void GrassDensityMap::apply(GrassInfo& grass)
{
	auto buffer = grass.bake.layer->getColorMap()->getPixelBox();

	for (int h = 0; h < buffer.getHeight(); h++)
	{
		for (int w = 0; w < buffer.getWidth(); w++)
		{
			auto c = buffer.getColourAt(h, w, 0);

			c.a = c.a * (h > 200 || w > 200) ? 1 : 0;

			buffer.setColourAt(c, h, w, 0);
		}
	}
}

float GrassDensityMap::get(float x, float y)
{
	if (x < minX || y > maxY || x > maxX || y < minY)
		return 0;

	return read(x, y);
}

float& GrassDensityMap::read(float x, float y)
{
	int row = (int)x - (int)minX;
	int col = (int)y - (int)minY;

	return data[rows*col + row];
}
