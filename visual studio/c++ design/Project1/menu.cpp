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
    
    Color_print("                   1�������Ʒ               \n", 6);
    Color_print("                   2����ʾ��Ʒ��               \n", 6);
    Color_print("  /\\_/\\  ", 14); Color_print("          3����ѯ��Ʒ                    \n", 6);
    Color_print(" ( o.o )---|\\", 14); Color_print("      4���༭��Ʒ                       \n", 6);
    Color_print("  > * < ---|/ ", 14); Color_print("     5��ɾ����Ʒ                        \n", 6);
    Color_print("                   6��ͳ����Ϣ                           \n", 6);
    Color_print("                   0���˳���ǰ����                     \n", 12);
}

