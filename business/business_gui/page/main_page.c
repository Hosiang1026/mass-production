#include <math.h>
#include <stdio.h>
#include <ui.h>
#include <config.h>
#include <page_manager.h>

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];

static void GenerateButtons(void)
{
    int n;
    int col;
    int i = 0;
    int row, rows;
    int n_per_line;
    int width, height;
    int xres, yres;
    int start_x, start_y;
    int pre_start_x, pre_start_y;
    PButton pButton;
    PDispBuff pDispBuff;

    /* 算出单个按钮的width/height */
    n = GetItemCfgCount();

    pDispBuff = GetDisplayBuffer();
    xres = pDispBuff->iXres;
    yres = pDispBuff->iYres;
    width = sqrt(1.0/0.618 * xres * yres / n);
    width = xres / n_per_line;
    height = 0.618 * width;

    /* 居中显示: 计算每个按钮的region */
    start_x = (xres - width * n_per_line) / 2;
    rows = n / n_per_line;
    if (rows * n_per_line < n)
    {
        rows++;
    }
    start_y = (yres - height * rows) / 2;

    /* 计算每个按钮的region */
    for (row = 0; (row < rows) && (i < n); row++)
    {
        pre_start_x = start_x - width;
        pre_start_y = start_y + row * height;
        for (col = 0; (col < n_per_line) && (i < n); col++)
        {
            pButton = g_tButtons[i];
            pButton->tRegion.iLeftUpX = pre_start_x + width;
            pButton->tRegion.iLeftUpY = pre_start_y;
            pButton->tRegion.iWidth = width - X_GAP;
            pButton->tRegion.iHeigh = height - Y_GAP;

            pre_start_x = pButton->tRegion.iLeftUpX;

            /* 初始化按钮InitButton */
            InitButton(pButton, GetItemCfgByIndex(i)->name, NULL, NULL, NULL);
            i++;
        }
    }

    /* 绘制显示按钮OnDraw */
    for (i = 0; i < n; i++)
    {
        g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);
    }
    
}

static void MainPageRun(void *pParams)
{
    int error;
    /* 读取配置文件 */
    error = ParseConfigFile();
    if (error)
    {
        return error;
    }
    
    /* 根据配置文件生成按钮、界面 */
    GenerateButtons();

    while (1)
    {
        /* 读取输入事件 */

        /* 根据输入事件找到按钮 */

        /* 调用按钮的OnPressed函数 */
    }
    
}

static PageAction g_tMainPage = {
    .name = "main",
	.Run = MainPageRun,
};

void MainPageRegister(void)
{
    PageRegister(&g_tMainPage);
}