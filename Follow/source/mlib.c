#include "mlib.h"

void m_sort3(unsigned int range[],double angle[],unsigned int rec_data[],int k)
{
	int m, n;
	for (m = 0; m<k-1; m++)
	{
		for (n = 0; n<k-1; n++)
		{
			if (range[n]>range[n + 1])
			{
				unsigned int cup1 = range[n];
				unsigned int cup2 = rec_data[n];
                double cup3 = angle[n];
				range[n] = range[n + 1]; rec_data[n] = rec_data[n + 1];angle[n] = angle[n+1];
				range[n + 1] = cup1; rec_data[n + 1] = cup2;angle[n+1] = cup3;
			}
		}
	}
}

void m_sort2(unsigned int range[],unsigned int rec_data[],int k)
{
	int m, n;
	for (m = 0; m<k-1; m++)
	{
		for (n = 0; n<k-1; n++)
		{
			if (range[n]>range[n + 1])
			{
				unsigned int cup1 = range[n];
				unsigned int cup2 = rec_data[n];
				range[n] = range[n + 1]; rec_data[n] = rec_data[n + 1];
				range[n + 1] = cup1; rec_data[n + 1] = cup2;
			}
		}
	}
}


void m_sort1(unsigned int range[],int k)
{
    int m, n;
	for (m = 0; m<k-1; m++)
	{
		for (n = 0; n<k-1; n++)
		{
			if (range[n]>range[n + 1])
			{
				unsigned int cup = range[n];
				range[n] = range[n + 1];
				range[n + 1] = cup;
			}
		}
	}
}