#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<Windows.h>
#include<math.h>
#include<time.h>

#define DS 20
#define MS ((DS * 2) + 1)
int map[MS][MS] = { 0 };

enum key { UP = 1, LEFT, RIGHT, DOWN };

int x = 0, y = 0;

typedef struct {
	char name[20];
	double score;
}Rank;

Rank *ranking;
int size = 0;

void ShowBoard()
{
	int i, j;
	system("cls");
	for (i = 0; i < MS; i++)
	{
		for (j = 0; j < MS; j++)
		{
			if (x == j && y == i)
			{
				printf("ⓧ");
				continue;
			}
			if (map[j][i] == 0)
				//printf("%d ", map[i][j]);
				printf("■");
			else if (map[j][i] == 5)
				//printf("%d ", map[i][j]);
				printf("  ");
			else
				printf("%d ", map[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

void ReMaze()
{
	int i;
	int* ptr = map;
	for (i = 0; i < MS*MS; i++)
	{
		if (ptr[i] < 5)
			ptr[i] = 0;
		if (ptr[i] > 5)
			ptr[i] = 5;
	}
}

int *MakeMaze()
{
	int rotate, i, j;
	int remain = DS*DS - 1;
	int start, startx, starty;
	int	endx, endy;
	int count;
	map[1][MS-2] = 5;
	srand(time(NULL));
	while(remain > 0)
	{
		count = 0;
		start = rand() % remain + 1;	//random start point in remain
		for (i = 0; i < DS; i++)
		{
			for (j = 0; j < DS; j++)
			{
				if (map[2 * j + 1][2 * i + 1] == 0)
				{
					count++;
					if (start == count)
					{
						startx = 2 * j + 1;
						starty = 2 * i + 1;
						break;
					}
				}
			}
		}
		x = startx;
		y = starty;
		while (map[x][y] != 5)
		{
			rotate = rand() % 4 + 1;
			if (rotate == UP)
			{
				if (y <= 1)
					continue;//만약 위에 공간이 없다면 rotate 다시설정
				if(map[x][y] >= 5)
					map[x][y] = 5 + UP; 
				else
					map[x][y] = UP;
				y--;
				y--;
			}
			if (rotate == LEFT)
			{
				if (x <= 1)
					continue;
				if (map[x][y] >= 5)
					map[x][y] = 5 + LEFT; 
				else
					map[x][y] = LEFT; 
				x--;
				x--;
			}
			if (rotate == RIGHT)
			{
				if (x >= MS - 2)
					continue;
				if (map[x][y] >= 5)
					map[x][y] = 5 + RIGHT; 
				else
					map[x][y] = RIGHT; 
				x++;
				x++;
			}
			if (rotate == DOWN)
			{
				if (y >= MS - 2)
					continue;
				if (map[x][y] >= 5)
					map[x][y] = 5 + DOWN;
				else
					map[x][y] = DOWN;
				y++;
				y++;
			}
		}
		endx = x;
		endy = y;
		x = startx;
		y = starty;
		do
		{
			if (map[x][y] % 5 == UP)
			{
				if (map[x][y] == UP)
					remain--;
				map[x][y] = 5; y--;
				map[x][y] = 5; y--;
				continue;
			}
			if (map[x][y] % 5 == LEFT)
			{
				if (map[x][y] == LEFT)
					remain--;
				map[x][y] = 5; x--;
				map[x][y] = 5; x--;
				continue;
			}
			if (map[x][y] % 5 == RIGHT)
			{
				if(map[x][y] == RIGHT)
					remain--;
				map[x][y] = 5; x++;
				map[x][y] = 5; x++;
				continue;
			}
			if (map[x][y] % 5 == DOWN)
			{
				if (map[x][y] == DOWN)
					remain--;
				map[x][y] = 5; y++;
				map[x][y] = 5; y++;
				continue;
			}
		} while (x != endx || y != endy);
		ReMaze();
	}
}
void Move(int k)
{
	switch (k)
	{
	case 72:
		if (map[x][y - 1] != 0)
		{
			y--;
		}
		return;
	case 75:
		if (map[x - 1][y] != 0)
		{
			x--;
		}
		return;
	case 77:
		if (map[x + 1][y] != 0)
		{
			x++;
		}
		return;
	case 80:
		if (map[x][y + 1] != 0)
		{
			y++;
		}
		return;
	default:
		break;
	}
	return;
}

void ReadRanking()
{
	FILE *file = fopen("Ranking.txt", "r");
	if (file == NULL)
	{
		printf("file connect error");
		ranking = realloc(ranking, sizeof(Rank) * ++size);
		return;
	}
	do {
		ranking = realloc(ranking, sizeof(Rank) * ++size);
	} while (fread(&ranking[size - 1], sizeof(Rank), 1, file));
	fclose(file);
}

void WriteRanking()
{
	FILE *file = fopen("Ranking.txt", "w");
	fwrite(ranking, sizeof(Rank), size, file);
	fclose(file);
}

void ShowRank()
{
	int i;
	printf("Ranking recorded\n");
	for (i = 0; i < size; i++)
	{
		printf("%d. name: %20s\n", i+1, ranking[i].name);
		printf("score: %20f\n\n", ranking[i].score);
		printf("------------------------------\n\n");
	}
	system("pause");
}

void RankingSort(double score)
{
	int i;
	Rank rank;
	printf("your name?: ");
	scanf("%s", rank.name);
	rank.score = score;
	ranking[size-1] = rank;
	if(size == 1)
	{
		return;
	}
	for (i = size-1; i >= 0; i--)
	{
		if (ranking[i].score < ranking[i - 1].score)
		{
			rank = ranking[i];
			ranking[i] = ranking[i - 1];
			ranking[i - 1] = rank;
		}
		else
		{
			return;
		}
	}
	if(i == 0)
		printf("New record!!\n");
}

int main()
{
	MakeMaze();
	map[1][0] = 5;		//start
	map[MS-2][MS-1] = 5;//finish
	int key;
	clock_t start, end;
	double tscore;
	x = 1;
	y = 0;
	ReadRanking();
	printf("Press any key to start");
	system("pause");
	start = clock();
	while (1)
	{
		ShowBoard();
		key = getch();
		if (key == 224 || key == 0)
		{
			key = getch();
			Move(key);
		}
		if (x == MS - 2 && y == MS-1)
		{
			end = clock();
			tscore = ((double)end - (double)start)/1000;
			printf("\nclear!!!!!\n");
			printf("clear time: %fsec\n\n", tscore);
			RankingSort(tscore);
			ShowRank();
			WriteRanking(); 
			return 0;
		}
	}
}
