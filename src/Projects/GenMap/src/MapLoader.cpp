#include "MapLoader.h"
#include "NoiseMap.h"

SRegions* MapLoader::GenerateMap(unsigned int& r, unsigned int& c)
{
	Regions regions;
	NoiseMap *map = new NoiseMap(r, c);
	regions = map->getRegions();
	unsigned int nbR, nbC;
	SRegions* sregions = ConvertMap(regions, nbR, nbC);
	map->getSize(nbR, nbC);
	r = nbR;
	c = nbC;

	Maps.insert(sregions);
	delete map;
	return(sregions);
}

void MapLoader::DeleteMap(SRegions* regions)
{
	Maps.erase(regions);
	DeleteMapWithoutSet(regions);
}

void MapLoader::DeleteMapWithoutSet(SRegions* regions)
{
	for (unsigned int i = 0; i < regions->nbRegions; ++i)
	{
		delete[](regions->region[i].cells);
	}
	delete[] regions->region;
	delete regions;
}

SRegions* MapLoader::ConvertMap(Regions& regions, unsigned int& nbR, unsigned int& nbC)
{
	SRegions* reg = new SRegions;

	nbR = 0;
	nbC = 0;
	reg->nbRegions = regions.size();
	reg->region = new SRegion[reg->nbRegions];
	for (unsigned int i = 0; i < reg->nbRegions; ++i)
	{
		reg->region[i].nbCells = regions[i].size();
		reg->region[i].cells = new SRegionCell[reg->region[i].nbCells];
		for (unsigned int j = 0; j < reg->region[i].nbCells; ++j)
		{
			if (nbR <= regions[i][j].first) nbR = regions[i][j].first + 1;
			if (nbC <= regions[i][j].second) nbC = regions[i][j].second + 1;
			reg->region[i].cells[j].y = regions[i][j].first;
			reg->region[i].cells[j].x = regions[i][j].second;
		}
	}

	return(reg);
}
