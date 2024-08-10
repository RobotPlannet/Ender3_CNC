#include "cv_laser_module.h"
#include "../gcode/gcode.h"


xyz_pos_t spindle_laser_fan_pwm::homepos = {0};
bool spindle_laser_fan_pwm::laser_enabled = false;
// float pause_before_position_x=0, pause_before_position_y=0;
uint8_t spindle_laser_fan_pwm::save_power = 255;// 激光功率

void spindle_laser_fan_pwm::init_device() {
}

// 快速停止激光 等同命令 "M5 I"
void spindle_laser_fan_pwm::quick_stop() {
    gcode.process_subcommands_now(F("M107"));
}

bool spindle_laser_fan_pwm::is_laser_device() {
    return laser_enabled;
}

bool spindle_laser_fan_pwm::is_fdm_device() {
    return !laser_enabled;
}

// 0-1000 比例转换为0-255
uint8_t spindle_laser_fan_pwm::power16_to_8(const_float_t power) {
    return power * 255 /1000;
}

// 设置并启动timer3 的pwm
void spindle_laser_fan_pwm::laser_power_start(const uint8_t power) {
    gcode.process_subcommands_now(TS(F("M106 S"), power));
}

// 将pwm占空比设置为1，以最弱的激光输出
void spindle_laser_fan_pwm::laser_power_stop() {
    laser_power_start(1);
}

// 关闭激光PWM输出， 暂停timer3中断，无激光输出
void spindle_laser_fan_pwm::laser_power_close() {
    gcode.process_subcommands_now(F("M107"));
}

// 打开激光pwm，以最弱的激光输出
void spindle_laser_fan_pwm::laser_power_open() {
    laser_power_start(2);
}

#if HAS_MEDIA
    void spindle_laser_fan_pwm::laser_range() {}

#endif

void spindle_laser_fan_pwm::laser_home() {
    gcode.process_subcommands_now(F("G0 Z5"));
}

void spindle_laser_fan_pwm::laser_set(const bool turn_on) {
    laser_enabled = turn_on;
    if(turn_on){
        // hal.set_pwm_frequency(pin_t(FAN0_PIN), 1000);
        laser_power_open();
    }else{
        // hal.set_pwm_frequency(pin_t(FAN0_PIN), 10);
        laser_power_close();
    }

}
