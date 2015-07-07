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

	bool operator==(const XY& other) const
	{
		return (x == other.x && y == other.y);
	}
};

//0,0 0,0 represents a rect of size 1
struct rect
{
	XY topleft; //represents the topmost leftmost cell in the rectangle
	XY botright; //represents the bottommost rightmost coordiante in the rectangle

	rect() {}
	rect(XY a, XY b) : topleft(a), botright(b) {}

	int area() const
	{
		//+1 here to account for botright representing last index, not out-of-index position
		return (botright.x - topleft.x + 1) * (botright.y - topleft.y + 1);
	}

	bool operator==(const rect& other) const
	{
		return (topleft == other.topleft && botright == other.botright);
	}
};

template <typename T> 
T zeroed_array_access(const vector<vector<T>>& arr, const XY& point)
{
	if (point.x < 0 || point.y < 0)
		return 0;

	else
		return arr[point.x][point.y];
}

void printnum(int in)
{
	printf("%d\t", in);
}
void printnum(long long in)
{
	printf("%lld\t", in);
}

template <typename T>
void print_array(const vector<vector<T>>& arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < arr[i].size(); j++)
		{
			printnum(arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

//O(n^2)
void calculate_running_sum(const vector<vector<int>>& original, vector<vector<long long>>& output)
{
	if (original.size() == 0)
		return;

	for (int i = 0; i < original.size(); i++)
	{
		for (int j = 0; j < original[i].size(); j++)
		{
			output[i][j] = zeroed_array_access(output, XY(i-1,j))
				+ zeroed_array_access(output, XY(i, j-1))
				- zeroed_array_access(output, XY(i-1, j - 1))
				+ original[i][j];
		}
	}
	//print_array(original);
	//print_array(output);
}

template <typename T>
void resize_array(vector<vector<T>>& out, int x, int y)
{
	out.resize(x);
	for (int i = 0; i < x; i++)
	{
		out[i].resize(y);
	}
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
					if (sum > max_area || ( sum == max_area && out.area() <= test.area() ))
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

//O(n^5)
long long faster_max_rect(const vector<vector<int>>& original, rect& out)
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
				long long running_sum_y = 0;
				for (int y = j; y < original[i].size(); y++)
				{

					//do the sum
					for (int a = i; a <= x; a++)
					{
						running_sum_y += original[a][y];
					}

					rect test = rect(XY(i, j), XY(x, y));
					if ( running_sum_y > max_area || ( running_sum_y == max_area && out.area() <= test.area()))
					{
						out = test;
						max_area = running_sum_y;

					}

				}
			}

		}
	}

	return max_area;
}

//O(n^4)
long long LUT_max_rect(const vector<vector<int>>& original, rect& out)
{
	long long max_area = 0;
	out.botright = XY(-1, -1);

	if (original.size() == 0)
		return 0;

	vector<vector<long long>> lut;
	resize_array(lut, original.size(), original[0].size());
	calculate_running_sum( original, lut );

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

					long long sum = zeroed_array_access( lut, XY(x, y ))
						- zeroed_array_access( lut, XY(i - 1,y ))
						- zeroed_array_access( lut, XY( x, j - 1 ))
						+ zeroed_array_access( lut, XY( i - 1, j - 1 ));

					rect test = rect(XY(i, j), XY(x, y));
					if (sum > max_area || ( sum == max_area && out.area() <= test.area()))
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

void one_D_integral(const vector<int>& original, vector<long long>& out, bool prepend_zero)
{
	out.reserve(original.size() + 1*prepend_zero);
	if (prepend_zero)
		out.push_back(0);

	out.push_back( original[0] );
	for (int i = 1; i < original.size(); i++)
	{
		out.push_back( out.back() + original[i] );
	}
}

long long naive_one_D_highest_delta(const vector<long long>& original, XY& out)
{
	long long max_delta = 0;
	out.y = -1;

	for (int i = 0; i < original.size(); i++)
	{
		for (int j = i; j < original.size(); j++)
		{
			long long delta = original[j] - original[i];
			if (delta > max_delta || (delta == max_delta && (j - i) >(out.x - out.y)))
			{
				max_delta = delta;
				out.x = i;
				out.y = j;
			}
		}
	}
	return max_delta;
}

long long one_D_highest_delta(const vector<long long>& original, XY& out)
{
	int min_index = 0;
	int max_index = 0;
	int new_min_index = 0;
	long long max_delta = 0;

	for (int i = 0; i < original.size(); i++)
	{
		if ((original[i] - original[new_min_index]) > max_delta)
		{
			max_index = i;
			min_index = new_min_index;
			max_delta = original[i] - original[new_min_index];
		}
		if (original[i] < original[new_min_index])
		{
			new_min_index = i;
		}
	}

	out.x = min_index;
	out.y = max_index;

	return max_delta;
}

//solve the problem for the 1D case
long long one_D_max(const vector<int>& original, XY& out)
{
	if (original.size() == 0)
	{
		out.y = -1;
		return 0;
	}
	
	vector<long long> integral;
	one_D_integral(original, integral, true);

	long long result = naive_one_D_highest_delta(integral, out);

	return result;
}

//O(n^2)
long long naive_one_D_max(const vector<int>& original, XY& out)
{
	long long max_sum = 0;
	out.y = -1;

	for (int i = 0; i < original.size(); i++)
	{
		long long sum = 0;
		for (int j = i; j < original.size(); j++)
		{
			sum += original[j];
			if (sum > max_sum || (sum == max_sum && (j-i+1) > (out.x - out.y) ))
			{
				max_sum = sum;
				out.x = i;
				out.y = j;
			}
		}
	}
	return max_sum;
}

//O(n^3)
long long one_D_based_max_rect(const vector<vector<int>>& original, rect& out)
{
	long long max_area = 0;
	out.botright = XY(-1, -1);

	for (size_t x_left = 0; x_left < original.size(); x_left++)
	{
		vector<int> accum;
		accum.resize(original[x_left].size());

		for (size_t x_right = x_left; x_right < original.size(); x_right++)
		{
			for (size_t i = 0; i < accum.size(); i++)
			{
				accum[i] += original[x_right][i];
			}

			XY range;
			long long sum = one_D_max(accum, range);

			rect test = rect(XY(x_left, range.x), XY(x_right, range.y));
			if (sum > max_area || ( sum == max_area && out.area() <= test.area()))
			{
				out = test;
				max_area = sum;

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

		const int NUM_IMPLEMENTATIONS = 4;

		vector<rect> rects;
		rects.resize(NUM_IMPLEMENTATIONS);
		vector<long long> results;
		results.resize(NUM_IMPLEMENTATIONS);
		
		results[0] = naive_max_rect(arr, rects[0]);
		results[1] = faster_max_rect(arr, rects[1]);
		results[2] = LUT_max_rect(arr, rects[2]);
		results[3] = one_D_based_max_rect(arr, rects[3]);
		
		for (int i = 0; i < NUM_IMPLEMENTATIONS - 1; i++)
		{
			if (results[i + 1] != results[i])
			{
				printf("test failed\n");
				break;
			}
			if (!(rects[i + 1] == rects[i]))
			{
				printf("rectangle result varies\n");
				break;
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}

