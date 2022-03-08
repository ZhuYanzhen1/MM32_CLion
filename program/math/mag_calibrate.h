typedef struct{
	unsigned char vflg; 
	float offset[3];  //bias error
    float diag[3];    //scale-factor error
    float offdiag[3]; //cross-axis sensitivity
    float radius;     //sphere radius
} calpara_t;

#define COMPASS_CAL_NUM_SAMPLES 2048   //总采样数

char CompassCal(float sample[3]);
