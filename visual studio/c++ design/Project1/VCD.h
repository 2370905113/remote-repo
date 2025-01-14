#ifndef VCD_H
#define VCD_H

#include "Item.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

class VCD : public Item
{
public:
    friend istream& operator>>(istream&, VCD& vcd);
    friend ostream& operator<<(ostream&, const VCD& vcd);

    void Set(string id, string title, string author, string level, string producer, string year, string duration);
    void Get();

    inline string Get_Producer();
    inline string Get_Year();
    inline string Get_Duration();

    VCD() : Item("", "", "", ""), Producer(""), Year(""), Duration("") {}
    VCD(string id, string title, string author, string level, string producer, string year, string duration)
        : Item(id, title, author, level), Producer(producer), Year(year), Duration(duration) {
    }

private:
    string Producer;
    string Year;
    string Duration;
};

class VCD_Note
{
public:
    VCD vcd;
    VCD_Note* next;
    VCD_Note(VCD v) : vcd(v), next(NULL) {}
};

class VCD_Data {
public:
    VCD_Data() : head(NULL), p(NULL), tail(NULL) {}

    void Add_VCD(VCD& vcd);
    void Display_VCD();
    void Edit_VCD_Title(string title);
    void Edit_VCD_ID(string id);
    void Delete_VCD(string title);
    int isExist_VCD_Title(string title);
    int isExist_VCD_ID(string id);
    void Find_VCD_Title(string title);
    void Find_VCD_ID(string id);
    void Storage_VCD();
    void Load_VCD();
    inline int Get_Size();

private:
    VCD_Note* head;
    VCD_Note* p;
    VCD_Note* tail;
    int size = 0;
};

int VCD_Data::Get_Size()
{
    return size;
}

string VCD::Get_Producer()
{
    return Producer;
}

string VCD::Get_Year()
{
    return Year;
}

string VCD::Get_Duration()
{
    return Duration;
}

#endif // VCD_H
#pragma once
#pragma once
