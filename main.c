#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
// #include <unistd.h>

#define MAX_LINE_LENGTH 100
FILE* test_file = NULL;

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

int is_valid_input(const char* input) {
    // 입력값 길이가 1이고, 값이 '0' 또는 '1'인지 확인
    if (strlen(input) == 1 && (input[0] == '0' || input[0] == '1')) {
        return 1; // 유효한 입력
    }
    return 0; // 무효한 입력
}

// 센서 값을 안전하게 받는 함수
int get_sensor_value(const char* sensor_name) {
    char input[10]; // 임시 입력 버퍼
    printf("Enter %s sensor value (0 or 1): ", sensor_name);

    // fgets를 사용하여 입력 받기
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read input.\n");
        exit(EXIT_FAILURE); // 프로그램 종료
    }

    // 개행 문자 제거
    input[strcspn(input, "\n")] = 0;

    // 유효성 검사
    if (!is_valid_input(input)) {
        fprintf(stderr, "Error: Invalid input. Please enter 0 or 1.\n");
        exit(EXIT_FAILURE); // 프로그램 종료
    }

    return atoi(input); // 정수로 변환 후 반환
}

// 센서 인터페이스 함수
int front_sensor_interface() {
    return get_sensor_value("front");
}

int left_sensor_interface() {
    return get_sensor_value("left");
}

int right_sensor_interface() {
    return get_sensor_value("right");
}

int dust_sensor_interface() {
    return get_sensor_value("dust");
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
    if (trigger == 1) {
        motor_interface(motor_command);
    }
    else {
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
    }
    else {
        fprintf(stderr, "An invalid trigger has occurred.");
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
        fprintf(stderr, "An invalid trigger has occurred.");
    }
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

int read_sensor_values_from_file(int* f, int* l, int* r, int* d) {
    char line[MAX_LINE_LENGTH];

    if (fgets(line, sizeof(line), test_file) == NULL) {
        return -1; // EOF
    }

    if (sscanf(line, "%d %d %d %d", f, l, r, d) != 4) {
        fprintf(stderr, "Error: Invalid test case format in file.\n");
        return 0; // Format error
    }

    return 1; // Success
}

// 수정된 initialization_test 함수
int initialization_test(Sensor* d) {
    int result = read_sensor_values_from_file(&d->f, &d->l, &d->r, &d->d);

    if (result == -1) {
        printf("End of test cases. Exiting loop.\n");
        return 0; // EOF, 루프 종료 신호
    }
    else if (result == 0) {
        fprintf(stderr, "Error: Invalid test case format. Skipping...\n");
        return 1; // Format error, 다음 케이스로 진행
    }

    for (int i = 0; i < 3; i++) {
        d->trigger[i] = 0;
        d->obstacle_location[i] = 0;
    }
    d->dust_existence[0] = 0;

    printf("Initialized with: Front=%d, Left=%d, Right=%d, Dust=%d\n", d->f, d->l, d->r, d->d);
    return 1; // Success
}



void controller() {
    Sensor data;
    int motor_command = -1; // disable
    int cleaner_command = 1; // off
    int tick = 1;
    data.is_forward[0] = 0;
    data.is_forward[1] = 1;

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
                initialization(&data);
                determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                determine_dust_existence(data.dust_existence, data.d);

                if (data.obstacle_location[0] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                    power(1); // off
                    printf("Turn에 진입합니다.\n");
                    break;
                }
                else { // 앞에 장애물이 없는 경우 + motor_command 오류
                    data.is_forward[0] = 1; // enable : true
                    data.is_forward[1] = 0; // disable : false
                    move_forward(data.is_forward);
                    power(0); // on
                    printf("MoveForward에 진입합니다.\n");
                    break;
                }
            }
        }

        if (data.obstacle_location[0] == 1) { // 2틱 turn
            last_second_time = time(NULL);
            while (1) {
                current_time = time(NULL);
                if (difftime(current_time, last_second_time) >= tick) {
                    last_second_time = current_time;
                    initialization(&data);
                    determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                    determine_dust_existence(data.dust_existence, data.d);
                    data.obstacle_location[0] = 1; // dfd 처럼 할려면 f 고정하고 해야할듯

                    if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                        printf("Turn Left에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[0] = 1;
                        power(1); // off
                        break;
                    }
                    else if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 1 && data.obstacle_location[2] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                        printf("Turn Right에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[1] = 1;
                        power(1); // off
                        break;
                    }
                    else if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 1 && data.obstacle_location[2] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                        printf("MoveBackward에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[2] = 1;
                        power(1); // off
                        break;
                    }
                    else {
                        power(0);
                    }
                }
            }

            if (data.trigger[2] == 1) {
                move_backward(data.trigger[2]);
                last_time = time(NULL);
                while (1) { // 장애물이 없을 때까지 뒤로 이동 틱 계속 증가할 수 있음.
                    current_time = time(NULL);
                    if (difftime(current_time, last_time) >= tick) {
                        last_time = current_time;
                        initialization(&data);
                        determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                        determine_dust_existence(data.dust_existence, data.d);

                        if (data.obstacle_location[1] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[0] = 1;
                            power(1); // off
                            break;
                        }
                        else if (data.obstacle_location[1] == 0 && data.obstacle_location[2] == 0) { // 장애물 없음 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[0] = 1;
                            power(1); // off
                            break;
                        }
                        else if (data.obstacle_location[1] == 1 && data.obstacle_location[2] == 0) { // 좌만 장애물 -> 오른쪽 회전
                            printf("뒤로 이동 중... RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[1] = 1;
                            power(1); // off
                            break;
                        }
                        else {
                            printf("뒤로 이동합니다...\n");
                            power(1); // off
                            data.trigger[2] = 1;
                            move_backward(data.trigger[2]);
                        }
                    }
                }
                last_third_time = time(NULL);
                if (data.trigger[0] == 1) {
                    int count = 1;
                    turn_left(data.trigger[0]); // 일단 회전
                    while (1) { // 3틱 좌회전
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[0] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_left(data.trigger[0]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
                else if (data.trigger[1] == 1) {
                    int count = 1;
                    turn_right(data.trigger[1]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_right(data.trigger[1]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
            }
            else { // 3틱 회전 left or right
                last_third_time = time(NULL);
                if (data.trigger[0] == 1) {
                    int count = 1;
                    turn_left(data.trigger[0]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_left(data.trigger[0]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
                else if (data.trigger[1] == 1) {
                    int count = 1;
                    turn_right(data.trigger[1]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_right(data.trigger[1]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
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
                    initialization(&data);
                    determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                    determine_dust_existence(data.dust_existence, data.d);

                    if (data.obstacle_location[0] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                        power(1);
                        data.is_forward[0] = 0; // enable : false
                        data.is_forward[1] = 1; // disable : ture
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
                                initialization(&data);
                                determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                                determine_dust_existence(data.dust_existence, data.d);

                                if (data.obstacle_location[0] == 1) { // 앞에 장애물 있음
                                    power(1);
                                    data.is_forward[0] = 0; // enable : false
                                    data.is_forward[1] = 1; // disable : ture
                                    move_forward(data.is_forward);
                                    printf("RVC 청소 중 장애물 발견 Turn을 해야 합니다.\n");
                                    break;
                                }
                                else if (data.dust_existence[0] == 1) { // 장애물 없고, 먼지 있음
                                    power(2); // up
                                }
                                else {
                                    power(0); // on
                                }
                            }
                        }
                        break;
                    }
                }
            }

            if (data.obstacle_location[0] == 1) //  turn 이거 반복되는 2틱 턴의 과정임
            {

            }
        }
    }
}


void controller_test() {
    Sensor data;
    int motor_command = -1; // disable
    int cleaner_command = 1; // off
    int tick = 1;
    data.is_forward[0] = 0;
    data.is_forward[1] = 1;

    const char* filename = "test_cases.txt";
    test_file = fopen(filename, "r");
    if (!test_file) {
        fprintf(stderr, "Error: Failed to open file '%s'.\n", filename);
        exit(EXIT_FAILURE);
    }


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
        if (!initialization_test(&data)) {
            break; // EOF reached, exit loop
        }
        last_first_time = time(NULL);
        while (1) { // 1틱 stop -> turn or MoveForward
            current_time = time(NULL);
            if (difftime(current_time, last_first_time) >= tick) {
                last_first_time = current_time;
                initialization_test(&data);
                determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                determine_dust_existence(data.dust_existence, data.d);

                if (data.obstacle_location[0] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                    power(1); // off
                    printf("Turn에 진입합니다.\n");
                    break;
                }
                else { // 앞에 장애물이 없는 경우 + motor_command 오류
                    data.is_forward[0] = 1; // enable : true
                    data.is_forward[1] = 0; // disable : false
                    move_forward(data.is_forward);
                    power(0); // on
                    printf("MoveForward에 진입합니다.\n");
                    break;
                }
            }
        }

        if (data.obstacle_location[0] == 1) { // 2틱 turn
            last_second_time = time(NULL);
            while (1) {
                current_time = time(NULL);
                if (difftime(current_time, last_second_time) >= tick) {
                    last_second_time = current_time;
                    initialization_test(&data);
                    determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                    determine_dust_existence(data.dust_existence, data.d);
                    data.obstacle_location[0] = 1; // dfd 처럼 할려면 f 고정하고 해야할듯

                    if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 0) { // obstacle_location[2] == 1 우 장애물 or obstacle_location[2] == 0) -> 왼쪽 회전
                        printf("Turn Left에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[0] = 1;
                        power(1); // off
                        break;
                    }
                    else if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 1 && data.obstacle_location[2] == 0) { // 좌 장애물, 우 장애물 X -> 오른 쪽 회전
                        printf("Turn Right에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[1] = 1;
                        power(1); // off
                        break;
                    }
                    else if (data.obstacle_location[0] == 1 && data.obstacle_location[1] == 1 && data.obstacle_location[2] == 1) { // 좌, 우 장애물 -> 뒤로 이동
                        printf("MoveBackward에 진입합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                        data.trigger[2] = 1;
                        power(1); // off
                        break;
                    }
                    else {
                        power(0);
                    }
                }
            }

            if (data.trigger[2] == 1) {
                move_backward(data.trigger[2]);
                last_time = time(NULL);
                while (1) { // 장애물이 없을 때까지 뒤로 이동 틱 계속 증가할 수 있음.
                    current_time = time(NULL);
                    if (difftime(current_time, last_time) >= tick) {
                        last_time = current_time;
                        initialization_test(&data);
                        determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                        determine_dust_existence(data.dust_existence, data.d);

                        if (data.obstacle_location[1] == 0) { // 좌, 우 장애물 없음 -> 왼쪽 회전 or 우만 장애물 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[0] = 1;
                            power(1); // off
                            break;
                        }
                        else if (data.obstacle_location[1] == 0 && data.obstacle_location[2] == 0) { // 장애물 없음 -> 왼쪽 회전
                            printf("뒤로 이동 중... RVC가 왼쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[0] = 1;
                            power(1); // off
                            break;
                        }
                        else if (data.obstacle_location[1] == 1 && data.obstacle_location[2] == 0) { // 좌만 장애물 -> 오른쪽 회전
                            printf("뒤로 이동 중... RVC가 오른쪽으로 회전해야 합니다. 왼쪽 장애물 유무 %d 오른쪽 장애물 유무 %d\n", data.obstacle_location[1], data.obstacle_location[2]);
                            data.trigger[1] = 1;
                            power(1); // off
                            break;
                        }
                        else {
                            printf("뒤로 이동합니다...\n");
                            power(1); // off
                            data.trigger[2] = 1;
                            move_backward(data.trigger[2]);
                        }
                    }
                }
                last_third_time = time(NULL);
                if (data.trigger[0] == 1) {
                    int count = 1;
                    turn_left(data.trigger[0]); // 일단 회전
                    while (1) { // 3틱 좌회전
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization_test(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[0] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_left(data.trigger[0]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
                else if (data.trigger[1] == 1) {
                    int count = 1;
                    turn_right(data.trigger[1]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization_test(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_right(data.trigger[1]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
            }
            else { // 3틱 회전 left or right
                last_third_time = time(NULL);
                if (data.trigger[0] == 1) {
                    int count = 1;
                    turn_left(data.trigger[0]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization_test(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 왼쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_left(data.trigger[0]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
                            count++;
                        }
                    }
                }
                else if (data.trigger[1] == 1) {
                    int count = 1;
                    turn_right(data.trigger[1]); // 일단 회전
                    while (1) {
                        current_time = time(NULL);
                        if (difftime(current_time, last_third_time) >= tick) {
                            last_third_time = current_time;
                            initialization_test(&data);
                            determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                            determine_dust_existence(data.dust_existence, data.d);
                            data.trigger[1] = 1;
                            printf("RVC 오른쪽으로 회전 중입니다... %ds\n", count);
                            power(1);
                            turn_right(data.trigger[1]); // 쓸거면 고정하고 써야함
                            if (count > 4) break;
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
                    initialization_test(&data);
                    determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                    determine_dust_existence(data.dust_existence, data.d);

                    if (data.obstacle_location[0] == 1) { // 앞에 장애물이 있는 경우 -> Turn
                        power(1);
                        data.is_forward[0] = 0; // enable : false
                        data.is_forward[1] = 1; // disable : ture
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
                                initialization_test(&data);
                                determine_obstacle_location(data.obstacle_location, data.f, data.l, data.r);
                                determine_dust_existence(data.dust_existence, data.d);

                                if (data.obstacle_location[0] == 1) { // 앞에 장애물 있음
                                    power(1);
                                    data.is_forward[0] = 0; // enable : false
                                    data.is_forward[1] = 1; // disable : ture
                                    move_forward(data.is_forward);
                                    printf("RVC 청소 중 장애물 발견 Turn을 해야 합니다.\n");
                                    break;
                                }
                                else if (data.dust_existence[0] == 1) { // 장애물 없고, 먼지 있음
                                    power(2); // up
                                }
                                else {
                                    power(0); // on
                                }
                            }
                        }
                        break;
                    }
                }
            }

            if (data.obstacle_location[0] == 1) //  turn 이거 반복되는 2틱 턴의 과정임
            {

            }
        }
    }


    fclose(test_file); // 파일 닫기
}


int main(void) {
    srand(time(NULL));
    printf("RVC의 전원이 켜졌습니다.\n");
    controller();
    //controller_test();
    return 0;
}
