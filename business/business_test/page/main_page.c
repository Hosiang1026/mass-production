#include <config.h>
#include <stdio.h>
#include <ui.h>
#include <page_manager.h>
#include <math.h>
#include <string.h>

#define X_GAP 5
#define Y_GAP 5

static int g_tButtonCnt;
static Button g_tButtons[ITEMCFG_MAX_NUM];


/**
 * 根据输入事件处理按钮显示效果 
 */
static int MainPageOnPressed(struct Button *ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
    char *strButton;
    char name[100];
    char status[100];
    unsigned int dwColor = BUTTON_DEFAULT_COLOR;

    strButton = ptButton->name;

    /* 1. 对于触摸屏事件 */
    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        /* 1.1 分辨率能否被点击 */
        if (GetItemCfgByName(ptButton->name)->bCanBeTouched == 0)
        {
            return -1;
        }
        
        /* 1.2 修改颜色 */
        ptButton->status = !ptButton->status;
        if (ptButton->status)
        {
            dwColor = BUTTON_PRESSED_COLOR;
        }
    }
    /* 2. 对于网络事件 */
    else if (ptInputEvent->iType == INPUT_TYPE_NET)
    {
        /* 根据传入的字符串修改颜色: wifi ok, wifi err, burn 70 */
        sscanf(ptInputEvent->str, "%s %s", name, status);
        if (strcmp(status, "ok") == 0)
        {
           dwColor = BUTTON_PRESSED_COLOR;
        }
        else if (strcmp(status, "err") == 0)
        {
            dwColor = BUTTON_DEFAULT_COLOR;
        }
        else if (status[0] >= '0' && status[0] <= '9')
        {
            strButton = status;
            dwColor = BUTTON_PERCENT_COLOR;
        }
        else
        {
            return -1;
        }  
    }
    else
    {
        return -1;
    }

    /* 绘制区域底色 */
    DrawRegion(&ptButton->tRegion, dwColor);

    /* 居中写文字 */
    DrawTextRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR);

    /* flush to lcd/web */
    FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);

    return 0;
}

/**
 * 生成按钮界面
 */
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
    g_tButtonCnt = n = GetItemCfgCount();

    pDispBuff = GetDisplayBuffer();
    xres = pDispBuff->iXres;
    yres = pDispBuff->iYres;
    width = sqrt(1.0/0.618 * xres * yres / n);
    n_per_line = xres / width + 1;
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
            pButton = &g_tButtons[i];
            pButton->tRegion.iLeftUpX = pre_start_x + width;
            pButton->tRegion.iLeftUpY = pre_start_y;
            pButton->tRegion.iWidth = width - X_GAP;
            pButton->tRegion.iHeigh = height - Y_GAP;

            pre_start_x = pButton->tRegion.iLeftUpX;

            /* 初始化按钮InitButton */
            InitButton(pButton, GetItemCfgByIndex(i)->name, NULL, NULL, MainPageOnPressed);
            i++;
        }
    }

    /* 绘制显示按钮OnDraw */
    for (i = 0; i < n; i++)
    {
        g_tButtons[i].OnDraw(&g_tButtons[i], pDispBuff);
    }
    
}

/**
 * 判断触点所在的区域
 */
static int isTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
    if (iX < ptRegion->iLeftUpX || iX >= ptRegion->iLeftUpX + ptRegion->iWidth)
    {
        return 0;
    }

    if (iY < ptRegion->iLeftUpY || iY >= ptRegion->iLeftUpY + ptRegion->iHeigh)
    {
        return 0;
    }
    
    return 1; 
}

/**
 * 根据名称获取按钮 
 */
static PButton GetButtonByName(char *name)
{
    int i;
    for (i = 0; i < g_tButtonCnt; i++)
    {
        if(strcmp(name, g_tButtons[i].name) == 0)
        {
            return &g_tButtons[i];
        }
    }
    
    return NULL;
}
/**
 * 根据输入事件获取按钮
 */ 
static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
    int i;
    char name[100];

    if (ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        for (i = 0; i < g_tButtonCnt; i++)
        {
            if(isTouchPointInRegion(ptInputEvent->iX, ptInputEvent->iY, &g_tButtons[i].tRegion))
            {
                return &g_tButtons[i];
            }
        }
    }
    else if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
    {
        sscanf(ptInputEvent->str, "%s", name);
        return GetButtonByName(name);
    }
    else
    {
        return NULL;
    }
    return NULL;
}

static void MainPageRun(void *pParams)
{
    int error;
    PButton ptButton;
    InputEvent tInputEvent;
    PDispBuff ptDispBuff = GetDisplayBuffer();

    /* 读取配置文件 */
    error = ParseConfigFile();
    if (error)
    {
        return;
    }
    
    /* 根据配置文件生成按钮、界面 */
    GenerateButtons();

    while (1)
    {
        /* 读取输入事件 */
        error = GetInputEvent(&tInputEvent);
        if (error)
        {
            continue;
        }
        
        /* 根据输入事件找到按钮 */
        ptButton = GetButtonByInputEvent(&tInputEvent);
        if (!ptButton)
        {
            continue;
        }

        /* 调用按钮的OnPressed函数 */
        ptButton->OnPressed(ptButton, ptDispBuff, &tInputEvent);
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