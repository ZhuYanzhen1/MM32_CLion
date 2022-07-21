#ifndef RICCATI_SOLVER_H_
#define RICCATI_SOLVER_H_

///////////////////// Find Index ////////////////////////
#define OVERRUN_POINT               5
#define INDEX_OFFSET                20

///////////////////// Servo control /////////////////////
#define SERVO_MID_POINT             150
#define MAX_DECLINATION_ANGLE       20
#define DELTA_ANGLE                 1
#define YAW_TO_ANGLE                (-63.66203f)     // 180/pi * (-50/45)
#define ITERATION_ACCURACY          (0.1f)
#define ANGLE_TO_RADIAN             (0.0174533f)
#define _2PI_                       (6.2831853f)

///////////////////// Trace Transition //////////////////
#define DIVIDING_POINT_1            100
#define DIVIDING_POINT_2            230
#define DIVIDING_POINT_3            400
#define DIVIDING_POINT_4            530
#define TRANSITION_SECTION          30
#define CURVES_ATTENUATION_RATE     2
#define STRAIGHT_ATTENUATION_RATE   1.2f
#define BUFFER_BELT_1               (DIVIDING_POINT_1 + TRANSITION_SECTION)
#define BUFFER_BELT_2               (DIVIDING_POINT_2 - TRANSITION_SECTION)
#define BUFFER_BELT_3               (DIVIDING_POINT_3 + TRANSITION_SECTION)
#define BUFFER_BELT_4               (DIVIDING_POINT_4 - TRANSITION_SECTION)

#define OUTPUT_DEBUG_INFO           (0)

typedef struct {
    float pos_n;
    float pos_e;
    float angle;
} basic_status_t;

#if RUNNING_UNIT_TEST
float calculate_distance(int ind, float x, float y);
int dichotomy(int ind_start, int ind_end, float x, float y);
#else
float calculate_distance(int ind);
int dichotomy(int ind_start, int ind_end);
int find_index(int last_index);
#endif  // RUNNING_UNIT_TEST

void track_prediction(basic_status_t *current, float v, float t, float servo_angle);

void lqr_control_test(unsigned short index, int angle_ctrl, float angle_yaw, float distance_n, float distance_e);
unsigned char lqr_control(unsigned short index, basic_status_t status);

void solve_feedback_value(float p[3][3], double a[3][3], double b[3][2], double x[3][1], float r, float fb[2][1]);
void solve_riccati_equation(double a[3][3], double b[3][2], float q, float r, float p[3][3]);

#endif
