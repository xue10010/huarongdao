#include <iostream>
#include <graphics.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
IMAGE image;
void init()
{
	initgraph(1560 * 0.4, 1560 * 0.4);
	loadimage(&image, "华容道背景.png", 1560 * 0.4, 1560 * 0.4, true);

}
void update()//操作后更新
{

	putimage(0, 0, &image);
}
int main(void)
{
	init();
	update();
	while (1)
	{
		MOUSEMSG m = GetMouseMsg();

		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:
			break;
		case WM_LBUTTONUP:
			break;




		}

	}
}