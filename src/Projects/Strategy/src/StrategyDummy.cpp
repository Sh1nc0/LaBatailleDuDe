#include "StrategyDummy.h"
#include <iostream>
#include <vector>

StrategyDummy::StrategyDummy(unsigned int id, unsigned int nbPlayer, const SMap* map) :
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

StrategyDummy::~StrategyDummy()
{
	for (size_t i = 0; i < Map.nbCells; i++)
	{
		delete[] Map.cells[i].neighbors;
	}
	delete[] Map.cells;
}

bool StrategyDummy::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	for (int i = 0; i < Map.nbCells; ++i) {
		Map.cells[i].infos = state->cells[i];
	}
	std::vector<pSCell> regionPlayable;
	for (int i = 0; i < Map.nbCells; ++i) {
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
			for (unsigned j = 0; j < Map.cells[i].nbNeighbors; ++j) {
				if (Map.cells[i].neighbors[j]->infos.owner != Id) {
					regionPlayable.push_back(&Map.cells[i]);
					break;
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
