#include <stdio.h>
#include <stdlib.h>
#include "output.h"

int CreateBitmap(FILE *bitmap, char **Lawn, parameters *Param)
	{
	//tab will be used to store unique values of watering on pixels
	char *tab = malloc(4 * sizeof(*tab));
	if(tab == NULL)
		return 0;
	int size = 4;
	int id = 0;
	//> mean is bad lawn
	int mean = (int)Param->currentmean;
	*tab = **Lawn;
	++id;
	//iterate through Lawn and create tab
	for(int j = 0; j < Param->ysize; j++)
		{
		for(int i = 0; i < Param->xsize; i++)
			{
			//need for reallocation for tab
			if(id == size)
				{
				tab = realloc(tab, 2*size*sizeof(*tab));
				if(tab == NULL)
					return 0;
				size *= 2;
				}
			char tofind = *(*(Lawn+i)+j);
			int l = 0;
			int k = id - 1;
			int cen;
			//check whether currently considered pixel is unique
			while(l < k)
				{
				cen = (k+l)/2;
				if(*(tab+cen) < tofind)
					l = cen + 1;
				else
					k = cen;
				}
			//if value is unique insert it into tab
			if(*(tab+l) != tofind)//TODO: Can do better - binary insertion
				{
				int a = 0;
				while(a < id && *(tab+a) < tofind)
					++a;
				if(a == id)
					*(tab+a) = tofind;
				else
					{
					while(a < id)
						{
						char piv = *(tab+a);
						*(tab+a) = tofind;
						tofind = piv;
						++a;
						}
					*(tab+a) = tofind;
					}
				++id;
				}			
			}
		}
	tab = realloc(tab, id*sizeof(*tab));
	if(InitializeBitmap(bitmap, Param->xsize, Param->ysize) == 0)
		return 0;
	//create bitmap file
	for(int jj = Param->ysize - 1; jj >= 0; --jj)
		{
		for(int ii = 0; ii < Param->xsize; ++ii)
			{
			//wall
			if(*(*(Lawn+ii)+jj) == 0)
				PixelLawn(bitmap, 'x', 0);
			//pixel wasn't watered
			else if(*(*(Lawn+ii)+jj) == 1)
				PixelLawn(bitmap, 'n', 0);
			else//watered
				{
				int z = 0;
				while(z < id)
					{
					//is overwatered or not?
					if(*(*(Lawn+ii)+jj) == *(tab+z))
						{
						if(*(tab+z) > mean)
							PixelLawn(bitmap, 'o', z+2);
						else
							PixelLawn(bitmap, 'w', z+2);
						z = id;
						}
					++z;
					}
				}
			}
		}
	return 1;
	}

void CreateOutputFile(FILE *output, sprlist *Sprinklers)
{
    	sprlist *last = Sprinklers;
   	int cnt = 0;
 	int type;
 	int x;
 	int y;

    	while(last != NULL)
        {
            last = last->next;
            ++cnt;
        }

    	fprintf(output, "Liczba wszytskich podlewaczek: %d\n\n", cnt-1);
    	cnt = 0;
    	last = Sprinklers;

    	while(last != NULL)
        {
        if(cnt != 0)
            {
                type = last->type;
                x = last->x;
                y = last->y;
                if(type == 90)
                  fprintf(output, "typ: %d,  koordynaty: (%d, %d) \n", type, x, y);
                else
                  fprintf(output, "typ: %d, koordynaty: (%d, %d) \n", type, x, y);
            }
        	last = last->next;
        	if(cnt == 0)
            	{
                    ++cnt;
            	}
        }
}
