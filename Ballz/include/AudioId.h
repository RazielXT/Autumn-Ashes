#pragma once

enum AudioId
{
	WalkStone,
	WalkWood,
	WalkGrass,
	WalkMetal,
	//--------------
	StoneFall,
	WoodFall,
	GrassFall,
	MetalFall,
	//--------------
	MetalSlide,
	Freefall,
	HeavyFall,
	Pullup,
	//--------------
	Invalid
};

static const char* audioIdNames[] =
{
	"WalkStone",
	"WalkWood",
	"WalkGrass",
	"WalkMetal",

	"StoneFall",
	"WoodFall",
	"GrassFall",
	"MetalFall",

	"MetalSlide",
	"Freefall",
	"HeavyFall",
	"Pullup",
};