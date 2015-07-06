// 2dmatrix_runningsum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <limits>
#include <stdint.h>

using namespace std;

struct XY
{
	int x;
	int y;

	XY() : x(0), y(0) {}
	XY(int a, int b) : x(a), y(b) {}
};

//0,0 0,0 represents a rect of size 1
struct rect
{
	XY topleft; //represents the topmost leftmost cell in the rectangle
	XY botright; //represents the bottommost rightmost coordiante in the rectangle

	rect() {}
	rect(XY a, XY b) : topleft(a), botright(b) {}

	int area()
	{
		//+1 here to account for botright representing last index, not out-of-index position
		return (botright.x - topleft.x + 1) * (botright.y - topleft.y + 1);
	}
};

void calculate_running_sum(const vector<vector<int>>& original, vector<vector<long long>>& output)
{

}

//O(n^6)
long long naive_max_rect(const vector<vector<int>>& original, rect& out)
{
	out.botright = XY(-1, -1);
	long long max_area = 0;

	//left top coordinate
	for (int i = 0; i < original.size(); i++)
	{
		for (int j = 0; j < original[i].size(); j++)
		{

			//bottom right coordinate
			for (int x = i; x < original.size(); x++)
			{
				for (int y = j; y < original[i].size(); y++)
				{
					
					//do the sum
					long long sum = 0;
					for (int a = i; a <= x; a++)
					{
						for (int b = j; b <= y; b++)
						{
							sum += original[a][b];
						}
					}

					rect test = rect(XY(i, j), XY(x, y));
					if (sum >= max_area && out.area() >= test.area() )
					{
						out = test;
						max_area = sum;

					}

				}
			}

		}
	}

	return max_area;
}


void init_array(vector<vector<int>> & out, const int MAX_ARRAY_SZ, const int MAX_ABS_NUM)
{	
	XY sz = XY(rand() % (MAX_ARRAY_SZ + 1), rand() % (MAX_ARRAY_SZ + 1));
	out.resize(sz.x);
	for (int i = 0; i < sz.x; i++)
	{
		out[i].resize(sz.y);
		for (int j = 0; j < sz.y; j++)
		{
			out[i][j] = rand() % (MAX_ABS_NUM * 2 + 1) - MAX_ABS_NUM;
		}
	}
}

void test()
{
	const int NUMTESTS = 100;


	for (int test = 0; test < NUMTESTS; test++)
	{
		const int MAX_ARRAY_SZ = 10;
		const int MAX_ABS_NUM = 10;

		vector<vector<int>> arr;
		init_array(arr, MAX_ARRAY_SZ, MAX_ABS_NUM);

		rect rect1;
		long long result1 = naive_max_rect(arr, rect1);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}

