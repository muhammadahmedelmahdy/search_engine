#include "webpage.h"
#include<string>
#include<vector>
using namespace std;
webpage::webpage()
{
	pagerank = 0.0384615;
	clicks = 1;
}
void webpage::setname(string name)
{
	this->name = name;
}
string webpage::getname()
{
	return name;
}
void webpage::setCTR(double ctr)
{
	CTR = ctr;
}
double webpage::getCTR()
{
	return CTR;
}
void webpage::setimpressions(double imp)
{
	impressions = imp;
}
double webpage::getimpressions()
{
	return impressions;
}
void webpage::setPR(double rank)
{
	pagerank = rank;
}
double webpage::getPR()
{
	return pagerank;
}
void webpage::setwords(BSTwords* root)
{
	keywords = root;
}
BSTwords* webpage::getwords()
{
	return keywords;
}
void webpage::updateCTR()
{
	clicks++;
	CTR=double(clicks)/impressions;
}
void webpage::updateimpressions()
{
	impressions++;
}
void webpage::updaterank(double rank)
{
	pagerank = rank;
}
double webpage::getrank()
{
	return pagerank;
}
double webpage::calculatescore()
{
	score = 0.4 * pagerank + ((1 - ((0.1 * impressions) / (1 + 0.1 * impressions)) * pagerank) + ((0.1 * impressions) / (1 + 0.1 * impressions)) * CTR) * 0.6;
	return score;
}