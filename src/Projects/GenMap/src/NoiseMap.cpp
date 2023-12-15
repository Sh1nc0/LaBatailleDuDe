#include "FastNoiseLite.h"
#include "NoiseMap.h"
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <map>

// Déclaration du type Regions

void LoadNoiseMap(Regions& regions, unsigned int r, unsigned int c) {
    regions.clear();
    srand(static_cast<unsigned int>(time(NULL))); // Seed pour la génération aléatoire

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise.SetFrequency(0.2f);
    noise.SetSeed(rand() % 1000 + 1);
    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);

    std::map<float, std::vector<std::pair<unsigned, unsigned>>> m;
    for (int x = 0; x < r; x++){
        for (int y = 0; y < c; y++){
            float n = noise.GetNoise((float)x, (float)y);
            if (m.find(n) == m.end()) m[n].push_back({x,y});
            m[n].push_back({ x,y });
        }
    }

    for(auto it : m){
        if(it.second.size() > 10) regions.push_back(it.second);
    }
}
