% ���ݶ���
num = [1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 20000, 40000, 80000, 160000, 320000];
time_greedy = [0, 0, 0, 0, 0, 0, 0, 0, 5.88, 6.00, 9.34, 16.00, 29.88, 59.98, 128.42];
time_dp = [98.12, 264.00, 315.34, 380.85, 546.34, 660.00, 721.99, 860.45, 986.24, 1888.45, 2412.34, 4382.11, 9884.00, 19630.45, 41262.00];
num_bruteforce = [15, 20, 25];
time_bruteforce = [4.64, 155.00, 7348.88];
num_backtracking = [20, 25, 30];
time_backtracking = [11.34, 320.34, 15943.00];

% �����ϲ�ͼ
figure('Position', [100, 100, 800, 600]);

% ʹ�ö��������������������
semilogy(num, time_greedy, 'o-', 'LineWidth', 1.5, 'MarkerSize', 6, 'DisplayName', '̰�ķ�');
hold on;
semilogy(num, time_dp, 's-', 'LineWidth', 1.5, 'MarkerSize', 6, 'DisplayName', '��̬�滮');
semilogy(num_bruteforce, time_bruteforce, 'd-', 'LineWidth', 1.5, 'MarkerSize', 6, 'DisplayName', '������');
semilogy(num_backtracking, time_backtracking, '^-', 'LineWidth', 1.5, 'MarkerSize', 6, 'DisplayName', '���ݷ�');

% ����ͼ������
title('��ͬ�㷨����ʱ��Ա�', 'FontSize', 14);
xlabel('�����ģ', 'FontSize', 12);
ylabel('ʱ�� (��)', 'FontSize', 12);
grid on;
legend('Location', 'northeast');
set(gca, 'XScale', 'log'); % ����X��Ϊ�����̶�

% ����ͼ���������᷶Χ
legend('boxoff', 'FontSize', 10);
xlim([10, 1e6]); % ����X�᷶Χ�Ը�����ʾ�������ݵ�