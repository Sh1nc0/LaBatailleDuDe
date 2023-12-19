#include "NoiseMap.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "FastNoiseLite.h"
#include <map>
#include <iostream>
#include "MapLoader.h"
#include "Voisin.h"
#include <algorithm>

float getRatioEmpty(unsigned int nbR, unsigned int nbC, Regions& regions){
    unsigned int nbCells = nbR * nbC;
    unsigned int nbEmpty = nbCells;
    for (auto it = regions.begin(); it != regions.end(); ++it){
        nbEmpty -= it->size();
    }
    return (float)nbEmpty / (float)nbCells;
}

int getNbComponents(std::map<std::pair<unsigned, unsigned>, std::vector<std::pair<unsigned, unsigned>>> cellNeighbors){
    std::map<std::pair<unsigned, unsigned>, bool> visited;
    int nbComponents = 0;
    for (auto it = cellNeighbors.begin(); it != cellNeighbors.end(); ++it){
        if (visited.find(it->first) == visited.end()){
            nbComponents++;
            std::vector<std::pair<unsigned, unsigned>> queue;
            queue.push_back(it->first);
            visited[it->first] = true;
            while (!queue.empty()){
                std::pair<unsigned, unsigned> cell = queue.back();
                queue.pop_back();
                for (auto it2 = cellNeighbors[cell].begin(); it2 != cellNeighbors[cell].end(); ++it2){
                    if (visited.find(*it2) == visited.end()){
                        queue.push_back(*it2);
                        visited[*it2] = true;
                    }
                }
            }
        }
    }
    return nbComponents;
}

void NoiseMap::updateNeighbors(const std::map<float, std::vector<std::pair<unsigned int, unsigned int>>>& m) {
    cellNeighbors.clear();
    for (const auto& region : m) {
        for (const auto& cell : region.second) {
            std::vector<std::pair<unsigned, unsigned>> neighbors;
            for (auto voisin : Voisin(cell.first, cell.second)) {
                if (voisin.first < 0 || voisin.second < 0 || voisin.first > nbR || voisin.second > nbC) continue;
                neighbors.emplace_back(voisin.first, voisin.second);
            }
            cellNeighbors[cell] = std::move(neighbors);
        }
    }
}

void NoiseMap::updateNeighbors(Regions &r){
    cellNeighbors.clear();
    for(const auto& region : regions){
        for(const auto& cell : region){
            std::vector<std::pair<unsigned, unsigned>> neighbors;
            for(auto voisin : Voisin(cell.first, cell.second)){
                if (voisin.first < 0 || voisin.second < 0 || (voisin.first > nbR) || (voisin.second > nbC)) continue;
                neighbors.push_back({ voisin.first, voisin.second });
            }
            cellNeighbors[cell] = neighbors;
        }
    }
}

NoiseMap::NoiseMap(unsigned int nbR, unsigned int nbC){
    this->nbR = nbR;
    this->nbC = nbC;

    srand (time(NULL));

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    noise.SetFrequency(0.2f);
    noise.SetSeed(rand() % 1000 + 1);
    noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
    noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);

    std::map<float, std::vector<std::pair<unsigned, unsigned>>> m;
    for (int x = 0; x < nbR; x++){
        for (int y = 0; y < nbC; y++){
            float n = noise.GetNoise((float)x, (float)y);
            if (m.find(n) == m.end()) 
                m[n] = std::vector<std::pair<unsigned, unsigned>>();
            m[n].push_back({x, y});
        }
    }

    updateNeighbors(m);

    //Peupler la liste des régions
    for(auto it : m)
       regions.push_back(it.second);


    //Supprimer les régions trop petites pour atteindre le ratio cible
    std::sort(regions.begin(), regions.end(), [](const std::vector<std::pair<unsigned, unsigned>>& a, const std::vector<std::pair<unsigned, unsigned>>& b) {
        return a.size() < b.size();
    });

    const float ratioTarget = static_cast<float>((rand() % 3 + 1) / 10.0);
    int offset = 0;

    while (getRatioEmpty(nbR, nbC, regions) < ratioTarget) {
        auto region = std::move(regions[offset]);
        regions.erase(regions.begin() + offset);

        auto oldNeighbors = cellNeighbors;
        cellNeighbors.find(region[0])->second.clear();
        for (auto cell : region) {
            cellNeighbors.erase(cell);
        }

        if (getNbComponents(cellNeighbors) > 1) {
            regions.push_back(std::move(region));
            offset++;
            cellNeighbors = std::move(oldNeighbors);
        }
    }

    std::cout << "NoiseMap created nb: " << m.size()  << std::endl;
    std::cout << "Ratio empty: " << getRatioEmpty(nbR, nbC, regions) *100 << std::endl;
    std::cout << "Nb components: " << getNbComponents(cellNeighbors) << std::endl;
}

NoiseMap::~NoiseMap(){
    regions.clear();
    cellNeighbors.clear();
}
