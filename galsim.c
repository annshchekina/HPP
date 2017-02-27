#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include "galsim.h"
#include "file_operations/file_operations.h"
#include "graphics/graphics.h"

#define PERF_TEST_ORD	0

const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;	

double t_non_rep = 0, t_calc_calc = 0, t_calc_other = 0, 
	t_calc_calc_part, t_start, t_start1, t_m_start;
// assume no graphics

double get_wall_seconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double seconds = tv.tv_sec + (double)tv.tv_usec/1e6;
		
	return seconds;
}

void calculation(star* stars, star* stars_buffer, int N_stars, double delta_t) { 
	t_start = get_wall_seconds();
	const double G = 100./N_stars;	
	const double e = 0.001;
	
	t_calc_calc_part = 0;
	for (int i = 0; i < N_stars; i++) {			
		double Fx = 0, Fy = 0;
		t_start1 = get_wall_seconds();
		double star_x = stars[i].x, star_y = stars[i].y;
		for (int j = 0; j < N_stars; j++) {
			if (j == i) continue;
			double rx = star_x - stars[j].x;
			double ry = star_y - stars[j].y;				
			double length_r_e = sqrt(rx * rx + ry * ry) + e;
			// cube can be calculated once
			// i indexed can be loaded once and *
			double mult = stars[j].m / (length_r_e * length_r_e * length_r_e);
			Fx += mult * rx;				
			Fy += mult * ry;
		}
		Fx *= -G;
		Fy *= -G;
		t_calc_calc_part += get_wall_seconds() - t_start1;
		stars_buffer[i].vx = stars[i].vx + delta_t * Fx;	
		stars_buffer[i].x = stars[i].x + delta_t * stars_buffer[i].vx;
		stars_buffer[i].vy = stars[i].vy + delta_t * Fy;
		stars_buffer[i].y = stars[i].y + delta_t * stars_buffer[i].vy;
		// mass does not change
	}
	t_calc_calc += t_calc_calc_part;
	// sync buffers
	for (int i = 0; i < N_stars; i++) {	
		stars[i].vx = stars_buffer[i].vx;
		stars[i].x = stars_buffer[i].x;
		stars[i].vy = stars_buffer[i].vy;
		stars[i].y = stars_buffer[i].y;
	}	
	t_calc_other = get_wall_seconds() - t_start - t_calc_calc_part;
}

void galsim(int N, char * filename, int nsteps, double delta_t, int graphics) {
	
	double* buffer = malloc(5*N*sizeof(double));	// i/o 
	read_doubles_from_file(5*N, buffer, filename);	

	star stars[N], stars_buffer[N];	
	
	for (int i = 0; i < N; i++) {	
		stars[i].x = buffer[5*i];
		stars[i].y = buffer[5*i+1];
		stars[i].m = buffer[5*i+2];
		stars[i].vx = buffer[5*i+3];
		stars[i].vy = buffer[5*i+4];
	}

	float L=1, W=1;	

	if (graphics) {
		InitializeGraphics("galsim", windowWidth, windowWidth);
		SetCAxes(0,1);
	}

	for(int i = 0; i < nsteps; i++) {
		calculation(stars, stars_buffer, N, delta_t);	
		
		if (graphics) {
 			ClearScreen();
			for (int j = 0; j < N; j++)
				DrawCircle(stars[j].x, stars[j].y, L, W, circleRadius, circleColor);
    		Refresh();
    		usleep(3000);
		}
  	}	
	if (graphics) {
  		FlushDisplay();
  		CloseDisplay();
	}

	for (int i = 0; i < N; i++) {	
		buffer[5*i] = stars[i].x;
		buffer[5*i+1] = stars[i].y;
		buffer[5*i+2] = stars[i].m;
		buffer[5*i+3] = stars[i].vx;
		buffer[5*i+4] = stars[i].vy;
	}
	write_doubles_to_file(5*N, buffer, "result.gal");	
	free(buffer);
}

#if !PERF_TEST_ORD
int main(int argc, char **argv)
{
		
	if (argc < 6) {	
			printf("Give 5 input args: \'N filename nsteps delta_t graphics\'\n");
			exit(1);
	}

	int N = strtol(argv[1], NULL, 10);	
	char* filename = argv[2];	// initial configuration 
	int nsteps = strtol(argv[3], NULL, 10);	
	double delta_t = atof(argv[4]);	
	int graphics = strtol(argv[5], NULL, 10);	
	
	FILE *output;
	char out_file_name[64];
	sprintf(out_file_name, "time_cons_parts_opt_denom_%d.txt", N);	
	output = fopen(out_file_name, "w");
	
	t_m_start = get_wall_seconds();	
	galsim(N, filename, nsteps, delta_t, graphics);
	
	t_non_rep = get_wall_seconds() - t_m_start - t_calc_calc - t_calc_other;
	fprintf(output, "Overall time: %lf\nCalculation: %lf\nStores: %lf\n", t_non_rep, t_calc_calc, t_calc_other);	
	fclose(output);
	
	return 0;
}
#endif