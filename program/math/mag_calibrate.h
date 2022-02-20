typedef struct{
	unsigned char vflg; 
	double offset[3];  //bias error
	double diag[3];    //scale-factor error
	double offdiag[3]; //cross-axis sensitivity
	double radius;     //sphere radius
} calpara_t;

#define COMPASS_CAL_NUM_SAMPLES 300   //总采样数

char CompassCal(double sample[3]);
