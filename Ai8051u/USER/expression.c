#include "expression.h"
#include <stdio.h>
#include <stdlib.h>  // For atoi function

#define MAX_SIZE 16

// ��ջ������������ȼ�
typedef struct {
    float _data[MAX_SIZE];  // ʹ�� float ������֧�ָ�����
    int32_t top;
} Stack;

static void push(Stack *stack, float value) {
    if (stack->top < MAX_SIZE - 1) {
        stack->_data[++(stack->top)] = value;
    }
}

static float pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->_data[(stack->top)--];
    }
    return 0.0f; // ��ջ���� 0
}

static float peek(Stack *stack) {
    if (stack->top >= 0) {
        return stack->_data[stack->top];
    }
    return 0.0f;
}

static int32_t getPriority(uint8_t op) {
    if (op == 10 || op == 11) { // �ӷ��ͼ���
        return 1;
    }
    if (op == 12 || op == 13) { // �˷��ͳ���
        return 2;
    }
    return 0;
}

float expression_calc(uint8_t item[], uint8_t n) {
    Stack numStack, opStack;
    float current, left, right, result;
    uint8_t op;
    uint8_t i;
    numStack.top = -1;  // ��ʼ��ջΪ��
    opStack.top = -1;

    result = 0.0f; // ��ʼ�����Ϊ 0.0

    i = 0;
    while (i < n) {
        current = item[i];

        // �����ǰ������ (0-9)�������Ƕ�λ����һ����
        if (current >= 0 && current <= 9) {
            // ����������������ɶ�λ��
            int32_t number = current;
            i++;
            while (i < n && item[i] >= 0 && item[i] <= 9) {
                number = number * 10 + item[i];
                i++;
            }
            push(&numStack, (float)number);  // ����Ϻ��������ջ
        }
        // �����ǰ������� (10-13)
        else if (current >= 10 && current <= 13) {
            while (opStack.top >= 0 && getPriority(peek(&opStack)) >= getPriority(current)) {
                right = pop(&numStack);
                left = pop(&numStack);
                op = pop(&opStack);

                switch (op) {
                    case 10: left += right; break;  // �ӷ�
                    case 11: left -= right; break;  // ����
                    case 12: left *= right; break;  // �˷�
                    case 13: 
                        if (right == 0.0f) {
                            printf("��������Ϊ0!\n");
                            return 0.0f;  // ��ֹ������
                        }
                        left /= right; break;  // ����
                }
                push(&numStack, left);
            }
            push(&opStack, current);  // ��ǰ�������ջ
            i++;
        }
    }

    // ����ʣ��������
    while (opStack.top >= 0) {
        right = pop(&numStack);
        left = pop(&numStack);
        op = pop(&opStack);

        switch (op) {
            case 10: left += right; break;  // �ӷ�
            case 11: left -= right; break;  // ����
            case 12: left *= right; break;  // �˷�
            case 13: 
                if (right == 0.0f) {
                    printf("��������Ϊ0!\n");
                    return 0.0f;  // ��ֹ������
                }
                left /= right; break;  // ����
        }
        push(&numStack, left);
    }

    return pop(&numStack);  // ���ս��
}


void expression_to_string(uint8_t *expression, uint8_t expr_length, uint8_t *buffer) {
    uint8_t i;
    uint8_t buffer_index = 0;

    // �������ʽ����
    for (i = 0; i < expr_length; i++) {
        char ch;

        // ���ݱ��ʽ�����е�ֵѡ����Ӧ���ַ�
        if (expression[i] >= 0 && expression[i] <= 9) {
            ch = '0' + expression[i];
        } else if (expression[i] == 10) {
            ch = '+';
        } else if (expression[i] == 11) {
            ch = '-';
        } else if (expression[i] == 12) {
            ch = '*';
        } else if (expression[i] == 13) {
            ch = '/';
        } else {
            // ���������Ч�����֣�����
            continue;
        }
        buffer[buffer_index++] = ch;
    }

    // ȷ���������Կ��ַ���β
    buffer[buffer_index] = '\0';
}
