#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
#include "IMU.h"


#define LED1        (P20_9)
#define KEY1        (P11_3)

#define MY_PI       3.1415926535f

#define RAD_TO_DEG  (180.0f / MY_PI)
#define DEG_TO_RAD  (MY_PI / 180.0f)

uint8 g_cal_req = 0;
int core0_main(void)
{
    clock_init();

    debug_init();
    ips200_init(IPS200_TYPE_SPI);
    ips200_clear();
    gpio_init(LED1, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    while(1)
    {
       if(imu660ra_init())
           printf("\r\n IMU660RA init error.");
       else
           break;
       gpio_toggle_level(LED1);
    }

    key_init(10);
    load_offset();
    pit_ms_init(CCU60_CH0, 5);
    cpu_wait_event_ready();
    while (TRUE)
    {
        /*---------- 有校准请求就执行 ----------*/
        /* g_cal_req 由 5ms 中断在检测到按键时置 1。
         * 这里立刻清 0 再调 do_calibrate()，
         * 保证一次按键只触发一次校准。 */
        if (g_cal_req)
        {
            g_cal_req = 0;
            do_calibrate();
            /*等10秒获得offset */
        }

        imu_show_number();
        // 显示三个姿态角

        gpio_toggle_level(LED1);
        /* 每轮翻转一次 LED，用来看程序有没有卡死 */

        system_delay_ms(50);
    }
}

/*==================================================================
 * 5ms 定时中断
 * -----------------------------------------------------------------
 * 触发：pit_ms_init(CCU60_CH0, 5) 配置的定时器每 5ms 产生一次中断
 * 优先级：CCU6_0_CH0_ISR_PRIORITY（在 isr_config.h 里定义为 30）
 *
 * 【IFX_INTERRUPT 这个宏做三件事】
 *   1. 声明一个普通 C 函数 cc60_pit_ch0_isr
 *   2. 把它的地址填进中断向量表
 *   3. 第二个参数 0 表示由 CPU0 处理
 *
 * 【中断里只能做"短、快"的事】
 *   绝不延时、绝不打印、绝不 while 等标志
 *   否则会把其他中断全堵住，系统卡死
 *================================================================*/
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    static uint8 key_div = 0;
    /* 按键扫描分频计数器。
     * 【为什么需要它】
     *   库里 key_init(10) 要求 key_scanner() 每 10ms 调一次，
     *   但这个中断是 5ms 一次。
     *   所以每进两次中断才真正扫一次按键：
     *       第 1 次：key_div 变 1，不扫
     *       第 2 次：key_div 变 2，满足条件 -> 清零 + 扫描
     *   static 让它跨调用保持值（不会被重新初始化）。 */

    interrupt_global_enable(0);
    /* 允许更高优先级的中断打断我（中断嵌套）。
     * 不写的话，别的中断只能等这个跑完。 */

    pit_clear_flag(CCU60_CH0);
    /* 【必须清中断标志】
     * 硬件定时到了会置一个标志位，不清的话它会一直是 1，
     * 中断会立刻再次触发，程序就永远在这个中断里出不来。 */

    /*========== 按键扫描 ==========*/
    if (++ key_div >= 2)
    {
        key_div = 0;
        key_scanner();
        /* 库的按键扫描函数。它内部做三件事：
         *   1. 读四个引脚的电平
         *   2. 消抖（连续若干次采样一致才认为是真按下）
         *   3. 更新每个键的状态：KEY_RELEASE / KEY_SHORT_PRESS / KEY_LONG_PRESS
         * 必须保持 10ms 节奏调用，否则时间算错。 */
    }

    /*========== 检测按键  ==========*/
    if (KEY_SHORT_PRESS == key_get_state(KEY_4))
    {
        key_clear_state(KEY_4);
        /*清状态 */

        g_cal_req = 1;
        /* 只置标志，不干活 */
    }

    /*========== 读 IMU ==========*/
    imu660ra_get_acc();
    /* 读三轴加速度原始值 -> imu660ra_acc_x / y / z（int16）
     * 通过 SPI 传输，大约耗时 69us。
     * 目前这些值还没被用到（因为我们只用陀螺仪积分），
     * 但读一下没坏处，以后要做加速度计融合时直接用。 */

    imu660ra_get_gyro();
    /* 读三轴角速度原始值 -> imu660ra_gyro_x / y / z（int16）
     * 大约耗时 69us。这是姿态解算的核心数据。 */

    /*========== 姿态积分 ==========*/
    imu_i_angle();
}

#pragma section all restore
