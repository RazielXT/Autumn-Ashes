#include "stdafx.h"
#include "GrassDensityMap.h"
#include "GeometryManager.h"

GrassDensityMap::GrassDensityMap()
{
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

