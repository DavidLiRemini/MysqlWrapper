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
		std::vector<SqlHelper::SqlDataType>paramType;

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
		// Brief:	��ȡMysql����ʵ��
		//************************************
		static MysqlAccessHelper* GetInstance();

		//************************************
		// @Method:    GetConnection
		// @Returns:   SqlHelper::MysqlAccessHelper::ConnectStates
		// @Parameter: target ����������ͨ��ΪIP��ַ
		// @Parameter: user �û���
		// @Parameter: password ����
		// @Parameter: port �˿ں�
		// @Brief: �������ݿ�����
		//************************************
		ConnectStates GetConnection(const SqlHelper::SQLString& target, const SqlHelper::SQLString& user,
			const SqlHelper::SQLString& password, int port = 0);

		//************************************
		// @Method:    SetSchema
		// @Returns:   void
		// @Parameter: schemaName ���ݿ���
		// @Brief: ����Ĭ�����ݿ�
		//************************************
		void SetSchema(const SqlHelper::SQLString& schemaName);

		//************************************
		// @Method:    Update
		// @Returns:   bool �ɹ�����true��ʧ�ܷ���false
		// @Parameter: updateQuery
		// @Brief: ���±��¼��
		//************************************
		bool Update(const SqlHelper::SQLString& updateQuery);

		
		//************************************
		// @Method:    Delete
		// @Returns:   bool �ɹ�����True�����򷵻�false
		// @Parameter: deleteQuery
		// @Brief:	ɾ�����¼
		//************************************
		bool Delete(const SqlHelper::SQLString& deleteQuery);

		//************************************
		// @Method:    CreateDb
		// @Returns:   bool �ɹ���true��ʧ�ܷ�false
		// @Parameter: dbName ���ݿ���
		// @Brief: �������ݿ�
		//************************************
		bool CreateDb(const SqlHelper::SQLString& dbName);

	
		//************************************
		// @Method:    DropDb
		// @Returns:   bool
		// @Parameter: dbName ���ݿ���
		// @Brief: ɾ�����ݿ�
		//************************************
		bool DropDb(const SqlHelper::SQLString& dbName);

		//************************************
		// @Method:    CreateTable
		// @Returns:   bool
		// @Parameter: tbQuery �������ݱ��DMLQuery
		// @Brief:
		//************************************
		bool CreateTable(const SqlHelper::SQLString& tbQuery);

		//************************************
		// @Method:    DropTable
		// @Returns:   bool
		// @Parameter: tbName
		// @Brief: ɾ�����ݱ�
		//************************************
		bool DropTable(const SqlHelper::SQLString& tbName);

		//************************************
		// @Method:    GetData
		// @Returns:   MYSQL_RES* �����
		// @Parameter: dmlQuery
		// @Brief: ��ȡ���ݱ����ݣ� ���ؽ����
		//************************************
		MYSQL_RES* GetData(const SqlHelper::SQLString& dmlQuery);

		//************************************
		// @Method:    GetFields
		// @Returns:   MYSQL_FIELD* �����
		// @Parameter: res Ϊ�����
		// @Brief:	��ȡ������ĸ���������,
		//************************************
		MYSQL_FIELD* GetFields(MYSQL_RES* res);

		//************************************
		// @Method:    FetchRow
		// @Returns:   void
		// @Parameter: res
		// @Brief: ��ȡ������еĸ���
		//************************************
		void FetchRow(MYSQL_RES* res);

		
		//************************************
		// @Method:    Insert
		// @Returns:   bool
		// @Parameter: query DMLQuery ���
		// @Parameter: value 
		// @Parameter: rest ���ݼ�¼
		// @Brief: �����¼
		//************************************
		template <typename T, typename... Args>
		bool Insert(SqlHelper::SQLString query, const T& value, Args... rest)
		{
			ParseParameter(value, rest...);
			assert(!paramType.empty());

			std::vector<int>intContainer(5);
			std::vector<unsigned long>lenghContainer(5);
			std::vector<double>doubleContainer(5);
			std::vector<unsigned char>charContainer(5);
			std::vector<SqlHelper::SQLString>strContainer(5);
			MYSQL_STMT* stmt = nullptr;
			MYSQL_BIND* bind = new MYSQL_BIND[paramType.size()]();
			memset(bind, 0, sizeof(MYSQL_BIND)*paramType.size());
			stmt = mysql_stmt_init(mysql_Instance);
			if (!stmt)
			{
				fprintf(stderr, "mysql_stmt_init(), out of memory\n");
				return false;
			}
			if (mysql_stmt_prepare(stmt, query.c_str(), strlen(query.c_str())))
			{
				fprintf(stderr, "mysql_stmt_prepare() %s\n", mysql_stmt_error(stmt));
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
				return false;
			}

			if (mysql_stmt_execute(stmt))
			{
				fprintf(stderr, "mysql_stmt_execute() %s", mysql_stmt_error(stmt));
				return false;
			}
			mysql_stmt_close(stmt);
			paramType.clear();
			return true;
		}

		//************************************
		// @Method:    ParseParameter
		// @Returns:   void
		// @Parameter: val
		// @Parameter: rest
		// @Brief: �����������
		//************************************
		template <typename T, typename... Args>
		void ParseParameter(const T& val, Args... rest)
		{
			paramType.push_back(SqlHelper::SqlDataType(val));
			return ParseParameter(rest...);
		}

		//************************************
		// @Method:    ParseParameter
		// @Returns:   void
		// @Parameter: val
		// @Brief:	���������ķǿɱ�����汾��
		//************************************
		template <typename T>
		void ParseParameter(const T& val)
		{
			paramType.push_back(SqlHelper::SqlDataType(val));
		}
	};
}