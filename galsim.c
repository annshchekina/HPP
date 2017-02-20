#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "galsim.h"
#include "file_operations/file_operations.h"
#include "graphics/graphics.h"

const float circleRadius=0.0025, circleColor=0;
const int windowWidth=800;	

// calculate the next position and velocity
void calculation(star* stars, star* stars_buffer, int N_stars, double delta_t) { 	
	const double G = 100./N_stars;	
	const double e = 0.001;
	
	for (int i = 0; i < N_stars; i++) {			
		double rx = 0;	
		double ry = 0;
		double length_r = 0;
		double Fx = 0;
		double Fy = 0;
		for (int j = 0; j < N_stars; j++) {
			if (j == i) continue;
			rx = stars[i].x - stars[j].x;
			ry = stars[i].y - stars[j].y;				
			length_r = sqrt(rx*rx+ry*ry);
			Fx -= G*((stars[j].m * rx)/((length_r+e)*(length_r+e)*(length_r+e)));				
			Fy -= G*((stars[j].m * ry)/((length_r+e)*(length_r+e)*(length_r+e)));
		}		
		stars_buffer[i].vx = stars[i].vx + delta_t*Fx;	
		stars_buffer[i].x = stars[i].x + delta_t*stars_buffer[i].vx;
		stars_buffer[i].vy = stars[i].vy + delta_t*Fy;
		stars_buffer[i].y = stars[i].y + delta_t*stars_buffer[i].vy;
		// mass does not change
	}
	// sync buffers
	for (int i = 0; i < N_stars; i++) {	
		stars[i].vx = stars_buffer[i].vx;
		stars[i].x = stars_buffer[i].x;
		stars[i].vy = stars_buffer[i].vy;
		stars[i].y = stars_buffer[i].y;
	}	
}

void keep_within_box(float* xA, float* yA) {	
	if(*xA > 1)
	*xA = 0;
	if(*yA > 1)
	*yA = 0;
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
	
	galsim(N, filename, nsteps, delta_t, graphics);
	
	return 0;
}