//
// Adrian Aguada
// U. of Illinois, Chicago
// CS 251: Fall 2019
//
// Project #02: inputs movies and reviews, allowing the user to search
// by movie ID, movie Name, or review ID.  Uses binary search trees for 
// fast search; no other data structures are allowed.
//

#include <iostream>
#include <fstream>
#include <string>
#include "bst.h"

using namespace std;

struct MovieData {
  int pubYear = 0;
  int num5stars = 0;
  int num4stars = 0;
  int num3stars = 0;
  int num2stars = 0;
  int num1stars = 0;
  int id = 0;
  string movieName = " ";
};

//
// trim
//
// trims whitespace from beginning and end of string.
//
// Ref: https://stackoverflow.com/questions/25829143/trim-whitespace-from-a-string
// Modified: to handle empty strings, and strings of all blanks.
//
string trim(const string& str)
{
  size_t first = str.find_first_not_of(" \t\n\r");
  size_t last = str.find_last_not_of(" \t\n\r");

  if (first == string::npos && last == string::npos)  // empty, or all blanks:
    return "";
  else if (first == string::npos)  // no blanks at the start:
    first = 0;                     // 0-based
  else if (last == string::npos)   // no blanks trailing at end:
    last = str.length() - 1;       // 0-based

  return str.substr(first, (last - first + 1));
}


//
// InputMovies
//
// Inputs the data from the "movies" file, which contains N>0 lines, where
// each line contains:
//     id pubYear name
//
void InputMovies(string moviesFilename, binarysearchtree<int, MovieData>& idTree, binarysearchtree<string, MovieData>& nameTree)
{
  ifstream moviesFile(moviesFilename);
  int      id, pubYear;
  string   name;
  MovieData mData;

  if (!moviesFile.good())
  {
    cout << "**Error: unable to open movies file '" << moviesFilename << "', exiting" << endl;
    return;
  }

  moviesFile >> id;  // get first ID, or set EOF flag if file empty:

  while (!moviesFile.eof())
  {
    // we have more data, so input publication year and movie name:
    moviesFile >> pubYear; //x
    getline(moviesFile, name);  // movie name fills rest of input line:

    // trim potential whitespace from both ends of movie name:
    name = trim(name); //x

    // debugging:
    //cout << id << "," << pubYear << "," << name << endl;
	
	/* set instances of mData to insert into tree */
	mData.id = id;
	mData.pubYear = pubYear;
	mData.movieName = name;
	
	/* insert into both trees */
	idTree.insert(id, mData);
	nameTree.insert(name, mData);

    moviesFile >> id;  // get next ID, or set EOF flag if no more data:
  }
}

void InputReviews(string reviewsFilename, binarysearchtree<int, MovieData>& idTree, binarysearchtree<string, MovieData>& nameTree, int &counter)
{
  ifstream reviewsFile(reviewsFilename);
  int reviewID, movieID, rating;

  if (!reviewsFile.good())
  {
    cout << "**Error: unable to open movies file '" << reviewsFilename << "', exiting" << endl;
    return;
  }
	
  reviewsFile >> reviewID; // get first review ID, or set EOF flag if file empty:
	
  while (!reviewsFile.eof())
  {
	/* store movie ID and rating of current line */
	reviewsFile >> movieID;
	reviewsFile >> rating;
	
	/* pointer to idTree's node with correct rating  */
	MovieData *mData = idTree.search(movieID);
	string movieName = mData->movieName;
	
	/* pointer to nameTree's node with correct rating */
	MovieData *mData2 = nameTree.search(movieName);
	
	/* increment rating counter based on rating that was read in */
	if(rating == 5) {
		(mData->num5stars)++;
		(mData2->num5stars)++;
	}
	else if(rating == 4) {
		(mData->num4stars)++;
		(mData2->num4stars)++;
	}
	else if(rating == 3) {
		(mData->num3stars)++;
		(mData2->num3stars)++;
	}
	else if(rating == 2) {
		(mData->num2stars)++;
		(mData2->num2stars)++;
	}  
	else if(rating == 1) {
		(mData->num1stars)++;
		(mData2->num1stars)++;
	}
	
	counter++;
	  
	/* grab next id */  
	reviewsFile >> reviewID;
  }
}

/* iterates the input, returning true if a letter is found */
bool loopString(string input) {
	for(unsigned int i = 0; i < input.length(); ++i) {
	  /* input is a string (letter found) */
	  if(!isdigit(input[i])) {
		  return true;
	  }
	}/* input is an id (all numbers) */
	return false;
}

/* computes average rating of reviews */
double avgRating(MovieData *mData) {
  double total, fivestars, fourstars, threestars, twostars, onestar;
	
  total = mData->num5stars + mData->num4stars + mData->num3stars + mData->num2stars + mData->num1stars;
  if(total == 0) {
	  return 0;
  }
	
  fivestars = 5 * mData->num5stars;
  fourstars = 4 * mData->num4stars;
  threestars = 3 * mData->num3stars;
  twostars = 2 * mData->num2stars;
  onestar = 1 * mData->num1stars;
	
  int totalNumStars = fivestars + fourstars + threestars + twostars + onestar;
  double avg = totalNumStars / total;
	
  return avg;
}

/* determines if input was an id or name search, then displays movie details */
void movieLookUp(binarysearchtree<int, MovieData> &idTree, binarysearchtree <string, MovieData> &nameTree) {
  string input;
  while(input != "#") {
	  cout << "Enter a movie id or name (or # to quit)> ";
	  getline(cin, input); // read entire input line
	  MovieData *mData;

	  bool isString = loopString(input);
	  if(!isString) { //if input is an id number
		int idLookup = stoi(input);
		mData = idTree.search(idLookup);
	  }
	  else { //else input is a string
		mData = nameTree.search(input);
	  }
	  
	  if(mData != nullptr) {
		  cout << "Movie ID: " << mData->id << endl;
		  cout << "Movie Name: " << mData->movieName << endl;
		  cout << "Avg rating: " << avgRating(mData) << endl;
		  cout << "5 stars: " << mData->num5stars << endl;
		  cout << "4 stars: " << mData->num4stars << endl;
		  cout << "3 stars: " << mData->num3stars << endl;
		  cout << "2 stars: " << mData->num2stars << endl;
		  cout << "1 star: " << mData->num1stars << endl;
		  cout << endl;
	  }
	  else if(input == "#") {
		  return;
	  }
	  else {
		  cout << "not found..." << endl;
		  cout << endl;
	  }
  }
}

//
// main
//
int main()
{
  binarysearchtree<int, MovieData> *idTree = new binarysearchtree<int, MovieData>();
  binarysearchtree<string, MovieData> *nameTree = new binarysearchtree<string, MovieData>();
	
  string moviesFilename; // = "movies1.txt";
  string reviewsFilename; // = "reviews1.txt";

  cout << "movies file?> ";
  cin >> moviesFilename;

  cout << "reviews file?> ";
  cin >> reviewsFilename;

  string junk;
  getline(cin, junk);  // discard EOL following last input:
	
  int counter = 0;	
  InputMovies(moviesFilename, *idTree, *nameTree);
  InputReviews(reviewsFilename, *idTree, *nameTree, counter);
	
  /* print out size of either tree (trees are same size) */
  cout << "Num movies: ";
  cout << idTree->size() << endl;
	
  /* print out number of reviews */	
  cout << "Num reviews: ";
  cout << counter << endl;

  /* print out size and height of both trees */
  cout << endl << "Tree by movie id: size=" << idTree->size() << ", height=" << idTree->height() << endl;
  cout << "Tree by movie name: size=" << nameTree->size() << ", height=" << nameTree->height() << endl << endl;

  /* searches for movie */
  movieLookUp(*idTree, *nameTree);

  // done:
  return 0;
}
