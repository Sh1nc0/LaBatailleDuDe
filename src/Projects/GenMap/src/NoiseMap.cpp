#include "FastNoiseLite.h"
#include "NoiseMap.h"
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <map>
#include <iostream>
#include "Voisin.h"
#include <set>
#include <algorithm>

int getNbComposantes(const Regions& region, const float* m, unsigned int r, unsigned int c)
{
    std::map<float, std::set<float>> connexion;
    for (auto cells : region)
    {
		for(auto cell : cells){
            float value = m[(cell.first * r) + cell.second];
            if(connexion.find(value) == connexion.end()){
                std::pair<float, std::set<float>> pair;
                pair.first = value;
                connexion.insert(pair);
            }
            for(auto voisin : Voisin(cell.first,cell.second))
            {
                if (voisin.first < 0 || voisin.second < 0 || (voisin.first>= r) || (voisin.second >= c)) continue;
                float valueNeighbor = m[(voisin.first * r) + voisin.second];
                if(valueNeighbor == value){
	                continue;
                }
                if(!connexion[value].contains(valueNeighbor))
                {
                    connexion[value].insert(valueNeighbor);
                }
            }
	    }
    }

    std::vector<std::set<float>*> composantes;
	//on parcours toutes les connexion entre les region pour créer les composantes
    for(auto pair : connexion)
    {
        std::set<float>* compToEdit = nullptr;
	    for (auto composante : composantes)
	    {
            if (composante->contains(pair.first))
            {
                compToEdit = composante;
                break;
            }
	    }
        if(compToEdit == nullptr)
        {
            compToEdit = new std::set<float>();
            compToEdit->insert(pair.first);
            composantes.push_back(compToEdit);
        }

		//on ajoute si il existe pas déjà tout les voisin de cette region à la composantes
        for(float neighborOfPair : pair.second)
        {
            if(!compToEdit->contains(neighborOfPair))
            {
				//on regarde dans les autres composantes si il y a déjà cette id
				bool alredyExist = false;
				int indexOfComposantes = 0;
				for (auto composante : composantes)
				{
					if (composante->contains(neighborOfPair))
					{
						alredyExist = true;
						// si oui on delete compToEdit pour le merge avec la composante
						for(float region : *compToEdit)
						{
							if(!composante->contains(region))
							{
								composante->insert(region);
							}
						}
						composantes.erase(std::remove(composantes.begin(), composantes.end(), compToEdit));
						delete compToEdit;
						compToEdit = composante;
						break;
					}
					indexOfComposantes++;
				}
				if(!alredyExist) compToEdit->insert(neighborOfPair);
			}

		}


	}
	int nbComposantes = composantes.size();
	for (auto compToDel : composantes) delete compToDel;
	return nbComposantes;
}



void LoadNoiseMap(Regions& regions, unsigned int r, unsigned int c) {
	regions.clear();
	srand(static_cast<unsigned int>(time(NULL))); // Seed pour la génération aléatoire

	FastNoiseLite noise;
	noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noise.SetFrequency(0.25f);
	noise.SetSeed(rand() % 1000 + 1);
	noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
	noise.SetCellularReturnType(FastNoiseLite::CellularReturnType_CellValue);
	std::map<float, std::vector<std::pair<unsigned, unsigned>>> m;


	float* cellToRegionId = new float[r * c];
	for (unsigned int x = 0; x < r; x++) {
		for (unsigned int y = 0; y < c; y++) {
			float n = noise.GetNoise((float)x, (float)y);
			cellToRegionId[(r * x) + y] = n;
			if (m.find(n) == m.end()) m[n].push_back({ x,y });
			m[n].push_back({ x,y });
		}
	}
	for (auto it : m) {
		regions.push_back(it.second);
	}


	std::sort(regions.begin(), regions.end(), [](std::vector <std::pair<unsigned int, unsigned int>>& a, std::vector<std::pair<unsigned int, unsigned int>>& b) {
		return a.size() < b.size();
	});


	int offset = 0;
	//todo stopé la boucle quand le ratio case vide case plein est bon
	for (int i = 0; i < 30; ++i){
		auto region = regions[offset];
		regions.erase(regions.begin() + offset);
		if(getNbComposantes(regions, cellToRegionId, r,c) != 1)
		{
			regions.push_back(region);
			break;
		}
	}
	std::cout << "nb composantes :" << getNbComposantes(regions, cellToRegionId, r, c) << std::endl;
	delete[] cellToRegionId;
}


