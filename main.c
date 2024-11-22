#define _CTR_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void determine_obstacle_location(int* obstacle_location, int f, int l, int r) {
    // interface ȣ���ؼ� �迭�� ����
}

void determine_dust_existence(int* dust_existence, int d) {

}

int front_sensor_interface() {
    // ���� �� ����
}

int left_sensor_interface() {
    // ���� �� ����
}

int right_sensor_interface() {
    // ���� �� ����
}

int dust_sensor_interface() {
    // ���� �� ����
}


int move_forward(int* is_forward) {
    int motor_command = -10e9;
    // �迭 0�� �ε��� �� ����, 1�� �ε��� �� ���� �Ǵ� command ��ȯ

    return motor_command;
}

int turn_left() {
    int motor_command = -10e9;
    // trigger �߻� �ÿ��� ȣ��Ǵ� �Լ�

    return motor_command;
}

int turn_right() {
    int motor_command = -10e9;
    // trigger �߻� �ÿ��� ȣ��Ǵ� �Լ�

    return motor_command;
}

int move_backward() {
    int motor_command = -10e9;
    // trigger �߻� �ÿ��� ȣ��Ǵ� �Լ�

    return motor_command;
}

void motor_interface(int motor_command) {
    // motor command�� ���� ���
}

int power(int is_power) {
    int cleaner_command = -10e9;
    // is_power(on/off/up)�� ���� command ��ȯ

    return cleaner_command;
}

void cleaner_interface(int cleaner_command) {
    // command�� ���� ���
}

int divider(int* is_forward, int cleaner_command) {
    // 2�� ���� �����ؾ��� �� ȣ��

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

        if (motor_command == -1) { // �տ� ��ֹ��� �ִ� ���
            motor_interface(motor_command); // disable
            cleaner_command = 1; // off
            cleaner_interface(cleaner_command);

            if (obstacle_location[1] == 0) { // obstacle_location[2] == 1 �� ��ֹ� or obstacle_location[2] == 0) -> ���� ȸ��
                motor_command = turn_left();
                motor_interface(motor_command); // left
            }
            else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // �� ��ֹ�, �� ��ֹ� X -> ���� �� ȸ��
                motor_command = turn_right();
                motor_interface(motor_command); // right
            }
            else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // ��, �� ��ֹ� -> �ڷ� �̵�
                motor_command = move_backward();
                motor_interface(motor_command); // move_backward
                motor_command = -1; // disable
                motor_interface(motor_command); // ����

                f = 1; // �տ� ��ֹ� �ִٰ� ����. ��� �� ���� �Ⱦ�
                l = left_sensor_interface();
                r = right_sensor_interface();
                determine_obstacle_location(*obstacle_location, f, l, r);

                if (obstacle_location[1] == 0 && obstacle_location[2] == 0) { // ��, �� ��ֹ� ���� -> ���� ȸ�� or �츸 ��ֹ� -> ���� ȸ��
                    motor_command = turn_left();
                    motor_interface(motor_command); // left
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 1) { // ��ֹ� ���� -> ���� ȸ��
                    motor_command = turn_left();
                    motor_interface(motor_command); // left
                }
                else if (obstacle_location[1] == 1 && obstacle_location[2] == 0) { // �¸� ��ֹ� -> ������ ȸ��
                    motor_command = turn_right();
                    motor_interface(motor_command); // right
                }
            }
        }
        else { // �տ� ��ֹ��� ���� ��� + motor_command ����
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
    printf("RVC�� ������ �������ϴ�.\n");
    controller();
    return 0;
}