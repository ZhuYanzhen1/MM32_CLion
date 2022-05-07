#ifndef filter_h
#define filter_h

typedef struct {
    unsigned char m;
    float ep;
    float A[2];
    float d1[2];
    float d2[2];
    float w0[2];
    float w1[2];
    float w2[2];
} CHELowPass;

void create_che_low_pass_filter(float epsilon, float sampling_frequency,
                                float half_power_frequency, CHELowPass *filter);

float che_low_pass(CHELowPass *filter, float input);
float rc_low_pass(float input, float last_output);

#endif /* filter_h */