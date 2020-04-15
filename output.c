#include <stdio.h>
#include <stdlib.h>
#include "output.h"

int PutOnList(sprlist *Sprinklers, int type, int x, int y)
{
int error = 1;
sprlist *temp = malloc(sizeof(sprlist));
    if(temp == NULL)
        {
        error = 0;
        return error;
        }
temp->type = type;
temp->x = x;
temp->y = y;
temp->next = NULL;

sprlist *last = Sprinklers;

while(last->next != NULL)
        last = last->next;

last->next = temp;

return error;
}

void FreeSprinklers(sprlist *Sprinklers)
{
    sprlist *temp = Sprinklers;
    sprlist *rmv;
    while(temp != NULL)
    {
        rmv = temp;
        temp = temp->next;
        free(rmv);
    }
}

int CreateBitmap(FILE *bitmap, char **Lawn, parameters *Param)
{
    	int max = 16;
	uint32_t w1 = Param->xsize;
	uint32_t h1 = Param->ysize;
	int errorcode;
	errorcode = InitializeBitmap(b, w1, h1);
	
	if(errorcode = 0)
        {
            return 0;
        }
    
	for(int i = hl-1; i >= 0; i--)
        for (int j = wl-1; j >= 0; j--)
        {
            if(lawn[i][j] == 0)
            {
                PixelLawn(b, 'x', 0);
            }
            else if (lawn[i][j] == 1)
            {
                PixelLawn(b, 'n', 0);
            }
            else if (lawn[i][j] > 1 && lawn[i][j] < 12) 
            {
                PixelLawn(b, 'w', lawn[i][j]-1);
            }
            else 
            { 
                if(lawn[i][j] < max)
                    {
                         PixelLawn(b, 'o', lawn[i][j]-1);
                    }
                else
                    {
                        PixelLawn(b, 'o', max);
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
                  fprintf(output, "typ: %d,  współrzędne: (%d, %d) \n", type, x, y);
                else
                  fprintf(output, "typ: %d, współrzędne: (%d, %d) \n", type, x, y);
            }
        last = last->next;
        if(cnt == 0)
            {
                ++cnt;
            }
        }
}
