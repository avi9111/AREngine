#pragma once
#ifndef _COMMON_FUNCTION_H
#define _COMMON_FUNCTION_H

#include<iostream>
#include<string>
#include<strstream>
#include<sstream>
#include<Windows.h>
using namespace std;

/// <summary>
/// 不知道有什么卵用的方法，作用几乎==0
/// </summary>
/// <param name=""></param>
/// <returns></returns>
wstring Str2Wstr(string);
string Wstr2Str(wstring);

void int2str(const int &, string &);


#endif // !_COMMON_FUNCTION_H
