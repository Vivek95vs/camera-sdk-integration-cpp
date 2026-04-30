#include "stdafx.h"
#include "camselect.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <sql.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iomanip>

using namespace std;

#define CORSERVER_MAX_STRLEN		30

SQLHANDLE SQLEnvHandle;
SQLHANDLE SQLConnectionHandle = NULL;
SQLHANDLE SQLStatementHandle = NULL;
SQLWCHAR retConString[1024];

camselect::camselect()
{
}


camselect::~camselect()
{
}


void camselect::connectdatabase()
{

	cout << "Entering inside data" << endl;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLEnvHandle))
		disconnect();

	if (SQL_SUCCESS != SQLSetEnvAttr(SQLEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		disconnect();

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, SQLEnvHandle, &SQLConnectionHandle))
		disconnect();

	cout << "Attempting connection to SQL Server...";
	cout << "\n";

	switch (SQLDriverConnect(SQLConnectionHandle, NULL, (SQLWCHAR*)L"DRIVER={SQL Server};SERVER=localhost, 1433;DATABASE=SecondaryMLC;UID=sa;PWD=panacea;", SQL_NTS, retConString, 1024, NULL, SQL_DRIVER_NOPROMPT))
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
		disconnect();
	case SQL_INVALID_HANDLE:
		disconnect();
	case SQL_ERROR:
		disconnect();
	default:
		break;

	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle))
	{
		cout << "connection";
		cout << "\n";
		disconnect();
	}


}

void camselect::disconnect()
{
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}

void camselect::insertcamdetails()
{
	char name[256];
	char Serialnum[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	//SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DELETE FROM CameraSelectDevice WHERE CamName= 'CamA';", SQL_NTS);
	//SQLExecute(SQLStatementHandle);
	//SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DELETE FROM CameraSelectDevice WHERE CamName= 'CamB';", SQL_NTS);
	//SQLExecute(SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE CameraSelectDevice", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE CameraSelectDevice(CamName nvarchar(MAX),CamSerialNum nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO CameraSelectDevice ( CamName, CamSerialNum )" "values (?, ?);", SQL_NTS);
	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, Serialnum, sizeof(Serialnum), NULL);

	strcpy_s(name, "CamA");
	strcpy_s(Serialnum, "55555");
	SQLExecute(SQLStatementHandle);

	strcpy_s(name, "CamB");
	strcpy_s(Serialnum, "222222");
	SQLExecute(SQLStatementHandle);

	cout << "Executed Successfully" << endl;

	disconnect();
}
void camselect::BankB_mm_per_pixels()
{
	int i;
	int n = 10;
	char name[256];
	char Serialnum[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE BankB_mm_per_pixels", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE BankB_mm_per_pixels(BankB_mm_per_pixels nvarchar(MAX),pixelsB nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO BankB_mm_per_pixels ( BankB_mm_per_pixels, pixelsB)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Serialnum, sizeof(Serialnum), NULL);
	

	for (i = 1; i <= n; i++)
	{
		string s1 = "BankB_mm_per_pixels" + to_string(i);
		cout << "value change" << " " << s1 << endl;
		strcpy_s(name, s1.c_str());
		cout << "name" << " " << name << endl;
		//SQLExecute(SQLStatementHandle);
		strcpy_s(Serialnum, "222222");
		SQLExecute(SQLStatementHandle);
	}

	cout << "BankB_mm_per_pixels Table Creation";

	disconnect();

}
void camselect::BankA_mm_per_pixels()
{
	int i;
	int n = 10;
	char name[256];
	char Serialnum[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE BankA_mm_per_pixels", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE BankA_mm_per_pixels(BankA_mm_per_pixels nvarchar(MAX),pixelsA nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO BankA_mm_per_pixels ( BankA_mm_per_pixels, pixelsA)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Serialnum, sizeof(Serialnum), NULL);


	for (i = 1; i <= n; i++)
	{
		string s1 = "BankA_mm_per_pixels" + to_string(i);
		cout << "value change" << " " << s1 << endl;
		strcpy_s(name, s1.c_str());
		cout << "name" << " " << name << endl;
		//SQLExecute(SQLStatementHandle);
		strcpy_s(Serialnum, "222222");
		SQLExecute(SQLStatementHandle);
	}

	cout << "BankA_mm_per_pixels Table Creation";

	disconnect();

}

void camselect::BankA_points()
{
	int i;
	int n = 10;
	char name[256];
	char Serialnum[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE BankA_points", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE BankA_points(BankA_pixels nvarchar(MAX),BankA_pixelsVal nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO BankA_points ( BankA_pixels, BankA_pixelsVal)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Serialnum, sizeof(Serialnum), NULL);


	for (i = 1; i <= n; i++)
	{
		string s1 = "BankA_points" + to_string(i);
		cout << "value change" << " " << s1 << endl;
		strcpy_s(name, s1.c_str());
		cout << "name" << " " << name << endl;
		//SQLExecute(SQLStatementHandle);
		strcpy_s(Serialnum, "222222");
		SQLExecute(SQLStatementHandle);
	}

	cout << "BankA_points Table Creation";

	disconnect();

}

void camselect::BankB_points()
{
	int i;
	int n = 10;
	char name[256];
	char Serialnum[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE BankB_points", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE BankB_points(BankB_pixels nvarchar(MAX),BankB_pixelsVal nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO BankB_points ( BankB_pixels, BankB_pixelsVal)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Serialnum, sizeof(Serialnum), NULL);


	for (i = 1; i <= n; i++)
	{
		string s1 = "BankB_points" + to_string(i);
		cout << "value change" << " " << s1 << endl;
		strcpy_s(name, s1.c_str());
		cout << "name" << " " << name << endl;
		//SQLExecute(SQLStatementHandle);
		strcpy_s(Serialnum, "222222");
		SQLExecute(SQLStatementHandle);
	}

	cout << "BankB_points Table Creation";

	disconnect();

}

void camselect::lambdacreate()
{

	char paraname[256];
	char lambdaval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE lambda", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE lambda(parameter nvarchar(MAX),lambdaval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO lambda ( parameter, lambdaval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, paraname, sizeof(paraname), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, lambdaval, sizeof(lambdaval), NULL);

	strcpy_s(paraname, "lambda1");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "sz1");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "lambda2");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "sz2");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);


	strcpy_s(paraname, "contourMin");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "contourMax");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "contourMin2");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(paraname, "contourMax2");
	strcpy_s(lambdaval, "222222");
	SQLExecute(SQLStatementHandle);


	cout << "Lambda Table Creation";

	disconnect();
}

void camselect::cornerpoints()
{
	char cornerpoints[256];
	char cornerval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE corner_points", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE corner_points(cornerpoints nvarchar(MAX),cornerval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO corner_points ( cornerpoints, cornerval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cornerpoints, sizeof(cornerpoints), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cornerval, sizeof(cornerval), NULL);

	strcpy_s(cornerpoints, "A_top_left");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "A_top_right");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "A_bot_right");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "A_bot_left");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "B_top_left");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "B_top_right");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "B_bot_right");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cornerpoints, "B_bot_left");
	strcpy_s(cornerval, "222222");
	SQLExecute(SQLStatementHandle);

	cout << "cornerpoints Table Creation";

	disconnect();

}

void camselect::rect()
{
	char rect[256];
	char rectval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE rect", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE rect(rect nvarchar(MAX),rectval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO rect ( rect, rectval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, rect, sizeof(rect), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, rectval, sizeof(rectval), NULL);

	strcpy_s(rect, "rects1");
	strcpy_s(rectval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(rect, "rects2");
	strcpy_s(rectval, "222222");
	SQLExecute(SQLStatementHandle);

	cout << "rectvalue Table Creation";

	disconnect();

}

void camselect::Bank_min_max()
{
	char Bank[256];
	char Bankval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE Bank_min_max", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE Bank_min_max(Bank nvarchar(MAX),Bankval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO Bank_min_max ( Bank, Bankval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bank, sizeof(Bank), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bankval, sizeof(Bankval), NULL);

	strcpy_s(Bank, "BankA_min");
	strcpy_s(Bankval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Bank, "BankB_min");
	strcpy_s(Bankval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Bank, "BankA_max");
	strcpy_s(Bankval, "222222");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Bank, "BankB_max");
	strcpy_s(Bankval, "222222");
	SQLExecute(SQLStatementHandle);

	cout << "Bank_max_min Table Creation";

	disconnect();
}

void camselect::Bank_pixels()
{
	char Bank_pixels[256];
	char Bank_pixelval[1000];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE Bank_pixels", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE Bank_pixels(Bank_pixels nvarchar(MAX),Bank_pixelval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO Bank_pixels(Bank_pixels, Bank_pixelval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bank_pixels, sizeof(Bank_pixels), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bank_pixelval, sizeof(Bank_pixelval), NULL);

	
	int rect_cnt = 30;
	std::vector<float> vec1(rect_cnt);
	vec1.clear();
	vec1.push_back(1.40223608e+03);
	vec1.push_back(1.40359485e+03);
	vec1.push_back(1.40540759e+03);
	vec1.push_back(1.40413513e+03);
	vec1.push_back(1.40921497e+03);
	vec1.push_back(1.41132800e+03);
	vec1.push_back(1.41610217e+03);
	vec1.push_back(1.41546045e+03);
	vec1.push_back(1.41695154e+03);
	vec1.push_back(1.42349707e+03);
	vec1.push_back(1.42156653e+03);
	vec1.push_back(1.42061304e+03);
	vec1.push_back(1.42004309e+03);
	vec1.push_back(1.41904736e+03);
	vec1.push_back(1.42046472e+03);
	vec1.push_back(1.41971472e+03);
	vec1.push_back(1.42360376e+03);
	vec1.push_back(1.42255286e+03);
	vec1.push_back(1.42145300e+03);
	vec1.push_back(1.42606799e+03);
	vec1.push_back(1.41966162e+03);
	vec1.push_back(1.42480969e+03);
	vec1.push_back(1.42248059e+03);
	vec1.push_back(1.41959863e+03);
	vec1.push_back(1.42208752e+03);
	vec1.push_back(1.41941296e+03);
	vec1.push_back(1.41447778e+03);
	vec1.push_back(1.41857141e+03);
	vec1.push_back(1.41213940e+03);
	vec1.push_back(1.40740625e+03);

	setprecision(4);

	for (float i : vec1)
		std::cout << "vector value"<< "  "  << i << ' '<< endl;

	//std::cout << vec1 << std::endl;
	std::ostringstream vts;

	if (!vec1.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec1.begin(), vec1.end() - 1,
			std::ostream_iterator<float>(vts, ", "));

		// Now add the last element with no delimiter
		vts << vec1.back();
	}

	std::cout << vts.str() << std::endl;
	string s1 = vts.str();
	strcpy_s(Bank_pixels, "BankA_pixels");
	strcpy_s(Bank_pixelval, s1.c_str());
	SQLExecute(SQLStatementHandle);

	strcpy_s(Bank_pixels, "BankB_pixels");
	strcpy_s(Bank_pixelval, "222222");
	SQLExecute(SQLStatementHandle);


	cout << "Bank_pixels Table Creation";

	disconnect();
}

void camselect::cam_mlc_props()
{
	char cam_props[256];
	char cam_propval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE cam_mlc_props", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE cam_mlc_props(cam_props nvarchar(MAX),cam_propval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO cam_mlc_props( cam_props, cam_propval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cam_props, sizeof(cam_props), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cam_propval, sizeof(cam_propval), NULL);

	strcpy_s(cam_props, "rect_cnt");
	strcpy_s(cam_propval, "30");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "img_wd_req");
	strcpy_s(cam_propval, "3000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "img_ht_req");
	strcpy_s(cam_propval, "3000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "fps_req");
	strcpy_s(cam_propval, "30");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "img_wd");
	strcpy_s(cam_propval, "3000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "img_ht");
	strcpy_s(cam_propval, "3000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "thickLeafHt");
	strcpy_s(cam_propval, "5");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "thinLeafHt");
	strcpy_s(cam_propval, "50");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "leafHtError");
	strcpy_s(cam_propval, "50");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "thinLeaf_count");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "thinleaf_starting_cppindex");
	strcpy_s(cam_propval, "40");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "morph_width");
	strcpy_s(cam_propval, "3");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "morph_height");
	strcpy_s(cam_propval, "3");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "queCount_max_calib");
	strcpy_s(cam_propval, "10");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "queCount_max_running");
	strcpy_s(cam_propval, "5");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "BankA_touchShift");
	strcpy_s(cam_propval, "20");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "BankB_touchShift");
	strcpy_s(cam_propval, "40");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "oneLeafArea");
	strcpy_s(cam_propval, "70");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "contourMin");
	strcpy_s(cam_propval, "200");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "contourMax");
	strcpy_s(cam_propval, "10000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "flipParam");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "rotateParamA");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "rotateParamB");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "centerTol");
	strcpy_s(cam_propval, "20");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "display_field");
	strcpy_s(cam_propval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "close_wd");
	strcpy_s(cam_propval, "5");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "close_ht");
	strcpy_s(cam_propval, "5");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "x_margin");
	strcpy_s(cam_propval, "100");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "y_margin");
	strcpy_s(cam_propval, "50");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "cornerCentroid_tol");
	strcpy_s(cam_propval, "30");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "flipX");
	strcpy_s(cam_propval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "flipY");
	strcpy_s(cam_propval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "exposure_factor");
	strcpy_s(cam_propval, "1000");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "OverTravel");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_props, "Persp_correction");
	strcpy_s(cam_propval, "0");
	SQLExecute(SQLStatementHandle);

	cout << "camera properties Table Creation";

	disconnect();
}

void camselect::cam_features()
{
	char cam_feature[256];
	char cam_featureval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE camera_features", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE camera_features(cam_feature nvarchar(MAX),cam_featureval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO camera_features( cam_feature, cam_featureval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cam_feature, sizeof(cam_feature), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, cam_featureval, sizeof(cam_featureval), NULL);

	strcpy_s(cam_feature, "Gain1");
	strcpy_s(cam_featureval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "ExposureTime1");
	strcpy_s(cam_featureval, "16");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "BlackLevel1");
	strcpy_s(cam_featureval, "47");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "autoBrightnessMode1");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "Brightness1");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "ROI_Update1");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "flipX1");
	strcpy_s(cam_featureval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "flipY1");
	strcpy_s(cam_featureval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "Gain2");
	strcpy_s(cam_featureval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "ExposureTime2");
	strcpy_s(cam_featureval, "13");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "BlackLevel2");
	strcpy_s(cam_featureval, "50");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "autoBrightnessMode2");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "Brightness2");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "ROI_Update2");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "flipX2");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(cam_feature, "flipY2");
	strcpy_s(cam_featureval, "0");
	SQLExecute(SQLStatementHandle);

	cout << "camera feature Table Creation";

	disconnect();


}

void camselect::image_props_Trackbars()
{
	char Image_props[256];
	char Image_propval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE image_props_Trackbars", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE image_props_Trackbars(Image_props nvarchar(MAX),Image_propval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO image_props_Trackbars( Image_props, Image_propval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Image_props, sizeof(Image_props), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Image_propval, sizeof(Image_propval), NULL);

	strcpy_s(Image_props, "icenterH1");
	strcpy_s(Image_propval, "141");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iRangeH1");
	strcpy_s(Image_propval, "22");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iLowS1");
	strcpy_s(Image_propval, "66");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iHighS1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iLowV1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iHighV1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "dilate1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "erode1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "open1");
	strcpy_s(Image_propval, "3");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "close1");
	strcpy_s(Image_propval, "7");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "restore_track1");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "icenterH2");
	strcpy_s(Image_propval, "132");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iRangeH2");
	strcpy_s(Image_propval, "27");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iLowS2");
	strcpy_s(Image_propval, "78");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iHighS2");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iLowV2");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "iHighV2");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "dilate2");
	strcpy_s(Image_propval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "erode2");
	strcpy_s(Image_propval, "1");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "open2");
	strcpy_s(Image_propval, "2");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "close2");
	strcpy_s(Image_propval, "4");
	SQLExecute(SQLStatementHandle);

	strcpy_s(Image_props, "restore_track2");
	strcpy_s(Image_propval, "0");
	SQLExecute(SQLStatementHandle);



	cout << "image_props_Trackbars Table Creation";

	disconnect();


}

void camselect::fetchpixels()
{

	char Bank_pixels[256];
	char Bank_pixelval[1000];
	std::vector<string> vec2;
	vec2.clear();

	string val;
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"SELECT * FROM Bank_pixels", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_CHAR, &Bank_pixels, size(Bank_pixels), NULL);
		SQLGetData(SQLStatementHandle, 2, SQL_C_CHAR, &Bank_pixelval, size(Bank_pixelval), NULL);

		//string s(Bank_pixelval);
		cout << "BankPixels :" << " " << Bank_pixels << endl;
		cout << "BankPixelval: " << " " << Bank_pixelval << endl;
		vec2.push_back(Bank_pixelval);


	}

	//for (string i : vec2)
	//	std::cout << "vector value" << "  " << i << ' ' << endl;

	val = vec2[0];

	vector<string> BankpixA;
	stringstream s_stream(val); //create string stream from the string
	while (s_stream.good()) {
		string substr;
		getline(s_stream, substr, ','); //get first string delimited by comma
		BankpixA.push_back(substr);
	}
	for (int i = 0; i<BankpixA.size(); i++) {    //print all splitted strings
		cout << BankpixA.at(i) <<endl;
	}

	//std::cout << "String" << "  " << val << ' ' << endl;

	cout << "Pixel data fetched";
	disconnect();

}

void camselect::fetchcamera()
{

	string A1 = " ";
	string A2 = " ";
	char camName[256];
	char CamSerialNumber[256];
	std::vector<string> vecCamName;
	vecCamName.clear();

	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"SELECT CamName FROM CameraSelectDevice", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	while (SQLFetch(SQLStatementHandle) == SQL_SUCCESS)
	{
		SQLGetData(SQLStatementHandle, 1, SQL_C_CHAR, &camName, size(camName), NULL);

		vecCamName.push_back(camName);

	}

	A1 = vecCamName[0];
	A2 = vecCamName[1];

	cout << "cam value A" << " " << A1<<endl;
	cout << "cam value B" << " " << A2<< endl;


	disconnect();
}

void camselect::camserialnumberwrite()
{

	char Camname[256];
	char CamSerialnum[256];
	char acqServerSerialNumber1[30] = "1352641131346";
	char acqServerSerialNumber2[30] = "1356636631346";

	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE CameraSelectDevice", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE CameraSelectDevice(CamName nvarchar(MAX),CamSerialNum nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO CameraSelectDevice ( CamName, CamSerialNum )" "values (?, ?);", SQL_NTS);
	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, Camname, sizeof(Camname), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, CamSerialnum, sizeof(CamSerialnum), NULL);

	strcpy_s(Camname, "camB");
	strcpy_s(CamSerialnum, acqServerSerialNumber2);
	SQLExecute(SQLStatementHandle);

	strcpy_s(Camname, "camA");
	strcpy_s(CamSerialnum, acqServerSerialNumber1);
	SQLExecute(SQLStatementHandle);


	disconnect();

}

void camselect::distance_moved_mm()
{
	int i;
	int n = 5;
	char distance_moved_mm[256];
	char distance_moved_mm_B[256];
	char Bank_A_min_mm[256];
	char Bank_B_min_mm[256];

	float val = 14.1536;
	float val1 = 12.36;
	float val2 = 15.636;
	float val3 = 10.55455;
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE distance_moved_mm", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE distance_moved_mm(distance_moved_mm nvarchar(MAX),distance_moved_mm_B nvarchar(MAX),bank_moved nvarchar(MAX),bank_moved_val nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO distance_moved_mm (distance_moved_mm ,distance_moved_mm_B , bank_moved , bank_moved_val)" "values (?, ?, ?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, distance_moved_mm, sizeof(distance_moved_mm), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, distance_moved_mm_B, sizeof(distance_moved_mm_B), NULL);
	SQLBindParameter(SQLStatementHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, Bank_A_min_mm, sizeof(Bank_A_min_mm), NULL);
	SQLBindParameter(SQLStatementHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, Bank_B_min_mm, sizeof(Bank_B_min_mm), NULL);


	for (i = 1; i <= n; i++)
	{

	strcpy_s(distance_moved_mm, std::to_string(val).c_str());

	strcpy_s(distance_moved_mm_B, std::to_string(val1).c_str());

	strcpy_s(Bank_A_min_mm, std::to_string(val2).c_str());

	strcpy_s(Bank_B_min_mm, std::to_string(val3).c_str());

	cout << "value" << " " << distance_moved_mm << " " << distance_moved_mm_B << " " << Bank_A_min_mm << " " << Bank_B_min_mm << endl;

	SQLExecute(SQLStatementHandle);

	}

	cout << "distance_moved_mm Table Creation";


	disconnect();

}

void camselect::bankpoints()
{
	int i;
	int n = 5;
	char bankA_points[256];
	char bankB_points[256];
	char BankA_pixel_mm[256];
	char BankB_pixel_mm[256];

	float val = 14.1536;
	float val1 = 12.36;
	float val2 = 15.636;
	float val3 = 10.55455;
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE Bank_points", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE Bank_points(BankA_points nvarchar(MAX),BankB_points nvarchar(MAX),BankA_mm_per_pixels nvarchar(MAX),BankB_mm_per_pixels nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO Bank_points (BankA_points ,BankB_points , BankA_mm_per_pixels , BankB_mm_per_pixels)" "values (?, ?, ?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, bankA_points, sizeof(bankA_points), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, bankB_points, sizeof(bankB_points), NULL);
	SQLBindParameter(SQLStatementHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, BankA_pixel_mm, sizeof(BankA_pixel_mm), NULL);
	SQLBindParameter(SQLStatementHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, BankB_pixel_mm, sizeof(BankB_pixel_mm), NULL);


	for (i = 1; i <= n; i++)
	{

		strcpy_s(bankA_points, std::to_string(val).c_str());

		strcpy_s(bankB_points, std::to_string(val1).c_str());

		strcpy_s(BankA_pixel_mm, std::to_string(val2).c_str());

		strcpy_s(BankB_pixel_mm, std::to_string(val3).c_str());

		cout << "value" << " " << bankA_points << " " << bankB_points << " " << BankA_pixel_mm << " " << BankB_pixel_mm << endl;

		SQLExecute(SQLStatementHandle);

	}

	cout << "Bankpoints Table Creation";


	disconnect();
}

void camselect::bankpointsvec()
{
	int i;
	int n = 5;
	char bankA_points[1000];
	char bankB_points[1000];
	char BankA_pixel_mm[1000];
	char BankB_pixel_mm[1000];

	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE Bank_points", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE Bank_points(BankA_points nvarchar(MAX),BankB_points nvarchar(MAX),BankA_mm_per_pixels nvarchar(MAX),BankB_mm_per_pixels nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO Bank_points (BankA_points ,BankB_points , BankA_mm_per_pixels , BankB_mm_per_pixels)" "values (?, ?, ?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, bankA_points, sizeof(bankA_points), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, bankB_points, sizeof(bankB_points), NULL);
	SQLBindParameter(SQLStatementHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, BankA_pixel_mm, sizeof(BankA_pixel_mm), NULL);
	SQLBindParameter(SQLStatementHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, BankB_pixel_mm, sizeof(BankB_pixel_mm), NULL);

	int rect_cnt = 30;
	std::vector<float> vec1(rect_cnt);
	vec1.clear();
	std::vector<float> vec2(rect_cnt);
	vec2.clear();
	std::vector<float> vec3(rect_cnt);
	vec3.clear();
	std::vector<float> vec4(rect_cnt);
	vec4.clear();
	vec1.push_back(1.40223608e+03);
	vec1.push_back(1.40359485e+03);
	vec1.push_back(1.40540759e+03);
	vec1.push_back(1.40413513e+03);
	vec1.push_back(1.40921497e+03);
	vec1.push_back(1.41132800e+03);
	vec1.push_back(1.41610217e+03);
	vec1.push_back(1.41546045e+03);
	vec1.push_back(1.41695154e+03);
	vec1.push_back(1.42349707e+03);
	vec1.push_back(1.42156653e+03);
	vec1.push_back(1.42061304e+03);
	vec1.push_back(1.42004309e+03);
	vec1.push_back(1.41904736e+03);
	vec1.push_back(1.42046472e+03);
	vec1.push_back(1.41971472e+03);
	vec1.push_back(1.42360376e+03);
	vec1.push_back(1.42255286e+03);
	vec1.push_back(1.42145300e+03);
	vec1.push_back(1.42606799e+03);
	vec1.push_back(1.41966162e+03);
	vec1.push_back(1.42480969e+03);
	vec1.push_back(1.42248059e+03);
	vec1.push_back(1.41959863e+03);
	vec1.push_back(1.42208752e+03);
	vec1.push_back(1.41941296e+03);
	vec1.push_back(1.41447778e+03);
	vec1.push_back(1.41857141e+03);
	vec1.push_back(1.41213940e+03);
	vec1.push_back(1.40740625e+03);


	vec2.push_back(1.40223608e+03);
	vec2.push_back(1.40359485e+03);
	vec2.push_back(1.40540759e+03);
	vec2.push_back(1.40413513e+03);
	vec2.push_back(1.40921497e+03);
	vec2.push_back(1.41132800e+03);
	vec2.push_back(1.41610217e+03);
	vec2.push_back(1.41546045e+03);
	vec2.push_back(1.41695154e+03);
	vec2.push_back(1.42349707e+03);
	vec2.push_back(1.42156653e+03);
	vec2.push_back(1.42061304e+03);
	vec2.push_back(1.42004309e+03);
	vec2.push_back(1.41904736e+03);
	vec2.push_back(1.42046472e+03);
	vec2.push_back(1.41971472e+03);
	vec2.push_back(1.42360376e+03);
	vec2.push_back(1.42255286e+03);
	vec2.push_back(1.42145300e+03);
	vec2.push_back(1.42606799e+03);
	vec2.push_back(1.41966162e+03);
	vec2.push_back(1.42480969e+03);
	vec2.push_back(1.42248059e+03);
	vec2.push_back(1.41959863e+03);
	vec2.push_back(1.42208752e+03);
	vec2.push_back(1.41941296e+03);
	vec2.push_back(1.41447778e+03);
	vec2.push_back(1.41857141e+03);
	vec2.push_back(1.41213940e+03);
	vec2.push_back(1.40740625e+03);

	vec3.push_back(1.40223608e+03);
	vec3.push_back(1.40359485e+03);
	vec3.push_back(1.40540759e+03);
	vec3.push_back(1.40413513e+03);
	vec3.push_back(1.40921497e+03);
	vec3.push_back(1.41132800e+03);
	vec3.push_back(1.41610217e+03);
	vec3.push_back(1.41546045e+03);
	vec3.push_back(1.41695154e+03);
	vec3.push_back(1.42349707e+03);
	vec3.push_back(1.42156653e+03);
	vec3.push_back(1.42061304e+03);
	vec3.push_back(1.42004309e+03);
	vec3.push_back(1.41904736e+03);
	vec3.push_back(1.42046472e+03);
	vec3.push_back(1.41971472e+03);
	vec3.push_back(1.42360376e+03);
	vec3.push_back(1.42255286e+03);
	vec3.push_back(1.42145300e+03);
	vec3.push_back(1.42606799e+03);
	vec3.push_back(1.41966162e+03);
	vec3.push_back(1.42480969e+03);
	vec3.push_back(1.42248059e+03);
	vec3.push_back(1.41959863e+03);
	vec3.push_back(1.42208752e+03);
	vec3.push_back(1.41941296e+03);
	vec3.push_back(1.41447778e+03);
	vec3.push_back(1.41857141e+03);
	vec3.push_back(1.41213940e+03);
	vec3.push_back(1.40740625e+03);


	vec4.push_back(1.40223608e+03);
	vec4.push_back(1.40359485e+03);
	vec4.push_back(1.40540759e+03);
	vec4.push_back(1.40413513e+03);
	vec4.push_back(1.40921497e+03);
	vec4.push_back(1.41132800e+03);
	vec4.push_back(1.41610217e+03);
	vec4.push_back(1.41546045e+03);
	vec4.push_back(1.41695154e+03);
	vec4.push_back(1.42349707e+03);
	vec4.push_back(1.42156653e+03);
	vec4.push_back(1.42061304e+03);
	vec4.push_back(1.42004309e+03);
	vec4.push_back(1.41904736e+03);
	vec4.push_back(1.42046472e+03);
	vec4.push_back(1.41971472e+03);
	vec4.push_back(1.42360376e+03);
	vec4.push_back(1.42255286e+03);
	vec4.push_back(1.42145300e+03);
	vec4.push_back(1.42606799e+03);
	vec4.push_back(1.41966162e+03);
	vec4.push_back(1.42480969e+03);
	vec4.push_back(1.42248059e+03);
	vec4.push_back(1.41959863e+03);
	vec4.push_back(1.42208752e+03);
	vec4.push_back(1.41941296e+03);
	vec4.push_back(1.41447778e+03);
	vec4.push_back(1.41857141e+03);
	vec4.push_back(1.41213940e+03);
	vec4.push_back(1.40740625e+03);

	setprecision(4);


	for (float i : vec1)
		std::cout << "vector value" << "  " << i << ' ' << endl;

	//std::cout << vec1 << std::endl;
	std::ostringstream vts1;
	std::ostringstream vts2;
	std::ostringstream vts3;
	std::ostringstream vts4;

	if (!vec1.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec1.begin(), vec1.end() - 1,
			std::ostream_iterator<float>(vts1, ", "));

		// Now add the last element with no delimiter
		vts1 << vec1.back();
	}

	if (!vec2.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec2.begin(), vec2.end() - 1,
			std::ostream_iterator<float>(vts2, ", "));

		// Now add the last element with no delimiter
		vts2 << vec2.back();
	}

	if (!vec3.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec3.begin(), vec3.end() - 1,
			std::ostream_iterator<float>(vts3, ", "));

		// Now add the last element with no delimiter
		vts3 << vec1.back();
	}

	if (!vec4.empty())
	{
		// Convert all but the last element to avoid a trailing ","
		std::copy(vec4.begin(), vec4.end() - 1,
			std::ostream_iterator<float>(vts4, ", "));

		// Now add the last element with no delimiter
		vts4 << vec4.back();
	}


	string s1 = vts1.str();
	string s2 = vts2.str();
	string s3 = vts3.str();
	string s4 = vts4.str();


	cout << " bankA points" << " " << s1 << endl;
	cout << " bankB points" << " " << s2 << endl;
	cout << " bankA_pix" << " " << s3 << endl;
	cout << " bankB_pix" << " " << s4 << endl;

	for (i = 1; i <= n; i++)
	{

		strcpy_s(bankA_points, s1.c_str());

		strcpy_s(bankB_points, s2.c_str());

		strcpy_s(BankA_pixel_mm, s3.c_str());

		strcpy_s(BankB_pixel_mm, s4.c_str());

		//cout << "value" << " " << bankA_points << " " << bankB_points << " " << BankA_pixel_mm << " " << BankB_pixel_mm << endl;

		SQLExecute(SQLStatementHandle);

	}


	cout << "Bank_pixels Table Creation";


	disconnect();
}