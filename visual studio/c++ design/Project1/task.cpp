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
        

        cout << "����ѡ��������������뷽ʽ" << endl;
		cout << "1.��������" << endl;
		cout << "2.��������" << endl;
		int options;
		cin >> options;
        if (options == 1) 
        {
            cout<<"\033[H\033[J";
            Show_Menu();
            cout << "����������������Ʒ���(����)" << endl;
            cout << "�鼮" << endl;
            cout << "��Ƶ����" << endl;
            cout << "ͼƬ" << endl;
            while(true)
            {
                string category;
                cin >> category;
                if (category == "�鼮") 
                {
                    Book temp;
                    cin >> temp;
                    book_data->Add_Book(temp);
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                } 
                else if (category == "��Ƶ����") 
                {
                    VCD temp;
                    cin >> temp;
                    vcd_data->Add_VCD(temp);
                    cout << "\033[H\033[J";
                    Show_Menu();
                    break;
                } 
                else if (category == "ͼƬ") 
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
                    Color_print("����ѡ�����,����������\n", 70);
                }
            }
            
            cout<<"\033[H\033[J";
            break;
        }
        else if (options==2)
        {
            cout << "��������Ҫ���������" << endl;
            int num;
            cin >> num;    
            cout << "����������������Ʒ���(����)" << endl;
            cout << "�鼮" << endl;
            cout << "��Ƶ����" << endl;
            cout << "ͼƬ" << endl;
           while(true)
           {
                string category;
                cin >> category;
                if (category == "�鼮")
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
                else if (category == "��Ƶ����")
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
                else if (category == "ͼƬ")
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
                    Color_print("����ѡ�����,����������\n", 70);
                }
            }
            cout << "\033[H\033[J";
            Show_Menu();
            break;
        }
    }
    cout << "��ӳɹ�" << endl;
    cout << "�Ƿ����" << endl;
    cout << "���������";
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
    // �鿴�洢��Ϣ
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "�鼮��Ϣ���£�" << endl;
    book_instance.Display_Book();
    cout << "��Ƶ��Ϣ���£�" << endl;
    vcd_instance.Display_VCD();
    cout << "ͼƬ��Ϣ���£�" << endl;
    picture_instance.Display_Picture();
    cout << "�Ƿ����" << endl;
    cout << "���������";
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
    // ��ѯ�鼮
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "��������ѡ���ѯ����" << endl;
    cout << "1.��Ų�ѯ"<<endl;
    cout << "2.�����ѯ" << endl;
    while (true)
    {
        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "�������ѯ�鼮�ı��" << endl;
            string id;
            cin >> id;
            book_instance.Find_Book_ID(id);
            break;
        }
        else if (choice == 2)
        {
            cout << "�������ѯ�鼮�ı���" << endl;
            string title;
            cin >> title;
            book_instance.Find_Book_Title(title);
            break;
        }
        else
        {
            Color_print("����ѡ�����,����������", 70);
        }
    }
    cout << "�Ƿ����" << endl;
    cout << "���������";
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
    // �༭�鼮
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "����������������Ʒ���(����)" << endl;
    cout << "�鼮" << endl;
    cout << "��Ƶ����" << endl;
    cout << "ͼƬ" << endl;
    string option;
    cin >> option;
    if (option == "�鼮")
    {
        cout << "����������༭���鼮�ı���" << endl;
        string title;
        cin >> title;
        book_instance.Edit_Book_Title(title);
    }
    else if (option == "��Ƶ")
    {
        cout << "����������༭����Ƶ�ı���" << endl;
        string title;
        cin >> title;
        vcd_instance.Edit_VCD_Title(title);
    }
    else if (option == "ͼƬ")
    {
        cout << "����������༭��ͼƬ�ı���" << endl;
        string title;
        cin >> title;
        picture_instance.Edit_Picture_Title(title);
    }
    else
    {
        Color_print("����ѡ�����,����������", 70);
    }
    cout << "�Ƿ����" << endl;
    cout << "���������";
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
    // ɾ��
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "����������������Ʒ���(����)" << endl;
    cout << "�鼮" << endl;
    cout << "��Ƶ����" << endl;
    cout << "ͼƬ" << endl;
    string option;
    cin >> option;
    if (option == "�鼮")
    {
        cout << "��������Ҫɾ�����鼮�ı���" << endl;
        string title;
        cin >> title;
        book_instance.Delete_Book(title);
    }
    else if (option == "��Ƶ")
    {
        cout << "��������Ҫɾ������Ƶ�ı���" << endl;
        string title;
        cin >> title;
        vcd_instance.Delete_VCD(title);
    }
    else if (option == "ͼƬ")
    {
        cout << "��������Ҫɾ����ͼƬ�ı���" << endl;
        string title;
        cin >> title;
        picture_instance.Delete_Picture(title);
    }
    else
    {
        Color_print("����ѡ�����,����������", 70);
    }
    cout << "�Ƿ����" << endl;
    cout << "���������";
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
    // ͳ����Ϣ
    cout << "\033[H\033[J";
    Show_Menu();
    cout << "�鼮����Ϊ��" << book_instance.Get_Size() << endl;
    cout << "��Ƶ����Ϊ��" << vcd_instance.Get_Size() << endl;
    cout << "ͼƬ����Ϊ��" << picture_instance.Get_Size() << endl;
    cout << "�Ƿ����" << endl;
    cout << "���������";
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