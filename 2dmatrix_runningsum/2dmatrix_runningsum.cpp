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

struct rect
{
	XY topleft;
	XY botright;
};

void calculate_running_sum(const vector<vector<int>>& original, vector<vector<long long>>& output)
{

}

rect naive_max_rect(const vector<vector<int>>& original)
{
	rect out;

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


	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}

