// VS_Sqllite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SQLAPI.h"
#include <iostream>

using namespace std;

int main()
{

	SAConnection con;

	try
	{
		// connect to database
		// in this example, it is Oracle,
		// but can also be Sybase, Informix, DB2
		// SQLServer, InterBase, SQLBase and ODBC
		con.Connect("test",    // database name
			"sa",  // user name
			"panacea",  // password
			SA_Oracle_Client); //Oracle Client
		cout<< "We are connected!\n";

		// Disconnect is optional
		// autodisconnect will occur in destructor if needed
		con.Disconnect();
		cout << "We are disconnected!\n";
	}

	catch (SAException &amp)
	{
		// SAConnection::Rollback()
		// can also throw an exception
		// (if a network error for example),
		// we will be ready
		try
		{
			// on error rollback changes
			con.Rollback();
		}
		catch (SAException &amp)
		{
		}
		// print error message
		cout << "Errror";
	}
    return 0;
}

