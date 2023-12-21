#pragma once

#include "../../../Commun/IStrategyLib.h"


class StrategyBigBoy{
public:
    StrategyBigBoy(unsigned int id, unsigned int nbPlayer, const SMap* map);
    StrategyBigBoy(const StrategyBigBoy &obj) =delete;
    StrategyBigBoy(StrategyBigBoy &&obj) =delete;
    virtual ~StrategyBigBoy();
    StrategyBigBoy& operator=(const StrategyBigBoy &obj) =delete;
    StrategyBigBoy& operator=(StrategyBigBoy &&obj) =delete;

public:
    bool PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn);

protected:
    const unsigned int Id;
    const unsigned int NbPlayer;
    SMap Map;


};