#ifndef __STEPCOUNTER_H
#define __STEPCOUNTER_H

#include <stdbool.h>
#include <stdint.h>
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "lcd.h"
#include "delay.h"
#include "led.h"
#include "sys.h"
#include "timer.h"

void stepcounter_main(void);

#endif /* __STEPCOUNTER_H */
