#include <esc_pwm.h>

PwmOut esc_pwm(ESCPin);

void esc_init() {
    esc_pwm.pulsewidth_us(0);
}

void setup_pwm() {
    esc_pwm.period(5);
}

void arm(Serial* pc) {
    pc->printf("Start arming\n");
    // 1. PWM=0
    // esc_pwm.pulsewidth_us(0);
    // wait(1);
    // 2. PWM != 0
    esc_pwm.pulsewidth_us(MIN_THROTTLE+100);
    wait(0.3);
    pc->printf("Put signal\n");
    // 3. PWM = 0. Armed
    esc_pwm.pulsewidth_us(MIN_THROTTLE);
    wait(1);    // if too short, it is not detected by esc
    pc->printf("Armed\n");
    return;
}

void setupLimits(Serial* pc) {
    pc->printf("Start setup");
    // 1. PWM = 0
    esc_pwm.pulsewidth_us(0);
    wait(0.5);
    pc->printf("Disarmed");
    // 2. PWM = 2000us - peak throttle
    esc_pwm.pulsewidth_us(MAX_THROTTLE);
    wait(4);
    pc->printf("Max throttle set");
    // 3. MAX is set PWM=1000 - min throttle
    esc_pwm.pulsewidth_us(MIN_THROTTLE);
    wait(4);
    pc->printf("Min set");
    // 4. Calibration done. Values are saved.
    // esc_pwm.pulsewidth_us(0);
    // wait(1);
    pc->printf("Calibration done");
}

void esc_write(int val) {
    esc_pwm.pulsewidth_us(val);
}