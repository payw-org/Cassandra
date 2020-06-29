// BOP_actor.h
// created by cHoon on 2016.11.23
// Ver1.0

#ifndef _BOP_CATEGORY_H_
#define _BOP_CATEGORY_H_

#include <iostream>
#include <string>

using namespace std;

/*//////////////////////////////////////////////////////////////////////

Category class (parent class of Actor , Director , Genre , AgeGroup)

Actor class & Director class & Genre class & AgeGroup class are category of movie database.
*///////////////////////////////////////////////////////////////////////
class Category{
protected:
	string name;
	int totalNum = 0;				//total population of this category be included 
	float score;						//score by category

public:
	/*constructor*/
	Category();
	Category(string& aName) : name(aName){};
	
	/*getter*/
	string getName() const{ return this->name; }
	int getTotalNum() const{ return this->totalNum; }
	float getScore() const{ return this->score; }
	
	void addNum(int numOfSpec){ this->totalNum += numOfSpec; }			//add population of this category be included
	void calcScore(float grade){ this->score = (10 - grade) * 10; }		
};

/*actor class*/
class Actor : public Category{
private:

public:
	Actor();
	Actor(string& aName) : Category(aName){};
};

/*director class*/
class Director : public Category{
private:

public:
	Director();
	Director(string& aName) : Category(aName){};
};

/*genre class*/
class Genre : public Category{
private:

public:
	Genre();
	Genre(string& aName) : Category(aName){};
	
	void calcScore(float grade){ this->score = grade; }		//overriding
};

/*age group class*/
class AgeGroup : public Category{
private:

public:
	AgeGroup();
	AgeGroup(string& aName) : Category(aName){};

	void calcScore(float grade){ this->score = grade; }		//overriding
};

#endif
