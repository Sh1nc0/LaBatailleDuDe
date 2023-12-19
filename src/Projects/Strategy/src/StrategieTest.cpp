#include "StrategieTest.h"
#include <iostream>
#include <vector>

StrategieTest::StrategieTest(unsigned int id, unsigned int nbPlayer, const SMap* map) :
    Id(id),
    NbPlayer(nbPlayer)
{
    Map.cells = new SCell[map->nbCells];
    Map.nbCells = map->nbCells;
    for (size_t i = 0; i < Map.nbCells; i++)
    {
        Map.cells[i].infos = map->cells[i].infos;
        Map.cells[i].nbNeighbors = map->cells[i].nbNeighbors;
        Map.cells[i].neighbors = new pSCell[map->cells[i].nbNeighbors];
        for (size_t j = 0; j < Map.cells[i].nbNeighbors; j++)
        {
            Map.cells[i].neighbors[j] = &Map.cells[map->cells[i].neighbors[j]->infos.id];
        }

    }
}

StrategieTest::~StrategieTest()
{
    for (size_t i = 0; i < Map.nbCells; i++)
    {
        delete[] Map.cells[i].neighbors;
    }
    delete[] Map.cells;
}

bool isnotDangerous(const SCell& cell, unsigned int id, unsigned int diceAmount) {
	for (unsigned i = 0; i < cell.nbNeighbors; ++i) {
		if (cell.neighbors[i]->infos.owner != id && cell.neighbors[i]->infos.nbDices > diceAmount) {
			return false;
		}
	}
	return true;
}


bool StrategieTest::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	for (int i = 0; i < Map.nbCells; ++i) {
		Map.cells[i].infos = state->cells[i];
	}
	std::vector<pSCell> regionPlayable;
	for (int i = 0; i < Map.nbCells; ++i) {
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
			for (unsigned j = 0; j < Map.cells[i].nbNeighbors; ++j) {
				//if (Map.cells[i].neighbors[j]->infos.owner != Id && Map.cells[i].neighbors[j]->infos.nbDices < Map.cells[i].infos.nbDices) {
				if (Map.cells[i].neighbors[j]->infos.owner != Id && (Map.cells[i].infos.nbDices == 8 || Map.cells[i].neighbors[j]->infos.nbDices < Map.cells[i].infos.nbDices)) {  // on v�rifie que la case voisine n'est pas � nous et que soit la case voisine contient moins de d� que nous soit on a 8 d�s ce qui nous permet d'attaquer � coup sur
					
					if (isnotDangerous(*Map.cells[i].neighbors[j], Id, Map.cells[i].infos.nbDices - 1)) {//&& isnotDangerous(Map.cells[i], Map.cells[i].infos.id, 1)) {
						regionPlayable.push_back(&Map.cells[i]);
						break;
					}		
				
				
			
				}
			}
		}
	}
	if (regionPlayable.empty()) {
		std::cout << "fin du tour pas de coup possible" << std::endl;
		return false;
	}

	auto startingRegionId = regionPlayable[0]->infos.id;
	for (auto& it : regionPlayable) {
		if (it->infos.nbDices > Map.cells[startingRegionId].infos.nbDices) {
			startingRegionId = it->infos.id;
		}
	}

	std::cout << "nombre de" << Map.cells[startingRegionId].infos.nbDices << std::endl;
	auto& startingRegion = Map.cells[startingRegionId];
	std::vector<pSCell> cellsAttackable;
	for (unsigned i = 0; i < startingRegion.nbNeighbors; ++i) {
		if (startingRegion.neighbors[i]->infos.owner != Id) {
			cellsAttackable.push_back(startingRegion.neighbors[i]);
			break;
		}
	}
	if (cellsAttackable.empty()) {
		return false; 
	}
	auto endingRegion = cellsAttackable[0]->infos.id;
	for (auto& it : cellsAttackable) {
		if (it->infos.nbDices > Map.cells[endingRegion].infos.nbDices) {
			endingRegion = it->infos.id;
		}
	}
	turn->cellFrom = startingRegionId;
	turn->cellTo = endingRegion;

	std::cout << "fin du tour" << std::endl;
	return true;
}