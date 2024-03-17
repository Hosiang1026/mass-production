#include <config.h>
#include <stdio.h>
#include <string.h>

static ItemCfg g_tItemCfgs[ITEMCFG_MAX_NUM];
static int g_iItemCfgsCount = 0;

/* 解析配置文件 */
int ParseConfigFile(void)
{
    FILE *fp;
    char buf[100];
    char *p = buf;

    /* 1. 打开配置文件 */
    fp = fopen(CFG_FILE, "r");
    if (!fp)
    {
        printf("can not cfg file %s\n", CFG_FILE);
        return -1;
    }
    
    while (fgets(buf, 100, fp))
    {
        /* 2.1 读出每一行数据 */
        buf[99] = '\0';

        /* 2.2 吃掉开头的空格或TAB */
        p = buf;
        while (*p == ' ' || *p == '\t')
        {
            p++;
        }
        
        /* 2.3 忽略注释 */
        if (*p == '#')
        {
            continue;
        }

        /* 2.4 处理数据 */
        g_tItemCfgs[g_iItemCfgsCount].command[0] = '\0';
        g_tItemCfgs[g_iItemCfgsCount].index = g_iItemCfgsCount;
        sscanf(p, "%s %d %s", g_tItemCfgs[g_iItemCfgsCount].name, 
               &g_tItemCfgs[g_iItemCfgsCount].bCanBeTouched,
               g_tItemCfgs[g_iItemCfgsCount].command);
        g_iItemCfgsCount++;
    }

    return 0;
}

int GetItemCfgCount(void)
{
    return g_iItemCfgsCount;
}

PItemCfg GetItemCfgByIndex(int index)
{
    if (index < g_iItemCfgsCount)
    {
        return &g_tItemCfgs[index];
    }
    else
    {
        return NULL;
    } 
}

PItemCfg GetItemCfgByName(char *name)
{
    int i;
    for (i = 0; i < g_iItemCfgsCount; i++)
    {
        if (strcmp(name, g_tItemCfgs[i].name) == 0)
        {
            return &g_tItemCfgs[i];
        }
    }
    
    return NULL;
}
