#pragma once
#include<iostream>
#include<string>
using namespace std;
class BSTwords
{
public:
	string word;
	BSTwords* right, *left;
	BSTwords();
	BSTwords(string word);
	BSTwords* insert(BSTwords* root, string value);
	bool wordfound(BSTwords* root, string word);
	void deletetree(BSTwords* node);
	void display(BSTwords* root);
	
};

