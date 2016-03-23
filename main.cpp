
// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
#include <cassert>
#include "MersenneTwister.h"

using namespace std;


// ASSIGNMENT: FILL IN YOUR OWN MAP STRUCTURE

//smallest map. Used in all windows.
typedef map<string, int> MINI_MAP;
//medium sized map. Used in all windows. The pair holds the total of values inside the maps it contains
typedef map<string, pair<MINI_MAP, int> >  MY_MAP;
//Large sized map. Used in windows greater than 2. The pair holds the total of values inside the maps it contains
typedef map<string, pair<MY_MAP, int> > BIG_MAP;  




// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
    char c;
    word.clear();
    while (istr) {
        // just "peek" at the next character in the stream
        c = istr.peek();
        if (isspace(c)) {
            // skip whitespace before a word starts
            istr.get(c);
            if (word != "") {
                // break words at whitespace
                return true;
            }
        } else if (c == '"') {
            // double quotes are a delimiter and a special "word"
            if (word == "") {
                istr.get(c);
                word.push_back(c);
            }
            return true;
        } else if (isalpha(c)) {
            // this a an alphabetic word character
            istr.get(c);
            word.push_back(tolower(c));
        } else {
            // ignore this character (probably punctuation)
            istr.get(c);
        }
    }
    return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
    // returns a vector of strings of the different words
    std::vector<std::string> answer;
    std::string word;
    bool open_quote = false;
    while (ReadNextWord(istr,word)) {
        if (word == "\"") {
            if (open_quote == false) {
                open_quote=true;
            }
            else {
                break;
            }
        } else {
            // add each word to the vector
            answer.push_back(word);
        }
    }

    return answer;
}



int main () {
	// declaration of variables for input
    int window;
    MY_MAP data;
    BIG_MAP OuterData;
	
    // Parse each command
    std::string command;
    while (std::cin >> command) {

        // load the text file holding the instructions
        if (command == "load") {
            std::string filename;
            std::string parse_method;
            std::cin >> filename >> window >> parse_method;

            // verify the window parameter is appropriate
            if (window < 2) {
                std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
                exit(1);
            }
            // verify that the parse method is appropriate
            bool ignore_punctuation = false;
            if (parse_method == "ignore_punctuation") {
                ignore_punctuation = true;
            } else {
                std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
                exit(1);
            }


            //output of information inputed to top of output
            cout << "Loaded " << filename << " with window = " << window << " and parse method = " << parse_method << endl << endl;

			//Readying input from text of words
            std::ifstream istr_words(filename.c_str());
            if (!istr_words) {
                std::cerr << "ERROR cannot open word file: " << filename << std::endl;
                exit(1);
            }



			//Case if map = is 2
            if (window == 2) {

				//Declaration of strings to be set to NULL. Helpfull for adding the words for the first time
                string word_next = "NULL";
                string current = "NULL";
                while (ReadNextWord(istr_words,word_next)) {

                    // skip the quotation marks (not used for this part)
                    if (word_next == "\"") {
                        continue;
                    }
					//check to see if its the first time through and if so insert word_next instead of the usual current
                    if (current == "NULL") {
                        MINI_MAP temp;
                        data.insert(make_pair(word_next,make_pair(temp,0)));
                    }
                    else {
						//check to see if the current word being added is already in the list
                        if (data.find(current) != data.end()) {
                            MY_MAP::iterator a = (data.find(current));
							//check to see if the next word is already in the list of folliwng words
                            if (a->second.first.find(word_next) != a->second.first.end()) {
								//if so increment it
                                (a->second.first.find(word_next)->second)++;
								//increment the total
                                data.find(current)->second.second++;
                            }
                            else {
							//if it isnt found in the list of words that follow add it
                                a->second.first.insert(make_pair(word_next, 1));
								//increment the total
                                data.find(current)->second.second++;
                            }
                        }
						//if current word being added is not already in list
                        else {
							//create a MINI_MAP holding the next word
                            MINI_MAP temp;
                            temp.insert(make_pair(word_next, 1));
							//add that minimap to the current word and then add that.
                            data.insert(make_pair(current,make_pair(temp,1)));
                        }
                    }
					//set current to the next word so you can check for it in Data in the next run through the loop
                    current = word_next;
                }

                //increase count of last word
                data.find(current)->second.second++;
            }
            else {
				//window check
                assert(window > 2);
				//Declaration of strings to be set to NULL. Helpfull for adding the words for the first time
                string current = "NULL";
                string word_next = "NULL";
                string word_next2 = "NULL";

                while (ReadNextWord(istr_words,word_next2)) {
                    // skip the quotation marks (not used for this part)
                    if (word_next2 == "\"") continue;
//check to see if its the first or second time through
                    if (current == "NULL") {
                        if (word_next == "NULL") {
							//if first time
                            MY_MAP temp;
                            OuterData.insert(make_pair(word_next2,make_pair(temp,0)));
                        }
                        else {
							//if second time
                            MY_MAP data;
                            MINI_MAP temp;
                            data.insert(make_pair(word_next,make_pair(temp,0)));
                            OuterData.insert(make_pair(word_next2,make_pair(data,0)));
                        }
                    }
                    else {
                        //if found current in the biggest map
                        if (OuterData.find(current) != OuterData.end()) {
                            BIG_MAP::iterator a = (OuterData.find(current));
                            //if found the next word in the list of words following current
                            if (a->second.first.find(word_next) != a->second.first.end()) {
                                MY_MAP::iterator b = (a->second.first.find(word_next));
                                //if found the third trailing word in the list of words following the next word
                                if (b->second.first.find(word_next2) != b->second.first.end()) {
									//increment all of them
                                    (b->second.first.find(word_next2)->second)++;
                                    b->second.second++;
                                    a->second.second++;
                                }
                                //if havent found the third trailing word in MINIMAP 
                                else {
								//add word_next2 then increment rest
                                    b->second.first.insert(make_pair(word_next2,1));
                                    b->second.second++;
                                    a->second.second++;
                                }
                            }
                            //if havent found the trailing word in list of following words 
                            else {
								//add trailing word
                                MINI_MAP temp;
                                temp.insert(make_pair(word_next2, 1));
                                a->second.first.insert(make_pair(word_next, make_pair(temp,1)));
                                a->second.second++;
                            }
                        }
						//if havent found the current word in the list of words
                        else {
							//add the current word and the 2 trailing words
                            MINI_MAP temp;
                            MY_MAP data;
                            temp.insert(make_pair(word_next2, 1));
                            data.insert(make_pair(word_next,make_pair(temp,1)));
                            OuterData.insert(make_pair(current,make_pair(data,1)));
                        }
                    }
					//incrementing of strings so that they can be used in the next loop runthrough
                    current = word_next;
                    word_next = word_next2;
                }

                //increase count of last 2 words. Unknown if necessary anymore
                OuterData.find(current)->second.second++;
                OuterData.find(current)->second.first.find(word_next)->second.second++;
            }
        }

        // print the portion of the map structure with the choices for the
        // next word given a particular sequence.
        else if (command == "print") {

            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
			//case if window size is 2
            if (window == 2) {
				//check to see if sentence can be correctly printed with this window size
                assert(sentence.size() == 1);
				//creating iterator pointing to the first word of the sentence
                MY_MAP::iterator word = (data.find(sentence[0]));
				//Check to see if sentence is even in list
                assert (word != data.end());
				//printing of the first word and total number of following words
                cout << word->first << " (" <<  data.find(sentence[0])->second.second  << ")" << endl;
				//Print the following words with the amount of times they show up
                for (MINI_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                    cout << word->first << " " << it->first << " (" << it->second << ")" << endl;
                }
                cout << endl;
            }
			//case if window != 2 but still only has one word
            else if (window > 2 && sentence.size() == 1) {
               //effectivly the same code as earlier except using BIG_MAP and MY_MAP while ignoring MINI_MAP completly
                BIG_MAP::iterator word = (OuterData.find(sentence[0]));
                assert (word != OuterData.end());
                cout << word->first << " (" <<  OuterData.find(sentence[0])->second.second  << ")" << endl;

                for (MY_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                    cout << word->first << " " << it->first << " (" << it->second.second << ")" << endl;

                }
                cout << endl;
            }
			//case if window size is bigger then 2 and sentence size is 2
            else if (window > 2 && sentence.size() == 2) {
//goes through the two following words too
                BIG_MAP::iterator word = (OuterData.find(sentence[0]));
				//check to see if word is even in list
                assert (word != OuterData.end());
				//output of first 2 words and the total of the words following
                cout << sentence[0] << " " << sentence[1] << " (" <<  OuterData.find(sentence[0])->second.first.find(sentence[1])->second.second  << ")" << endl;
                MY_MAP::iterator it = OuterData.find(sentence[0])->second.first.find(sentence[1]);
				//Prints out the whole sets and the amount of times each happens
                for (MINI_MAP::iterator small = ((it->second).first).begin(); small != ((it->second).first).end(); small++) {
                    cout << sentence[0] << " " << sentence[1] << " " << small->first << " (" << small->second << ")" << endl;
                }
                cout << endl;
            }
            else {
				//incorrect window/sentence size
                cerr << "error: you cannot print this many words with this small of window" << endl;
                exit(1);
            }
        }

        // generate the specified number of words in a specified order
        else if (command == "generate") {
			//the sentence to generate words from
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            // how many additional words to generate
            int length;
            std::cin >> length;
			//how words are to be generated
            std::string selection_method;
            std::cin >> selection_method;
			//if random or most common is to be used for word generation
            bool random_flag;
            if (selection_method == "random") {
                random_flag = true;
            } else {
                assert (selection_method == "most_common");
                random_flag = false;
            }
			//case if window is 2
            if (window == 2) {
				//make sure sentence only has 1 word
                assert(sentence.size() == 1);
				//finding the word specified
                MY_MAP::iterator word = (data.find(sentence[0]));
				//making sure that word exists
                assert (word != data.end());
				//outputting the word that others are generated from
                cout << word->first << " ";
                MTRand a;
				//for the amount of words you want to generate
                for (int i = 0; i < length; i++) {
                    pair<string,int> max;
					//if generated by most commong
                    if(random_flag == false) {
						//find the most common by comparison of how many times each word following comes up
                        for (MINI_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                            if ( it->second > max.second) {
                                max = make_pair(it->first,it->second);
                            }
							//if they are the same check alphabetically
                            else if (it->second == max.second) {
                                assert(it->first != max.first);
                                if (it->first < max.first) {
                                    max = make_pair(it->first,it->second);
                                }

                            }
                        }
                    }
					//if generated by random
                    else {
						//creation of random integer
                        int rand = a.randInt(word->second.first.size() - 1);
                        MINI_MAP::iterator it = ((word->second).first).begin();
                        int j = 0;
						//finding the iterator that the random points to
                        while (j < rand) {
                            it++;
                            j++;
                        }
						//saving it
                        max = make_pair(it->first,it->second);
                    }
					//outputing of word found and saved in max
                    cout << max.first;
					//outputting case to make sure not printing extra space at end
                    if (i != length - 1) {
                        cout << " ";
                    }
					//saving the found word so it can be used
                    word = data.find(max.first);
                }
                cout << endl << endl;
            }

			//for a window size greater than 2
            else if(window > 2) {
			//This is like the previous generate code except checks 2 words ahead instead of 1.
                BIG_MAP::iterator word = (OuterData.find(sentence[sentence.size() - 1]));
				//making sure we have the word
                assert (word != OuterData.end());
				//output of the first word
                cout << sentence[0] << " ";
				//if the word size is greater than 1 and you are generating random. Output the second word in sentence too
                if (sentence.size() == 2 && random_flag == true) {
                    cout << sentence[1] << " ";
                    BIG_MAP::iterator word = (OuterData.find(sentence[1]));
                }
                MTRand a;
                pair<string,int> max;
				//for the amount of words you want to generate
                for (int i = 0; i < length; i++) {
					//declaration of pair which is used to store values
                    pair<string,int> maxb;
					//if you are generating by most common
                    if(random_flag == false) {
					//only run the first time
                        if (max.second == 0) {
                            MY_MAP::iterator it = ((word->second).first).begin();
							//finding the most common word following the first
                            while (it != ((word->second).first).end()) {
                                if ( it->second.second > max.second) {
                                    max = make_pair(it->first,it->second.second);
                                }
								//if equally common compare alphabetically
                                else if (it->second.second == max.second) {
                                    assert(it->first != max.first);
                                    if (it->first < max.first) {
                                        max = make_pair(it->first,it->second.second);
                                    }
                                }
                                it++;
                            }
                        }
						//find the most common 3rd word to follow the previous two words
                        for (MINI_MAP::iterator itr_small = (((word->second).first).find(max.first)->second.first).begin(); itr_small != (((word->second).first).find(max.first)->second).first.end(); itr_small++) {

                            if ( itr_small->second > maxb.second) {

                                maxb = make_pair(itr_small->first,itr_small->second);

                            }
							//if equally common compare alphabetically
                            else if (itr_small->second == maxb.second) {
								//check to see if they arent the same name
                                assert(itr_small->first != maxb.first);
                                if (itr_small->first < maxb.first) {
                                    maxb = make_pair(itr_small->first,itr_small->second);
                                }
                            }
                        }
                    }
					//if random generation
                    else {
						//creation of random  number
                        int rand = a.randInt(word->second.first.size() - 1);
                        MY_MAP::iterator it = ((word->second).first).begin();
                        int j = 0;
						//get the iterator that random pointed to
                        while (j < rand) {
                            it++;
                            j++;
                        }
						//save it
                        max = make_pair(it->first,it->second.second);
                    }
					//output the saved string in max
                    cout << max.first;
                    if (i != length - 1) {
                        cout << " ";
                    }
					//reset word for the next runthrough
                    word = OuterData.find(max.first);
                    if (random_flag == false)
						//if random generation save max for the next runthrough
                        max = make_pair(word->second.first.find(maxb.first)->first,maxb.second);

                }
                cout << endl << endl;

            }
            else {
				//mismatch of window size
                cerr << "error: you cannot generate this words with this small of window" << endl;
                exit(1);
            }



        } else if (command == "quit") {
			//call for the program to quit
            break;
        } else {
			//unknown command passed in
            std::cout << "WARNING: Unknown command: " << command << std::endl;
        }
    }
    return 0;
}
