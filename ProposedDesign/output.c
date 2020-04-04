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
