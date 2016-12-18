#pragma once

struct GrassInfo;

struct GrassDensityMap
{
	GrassDensityMap();

	void applyPaint(float x, float y, float w, float size);

	void fill(float value);
	void allocate(float minX, float maxX, float minY, float maxY);

	void preserveOriginal(bool enable);
	void apply(GrassInfo& grass);

private:

	bool preserve = false;
	float minX, maxX, minY, maxY;
	float* data = nullptr;
};