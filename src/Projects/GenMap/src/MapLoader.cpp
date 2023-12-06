#include "MapLoader.h"
#include <ctime>
#include <map>
#include "FastNoiseLite.h"

SRegions* MapLoader::GenerateMap(unsigned int& r, unsigned int& c)
{
    Regions regions;
    regions.clear();
    srand(time(NULL));
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise.SetFrequency(0.25f);
    noise.SetSeed(rand() % 1000 + 1);
    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);

    std::map<float, std::vector<std::pair<unsigned, unsigned>>> m;
    for (int x = 0; x < r; x++){
        for (int y = 0; y < c; y++){
            float n = noise.GetNoise((float)x, (float)y);
            m[n].push_back({ x,y });
        }
    }

    for(auto it : m){
        if(it.second.size() > 6)
        regions.push_back(it.second);
    }
    unsigned int nbR, nbC;
    SRegions* sregions = ConvertMap(regions, nbR, nbC);
    r = nbR;
    c = nbC;

    Maps.insert(sregions);

    return(sregions);
}

void MapLoader::DeleteMap(SRegions* regions)
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
			if (nbR < regions[i][j].first) nbR = regions[i][j].first;
			if (nbC < regions[i][j].second) nbC = regions[i][j].second;
			reg->region[i].cells[j].y = regions[i][j].first;
			reg->region[i].cells[j].x = regions[i][j].second;
		}
	}

	return(reg);
}
