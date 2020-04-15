//stdlib, output
#include "lawnholder.h"

/*
	TODO:
 - Water missing in certain situations - i think i can find a place to place it
 - Merge some functions
	TODO:
*/

double CalculateMean(char **Lawn, parameters *Param)
	{
	return 1.0;
	}

//dwa razy podlać od razu
void PutPlusUp(char **Lawn, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = y-1;
	int yi;
	int direction;
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
	while(i >= y-radius)
		{
		*(*(Lawn+x)+yi) += 2*time*water;
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

void PutPlusDown(char **Lawn, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = y+1;
	int yi;
	int direction;
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
	while(i <= y+radius)
		{
		*(*(Lawn+x)+yi) += 2*time*water;
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

void PutPlusLeft(char **Lawn, int xsize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	int xi;
	int direction;
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
	while(i >= x-radius)
		{
		*(*(Lawn+xi)+y) += 2*time*water;
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

void PutPlusRight(char **Lawn, int xsize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	int xi;
	int direction;
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
	while(i <= x+radius)
		{
		*(*(Lawn+xi)+y) += 2*time*water;
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

void Quadrant1DownUp(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	while(i <= x + radius && i < xsize && *(*(Lawn+i)+y) != 0)
		{
		int j = y-1;
		int yi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
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

void Quadrant1LeftRight(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y - 1;
	while(j >= y - radius && j >= 0 && *(*(Lawn+x)+j) != 0)
		{
		int i = x+1;
		int xi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
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

void Quadrant2DownUp(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	while(i >= x - radius && i >= 0 && *(*(Lawn+i)+y) != 0)
		{
		int j = y-1;
		int yi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
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

void Quadrant2RightLeft(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y - 1;
	while(j >= y - radius && j >= 0 && *(*(Lawn+x)+j) != 0)
		{
		int i = x-1;
		int xi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
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

void Quadrant3UpDown(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x-1;
	while(i >= x - radius && i >= 0 && *(*(Lawn+i)+y) != 0)
		{
		int j = y+1;
		int yi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
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

void Quadrant3RightLeft(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y + 1;
	while(j <= y + radius && j < ysize && *(*(Lawn+x)+j) != 0)
		{
		int i = x-1;
		int xi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
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

void Quadrant4UpDown(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int i = x+1;
	while(i <= x + radius && i < xsize && *(*(Lawn+i)+y) != 0)
		{
		int j = y+1;
		int yi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+i)+yi) += time*water;
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

void Quadrant4LeftRight(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	int j = y + 1;
	while(j <= y + radius && j < ysize && *(*(Lawn+x)+j) != 0)
		{
		int i = x+1;
		int xi;
		int direction;
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
		while((x-i)*(x-i)+(y-j)*(y-j) <= radius*radius)
			{
			*(*(Lawn+xi)+j) += time*water;
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

void Quadrant1(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant1DownUp(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant1LeftRight(Lawn, xsize, ysize, x, y, radius, time, water);
	}

void Quadrant2(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant2DownUp(Lawn, xsize, ysize, x, y, radius, time, water);	
	Quadrant2RightLeft(Lawn, xsize, ysize, x, y, radius, time, water);
	}

void Quadrant3(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant3UpDown(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant3RightLeft(Lawn, xsize, ysize, x, y, radius, time, water);
	}

void Quadrant4(char **Lawn, int xsize, int ysize, int x, int y, int radius, int time, int water)
	{
	Quadrant4UpDown(Lawn, xsize, ysize, x, y, radius, time, water);
	Quadrant4LeftRight(Lawn, xsize, ysize, x, y, radius, time, water);
	}

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
