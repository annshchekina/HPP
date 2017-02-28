typedef struct {	
	double x;
	double y;
	double m;
	double vx;
	double vy;
} star;

double get_wall_seconds();

void calculation(star * __restrict stars, star * __restrict stars_buffer, 
	const int N_stars, const double delta_t);

void galsim(const int N, const char * filename, 
	const int nsteps, const double delta_t, const int graphics);