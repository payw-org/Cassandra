#include "BOP_database.h"
/*////////////////////////////

class Database

*/////////////////////////////

Database::Database(string filePath){							//database constructor. parameter is file path of old movie list.
	this->buildDB(filePath);					//movie object generate

	this->calcPersonsScore();					//make score of each person(director & actor)
	this->calcPersonTotalScore();				//make person total score of a movie 
	this->calcWinterCoefficient();				//make score of each genre
	this->calcAgeGroupsScore();					//make score of each age group

	this->movieClassifyBySpec();				//classify movie by number of spectator
}

void Database::buildDB(string filePath){					//categorize movie data and generate each movie object
	/*fetch the old movie data from file*/
	vector<string> movieData;

	FileData* file = new FileData;
	file->openFile(filePath);
	file->fileToString();
	movieData = file->getData();
	file->closeFile();
	delete file;

	/*categorize each category of movie and construct movie object*/
	string name, director, actor, genre, ageGroup;
	int numOfSpec, currCategory = 0;
	vector<string> tokenList, directorList, actorList;
	vector<string>::iterator i, ism, isd, isac, isg, isag;

	for (i = movieData.begin(); i != movieData.end(); i++){					//iterate all movie data 
		tokenList.clear();
		this->tokenize(*i, tokenList, ",");
		for (ism = tokenList.begin(); ism != tokenList.end(); ism++){		//about each movie token
			switch (currCategory){
			case 0:															//movie name
				name = *ism;
				break;
			case 1:															//directors 
				directorList.clear();
				this->tokenize(*ism, directorList, "/");
				/*construct director db*/
				for (isd = directorList.begin(); isd != directorList.end(); isd++){
					if (!(this->directorMap.empty())){							//if director db is not empty,
						this->i_Direc = this->directorMap.find(*isd);			//find in db
						if (this->i_Direc == this->directorMap.end())			//if not found,
							this->addDirectorToHASH(*isd);						
					}
					else                                                        //if empty,
						this->addDirectorToHASH(*isd);
				}
				break;
			case 2:															//actors. same as case1
				actorList.clear();
				this->tokenize(*ism, actorList, "/");
				/*construct actor db*/
				for (isac = actorList.begin(); isac != actorList.end(); isac++){
					if (!(this->actorMap.empty())){								
						this->i_Act = this->actorMap.find(*isac);
						if (this->i_Act == this->actorMap.end())
							this->addActorToHASH(*isac);
					}
					else
						this->addActorToHASH(*isac);
				}
				break;
			case 3:															//genre.  same 
				genre = *ism;
				if (!(this->genreMap.empty())){
					this->i_Genre = this->genreMap.find(genre);
					if (this->i_Genre == this->genreMap.end())
						this->addGenreToHASH(genre);
				}
				else
					this->addGenreToHASH(genre);
				break;
			case 4:															//age group.  same 
				ageGroup = *ism;
				if (!(this->ageMap.empty())){
					this->i_Age = this->ageMap.find(ageGroup);
					if (this->i_Age == this->ageMap.end())
						this->addAgeGroupToHASH(ageGroup);
				}
				else
					this->addAgeGroupToHASH(ageGroup);
				break;
			case 5:															//# of spectator
				numOfSpec = stoi(*ism);
				
				/*add number of spectator of this movie to each category object*/

				for (isd = directorList.begin(); isd != directorList.end(); isd++){
					this->i_Direc = this->directorMap.find(*isd);
					this->i_Direc->second.addNum(numOfSpec);
				}

				for (isac = actorList.begin(); isac != actorList.end(); isac++){
					this->i_Act = this->actorMap.find(*isac);
					this->i_Act->second.addNum(numOfSpec);
				}

				this->i_Genre = this->genreMap.find(genre);
				this->i_Genre->second.addNum(numOfSpec);

				this->i_Age = this->ageMap.find(ageGroup);
				this->i_Age->second.addNum(numOfSpec);
			}
			currCategory++;													//next category
		}
		currCategory = 0;													//next movie

		Movie movie(name, 0, directorList, actorList, genre, ageGroup, numOfSpec);		//movie construct
		this->oldMovieList.push_back(movie);					//add to movie db
	}
}

void Database::addDirectorToHASH(string& name){
	Director temp(name);
	this->directorMap.insert(pair<string, Director>(name, temp));
}

void Database::addActorToHASH(string& name){
	Actor temp(name);
	this->actorMap.insert(pair<string, Actor>(name, temp));
}

void Database::addGenreToHASH(string& name){
	Genre temp(name);
	this->genreMap.insert(pair<string, Genre>(name, temp));
}

void Database::addAgeGroupToHASH(string& name){
	AgeGroup temp(name);
	this->ageMap.insert(pair<string, AgeGroup>(name, temp));
}

//////////////////////////////////////////////////////

void Database::calcPersonsScore(){								//calculate each actor's and director's score
	vector<Category> actors, directors;

	/*move db*/
	for (this->i_Act = this->actorMap.begin(); this->i_Act != this->actorMap.end(); (this->i_Act)++){
		actors.push_back(this->i_Act->second);
	}
	for (this->i_Direc = this->directorMap.begin(); this->i_Direc != this->directorMap.end(); (this->i_Direc)++){
		directors.push_back(this->i_Direc->second);
	}

	//sort by number of spectator
	sort(actors.begin(), actors.end(), [](const Category& a1, const Category& a2){return a1.getTotalNum() > a2.getTotalNum(); });
	sort(directors.begin(), directors.end(), [](const Category& d1, const Category& d2){return d1.getTotalNum() > d2.getTotalNum(); });

	int rank, j,  numOfPersons;
	float tempGrade;
	float grade1 = 0.04, grade2 = 0.11, grade3 = 0.23, grade4 = 0.4, grade5 = 0.6, grade6 = 0.77, grade7 = 0.89, grade8 = 0.96;
	unordered_map<string, Actor>::size_type numOfActors = this->actorMap.size();
	unordered_map<string, Director>::size_type numOfDirectors = this->directorMap.size();
	vector<Category> person;

	/*classify into grade*/
	for (j = 0; j < 2; j++){
		if (j == 0){
			person = actors;
			numOfPersons = numOfActors;
		}
		else{
			person = directors;
			numOfPersons = numOfDirectors;
		}
		rank = 1;
		for (Category c : person){
			if (rank <= numOfPersons*grade1)				//grade1
				tempGrade = 1;
			else if (numOfPersons*grade1 < rank && rank <= numOfPersons*grade2)		//grade2
				tempGrade = 2;
			else if (numOfPersons*grade2 < rank && rank <= numOfPersons*grade3)		//grade3
				tempGrade = 3;
			else if (numOfPersons*grade3 < rank && rank <= numOfPersons*grade4)		//grade4		
				tempGrade = 4;
			else if (numOfPersons*grade4 < rank && rank <= numOfPersons*grade5)		//grade5
				tempGrade = 5;
			else if (numOfPersons*grade5 < rank && rank <= numOfPersons*grade6)		//grade6
				tempGrade = 6;
			else if (numOfPersons*grade6 < rank && rank <= numOfPersons*grade7)		//grade7
				tempGrade = 7;
			else if (numOfPersons*grade7 < rank && rank <= numOfPersons*grade8)		//grade8
				tempGrade = 8;
			else if (numOfPersons*grade8 < rank)									//grade9
				tempGrade = 9;

			if (j == 0){
				this->i_Act = this->actorMap.find(c.getName());
				this->i_Act->second.calcScore(tempGrade);
			}
			else{
				this->i_Direc = this->directorMap.find(c.getName());
				this->i_Direc->second.calcScore(tempGrade);
			}
			rank++;
		}
	}
}

void Database::calcPersonTotalScore(){									//calculate total score of persons(directors or actors) of a movie
	vector<Movie>::iterator i;
	float dirScore, actScore;
	for (i = this->oldMovieList.begin(); i != this->oldMovieList.end(); i++){
		dirScore = 0, actScore = 0;

		/*director score = max score among all directors in the movie*/
		float currDirScore = 0.0;
		for (string s : i->getDirectors()){
			i_Direc = directorMap.find(s);
			if (i_Direc == directorMap.end())
				currDirScore = 10;
			else
				currDirScore = i_Direc->second.getScore();

			if (currDirScore > dirScore)
				dirScore = currDirScore;
		}
		i->setDirectorScore(dirScore);

		for (string s : i->getActors()){
			i_Act = actorMap.find(s);
			if (i_Act == actorMap.end())
				actScore += 10;
			else
				actScore += i_Act->second.getScore();
		}
		i->setActorScore(actScore);
	}
	this->standardizationActorScore();
}

void Database::standardizationActorScore(){
	vector<Movie>::iterator i;
	float maxScore = 0, currScore;
	for (i = this->oldMovieList.begin(); i != this->oldMovieList.end(); i++){
		if (i->getActorScore() > maxScore)
			maxScore = i->getActorScore();
	}

	this->maxActorCategory = maxScore;
	for (i = this->oldMovieList.begin(); i != this->oldMovieList.end(); i++){
		currScore = i->getActorScore() * (100.0 / maxScore);
		i->setActorScore(currScore);
	}
}

void Database::calcWinterCoefficient(){
	priority_queue<int> genreQueue;
	for (this->i_Genre = this->genreMap.begin(); this->i_Genre != this->genreMap.end(); (this->i_Genre)++){
		genreQueue.push(this->i_Genre->second.getTotalNum());
	}

	int maxGenre = genreQueue.top();
	int currGenre;
	for (this->i_Genre = this->genreMap.begin(); this->i_Genre != this->genreMap.end(); (this->i_Genre)++){
		currGenre = this->i_Genre->second.getTotalNum();
		this->i_Genre->second.calcScore(100.0 * (float)currGenre / (float)maxGenre);
	}
}

void Database::calcAgeGroupsScore(){
	priority_queue<int> ageQueue;
	for (this->i_Age = this->ageMap.begin(); this->i_Age != this->ageMap.end(); (this->i_Age)++){
		ageQueue.push(this->i_Age->second.getTotalNum());
	}

	int maxAge = ageQueue.top();
	int currAge;
	for (this->i_Age = this->ageMap.begin(); this->i_Age != this->ageMap.end(); (this->i_Age)++){
		currAge = this->i_Age->second.getTotalNum();
		this->i_Age->second.calcScore(100.0 * (float)currAge / (float)maxAge);
	}
}

//////////////////////////////////////////////////////

void Database::movieClassifyBySpec(){
	sort(oldMovieList.begin(), oldMovieList.end(), [](const Movie& m1, const Movie& m2){return m1.getNumOfSpectator() > m2.getNumOfSpectator(); });

	int rank = 1, tempGrade;
	int numOfMovies = this->oldMovieList.size();
	vector<Movie>::iterator i;
	float grade1 = 0.04, grade2 = 0.11, grade3 = 0.23, grade4 = 0.4, grade5 = 0.6, grade6 = 0.77, grade7 = 0.89, grade8 = 0.96;
	for (i = this->oldMovieList.begin(); i != this->oldMovieList.end(); i++){
		if (rank <= numOfMovies*grade1)
			tempGrade = 1;
		else if (numOfMovies*grade1 < rank && rank <= numOfMovies*grade2)
			tempGrade = 2;
		else if (numOfMovies*grade2 < rank && rank <= numOfMovies*grade3)
			tempGrade = 3;
		else if (numOfMovies*grade3 < rank && rank <= numOfMovies*grade4)
			tempGrade = 4;
		else if (numOfMovies*grade4 < rank && rank <= numOfMovies*grade5)
			tempGrade = 5;
		else if (numOfMovies*grade5 < rank && rank <= numOfMovies*grade6)
			tempGrade = 6;
		else if (numOfMovies*grade6 < rank && rank <= numOfMovies*grade7)
			tempGrade = 7;
		else if (numOfMovies*grade7 < rank && rank <= numOfMovies*grade8)
			tempGrade = 8;
		else if (numOfMovies*grade8 < rank)
			tempGrade = 9;

		i->setGrade(tempGrade);
		rank++;
	}
}

void Database::movieClassifyByScore(){
	sort(this->oldMovieList.begin(), this->oldMovieList.end(), [](const Movie& m1, const Movie& m2){return m1.getTotalScore() > m2.getTotalScore(); });
}

//////////////////////////////////////////////////////

void Database::calcTotalScore(int dirCoef, int actCoef, int genCoef, int ageCoef){
	vector<Movie>::iterator i;

	float tempTotalScore, dirScore, actScore, genScore, ageScore;
	for (i = this->oldMovieList.begin(); i != this->oldMovieList.end(); i++){
		tempTotalScore = 0, dirScore = 0, actScore = 0, genScore = 0, ageScore = 0;
		
		dirScore *= ((float)dirCoef / 100.0);

		actScore = i->getActorScore();
		actScore *= ((float)actCoef / 100.0);

		i_Genre = genreMap.find(i->getGenre());
		genScore = i_Genre->second.getScore() * ((float)genCoef / 100.0);
		

		i_Age = ageMap.find(i->getAgeGroup());
		ageScore = i_Age->second.getScore() * ((float)ageCoef / 100.0);

		tempTotalScore = dirScore + actScore + ageScore + genScore;
		i->setTotalScore(tempTotalScore);
	}
}

void Database::calcTotalScore(int dirCoef, int actCoef, int genCoef, int ageCoef, Movie& movie){
	float tempTotalScore, dirScore, actScore, genScore, ageScore;

	tempTotalScore = 0, dirScore = 0, actScore = 0, genScore = 0, ageScore = 0;

	float maxDirecScore = 0.0;
	for (string s : movie.getDirectors()){
		i_Direc = directorMap.find(s);
		if (i_Direc == directorMap.end())
			dirScore = 10;
		else
			dirScore = i_Direc->second.getScore();

		if (dirScore > maxDirecScore)
			maxDirecScore = dirScore;
	}
	movie.setDirectorScore(maxDirecScore);

	maxDirecScore *= ((float)dirCoef / 100.0);


	for (string s : movie.getActors()){
		i_Act = actorMap.find(s);
		if (i_Act == actorMap.end())
			actScore += 10;
		else
			actScore += i_Act->second.getScore();
	}
	actScore *= (100.0 / (this->maxActorCategory));
	movie.setActorScore(actScore);

	actScore *= (float)((float)actCoef / 100.0);

	i_Genre = genreMap.find(movie.getGenre());
	assert(i_Genre != genreMap.end());
	genScore = i_Genre->second.getScore() * ((float)genCoef / 100.0);

	i_Age = ageMap.find(movie.getAgeGroup());
	assert(i_Age != ageMap.end());
	ageScore = i_Age->second.getScore() * ((float)ageCoef / 100.0);

	tempTotalScore = maxDirecScore + actScore + genScore + ageScore;
	movie.setTotalScore(tempTotalScore);
}

//////////////////////////////////////////////////////

void Database::storeMovieToBST(){
	for (Movie m : this->oldMovieList){
		this->movieMap.insert(pair<float, Movie>(m.getTotalScore(), m));
	}
}

//////////////////////////////////////////////////////

void Database::addMovieToNewMovieList(Movie& m){
	this->newMovieList.push_back(m);
}

void Database::searchNearestNode(Movie& m){
	map<float, Movie>::iterator i, resultI;
	int range = 1;
	bool existFlag = false;
	float currentScore = m.getTotalScore(), scoreGap = 100.0;

	while (!existFlag){
		for (i = (this->movieMap).lower_bound(currentScore - range); i != (this->movieMap).upper_bound(currentScore + range); i++){
			existFlag = true;
			if (abs(i->second.getTotalScore() - currentScore) < scoreGap){
				resultI = i;
				scoreGap = abs(i->second.getTotalScore() - currentScore);
			}
		}
		range *= 2;
	}
	map<float, Movie>::iterator lastNode = this->movieMap.end();
	lastNode--;

	if (resultI == this->movieMap.begin())
		resultI++;

	else if (resultI == lastNode){
		resultI--;
	}
	m.setNumOfSpectator(predictSpectator(resultI));
}

float Database::predictSpectator(map<float, Movie>::iterator i){
	//map<float, Movie>::iterator temp;
	int totalSpectator = 0;
	i--;
	for (int n = 0; n < 3; n++){
		totalSpectator += i->second.getNumOfSpectator();
		i++;
	}

	int expectSpectator = totalSpectator / 3;

	return expectSpectator;
}

//////////////////////////////////////////////////////

void Database::showDB(){
	map<float, Movie>::iterator i;

	filebuf fb;
	fb.open("test.txt", ios::out);
	ostream os(&fb);

	for (i = this->movieMap.begin();i!=this->movieMap.end();i++){
		os <<i->second.getName()<< "  " << i->second.getTotalScore() << "  " << i->second.getNumOfSpectator() <<"Grade: " <<i->second.getGrade()<<endl;
	}


	fb.close();

}

void Database::tokenize(const string& str, vector<string>& tokens, const string& delimiters){
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos - lastPos));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}




/*//////////////////////////////////////////////////////////////////////

class FileData

*///////////////////////////////////////////////////////////////////////
void FileData::openFile(string filePath){
    this->filePointer.open(filePath, ios::in);
	if (!(this->filePointer)){
		cout << " file does not exist!" << endl;
		exit(1);
	}
}

void FileData::closeFile(){
	this->filePointer.close();
}

void FileData::fileToString(){
	bool fileEnd = false;
	string buffer;
	assert(!(this->filePointer.eof()));
	getline(this->filePointer, buffer);
	while (!fileEnd){
		getline(this->filePointer, buffer);
		if (!(this->filePointer.eof()))
			this->readData.push_back(buffer);
		else
			fileEnd = true;
	}
}

vector<string> FileData::getData(){
	return (this->readData);
}





