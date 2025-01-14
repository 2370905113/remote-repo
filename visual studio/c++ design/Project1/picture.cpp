#include "picture.h"

void Picture::Set(string id, string title, string author, string level, string nation, string length, string width)
{
    ID = id;
    Title = title;
    Author = author;
    Level = level;
    Nation = nation;
    Length = length;
    Width = width;
}

void Picture::Get()
{
    cout << "���: " << setw(2) << ID
         << setw(12) << "����:" << setw(10) << Title
         << setw(12) << "����:" << setw(8) << Author
         << setw(12) << "�ȼ�:" << setw(3) << Level
         << setw(12) << "����:" << setw(8) << Nation
         << setw(12) << "����:" << setw(6) << Length
         << setw(12) << "���:" << setw(6) << Width << endl;
}

istream& operator>>(istream& in, Picture& Picture) {
    cout << "��������:  ";
    in >> Picture.ID;
    cout << "���������: ";
    in >> Picture.Title;
    cout << "���������ߣ� ";
    in >> Picture.Author;
    cout << "������ȼ�: ";
    in >> Picture.Level;
    cout << "���������: ";
    in >> Picture.Nation;
    cout << "�����볤��: ";
    in >> Picture.Length;
    cout << "��������: ";
    in >> Picture.Width;
    return in;
}

ostream& operator<<(ostream& out, const Picture& Picture) {
    out << "���: " << setw(2) << Picture.ID
        << setw(12) << "����:" << setw(10) << Picture.Title
        << setw(12) << "����:" << setw(8) << Picture.Author
        << setw(12) << "�ȼ�:" << setw(3) << Picture.Level
        << setw(12) << "����:" << setw(8) << Picture.Nation
        << setw(12) << "����:" << setw(6) << Picture.Length
        << setw(12) << "���:" << setw(6) << Picture.Width << endl;
    return out;
}

int Picture_Data::isExist_Picture_Title(string title) {
    int ret = -1;
    Picture_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->picture.Get_Title() == title) {
            break;// �ҵ�����������ѭ��
        }
        current = current->next;
    }
    return ret;
}

int Picture_Data::isExist_Picture_ID(string id) {
    int ret = -1;
    Picture_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->picture.Get_ID() == id) {
            break;// �ҵ�����������ѭ��
        }
        current = current->next;
    }
    return ret;
}

void Picture_Data::Find_Picture_Title(string title) {
    int index = isExist_Picture_Title(title);
    if (index != -1) {
        Picture_Note* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        cout << current->picture << endl;
        cout << "��ѯ�ɹ�" << endl;
    } else {
        cout << "δ��ѯ������" << endl;
    }
}

void Picture_Data::Find_Picture_ID(string id) {
    int index = isExist_Picture_ID(id);
    if (index != -1) {
        Picture_Note* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        cout << current->picture << endl;
        cout << "��ѯ�ɹ�" << endl;
    }
    else {
        cout << "δ��ѯ������" << endl;
    }
}

void Picture_Data::Delete_Picture(string title)
{
    Picture_Note* current = head;
    Picture_Note* previous = NULL;
    while (current != NULL)
    {
        if (current->picture.Get_Title() == title)
        {
            if (previous == NULL)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            if (current == tail)
            {
                tail = previous;
            }
            delete current;
            size--;
            cout << "ɾ���ɹ�" << endl;
            return;
        }
        previous = current;
        current = current->next;
    }
    cout << "���鲻����" << endl;
}

void Picture_Data::Edit_Picture_Title(string title)
{
    Picture_Note* current = head;
    bool flag = false;
    while(current!=NULL)
    {
        if(current->picture.Get_Title()==title)
        {
            cout<<current->picture<<endl;
            cout<<"�������µ�ͼƬ��Ϣ"<<endl;
            Picture temp;
            cin>>temp;
            current->picture=temp;
            flag=true;
            break;
        }
        current=current->next;
    }
    if(flag)
    {
        cout<<"�޸ĳɹ�"<<endl;
    }
    else
    {
        cout<<"�޸�ʧ��"<<endl;    
    }
}
void Picture_Data::Edit_Picture_ID(string id)
{
    Picture_Note* current = head;
    bool flag = false;
    while (current != NULL)
    {
        if (current->picture.Get_ID() == id)
        {
            cout << current->picture << endl;
            cout << "�������µ�ͼƬ��Ϣ" << endl;
		    Picture temp;
            cin >> temp;
            current->picture = temp;
            flag = true;
            break;
        }
        current = current->next;
    }
        if(flag)
        {
            cout<<"�޸ĳɹ�"<<endl;
        }
        else
        {
            cout<<"�޸�ʧ��"<<endl;    
        }
}

void Picture_Data::Add_Picture(Picture& Picture)
{
    Picture_Note* newNode = new Picture_Note(Picture);
    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

void Picture_Data::Display_Picture()
{
    p = head;
    while (p != NULL)
    {
        cout << p->picture << endl;
        p = p->next;
    }
}

void Picture_Data::Storage_Picture()
{
    Picture_Note* current=head;
    fstream output("picture.txt", ios::out);
    
    output<<"ͼƬ����Ϊ��"<<size<<endl;
    if(current!=NULL)
    {
        output << left << setw(10) << "���" 
           << left << setw(20) << "����" 
           << left << setw(20) << "����" 
           << left << setw(15) << "�ȼ�" 
           << left << setw(20) << "������" 
           << left << setw(20) << "ISBN" 
           << left << setw(10) << "ҳ��" << endl;
    }
    else
    {
        output<<"!!!���Ϊ��!!!"<<endl;
    }   
    while (current != NULL) {
        output << left << setw(10) << current->picture.Get_ID()
               << left << setw(20) << current->picture.Get_Title()
               << left << setw(20) << current->picture.Get_Author()
               << left << setw(15) << current->picture.Get_Level()
               << left << setw(20) << current->picture.Get_Nation()
               << left << setw(20) << current->picture.Get_Length()
               << left << setw(10) << current->picture.Get_Width()<< endl;
        current = current->next;
    }
}

void Picture_Data::Load_Picture()
{
    fstream input("picture.txt", ios::in);
    if (!input)
    {
        cout << "�ļ���ʧ��" << endl;
        return;
    }
    string id, title, author, level,nation, length, width;
    string judge;
    while(input >> judge)
    {
        if(judge=="�鼮����Ϊ��")
        {
            input >> size;
        }
        else if(judge=="ҳ��")
        {
            input >> id >> title >> author >> level >> nation >> length >> width;
            Picture picture;
            picture.Set(id, title, author, level, nation, length, width);
            Add_Picture(picture);
        }
    }
    input.close();
}