#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int max_value_brute_force(int *weights, int *values, int n, int capacity, int *selected_items) {
    int max_value = 0;
    int best_combination[n];

    // ������п��ܵ����
    for (int i = 0; i < (1 << n); i++) {
        int current_weight = 0;
        int current_value = 0;
        int combination[n] = {0};

        for (int j = 0; j < n; j++) {
            if (i & (1 << j)) { // �����j����Ʒ��ѡ��
                current_weight += weights[j];
                current_value += values[j];
                combination[j] = 1;
            }
        }

        if (current_weight <= capacity && current_value > max_value) {
            max_value = current_value;
            for (int j = 0; j < n; j++) {
                selected_items[j] = combination[j];
            }
        }
    }

    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("������ɵ���Ʒ�б�\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int total_weight, int capacity) {
    printf("\n����ѡ�����Ʒ��\n");
    printf("���\t����\t��ֵ\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\n", i + 1, weights[i], values[i]);
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

    clock_t start, end;
    double cpu_time_used;

    // ��ӡ������ɵ���Ʒ
    print_items(weights, values, n);

    start = clock(); // ��ʼ��ʱ
    int max_value = max_value_brute_force(weights, values, n, capacity, selected_items);
    end = clock(); // ������ʱ

    // ����������
    int total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            total_weight += weights[i];
        }
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000; // ת��Ϊ����
    printf("\n����ִ��ʱ��: %.2f ms\n", cpu_time_used);

    // ��ӡ���
    print_results(selected_items, weights, values, n, max_value, total_weight, capacity);

    return 0;
}
