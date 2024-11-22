#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
// #include <unistd.h>

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

void motor_interface(int motor_command) {
    // motor command에 따른 출력
}

int move_forward(int* is_forward) {
    int motor_command = -10e9;
    // 배열 0번 인덱스 앞 여부, 1번 인덱스 뒤 여부 판단 command 반환

    return motor_command;
}

void turn_left(int trigger) {
    int motor_command = 1;
    // trigger 발생 시에만 호출되는 함수
    printf("turn_left get a trigger with %d\n", trigger);
    if (trigger == 1){
        motor_interface(motor_command);
    } else {
        fprintf(stderr, "An invalid trigger has occurred.");
        return -1;
    }
}

void turn_right(int trigger) {
    int motor_command = 2;
    // trigger 발생 시에만 호출되는 함수
    printf("turn_right get a trigger with %d\n", trigger);
    if (trigger == 1) {
        motor_interface(motor_command);
    } else {
        fprintf(stderr, "An invalid trigger has occurred.");
        return -1;
    }
}

void move_backward(int trigger) {
    int motor_command = 3;
    // trigger 발생 시에만 호출되는 함수
    printf("move_backward get a trigger with %d\n", trigger);
    if (trigger == 1) {
        motor_interface(motor_command);
    } else {
        fprintf(stderr, "An invalid trigger has occurred.");
        return -1;
    }
    return 0;
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

void stop() {
    printf("stop>> motor_command : DISABLE, cleaner_command : OFF\n");
    // disable MoveForward
    motor_interface(-1);
    // Cleaner Command(OFF)
    cleaner_interface(1);
}

void init_trigger(int* lt, int* rt, int* bt) {
    printf("init_trigger>> Before trigger value(left, right, back) : (%d, %d, %d)\n", *lt, *rt, *bt);
    *lt = 0;
    *rt = 0;
    *bt = 0;
    printf("init_trigger>> After trigger value(left, right, back) : (%d, %d, %d)\n", *lt, *rt, *bt);
}

void controller() {
    int obstacle_location[3] = { 0 };
    int dust_existence[1] = { 0 };
    int left_trigger = 1;
    int right_trigger = 1;
    int back_trigger = 1;
    int f, l, r, d;
    int motor_command = -1; // disable
    int cleaner_command = 1; // off
    int tick = 1;
    time_t last_time = time(NULL);

    while (1) {
        time_t cur_time = time(NULL);

        if(difftime(cur_time, last_time) >= tick){
            printf("\n------------------------ cycle start ------------------------\n");
            last_time = cur_time;

            if (back_trigger == 1) {
                init_trigger(&left_trigger, &right_trigger, &back_trigger);
                motor_command = -1; // disable
                motor_interface(motor_command); // 정지

                f = 1; // 앞에 장애물 있다고 고정. 없어도 됨 차피 안씀
                l = left_sensor_interface();
                r = right_sensor_interface();
                determine_obstacle_location(*obstacle_location, f, l, r);

                if (obstacle_location[1] == 0 && obstacle_location[2] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                    printf("뒤로 이동 후 RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    left_trigger = 1;
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // 장애물 없음 -> 왼쪽 회전
                    printf("뒤로 이동 후 RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    left_trigger = 1;
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // 좌만 장애물 -> 오른쪽 회전
                    printf("뒤로 이동 후 RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    right_trigger = 1;
                }

                turn_left(left_trigger);
                turn_right(right_trigger);
            }

            init_trigger(&left_trigger, &right_trigger, &back_trigger);

            stop();

            f = front_sensor_interface();
            l = left_sensor_interface();
            r = right_sensor_interface();
            d = dust_sensor_interface();

            determine_obstacle_location(*obstacle_location, f, l, r);
            motor_command = obstacle_location[0] == 1 ? -1 : 0;
            determine_dust_existence(*dust_existence, d);

            if (motor_command == -1) { // 앞에 장애물이 있는 경우
                printf("RVC앞에 장애물이 있습니다. 앞쪽 장애물 유무 %d\n", obstacle_location[0]);
                motor_interface(motor_command); // disable
                cleaner_command = 1; // off
                cleaner_interface(cleaner_command);

                if (obstacle_location[1] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                    printf("RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    left_trigger = 1;
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                    printf("RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    right_trigger = 1;
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                    printf("RVC가 뒤쪽으로 이동해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", obstacle_location[1], obstacle_location[2]);
                    back_trigger = 1;
                }

                if (back_trigger == 1) {
                    move_backward(back_trigger);
                } else {
                    turn_left(left_trigger);
                    turn_right(right_trigger);
                }
            }
            else { // 앞에 장애물이 없는 경우 + motor_command 오류
                printf("RVC가 앞으로 이동합니다. 앞쪽 장애물 유무 %d\n", obstacle_location[0]);
                motor_command = 0; // enable
                motor_interface(motor_command);
                cleaner_command = 0; // on
                cleaner_interface(cleaner_command);
                if (dust_existence[0] == 1) {
                    printf("먼지가 많습니다. 파워를 높여야 합니다. 먼지 많음 정도 %d\n", dust_existence[0]);
                    cleaner_command = 2; // up
                    cleaner_interface(cleaner_command);
                }
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
