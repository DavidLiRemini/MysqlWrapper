#pragma once
#include "build_config.h"
#include <iostream>
#include <string>


namespace SqlHelper
{
	class LIBSQLSTRING_API SQLString
	{
	private:
		
		std::string realStr;
		static const size_t npos = static_cast<std::string::size_type>(-1);
	public:
		SQLString();
		SQLString(const SQLString&);
		SQLString(const char[]);
		SQLString(const std::string&);
		SQLString(const char*, size_t);
		//operator=
		SQLString& operator=(const SQLString&);
		SQLString& operator=(const std::string&);
		SQLString& operator=(const char*);
		SQLString& operator+=(const SQLString&);
		operator const std::string&()const;
		std::string* operator->();
		//compare
		int Compare(const SQLString&)const;
		int Compare(const std::string&)const;
		int Compare(const char*)const;
		int CaseInsensitiveCompare(const SQLString&)const;
		int CaseInsensitiveCompare(const std::string&)const;
		int CaseInsensitiveCompare(const char*)const;
		//
		size_t length()const;
		const char* c_str()const;
		const std::string& AsStdString()const;
		SQLString& Append(const SQLString&);
		SQLString& Append(const std::string&);
		SQLString& Append(const char*);
		const char& operator[](size_t)const;
		//find
		size_t find(char, size_t = 0)const;
		size_t find(const SQLString&, size_t = 0)const;
		SQLString substr(size_t, size_t = npos)const;
		size_t find_first_of(char, size_t = 0)const;
		size_t find_last_of(char, size_t = npos)const;
		SQLString& replace(size_t pos, size_t len, const SQLString& str);
		bool Contains(char ch)const;
	};


	//NonMember operator
	inline const SQLString operator+(const SQLString& lhs, const SQLString& rhs)
	{
		return SQLString(lhs.AsStdString() + rhs.AsStdString());
	}
	inline bool operator==(const SQLString& lhs, const SQLString& rhs)
	{
		return (lhs.AsStdString() == rhs.AsStdString());
	}
	inline bool operator!= (const SQLString& lhs, const SQLString& rhs)
	{
		return !(lhs == rhs);
	}
}

namespace std
{
	inline ostream& operator<<(ostream & os, const SqlHelper::SQLString & str)
	{
		return os << str.AsStdString();
	}
}