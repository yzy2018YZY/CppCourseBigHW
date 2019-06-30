//1850059 计1班 杨志远

#include "90-b0.h"

struct Point
{
	int X = 0;
	int Y = 0;
	int fatherX = 0;
	int fatherY = 0;
};

static int FindPointInList(Point theList[], int start, int end, int x, int y);
static void PushPointFromListToList(Point dList[], int &dLength, Point sList[], int index);
static bool DeletePointFromCloseList(Point surroundList[], int surroundIndex, Point closeList[], int closeListLength);
static int getSurroundPoints(int map[][MaxCol + 1], int row, int col, int tempPointIndex, Point surroundList[], Point openList[]);
static int find_path_output(int endX, int endY, int pointListX[], int pointListY[], Point allList[], int allListLength);
static int check_map_find_i_start(int map[][MaxCol + 1], int row, int col, const int direction[][2], int d, int value, int maxRange, int x, int y);

bool game_over(int map[][MaxCol + 1], int row, int col)
{
	return map_is_full(map, row, col);
}

static int check_map_find_i_start(int map[][MaxCol + 1], int row, int col, const int direction[][2], int d, int value, int maxRange, int x, int y)
{
	int i = -maxRange;
	while (++i < 0)
		if (in_map(row, col, x + i * direction[d][0], y + i * direction[d][1]) && map[y + i * direction[d][1]][x + i * direction[d][0]] == value)
			break;
	return i;
}

int check_map(int map[][MaxCol + 1], int row, int col, int x, int y, int &score, int &scoreAll, bool consoleMode, int ballsRemoved[])
{
	const int direction[8][2] = { {0, 1}, {1, 1}, {1, 0}, {-1, 1} , {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
	int count = 0, result = 0, value = map[y][x], maxRange = max(MaxRow, MaxCol);
	score = 0;
	for (int d = 0; d < 8; d++)
	{
		count = 0;
		int i = check_map_find_i_start(map, row, col, direction, d, value, maxRange, x, y);
		for (; i <= maxRange; i++)
		{
			if (in_map(row, col, x + direction[d][0], y + direction[d][1]) && map[y + i * direction[d][1]][x + i * direction[d][0]] == value)
				count++;
			else
				break;
		}
		if (count >= MinBallsCount)
		{
			if (ballsRemoved != NULL)
				ballsRemoved[map[y][x]] = count;
			result++;
			i = check_map_find_i_start(map, row, col, direction, d, value, maxRange, x, y);
			for (; i <= maxRange; i++)
			{
				if (i != 0 && in_map(row, col, x + direction[d][0], y + direction[d][1]) && map[y + i * direction[d][1]][x + i * direction[d][0]] == value)
				{
					map[y + i * direction[d][1]][x + i * direction[d][0]] = 0;
					if (consoleMode)
						showstr((x + i * direction[d][0]) * 4 + 2, (y + i * direction[d][1]) * 2 + 2, "  ", 15, 0);
				}
				else if(i != 0)
					break;
			}
			score += result * (count - 1) * (count - 2);
		}
	}
	if (result > 0)
	{
		map[y][x] = 0;
		if (consoleMode)
			showstr(x * 4 + 2, y * 2 + 2, "  ", 15, 0);
	}
	scoreAll += score;
	return result;
}

int statistics_balls(int map[][MaxCol + 1], int row, int col, int color)
{
	int result = 0;
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			if (color == map[i][j])
				result++;
	return result;
}

int find_path(int row, int col, int startX, int startY, int endX, int endY, int map[][MaxCol + 1], int *pointListX, int *pointListY)
{
	int startPoint = map[startY][startX];
	Point openList[MaxPath] = {startX, startY, -1, -1};
	Point closeList[MaxPath];
	Point allList[MaxPath] = { startX, startY, -1, -1};
	int index_of_myPoint = 0, openListStart = 0, openListEnd = 0, closeListLength = 0, allListLength = 0, surroundLength = 0;
	map[startY][startX] = 0;
	openListEnd++;
	allListLength++;
	while (openListEnd - openListStart > 0 && allListLength < MaxPath - 5)
	{
		Point surroundList[4] = { {-1, -1, -1, -1 }, {-1, -1, -1, -1 }, {-1, -1, -1, -1 }, {-1, -1, -1, -1 }};
		PushPointFromListToList(closeList, closeListLength, openList, openListStart);
		surroundLength = getSurroundPoints(map, row, col, openListStart, surroundList, openList);
		openListStart++;
		for (int i = 0; i < surroundLength; i++)
			DeletePointFromCloseList(surroundList, i, closeList, closeListLength);
		for (int i = 0; i < surroundLength; i++)
		{
			if (surroundList[i].X != -1)
			{
				if (FindPointInList(allList, 0, allListLength, surroundList[i].X, surroundList[i].Y) == -1)
					PushPointFromListToList(openList, openListEnd, surroundList, i);
				if (FindPointInList(allList, 0, allListLength, surroundList[i].X, surroundList[i].Y) == -1)
					PushPointFromListToList(allList, allListLength, surroundList, i);
			}
		}
		if (FindPointInList(openList, openListStart, openListEnd, endX, endY) > -1)//判断何时退出
		{
			map[startY][startX] = startPoint;
			return find_path_output(endX, endY, pointListX, pointListY, allList, allListLength);;
		}
	}
	map[startY][startX] = startPoint;
	return 0;
}

static int FindPointInList(Point theList[], int start, int end, int x, int y)
{
	for (int i = start; i < end; i++)
		if (theList[i].X == x && theList[i].Y == y)
			return i;
	return -1;
}

static void PushPointFromListToList(struct Point dList[], int &dLength, struct Point sList[], int index)
{
	dList[dLength].X= sList[index].X;
	dList[dLength].Y = sList[index].Y;
	dList[dLength].fatherX = sList[index].fatherX;
	dList[dLength].fatherY = sList[index].fatherY;
	dLength++;
}

static bool DeletePointFromCloseList(Point surroundList[], int surroundIndex, Point closeList[], int closeListLength)
{
	bool result = false;
	for (int i = 0; i < closeListLength; i++)
	{
		if (closeList[i].X == surroundList[surroundIndex].X && closeList[i].Y == surroundList[surroundIndex].Y)
		{
			surroundList[surroundIndex].X = -1;
			surroundList[surroundIndex].Y = -1;
			result = true;
		}
	}
	return result;
}

static int getSurroundPoints(int map[][MaxCol + 1], int row, int col, int tempPointIndex, Point surroundList[], Point openList[])
{
	struct tPoint
	{
		int X;
		int Y;
		bool isWall;
	}tempPoints[4] = { {-1, -1, false}, {-1, -1, false},{-1, -1, false} };
	int tempPointsLength = 0;
	if (openList[tempPointIndex].Y < row - 1)
	{
		tempPoints[tempPointsLength].X = openList[tempPointIndex].X;
		tempPoints[tempPointsLength].Y = openList[tempPointIndex].Y + 1;
		tempPoints[tempPointsLength++].isWall = map[openList[tempPointIndex].Y][openList[tempPointIndex].X];
	}
	if (openList[tempPointIndex].Y > 0)
	{
		tempPoints[tempPointsLength].X = openList[tempPointIndex].X;
		tempPoints[tempPointsLength].Y = openList[tempPointIndex].Y - 1;
		tempPoints[tempPointsLength++].isWall = map[openList[tempPointIndex].Y][openList[tempPointIndex].X];
	}
	if (openList[tempPointIndex].X > 0)
	{
		tempPoints[tempPointsLength].X = openList[tempPointIndex].X - 1;
		tempPoints[tempPointsLength].Y = openList[tempPointIndex].Y;
		tempPoints[tempPointsLength++].isWall = map[openList[tempPointIndex].Y][openList[tempPointIndex].X];
	}
	if (openList[tempPointIndex].X < col - 1)
	{
		tempPoints[tempPointsLength].X = openList[tempPointIndex].X + 1;
		tempPoints[tempPointsLength].Y = openList[tempPointIndex].Y;
		tempPoints[tempPointsLength++].isWall = map[openList[tempPointIndex].Y][openList[tempPointIndex].X];
	}
	for (int i = 0; i < tempPointsLength; i++)
	{
		if (tempPoints[i].isWall == false)
		{
			surroundList[i].X = tempPoints[i].X;
			surroundList[i].Y = tempPoints[i].Y;
			surroundList[i].fatherX = openList[tempPointIndex].X;
			surroundList[i].fatherY = openList[tempPointIndex].Y;
		}
	}
	return tempPointsLength;
}

static int find_path_output(int endX, int endY, int pointListX[], int pointListY[], Point allList[], int allListLength)
{
	int index_of_return = FindPointInList(allList, 0, allListLength, endX, endY);
	int m_pointListSize = 0;
	for (; index_of_return != -1 && m_pointListSize <= allListLength; m_pointListSize++)
	{
		pointListX[m_pointListSize] = allList[index_of_return].X;
		pointListY[m_pointListSize] = allList[index_of_return].Y;
		index_of_return = FindPointInList(allList, 0, allListLength, allList[index_of_return].fatherX, allList[index_of_return].fatherY);
	}
	for (int i = 0; i < m_pointListSize / 2; i++)
	{
		exchange(pointListX[m_pointListSize - i - 1], pointListX[i]);
		exchange(pointListY[m_pointListSize - i - 1], pointListY[i]);
	}
	return m_pointListSize;
}