// sqlfetch_VS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;


int main()
{

	SQLHANDLE SQLEnvHandle ;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle = NULL;
	SQLWCHAR retConString[1024];

	cout << "Entering inside data"<<endl;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
			goto Finsished;

		if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			goto Finsished;

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
			goto Finsished;

		cout << "Attempting connection to SQL Server...";
		cout << "\n";

		switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=register;UID=sa;PWD=panacea;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
		{
		case SQL_SUCCESS:
			cout << "Successfully connected to SQL Server--1-";
			cout << "\n";
			break;
		case SQL_SUCCESS_WITH_INFO:
			cout << "Successfully connected to SQL Server--2";
			cout << "\n";
			break;
		case SQL_NO_DATA_FOUND:
			goto Finsished;
		case SQL_INVALID_HANDLE:
			goto Finsished;
		case SQL_ERROR:
			goto Finsished;
		default:
			break;

		}

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
		{
			cout << "connection";
			cout << "\n";
			goto Finsished;
		}
		if (SQL_SUCCESS != SQLExecDirect(SQLStatementHandle, (SQLWCHAR*)L"SELECT * FROM student", SQL_NTS))
		{
			cout << "Attempting connection to SQL Server...123";
			cout << "\n";
			goto Finsished;
		}
		else
		{
			cout << "Entering fetch data"<<endl;
			char name[256];
			int age;
			while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
			{
				SQLGetData(SQLStatementHandle, 1, SQL_C_DEFAULT, &name, size(name), NULL);
				SQLGetData(SQLStatementHandle, 2, SQL_C_DEFAULT, &age, size(name), NULL);

				cout << "NAME :" <<" "<< name << endl;
				cout << "age: " << " " << age << endl;
			}


			SQLExecDirect(SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO student (name, age)"  "VALUES (yoga, 12);", SQL_NTS);
			cout << "Execution done";
		}


	Finsished:
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);

	getchar();

    return 0;
}

