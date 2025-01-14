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
    cout << "编号: " << setw(2) << ID
         << setw(12) << "标题:" << setw(10) << Title
         << setw(12) << "作者:" << setw(8) << Author
         << setw(12) << "等级:" << setw(3) << Level
         << setw(12) << "国家:" << setw(8) << Nation
         << setw(12) << "长度:" << setw(6) << Length
         << setw(12) << "宽度:" << setw(6) << Width << endl;
}

istream& operator>>(istream& in, Picture& Picture) {
    cout << "请输入编号:  ";
    in >> Picture.ID;
    cout << "请输入标题: ";
    in >> Picture.Title;
    cout << "请输入作者： ";
    in >> Picture.Author;
    cout << "请输入等级: ";
    in >> Picture.Level;
    cout << "请输入国家: ";
    in >> Picture.Nation;
    cout << "请输入长度: ";
    in >> Picture.Length;
    cout << "请输入宽度: ";
    in >> Picture.Width;
    return in;
}

ostream& operator<<(ostream& out, const Picture& Picture) {
    out << "编号: " << setw(2) << Picture.ID
        << setw(12) << "标题:" << setw(10) << Picture.Title
        << setw(12) << "作者:" << setw(8) << Picture.Author
        << setw(12) << "等级:" << setw(3) << Picture.Level
        << setw(12) << "国家:" << setw(8) << Picture.Nation
        << setw(12) << "长度:" << setw(6) << Picture.Length
        << setw(12) << "宽度:" << setw(6) << Picture.Width << endl;
    return out;
}

int Picture_Data::isExist_Picture_Title(string title) {
    int ret = -1;
    Picture_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->picture.Get_Title() == title) {
            break;// 找到后立即跳出循环
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
            break;// 找到后立即跳出循环
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
        cout << "查询成功" << endl;
    } else {
        cout << "未查询到此书" << endl;
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
        cout << "查询成功" << endl;
    }
    else {
        cout << "未查询到此书" << endl;
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
            cout << "删除成功" << endl;
            return;
        }
        previous = current;
        current = current->next;
    }
    cout << "此书不存在" << endl;
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
            cout<<"请输入新的图片信息"<<endl;
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
        cout<<"修改成功"<<endl;
    }
    else
    {
        cout<<"修改失败"<<endl;    
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
            cout << "请输入新的图片信息" << endl;
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
            cout<<"修改成功"<<endl;
        }
        else
        {
            cout<<"修改失败"<<endl;    
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
    
    output<<"图片总数为："<<size<<endl;
    if(current!=NULL)
    {
        output << left << setw(10) << "编号" 
           << left << setw(20) << "标题" 
           << left << setw(20) << "作者" 
           << left << setw(15) << "等级" 
           << left << setw(20) << "出版社" 
           << left << setw(20) << "ISBN" 
           << left << setw(10) << "页数" << endl;
    }
    else
    {
        output<<"!!!书库为空!!!"<<endl;
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
        cout << "文件打开失败" << endl;
        return;
    }
    string id, title, author, level,nation, length, width;
    string judge;
    while(input >> judge)
    {
        if(judge=="书籍总数为：")
        {
            input >> size;
        }
        else if(judge=="页数")
        {
            input >> id >> title >> author >> level >> nation >> length >> width;
            Picture picture;
            picture.Set(id, title, author, level, nation, length, width);
            Add_Picture(picture);
        }
    }
    input.close();
}