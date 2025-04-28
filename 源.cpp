#include <iostream>
#include <graphics.h>
#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
#pragma comment(lib, "winmm.lib")

IMAGE image;
int X0 = 7;//�߿�
int Y0 = 7;

struct Button {
    IMAGE img;
    IMAGE imgpress;
    int width, height;
    int x, y;
    int flag;
    bool ifpressed;
};//��ť�ṹ��

Button btns[15];
int s[4][4] = {};


// �޸�ͼƬ���ش�С
void initButton(Button* btn, char* Img, int width, int height, int flag)
{
    loadimage(&btn->img, Img, width, height, flag);
    btn->width = width;
    btn->height = height;
    btn->ifpressed = false;
    btn->flag = flag;
}

void buttonsInit()
{
    int order[15];
    for (int i = 0; i < 15; i++) {
        order[i] = i + 1;
    }//����һ�����飬���ڶԳ�ʼ���н��д��

   
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(order, order + 15, std::default_random_engine(seed)); // ʹ��������������˳��

    int index = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == 3 && j == 3) {
                s[i][j] = 0;
            }
            else {
                char name1[64];
                int k = order[index];
                s[i][j] = k;
                sprintf_s(name1, "%d.png", k);
                initButton(&btns[index], name1, 150, 150, k);
                btns[index].x = X0 + j * (150 + 5);
                btns[index].y = Y0 + i * (150 + 5);
                index++;
            }
        }
    }
}

void drawButtons(Button* btn)//���ư�ť
{
    for (int i = 0; i < 15; i++)
    {
        putimage(btns[i].x, btns[i].y, &btns[i].img);
    }
}

void init()//��ʼ������ͼƬ
{
    initgraph(1560 * 0.4, 1560 * 0.4);
    loadimage(&image, "���ݵ�����.png", 1560 * 0.4, 1560 * 0.4, true);
    buttonsInit();
}

void update()//���������
{
    putimage(0, 0, &image);
    for (int i = 0; i < 15; i++)
    {
        putimage(btns[i].x, btns[i].y, &btns[i].img);
    }
}

bool checkifselected(Button* btn, MOUSEMSG* msg)//������괥�ر߽�
{
    if (
        msg->x >= btn->x + btn->width * 0.1 &&//��ഥ���߽�
        msg->x <= btn->x + btn->width * 0.9 &&//�Ҳഥ���߽�
        msg->y >= btn->y + btn->height * 0.1 &&//�Ϸ������߽�
        msg->y <= btn->y + btn->height * 0.9//�·������߽�
        )
    {
        return true;
    }
    else
    {
        return false;
    }

}

Button* checkButtonClick(MOUSEMSG msg)//��������
{
    for (int i = 0; i < 15; i++)
    {
        if (checkifselected(&btns[i], &msg))
        {
            return &btns[i];
        }
    }
    return NULL;
}

bool canmove(int sz, int* x, int* y)//�ж��Ƿ�����ƶ�
{
    int p = 0, q = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (s[i][j] == sz)
            {
                p = i;
                q = j;
                break;
            }
        }
    }

    if (p > 0 && s[p - 1][q] == 0)
    {
        *x = 0;
        *y = -1;
        return true;
    }//��
    if (p < 3 && s[p + 1][q] == 0)
    {
        *x = 0;
        *y = 1;
        return true;
    }//��
    if (q > 0 && s[p][q - 1] == 0)
    {
        *x = -1;
        *y = 0;
        return true;
    }//��
    if (q < 3 && s[p][q + 1] == 0)
    {
        *x = 1;
        *y = 0;
        return true;
    }//��

    return false;
}

void moveButton(Button* button)
{
    int x, y;
    if (canmove(button->flag, &x, &y))
    {
        // �ҵ���ǰ��ť�� s �����е�λ��
        int p, q;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (s[i][j] == button->flag)
                {
                    p = i;
                    q = j;
                    break;
                }
            }
        }
        // ���� s ����
        s[p][q] = 0;
        s[p + y][q + x] = button->flag;

        button->x += x * (150 + 5);
        button->y += y * (150 + 5);
        mciSendString("play click.mp3", 0, 0, 0);
        update();
    }
}

int main(void)
{
    init();
    update();
    Button* btn = NULL;
    while (1)
    {
        MOUSEMSG m = GetMouseMsg();

        switch (m.uMsg)
        {
        case WM_LBUTTONDOWN:
            btn = checkButtonClick(m);
            break;
        case WM_LBUTTONUP:
            if (btn)
            {
                moveButton(btn);
            }
            break;
        }
    }
    drawButtons(btn);
    return 0;
}
