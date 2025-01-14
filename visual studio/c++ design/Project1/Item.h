#ifndef ITEM_H
#define ITEM_H

#include <string>
#define MAX 100
using namespace std;

class Item
{
protected:
    string ID;
    string Title;
    string Author;
    string Level;

public:
    Item(string id, string title, string author, string level)
        :ID(id), Title(title), Author(author), Level(level) {
    }
    void Get_Item(string id, string title, string author, string level);

    inline virtual string Get_Title();

	inline virtual string Get_ID();
    
    inline virtual string Get_Author();

    inline virtual string Get_Level();

};
string Item::Get_Title()
{
    return Title;
}

string Item::Get_ID()
{
    return ID;
}

string Item::Get_Author()
{
    return Author;
}

string Item::Get_Level()
{
    return Level;
}

#endif // ITEM_H
#pragma once
