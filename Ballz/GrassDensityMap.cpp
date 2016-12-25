#include "stdafx.h"
#include "GrassDensityMap.h"
#include "GeometryManager.h"

GrassDensityMap::GrassDensityMap()
{
}

void GrassDensityMap::paint(float _x, float _y, float w, float size)
{
	float xStart = _x - size;
	float xEnd = _x + size;

	float yStart = _y - size;
	float yEnd = _y + size;

	float step = 1 / (float)grid.pixelsPerUnit;

	for (float x = xStart; x < xEnd; x+=step)
	{
		for (float y = yStart; y < yEnd; y+=step)
		{
			float distW = std::max(0.f, 1 - (Ogre::Vector2(x, y).distance(Ogre::Vector2(_x, _y)) / size));

			if (distW > 0 && grid.inside(x, y))
			{
				auto& f = grid.read(x, y);
				f = std::min(1.f, f + distW*w);
			}
		}
	}
}

void GrassDensityMap::fill(float value)
{
	grid.fill(value);
}

void GrassDensityMap::resize(GrassInfo& grass)
{
	auto old = grid;

	auto b = grass.bake.layer->mapBounds;
	grid.init(b.left, b.right, b.top, b.bottom);

	if (old.data == nullptr)
		copyDensity(grass);
	else
		grid.import(old);

	delete old.data;
}

void GrassDensityMap::relocate(GrassInfo& grass)
{
	auto b = grass.bake.layer->mapBounds;

	grid.minX = b.left;
	grid.maxX = b.right;
	grid.minY = b.top;
	grid.maxX = b.bottom;

	apply(grass);
}

void GrassDensityMap::copyDensity(GrassInfo& grass)
{
	auto buffer = grass.bake.layer->getColorMap()->getPixelBox();

	for (size_t h = 0; h < buffer.getHeight(); h++)
	{
		for (size_t w = 0; w < buffer.getWidth(); w++)
		{
			float xRel = h / (float)buffer.getHeight();
			float yRel = w / (float)buffer.getWidth();

			auto c = buffer.getColourAt(h, w, 0);

			float xPos = grid.minX + (grid.maxX - grid.minX)*xRel;
			float yPos = grid.minY + (grid.maxY - grid.minY)*yRel;

			if(grid.inside(xPos, yPos))
				grid.read(xPos, yPos) = c.a;
		}
	}
}

void GrassDensityMap::preserveOriginal(bool enable)
{
	preserve = enable;
}

void GrassDensityMap::apply(GrassInfo& grass)
{
	if (grid.data == nullptr)
		return;

	auto buffer = grass.bake.layer->getColorMap()->getPixelBox();

	for (size_t h = 0; h < buffer.getHeight(); h++)
	{
		for (size_t w = 0; w < buffer.getWidth(); w++)
		{
			float xRel = h / (float)buffer.getHeight();
			float yRel = w / (float)buffer.getWidth();

			auto c = buffer.getColourAt(h, w, 0);

			float xPos = grid.minX + (grid.maxX - grid.minX)*xRel;
			float yPos = grid.minY + (grid.maxY - grid.minY)*yRel;

			if(grid.inside(xPos, yPos))
				c.a = grid.read(xPos, yPos);

			buffer.setColourAt(c, h, w, 0);
		}
	}

	grass.pg->reloadGeometry();
	grass.pg->update();
}

float GrassDensityMap::get(float x, float y)
{
	if (!grid.inside(x,y))
		return 0;

	return grid.read(x, y);
}

void GrassDensityMap::deinit()
{
	delete grid.data;
}

bool GrassDensityMap::empty()
{
	return grid.data == nullptr;
}

float& GrassDensityMap::WorldGrid::read(float x, float y)
{
	int row = (int)x - (int)minX;
	int col = (int)y - (int)minY;

	return data[rows*col + row];
}

bool GrassDensityMap::WorldGrid::inside(float x, float y)
{
	return !(x < minX || y > maxY || x > maxX || y < minY);
}

void GrassDensityMap::WorldGrid::fill(float value)
{
	if(data)
		for (int i = 0; i < rows*cols; i++)
		{
			data[i] = value;
		}
}

void GrassDensityMap::WorldGrid::import(WorldGrid& from)
{
	float step = 1 / (float)pixelsPerUnit;

	for (int x = 0; x < rows; x++)
	{
		for (int y = 0; y < cols; y++)
		{
			float wpx = minX + x*step;
			float wpy = minY + y*step;

			if (from.inside(wpx, wpy))
			{
				data[rows*y + x] = from.read(wpx, wpy);
			}
		}
	}
}

void GrassDensityMap::WorldGrid::init(float _minX, float _maxX, float _minY, float _maxY)
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
	memset(data, 0, rows*cols*sizeof(float));
}