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
