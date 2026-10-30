#ifndef CODE_IMU_H_
#define CODE_IMU_H_

extern volatile float real_roll;
extern volatile float real_pitch;
extern volatile float real_yaw;

#define IMU_DT   0.005f

extern float offset_roll;
extern float offset_pitch;
extern float offset_yaw;

extern uint8 calibrated;

#define CAL_PAGE    1

#define CAL_MAGIC   0x5A5A1234u

void imu_show_number (void);

void imu_i_angle (void);

uint8 load_offset (void);

void save_offset (void);

void do_calibrate (void);

#endif
