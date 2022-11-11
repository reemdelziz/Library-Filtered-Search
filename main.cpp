#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ArgumentManager.h"

using namespace std;

struct Book {
  string genre;
  string title;
  string author;
  string year;
  bool isValid = false;
};

struct Filter {
  string category;
  string value;
};

string removeSpaces(string line) {
  string filtered;
  for (int i = 0; i < line.length(); i++) {
    if (line[i] == ' ') continue;
    filtered += line[i];
  }
  return filtered;
};

void filterList(vector<Book>& library, vector<Filter>& filters) {
  bool filterCategories[4] = {false, false, false, false};
  bool bookCategories[4] = {false, false, false, false};

  //Record which category has a filter in case of multiple filters in each
  for (int i = 0; i < filters.size(); i++) {
    if (filters[i].category == "genre") filterCategories[0] = true;
    if (filters[i].category == "title") filterCategories[1] = true;
    if (filters[i].category == "author") filterCategories[2] = true;
    if (filters[i].category == "year") filterCategories[3] = true;
  }
  
  for (int i = 0; i < library.size(); i++) {
    for (int j = 0; j < filters.size(); j++) {
      if (library[i].genre == filters[j].value || library[i].title == filters[j].value || library[i].author == filters[j].value || library[i].year == filters[j].value) {
        if (filters[j].category == "genre") bookCategories[0] = true;
        else if (filters[j].category == "title") bookCategories[1] = true;
        else if (filters[j].category == "author") bookCategories[2] = true;
        else if (filters[j].category == "year") bookCategories[3] = true;
      }
    }
    if (filterCategories[0] == bookCategories[0] && filterCategories[1] == bookCategories[1] && filterCategories[2] == bookCategories[2] && filterCategories[3] == bookCategories[3]) library[i].isValid = true;

    for(int i = 0; i < 4; i++) {
      bookCategories[i] = false;
    }
  }
  
}

int main(int argc, char* argv[]) {
  
  ArgumentManager am(argc, argv);
  string infilename = am.get("input");
  string commandfilename = am.get("command");
  string outfilename = am.get("output");

  ofstream output(outfilename);

  ifstream command(commandfilename);
  ifstream input(infilename);
  
  string line;
  Book currBook;
  vector<Book> bookList;
  vector<Filter> filters;

  while (!input.eof()) {
    getline(input, line);
    //if line contains no information, get next one
    if (line == "") continue;
    //call function to remove all spaces from the line
    else line = removeSpaces(line);
    //check for spelling and ordering mistakes and recieve all book details
    if (line.substr(1, line.find(':') - 1) == "genre") {
      line = line.substr(line.find(':')+1);
      currBook.genre = line.substr(0, line.find(','));
      line = line.substr(line.find(',') + 1);
    } else continue;
    if (line.substr(0, line.find(':')) == "title") {
      line = line.substr(line.find(':')+1);
      currBook.title = line.substr(0, line.find(','));
      line = line.substr(line.find(',') + 1);
    } else continue;
    if (line.substr(0, line.find(':')) == "author") {
      line = line.substr(line.find(':')+1);
      currBook.author = line.substr(0, line.find(','));
      line = line.substr(line.find(',') + 1);
    } else continue;
    if (line.substr(0, line.find(':')) == "year") {
      line = line.substr(line.find(':')+1);
      currBook.year = line.substr(0, line.find('}'));
    } else continue;
    //populate vector with book
    bookList.push_back(currBook);
  }
  
  //recieve restrictions from command file
  Filter currFil;
  while (!command.eof()) {
    getline(command, line);
    //if line is empty, get next
    if (line == "") continue;
    //Fill temporary filter category with everything before ':' and fill value with everything after
    currFil.category = line.substr(0, line.find(':'));
    line = line.substr(line.find(':')+1);
    currFil.value = line;

    //fill vector with the temporary filter
    filters.push_back(currFil);
  }

  //if there are no filters, output all books then end program, else filter bookList
  if (filters.size() == 0) {
    for (int i = 0; i < bookList.size(); i++) {
      output << "{genre:" << bookList[i].genre << ",title:" << bookList[i].title << ",author:" << bookList[i].author << ",year:" << bookList[i].year << "}" << endl;
    }
    return 0;
  } else filterList(bookList, filters);

  //only print
  for (int i = 0; i < bookList.size(); i++) {
    if (bookList[i].isValid == true) output << "{genre:" << bookList[i].genre << ",title:" << bookList[i].title << ",author:" << bookList[i].author << ",year:" << bookList[i].year << "}" << endl;
  }
  return 0;
}