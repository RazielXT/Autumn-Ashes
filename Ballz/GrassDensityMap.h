#pragma once

struct GrassInfo;

struct GrassDensityMap
{
	GrassDensityMap();

	void applyPaint(float x, float y, float w, float size);

	void fill(float value);
	void scale(float x, float y);

	void preserveOriginal(bool enable);
	void apply(GrassInfo& grass);

	float get(float x, float y);

private:

	float& read(float x, float y);

	const int pixelsPerUnit = 2;

	void allocate(float minX, float maxX, float minY, float maxY);

	bool preserve = false;
	float minX, maxX, minY, maxY;
	int rows, cols;
	float* data = nullptr;
};