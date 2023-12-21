#include "StrategyBigBoy.h"
#include <iostream>
#include <vector>


//Rcuprer les infos d'une cellule  partir de son id
SCellInfo *RecupCelluleOwn(unsigned int id, const SGameState *ss) {

	for (unsigned int i = 0; i < ss->nbCells; ++i) {
		if (ss->cells[i].id == id) {
			return &ss->cells[i];
		}
	}

	return nullptr;
}


StrategyBigBoy::StrategyBigBoy(unsigned int id, unsigned int nbPlayer, const SMap* map) :
    Id(id),
    NbPlayer(nbPlayer)
{
	// Initialisation de la carte avec les informations de la carte initiale
    Map.cells = new SCell[map->nbCells];
    Map.nbCells = map->nbCells;
    for (size_t i = 0; i < Map.nbCells; i++)
    {
        Map.cells[i].infos = map->cells[i].infos;
        Map.cells[i].nbNeighbors = map->cells[i].nbNeighbors;
        Map.cells[i].neighbors = new pSCell[map->cells[i].nbNeighbors];
        for (size_t j = 0; j < Map.cells[i].nbNeighbors; j++)
        {
			// Lien des voisins de chaque cellule
			Map.cells[i].neighbors[j] = &Map.cells[map->cells[i].neighbors[j]->infos.id];
        }

    }
}

StrategyBigBoy::~StrategyBigBoy()
{
	// Lib�ration de la m�moire allou�e pour les voisins de chaque cellule
	for (size_t i = 0; i < Map.nbCells; i++)
    {
        delete[] Map.cells[i].neighbors;
    }
    delete[] Map.cells;
}

// Fonction qui retourne true si toute les case autour de la notre non pas plus de dés que nous 
bool isnotDangerous(const SCell& cell, unsigned int id, unsigned int diceAmount) {
	for (unsigned i = 0; i < cell.nbNeighbors; ++i) {
		if (cell.neighbors[i]->infos.owner != id && cell.neighbors[i]->infos.nbDices > diceAmount) {
			return false;
		}
	}
	return true;
}


bool StrategyBigBoy::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	// Met � jour les informations de la carte avec l'�tat actuel du jeu
	for (int i = 0; i < Map.nbCells; ++i) {
		Map.cells[i].infos = state->cells[i];
	}
	std::vector<pSCell> regionPlayable;
	// Parcours de toutes les cellules pour trouver les mouvements possibles
	for (int i = 0; i < Map.nbCells; ++i) {
		// Vérifie si la cellule appartient au joueur et a plus de 2 dé
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 2) {
			for (unsigned j = 0; j < Map.cells[i].nbNeighbors; ++j) {

				// on vérifie que la case voisine n'est pas à nous et que soit la case voisine contient moins de dé que nous
				if (Map.cells[i].neighbors[j]->infos.owner != Id && (Map.cells[i].infos.nbDices == 8 || Map.cells[i].neighbors[j]->infos.nbDices < Map.cells[i].infos.nbDices))
						regionPlayable.push_back(&Map.cells[i]);

			}
		}
	}

	if (regionPlayable.empty()) {
		std::cout << "fin du tour pas de coup possible" << std::endl;
		return false;
	}

	// Choix de la r�gion de d�part en fonction du nombre de d�s
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
		if (startingRegion.neighbors[i]->infos.owner != Id && (startingRegion.neighbors[i]->infos.nbDices < startingRegion.infos.nbDices)) {
			cellsAttackable.push_back(startingRegion.neighbors[i]);
			break;
		}
	}

	if (cellsAttackable.empty()) {
		return false; 
	}

	// On commence par attaquer la case qui a la diff�rence de dés la plus faible avec nous
	auto endingRegion = cellsAttackable[0]->infos.id;
	int score =1000;
	for (auto& it : cellsAttackable) {
		int temp = startingRegion.infos.nbDices - it->infos.nbDices-2;
		if ( temp < score) {
			score = temp;
			endingRegion = it->infos.id;
		}
	}

	turn->cellFrom = startingRegionId;
	turn->cellTo = endingRegion;

	std::cout << "fin du tour" << std::endl;
	return true;
}