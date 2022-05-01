#include <esc_pwm.h>

PwmOut esc_pwm(ESCPin);

void setup_pwm() {
    esc_pwm.period(5);
}

void arm(Serial* pc) {
    pc->printf("Start arming\n");
    // esc_pwm.pulsewidth_us(0);
    // wait(0.2);
    // esc_pwm.pulsewidth_us(MIN_THROTTLE);
    // wait(0.2);
    // esc_pwm.pulsewidth_us(MIN_THROTTLE+10);
    // wait(0.2);
    // esc_pwm.pulsewidth_us(0);
    // wait(5);
    // esc_pwm.pulsewidth_us(MIN_THROTTLE+2);
    esc_pwm.pulsewidth_us(MIN_THROTTLE);
    // esc_pwm.write(0.9);
    // wait(10);
    wait(0.3);
    pc->printf("Min throttle setup done\n");
    esc_pwm.pulsewidth_us(MAX_THROTTLE);
    wait(0.3);
    pc->printf("Max throttle setup done\n");
    esc_pwm.pulsewidth_us(0);
    // esc_pwm.pulsewidth_us(1200);
    wait(0.1);
    pc->printf("Arming done\n");
    return;
}

void esc_write(int val) {
    esc_pwm.pulsewidth_us(val);
}