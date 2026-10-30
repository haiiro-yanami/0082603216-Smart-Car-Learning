#include "zf_common_headfile.h"
#include "IMU.h"

volatile float real_roll  = 0;
volatile float real_pitch = 0;
volatile float real_yaw   = 0;

uint8 calibrated = 0;

float offset_roll  = 0;
float offset_pitch = 0;
float offset_yaw   = 0;

void do_calibrate()
{

    float sum_roll  = 0;
    float sum_pitch = 0;
    float sum_yaw   = 0;

    for (uint16 i = 0 ; i < 2000 ; i++ )
    {

        float gyro_roll  = imu660ra_gyro_transition(imu660ra_gyro_x);
        float gyro_pitch = imu660ra_gyro_transition(imu660ra_gyro_y);
        float gyro_yaw   = imu660ra_gyro_transition(imu660ra_gyro_z);

        sum_roll  = sum_roll  + gyro_roll;
        sum_pitch = sum_pitch + gyro_pitch;
        sum_yaw   = sum_yaw   + gyro_yaw;

        system_delay_ms(5);
    }

    offset_roll  = sum_roll  / 2000.0f;
    offset_pitch = sum_pitch / 2000.0f;
    offset_yaw   = sum_yaw   / 2000.0f;

    if (fabsf(offset_roll) > 2 || fabsf(offset_pitch) > 2 || fabsf(offset_yaw) > 2 )
    {

        ips200_show_string(0, 140, "Error: Don't move!");

        offset_roll  = 0;
        offset_pitch = 0;
        offset_yaw   = 0;

        return;
    }

    calibrated = 1;
    save_offset();
    ips200_show_string(0, 140, "Calibrated OK!");
}

void save_offset(void)
{

    if (flash_check(0, CAL_PAGE))
    {
        flash_erase_page(0, CAL_PAGE);
    }

    flash_buffer_clear();

    flash_union_buffer[0].uint32_type = CAL_MAGIC;

    flash_union_buffer[1].float_type  = offset_roll;
    flash_union_buffer[2].float_type  = offset_pitch;
    flash_union_buffer[3].float_type  = offset_yaw;

    flash_write_page_from_buffer(0, CAL_PAGE);

    system_delay_ms(200);
}

uint8 load_offset (void)
{

    offset_roll  = 0;
    offset_pitch = 0;
    offset_yaw   = 0;

    if (flash_check(0, CAL_PAGE) == 0)
    {
        ips200_show_string(0, 100, "uncalibrated,use 0");
        return 0;
    }

    flash_read_page_to_buffer(0, CAL_PAGE);

    if (flash_union_buffer[0].uint32_type != CAL_MAGIC)
    {
        ips200_show_string(0, 100, "No calibrated dig in Flash");
        return 0;
    }

    offset_roll  = flash_union_buffer[1].float_type;
    offset_pitch = flash_union_buffer[2].float_type;
    offset_yaw   = flash_union_buffer[3].float_type;

    calibrated = 1;
    return 1;
}

void imu_i_angle (void)
{

    float gyro_z_dps = imu660ra_gyro_transition(imu660ra_gyro_z);
    float gyro_x_dps = imu660ra_gyro_transition(imu660ra_gyro_x);
    float gyro_y_dps = imu660ra_gyro_transition(imu660ra_gyro_y);

    real_yaw   = real_yaw   + (gyro_z_dps - offset_yaw)   * IMU_DT;
    real_pitch = real_pitch + (gyro_y_dps - offset_pitch) * IMU_DT;
    real_roll  = real_roll  + (gyro_x_dps - offset_roll)  * IMU_DT;

    if(real_yaw > 180.0f)    real_yaw   -= 360.0f;
    if(real_yaw < -180.0f)   real_yaw   += 360.0f;
    if(real_pitch > 180.0f)  real_pitch -= 360.0f;
    if(real_pitch < -180.0f) real_pitch += 360.0f;
    if(real_roll > 180.0f)   real_roll  -= 360.0f;
    if(real_roll < -180.0f)  real_roll  += 360.0f;
}

void imu_show_number ()
{

    ips200_show_string(0, 20, "pitch:");
    ips200_show_string(0, 40, "  yaw:");
    ips200_show_string(0, 60, " roll:");
    ips200_show_string(140, 20, "os:");
    ips200_show_string(140, 40, "os:");
    ips200_show_string(140, 60, "os:");
    ips200_show_float(180, 20, offset_roll, 1, 3);
    ips200_show_float(180, 40, offset_pitch, 1, 3);
    ips200_show_float(180, 60, offset_yaw, 1, 3);

    ips200_show_float(50, 20, real_pitch, 6 , 3 );
    ips200_show_float(50, 40, real_yaw,   6 , 3 );
    ips200_show_float(50, 60, real_roll,  6 , 3 );
}
