/**
 * servo.h:
 * Updated on 4/5/2024
 *
 *  @author Jason Di Giovanni, Brant Gicante
 *  @date 4/5/2024
 *
 *
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inc/tm4c123gh6pm.h>
#include "Timer.h"

void servo_init(void);

void servo_move(unit16_t degrees);

#endif /* SERVO_H_ */
