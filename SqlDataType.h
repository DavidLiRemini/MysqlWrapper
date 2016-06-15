#pragma once

#include "build_config.h"

namespace SqlHelper
{
	class SQLString;
	class LIBSQLSTRING_API SqlDataType
	{
	public:
		enum class Type
		{
			SQL_UNKNOWN_TYPE = -1,
			SQL_BOOLEAN,
			SQL_BYTE,
			SQL_FLOAT,
			SQL_DOUBLE,
			SQL_SMALLINTEGER,
			SQL_INTERGER,
			SQL_BIGINTEGER,
			SQL_STRING,
			SQL_DATE,
			SQL_DATETIME,
			SQL_TIMESTAMP,
			SQL_ENUM
		};
	private:
		union
		{
			int intVal;
			SQLString* strVal;
			double doubleVal;
			float floatVal;
			bool boolVal;
			unsigned char byteVal;

		}dataField;
		Type variableType;
		void Clear();
	public:
		SqlDataType();
		~SqlDataType();
		explicit SqlDataType(int);
		explicit SqlDataType(double);
		explicit SqlDataType(float);
		explicit SqlDataType(bool);
		explicit SqlDataType(SQLString);
		explicit SqlDataType(const char*);
		explicit SqlDataType(char);
		SqlDataType(const SqlDataType&);
		SqlDataType(SqlDataType&&);
		SqlDataType& operator=(const SqlDataType&);
		SqlDataType& operator=(SqlDataType&&);

		Type GetType()const;
		unsigned char AsByte()const;
		int AsInt()const;
		double AsDouble()const;
		bool AsBool()const;
		float AsFloat()const;
		SqlHelper::SQLString AsSqlString()const;
	};
}