#include "task.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main()
{
    
    int class_choice = 0;
    Task_Init();

    while (true)
    {
        Show_Menu();
        Color_print("请输入操作\n", 66);
        cin >> class_choice;
        switch (class_choice)
        {
        case 1:
        {
            Task_Callback_1();
            
        }
        break;
        case 2:
        {
            Task_Callback_2();
            
        }
        break;
        case 3:
        {
            Task_Callback_3();
            
        }
        break;
        case 4:
        {
            Task_Callback_4();
            
        }
        break;
        case 5:
        {
            Task_Callback_5();
            
        }
        break;
        case 6:
        {
            Task_Callback_6();
            
        }
        break;
        case 0:
        {
            return 0;
        }
        break;
        default:
        {
            cout << "\033[H\033[J";
            Show_Menu();
            Color_print("输入选项错误,请重新输入", 70);
        }
        break;
        }
        Task_End();
    }
    system("pause");
    return 0;
}
