#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void determine_obstacle_location(int* obstacle_location, int f, int l, int r) {
    // interface 호출해서 배열에 저장
}

void determine_dust_existence(int* dust_existence, int d) {

}

int front_sensor_interface() {
    // 랜덤 값 생성
}

int left_sensor_interface() {
    // 랜덤 값 생성
}

int right_sensor_interface() {
    // 랜덤 값 생성
}

int dust_sensor_interface() {
    // 랜덤 값 생성
}


int move_forward(int* is_forward) {
    int motor_command = -10e9;
    // 배열 0번 인덱스 앞 여부, 1번 인덱스 뒤 여부 판단 command 반환

    return motor_command;
}

int turn_left() {
    int motor_command = -10e9;
    // trigger 발생 시에만 호출되는 함수

    return motor_command;
}

int turn_right() {
    int motor_command = -10e9;
    // trigger 발생 시에만 호출되는 함수

    return motor_command;
}

int move_backward() {
    int motor_command = -10e9;
    // trigger 발생 시에만 호출되는 함수

    return motor_command;
}

void motor_interface(int motor_command) {
    // motor command에 따른 출력
}

int power(int is_power) {
    int cleaner_command = -10e9;
    // is_power(on/off/up)에 따른 command 반환

    return cleaner_command;
}

void cleaner_interface(int cleaner_command) {
    // command에 따른 출력
}

int divider(int* is_forward, int cleaner_command) {
    // 2개 동작 수행해야할 때 호출

}

void stop(int motor_command, int cleaner_command) {
    printf("stop>> motor_command : %d, cleaner_command : %d\n", motor_command, cleaner_command);
    // disable MoveForward
    motor_interface(motor_command);
    // Cleaner Command(OFF)
    cleaner_interface(cleaner_command);
}

void controller() {
    int obstacle_location[3] = { 0 };
    int dust_existence[1] = { 0 };
    int trigger = 0;
    int f, l, r, d;
    int motor_command = -1; // disable
    int cleaner_command = 1; // off

    stop(motor_command, cleaner_command);

    while (1) {
        f = front_sensor_interface();
        l = left_sensor_interface();
        r = right_sensor_interface();
        d = dust_sensor_interface();

        determine_obstacle_location(*obstacle_location, f, l, r);
        motor_command = obstacle_location[0] == 1 ? -1 : 0;
        determine_dust_existence(*dust_existence, d);

        if (motor_command == -1) { // 앞에 장애물이 있는 경우
            motor_interface(motor_command); // disable
            cleaner_command = 1; // off
            cleaner_interface(cleaner_command);

            if (obstacle_location[1] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                motor_command = turn_left();
                motor_interface(motor_command); // left
            }
            else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                motor_command = turn_right();
                motor_interface(motor_command); // right
            }
            else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                motor_command = move_backward();
                motor_interface(motor_command); // move_backward
                motor_command = -1; // disable
                motor_interface(motor_command); // 정지

                f = 1; // 앞에 장애물 있다고 고정. 없어도 됨 차피 안씀
                l = left_sensor_interface();
                r = right_sensor_interface();
                determine_obstacle_location(*obstacle_location, f, l, r);

                if (obstacle_location[1] == 0 && obstacle_location[2] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                    motor_command = turn_left();
                    motor_interface(motor_command); // left
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // 장애물 없음 -> 왼쪽 회전
                    motor_command = turn_left();
                    motor_interface(motor_command); // left
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // 좌만 장애물 -> 오른쪽 회전
                    motor_command = turn_right();
                    motor_interface(motor_command); // right
                }
            }
        }
        else { // 앞에 장애물이 없는 경우 + motor_command 오류
            motor_command = 0; // enable
            motor_interface(motor_command);
            cleaner_command = 0; // on
            cleaner_interface(cleaner_command);
            if (dust_existence[0] == 1) {
                cleaner_command = 2; // up
                cleaner_interface(cleaner_command);
            }
        }

    }
}

int main(void) {
    srand(time(NULL));
    printf("RVC의 전원이 켜졌습니다.\n");
    controller();
    return 0;
}