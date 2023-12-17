
#include <vector>
#include "MapLoader.h"
#include <map>

class NoiseMap{
    private:
        unsigned int nbR;
        unsigned int nbC;
        Regions regions;
        std::map<std::pair<unsigned, unsigned>, std::vector<std::pair<unsigned, unsigned>>> cellNeighbors;


    public:
        NoiseMap(unsigned int nbR, unsigned int nbC);
        Regions getRegions(){return regions;}
        ~NoiseMap();

};