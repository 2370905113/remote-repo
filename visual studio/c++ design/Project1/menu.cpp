#include "menu.h"

void Color_print(const char* s, int color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | color);
    //	 printf(s);
    cout<< s;
    SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | 7);
}
void Show_Menu()
{
    
    Color_print("                   1、添加物品               \n", 6);
    Color_print("                   2、显示物品库               \n", 6);
    Color_print("  /\\_/\\  ", 14); Color_print("          3、查询物品                    \n", 6);
    Color_print(" ( o.o )---|\\", 14); Color_print("      4、编辑物品                       \n", 6);
    Color_print("  > * < ---|/ ", 14); Color_print("     5、删除物品                        \n", 6);
    Color_print("                   6、统计信息                           \n", 6);
    Color_print("                   0、退出当前功能                     \n", 12);
}

