// BOP_movie.h
// created by cHoon on 2016.11.23
// Ver1.0


#ifndef _BOP_MOVIE_H_
#define _BOP_MOVIE_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Movie{
private:
	/*movie metadata*/
	string name;
	vector<string> directors;		//director list of this movie
	vector<string> actors;			//actor list of this movie
	string genre;
	string ageGroup;
	int runningTime;

	/*score*/
	float directorScore;			//score of director category of a movie (standardization value. range: 0 ~ 100)
	float actorScore;				//score of actor category of a movie (standardization value. range: 0 ~ 100)
	float totalScore;				//score of a movie 

	int numOfSpectator;				//if this movie is old movie, it is real value and else if new movie, it is expected value
	int grade;						//range : 1 ~ 9 
	
	// For time Scheduling
	int todayExpectedSpectator;						// today total weigths
	int currExpectedSpectator;						// current weights

	int sMonth;
	int sDate;
public:
	Movie();		//default constructor

	/*constructor for new movie*/
	Movie(string mName, int mRunningTime, 
		vector<string> mDirectors, vector<string> mActors, 
		string mGenre, string mAgeGroup) :
		name(mName), runningTime(mRunningTime),
		directors(mDirectors), actors(mActors),
		genre(mGenre), ageGroup(mAgeGroup){}	

	/*constructor for old movie*/
	Movie(string mName, int mRunningTime, 
		vector<string> mDirectors, vector<string> mActors, 
		string mGenre, string mAgeGroup, int mNumOfSpectator) : 
		name(mName), runningTime(mRunningTime), 
		directors(mDirectors), actors(mActors), 
		genre(mGenre), ageGroup(mAgeGroup), numOfSpectator(mNumOfSpectator){}	
	//~Movie();		//destructor


	/*getter*/
	string getName() const { return name; }
	vector<string> getDirectors() const { return directors; }
	vector<string> getActors() const { return actors; }
	string getGenre() const { return genre; }
	string getAgeGroup() const { return ageGroup; }
	int getRunningTime() const { return runningTime; }

	float getDirectorScore() const{ return directorScore; }
	float getActorScore() const{ return actorScore; }
	float getTotalScore() const { return totalScore; }

	int getNumOfSpectator() const { return numOfSpectator; }
	int getGrade() const{ return grade; }

	int getTodayExpected() const { return todayExpectedSpectator; }
	int getCurrExpected() const { return currExpectedSpectator; }
	
	int getStartMonth() const { return sMonth; }
	int getStartDate() const { return sDate; }

	/*setter*/
	void setDirectorScore(float score){ directorScore = score; }
	void setActorScore(float score){ actorScore = score; }
	void setTotalScore(float score){ totalScore = score; }
	
	void setNumOfSpectator(int num){ numOfSpectator = num; }
	void setGrade(int mGrade){ grade = mGrade; }

	void setTodayExpected(int num){ todayExpectedSpectator = num; }
	void setCurrExpected(int num){ currExpectedSpectator = num; }
	
	void setStartMonth(int month) { sMonth = month; }
	void setStartDate(int date) { sDate = date; }
};
#endif