#include "StrategieTest.h"
#include <iostream>
#include <vector>

StrategieTest::StrategieTest(unsigned int id, unsigned int nbPlayer, const SMap* map) :
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

StrategieTest::~StrategieTest()
{
	// Libération de la mémoire allouée pour les voisins de chaque cellule
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


bool StrategieTest::PlayTurn(unsigned int gameTurn, const SGameState* state, STurn* turn)
{
	// Met à jour les informations de la carte avec l'état actuel du jeu
	for (int i = 0; i < Map.nbCells; ++i) {
		Map.cells[i].infos = state->cells[i];
	}
	std::vector<pSCell> regionPlayable;
	// Parcours de toutes les cellules pour trouver les mouvements possibles
	for (int i = 0; i < Map.nbCells; ++i) {
		// Vérifie si la cellule appartient au joueur et a plus d'un dé
		if (Map.cells[i].infos.owner == Id && Map.cells[i].infos.nbDices > 1) {
			for (unsigned j = 0; j < Map.cells[i].nbNeighbors; ++j) {
				
				//if (Map.cells[i].neighbors[j]->infos.owner != Id && Map.cells[i].neighbors[j]->infos.nbDices < Map.cells[i].infos.nbDices) {
				
				// on vérifie que la case voisine n'est pas à nous et que soit la case voisine contient moins de dé que nous soit on a 8 dés ce qui nous permet d'attaquer à coup sur
				if (Map.cells[i].neighbors[j]->infos.owner != Id && (Map.cells[i].infos.nbDices == 8 || Map.cells[i].neighbors[j]->infos.nbDices < Map.cells[i].infos.nbDices)) {  
					
					if (Map.cells[i].infos.nbDices == 8 ) {  // si on a 8 dé attaquer 
						regionPlayable.push_back(&Map.cells[i]);
						//break;
					}
					
					if (isnotDangerous(*Map.cells[i].neighbors[j], Id, Map.cells[i].infos.nbDices - 1)) {//&& isnotDangerous(Map.cells[i], Id, 2)) { // si on décommente cette zone, on attaque que si la zone où l'on va laisser un seul dé est entouré de tour adverse de taille 2 max. On pourrais améliorer en disant que si la plus grande tour autour est à nous -> attaquer
						regionPlayable.push_back(&Map.cells[i]);
						//break;
					}		
					
				
			
				}
			}
		}
	}
	
	// Si aucune région n'est jouable, affiche un message et termine le tour
	if (regionPlayable.empty()) {
		std::cout << "fin du tour pas de coup possible" << std::endl;
		return false;
	}

	// Choix de la région de départ en fonction du nombre de dés
	auto startingRegionId = regionPlayable[0]->infos.id;
	for (auto& it : regionPlayable) {
		if (it->infos.nbDices > Map.cells[startingRegionId].infos.nbDices) {
			startingRegionId = it->infos.id;
		}
	}

	std::cout << "nombre de" << Map.cells[startingRegionId].infos.nbDices << std::endl;
	auto& startingRegion = Map.cells[startingRegionId];
	std::vector<pSCell> cellsAttackable;
	
	// Recherche de la cible attaquable
	for (unsigned i = 0; i < startingRegion.nbNeighbors; ++i) {
		if (startingRegion.neighbors[i]->infos.owner != Id) {
			cellsAttackable.push_back(startingRegion.neighbors[i]);
			break;
		}
	}
	
	// Si aucune cible attaquable, termine le tour
	if (cellsAttackable.empty()) {
		return false; 
	}
	
	// Choix de la cible en fonction du nombre de dés
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