#include "math.h"
#include "stdbool.h"
#include "sys.h"
#include "string.h"
#include "stdio.h"

#define WHEEL_INTERVAL 0.38f
#define MULTIPLIER
#define DECELERATION_RATIO
#define WHEEL_DIAMETER 1
#define PI_1_2 1.570796f
#define PI 3.1415926f
#define PI_3_2 4.712389f
#define PI_2 6.283186f
#define LINE_NUM
#define OPIENTION_INTERVAL
#define SYSCLKTOMS 400 * 1000 * 1000
#define KINCO_RPM_HEX 2730
#define ENCODER_RELOAD 4294967296
#define DECEL_RATIO 21
#define DIR_LEFT (-1)
#define DIR_RIGHT 1
#define MAX_POSE 2147483647.0

#define CONST_FRAME (WHEEL_DIAMETER * PI / (LINE_NUM * MULTIPLIER * DECELERATION_RATIO))

#define MOTOR_PROCESS_LEFT 0x281
#define MOTOR_PROCESS_RIGHT 0x282
#define WARNING_FRAME_ONE 0x181
#define WARNING_FRAME_TWO 0x182

#define DATA_FRAME_LEN 8
#define DISTANCE_PER_PULSE 0.00000299f // pi*2*0.1/10000/21

typedef struct
{
	int32_t speed;
	int32_t pos;
} MOTOR_INFO;

typedef union {
	struct
	{
		float x;
		float y;
		float z;
		float th;
	} pose;
	struct
	{
		float linear_x;
		float linear_y;
		float angualr_z;
	} twist;
} ODOMETRY;

typedef struct
{
	u32 seq;
	char node_id[4];
	char sensor_id[4];
	u32 alarm_type;
	char alarm_msg[30];
} ALARM_MSG;

extern MOTOR_INFO info_left;
extern MOTOR_INFO info_right;
extern ODOMETRY odom;
extern ALARM_MSG alarm_msg;
extern bool ck_lf;
extern bool ck_rh;
extern bool first_cal;
extern int32_t prev_left_pos;
extern int32_t prev_right_pos;
extern float vxy;
extern float vth;
extern float x;
extern float y;
extern float temp_x;
extern float temp_y;
extern float th;
extern u32 alarm_count;
extern u32 told;
extern u32 tnow;
extern float test_ss;
extern bool connection_check;
extern bool connected;
extern bool alarm_confirm;
extern int disconnect_cnt;
extern float pose_cov_frez[36];
extern float twist_cov_frez[36];
extern float pose_cov_move[36];
extern float twist_cov_move[36];
extern char *warn_group1[];
extern char *warn_group2[];
