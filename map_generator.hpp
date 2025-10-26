void place_tree( int *MAP, int MAP_WIDTH, int X )
{
	
	int Y=0;
	
	while( MAP[Y*MAP_WIDTH+X]!=0 )
	{
		
		Y++;
		
	}
	
	int h=rand()%3+3;
	
	if( rand()%2 )
	{
		
		for(int i=3;i<=h+2;i++)
		{
		
			for(int j=0;j<i;j++)
			{
			
				MAP[(Y+h+5-i)*MAP_WIDTH+X+i-j-1-i/2]=4;
			
			}
		
		}
		
	}
	else
	{
		
		for(int i=3;i<=h+2;i++)
		{
		
			for(int j=0;j<i;j++)
			{
			
				MAP[(Y+h+5-i)*MAP_WIDTH+X+i-j-i/2-(rand()%3)]=4;
			
			}
		
		}
		
	}
	
	for(int i=0;i<h;i++)
	{
		
		MAP[Y*MAP_WIDTH+X]=3;
		Y++;
		
	}
	
	MAP[Y*MAP_WIDTH+X]=4;
	MAP[Y*MAP_WIDTH+X+1]=4;
	MAP[Y*MAP_WIDTH+X-1]=4;
	MAP[(Y-1)*MAP_WIDTH+X+1]=4;
	MAP[(Y-1)*MAP_WIDTH+X-1]=4;
	
	for(int i=1;i<=h-3;i++)
	{
		
		MAP[(Y-i-1)*MAP_WIDTH+X+1]=4;
		MAP[(Y-i-1)*MAP_WIDTH+X-1]=4;
		
	}
	
}

void place_flower( int *MAP, int MAP_WIDTH, int X )
{
	
	int Y=0;
	
	while( MAP[Y*MAP_WIDTH+X]!=0 )
	{
		
		Y++;
		
	}
	
	MAP[Y*MAP_WIDTH+X]=5;
	
	if( rand()%2 )
	{
		
		MAP[Y*MAP_WIDTH+X]=5;
		
	}
	else
	{
		
		MAP[Y*MAP_WIDTH+X]=6;
		
	}
	
	if( MAP[Y*MAP_WIDTH+X+1]==0 && MAP[Y*MAP_WIDTH+X+1-MAP_WIDTH]!=0 )
	{
		
		int los=rand()%3;
		
		if( los == 0 || los == 1 )
		{
		
			MAP[Y*MAP_WIDTH+X+1]=MAP[Y*MAP_WIDTH+X];
		
		}
		
		if( MAP[Y*MAP_WIDTH+X+2]==0 && MAP[Y*MAP_WIDTH+X+2-MAP_WIDTH]!=0 )
		{
			
			int los=rand()%3;
			
			if( los == 0 )
			{
			
				MAP[Y*MAP_WIDTH+X+2]=MAP[Y*MAP_WIDTH+X];
			
			}
		
		}
		
	}
	
}

void place_cloud( int *MAP, int MAP_WIDTH, int X )
{
	
	int Y=0;
	
	while( MAP[Y*MAP_WIDTH+X]!=0 )
	{
		
		Y++;
		
	}
	
	if( rand()%2 )
	{
		
		MAP[(Y+4)*MAP_WIDTH+X]=10;
		MAP[(Y+5)*MAP_WIDTH+X]=9;
		MAP[(Y+4)*MAP_WIDTH+X+1]=8;
		MAP[(Y+5)*MAP_WIDTH+X+1]=7;
		
	}
	else
	{
		
		MAP[(Y+5)*MAP_WIDTH+X]=10;
		MAP[(Y+6)*MAP_WIDTH+X]=9;
		MAP[(Y+5)*MAP_WIDTH+X+1]=8;
		MAP[(Y+6)*MAP_WIDTH+X+1]=7;
		
	}
	
}

int *generate_map(int MAP_HEIGHT, int MAP_WIDTH, int maxDH, int flatness=1, int plants=3)
{
	
	int MAP_SIZE=MAP_HEIGHT*MAP_WIDTH;
	int *MAP=new int[MAP_SIZE+1];
	
	for(int i=0;i<MAP_SIZE;i++)
	{
		
		MAP[i]=0;
		
	}
	
	int ac_height=MAP_HEIGHT/2;
	int delta_height;
	
	int TF=0;
	
	for(int i=0;i<MAP_WIDTH;i++)
	{
		
		if( rand()%flatness == 0 )
		{
			
			TF=0;
			delta_height=( rand() % 2*maxDH+1 ) - maxDH;
			
		}
		else
		{
			
			TF++;
			delta_height=0;
			
			if( ( TF>=5 ) )
			{
				
				if( rand()%plants == 0 )
				{
					
					TF=0;
				
					place_tree( MAP, MAP_WIDTH, i-2 );
					
				}
				else
				{
					
					if( rand()%2 == 0 )
					{
						
						TF=0;
						
						place_cloud( MAP, MAP_WIDTH, i-2 );
						
					}
					
				}
				
			}
			else
			{
				
				if( (rand()%5*plants == 0) && TF>=2 )
				{
					
					TF=0;
				
					place_flower( MAP, MAP_WIDTH, i-2 );
					
				}
				
			}
			
		}
		
		ac_height+=delta_height;
		
		int a=0;
		
		for(;a<ac_height;a++)
		{
			
			MAP[a*MAP_WIDTH+i]=1;
			
		}
		
		MAP[a*MAP_WIDTH+i]=2;
		
	}
	
	reverse(MAP, MAP+MAP_SIZE+1);
	
	return MAP;
	
}
