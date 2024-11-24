#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
// #include <unistd.h>

#define ON 0
#define OFF 1
#define UP 2

#define ENABLE 0
#define DISABLE 1

#define LEFT_TRIGGER 0
#define RIGHT_TRIGGER 1
#define BACK_TRIGGER 2

#define STOP -1
#define FORWARD 0
#define LEFT 1
#define RIGHT 2
#define BACKWARD 3

#define F 0
#define L 1
#define R 2
#define D 0

typedef struct {
    int f;
    int l;
    int r;
    int d;
    int trigger[3]; // l, r, b
    int obstacle_location[3]; // f, l, r
    int dust_existence[1]; // d
    int is_forward[2]; // enable, disable
} Sensor;


void determine_obstacle_location(int* obstacle_location, int f, int l, int r) {
    obstacle_location[0] = f; // 앞 센서 값
    obstacle_location[1] = l; // 왼쪽 센서 값
    obstacle_location[2] = r; // 오른쪽 센서 값
}


void determine_dust_existence(int* dust_existence, int d) {
    *dust_existence = d; // 먼지 센서 값
}

// 앞 센서 인터페이스
int front_sensor_interface() {
    return rand() % 2; // 0 또는 1 반환 (장애물 존재 여부)
}

// 왼쪽 센서 인터페이스
int left_sensor_interface() {
    return rand() % 2; // 0 또는 1 반환 (장애물 존재 여부)
}

// 오른쪽 센서 인터페이스
int right_sensor_interface() {
    return rand() % 2; // 0 또는 1 반환 (장애물 존재 여부)
}

// 먼지 센서 인터페이스
int dust_sensor_interface() {
    return rand() % 2; // 0 또는 1 반환 (먼지 존재 여부)
}

void motor_interface(int motor_command) {
    // motor command에 따른 출력
    switch (motor_command) {
        case STOP:
            printf("Motor: Stop\n");
            break;
        case FORWARD:
            printf("Motor: Move Forward\n");
            break;
        case LEFT:
            printf("Motor: Turn Left\n");
            break;
        case RIGHT:
            printf("Motor: Turn Right\n");
            break;
        case BACKWARD:
            printf("Motor: Move Backward\n");
            break;
        default:
            fprintf(stderr, "Motor: Invalid Command (%d)\n", motor_command);
    }
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
    if (trigger == 1) {
        motor_interface(motor_command);
    }
    else {
        fprintf(stderr, "An invalid trigger has occurred.\n");
        return -1;
    }
}

void turn_right(int trigger) {
    int motor_command = 2;
    // trigger 발생 시에만 호출되는 함수
    printf("turn_right get a trigger with %d\n", trigger);
    if (trigger == 1) {
        motor_interface(motor_command);
    }
    else {
        fprintf(stderr, "An invalid trigger has occurred.\n");
    }
}

void move_backward(int trigger) {
    int motor_command = 3;
    // trigger 발생 시에만 호출되는 함수
    printf("move_backward get a trigger with %d\n", trigger);
    if (trigger == 1) {
        motor_interface(motor_command);
    }
    else {
        fprintf(stderr, "An invalid trigger has occurred.\n");
    }
}

int power(int is_power) {
    int cleaner_command = -10e9;
    // is_power(on/off/up)에 따른 command 반환

    return cleaner_command;
}

void cleaner_interface(int cleaner_command) {
    // command에 따른 출력
    switch (cleaner_command) {
        case OFF:
            printf("Cleaner: Power Off\n");
            break;
        case ON:
            printf("Cleaner: Power On\n");
            break;
        case UP:
            printf("Cleaner: Power Up (Boost Mode)\n");
            break;
        default:
            fprintf(stderr, "Cleaner: Invalid Command (%d)\n", cleaner_command);
    }
}

int divider(int* is_forward, int cleaner_command) {
    // 2개 동작 수행해야할 때 호출

}

void stop() {
    printf("\n--------------------------------------------------------\n");
    printf("RVC가 STOP에 진입했습니다.\n");
}

void initialization(Sensor* d) {
    for (int i = 0; i < 3; i++) {
        d->trigger[i] = 0;
    }
    d->f = front_sensor_interface();
    d->l = left_sensor_interface();
    d->r = right_sensor_interface();
    d->d = dust_sensor_interface();

    for (int i = 0; i < 3; i++) {
        d->obstacle_location[i] = 0;
    }

    d->dust_existence[0] = 0;
}

void update_sensor_data(Sensor* data) {
    initialization(data);
    determine_obstacle_location(data->obstacle_location, data->f, data->l, data->r);
    determine_dust_existence(data->dust_existence, data->d);
}


void controller() {
    Sensor data;
    int motor_command = STOP;
    int cleaner_command = OFF;
    int tick = 1;
    data.is_forward[ENABLE] = 0;
    data.is_forward[DISABLE] = 1;

    // 처음 작동시 해야할 것
    move_forward(data.is_forward);  // disable MoveForward
    power(1); // Cleaner Command(OFF)
    stop();
    // 첫 번째 시간 저장 (현재 시간으로 초기화)
    time_t current_time;
    time_t last_first_time;
    time_t last_second_time;
    time_t last_third_time;
    time_t last_time;  // 첫 번째 Tick을 위한 시간 저장


    while (1) { // 전체 사이클을 반복
        
        last_first_time = time(NULL);
        while (1) { // 1틱 stop -> turn or MoveForward
            current_time = time(NULL);
            if (difftime(current_time, last_first_time) >= tick) {
                last_first_time = current_time;
                update_sensor_data(&data);

                if (data.obstacle_location[F] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                    power(OFF);
                    printf("Turn에 진입합니다.\n");
                    break;
                }
                else { // 앞에 장애물이 없는 경우 + motor_command 오류
                    data.is_forward[ENABLE] = 1; // enable : true
                    data.is_forward[DISABLE] = 0; // disable : false
                    move_forward(data.is_forward);
                    power(ON);
                    printf("MoveForward에 진입합니다.\n");
                    break;
                }
            }
        }

        if (data.obstacle_location[F] == 1) { // 2틱 turn
            last_second_time = time(NULL);
            while (1) {
                current_time = time(NULL);
                if (difftime(current_time, last_second_time) >= tick) {
                    last_second_time = current_time;
                    update_sensor_data(&data);
                    data.obstacle_location[0] = 1; // dfd 처럼 할려면 f 고정하고 해야할듯

                    if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                        printf("Turn Left에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                        data.trigger[LEFT_TRIGGER] = 1;
                        power(OFF);
                        break;
                    }
                    else if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 1 && data.obstacle_location[R] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                        printf("Turn Right에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                        data.trigger[RIGHT_TRIGGER] = 1;
                        power(OFF);
                        break;
                    }
                    else if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 1 && data.obstacle_location[R] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                        printf("MoveBackward에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                        data.trigger[BACK_TRIGGER] = 1;
                        power(OFF);
                        break;
                    }
                    else {
                        power(ON);
                    }
                }
            }

            if (data.trigger[BACK_TRIGGER] == 1) {
                move_backward(data.trigger[BACK_TRIGGER]);
                last_time = time(NULL);
                while (1) { // 장애물이 없을 때까지 뒤로 이동 틱 계속 증가할 수 있음.
                    current_time = time(NULL);
                    if (difftime(current_time, last_time) >= tick) {
                        last_time = current_time;
                        update_sensor_data(&data);

                        if (data.obstacle_location[L] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[LEFT_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else if (data.obstacle_location[L] == 0 && data.obstacle_location[R] == 0) { // 장애물 없음 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[LEFT_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else if (data.obstacle_location[L] == 1 && data.obstacle_location[R] == 0) { // 좌만 장애물 -> 오른쪽 회전
                            printf("뒤로 이동 중... RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[RIGHT_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else {
                            printf("뒤로 이동합니다...\n");
                            power(OFF);
                            data.trigger[BACK_TRIGGER] = 1;
                            move_backward(data.trigger[BACK_TRIGGER]);
                        }
                    }
                }
                last_third_time = time(NULL);
                if (data.trigger[LEFT_TRIGGER] == 1) {
                    int count = 1;
                    turn_left(data.trigger[LEFT_TRIGGER]); // 일단 회전
                    while (count < 6) { // 3틱 좌회전
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            update_sensor_data(&data);

                            data.trigger[LEFT_TRIGGER] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(OFF);
                            turn_left(data.trigger[LEFT_TRIGGER]); // 쓸거면 고정하고 써야함
                            count++;
                        }
                    }
                }
                else if (data.trigger[RIGHT_TRIGGER] == 1) {
                    int count = 1;
                    turn_right(data.trigger[RIGHT_TRIGGER]); // 일단 회전
                    while (count < 6) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            update_sensor_data(&data);

                            data.trigger[RIGHT_TRIGGER] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(OFF);
                            turn_right(data.trigger[RIGHT_TRIGGER]); // 쓸거면 고정하고 써야함
                            count++;
                        }
                    }
                }
            }
            else { // 3틱 회전 left or right
                last_third_time = time(NULL);
                if (data.trigger[LEFT_TRIGGER] == 1) {
                    int count = 1;
                    turn_left(data.trigger[LEFT_TRIGGER]); // 일단 회전
                    while (count < 6) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            update_sensor_data(&data);

                            data.trigger[LEFT_TRIGGER] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(OFF);
                            turn_left(data.trigger[LEFT_TRIGGER]); // 쓸거면 고정하고 써야함
                            count++;
                        }
                    }
                }
                else if (data.trigger[RIGHT_TRIGGER] == 1) {
                    int count = 1;
                    turn_right(data.trigger[RIGHT_TRIGGER]); // 일단 회전
                    while (count < 6) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            update_sensor_data(&data);

                            data.trigger[RIGHT_TRIGGER] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(OFF);
                            turn_right(data.trigger[RIGHT_TRIGGER]); // 쓸거면 고정하고 써야함
                            count++;
                        }
                    }
                }
            }
        }
        else { // data.obstacle_location[0] != 1
            last_second_time = time(NULL);
            while (1) { // 2틱 moveforward
                current_time = time(NULL);
                if (difftime(current_time, last_second_time) >= tick) {
                    last_first_time = current_time;
                    update_sensor_data(&data);

                    if (data.obstacle_location[F] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                        power(OFF);
                        data.is_forward[ENABLE] = 0; // enable : false
                        data.is_forward[DISABLE] = 1; // disable : ture
                        move_forward(data.is_forward);

                        printf("Turn을 해야 합니다.\n");
                        break;
                    }
                    else { // 장애물이 없는 경우 moveforward 유지 or 파워 업
                        last_time = time(NULL);
                        while (1) { // 앞에 장애물이 없으면 계속 청소함. 틱 계속 증가할 수 있음.
                            current_time = time(NULL);
                            if (difftime(current_time, last_time) >= tick) {
                                last_time = current_time;
                                update_sensor_data(&data);

                                if (data.obstacle_location[F] == 1) { // 앞에 장애물 있음
                                    power(OFF);
                                    data.is_forward[ENABLE] = 0; // enable : false
                                    data.is_forward[DISABLE] = 1; // disable : ture
                                    move_forward(data.is_forward);
                                    printf("RVC 청소 중 장애물 발견 Turn을 해야 합니다.\n");
                                    break;
                                }
                                else if (data.dust_existence[D] == 1) { // 장애물 없고, 먼지 있음
                                    power(UP); // up
                                }
                                else {
                                    power(ON); // on
                                }
                            }
                        }
                        break;
                    }
                }
            }

            if (data.obstacle_location[F] == 1) { // 2틱 turn
                last_second_time = time(NULL);
                while (1) {
                    current_time = time(NULL);
                    if (difftime(current_time, last_second_time) >= tick) {
                        last_second_time = current_time;
                        update_sensor_data(&data);
                        data.obstacle_location[F] = 1; // dfd 처럼 할려면 f 고정하고 해야할듯

                        if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                            printf("Turn Left에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[LEFT_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 1 && data.obstacle_location[R] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                            printf("Turn Right에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[RIGHT_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else if (data.obstacle_location[F] == 1 && data.obstacle_location[L] == 1 && data.obstacle_location[R] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                            printf("MoveBackward에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[L], data.obstacle_location[R]);
                            data.trigger[BACK_TRIGGER] = 1;
                            power(OFF);
                            break;
                        }
                        else {
                            power(ON);
                        }
                    }
                }

                if (data.trigger[BACK_TRIGGER] == 1) {
                    move_backward(data.trigger[BACK_TRIGGER]);
                    last_time = time(NULL);
                    while (1) { // 장애물이 없을 때까지 뒤로 이동 틱 계속 증가할 수 있음.
                        current_time = time(NULL);
                        if (difftime(current_time, last_time) >= tick) {
                            last_time = current_time;
                            update_sensor_data(&data);

                            if (data.obstacle_location[L] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                                printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                                data.trigger[LEFT_TRIGGER] = 1;
                                power(OFF);
                                break;
                            }
                            else if (data.obstacle_location[L] == 0 && data.obstacle_location[R] == 0) { // 장애물 없음 -> 왼쪽 회전
                                printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                                data.trigger[LEFT_TRIGGER] = 1;
                                power(OFF);
                                break;
                            }
                            else if (data.obstacle_location[L] == 1 && data.obstacle_location[R] == 0) { // 좌만 장애물 -> 오른쪽 회전
                                printf("뒤로 이동 중... RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                                data.trigger[RIGHT_TRIGGER] = 1;
                                power(OFF);
                                break;
                            }
                            else {
                                printf("뒤로 이동합니다...\n");
                                power(OFF);
                                data.trigger[BACK_TRIGGER] = 1;
                                move_backward(data.trigger[BACK_TRIGGER]);
                            }
                        }
                    }
                    last_third_time = time(NULL);
                    if (data.trigger[LEFT_TRIGGER] == 1) {
                        int count = 1;
                        turn_left(data.trigger[LEFT_TRIGGER]); // 일단 회전
                        while (count < 6) { // 3틱 좌회전
                            current_time = time(NULL);
                            if (difftime(current_time, last_third_time) >= tick) {
                                last_third_time = current_time;
                                update_sensor_data(&data);

                                data.trigger[LEFT_TRIGGER] = 1;
                                printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                                power(OFF);
                                turn_left(data.trigger[LEFT_TRIGGER]); // 쓸거면 고정하고 써야함
                                count++;
                            }
                        }
                    }
                    else if (data.trigger[RIGHT_TRIGGER] == 1) {
                        int count = 1;
                        turn_right(data.trigger[RIGHT_TRIGGER]); // 일단 회전
                        while (count < 6) {
                            current_time = time(NULL);
                            if (difftime(current_time, last_third_time) >= tick) {
                                last_third_time = current_time;
                                update_sensor_data(&data);

                                data.trigger[RIGHT_TRIGGER] = 1;
                                printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                                power(OFF);
                                turn_right(data.trigger[RIGHT_TRIGGER]); // 쓸거면 고정하고 써야함
                                count++;
                            }
                        }
                    }
                }
                else { // 3틱 회전 left or right
                    last_third_time = time(NULL);
                    if (data.trigger[LEFT_TRIGGER] == 1) {
                        int count = 1;
                        turn_left(data.trigger[LEFT_TRIGGER]); // 일단 회전
                        while (count < 6) {
                            current_time = time(NULL);
                            if (difftime(current_time, last_third_time) >= tick) {
                                last_third_time = current_time;
                                update_sensor_data(&data);

                                data.trigger[LEFT_TRIGGER] = 1;
                                printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                                power(OFF);
                                turn_left(data.trigger[LEFT_TRIGGER]); // 쓸거면 고정하고 써야함
                                count++;
                            }
                        }
                    }
                    else if (data.trigger[RIGHT_TRIGGER] == 1) {
                        int count = 1;
                        turn_right(data.trigger[RIGHT_TRIGGER]); // 일단 회전
                        while (count < 6) {
                            current_time = time(NULL);
                            if (difftime(current_time, last_third_time) >= tick) {
                                last_third_time = current_time;
                                update_sensor_data(&data);

                                data.trigger[RIGHT_TRIGGER] = 1;
                                printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                                power(OFF);
                                turn_right(data.trigger[RIGHT_TRIGGER]); // 쓸거면 고정하고 써야함
                                count++;
                            }
                        }
                    }
                }
            }
        }
        stop();
    }
}

int main(void) {
    srand(time(NULL));
    printf("RVC의 전원이 켜졌습니다.\n");
    controller();
    return 0;
}
