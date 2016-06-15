#include "stdafx.h"

#include "SqlAccessor.h"
#include <stdlib.h>
#include <string.h>

namespace SqlHelper
{
	MysqlAccessHelper* MysqlAccessHelper::helperInstance = nullptr;

	MysqlAccessHelper::MysqlAccessHelper()
		:mysql_Instance(nullptr)
	{
		Initialize();
	}

	MysqlAccessHelper* MysqlAccessHelper::GetInstance()
	{
		if (helperInstance == nullptr)
		{
			helperInstance = new MysqlAccessHelper();
		}
		return helperInstance;
	}
	void MysqlAccessHelper::Initialize()
	{
		mysql_Instance = new MYSQL();
		mysql_init(mysql_Instance);

	}

	MysqlAccessHelper::ConnectStates MysqlAccessHelper::GetConnection(const SqlHelper::SQLString& target,
		const SqlHelper::SQLString& user, const SqlHelper::SQLString& pwd, int port)
	{
		if (mysql_Instance == nullptr)
		{
			return ConnectStates::CONNECTFAILED;
		}
		MYSQL* connect_Ptr = mysql_real_connect(mysql_Instance, target.c_str(), user.c_str(), pwd.c_str(),
			nullptr, port, nullptr, CLIENT_MULTI_STATEMENTS);
		if (connect_Ptr == nullptr)
		{
			fprintf(stderr, "Error Code %s\n", mysql_error(mysql_Instance));
			mysql_close(mysql_Instance);
			return ConnectStates::CONNECTFAILED;
		}
		return ConnectStates::CONNECTED;
	}

	void MysqlAccessHelper::SetSchema(const SqlHelper::SQLString& db)
	{
		int code = mysql_select_db(mysql_Instance, db.c_str());
		if (code != 0)
		{
			fprintf(stderr, "Select Db failed, Error code %ld\n", mysql_errno(mysql_Instance));
			mysql_close(mysql_Instance);
		}
	}
	bool MysqlAccessHelper::Update(const SqlHelper::SQLString& query)
	{
		if (mysql_query(mysql_Instance, query.c_str()))
		{
			fprintf(stderr, "update error %s\n", mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}
	bool MysqlAccessHelper::Delete(const SqlHelper::SQLString& record)
	{
		if (mysql_query(mysql_Instance, record.c_str()))
		{
			fprintf(stderr, "Delete error %s\n", mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}

	MYSQL_RES* MysqlAccessHelper::GetData(const SqlHelper::SQLString& str)
	{
		unsigned long long num_Rows = 0;
		if (mysql_query(mysql_Instance, str.c_str()))
		{
			fprintf(stderr, "Getting Data error %s\n", mysql_error(mysql_Instance));
			return nullptr;
		}
		MYSQL_RES* res = mysql_store_result(mysql_Instance);
		if (res == nullptr)
		{
			if (mysql_field_count(mysql_Instance) == 0)
			{
				num_Rows = mysql_affected_rows(mysql_Instance);
			}
			else
				fprintf(stderr, "Error: %s\n", mysql_error(mysql_Instance));
			mysql_free_result(res);
			return nullptr;
		}
		return res;
	}

	bool MysqlAccessHelper::CreateDb(const SqlHelper::SQLString& dbName)
	{
		assert(dbName.length() > 0);
		const SqlHelper::SQLString str = "Drop schema if exists " + dbName;
		const SqlHelper::SQLString create_str = "create schema " + dbName;
		if (mysql_query(mysql_Instance, create_str.c_str()))
		{
			fprintf(stderr, "Failed to create schema %s Error: %s\n", dbName.c_str(), mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}

	bool MysqlAccessHelper::DropDb(const SqlHelper::SQLString& dbName)
	{
		assert(dbName.length() > 0);
		const SqlHelper::SQLString str = "Drop schema if exists " + dbName;
		if (mysql_query(mysql_Instance, str.c_str()))
		{
			fprintf(stderr, "Failed to drop schema %s Error: %s\n", dbName.c_str(), mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}

	bool MysqlAccessHelper::CreateTable(const SqlHelper::SQLString& tbQuery)
	{
		assert(tbQuery.length() > 0);
		if (mysql_query(mysql_Instance, tbQuery.c_str()))
		{
			fprintf(stderr, "Failed to create table %s Error: %s\n", tbQuery.c_str(), mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}

	bool MysqlAccessHelper::DropTable(const SqlHelper::SQLString& tbName)
	{
		assert(tbName.length() > 0);
		const SqlHelper::SQLString str = "Drop table if exists " + tbName;
		if (mysql_query(mysql_Instance, str.c_str()))
		{
			fprintf(stderr, "Failed to drop table %s Error: %s\n", tbName.c_str(), mysql_error(mysql_Instance));
			return false;
		}
		return true;
	}

	MYSQL_FIELD* MysqlAccessHelper::GetFields(MYSQL_RES* result)
	{
		assert(result != nullptr);

		unsigned numFileds = mysql_num_fields(result);
		MYSQL_FIELD* fields = mysql_fetch_fields(result);

		for (unsigned i = 0; i < numFileds; i++)
		{
			printf("Field %d is %s\n", i, fields[i].name);
		}
		return fields;
	}

	void MysqlAccessHelper::FetchRow(MYSQL_RES* result)
	{
		MYSQL_ROW row;
		unsigned int num_fields;
		unsigned int i;

		num_fields = mysql_num_fields(result);
		while ((row = mysql_fetch_row(result)))
		{
			unsigned long *lengths = nullptr;
			lengths = mysql_fetch_lengths(result);
			for (i = 0; i < num_fields; i++)
			{
				printf("[%.*s] ", (int)lengths[i],
					row[i] ? row[i] : "NULL");
			}
			printf("\n");
		}
	}

	MysqlAccessHelper::~MysqlAccessHelper()
	{
		mysql_close(mysql_Instance);
		CC_SAFE_RELEASE(mysql_Instance);
		CC_SAFE_RELEASE(helperInstance);
	}
}