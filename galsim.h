typedef struct {	
	double x;
	double y;
	double m;
	double vx;
	double vy;
} star;

void calculation(star* stars, star* stars_buffer, int N_stars, double delta_t);

void keep_within_box(float* xA, float* yA);

void galsim(int N, char * filename, int nsteps, double delta_t, int graphics);