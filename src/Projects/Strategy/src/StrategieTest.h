#pragma once

#include "../../../Commun/IStrategyLib.h"


class StrategieTest{
public:
    StrategieTest(unsigned int id, unsigned int nbPlayer, const SMap* map);
    StrategieTest(const StrategieTest &obj) =delete;
    StrategieTest(StrategieTest &&obj) =delete;
    virtual ~StrategieTest();
    StrategieTest& operator=(const StrategieTest &obj) =delete;
    StrategieTest& operator=(StrategieTest &&obj) =delete;

public:
    bool PlayTurn(unsigned int gameTurn, const SGameState *state, STurn *turn);

protected:
    const unsigned int Id;
    const unsigned int NbPlayer;
    SMap Map;


};