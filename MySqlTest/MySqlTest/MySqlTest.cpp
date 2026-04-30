// MySqlTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <mysql.h>
#include <sstream>
#include <iostream>

using namespace std;
//int qstate;


int main()
{
	MYSQL* conn;
	//MYSQL_ROW row;
	//MYSQL_RES *res;

	conn = mysql_init(0);

	conn = mysql_real_connect(conn, "local", "sa", "panacea", "test", 3306, NULL, 0);
	if (conn)
	{
		cout << "Database connected to sql" << conn << endl;

		//string insert_query = "SELECT * FROM test";

		//const char * q = insert_query.c_str();

		//qstate = mysql_query(conn, q);
		//if (!qstate)
		//{

		//}

		//else
		//{
		//	cout << "query execution problem" << mysql_errno(conn) << endl;
		//}
	}
	else
	{
		cout << "Failed to connect!--2" << mysql_error(conn) << endl;
	}
}

