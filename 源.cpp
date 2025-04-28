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
int X0 = 7;//边框
int Y0 = 7;

struct Button {
    IMAGE img;
    IMAGE imgpress;
    int width, height;
    int x, y;
    int flag;
    bool ifpressed;
};//按钮结构体

Button btns[15];
int s[4][4] = {};


// 修改图片加载大小
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
    }//创建一个数组，用于对初始排列进行打断

   
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(order, order + 15, std::default_random_engine(seed)); // 使用随机数引擎打乱顺序

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

void drawButtons(Button* btn)//绘制按钮
{
    for (int i = 0; i < 15; i++)
    {
        putimage(btns[i].x, btns[i].y, &btns[i].img);
    }
}

void init()//初始化加载图片
{
    initgraph(1560 * 0.4, 1560 * 0.4);
    loadimage(&image, "华容道背景.png", 1560 * 0.4, 1560 * 0.4, true);
    buttonsInit();
}

void update()//操作后更新
{
    putimage(0, 0, &image);
    for (int i = 0; i < 15; i++)
    {
        putimage(btns[i].x, btns[i].y, &btns[i].img);
    }
}

bool checkifselected(Button* btn, MOUSEMSG* msg)//设置鼠标触控边界
{
    if (
        msg->x >= btn->x + btn->width * 0.1 &&//左侧触摸边界
        msg->x <= btn->x + btn->width * 0.9 &&//右侧触摸边界
        msg->y >= btn->y + btn->height * 0.1 &&//上方触摸边界
        msg->y <= btn->y + btn->height * 0.9//下方触摸边界
        )
    {
        return true;
    }
    else
    {
        return false;
    }

}

Button* checkButtonClick(MOUSEMSG msg)//检测鼠标点击
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

bool canmove(int sz, int* x, int* y)//判断是否可以移动
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
    }//上
    if (p < 3 && s[p + 1][q] == 0)
    {
        *x = 0;
        *y = 1;
        return true;
    }//下
    if (q > 0 && s[p][q - 1] == 0)
    {
        *x = -1;
        *y = 0;
        return true;
    }//左
    if (q < 3 && s[p][q + 1] == 0)
    {
        *x = 1;
        *y = 0;
        return true;
    }//右

    return false;
}

void moveButton(Button* button)
{
    int x, y;
    if (canmove(button->flag, &x, &y))
    {
        // 找到当前按钮在 s 数组中的位置
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
        // 更新 s 数组
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
