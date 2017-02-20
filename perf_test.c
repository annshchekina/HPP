#include <stdio.h>
#include <sys/time.h>
#include "galsim.h"

double get_wall_seconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double seconds = tv.tv_sec + (double)tv.tv_usec/1e6;
		
	return seconds;
}

int main()
{
	FILE *output;
	output = fopen("perf_O3.txt", "w");
	
	char file_name[64];

	for (int i = 1; i < 10; i++)
	{	
		double t0 = get_wall_seconds();
		// this is why 3 similar loops, digit
		sprintf(file_name, "../input_data/ellipse_N_000%d0.gal", i);
		galsim(i*10, file_name, 200, 1e-5, 0);
		double test_time = get_wall_seconds() - t0;
		fprintf(output, "%d\t %.5f\n", i*10, test_time);
	}
	for (int i = 1; i < 10; i++)
	{	
		double t0 = get_wall_seconds();
		sprintf(file_name, "../input_data/ellipse_N_00%d00.gal", i);
		galsim(i*100, file_name, 200, 1e-5, 0);
		printf("Test for N = %d finished\n", i*100);
		double test_time = get_wall_seconds() - t0;
		fprintf(output, "%d\t %.5f\n", i*100, test_time);
	}
	for (int i = 1; i < 10; i++)
	{	
		double t0 = get_wall_seconds();
		sprintf(file_name, "../input_data/ellipse_N_0%d000.gal", i);
		galsim(i*1000, file_name, 200, 1e-5, 0);
		printf("Test for N = %d finished\n", i*1000);
		double test_time = get_wall_seconds() - t0;
		fprintf(output, "%d\t %.5f\n", i*1000, test_time);
	}
	
	fclose(output);
	
	return 0;
}