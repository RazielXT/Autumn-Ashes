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
	"HeavyFall",
	"Pullup",
};