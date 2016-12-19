#pragma once

struct GrassInfo;

struct GrassDensityMap
{
	GrassDensityMap();

	void paint(float x, float y, float w, float size);

	void fill(float value);
	void resize(float minX, float maxX, float minY, float maxY);

	void preserveOriginal(bool enable);
	void apply(GrassInfo& grass);

	float get(float x, float y);
	void deinit();

private:

	bool preserve = false;

	struct WorldGrid
	{
		void init(float minX, float maxX, float minY, float maxY);
		bool inside(float x, float y);
		float& read(float x, float y);
		void fill(float value);

		float minX, maxX, minY, maxY;
		int rows, cols;
		float* data = nullptr;
		int pixelsPerUnit = 2;
	}
	grid;

};