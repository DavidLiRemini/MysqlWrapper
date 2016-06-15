#include "stdafx.h"

#include "SqlString.h"

#include <algorithm>

namespace SqlHelper
{
	SQLString::SQLString()
	{}

	SQLString::SQLString(const char* str, size_t n)
		:realStr(str, n)
	{}

	SQLString::SQLString(const std::string& str)
		: realStr(str)
	{}

	SQLString::SQLString(const char p[])
		: realStr(p)
	{}

	SQLString::SQLString(const SQLString& sqlstr)
	{
		realStr = sqlstr.AsStdString();
	}

	SQLString& SQLString::operator=(const SQLString& rhs)
	{
		realStr = rhs.realStr;
		return *this;
	}

	SQLString& SQLString::operator=(const std::string& rhs)
	{
		realStr = rhs;
		return *this;
	}

	SQLString& SQLString::operator=(const char* str)
	{
		realStr = str;
		return *this;
	}

	SQLString& SQLString::operator+=(const SQLString& rhs)
	{
		realStr += rhs.realStr;
		return *this;
	}

	SQLString::operator const std::string &()const
	{
		return realStr;
	}

	std::string* SQLString::operator->()
	{
		return &realStr;
	}

	int SQLString::Compare(const char* str)const
	{
		return realStr.compare(str);
	}

	int SQLString::Compare(const std::string& rhs)const
	{
		return realStr.compare(rhs);
	}

	int SQLString::Compare(const SQLString& rhs)const
	{
		return realStr.compare(rhs.AsStdString());
	}

	int SQLString::CaseInsensitiveCompare(const char* str)const
	{
		std::string tmp = realStr;
		std::string temp = str;
		auto func = [](char c)
		{
			if (c >= 65 && c <= 90)
			{
				c = c + 32;
			}
			return c;
		};
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), func);
		std::transform(temp.begin(), temp.end(), temp.begin(), func);
		return tmp.compare(temp);
	}

	int SQLString::CaseInsensitiveCompare(const std::string& rhs)const
	{
		std::string tmp = realStr;
		std::string temp = rhs;
		auto func = [](char c)
		{
			if (c >= 65 && c <= 90)
			{
				c = c + 32;
			}
			return c;
		};
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), func);
		std::transform(temp.begin(), temp.end(), temp.begin(), func);
		return tmp.compare(temp);
	}

	int SQLString::CaseInsensitiveCompare(const SQLString& rhs)const
	{
		std::string tmp = realStr;
		std::string temp = rhs.AsStdString();
		auto func = [](char c)
		{
			if (c >= 65 && c <= 90)
			{
				c = c + 32;
			}
			return c;
		};
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), func);
		std::transform(temp.begin(), temp.end(), temp.begin(), func);
		return tmp.compare(temp);
	}

	size_t SQLString::length()const
	{
		return realStr.length();
	}

	const char* SQLString::c_str()const
	{
		return realStr.c_str();
	}

	const std::string& SQLString::AsStdString()const
	{
		return realStr;
	}

	SQLString& SQLString::Append(const char* str)
	{
		realStr.append(str);
		return *this;
	}

	SQLString& SQLString::Append(const std::string& rhs)
	{
		realStr.append(rhs);
		return *this;
	}

	SQLString& SQLString::Append(const SQLString& rhs)
	{
		realStr.append(rhs);
		return *this;
	}

	const char& SQLString::operator[](size_t index)const
	{
		return realStr[index];
	}

	size_t SQLString::find(const SQLString& rhs, size_t index/* = 0 */)const
	{
		return realStr.find(rhs, index);
	}

	size_t SQLString::find(char c, size_t index/* = 0 */)const
	{
		return realStr.find(c, index);
	}

	SQLString SQLString::substr(size_t left, size_t len/* = npos */)const
	{
		return SQLString(realStr.substr(left, len));
	}

	size_t SQLString::find_first_of(char c, size_t pos /* = 0 */)const
	{
		return realStr.find_first_of(c, pos);
	}

	size_t SQLString::find_last_of(char c, size_t npos/* = npos */)const
	{
		return realStr.find_last_of(c, npos);
	}

	SQLString& SQLString::replace(size_t pos, size_t len, const SQLString& str)
	{
		realStr.replace(pos, len, str.AsStdString());
		return *this;
	}

	bool SQLString::Contains(char ch)const
	{
		int existOrNot = this->find(ch);
		if (existOrNot != -1)
		{
			return true;
		}
		return false;
	}
}
