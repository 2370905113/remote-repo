#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <string>
#include "book.h"
#include "VCD.h"
#include "picture.h"
#include "menu.h"
#include <iomanip>
using namespace std;

extern Book_Data book_instance;
extern Book_Data* book_data;
extern VCD_Data vcd_instance;
extern VCD_Data* vcd_data ;
extern Picture_Data picture_instance;
extern Picture_Data* picture_data;

void Task_Init();

void Task_Callback_1();

void Task_Callback_2();

void Task_Callback_3();

void Task_Callback_4();

void Task_Callback_5();

void Task_Callback_6();

void Task_End();
#endif //TASK_H
#pragma once
