#pragma once

#include "SqlString.h"
#include "SqlDataType.h"
#include <mysql.h>
#include <iostream>
#include <vector>
#include <assert.h>

#define CC_SAFE_RELEASE(p)\
	do \
	{\
		if(p)\
			delete(p);\
	} while (0);

#define DROP_SAMPLE_TABLE "drop table if exists test_table"
#define CREATE_SAMPLE_TABLE "Create table test_table\
	col1 int,\
	col2 varchar(40),\
	col3 varchar(30),\
	col4 varchar(30)"

namespace SqlHelper
{
	class LIBSQLSTRING_API MysqlAccessHelper
	{
	private:
		
		
		MYSQL* mysql_Instance;
		

		static MysqlAccessHelper* helperInstance;
		void Initialize();
	protected:
		
		MysqlAccessHelper();
	public:
		~MysqlAccessHelper();
		enum class ConnectStates
		{
			CONNECTED, CONNECTFAILED
		};

		//************************************
		// Method:    GetInstance
		// Returns:   MysqlAccessHelper*
		// Brief:	获取Mysql连接实例
		//************************************
		static MysqlAccessHelper* GetInstance();

		//************************************
		// @Method:    GetConnection
		// @Returns:   SqlHelper::MysqlAccessHelper::ConnectStates
		// @Parameter: target 连接主机，通常为IP地址
		// @Parameter: user 用户名
		// @Parameter: password 密码
		// @Parameter: port 端口号
		// @Brief: 建立数据库连接
		//************************************
		ConnectStates GetConnection(const SqlHelper::SQLString& target, const SqlHelper::SQLString& user,
			const SqlHelper::SQLString& password, int port = 0);

		//************************************
		// @Method:    SetSchema
		// @Returns:   void
		// @Parameter: schemaName 数据库名
		// @Brief: 设置默认数据库
		//************************************
		void SetSchema(const SqlHelper::SQLString& schemaName);

		//************************************
		// @Method:    Update
		// @Returns:   bool 成功返回true，失败返回false
		// @Parameter: updateQuery
		// @Brief: 更新表记录，
		//************************************
		bool Update(const SqlHelper::SQLString& updateQuery);

		
		//************************************
		// @Method:    Delete
		// @Returns:   bool 成功返回True，否则返回false
		// @Parameter: deleteQuery
		// @Brief:	删除表记录
		//************************************
		bool Delete(const SqlHelper::SQLString& deleteQuery);

		//************************************
		// @Method:    CreateDb
		// @Returns:   bool 成功返true，失败返false
		// @Parameter: dbName 数据库名
		// @Brief: 创建数据库
		//************************************
		bool CreateDb(const SqlHelper::SQLString& dbName);

	
		//************************************
		// @Method:    DropDb
		// @Returns:   bool
		// @Parameter: dbName 数据库名
		// @Brief: 删除数据库
		//************************************
		bool DropDb(const SqlHelper::SQLString& dbName);

		//************************************
		// @Method:    CreateTable
		// @Returns:   bool
		// @Parameter: tbQuery 创建数据表的DMLQuery
		// @Brief:
		//************************************
		bool CreateTable(const SqlHelper::SQLString& tbQuery);

		//************************************
		// @Method:    DropTable
		// @Returns:   bool
		// @Parameter: tbName
		// @Brief: 删除数据表
		//************************************
		bool DropTable(const SqlHelper::SQLString& tbName);

		//************************************
		// @Method:    GetData
		// @Returns:   MYSQL_RES* 结果集
		// @Parameter: dmlQuery
		// @Brief: 获取数据表数据， 返回结果集 
		// 普通的非insert update delete等查询语句皆归于此如select 语句。
		//************************************
		MYSQL_RES* GetData(const SqlHelper::SQLString& dmlQuery);

		//************************************
		// @Method:    GetFields
		// @Returns:   MYSQL_FIELD* 结果集
		// @Parameter: res 为结果集
		// @Brief:	获取结果集的各列属性名,
		//************************************
		MYSQL_FIELD* GetFields(MYSQL_RES* res);

		//************************************
		// @Method:    FetchRow
		// @Returns:   void
		// @Parameter: res
		// @Brief: 获取结果集中的各行
		//************************************
		void FetchRow(MYSQL_RES* res);

		
		//************************************
		// @Method:    Insert
		// @Returns:   bool
		// @Parameter: query DMLQuery 语句
		// @Parameter: value 
		// @Parameter: rest 数据记录
		// @Brief: 插入记录
		//************************************
		template <typename T, typename... Args>
		bool Insert(const SqlHelper::SQLString& query, const T& value, Args... rest)
		{
			std::vector<SqlHelper::SqlDataType>paramType;
			ParseParameter(paramType, value, rest...);
			assert(!paramType.empty());
			int paraCount= sizeof...(Args) + 1;
			std::vector<int>intContainer(paraCount);
			std::vector<unsigned long>lenghContainer(paraCount);
			std::vector<double>doubleContainer(paraCount);
			std::vector<unsigned char>charContainer(paraCount);
			std::vector<SqlHelper::SQLString>strContainer(paraCount);
			MYSQL_STMT* stmt = nullptr;
			MYSQL_BIND* bind = new MYSQL_BIND[paramType.size()]();
			memset(bind, 0, sizeof(MYSQL_BIND)*paramType.size());
			stmt = mysql_stmt_init(mysql_Instance);
			if (!stmt)
			{
				fprintf(stderr, "mysql_stmt_init(), out of memory\n");
				delete []bind;
				return false;
			}
			if (mysql_stmt_prepare(stmt, query.c_str(), strlen(query.c_str())))
			{
				fprintf(stderr, "mysql_stmt_prepare() %s\n", mysql_stmt_error(stmt));
				mysql_stmt_close(stmt);
				delete []bind;
				return false;
			}
			int index = 0;
			int icIndex = 0;
			int ulIndex = 0;
			int dCIndex = 0;
			int cCIndex = 0;
			int strIndex = 0;

			for (auto it : paramType)
			{
				SqlHelper::SqlDataType::Type tp = it.GetType();
				switch (tp)
				{
				case SqlHelper::SqlDataType::Type::SQL_UNKNOWN_TYPE:
					break;
				case SqlHelper::SqlDataType::Type::SQL_BOOLEAN:
				case SqlHelper::SqlDataType::Type::SQL_BYTE:
					charContainer[cCIndex] = paramType.at(index).AsBool();
					bind[index].buffer_type = MYSQL_TYPE_TINY;
					bind[index].buffer = (char*)&charContainer[cCIndex++];
					bind[index].is_null = 0;
					bind[index].length = 0;
					index++;
					break;

				case SqlHelper::SqlDataType::Type::SQL_DOUBLE:
					doubleContainer[dCIndex] = paramType.at(index).AsDouble();
					bind[index].buffer_type = MYSQL_TYPE_DOUBLE;
					bind[index].buffer = (char*)&doubleContainer[dCIndex++];
					bind[index].is_null = 0;
					bind[index].length = 0;
					index++;
					break;

				case SqlHelper::SqlDataType::Type::SQL_SMALLINTEGER:
				case SqlHelper::SqlDataType::Type::SQL_INTERGER:
				case SqlHelper::SqlDataType::Type::SQL_BIGINTEGER:
					intContainer[icIndex] = paramType.at(index).AsInt();
					bind[index].buffer_type = MYSQL_TYPE_LONG;
					bind[index].buffer = (char*)&intContainer[icIndex++];
					bind[index].is_null = 0;
					bind[index].length = 0;
					index++;
					break;
				case SqlHelper::SqlDataType::Type::SQL_STRING:
				case SqlHelper::SqlDataType::Type::SQL_DATE:
				case SqlHelper::SqlDataType::Type::SQL_DATETIME:
					strContainer[strIndex] = paramType.at(index).AsSqlString();
					lenghContainer[ulIndex] = strlen(strContainer.at(strIndex).c_str());
					bind[index].buffer_type = MYSQL_TYPE_VARCHAR;
					bind[index].buffer = (char*)strContainer[strIndex++].c_str();
					bind[index].is_null = 0;
					bind[index].length = &lenghContainer[ulIndex++];
					index++;
					break;
				default:
					break;
				}
			}
			int code = mysql_stmt_bind_param(stmt, bind);
			if (code != 0)
			{
				fprintf(stderr, "%s\n", mysql_stmt_error(stmt));
				mysql_stmt_close(stmt);
				delete []bind;
				return false;
			}

			if (mysql_stmt_execute(stmt))
			{
				fprintf(stderr, "mysql_stmt_execute() %s", mysql_stmt_error(stmt));
				mysql_stmt_close(stmt);
				delete []bind;
				return false;
			}
			mysql_stmt_close(stmt);
			
			return true;
		}

		//************************************
		// @Method:    ParseParameter
		// @Returns:   void
		// @Parameter: val
		// @Parameter: rest
		// @Brief: 解析传入参数
		//************************************
		template <typename T, typename... Args>
		void ParseParameter(std::vector<SqlHelper::SqlDataType>& paramType,const T& val, Args... rest)
		{
			paramType.push_back(SqlHelper::SqlDataType(val));
			return ParseParameter(paramType, rest...);
		}

		//************************************
		// @Method:    ParseParameter
		// @Returns:   void
		// @Parameter: val
		// @Brief:	解析参数的非可变参数版本。
		//************************************
		template <typename T>
		void ParseParameter(std::vector<SqlHelper::SqlDataType>&paramType,const T& val)
		{
			paramType.push_back(SqlHelper::SqlDataType(val));
		}
	};
}
