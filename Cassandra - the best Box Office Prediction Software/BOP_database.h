// BOP_database.h
// created by cHoon on 2016.11.23
// Ver1.0


#ifndef _BOP_DATABASE_H_
#define _BOP_DATABASE_H_

#include <iostream>
#include <sstream>
#include <fstream>

#include <string>

/*database header*/
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>

#include <algorithm>

#include <assert.h>
#include "BOP_category.h"
#include "BOP_movie.h"

using namespace std;



class Database {
private:
	/* DB for each category of movie. Structure is hash map. So O(n) = 1 */
	unordered_map<string, Director> directorMap;			//key : director name  				
	unordered_map<string, Actor> actorMap;					//key : actor name
	unordered_map<string, Genre> genreMap;					//key : genre name
	unordered_map<string, AgeGroup> ageMap;					//key : ageGroup name

	/*iterator of each category*/
	unordered_map<string, Director>::iterator i_Direc;
	unordered_map<string, Actor>::iterator i_Act;
	unordered_map<string, Genre>::iterator i_Genre;
	unordered_map<string, AgeGroup>::iterator i_Age;

	/* DB for movie itself */
	vector<Movie> oldMovieList;               //old movie list (existing during 10 years)
	vector<Movie> newMovieList;               //new movie list (target of BOX OFFICE PREDICTION)
	map<float, Movie> movieMap;               //data structure(balanced BST) for old movies and key : score of movie

	float maxActorCategory;					  //maximum population of actor category among all movie						  

	/*******************************************************************************************************/
	/*******************************************************************************************************/
	/*******************************************************************************************************/

public:
	Database(string filePath);				//database constructor. parameter is file path of old movie list.	

	void buildDB(string filePath);			//categorize movie data and generate each movie object
	
	/*make easy to add each category data in DB*/
	void addDirectorToHASH(string& directorName);
	void addActorToHASH(string& actorName);
	void addGenreToHASH(string& genreName);
	void addAgeGroupToHASH(string& ageName);

	void calcPersonsScore();                //calculate each actor's and director's score
	void calcPersonTotalScore();			//calculate total score of persons(directors or actors) of a movie
	void standardizationActorScore();		//inner call. standardization actor total score of a movie 
	void calcWinterCoefficient();           //calculate winter coefficient score
	void calcAgeGroupsScore();              //calculate each age group's score

	void movieClassifyBySpec();				//classify movie by number of spectator 
	void movieClassifyByScore();			//classify movie by total score

	/*function overloading*/
	void calcTotalScore(int coefA, int coefB, int coefC, int coefD);				//calculate score of all old movie 
	void calcTotalScore(int coefA, int coefB, int coefC, int coefD, Movie& m);		//calculate score of a movie

	void storeMovieToBST();															//store all old movie to balanced BST																	

	/*for new movie*/
	void addMovieToNewMovieList(Movie& m);							//if you generate new movie, input this object to new movie list.
	void searchNearestNode(Movie& m);								//use balanced bst, search nearest movie with parameter's score
	float predictSpectator(map<float, Movie>::iterator i);			//predict number of spectator of new movie

	/*getter*/
	unordered_map<string, Actor> getActorMap() const { return actorMap; }
	unordered_map<string, Director> getDirectorMap() const { return directorMap; }
	vector<Movie> getOldMovieList()  { return this->oldMovieList; }
	vector<Movie> getNewMovieList()  { return this->newMovieList; }


	/*for comfortable*/
	void showDB();
	void tokenize(const string& str, vector<string>& tokens, const string& delimiters);			//string tokenizer
};


/*///////////////////////////////////////////////////

FildData class.
for task about file data
*////////////////////////////////////////////////////
class FileData {
private:
	ifstream filePointer;
	vector<string> readData;

public:
	void openFile(string filename);
	void closeFile();
	void fileToString();				//transform file data to string
	vector<string> getData();			//getter
};

#endif