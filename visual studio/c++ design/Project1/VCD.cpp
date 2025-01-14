#include "VCD.h"


void VCD::Set(string id, string title, string author, string level, string producer, string year, string duration)
{
    ID = id;
    Title = title;
    Author = author;
    Level = level;
    Producer = producer;
    Year = year;
    Duration = duration;
}

void VCD::Get()
{
    cout << "���: " << setw(2) << ID
        << setw(12) << "����:" << setw(10) << Title
        << setw(12) << "����:" << setw(8) << Author
        << setw(12) << "�ȼ�:" << setw(3) << Level
        << setw(12) << "������:" << setw(8) << Producer
        << setw(12) << "���:" << setw(12) << Year
        << setw(12) << "ʱ��:" << setw(6) << Duration << endl;
}

istream& operator>>(istream& in, VCD& vcd) {
    cout << "��������:  ";
    in >> vcd.ID;
    cout << "���������: ";
    in >> vcd.Title;
    cout << "���������ߣ� ";
    in >> vcd.Author;
    cout << "������ȼ�: ";
    in >> vcd.Level;
    cout << "������������: ";
    in >> vcd.Producer;
    cout << "���������: ";
    in >> vcd.Year;
    cout << "������ʱ��: ";
    in >> vcd.Duration;
    return in;
}

ostream& operator<<(ostream& out, const VCD& vcd) {
    out << "���: " << setw(2) << vcd.ID
        << setw(12) << "����:" << setw(10) << vcd.Title
        << setw(12) << "����:" << setw(8) << vcd.Author
        << setw(12) << "�ȼ�:" << setw(3) << vcd.Level
        << setw(12) << "������:" << setw(8) << vcd.Producer
        << setw(12) << "���:" << setw(12) << vcd.Year
        << setw(12) << "ʱ��:" << setw(6) << vcd.Duration << endl;
    return out;
}

int VCD_Data::isExist_VCD_Title(string title) {
    int ret = -1;
    VCD_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->vcd.Get_Title() == title) {
            break;
        }
        current = current->next;
    }
    return ret;
}

int VCD_Data::isExist_VCD_ID(string id) {
    int ret = -1;
    VCD_Note* current = head;
    while (current != nullptr) {
        ret++;
        if (current->vcd.Get_ID() == id) {
            break;
        }
        current = current->next;
    }
    return ret;
}

void VCD_Data::Add_VCD(VCD& vcd) {
    VCD_Note* new_vcd = new VCD_Note(vcd);
    if (head == NULL) {
        head = new_vcd;
        tail = new_vcd;
    }
    else {
        tail->next = new_vcd;
        tail = new_vcd;
    }
    size++;
}

void VCD_Data::Display_VCD() {
    VCD_Note* current = head;
    while (current != NULL) {
        cout << current->vcd << endl;
        current = current->next;
    }
}

void VCD_Data::Storage_VCD()
{
    VCD_Note* current = head;
    fstream output("vcd.txt", ios::out);
    output << "��Ƶ��������Ϊ��" << size << endl;
    if (current != NULL) {
        output << left << setw(10) << "���"
            << left << setw(20) << "����"
            << left << setw(20) << "����"
            << left << setw(15) << "�ȼ�"
            << left << setw(15) << "������"
            << left << setw(15) << "���"
            << left << setw(15) << "ʱ��" << endl;
    }
    else {
        output << "!!!��Ƶ���̿�Ϊ��!!!" << endl;
    }
    while (current != NULL) {
        output << left << setw(10) << current->vcd.Get_ID()
            << left << setw(20) << current->vcd.Get_Title()
            << left << setw(20) << current->vcd.Get_Author()
            << left << setw(15) << current->vcd.Get_Level()
            << left << setw(15) << current->vcd.Get_Producer()
            << left << setw(15) << current->vcd.Get_Year()
            << left << setw(15) << current->vcd.Get_Duration() << endl;
        current = current->next;
    }
    output.close();
}

void VCD_Data::Load_VCD()
{
    fstream input("vcd.txt", ios::in);
    if (!input)
    {
        cout << "�ļ�������" << endl;
        return;
    }
    string temp;
    getline(input, temp);
    while (!input.eof())
    {
        string id, title, author, level, producer, year, duration;
        input >> id >> title >> author >> level >> producer >> year >> duration;
        VCD vcd;
        vcd.Set(id, title, author, level, producer, year, duration);
        Add_VCD(vcd);
    }
    input.close();
}

void VCD_Data::Edit_VCD_Title(string title)
{
    VCD_Note* current = head;
    bool flag = false;
    while (current != NULL)
    {
        if (current->vcd.Get_Title() == title)
        {
            cout << current->vcd << endl;
            cout << "�������µ���Ƶ������Ϣ" << endl;
            VCD temp;
            cin >> temp;
            current->vcd = temp;
            flag = true;
            break;
        }
        current = current->next;
    }
    if (flag)
    {
        cout << "�޸ĳɹ�" << endl;
    }
    else
    {
        cout << "�޸�ʧ��" << endl;
    }
}

void VCD_Data::Edit_VCD_ID(string id)
{
    VCD_Note* current = head;
    bool flag = false;
    while (current != NULL)
    {
        if (current->vcd.Get_ID() == id)
        {
            cout << current->vcd << endl;
            cout << "�������µ���Ƶ������Ϣ" << endl;
            VCD temp;
            cin >> temp;
            current->vcd = temp;
            flag = true;
            break;
        }
        current = current->next;
    }
    if (flag)
    {
        cout << "�޸ĳɹ�" << endl;
    }
    else
    {
        cout << "�޸�ʧ��" << endl;
    }
}

void VCD_Data::Delete_VCD(string title)
{
    VCD_Note* current = head;
    VCD_Note* previous = NULL;
    while (current != NULL) {
        if (current->vcd.Get_Title() == title) {
            if (previous == NULL) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }
            if (current == tail) {
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
    cout << "��VCD������" << endl;
}

