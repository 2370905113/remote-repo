#ifndef BOOK_H
#define BOOK_H

#include "Item.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

class Book : public Item
{
public:
   
    friend istream& operator>>(istream&, Book& book);

    friend ostream& operator<<(ostream&, const Book& book);

    Book() : Item("", "", "", ""), Publisher(""), IBSN(""), Page("") {} 

    Book(string id, string title, string author, string level, string publish, string ibsn, string page) :Item(id, title, author, level), Publisher(publish), IBSN(ibsn), Page(page) {}

    void Set(string id, string title, string author, string level, string publisher, string ibsn, string page);
   
    void Get();

    inline string Get_Publisher(); 

    inline string Get_IBSN();

    inline string Get_Page();
private:
   
    string Publisher;
   
    string IBSN;
   
    string Page;
};


class Book_Note
{
public:
    Book book;
    Book_Note* next;
    Book_Note(Book b) : book(b), next(NULL) {}
};

class Book_Data {
public:


    Book_Data() : head(NULL), p(NULL), tail(NULL) {} // 默认构造函数

    inline int Get_Size();
    
    void Add_Book(Book& book);

    void Display_Book();

    int isExist_Book_Title(string title);

    int isExist_Book_ID(string id);
    
    void Find_Book_Title(string title);
    
    void Find_Book_ID(string id);

    void Delete_Book(string title); 

    void Edit_Book_Title(string title);

    void Edit_Book_ID(string id);

    void Storage_Book();

    void Load_Book();

private:

    Book_Note* head;

    Book_Note* p;

    Book_Note* tail;
    
    int size = 0;
};

int Book_Data::Get_Size()
{
    return size;
}

string Book::Get_Publisher()
{
    return Publisher;
}

string Book::Get_IBSN()
{
    return IBSN;
}

string Book::Get_Page()
{
    return Page;
}

#endif // BOOK_H