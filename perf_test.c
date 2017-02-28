#include <stdio.h>
#include <sys/time.h>
#include <float.h>
#include "galsim.h"

int N_test = 5;

int main()
{
	FILE *output;
	output = fopen("../perf_opt_O3_unroll_restr.txt", "w");
	
	char file_name[64];

	for (int i = 1; i < 10; i++)
	{			
		// this is why 3 similar loops, digit in name
		sprintf(file_name, "../input_data/ellipse_N_000%d0.gal", i);
		double test_time = DBL_MAX;
		// take minimum time
		for (int j = 0; j < N_test; j++)
		{
			double t0 = get_wall_seconds();
			galsim(i*10, file_name, 200, 1e-5, 0);
			double curr_test_time = get_wall_seconds() - t0;
			if (curr_test_time < test_time) 
				test_time = curr_test_time;
		}
		fprintf(output, "%d\t %.5f\n", i*10, test_time);
	}
	for (int i = 1; i < 10; i++)
	{		
		sprintf(file_name, "../input_data/ellipse_N_00%d00.gal", i);
		double test_time = DBL_MAX;
		for (int j = 0; j < N_test; j++)
		{
			double t0 = get_wall_seconds();
			galsim(i*100, file_name, 200, 1e-5, 0);
			double curr_test_time = get_wall_seconds() - t0;
			if (curr_test_time < test_time) 
				test_time = curr_test_time;
		}	
		printf("Test for N = %d finished\n", i*100);
		fprintf(output, "%d\t %.5f\n", i*100, test_time);
	}
	for (int i = 1; i < 6; i++)
	{	
		sprintf(file_name, "../input_data/ellipse_N_0%d000.gal", i);
		double test_time = DBL_MAX;
		for (int j = 0; j < N_test; j++)
		{
			double t0 = get_wall_seconds();
			galsim(i*1000, file_name, 200, 1e-5, 0);
			double curr_test_time = get_wall_seconds() - t0;
			if (curr_test_time < test_time) 
				test_time = curr_test_time;
		}	
		printf("Test for N = %d finished\n", i*1000);
		fprintf(output, "%d\t %.5f\n", i*1000, test_time);
	}
	
	fclose(output);
	
	return 0;
}