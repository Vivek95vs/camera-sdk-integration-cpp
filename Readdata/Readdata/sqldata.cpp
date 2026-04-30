
#include "sqldata.h"
#include "iostream"
#include "sqlite\sqlite3.h"
#include "SQLAPI.h"


using namespace std;
sqldata::sqldata()
{
	calldata();
}


sqldata::~sqldata()
{

}

void sqldata::calldata()
{
	//sqlite3 *db;
	//char *zErrMsg = 0;
	//int rc;
	//rc = sqlite3_open("MLCShaper.db", &db);
	//if (rc) {
	//	cout<< "Can't open database: %s\n"<< sqlite3_errmsg(db);
	//}
	//else {
	//	cout << "Opened database successfully";
	//}
	//sqlite3_close(db);

	SAConnection con;
	con.Connect(_TSA("MLCShaper.db"), _TSA("sa"), _TSA("panacea"), SA_SQLServer_Client);
	cout << "connected";
		
}
