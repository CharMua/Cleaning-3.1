#include "global.h"

MOTOR_INFO info_left, info_right;
ODOMETRY odom;
ALARM_MSG alarm_msg;
bool ck_lf = false;
bool ck_rh = false;
bool first_cal = true;
u32 told;
u32 tnow = 0;
bool connection_check = false;
bool connected = true;
bool alarm_confirm = false;
int disconnect_cnt = 0;
u32 alarm_count = 0;
int32_t prev_left_pos;
int32_t prev_right_pos;
float vxy, vth;
float x, y, th, temp_x, temp_y;
float test_ss = 1.0;

char *warn_group1[] = {"ERROR_EXPEND", "ERROR_ABZ", "ERROR_UVW", "ERROR_COUNT",
                       "ERROR_DTEMPER", "ERROR_OVOL", "ERROR_LVOL", "ERROR_OCUR",
                       "ERROR_RES", "ERROR_POS", "ERROR_LLOW", "ERROR_IIT", "ERROR_HFRE",
                       "ERROR_MTEMPER", "ERROR_COMM", "ERROR_EEPROM"};

char *warn_group2[] = {"ERROR_CSEN", "ERROR_WDOG", "ERROR_IR", "ERROR_MCU",
                       "ERROR_MSET", "ERROR_LOUT", "ERROR_STO1", "ERROR_STO2",
                       "ERROR_OD", "ERROR_PS", "ERROR_NP", "ERROR_SPI", "ERROR_CAN",
                       "ERROR_CORI", "ERROR_ABZ", "ERROR_COUNT"};

float pose_cov_move[36] = {1e-3, 0, 0, 0, 0, 0,
                           0, 1e-3, 0, 0, 0, 0,
                           0, 0, 1e6, 0, 0, 0,
                           0, 0, 0, 1e6, 0, 0,
                           0, 0, 0, 0, 1e6, 0,
                           0, 0, 0, 0, 0, 1e3};
float twist_cov_move[36] = {1e-3, 0, 0, 0, 0, 0,
                            0, 1e-3, 0, 0, 0, 0,
                            0, 0, 1e6, 0, 0, 0,
                            0, 0, 0, 1e6, 0, 0,
                            0, 0, 0, 0, 1e6, 0,
                            0, 0, 0, 0, 0, 1e3};
float pose_cov_frez[36] = {1e-9, 0, 0, 0, 0, 0,
                           0, 1e-3, 1e-9, 0, 0, 0,
                           0, 0, 1e6, 0, 0, 0,
                           0, 0, 0, 1e6, 0, 0,
                           0, 0, 0, 0, 1e6, 0,
                           0, 0, 0, 0, 0, 1e-9};
float twist_cov_frez[36] = {1e-9, 0, 0, 0, 0, 0,
                            0, 1e-3, 1e-9, 0, 0, 0,
                            0, 0, 1e6, 0, 0, 0,
                            0, 0, 0, 1e6, 0, 0,
                            0, 0, 0, 0, 1e6, 0,
                            0, 0, 0, 0, 0, 1e-9};
