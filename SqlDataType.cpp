#include "stdafx.h"

#include "SqlDataType.h"
#include "SqlString.h"

#include <string.h>
#include <sstream>
#include <assert.h>

namespace SqlHelper
{
	SqlDataType::SqlDataType()
		:variableType(Type::SQL_UNKNOWN_TYPE)
	{
		memset(&dataField, 0, sizeof(dataField));
	}

	SqlDataType::~SqlDataType()
	{
		Clear();
	}
	SqlDataType::SqlDataType(int value)
		: variableType(Type::SQL_INTERGER)
	{
		dataField.intVal = value;
	}

	SqlDataType::SqlDataType(float value)
		: variableType(Type::SQL_FLOAT)
	{
		dataField.floatVal = value;
	}

	SqlDataType::SqlDataType(double value)
		: variableType(Type::SQL_DOUBLE)
	{
		dataField.doubleVal = value;
	}

	SqlDataType::SqlDataType(bool value)
		: variableType(Type::SQL_BOOLEAN)
	{
		dataField.boolVal = value;
	}

	SqlDataType::SqlDataType(SQLString value)
		: variableType(Type::SQL_STRING)
	{
		dataField.strVal = new SQLString();
		*dataField.strVal = value;
		auto index = dataField.strVal->find_first_of('-');
		auto index2 = dataField.strVal->find_last_of(':');
		if ((index == 4) && (index2 == 16) && (dataField.strVal->length() == 19))
		{
			variableType = Type::SQL_DATETIME;
		}
	}

	SqlDataType::SqlDataType(const char* str)
		:variableType(Type::SQL_STRING)
	{
		dataField.strVal = new SQLString();
		*dataField.strVal = str;
		auto index = dataField.strVal->find_first_of('-');
		auto index2 = dataField.strVal->find_last_of(':');
		if (((index == 4) && (index2 == 16) && (dataField.strVal->length() == 19)) ||
			(dataField.strVal->length() == 14))
		{
			bool flag = true;
			for (const auto t : dataField.strVal->AsStdString())
			{
				if (!(t >= '0' && t <= '9'))
				{
					flag = false;
					break;
				}
			}
			if (dataField.strVal->length() == 19 || flag)
			{
				variableType = Type::SQL_DATETIME;
			}
		}
	}

	SqlDataType::SqlDataType(char value)
		:variableType(Type::SQL_BYTE)
	{
		dataField.byteVal = value;
	}

	void SqlDataType::Clear()
	{
		Type tp = variableType;
		switch (tp)
		{
		case SqlHelper::SqlDataType::Type::SQL_UNKNOWN_TYPE:
			break;
		case SqlHelper::SqlDataType::Type::SQL_BOOLEAN:
			dataField.boolVal = false;
			break;
		case SqlHelper::SqlDataType::Type::SQL_BYTE:
			dataField.byteVal = 0;
			break;
		case SqlHelper::SqlDataType::Type::SQL_FLOAT:
			dataField.floatVal = 0.0f;
			break;
		case SqlHelper::SqlDataType::Type::SQL_DOUBLE:
			dataField.doubleVal = 0.0;
			break;
		case SqlHelper::SqlDataType::Type::SQL_SMALLINTEGER:
		case SqlHelper::SqlDataType::Type::SQL_INTERGER:
		case SqlHelper::SqlDataType::Type::SQL_BIGINTEGER:
			dataField.intVal = 0;
			break;
		case SqlHelper::SqlDataType::Type::SQL_STRING:
		case SqlHelper::SqlDataType::Type::SQL_DATE:
		case SqlHelper::SqlDataType::Type::SQL_DATETIME:
		case SqlHelper::SqlDataType::Type::SQL_ENUM:
		case SqlHelper::SqlDataType::Type::SQL_TIMESTAMP:
			if (dataField.strVal != nullptr)
			{
				delete dataField.strVal;
			}
			dataField.strVal = nullptr;
			break;
		default:
			break;
		}
	}

	SqlDataType::SqlDataType(const SqlDataType& rhs)
		:variableType(Type::SQL_UNKNOWN_TYPE)
	{
		memset(&dataField, 0, sizeof(dataField));
		*this = rhs;
	}
	SqlDataType::SqlDataType(SqlDataType&& rhs)
		: variableType(Type::SQL_UNKNOWN_TYPE)
	{
		*this = std::move(rhs);
	}

	SqlDataType& SqlDataType::operator=(const SqlDataType& rhs)
	{

		if (this != &rhs)
		{
			Clear();
			variableType = rhs.variableType;
			switch (variableType)
			{
			case SqlHelper::SqlDataType::Type::SQL_UNKNOWN_TYPE:
				break;
			case SqlHelper::SqlDataType::Type::SQL_BOOLEAN:
				dataField.boolVal = rhs.dataField.boolVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_BYTE:
				dataField.byteVal = rhs.dataField.byteVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_FLOAT:
				dataField.floatVal = rhs.dataField.floatVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_DOUBLE:
				dataField.doubleVal = rhs.dataField.doubleVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_SMALLINTEGER:
			case SqlHelper::SqlDataType::Type::SQL_INTERGER:
			case SqlHelper::SqlDataType::Type::SQL_BIGINTEGER:
				dataField.intVal = rhs.dataField.intVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_STRING:
			case SqlHelper::SqlDataType::Type::SQL_DATE:
			case SqlHelper::SqlDataType::Type::SQL_DATETIME:
			case SqlHelper::SqlDataType::Type::SQL_ENUM:
			case SqlHelper::SqlDataType::Type::SQL_TIMESTAMP:
				if (dataField.strVal == nullptr)
				{
					dataField.strVal = new SQLString();
				}
				*dataField.strVal = *rhs.dataField.strVal;
				break;
			default:
				break;
			}
		}
		return *this;
	}

	SqlDataType& SqlDataType::operator=(SqlDataType&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			switch (rhs.variableType)
			{
			case SqlHelper::SqlDataType::Type::SQL_UNKNOWN_TYPE:
				break;
			case SqlHelper::SqlDataType::Type::SQL_BOOLEAN:
				dataField.boolVal = rhs.dataField.boolVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_BYTE:
				dataField.byteVal = rhs.dataField.byteVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_FLOAT:
				dataField.floatVal = rhs.dataField.floatVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_DOUBLE:
				dataField.doubleVal = rhs.dataField.doubleVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_SMALLINTEGER:
			case SqlHelper::SqlDataType::Type::SQL_INTERGER:
			case SqlHelper::SqlDataType::Type::SQL_BIGINTEGER:
				dataField.intVal = rhs.dataField.intVal;
				break;
			case SqlHelper::SqlDataType::Type::SQL_STRING:
			case SqlHelper::SqlDataType::Type::SQL_DATE:
			case SqlHelper::SqlDataType::Type::SQL_DATETIME:
			case SqlHelper::SqlDataType::Type::SQL_ENUM:
			case SqlHelper::SqlDataType::Type::SQL_TIMESTAMP:
				dataField.strVal = rhs.dataField.strVal;
				break;
			default:
				break;
			}
			variableType = rhs.variableType;
			memset(&rhs.dataField, 0, sizeof(rhs.dataField));
			rhs.variableType = Type::SQL_UNKNOWN_TYPE;
		}
		return *this;
	}
	unsigned char SqlDataType::AsByte()const
	{
		assert((variableType != Type::SQL_STRING&& variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_BYTE)
		{
			return dataField.byteVal;
		}
		if (variableType == Type::SQL_BOOLEAN)
		{
			return dataField.boolVal ? 1 : 0;
		}
		if (variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_BIGINTEGER ||
			variableType == Type::SQL_SMALLINTEGER)
		{
			return static_cast<unsigned char>(dataField.intVal);
		}
		if (variableType == Type::SQL_FLOAT)
		{
			return static_cast<unsigned char>(dataField.floatVal);
		}
		if (variableType == Type::SQL_DOUBLE)
		{
			return static_cast<unsigned char>(dataField.doubleVal);
		}
		return 0;
	}

	int SqlDataType::AsInt()const
	{
		assert((variableType != Type::SQL_STRING&& variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_BYTE)
		{
			return dataField.byteVal;
		}
		if (variableType == Type::SQL_BOOLEAN)
		{
			return dataField.boolVal ? 1 : 0;
		}
		if (variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_BIGINTEGER ||
			variableType == Type::SQL_SMALLINTEGER)
		{
			return (dataField.intVal);
		}
		if (variableType == Type::SQL_FLOAT)
		{
			return static_cast<int>(dataField.floatVal);
		}

		if (variableType == Type::SQL_DOUBLE)
		{
			return static_cast<int>(dataField.doubleVal);
		}
		return 0;
	}

	float SqlDataType::AsFloat()const
	{
		assert((variableType != Type::SQL_STRING && variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_FLOAT)
		{
			return dataField.floatVal;
		}
		else if (variableType == Type::SQL_DOUBLE)
		{
			return static_cast<float>(dataField.doubleVal);
		}
		else if (variableType == Type::SQL_BOOLEAN)
		{
			return dataField.boolVal ? 1.0f : 0.0f;
		}
		else if (variableType == Type::SQL_BYTE)
		{
			return dataField.byteVal;
		}
		else if (variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_SMALLINTEGER ||
			variableType == Type::SQL_BIGINTEGER)
		{
			return static_cast<float>(dataField.intVal);
		}
		return 0.0f;
	}

	double SqlDataType::AsDouble()const
	{
		assert((variableType != Type::SQL_STRING && variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_DOUBLE)
		{
			return dataField.doubleVal;
		}
		if (variableType == Type::SQL_BOOLEAN)
		{
			return dataField.boolVal ? 1.0 : 0.0;
		}
		if (variableType == Type::SQL_BYTE)
		{
			return dataField.byteVal;
		}
		if (variableType == Type::SQL_FLOAT)
		{
			return dataField.floatVal;
		}
		if (variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_SMALLINTEGER ||
			variableType == Type::SQL_BIGINTEGER)
		{
			return static_cast<double>(dataField.intVal);
		}
		return 0.0;
	}

	bool SqlDataType::AsBool()const
	{
		assert((variableType != Type::SQL_STRING && variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_BOOLEAN)
		{
			return dataField.boolVal;
		}
		if (variableType == Type::SQL_BYTE)
		{
			return dataField.byteVal ? true : false;
		}
		if (variableType == Type::SQL_FLOAT)
		{
			return dataField.floatVal ? true : false;
		}
		if (variableType == Type::SQL_DOUBLE)
		{
			return dataField.doubleVal ? true : false;
		}
		if (variableType == Type::SQL_BIGINTEGER ||
			variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_SMALLINTEGER)
		{
			return dataField.intVal ? true : false;
		}
		return false;
	}
	SqlHelper::SQLString SqlDataType::AsSqlString()const
	{
		assert((variableType != Type::SQL_UNKNOWN_TYPE) && "type should only in double int float bool");
		if (variableType == Type::SQL_STRING ||
			variableType == Type::SQL_DATETIME ||
			variableType == Type::SQL_TIMESTAMP ||
			variableType == Type::SQL_ENUM)
		{
			return *dataField.strVal;
		}
		std::stringstream retValue;
		if (variableType == Type::SQL_BYTE)
		{
			retValue << dataField.byteVal;
		}
		else if (variableType == Type::SQL_BOOLEAN)
		{
			retValue << (dataField.boolVal ? "true" : "false");
		}
		else if (variableType == Type::SQL_BIGINTEGER ||
			variableType == Type::SQL_INTERGER ||
			variableType == Type::SQL_SMALLINTEGER)
		{
			retValue << dataField.intVal;
		}
		else if (variableType == Type::SQL_FLOAT)
		{
			char temp[20] = { 0 };
			int sz = sprintf_s(temp, sizeof(temp), "%-10.6f", dataField.floatVal);
			assert(sz == 10);
			retValue << temp;
		}
		else if (variableType == Type::SQL_DOUBLE)
		{
			char temp[20] = { 0 };
			int sz = sprintf_s(temp, sizeof(temp), "%-16.12f", dataField.doubleVal);
			assert(sz == 16);
			retValue << temp;
		}

		return retValue.str();
	}
	SqlDataType::Type SqlDataType::GetType()const
	{
		return  variableType;
	}
}