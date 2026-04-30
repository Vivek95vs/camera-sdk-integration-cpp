#include "stdafx.h"
#include "camselect.h"
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <sql.h>

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
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DELETE FROM CameraSelectDevice WHERE CamName= 'CamA';", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DELETE FROM CameraSelectDevice WHERE CamName= 'CamB';", SQL_NTS);
	SQLExecute(SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO CameraSelectDevice ( CamName, CamSerialNum )" "values (?, ?);", SQL_NTS);
	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, name, sizeof(name), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 20, 0, Serialnum, sizeof(Serialnum), NULL);

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
	char Bank_pixelval[256];
	SQLAllocHandle(SQL_HANDLE_STMT, SQLConnectionHandle, &SQLStatementHandle);
	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"DROP TABLE Bank_pixels", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"CREATE TABLE Bank_pixels(Bank_pixels nvarchar(MAX),Bank_pixelval nvarchar(MAX))", SQL_NTS);
	SQLExecute(SQLStatementHandle);

	SQLPrepare((SQLHSTMT)SQLStatementHandle, (SQLWCHAR*)L"INSERT INTO Bank_pixels( Bank_pixels, Bank_pixelval)" "values (?, ?);", SQL_NTS);

	SQLBindParameter(SQLStatementHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bank_pixels, sizeof(Bank_pixels), NULL);
	SQLBindParameter(SQLStatementHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 2048, 0, Bank_pixelval, sizeof(Bank_pixelval), NULL);

	strcpy_s(Bank_pixels, "BankA_pixels");
	strcpy_s(Bank_pixelval, "222222");
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