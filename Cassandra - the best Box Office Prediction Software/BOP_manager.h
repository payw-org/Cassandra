//
//  BOP_Manager.h
//  DSD16_TeamProj_BOP
//
//  Created by iHwan on 2016. 11. 25..
//  Copyright © 2016년 iHwan. All rights reserved.
//

#ifndef BOP_Manager_hpp
#define BOP_Manager_hpp

#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <iostream>

#include "BOP_database.h"

#define DIREC_COEF 30
#define ACT_COEF 69
#define GENRE_COEF 0
#define AGE_COEF 1

static const float grade[8] = { 0.04f, 0.11f, 0.23f, 0.4f, 0.6f, 0.77f, 0.89f, 0.96f };

#define WEEKDAY_PERCENT 0.06032
#define WEEKEND_PERCENT 0.3492

// -----------------------------
// BOP class definition
// -----------------------------
class BOP {
private:
	int       m_genreCoefficient;
	int       m_actorCoefficient;
	int       m_directorCoefficient;
	int       m_ageGroupCoefficient;

	Database* m_database;

	int       disRank = INT_MAX;
public:
	BOP(void);
	BOP(Database* database);
	//~BOP(void);

public:
	// find the most similar coefficient with reality.
	void calcCoeffiecient();

	// store the coefficient.
	void setCoefficient(int coef_Direc, int coef_Actor, int coef_Genre, int coef_Age);

	// store new movie in newMovieList.
	void addMovie(Movie &movie);

	// store movies in Balanced Search Tree.
	void completeDB();

	// store coefficient in text file.
	void storeCoefficientText();
};



// ——————————————
// distributionModel class definition
// ——————————————
class DistributionModel{
private:
	double     expLambda;
	double     norExpectation;
	double     norVariance;
	int         numIntervals = 1000000;

public:
	void setExponentialModel(double lambda){ expLambda = lambda; }
	void setNormalModel(double expectation, double variance){ norExpectation = expectation, norVariance = variance; }

	double exponFunc(double x);
	double normalFunc(double x);

	double calcExponentialProb(double lowBound, double upBound);
	double calcNormalProb(double lowBound, double upBound);
};


// ——————————————
// Screen class definition
// ——————————————
class Screen{
private:
	int          beginTime;						//오픈시간
	int          endTime;						//마감시간
	int          nextSchedulingTime;			
	int          numOfSeat;						//좌석 수

	queue<Movie> movieList;

public:
	Screen(void);
	Screen(int bTime, int eTime, int numOfS) : beginTime(bTime), nextSchedulingTime(bTime), endTime(eTime), numOfSeat(numOfS){};

	void setNextSchedulingTime(int t){ nextSchedulingTime = t; }
	void allocateMovie(Movie& m){					//영화 하나 할당(스케쥴링)
		nextSchedulingTime += m.getRunningTime();
		movieList.push(m);
	}

	int getBeginTime() const{ return beginTime; }
	int getEndTime() const{ return endTime; }
	int getNumOfSeat() const { return numOfSeat; }
	int getNextSchedulingTime() const { return nextSchedulingTime; }
	queue<Movie> getMovieList() const { return movieList; }
};

// ——————————————
// TimeScheduling class definition
// ——————————————
class TimeScheduling {
private:
	queue<vector<Screen>> m_timeTable;				//total time table

	vector<Movie> m_scheduleMovieList;				
	Database* m_database;
	

	DistributionModel dM;
	int m_beforeSchedulingTime;
	double m_share = 1.0 / 356.0;					// 1 / 전국 극장관 수

public:
	TimeScheduling(void);
	TimeScheduling(Database* database);

	void totalScheduling(int month, int date, int period);					 
	void stopScreening(int month, int date,int period);						 //아직 개봉되지 않거나, 개봉한지 한참된 영화를 스케쥴링 목록에서 지움
	void expectTodaySpectator(int month, int date,int period);               //영화 하나당 날짜별 기대 인원수를 구하여 movie객체에 넣어줌

	void dayInitialize();								 //다음날 시작
	void dayScheduling();								 //하루 스케쥴링
	vector<Screen>::iterator searchNextScreen();         //다음 스크린을 찾음
	void maxMovieChoice(vector<Screen>::iterator i);         //스케줄링 해줄 영화를 선택

	queue<vector<Screen>> getTimeTable(){ return m_timeTable; }

	long calcTotalDays(int year, int month, int day);		//날짜를 계산하는데 필요한 함수
	
};


// ——————————————
// Manager class definition
// ——————————————
class Manager {
private:
	BOP*         m_bop;        // Box Office Prediction
	TimeScheduling* m_timeScheduling;         // Time Scheduling

	Database*    m_database;

public:
	Manager(void);
	Manager(string strFilePath);
	//~Manager(void);

public:
	// store coefficient at text file.
	void storeCoefficientText();

	// build database.
	void buildDB(string strFilePath);

	// find the most similar coefficient with reality.
	void calcCoef();

	// store the calcCoeffiecient and complete database.
	void calcBOP();

	void calcTimeScheduling(int month, int date,int period);
	queue<vector<Screen>> getTimeTable(){ return m_timeScheduling->getTimeTable(); }

	// store new movie from GUI.
	void addMovie(string strName, string strDirectors, string strActors, string strRunningTime, string strGenre, string strAgeGroup, string month, string date);


	//map<float, Movie> getMovieMap() { return m_database->getMovieMap(); }
	vector<Movie> getNewMovieList() const { return m_database->getNewMovieList(); }

	unordered_map<string, Actor> getActorMap() const { return m_database->getActorMap(); }
	unordered_map<string, Director> getDirectorMap() const { return m_database->getDirectorMap(); }
};


#endif /* BOP_Manager_hpp */