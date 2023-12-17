#include "NoiseMap.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "FastNoiseLite.h"
#include <map>
#include <iostream>

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

    // Créer les voisins de chaque cellule
    for (const auto& region : regions) {
        for (const auto& cell : region) {
            std::vector<std::pair<unsigned, unsigned>> neighbors;
            int dx[] = {-1, 1, 0, 0};
            int dy[] = {0, 0, -1, 1};

            for (int i = 0; i < 4; ++i) {
                int newX = cell.first + dx[i];
                int newY = cell.second + dy[i];

                // Vérifier si le voisin est à l'intérieur des limites de la carte
                if (newX >= 0 && newX < nbR && newY >= 0 && newY < nbC)
                    neighbors.push_back({static_cast<unsigned>(newX), static_cast<unsigned>(newY)});
            }
            cellNeighbors[cell] = neighbors;
        }
    }

    //Peupler la liste des régions
    for(auto it : m)
       regions.push_back(it.second);

    // Supprimer les régions trop petites
    for (auto it = regions.begin(); it != regions.end();){
        if (it->size() < 15)
            it = regions.erase(it);
        else
            ++it;
    }

    std::cout << "NoiseMap created nb: " << m.size()  << std::endl;
    std::cout << "Ratio empty: " << getRatioEmpty(nbR, nbC, regions) *100 << std::endl;
    std::cout << "Nb components: " << getNbComponents(cellNeighbors) << std::endl;
}

NoiseMap::~NoiseMap(){
    regions.clear();
    cellNeighbors.clear();
}
