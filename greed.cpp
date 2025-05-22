#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int weight;
    int value;
    double ratio;
    int index;
} Item;

int compare(const void *a, const void *b) {
    Item *item1 = (Item *)a;
    Item *item2 = (Item *)b;
    return (item2->ratio > item1->ratio) - (item2->ratio < item1->ratio);
}

int max_value_greedy(int *weights, int *values, int n, int capacity, int *selected_items) {
    Item items[n];
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].ratio = (double)values[i] / weights[i];
        items[i].index = i;
    }

    // ����λ��ֵ�Ӹߵ�������
    qsort(items, n, sizeof(Item), compare);

    int max_value = 0;
    int current_capacity = capacity;

    for (int i = 0; i < n; i++) {
        if (current_capacity >= items[i].weight) {
            max_value += items[i].value;
            current_capacity -= items[i].weight;
            selected_items[items[i].index] = 1;
        }
    }

    return max_value;
}

void print_items(int *weights, int *values, int n) {
    printf("������ɵ���Ʒ�б�\n");
    printf("���\t����\t��ֵ\t��λ��ֵ\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%.2f\n", i + 1, weights[i], values[i], (double)values[i] / weights[i]);
    }
}

void print_results(int *selected_items, int *weights, int *values, int n, int total_value, int total_weight, int capacity) {
    printf("\n����ѡ�����Ʒ��\n");
    printf("���\t����\t��ֵ\t��λ��ֵ\n");
    for (int i = 0; i < n; i++) {
        if (selected_items[i]) {
            printf("%d\t%d\t%d\t%.2f\n", i + 1, weights[i], values[i], (double)values[i] / weights[i]);
        }
    }
    printf("������: %d/%d\n", total_weight, capacity);
    printf("�ܼ�ֵ: %d\n", total_value);
}

int main() {
    srand(time(NULL));

    int n = 10; // ������Ʒ�����Ա�鿴���
    int capacity = 50; // ������������
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
    int max_value = max_value_greedy(weights, values, n, capacity, selected_items);
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
