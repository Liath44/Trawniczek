//stdlib, output
#include "lawnholder.h"
#include <stdlib.h>

/*
	TODO:
 - Water missing in certain situations - i think i can find a place to place it
 - Merge some functions
	TODO:
*/

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
    	sprlist *temp = Sprinklers->next;
    	sprlist *rmv;
    	while(temp != NULL)
    	{
            rmv = temp;
       	    temp = temp->next;
            free(rmv);
   	}
}


double CalculateMean(char **Lawn, parameters *Param)
	{
	    double Mean = 0.0;
	    for (int i = 0; i < Param->ysize; i++)
        {
            for (int j = 0; j < Param->xsize; j++)
            {
                Mean += Lawn[i][j];
            }
        }
        Mean=Mean/Param->nlawn;
        return Mean;
	}

/*
 * Upadates Lawn north to the put sprinkler's center
 * 
 * char **Lawn - stores Lawn
 * 
 * int ysize - Lawn's vertical size
 * 
 * int x, y - coordinates of said sprinkler
 * 
 * int radius - radius of said sprinkler
 * 
 * int time - how long Lawn is watered
 * 
 * int water - number of sprinkler's rotations
 * 
 * Function's other parameters:
 * 
 * int i - used to count number of pixels to water
 * 
 * int yi - y coordinate of pixel to be watered
 * 
 * int direction - if water bounces direction changes. 
 * == 1 if water goes south, == -1 if water goes north
 */
void PutPlusUp(char **Lawn, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = y-1;
	int yi;
	int direction;
	//bounce immediately if wall
	if(y == 0 || *(*(Lawn+x)+y-1) == 0)
		{
		yi = y;
		direction = 1;
		}
	else
		{
		yi = y-1;
		direction = -1;
		}
	//basically counts how many pixels should we water
	while(i >= y-radius)
		{
		//yes - in that function we sprinkle two times
		*(*(Lawn+x)+yi) += 2*time*water;
		//should the direction be changed (bounce)?
		if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+x)+yi+direction) == 0)
			{
			direction *= -1;
			}
		else
			{
			yi += direction;
			}
		--i;
		}
	}

/*
 * Analogical to PutPlusUp(...) but updates Lawn south
 * to the put sprinkler's center
 */
void PutPlusDown(char **Lawn, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = y+1;
	int yi;
	int direction;
	//bounce immediately if wall
	if(y == ysize-1 || *(*(Lawn+x)+y+1) == 0)
		{
		yi = y;
		direction = -1;
		}
	else
		{
		yi = y+1;
		direction = 1;
		}
	//basically counts how many pixels should we water
	while(i <= y+radius)
		{
		//yes - in that function we sprinkle two times
		*(*(Lawn+x)+yi) += 2*time*water;
		//should the direction be changed (bounce)?
		if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+x)+yi+direction) == 0)
			{
			direction *= -1;
			}
		else
			{
			yi += direction;
			}
		++i;
		}
	}
/*
 * Analogical to PutPlusUp(...) but updates Lawn west
 * to the put sprinkler's center
 */
void PutPlusLeft(char **Lawn, int xsize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	int xi;
	int direction;
	//bounce immediately if wall
	if(x == 0 || *(*(Lawn+x-1)+y) == 0)
		{
		xi = x;
		direction = 1;
		}
	else
		{
		xi = x-1;
		direction = -1;
		}
	//basically counts how many pixels should we water
	while(i >= x-radius)
		{
		//yes - in that function we sprinkle two times
		*(*(Lawn+xi)+y) += 2*time*water;
		//should the direction be changed (bounce)?
		if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+y) == 0)
			{
			direction *= -1;
			}
		else
			{
			xi += direction;
			}
		--i;
		}
	}
/*
 * Analogical to PutPlusUp(...) but updates Lawn east
 * to the put sprinkler's center
 */
void PutPlusRight(char **Lawn, int xsize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	int xi;
	int direction;
	//bounce immediately if wall
	if(x+1 == xsize || *(*(Lawn+x+1)+y) == 0)
		{
		xi = x;
		direction = -1;
		}
	else
		{
		xi = x+1;
		direction = 1;
		}
	//basically counts how many pixels should we water
	while(i <= x+radius)
		{
		//yes - in that function we sprinkle two times
		*(*(Lawn+xi)+y) += 2*time*water;
		//should the direction be changed (bounce)?
		if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+y) == 0)
			{
			direction *= -1;
			}
		else
			{
			xi += direction;
			}
		++i;
		}
	}
/*
 * Updates Lawn's first quadrant from south to north
 * 
 * char **Lawn - stores the lawn
 * 
 * int xsize, ysize - Lawn's size
 * 
 * int x, y - coordinates of put sprinkler
 * 
 * int radius - radius of put sprinkler
 * 
 * int time - duration of watering
 * 
 * int water - number of sprinkler's rotations
 * 
 * Function's other parameters:
 *
 * int i - used to count which column to update 
 * 
 * int j - used to count pixels to update in column
 * 
 * int yi - y coordinate of pixel to be watered
 * 
 * int direction - if water bounces direction changes. 
 * == 1 if water goes south, == -1 if water goes north 
 */
void Quadrant1DownUp(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	//sprinkle towards north if you can (not a wall)
	while(i <= x + radius && i < xsize && *(*(Lawn+i)+y) != 0)
		{
		int j = y-1;
		int yi;
		int direction;
		//bounce immediately if wall
		if(y == 0 || *(*(Lawn+i)+y-1) == 0)
			{
			yi = y;
			direction = 1;
			}
		else
			{
			yi = y-1;
			direction = -1;
			}
		//sprinkling towards north inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
			//should the direction be changed (bounce)?
			if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+i)+yi+direction) == 0)
				{
				direction *= -1;
				}
			else
				{
				yi += direction;
				}
			--j;
			}
		++i;
		}
	}

/*
 * Analogical to Quadrant1DownUp(...) but updates
 * first quadrant from west to east
 */
void Quadrant1LeftRight(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y - 1;
	//sprinkle towards east if you can (not a wall)
	while(j >= y - radius && j >= 0 && *(*(Lawn+x)+j) != 0)
		{
		int i = x+1;
		int xi;
		int direction;
		//bounce immediately if wall
		if(x == xsize - 1 || *(*(Lawn+x+1)+j) == 0)
			{
			xi = x;
			direction = -1;
			}
		else
			{
			xi = x+1;
			direction = 1;
			}
		//sprinkling towards east inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
			//should the direction be changed (bounce)?
			if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+j) == 0)
				{
				direction *= -1;
				}
			else
				{
				xi += direction;
				}
			++i;
			}
		--j;
		}
	}
/*
 * Analogical to Quadrant1DownUp(...) but updates
 * second quadrant from south to north
 */
void Quadrant2DownUp(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	//sprinkle towards north if you can (not a wall)
	while(i >= x - radius && i >= 0 && *(*(Lawn+i)+y) != 0)
		{
		int j = y-1;
		int yi;
		int direction;
		//bounce immediately if wall
		if(y == 0 || *(*(Lawn+i)+y-1) == 0)
			{
			yi = y;
			direction = 1;
			}
		else
			{
			yi = y-1;
			direction = -1;
			}
		//sprinkling towards north inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
			//should the direction be changed (bounce)?
			if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+i)+yi+direction) == 0)
				{
				direction *= -1;
				}
			else
				{
				yi += direction;
				}
			--j;
			}
		--i;
		}
	}
/*
 * Analogical to Quadrant1DownUp(...) but updates
 * second quadrant from east to west
 */
void Quadrant2RightLeft(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y - 1;
	//sprinkle towards west if you can (not a wall)
	while(j >= y - radius && j >= 0 && *(*(Lawn+x)+j) != 0)
		{
		int i = x-1;
		int xi;
		int direction;
		//bounce immediately if wall
		if(x == 0 || *(*(Lawn+x-1)+j) == 0)
			{
			xi = x;
			direction = 1;
			}
		else
			{
			xi = x-1;
			direction = -1;
			}
		//sprinkling towards west inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
			//should the direction be changed (bounce)?
			if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+j) == 0)
				{
				direction *= -1;
				}
			else
				{
				xi += direction;
				}
			--i;
			}
		--j;
		}
	}
/*
 * Analogical to Quadrant1DownUp(...) but updates
 * third quadrant from north to south
 */
void Quadrant3UpDown(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	//sprinkle towards south if you can (not a wall)
	while(i >= x - radius && i >= 0 && *(*(Lawn+i)+y) != 0)
		{
		int j = y+1;
		int yi;
		int direction;
		//bounce immediately if wall
		if(y == ysize - 1 || *(*(Lawn+i)+y+1) == 0)
			{
			yi = y;
			direction = -1;
			}
		else
			{
			yi = y+1;
			direction = 1;
			}
		//sprinkling towards south inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
			//should the direction be changed (bounce)?
			if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+i)+yi+direction) == 0)
				{
				direction *= -1;
				}
			else
				{
				yi += direction;
				}
			++j;
			}
		--i;
		}
	}

/*
 * Analogical to Quadrant1DownUp(...) but updates
 * third quadrant from east to west
 */
void Quadrant3RightLeft(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y + 1;
	//sprinkle towards west if you can (not a wall)
	while(j <= y + radius && j < ysize && *(*(Lawn+x)+j) != 0)
		{
		int i = x-1;
		int xi;
		int direction;
		//bounce immediately if wall
		if(x == 0 || *(*(Lawn+x-1)+j) == 0)
			{
			xi = x;
			direction = 1;
			}
		else
			{
			xi = x-1;
			direction = -1;
			}
		//sprinkling towards west inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
			//should the direction be changed (bounce)?
			if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+j) == 0)
				{
				direction *= -1;
				}
			else
				{
				xi += direction;
				}
			--i;
			}
		++j;
		}
	}

/*
 * Analogical to Quadrant1DownUp(...) but updates
 * fourth quadrant from north to south
 */
void Quadrant4UpDown(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	//sprinkle towards south if you can (not a wall)
	while(i <= x + radius && i < xsize && *(*(Lawn+i)+y) != 0)
		{
		int j = y+1;
		int yi;
		int direction;
		//bounce immediately if wall
		if(y == ysize - 1 || *(*(Lawn+i)+y+1) == 0)
			{
			yi = y;
			direction = -1;
			}
		else
			{
			yi = y+1;
			direction = 1;
			}
		//sprinkling towards south inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
			//should the direction be changed (bounce)?
			if(yi+direction == -1 || yi+direction == ysize || *(*(Lawn+i)+yi+direction) == 0)
				{
				direction *= -1;
				}
			else
				{
				yi += direction;
				}
			++j;
			}
		++i;
		}
	}

/*
 * Analogical to Quadrant1DownUp(...) but updates
 * fourth quadrant from west to east
 */
void Quadrant4LeftRight(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y + 1;
	//sprinkle towards east if you can (not a wall)
	while(j <= y + radius && j < ysize && *(*(Lawn+x)+j) != 0)
		{
		int i = x+1;
		int xi;
		int direction;
		//bounce immediately if wall
		if(x == xsize - 1 || *(*(Lawn+x+1)+j) == 0)
			{
			xi = x;
			direction = -1;
			}
		else
			{
			xi = x+1;
			direction = 1;
			}
		//sprinkling towards east inside of a circle
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
			//should the direction be changed (bounce)?
			if(xi+direction == -1 || xi+direction == xsize || *(*(Lawn+xi+direction)+j) == 0)
				{
				direction *= -1;
				}
			else
				{
				xi += direction;
				}
			++i;
			}
		++j;
		}
	}
/*
 * Fills first quadrant of circle
 * 
 * Parameters explained in Quadrant1DownUp(...)
 */
void Quadrant1(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant1DownUp(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant1LeftRight(Lawn, xsize, ysize, x, y, radius, time, water);
	}

/*
 * Fills second quadrant of circle
 * 
 * Parameters explained in Quadrant1DownUp(...)
 */
void Quadrant2(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant2DownUp(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant2RightLeft(Lawn, xsize, ysize, x, y, radius, time, water);
	}

/*
 * Fills third quadrant of circle
 * 
 * Parameters explained in Quadrant1DownUp(...)
 */
void Quadrant3(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant3UpDown(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant3RightLeft(Lawn, xsize, ysize, x, y, radius, time, water);
	}

/*
 * Fills fourth quadrant of circle
 * 
 * Parameters explained in Quadrant1DownUp(...)
 */
void Quadrant4(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant4UpDown(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant4LeftRight(Lawn, xsize, ysize, x, y, radius, time, water);
	}

/*
 * Updates Lawn when 90 degree sprinkler was placed
 * 
 * char **Lawn - stores the lawn
 * 
 * int xsize, ysize - Lawn's size
 * 
 * int radius - radius of put sprinkler
 * 
 * int time - duration of watering
 * 
 * int x, y - coordinates of put sprinkler
 * 
 * int deg - sprinkler was set at an deg angle
 * 
 * int water - number of sprinkler's rotations
 */
void Put90(char **Lawn, int xsize, int ysize, int radius, int time, int x, int y, int deg, int water)
	{
	if(deg == 0)
		{
		PutPlusUp(Lawn, ysize, x, y, radius, time, water);
		PutPlusRight(Lawn, xsize, x, y, radius, time, water);
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 90)
		{
		PutPlusUp(Lawn, ysize, x, y, radius, time, water);
		PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 180)
		{
		PutPlusDown(Lawn, ysize, x, y, radius, time, water);
		PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 270)
		{
		PutPlusDown(Lawn, ysize, x, y, radius, time, water);
		PutPlusRight(Lawn, xsize, x, y, radius, time, water);
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	}

/*
 * Analogical to Put90 but updates Lawn when
 * 180 sprinkler was put
 */
void Put180(char **Lawn, int xsize, int ysize, int radius, int time, int x, int y, int deg, int water)
	{
	if(deg == 0)
		{
		PutPlusUp(Lawn, ysize, x, y, radius, time, water);
		PutPlusRight(Lawn, xsize, x, y, radius, time, water);
		PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 90)
		{
		PutPlusUp(Lawn, ysize, x, y, radius, time, water);
		PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
		PutPlusDown(Lawn, ysize, x, y, radius, time, water);
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 180)
		{
		PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
		PutPlusDown(Lawn, ysize, x, y, radius, time, water);
		PutPlusRight(Lawn, xsize, x, y, radius, time, water);
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 270)
		{
		PutPlusUp(Lawn, ysize, x, y, radius, time, water);
		PutPlusRight(Lawn, xsize, x, y, radius, time, water);
		PutPlusDown(Lawn, ysize, x, y, radius, time, water);
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	}

/*
 * Analogical to Put90 but updates Lawn when
 * 270 sprinkler was put
 */
void Put270(char **Lawn, int xsize, int ysize, int radius, int time, int x, int y, int deg, int water)
	{
	PutPlusUp(Lawn, ysize, x, y, radius, time, water);
	PutPlusDown(Lawn, ysize, x, y, radius, time, water);
	PutPlusRight(Lawn, xsize, x, y, radius, time, water);
	PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
	if(deg == 0)
		{
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 90)
		{
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 180)
		{
		Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	else if(deg == 270)
		{
		Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
		Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
		}
	}

/*
 * Analogical to Put90 but updates Lawn when
 * 360 sprinkler was put
 */
void Put360(char **Lawn, int xsize, int ysize, int radius, int time, int x, int y, int water)
	{
	PutPlusUp(Lawn, ysize, x, y, radius, time, water);
	PutPlusDown(Lawn, ysize, x, y, radius, time, water);
	PutPlusRight(Lawn, xsize, x, y, radius, time, water);
	PutPlusLeft(Lawn, xsize, x, y, radius, time, water);
	Quadrant1(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant2(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant3(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant4(Lawn, xsize, ysize, x, y, radius, time, water);
	}

int PutSprinkler(char **Lawn, int xsize, int ysize, int type, int radius, int time, int x, int y, int deg, sprlist *Sprinklers)
	{
	if(PutOnList(Sprinklers, type, x, y) == 0)
		return 0;
	type = (type-90)/90;
	*(*(Lawn+x)+y) += 2*time*(4-type);
	int water = 4-type;
	if(type == 0)
		Put90(Lawn, xsize, ysize, radius, time, x, y, deg, water);
	else if(type == 1)
		Put180(Lawn, xsize, ysize, radius, time, x, y, deg, water);
	else if(type == 2)
		Put270(Lawn, xsize, ysize, radius, time, x, y, deg, water);
	else if(type == 3)
		Put360(Lawn, xsize, ysize, radius, time, x, y, water);
	return 1;
	}
