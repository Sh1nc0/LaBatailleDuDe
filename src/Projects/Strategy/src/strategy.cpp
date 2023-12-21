#include "interface_lib.h"
#include "Strategy.h"
#include <iostream>

API_EXPORT void InitStrategy(SInfo* info)
{
	std::cout << "InitStrategy" << std::endl;

	// ATTENTION NE PAS UTILISER D'ACCENT DANS LES CHAINES DE CARACTERES
	info->SetName("Strategie BigBoy");
	info->AddMember("Theo LE BAIL");
	info->AddMember("Thomas LEBRETON");
	info->AddMember("Romain PIPON");
	info->AddMember("Leo BRIGARDIS");
}

API_EXPORT IStrategy* StrategyFactory()
{
	return(new Strategy());
}

API_EXPORT void StrategyDelete(IStrategy* obj)
{
	delete(obj);
}

// ----------------------------------------------------------------------------

void Strategy::InitGame(unsigned int id, unsigned int nbPlayer, const SMap* map)
{
	if (Current) delete Current;
	Current = new StrategyBigBoy(id, nbPlayer, map);
}

bool Strategy::PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn)
{
	if (Current) return(Current->PlayTurn(gameTurn, state, turn));
	return(false);
}

void Strategy::EndGame(unsigned int /*idWinner*/)
{
	if (Current)
	{
		delete Current;
		Current = nullptr;
	}
}
