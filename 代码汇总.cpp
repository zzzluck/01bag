#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_ITEMS 100000           // �����Ʒ����
#define MAX_CAPACITY 1000000       // ��󱳰�����

// ���ݷ�ȫ�ֱ�����ע�⣺��������С��ģ���⣩
int max_value_back = 0;
int current_weight_back = 0;
int current_value_back = 0;
int best_selected_back[MAX_ITEMS] = {0};

// ���ݷ�����
void backtrack(int *weights, int *values, int n, int capacity, int index, int *selected) {
    if (index == n) {
        if (current_value_back > max_value_back) {
            max_value_back = current_value_back;
            memcpy(best_selected_back, selected, n * sizeof(int));
        }
        return;
    }
    // ��ѡ��ǰ��Ʒ
    backtrack(weights, values, n, capacity, index + 1, selected);
    
    // ѡ��ǰ��Ʒ
    if (current_weight_back + weights[index] <= capacity) {
        selected[index] = 1;
        current_weight_back += weights[index];
        current_value_back += values[index];
        backtrack(weights, values, n, capacity, index + 1, selected);
        selected[index] = 0;
        current_weight_back -= weights[index];
        current_value_back -= values[index];
    }
}

int max_value_backtracking(int *weights, int *values, int n, int capacity, int *selected) {
    memset(selected, 0, n * sizeof(int));
    max_value_back = current_weight_back = current_value_back = 0;
    backtrack(weights, values, n, capacity, 0, selected);
    memcpy(selected, best_selected_back, n * sizeof(int));
    return max_value_back;
}

// ��̬�滮������
int max_value_dp(int *weights, int *values, int n, int capacity, int *selected) {
    int **dp = (int **)malloc((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; i++)
        dp[i] = (int *)malloc((capacity + 1) * sizeof(int));
    
    // ��ʼ��DP��
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= capacity; j++)
            dp[i][j] = 0;
    
    // ���DP��
    for (int i = 1; i <= n; i++) {
        for (int j = 0; j <= capacity; j++) {
            if (j >= weights[i-1])
                dp[i][j] = MAX(dp[i-1][j], dp[i-1][j - weights[i-1]] + values[i-1]);
            else
                dp[i][j] = dp[i-1][j];
        }
    }
    
    // ����ѡ�����Ʒ
    int c = capacity;
    memset(selected, 0, n * sizeof(int));
    for (int i = n; i > 0 && c > 0; i--) {
        if (dp[i][c] != dp[i-1][c]) {
            selected[i-1] = 1;
            c -= weights[i-1];
        }
    }
    
    int max_val = dp[n][capacity];
    // �ͷ��ڴ�
    for (int i = 0; i <= n; i++) free(dp[i]);
    free(dp);
    return max_val;
}

// ����ö�ٷ���������������n<20�������
int max_value_brute(int *weights, int *values, int n, int capacity, int *selected) {
    int max_val = 0;
    int best[MAX_ITEMS] = {0};
    
    for (int mask = 0; mask < (1 << n); mask++) {
        int w = 0, v = 0, comb[MAX_ITEMS] = {0};
        for (int j = 0; j < n; j++) {
            if (mask & (1 << j)) {
                w += weights[j];
                v += values[j];
                comb[j] = 1;
            }
        }
        if (w <= capacity && v > max_val) {
            max_val = v;
            memcpy(best, comb, n * sizeof(int));
        }
    }
    memcpy(selected, best, n * sizeof(int));
    return max_val;
}

// ̰���㷨�ṹ��ͱȽϺ���
typedef struct {
    int weight;
    int value;
    double ratio;
    int index;
} Item;

int compare_items(const void *a, const void *b) {
    Item *i1 = (Item*)a, *i2 = (Item*)b;
    return (i2->ratio > i1->ratio) - (i2->ratio < i1->ratio);
}

int max_value_greedy(int *weights, int *values, int n, int capacity, int *selected) {
    Item items[MAX_ITEMS];
    for (int i = 0; i < n; i++) {
        items[i] = (Item){weights[i], values[i], (double)values[i]/weights[i], i};
    }
    qsort(items, n, sizeof(Item), compare_items);
    
    int total_w = 0, total_v = 0;
    memset(selected, 0, n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        if (total_w + items[i].weight <= capacity) {
            total_w += items[i].weight;
            total_v += items[i].value;
            selected[items[i].index] = 1;
        }
    }
    return total_v;
}

// ͨ�ú��������������Ʒ
void generate_items(int *weights, int *values, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        weights[i] = rand() % 50 + 1;     // ����1-50
        values[i] = rand() % 200 + 50;    // ��ֵ50-250
    }
}

// ͨ�ú�������ӡ��Ʒ�б�
void print_items(int *weights, int *values, int n) {
    printf("\n��Ʒ�б����-����-��ֵ����\n");
    for (int i = 0; i < n; i++)
        printf("%d\t%d\t%d\n", i+1, weights[i], values[i]);
}

// ͨ�ú�������ӡ���
void print_result(int *selected, int *weights, int *values, int n, int capacity, int total_v, const char *method) {
    int total_w = 0;
    printf("\n---------------- %s ��� ----------------\n", method);
    printf("ѡ�����Ʒ�����-����-��ֵ����\n");
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("%d\t%d\t%d\n", i+1, weights[i], values[i]);
            total_w += weights[i];
        }
    }
    printf("��������%d/%d | �ܼ�ֵ��%d\n", total_w, capacity, total_v);
}

int main() {
    int n, capacity, choice;
    int weights[MAX_ITEMS], values[MAX_ITEMS], selected[MAX_ITEMS] = {0};
    
    // �������
    printf("��������Ʒ������������ݷ�/��������20����̬�滮��1000��̰�����⣩��");
    scanf("%d", &n);
    printf("�����뱳��������");
    scanf("%d", &capacity);
    
    // ���������Ʒ
    generate_items(weights, values, n);
    print_items(weights, values, n);
    
    // �㷨ѡ��˵�
    printf("\n��ѡ���㷨�������Ӧ���֣���\n");
    printf("1. ���ݷ����ݹ�������\n");
    printf("2. ��̬�滮����0-1������׼�ⷨ��\n");
    printf("3. ����ö�ٷ�����������С��ģ��\n");
    printf("4. ̰���㷨�����ƽ⣬�����ţ�\n");
    scanf("%d", &choice);
    
    clock_t start = clock();
    int max_val = 0;
    
    switch(choice) {
        case 1:
            if (n > 20) {
                printf("���棺���ݷ�������n��20����ǰn=%d���ܵ��³�ʱ��\n", n);
            }
            max_val = max_value_backtracking(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "���ݷ�");
            break;
        case 2:
            max_val = max_value_dp(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "��̬�滮��");
            break;
        case 3:
            if (n > 20) {
                printf("���󣺱���ö�ٷ���������n��20����ǰn=%d������Χ��\n", n);
                return 1;
            }
            max_val = max_value_brute(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "����ö�ٷ�");
            break;
        case 4:
            max_val = max_value_greedy(weights, values, n, capacity, selected);
            print_result(selected, weights, values, n, capacity, max_val, "̰���㷨");
            break;
        default:
            printf("��Чѡ��\n");
            return 1;
    }
    
    // ����ִ��ʱ��
    double time_ms = ((double)(clock() - start) / CLOCKS_PER_SEC) * 1000;
    printf("\nִ��ʱ�䣺%.2f ����\n", time_ms);
    
    return 0;
}
