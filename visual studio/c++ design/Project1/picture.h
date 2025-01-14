#ifndef PICTURE_H
#define PICTURE_H

#include "Item.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

class Picture : public Item
{
public:

    friend istream& operator>>(istream&, Picture& picture);

    friend ostream& operator<<(ostream&, const Picture& picture);

    Picture() : Item("", "", "", ""), Nation(""), Length(""), Width("") {}

    Picture(string id, string title, string author, string level, string nation, string length, string width)
        : Item(id, title, author, level), Nation(nation), Length(length), Width(width) {}

    void Set(string id, string title, string author, string level, string nation, string length, string width);

    void Get();

    inline string Get_Nation();

    inline string Get_Length();

    inline string Get_Width();
private:

    string Nation;

    string Length;

    string Width;
    
};


class Picture_Note
{
public:
    Picture picture;
    Picture_Note* next;
    Picture_Note(Picture p) : picture(p), next(NULL) {}
};

class Picture_Data {
public:


    Picture_Data() : head(NULL), p(NULL), tail(NULL) {} // 默认构造函数

    inline int Get_Size();

    void Add_Picture(Picture& picture);

    void Display_Picture();

    int isExist_Picture_Title(string title);

    int isExist_Picture_ID(string id);

    void Find_Picture_Title(string title);

    void Find_Picture_ID(string id);

    void Delete_Picture(string title);

    void Edit_Picture_Title(string title);

    void Edit_Picture_ID(string id);

    void Storage_Picture();

    void Load_Picture();

private:

    Picture_Note* head;

    Picture_Note* p;

    Picture_Note* tail;

    int size = 0;
};

int Picture_Data::Get_Size()
{
    return size;
}

string Picture::Get_Nation()
{
    return Nation;
}

string Picture::Get_Length()
{
    return Length;
}

string Picture::Get_Width()
{
    return Width;
}

#endif // PICTURE_H


