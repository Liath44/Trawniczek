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

    fclose(output);
}
