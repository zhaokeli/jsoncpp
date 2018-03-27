#include "../../include/json/JSONER.h"



JSONER::JSONER()
{
}


JSONER::~JSONER()
{
}

bool JSONER::parse(wstring &jsonstr, Json::Value &value)
{
	std::string jstr = "";
	jstr = WcharToChar(jsonstr.c_str());
	Json::Reader reader;
	return reader.parse(jstr, value);
}

bool JSONER::parse(string & jsonstr, Json::Value & value)
{
	Json::Reader reader;
	return reader.parse(jsonstr, value);
}

std::wstring JSONER::CharToWchar(const char* c, size_t m_encode)
{
	std::wstring str;
	int len = MultiByteToWideChar(m_encode, 0, c, strlen(c), NULL, 0);
	wchar_t*    m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(m_encode, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	str = m_wchar;
	delete m_wchar;
	return str;
}

std::string JSONER::WcharToChar(const wchar_t* wp, size_t m_encode)
{
	std::string str;
	int len = WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	char    *m_char = new char[len + 1];
	WideCharToMultiByte(m_encode, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}