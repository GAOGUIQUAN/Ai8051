#include "sys.h"
#include "Ai.h"
#include "canvas.h"

extern uint8_t xdata input_image[IMAGE_WIDTH * IMAGE_HEIGHT];
uint8_t xdata canvas[CANVAS_HEIGHT * CANVAS_WIDTH] = {0};

void clean_canvas()
{
    memset(canvas, 0, CANVAS_HEIGHT * CANVAS_WIDTH * sizeof(canvas[0]));
}

// ����һ���ַ���canvas����ʼ�У��ָ�����ַ�,���з���input_image����֤�ı߿հײ��ֵȿ�
bool canvas_process_character(uint32_t start_col, uint32_t end_col)
{
    uint32_t col, row;
    uint32_t top_row, bottom_row;
    uint32_t hor_offset, ver_offset;
    uint32_t effective_height;

    // ������ֻ��һ�е���������ֵ����Ч�ַ�
    if (end_col - start_col <= 3)
    {
        uint32_t count = 0;
        for (col = start_col; col <= end_col; col++)
        {
            for (row = 0; row < CANVAS_HEIGHT; row++)
            {
                if (canvas[row * CANVAS_WIDTH + col] > 0)
                {
                    count++;
                }
            }
        }

        if (count < 10)
        {
            return false; // ��Ϊ�Ǹ��ŵ㣬����
        }
    }

    clean_input_image();

    // ����ˮƽƫ����
    hor_offset = (CHAR_IMG_SIZE - (end_col - start_col + 1)) / 2;

    // ������Ч���ַ��߶ȷ�Χ
    top_row = CANVAS_HEIGHT;
    bottom_row = 0;
    for (row = 0; row < CANVAS_HEIGHT; row++)
    {
        for (col = start_col; col <= end_col; col++)
        {
            if (canvas[row * CANVAS_WIDTH + col] > 0)
            {
                if (row < top_row)
                    top_row = row;
                if (row > bottom_row)
                    bottom_row = row;
            }
        }
    }

    // ������Ч�ַ�����ĸ߶�
    effective_height = bottom_row - top_row + 1;

    // ���㴹ֱƫ������������Ч�߶Ⱦ��У�
    ver_offset = (CHAR_IMG_SIZE - effective_height) / 2;

    // �ָ��ַ�������չ�� 28x28 ��С�����д���
    for (col = start_col; col <= end_col; col++)
    {
        for (row = 0; row < CANVAS_HEIGHT; row++)
        {
            if (canvas[row * CANVAS_WIDTH + col] > 0)
            {
                // ���ַ�������չ�� 28x28 ��С������
                uint32_t new_col = col - start_col + hor_offset;
                uint32_t new_row = row - top_row + ver_offset;

                // ����ַ��߶Ⱥ�λ�ò��ʺϾ��У����е���
                if (new_row < 0)
                {
                    new_row = 0;
                }
                else if (new_row >= CHAR_IMG_SIZE)
                {
                    new_row = CHAR_IMG_SIZE - 1;
                }

                input_image[new_row * CHAR_IMG_SIZE + new_col] = canvas[row * CANVAS_WIDTH + col];
            }
        }
    }
#if COLLECT_MODE
    {
        int xxx;
        for (xxx = 0; xxx < IMAGE_WIDTH * IMAGE_HEIGHT; xxx++)
        {
            printf("%3d,", input_image[xxx]);
            if (xxx % IMAGE_WIDTH == IMAGE_WIDTH - 1)
                printf("\r\n");
        }
        printf("@\r\n");
    }
#endif
    return true;
}