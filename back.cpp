#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value = 0;
int current_weight = 0;
int current_value = 0;
int best_selected_items[1000000]; 

void backtrack(int *weights, int *values, int n, int capacity, int index, int *selected_items) {
    if (index == n) {
        if (current_value > max_value) {
            max_value = current_value;
            for (int i = 0; i < n; i++) {
                best_selected_items[i] = selected_items[i];
            }
        }
        return;
    }
    // ��ѡ��ǰ��Ʒ
    backtrack(weights, values, n, capacity, index + 1, selected_items);

    // ѡ��ǰ��Ʒ
    if (current_weight + weights[index] <= capacity) {
        selected_items[index] = 1;
        current_weight += weights[index];
        current_value += values[index];
        backtrack(weights, values, n, capacity, index + 1, selected_items);
        selected_items[index] = 0;
        current_weight -= weights[index];
        current_value -= values[index];
    }
}

int max_value_backtracking(int *weights, int *values, int n, int capacity, int *selected_items) {
    max_value = 0;
    current_weight = 0;
    current_value = 0;
    backtrack(weights, values, n, capacity, 0, selected_items);
    for (int i = 0; i < n; i++) {
        selected_items[i] = best_selected_items[i];
    }
    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("������ɵ���Ʒ��Ϣ��\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int capacity) {
    int total_weight = 0;
    printf("\n����ѡ�����Ʒ��Ϣ��\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
            total_weight += weights[i];
        }
    }
    printf("������: %d/%d\n", total_weight, capacity);
    printf("�ܼ�ֵ: %d\n", total_value);
}

int main() {
    srand(time(NULL));

    int n = 10; // ʾ����Ʒ����
    int capacity = 50; // ʾ����������
    int weights[n];
    int values[n];
    int selected_items[n] = {0};

    // ���������Ʒ�������ͼ�ֵ
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 100 + 1; // ������1��100֮��
        values[i] = rand() % 900 + 100; // ��ֵ��100��1000֮��
    }

    // ��ʾ������ɵ���Ʒ
    print_items(weights, values, n);

    clock_t start, end;
    double cpu_time_used;

    start = clock(); // ��ʼ��ʱ
    int max_value = max_value_backtracking(weights, values, n, capacity, selected_items);
    end = clock(); // ������ʱ

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // ת��Ϊ����
    printf("\n������ִ��ʱ��: %.2f ms\n", cpu_time_used);

    // ��ʾ���ս��
    print_results(selected_items, weights, values, n, max_value, capacity);

    return 0;
}
