// MysqLConnection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cppconn\driver.h>
#include <cppconn\exception.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>

using namespace std;
int main()
{
	cout << endl;
	cout << "Entering function" << endl;

		try {
		sql::Driver *driver;
		sql::Connection *con;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://192.168.10.11", "sa", "panacea");
		/* Connect to the MySQL test database */
		con->setSchema("test");
		cout << "Connected to database" << endl;
		}
		catch (sql::SQLException &ex) {
			cout << "Error connecting to DB: " << ex.what() << endl;
		}
		catch (...) {
			cout << "Something has gone wrong" << endl;
		}

	cout << endl;

}

