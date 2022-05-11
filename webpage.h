#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"BSTwords.h"
using namespace std;
class webpage
{
private:
	std::string name;
	int clicks;
	double CTR;
	double impressions;
	double pagerank;
	double score;
	BSTwords* keywords;
public:
	int originalindex;
	webpage();
	void setname(string);
	string getname();
	void setCTR(double);
	double getCTR();
	void setimpressions(double);
	double getimpressions();
	void setPR(double);
	double getPR();
	void setwords(BSTwords* root);
	BSTwords* getwords();
	double calculatescore();
	void updaterank(double);
	void updateCTR();
	void updateimpressions();
	double getrank();
};

