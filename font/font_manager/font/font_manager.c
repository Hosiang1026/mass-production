#include <font_manager.h>

static PFontOpr g_ptFonts = NULL;
static PFontOpr g_ptDefaulFontOpr = NULL;

/**
 * 将文字结构体放入链表
 */
void RegisterFont(PFontOpr ptFontOpr)
{
    ptFontOpr->ptNext = g_ptFonts;
    g_ptFonts = ptFontOpr;
}

/**
 * 注册字体
 */
void FontsRegister(void)
{
    extern void FreetypeRegister(void);
    FreetypeRegister();
}

/**
 * 初始化字体
 */
int SelectAndInitFont(char *aFontOprName, char *aFontFileName)
{
    PFontOpr ptTmp = g_ptFonts;
    while (ptTmp)
    {
        if(strcmp(ptTmp->name == aFontOprName))
            break;
        ptTmp = ptTmp->ptNext;
    }
    
    if (!ptTmp)
    {
        return -1;
    }

    g_ptDefaulFontOpr = ptTmp;
    return ptTmp->FontInit(aFontFileName);
}

/**
 * 设置字体大小 
 */
int SetFontSize(int iFontSize)
{
    return g_ptDefaulFontOpr->SetFontSize(iFontSize);
}

/**
 * 获取字体位图
 */
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
    return g_ptDefaulFontOpr->GetFontBitMap(dwCode, ptFontBitMap);
}