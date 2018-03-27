#pragma once
#include <stdint.h>
#include "stdio.h"
#include "windows.h"
using namespace std;
//引入json解析库
#include "json.h"
////定义tstring类型
//#ifndef __TSTRING__
//#define  __TSTRING__
//#	ifdef _UNICODE
//typedef std::wstring tstring;
//#	else
//typedef std::string tstring;
//#	endif
//#endif

//typedef unsigned __int32 uint32_t;
//typedef unsigned short  uint16_t;
class JSON_API JSONER
{
public:
	JSONER();
	~JSONER();
	static bool parse(wstring &jsonstr, Json::Value &value);
	static bool parse(string &jsonstr, Json::Value &value);
	static std::wstring CharToWchar(const char* c, size_t m_encode = 0);
	static std::string WcharToChar(const wchar_t* wp, size_t m_encode = 0);
};

