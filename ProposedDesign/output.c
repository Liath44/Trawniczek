#include "output.h"

int PutOnList(sprlist **Sprinklers, int type, int x, int y)
{
    sprlist *nw = malloc(sizeof *nw);
    if (nw == NULL)
    {
        printf(stderr, "Memory allocation error.\n");
        return 1;
    }
    nw->type = type;
    nw->x = x;
    nw->y = y;
    nw->next = NULL;
    if (*Sprinklers == NULL)
    {
        *Sprinklers = nw;
        return 0;
    }
    sprlist *tmp = *Sprinklets;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = nw;
    return 0;
}

//int CreateBitmap(FILE *bitmap, char **Lawn, parameters *Param)

void CreateOutputFile(FILE *output, sprlist *Sprinklers)
{
    int licz = 0;
    while(Sprinklers != NULL)
    {
        fptintf(output, "%3d: [%3d] in (%d, %d)\n",
                licz++, Sprinklers->type, Sprinklers->x, Sprinklers->y);
        Sprinklers = Sprinklers->next;
    }
}
