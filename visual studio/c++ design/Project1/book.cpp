#include "book.h"

void Book::Set(string id, string title, string author, string level, string publisher, string ibsn, string page)
{
    ID = id;
    Title = title;
    Author = author;
    Level = level;
    Publisher = publisher;
    IBSN = ibsn;
    Page = page;
}

void Book::Get()
{
               cout << "编号: " << setw(2) << ID
        << setw(12) << "标题:" << setw(10) << Title
        << setw(12) << "作者:" << setw(8) << Author
        << setw(12) << "等级:" << setw(3) << Level
        << setw(12) << "出版社:" << setw(8) << Publisher
        << setw(12) << "IBSN:" << setw(12) << IBSN
        << setw(12) << "页数:" << setw(6) << Page << endl;
}

istream& operator>>(istream& in, Book& book) {
    cout << "请输入编号:  ";
    in >> book.ID;
    cout << "请输入标题: ";
    in >> book.Title;
    cout << "请输入作者： ";
    in >> book.Author;
    cout << "请输入等级: ";
    in >> book.Level;
    cout << "请输入出版社: ";
    in >> book.Publisher;
    cout << "请输入IBSN: ";
    in >> book.IBSN;
    cout << "请输入页数: ";
    in >> book.Page;
    return in;
}

ostream& operator<<(ostream& out, const Book& book) { // 修改为 const Book&
    out << "编号: " << setw(2) << book.ID
        << setw(12) << "标题:" << setw(10) << book.Title
        << setw(12) << "作者:" << setw(8) << book.Author
        << setw(12) << "等级:" << setw(3) << book.Level
        << setw(12) << "出版社:" << setw(8) << book.Publisher
        << setw(12) << "IBSN:" << setw(12) << book.IBSN
        << setw(12) << "页数:" << setw(6) << book.Page << endl;
    return out;
}

int Book_Data::isExist_Book_Title(string title) {
    int ret = -1;
    Book_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->book.Get_Title() == title) {
            break;// 找到后立即跳出循环
        }
        current = current->next;
    }
    return ret;
}

int Book_Data::isExist_Book_ID(string id) {
    int ret = -1;
    Book_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->book.Get_ID() == id) {
            break;// 找到后立即跳出循环
        }
        current = current->next;
    }
    return ret;
}

void Book_Data::Find_Book_Title(string title) {
    int index = isExist_Book_Title(title);
    if (index != -1) {
        Book_Note* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        cout << current->book << endl;
        cout << "查询成功" << endl;
    } else {
        cout << "未查询到此书" << endl;
    }
}

void Book_Data::Find_Book_ID(string id) {
    int index = isExist_Book_ID(id);
    if (index != -1) {
        Book_Note* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        cout << current->book << endl;
        cout << "查询成功" << endl;
    }
    else {
        cout << "未查询到此书" << endl;
    }
}

void Book_Data::Delete_Book(string title)
{
    Book_Note* current = head;
    Book_Note* previous = NULL;
    while (current != NULL)
    {
        if (current->book.Get_Title() == title)
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

void Book_Data::Edit_Book_Title(string title)
{
    Book_Note* current = head;
    bool flag = false;
    while(current!=NULL)
    {
        if(current->book.Get_Title()==title)
        {
            cout<<current->book<<endl;
            cout<<"请输入新的书籍信息"<<endl;
            Book temp;
            cin>>temp;
            current->book=temp;
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
void Book_Data::Edit_Book_ID(string id)
{
    Book_Note* current = head;
    bool flag = false;
    while (current != NULL)
    {
        if (current->book.Get_ID() == id)
        {
            cout << current->book << endl;
            cout << "请输入新的书籍信息" << endl;
            Book temp;
            cin >> temp;
            current->book = temp;
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

void Book_Data::Add_Book(Book& book)
{
    Book_Note* newNode = new Book_Note(book);
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

void Book_Data::Display_Book()
{
    p = head;
    while (p != NULL)
    {
        cout << p->book << endl;
        p = p->next;
    }
}

void Book_Data::Storage_Book()
{
    Book_Note* current=head;
    fstream output("book.txt", ios::out);
    
    output<<"书籍总数为："<<size<<endl;
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
        output << left << setw(10) << current->book.Get_ID()
               << left << setw(20) << current->book.Get_Title()
               << left << setw(20) << current->book.Get_Author()
               << left << setw(15) << current->book.Get_Level()
               << left << setw(20) << current->book.Get_Publisher()
               << left << setw(20) << current->book.Get_IBSN()
               << left << setw(10) << current->book.Get_Page() << endl;
        current = current->next;
    }
}

void Book_Data::Load_Book()
{
    fstream input("book.txt", ios::in);
    if (!input)
    {
        cout << "文件打开失败" << endl;
        return;
    }
    string id, title, author, level, publisher, ibsn, page;
    string judge;
    while(input >> judge)
    {
        if(judge=="书籍总数为：")
        {
            input >> size;
        }
        else if(judge=="页数")
        {
            while(input >> id >> title >> author >> level >> publisher >> ibsn >> page)
            {
                Book temp;
                temp.Set(id, title, author, level, publisher, ibsn, page);
                Add_Book(temp);
            }
        }
    }
    input.close();
}