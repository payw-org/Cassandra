//
//  BOP_Manager.cpp
//  DSD16_TeamProj_BOP
//
//  Created by iHwan on 2016. 11. 25..
//  Copyright © 2016년 iHwan. All rights reserved.
//

#include "BOP_manager.h"

// -----------------------------
// BOP class implementation
// -----------------------------
BOP::BOP(void)
{
   m_genreCoefficient = 0;
   m_actorCoefficient = 0;
   m_directorCoefficient = 0;
   m_ageGroupCoefficient = 0;
}

BOP::BOP(Database* database)
{
   m_genreCoefficient = 0;
   m_actorCoefficient = 0;
   m_directorCoefficient = 0;
   m_ageGroupCoefficient = 0;

   m_database = database;
}

void BOP::setCoefficient(int coef_Direc, int coef_Actor, int coef_Genre, int coef_Age)
{
   m_actorCoefficient = coef_Actor;
   m_genreCoefficient = coef_Genre;
   m_ageGroupCoefficient = coef_Age;
   m_directorCoefficient = coef_Direc;
}

void BOP::calcCoeffiecient()
{
   int       i;

   int       genreCoefficient;
   int       actorCoefficient;
   int       directorCoefficient;
   int       ageGroupCoefficient;

   //int       disRank = INT_MAX;    // difference in rank of population and total score.

   filebuf fb;
   fb.open("allResult.txt", ios::out);
   ostream os(&fb);

   for (genreCoefficient = 0; genreCoefficient <= 100; genreCoefficient++) {
      for (actorCoefficient = 0; actorCoefficient <= 100 - genreCoefficient; actorCoefficient++) {
         for (directorCoefficient = 0; directorCoefficient <= 100 - actorCoefficient - genreCoefficient; directorCoefficient++) {
            ageGroupCoefficient = 100 - actorCoefficient - directorCoefficient - genreCoefficient;

            // calculate total score.
            m_database->calcTotalScore(directorCoefficient, actorCoefficient, genreCoefficient, ageGroupCoefficient);

            // sort movie list.
            m_database->movieClassifyByScore();

            // compare movie list sorted by total score with by population.
            vector<Movie> movieList = m_database->getOldMovieList();

            int sizeOfList = (int)movieList.size();
            int tempDisRank = 0;

            for (i = 0; i < sizeOfList; i++) {
               if ((float)i / sizeOfList <= grade[0]) { tempDisRank += abs(movieList[i].getGrade() - 1); }
               else if ((float)i / sizeOfList <= grade[1]) { tempDisRank += abs(movieList[i].getGrade() - 2); }
               else if ((float)i / sizeOfList <= grade[2]) { tempDisRank += abs(movieList[i].getGrade() - 3); }
               else if ((float)i / sizeOfList <= grade[3]) { tempDisRank += abs(movieList[i].getGrade() - 4); }
               else if ((float)i / sizeOfList <= grade[4]) { tempDisRank += abs(movieList[i].getGrade() - 5); }
               else if ((float)i / sizeOfList <= grade[5]) { tempDisRank += abs(movieList[i].getGrade() - 6); }
               else if ((float)i / sizeOfList <= grade[6]) { tempDisRank += abs(movieList[i].getGrade() - 7); }
               else if ((float)i / sizeOfList <= grade[7]) { tempDisRank += abs(movieList[i].getGrade() - 8); }
               else { tempDisRank += abs(movieList[i].getGrade() - 9); }

               if (tempDisRank > disRank) { break; }
            }
            os << "director: " << directorCoefficient << "  actor:  " << actorCoefficient << "  genre:  " << genreCoefficient << "  agegroup:  " << ageGroupCoefficient;
            os << "  disRank: " << tempDisRank << endl;

            // choose smallest disRank and most similar coefficient.
            if (disRank > tempDisRank) {
               disRank = tempDisRank;
               m_genreCoefficient = genreCoefficient;
               m_actorCoefficient = actorCoefficient;
               m_directorCoefficient = directorCoefficient;
               m_ageGroupCoefficient = ageGroupCoefficient;
            }
         }
      }
   }
   fb.close();
}

void BOP::completeDB()
{
   m_database->calcTotalScore(m_directorCoefficient, m_actorCoefficient, m_genreCoefficient, m_ageGroupCoefficient);
   m_database->storeMovieToBST();

   m_database->showDB();
}

void BOP::storeCoefficientText()
{
   cout << "director: " << m_directorCoefficient << "  actor:  " << m_actorCoefficient << "  genre:  " << m_genreCoefficient << "  agegroup:  " << m_ageGroupCoefficient << endl;
   filebuf fb;
   fb.open("BestResult.txt", ios::out);
   ostream os(&fb);
   os << "  director: " << m_directorCoefficient << "  actor:  " << m_actorCoefficient << "  genre:  " << m_genreCoefficient << "  agegroup:  " << m_ageGroupCoefficient << endl;
   os << "  disRank: " << disRank << endl;
   fb.close();
   //cout << this->disRank << endl;
}

void BOP::addMovie(Movie &movie)
{
   m_database->calcTotalScore(m_directorCoefficient, m_actorCoefficient, m_genreCoefficient, m_ageGroupCoefficient, movie);
   m_database->searchNearestNode(movie);
   m_database->addMovieToNewMovieList(movie);
}


// ——————————————
// distributionModel class definition
// ——————————————
double DistributionModel::normalFunc(double x){
   return pow(M_E, -0.5 * pow((x - norExpectation) / norVariance, 2.0)) / (sqrt(2 * M_PI)*norVariance);
}

double DistributionModel::exponFunc(double x){
   if (x >= 0)
      return expLambda*pow(M_E, (-1)*expLambda*x);
   else
      return 0;
}

double DistributionModel::calcNormalProb(double lowBound, double upBound){
   lowBound -= 510;
   lowBound /= 60;

   upBound -= 510;
   upBound /= 60;

   int i;
   double ans = 0.0;
   double dx = ((upBound - lowBound) / (numIntervals + 1));

   for (int i = 1; i <= numIntervals; i++)
   {
      ans = ans + normalFunc(lowBound + i * dx);
   }

   ans = ans * dx;
   return ans;
}

double DistributionModel::calcExponentialProb(double lowBound, double upBound){

   int i;
   double ans = 0.0;
   double dx = ((upBound - lowBound) / (numIntervals + 1));
   for (int i = 1; i <= numIntervals; i++)
   {
      ans = ans + exponFunc(lowBound + i * dx);
   }

   ans = ans * dx;
   return ans;
}


// ——————————————
// TimeScheduling class definition
// ——————————————
TimeScheduling::TimeScheduling(Database* database){
   this->m_database = database;
   //this->m_scheduleMovieList = m_database->getNewMovieList();
   this->dM.setNormalModel(8.0, 16.0 / 7.0);
   this->dM.setExponentialModel(1.0 / 7.5);
}

void TimeScheduling::totalScheduling(int month, int date, int period){
   int i;
   for (i = 0; i < period; i++){
      this->m_scheduleMovieList = m_database->getNewMovieList();
      stopScreening(month, date, i);
      expectTodaySpectator(month , date, i);
      dayScheduling();
   }
}

void TimeScheduling::stopScreening(int month, int date, int peroid){
   int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   vector<Movie>::iterator i;
   int tempMonth = 0;
   int tempDate = 0;
   for (i = m_scheduleMovieList.begin(); i != m_scheduleMovieList.end(); i++){
      if (i->getStartMonth() < month){                  //개봉한지 한참 지난 영화, 스케쥴링 목록에서 삭제
         tempDate = i->getStartDate() + 30;
         tempDate %= months[i->getStartMonth() - 1];
         if (tempDate != i->getStartDate() + 30){
            tempMonth = (i->getStartMonth() + 1) % 13 + 1;
         }
         if (tempMonth < month || (tempMonth == month && tempDate < date)){
            m_scheduleMovieList.erase(i);
         }
      }
      else if (i->getStartDate() > date + peroid){         //아직 개봉되지 않은 영화, 스케쥴링 목록에서 삭제
         m_scheduleMovieList.erase(i);
      }
   }
}

void TimeScheduling::expectTodaySpectator(int month, int date, int period){
   double weekProb, dayProb, totalProb;
   int week;
   vector<Movie>::iterator i;
   long totalDays, whichDay;
   for (i = m_scheduleMovieList.begin(); i != m_scheduleMovieList.end(); i++){            //각각의 영화에 대해서 오늘 하루 예상관객수를 구함
      totalDays = calcTotalDays(2016, i->getStartMonth(), i->getStartDate()) - calcTotalDays(2016, month, date + period);
      totalDays = abs(totalDays);

      week = (totalDays / 7) + 1;
      weekProb = dM.calcExponentialProb((week - 1) * 7, week * 7);

      whichDay = calcTotalDays(2016, month, date + period);

      if (whichDay%7 == 0 || whichDay%7 == 6)
         dayProb = weekProb * WEEKEND_PERCENT;
      else
         dayProb = weekProb * WEEKDAY_PERCENT;

      totalProb = dayProb * m_share;
      i->setTodayExpected(i->getNumOfSpectator() * totalProb);
   }
}

void TimeScheduling::dayInitialize(){
   Screen screen_1(9 * 60, 24 * 60 + 30, 176);
   Screen screen_2(9 * 60 + 10, 24 * 60 + 40, 174);
   Screen screen_3(9 * 60 + 15, 24 * 60 + 45, 174);
   Screen screen_4(9 * 60 + 35, 25 * 60 + 5, 174);
   Screen screen_5(9 * 60 + 20, 24 * 60 + 50, 174);
   Screen screen_6(9 * 60 + 25, 24 * 60 + 55, 174);

   vector<Screen> dayScreen;
   dayScreen.push_back(screen_1);
   dayScreen.push_back(screen_2);
   dayScreen.push_back(screen_3);
   dayScreen.push_back(screen_4);
   dayScreen.push_back(screen_5);
   dayScreen.push_back(screen_6);

   m_timeTable.push(dayScreen);

   m_beforeSchedulingTime = 8 * 60 + 30;
}

void TimeScheduling::dayScheduling(){
   dayInitialize();
   vector<Screen>::iterator sI;
   bool deadLineFlag = false;
   while (!deadLineFlag){
      sI = this->searchNextScreen();            // search next scheduling
      maxMovieChoice(sI);                     // choice movie what have maximum weight
      deadLineFlag = true;
      for (Screen s : m_timeTable.back()){      //check if all screen is dead
         if (s.getEndTime() > s.getNextSchedulingTime()){
            deadLineFlag = false;
            break;
         }
      }
   }
}

vector<Screen>::iterator TimeScheduling::searchNextScreen(){            //다음으로 스케쥴링 해줘야하는 스크린을 찾음
   vector<Screen>::iterator i, resultI;
   int nextSch = 48 * 60;

   for (i = m_timeTable.back().begin(); i != m_timeTable.back().end(); i++){
      if (i->getNextSchedulingTime() < nextSch){
         nextSch = i->getNextSchedulingTime();
         resultI = i;
      }
   }

   return resultI;
}

void TimeScheduling::maxMovieChoice(vector<Screen>::iterator iS){         //현재 기대인구수가 가장 높을 것으로 예상되는 영화 선택
   double addProb;
   addProb = dM.calcNormalProb(m_beforeSchedulingTime, iS->getNextSchedulingTime());

   m_beforeSchedulingTime = iS->getNextSchedulingTime();                  //가장 최근에 스케줄링 해준 시간 업데이트

   vector<Movie>::iterator iM;
   for (iM = m_scheduleMovieList.begin(); iM != m_scheduleMovieList.end(); iM++){
      iM->setCurrExpected(iM->getCurrExpected() + addProb*(iM->getTodayExpected()));      //각 영화마다 가중치 업데이트(가중치 recovery)
   }

   iM = max_element(m_scheduleMovieList.begin(), m_scheduleMovieList.end(), [](const Movie& m1, const Movie& m2)
   {return m1.getCurrExpected() < m2.getCurrExpected(); });                        //max 가중치 값을 가진 영화 search

   /*상영 확정된 영화의 가중치 업데이트(가중치를 좌석수만큼 down)*/
   if (iM->getCurrExpected() - iS->getNumOfSeat() >= 0)
      iM->setCurrExpected(iM->getCurrExpected() - iS->getNumOfSeat());
   else
      iM->setCurrExpected(0);

   iS->allocateMovie(*iM);                     //현재 스크린에 상영확정 된 영화 할당
   //iS->setNextSchedulingTime(iS->getNextSchedulingTime() + iM->getRunningTime());         //관 종료시간 업뎃
}

long TimeScheduling::calcTotalDays(int year, int month, int day)            //날짜, 시간을 계산하는 함수

{
   int months[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
   int i;
   long total = 0L;

   total = (year - 1) * 365L + (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400;
   if (!(year % 4) && year % 100 || !(year % 400))
      months[1]++;
   for (i = 0; i<month - 1; i++)
      total += months[i];

   total += day;

   return total;
}


// -----------------------------
// Manager class implementation
// -----------------------------
Manager::Manager(void) { }

Manager::Manager(string strFilePath)
{
   buildDB(strFilePath);
   m_bop = new BOP(m_database);
   m_timeScheduling = new TimeScheduling(m_database);
}

void Manager::storeCoefficientText() { m_bop->storeCoefficientText(); }

void Manager::buildDB(string strFilePath) { m_database = new Database(strFilePath); }

void Manager::calcCoef() { m_bop->calcCoeffiecient(); }

void Manager::calcBOP()
{
   m_bop->setCoefficient(DIREC_COEF, ACT_COEF, GENRE_COEF, AGE_COEF);
   m_bop->completeDB();
}

void Manager::calcTimeScheduling(int month, int date, int period){
   m_timeScheduling->totalScheduling(month, date, period);
}

void Manager::addMovie(string strName, string strDirectors,
   string strActors, string strRunningTime,
   string strGenre, string strAgeGroup,
   string month, string date)
{
   string name = strName;
   string genre = strGenre;
   string ageGroup = strAgeGroup;

   int runningTime = stoi(strRunningTime);

   vector<string> vectorDirectors, vectorActors;
   m_database->tokenize(strDirectors, vectorDirectors, "/");
   m_database->tokenize(strActors, vectorActors, "/");

   Movie movie(name, runningTime, vectorDirectors, vectorActors, genre, ageGroup);
   movie.setStartMonth(stoi(month));
   movie.setStartDate(stoi(date));
   m_bop->addMovie(movie);
}



/*

#include<stdio.h>
#include<math.h>
#include"DistributionModel.h"
# define M_E  2.7182818284590452354   // e
# define PI   3.141592653589793238
static int numIntervals = 1000000;

double exponentialFunc(double x, double y)
{
return x;
}

double DistributionModel::test(double x)
{
return 1 / pow(M_E, ((pow(x, 2) / 2))* (pow(2 * PI, 0.5)));
}

double normalFunc(double x)
{
return  pow(M_E, (-0.5)*(pow((x - 8) / (16.0 / 7.0), 2))) / ((16.0 / 7.0)*pow(2 * PI, 0.5));
}
double testFunc(double x)
{
return  pow(M_E, -1 * ((pow(x, 2) / 2))) / (pow(2 * PI, 0.5));
}


double DistributionModel::calcExponentialProb(double lowBound, double upBound) {

double ans = 0.0;

return ans;
}

double DistributionModel::calcNormalProb(double lowBound, double upBound) {

double ans = 0.0;
double dx = ((upBound - lowBound) / (numIntervals + 1));
int i = 0;
for (int i = 1; i <= numIntervals; i++)
{
ans = ans + normalFunc(lowBound + i * dx);

}

ans = ans * dx;
return ans;
}

double DistributionModel::testProb(double lowBound, double upBound) {

double ans = 0.0;
double dx = ((upBound - lowBound) / (numIntervals + 1));
int i = 0;
for (int i = 1; i <= numIntervals; i++)
{
ans = ans + testFunc(lowBound + i * dx);
}
ans = ans * dx;
return ans;
}

*/
