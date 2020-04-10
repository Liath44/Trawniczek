#include <stdio.h>
#include <stdlib.h>
#define height 25
#define width 20

void setpixel(int **arr, int y, int x, int lvl)
{
arr[y][x]=lvl;
}

void deg90(int **lawn, int y, int x, int y0, int x0, int deg, int time)
{
    int lvl = 4*time;
    switch (deg)
        {
            case 4:
                {
                    if(x>-1 && y>-1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 3:
                {
                    if(x<1 && y>-1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 2:
                {
                    if(x<1 && y<1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 1:
                {
                    if(x>-1 && y<1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
        }
}

void deg180(int **lawn, int y, int x, int y0, int x0, int deg, int time)
{
    int lvl = 3*time;
    switch (deg)
        {
            case 12:
                {
                    if(y<1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 23:
                {
                    if(x<1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 34:
                {
                    if(y>-1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 41:
                {
                    if(x>-1)
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
        }
}

void deg270(int **lawn, int y, int x, int y0, int x0, int deg, int time)
{
    int lvl = 2*time;
    switch (deg)
        {
            case 4:
                {
                    if(!(x>-1 && y>-1))
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 3:
                {
                    if(!(x<1 && y>-1))
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 2:
                {
                    if(!(x<1 && y<1))
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
            case 1:
                {
                    if(!(x>-1 && y<1))
                    {
                        setpixel(lawn, y0, x0, lvl);
                    }
                    break;
                }
        }
}

void deg360(int **lawn, int y0, int x0, int time)
{
    int lvl = time;
    setpixel(lawn, y0, x0, lvl);

}

void draw(int **lawn, int y0, int x0, int radius, int type, int deg, int time)
{
    for(int y=-radius; y<=radius; y++)
		for(int x=-radius; x<=radius; x++)
			if(x*x+y*y <= radius*radius)
            {
				switch (type)
				{
                    case 90:
                        {
                            deg90(lawn, y, x, y0+y, x0+x, deg, time);
                            break;
                        }
                    case 180:
                        {
                            deg180(lawn, y, x, y0+y, x0+x, deg, time);
                            break;
                        }
                    case 270:
                        {
                            deg270(lawn, y, x, y0+y, x0+x, deg, time);
                            break;
                        }
                    case 360:
                        {
                            deg360(lawn, y0+y, x0+x, time);
                            break;
                        }
				}
			}
}

int main ()
{
int **lawn;
lawn = malloc(sizeof(*lawn)*width);
int i, j;

for (j = 0; j < width; j++)
        lawn[j] = malloc(sizeof(*lawn[0])*height);


	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			lawn[i][j] = 0;


	int x0 = 8;
	int y0 = 8;

	int radius;
    int type = 270;
    switch (type)
				{
                    case 90:
                        {
                            radius = 4;
                            break;
                        }
                    case 180:
                        {
                            radius = 4;
                            break;
                        }
                    case 270:
                        {
                            radius = 4;
                            break;
                        }
                    case 360:
                        {
                            radius = 4;
                            break;
                        }
				}
    int time = 1; //liczba cykli dla 360
    int deg = 1; //nr cwiartki która jest: 90 zajeta, 270 pusta, 180 12 to 1 i 2 zajeta
    draw(lawn, y0, x0, radius, type, deg, time);

	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
            {
			printf(" %d", lawn[i][j]);
			if (j == height-1)
			{
				printf("\n");
			}
		}

for (j = 0; j < width; j++)
        free(lawn[j]);

free(lawn);

return 0;
}
