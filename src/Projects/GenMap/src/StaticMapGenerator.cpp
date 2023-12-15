#include "interface_lib.h"
#include "MapLoader.h"

API_EXPORT void InitMap(SInfo* info)
{
	info->SetName("Noise map");
	info->AddMember("LEBRETON Thomas");
	info->AddMember("LE BAIL Théo");
	info->AddMember("PIPON Romain");
	info->AddMember("BRIGARDIS Léo");
}

API_EXPORT IMap* MapFactory()
{
	IMap* map=new MapLoader();
	return(map);
}

API_EXPORT void MapDelete(IMap* obj)
{
	delete(obj);
}
