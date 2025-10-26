#include "global_variables.hpp"

tile_data *load_tile_data()
{
	
	tile_data *TILE_DATA=new tile_data[20];
	
	TILE_DATA[0].first=0;
	TILE_DATA[0].second={0, 0};
	
	TILE_DATA[1].first=1;
	TILE_DATA[1].second={0, 0};
	
	TILE_DATA[2].first=1;
	TILE_DATA[2].second={0, 0};
	
	TILE_DATA[3].first=0;
	TILE_DATA[3].second={0, 0};
	
	TILE_DATA[4].first=0;
	TILE_DATA[4].second={0, 0};
	
	TILE_DATA[5].first=0;
	TILE_DATA[5].second={0, 0};
	
	TILE_DATA[6].first=0;
	TILE_DATA[6].second={0, 0};
	
	
	//chmura
	TILE_DATA[7].first=0;
	TILE_DATA[7].second={0, 0};
	
	TILE_DATA[8].first=1;
	TILE_DATA[8].second={0, 0};
	
	TILE_DATA[9].first=0;
	TILE_DATA[9].second={0, 0};
	
	TILE_DATA[10].first=1;
	TILE_DATA[10].second={0, 0};
	
	return TILE_DATA;
	
}
