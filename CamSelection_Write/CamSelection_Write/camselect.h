#pragma once
class camselect
{
public:
	camselect();
	~camselect();


	void connectdatabase();
	void disconnect();
	void insertcamdetails();
	void BankB_mm_per_pixels();
	void BankA_mm_per_pixels();
	void lambdacreate();
	void cornerpoints();
	void rect();
	void Bank_min_max();
	void Bank_pixels();
	void cam_mlc_props();
	void cam_features();
};

