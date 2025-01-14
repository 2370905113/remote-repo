#include "task.h"

Book_Data book_instance;
Book_Data* book_data = &book_instance;
VCD_Data vcd_instance;
VCD_Data* vcd_data = &vcd_instance;
Picture_Data picture_instance;
Picture_Data* picture_data = &picture_instance;

void Task_Init()
{
    book_instance.Load_Book();
    vcd_instance.Load_VCD();
    picture_instance.Load_Picture();
}

void Task_Callback_1()
{
    
    while (true)
    {
        

        cout << "请您选择的以下两种输入方式" << endl;
		cout << "1.单个输入" << endl;
		cout << "2.批量输入" << endl;
		int options;
		cin >> options;
        if (options == 1) 
        {
            cout<<"\033[H\033[J";
            Show_Menu();
            cout << "请输入以下三种物品类别(汉字)" << endl;
            cout << "书籍" << endl;
            cout << "视频光盘" << endl;
            cout << "图片" << endl;
            while(true)
            {
                string category;
                cin >> category;
                if (category == "书籍") 
                {
                    Book temp;
                    cin >> temp;
                    book_data->Add_Book(temp);
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                } 
                else if (category == "视频光盘") 
                {
                    VCD temp;
                    cin >> temp;
                    vcd_data->Add_VCD(temp);
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                } 
                else if (category == "图片") 
                {
                    Picture temp;
                    cin >> temp;
                    picture_data->Add_Picture(temp);
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                }
                else
                {
                    Color_print("输入选项错误,请重新输入\n", 70);
                }
            }
            
            cout<<"\033[H\033[J";
            break;
        }
        else if (options==2)
        {
            cout << "请输入您要输入的数量" << endl;
            int num;
            cin >> num;    
            cout << "请输入以下三种物品类别(汉字)" << endl;
            cout << "书籍" << endl;
            cout << "视频光盘" << endl;
            cout << "图片" << endl;
           while(true)
           {
                string category;
                cin >> category;
                if (category == "书籍")
                {
                    for (int i = 0; i < num;i++)
                    {
                        string id, title, author, level, publisher, ibsn, page;
                        cin >> id >> title >> author >> level >> publisher >> ibsn >> page;
                        Book temp(id, title, author, level, publisher, ibsn, page);
                        book_data->Add_Book(temp);
                    }
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                }
                else if (category == "视频光盘")
                { 
                    for (int i = 0; i < num; i++)
                    {
                    string id,title,author,level,nation,length,width;
                    cin >> id >> title >> author >> level >> nation >> length >> width;
                        {
                            VCD temp(id, title, author, level, nation, length, width);
                            vcd_data->Add_VCD(temp);
                        }
                    }
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                }
                else if (category == "图片")
                {
                    for (int i = 0; i < num; i++)
                    {
                        string id, title, author, level, pixel, length, width;
                        cin >> id >> title >> author >> level >> pixel >> length >> width;
                        Picture temp(id, title, author, level, pixel, length, width);
                        picture_data->Add_Picture(temp);
                    }
                    break;
                }
                else
                {
                    Color_print("输入选项错误,请重新输入\n", 70);
                }
            }
            cout << "\033[H\033[J";
            Show_Menu();
            break;
        }
    }
    cout << "添加成功" << endl;
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_1();
    }
}

void Task_Callback_2()
{
    // 查看存储信息
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "书籍信息如下：" << endl;
    book_instance.Display_Book();
    cout << "视频信息如下：" << endl;
    vcd_instance.Display_VCD();
    cout << "图片信息如下：" << endl;
    picture_instance.Display_Picture();
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_2();
    }
}

void Task_Callback_3()
{
    // 查询书籍
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "请输入您选择查询方法" << endl;
    cout << "1.编号查询"<<endl;
    cout << "2.标题查询" << endl;
    while (true)
    {
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "请输入查询书籍的编号" << endl;
            string id;
            cin >> id;
            book_instance.Find_Book_ID(id);
            break;
        }
        else if (choice == 2)
        {
            cout << "请输入查询书籍的标题" << endl;
            string title;
            cin >> title;
            book_instance.Find_Book_Title(title);
            break;
        }
        else
        {
            Color_print("输入选项错误,请重新输入", 70);
        }
    }
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_3();
    }
}

void Task_Callback_4()
{
    // 编辑书籍
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "请输入以下三种物品类别(汉字)" << endl;
    cout << "书籍" << endl;
    cout << "视频光盘" << endl;
    cout << "图片" << endl;
    string option;
    cin >> option;
    if (option == "书籍")
    {
        cout << "请输入您想编辑的书籍的标题" << endl;
        string title;
        cin >> title;
        book_instance.Edit_Book_Title(title);
    }
    else if (option == "视频")
    {
        cout << "请输入您想编辑的视频的标题" << endl;
        string title;
        cin >> title;
        vcd_instance.Edit_VCD_Title(title);
    }
    else if (option == "图片")
    {
        cout << "请输入您想编辑的图片的标题" << endl;
        string title;
        cin >> title;
        picture_instance.Edit_Picture_Title(title);
    }
    else
    {
        Color_print("输入选项错误,请重新输入", 70);
    }
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_4();
    }
}

void Task_Callback_5()
{
    // 删除
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "请输入以下三种物品类别(汉字)" << endl;
    cout << "书籍" << endl;
    cout << "视频光盘" << endl;
    cout << "图片" << endl;
    string option;
    cin >> option;
    if (option == "书籍")
    {
        cout << "请输入您要删除的书籍的标题" << endl;
        string title;
        cin >> title;
        book_instance.Delete_Book(title);
    }
    else if (option == "视频")
    {
        cout << "请输入您要删除的视频的标题" << endl;
        string title;
        cin >> title;
        vcd_instance.Delete_VCD(title);
    }
    else if (option == "图片")
    {
        cout << "请输入您要删除的图片的标题" << endl;
        string title;
        cin >> title;
        picture_instance.Delete_Picture(title);
    }
    else
    {
        Color_print("输入选项错误,请重新输入", 70);
    }
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_5();
    }
}

void Task_Callback_6()
{
    // 统计信息
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "书籍总数为：" << book_instance.Get_Size() << endl;
    cout << "视频总数为：" << vcd_instance.Get_Size() << endl;
    cout << "图片总数为：" << picture_instance.Get_Size() << endl;
    cout << "是否继续" << endl;
    cout << "请输入操作";
    Color_print("1/0", 66);
    cout << endl;
    int choice;
    cin >> choice;
    cout << "\033[H\033[J";
    if (!(choice == 0))
    {
        Show_Menu();
        Task_Callback_6();
    }
}

void Task_End()
{
    book_instance.Storage_Book();
    vcd_instance.Storage_VCD();
    picture_instance.Storage_Picture();
}