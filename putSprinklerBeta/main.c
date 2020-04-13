#include <stdio.h>
#include <stdlib.h>
#define height 15
#define width 22
#define r 1

void setpixel(char **lawn, int y, int x, int lvl)
{
    lawn[y][x]+=lvl;
}

void stripeHorPos(char **lawn, int y0, int x0, int lvl, int radius)
{
        int y=0;
		for(int x=1; x<=radius; x++)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                        if(lawn[y0-y][x0+x] == 0)
                        {
                        int mirror = x;
                        for(int i=x; i<=radius; i++)
                            if(i*i+y*y <= radius*radius)
                                {
                                setpixel(lawn, y0-y, x0+mirror+(mirror - i - 1), lvl);
                                }
                        break;
                        }
                    }
}

void stripeHorNeg(char **lawn, int y0, int x0, int lvl, int radius)
{
        int y=0;
		for(int x=-1; x>=-radius; x--)          // for(int x=-radius; x<0; x++)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                        if(lawn[y0-y][x0+x] == 0)
                        {
                        int mirror = x;
                        for(int i=x; i>=-radius; i--)
                            if(i*i+y*y <= radius*radius)
                                {
                                setpixel(lawn, y0-y, x0+mirror+(mirror - i + 1), lvl);
                                }
                        break;
                        }
                    }
}

void stripeVerPos(char **lawn, int y0, int x0, int lvl, int radius)
{
        int x=0;
        for(int y=1; y<=radius; y++)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                        if(lawn[y0-y][x0+x] == 0)
                        {
                        int mirror = y;
                        for(int i=y; i<=radius; i++)
                            if(i*i+x*x <= radius*radius)
                                {
                                setpixel(lawn, y0-mirror - (mirror - i - 1), x0, lvl);
                                }
                        break;
                        }
                    }
}

void stripeVerNeg(char **lawn, int y0, int x0, int lvl, int radius)
{
        int x=0;
        for(int y=-1; y>=-radius; y--)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                        if(lawn[y0-y][x0+x] == 0)
                        {
                        int mirror = y;
                        for(int i=y; i>=-radius; i--)
                            if(i*i+x*x <= radius*radius)
                                {
                                setpixel(lawn, y0-mirror - (mirror - i + 1), x0, lvl);
                                }
                        break;
                        }
                    }
}

void quadrant1(char **lawn, int y0, int x0, int lvl, int radius)
{
    int arr[14];
    int cnt = 0;
   //if(x>-1 && y>-1)
    for(int y=radius; y>0; y--)
		for(int x=1; x<=radius; x++)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] == 0)
                        {
                            int xh = x; //do tangensa jako gora
                            int yh = y;
                            arr[cnt]= xh;
                            arr[cnt+1] = yh
                            cnt = cnt + 2;
                            --y;
                            for(; y>0; y--)
                            {
                                if(lawn[y0-y][x0+x] == 0)
                                 {
                                    if(lawn[y0-y][x0+x+1] == 0 && lawn[y0-y][x0+x-1] != 0 && (y-1 > 1) && lawn[y0-y-1][x0+x] != 0)
                                      {
                                       int xm = x;
                                       int ym = y;
                                       for(; x<=radius; x++)
                                        {
                                            if(lawn[y0-y-1][x0+x] == 0 || x*x+y*y = radius*radius)
                                            {
                                                int xl = x;
                                                int yl = y;
                                                double tanh = yh/xh;
                                                double tanm = ym/xm;
                                                double tanl = ye/xe;
                                                for(int yy=radius; yy>0; yy--)
                                                    for(int xx=1; xx<=radius; xx++)
                                                        if(xx*xx+yy*yy <= radius*radius)
                                                        {
                                                         tanThis = yy/xx;
                                                         if(tanThis < tanm && tanThis >= tanl)
                                                            {
                                                                if(lawn[y0-yy][x0+xx] != 0 && yy <ym)
                                                                    {
                                                                        setpixel(lawn, y0-yy, x0+xx, lvl);
                                                                    }
                                                                if(yy >=ym)
                                                                    {
                                                                        int mirror = ym;
                                                                        setpixel(lawn, y0-mirror - (mirror - i - 1), x0-xx, lvl);
                                                                    }
                                                            }
                                                         if(tanThis < tanh && tanThis >= tanm)
                                                            {
                                                                if(lawn[y0-yy][x0+xx] != 0 && xx <xm)
                                                                    {
                                                                        setpixel(lawn, y0-yy, x0+xx, lvl);
                                                                    }
                                                                if(xx >=xm)
                                                                    {
                                                                        int mirror = xm;
                                                                        setpixel(lawn, y0-y, x0+mirror+(mirror - i - 1), lvl);
                                                                    }
                                                            }
                                                        }
                                                break;
                                            }
                                        }
                                       break;
                                      }

                                    if(lawn[y0-y][x0+x-1] == 0)
                                      {
                                       for(; x>=1; x--)
                                        {
                                            if(x=1)
                                            {
                                               int mirror = 1;
                                               for(int yy=radius; yy>0; yy--)
                                                    for(int xx=1; xx<=radius; xx++)
                                                        if(xx*xx+yy*yy <= radius*radius)
                                                        {
                                                            setpixel(lawn, y0-y, x0+mirror+(mirror - i - 1), lvl);
                                                        }
                                                break;
                                            }
                                            if(lawn[y0-y-1][x0+x] == 0 && lawn[y0-y][x0+x-1] != 0)
                                            {
                                                int xl = x;
                                                int yl = y;
                                                double tanh = yh/xh;
                                                double tanl = yl/xl;
                                                for(int yy=radius; yy>0; yy--)
                                                    for(int xx=1; xx<=radius; xx++)
                                                        if(xx*xx+yy*yy <= radius*radius)
                                                        {
                                                         tanThis = yy/xx;
                                                         if(tanThis < tanh && tanThis >= tanl)
                                                            {
                                                                if(lawn[y0-yy][x0+xx] != 0 && xx <xh)
                                                                    {
                                                                        setpixel(lawn, y0-yy, x0+xx, lvl);
                                                                    }
                                                                if(xx >=xh)
                                                                    {
                                                                        int mirror = xh;
                                                                        setpixel(lawn, y0-y, x0+mirror+(mirror - i - 1), lvl);
                                                                    }
                                                            }
                                                        }
                                                break;
                                            }
                                        }
                                       break;
                                      }
                                    if(y=1)   //sciana ||||
                                      {
                                        //odbicie OY
                                       break;
                                      }
                                 }
                                if(lawn[y0-y][x0+x] != 0)
                                 {
                                    ++y;
                                    xh = x;
                                    yh = y;
                                    for(; x<radius; x++)
                                        {
                                            if(lawn[y0-y-1][x0+x] == 0 || x*x+y*y = radius*radius)  //sciana pozioma ________
                                            {
                                                //funkcja
                                                break;
                                            }
                                        }
                                    break;
                                 }
                            }
                           break;
                        }
                    }
}

void quadrant2(char **lawn, int y0, int x0, int lvl, int radius)
{
 //if(x<1 && y>-1)
    for(int y=radius; y>0; y--)
		for(int x=-1; x>=-radius; x--)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                    }
}

void quadrant3(char **lawn, int y0, int x0, int lvl, int radius)
{
//if(x<1 && y<1)
    for(int y=-radius; y<0; y++)
		for(int x=-1; x>=-radius; x--)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                    }
}

void quadrant4(char **lawn, int y0, int x0, int lvl, int radius)
{
//if(x>-1 && y<1)
    for(int y=-radius+1; y<0; y++)
		for(int x=1; x<radius; x++)
			if(x*x+y*y <= radius*radius)
                    {
                        if(lawn[y0-y][x0+x] != 0)
                        {
                        setpixel(lawn, y0-y, x0+x, lvl);
                        }
                    }
}



void deg90(char **lawn, int y0, int x0, int deg, int time, int radius)
{
    int lvl = 4*time;
    lawn[y0][x0]+=lvl;
    switch (deg)
        {
            case 4:
                {
                    quadrant4(lawn, y0, x0, lvl, radius);
                    stripeHorPos(lawn, y0, x0, lvl, radius);
                    stripeVerNeg(lawn, y0, x0, lvl, radius);
                    break;
                }
            case 3:
                {
                   quadrant3(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 2:
                {
                   quadrant2(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 1:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   break;
                }
        }
}

void deg180(char **lawn, int y0, int x0, int deg, int time, int radius)
{
    int lvl = 3*time;
    lawn[y0][x0]+=lvl;
    switch (deg)
        {
            case 12:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   quadrant2(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 23:
                {
                   quadrant2(lawn, y0, x0, lvl, radius);
                   quadrant3(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 34:
                {
                   quadrant3(lawn, y0, x0, lvl, radius);
                   quadrant4(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 14:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   quadrant4(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
        }
}

void deg270(char **lawn, int y0, int x0, int deg, int time, int radius)
{
    int lvl = 2*time;
    lawn[y0][x0]+=lvl;
    switch (deg)
        {
            case 4:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   quadrant2(lawn, y0, x0, lvl, radius);
                   quadrant3(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 3:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   quadrant2(lawn, y0, x0, lvl, radius);
                   quadrant4(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 2:
                {
                   quadrant1(lawn, y0, x0, lvl, radius);
                   quadrant3(lawn, y0, x0, lvl, radius);
                   quadrant4(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
            case 1:
                {
                   quadrant2(lawn, y0, x0, lvl, radius);
                   quadrant3(lawn, y0, x0, lvl, radius);
                   quadrant4(lawn, y0, x0, lvl, radius);
                   stripeHorPos(lawn, y0, x0, lvl, radius);
                   stripeHorNeg(lawn, y0, x0, lvl, radius);
                   stripeVerPos(lawn, y0, x0, lvl, radius);
                   stripeVerNeg(lawn, y0, x0, lvl, radius);
                   break;
                }
        }
}

void deg360(char **lawn, int y0, int x0, int time, int radius)
{
    int lvl = time;
    lawn[y0][x0]+=lvl;
    quadrant1(lawn, y0, x0, lvl, radius);
    quadrant2(lawn, y0, x0, lvl, radius);
    quadrant3(lawn, y0, x0, lvl, radius);
    quadrant4(lawn, y0, x0, lvl, radius);
    stripeHorPos(lawn, y0, x0, lvl, radius);
    stripeHorNeg(lawn, y0, x0, lvl, radius);
    stripeVerPos(lawn, y0, x0, lvl, radius);
    stripeVerNeg(lawn, y0, x0, lvl, radius);
}

void typedeg(char **lawn, int y0, int x0, int type, int deg, int time)
{
    int radius;
    switch (type)
        {
            case 90:
                {
                    radius = 5*r;
                    deg90(lawn, y0, x0, deg, time, radius);
                    break;
                }
            case 180:
                {
                    radius = 4*r;
                    deg180(lawn, y0, x0, deg, time, radius);
                    break;
                }
            case 270:
                {
                    radius = 3*r;
                    deg270(lawn, y0, x0, deg, time, radius);
                    break;
                }
            case 360:
                {
                    radius = 2*r;
                    deg360(lawn, y0, x0, time, radius);
                    break;
                }
        }

}

int main ()
{
char **lawn;
lawn = malloc(sizeof(*lawn)*height);
int i, j;

for (j = 0; j < height; j++)
        lawn[j] = malloc(sizeof(*lawn[0])*width);


	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
			lawn[i][j] = 1;


	for (i = height - 10; i < height; i++)
		for (j = width - 10; j < width; j++)
			lawn[i][j] = 0;

	int x0 = 9;
	int y0 = 8;

    int type = 90;
    int time = 1; //liczba cykli dla 360
    int deg = 2; //nr cwiartki która jest: 90 zajeta, 270 pusta, 180 12 to 1 i 2 zajeta

    typedeg(lawn, y0, x0, type, deg, time);

	for (i = 0; i < height; i++)
		for (j = 0; j < width; j++)
            {
			printf(" %d", lawn[i][j]);
			if (j == width-1)
			{
				printf("\n");
			}
		}

for (j = 0; j < height; j++)
        free(lawn[j]);

free(lawn);


return 0;
}
