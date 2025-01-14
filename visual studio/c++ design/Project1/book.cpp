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
               cout << "���: " << setw(2) << ID
        << setw(12) << "����:" << setw(10) << Title
        << setw(12) << "����:" << setw(8) << Author
        << setw(12) << "�ȼ�:" << setw(3) << Level
        << setw(12) << "������:" << setw(8) << Publisher
        << setw(12) << "IBSN:" << setw(12) << IBSN
        << setw(12) << "ҳ��:" << setw(6) << Page << endl;
}

istream& operator>>(istream& in, Book& book) {
    cout << "��������:  ";
    in >> book.ID;
    cout << "���������: ";
    in >> book.Title;
    cout << "���������ߣ� ";
    in >> book.Author;
    cout << "������ȼ�: ";
    in >> book.Level;
    cout << "�����������: ";
    in >> book.Publisher;
    cout << "������IBSN: ";
    in >> book.IBSN;
    cout << "������ҳ��: ";
    in >> book.Page;
    return in;
}

ostream& operator<<(ostream& out, const Book& book) { // �޸�Ϊ const Book&
    out << "���: " << setw(2) << book.ID
        << setw(12) << "����:" << setw(10) << book.Title
        << setw(12) << "����:" << setw(8) << book.Author
        << setw(12) << "�ȼ�:" << setw(3) << book.Level
        << setw(12) << "������:" << setw(8) << book.Publisher
        << setw(12) << "IBSN:" << setw(12) << book.IBSN
        << setw(12) << "ҳ��:" << setw(6) << book.Page << endl;
    return out;
}

int Book_Data::isExist_Book_Title(string title) {
    int ret = -1;
    Book_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->book.Get_Title() == title) {
            break;// �ҵ�����������ѭ��
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
            break;// �ҵ�����������ѭ��
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
        cout << "��ѯ�ɹ�" << endl;
    } else {
        cout << "δ��ѯ������" << endl;
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
        cout << "��ѯ�ɹ�" << endl;
    }
    else {
        cout << "δ��ѯ������" << endl;
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
            cout << "ɾ���ɹ�" << endl;
            return;
        }
        previous = current;
        current = current->next;
    }
    cout << "���鲻����" << endl;
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
            cout<<"�������µ��鼮��Ϣ"<<endl;
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
        cout<<"�޸ĳɹ�"<<endl;
    }
    else
    {
        cout<<"�޸�ʧ��"<<endl;    
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
            cout << "�������µ��鼮��Ϣ" << endl;
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
            cout<<"�޸ĳɹ�"<<endl;
        }
        else
        {
            cout<<"�޸�ʧ��"<<endl;    
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
    
    output<<"�鼮����Ϊ��"<<size<<endl;
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
        cout << "�ļ���ʧ��" << endl;
        return;
    }
    string id, title, author, level, publisher, ibsn, page;
    string judge;
    while(input >> judge)
    {
        if(judge=="�鼮����Ϊ��")
        {
            input >> size;
        }
        else if(judge=="ҳ��")
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