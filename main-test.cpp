//to do
//random
//indent
//comment

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

typedef map<string, int> MINI_MAP;
typedef map<string, pair<MINI_MAP, int> >  MY_MAP;
typedef map<string, pair<MY_MAP, int> > BIG_MAP;  //use if window  = 3
// 3 // typedef map<string, map<string,  map<string, int > > > MY_MAP;




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





// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
// void LoadSampleText(MY_MAP &data, const std::string &filename, int window, const std::string &parse_method) {
// // open the file stream


// }



int main () {


    //add extra check

    int window;
    MY_MAP data;
    BIG_MAP OuterData;
    // Parse each command
    std::string command;
    while (std::cin >> command) {

        // load the sample text file
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


            //
            // ASSIGNMENT: ADD YOUR COMMANDS HERE
            //
            cout << "Loaded " << filename << " with window = " << window << " and parse method = " << parse_method << endl << endl;

            std::ifstream istr_words(filename.c_str());
            if (!istr_words) {
                std::cerr << "ERROR cannot open word file: " << filename << std::endl;
                exit(1);
            }





            //
            // ASSIGNMENT:SETUP YOUR MAP DATA AS NEEDED
            //


            if (window == 2) {


                string word_next = "NULL";
                string current = "NULL";
                while (ReadNextWord(istr_words,word_next)) {

                    // skip the quotation marks (not used for this part)
                    if (word_next == "\"") {
                        continue;
                    }
                    if (current == "NULL") {
                        MINI_MAP temp;
                        data.insert(make_pair(word_next,make_pair(temp,0)));
                    }
                    else {

                        if (data.find(current) != data.end()) {
                            MY_MAP::iterator a = (data.find(current));

                            if (a->second.first.find(word_next) != a->second.first.end()) {

                                (a->second.first.find(word_next)->second)++;
                                data.find(current)->second.second++;
                            }
                            else {

                                a->second.first.insert(make_pair(word_next, 1));
                                data.find(current)->second.second++;
                            }
                        }
                        else {

                            MINI_MAP temp;


                            temp.insert(make_pair(word_next, 1));
                            data.insert(make_pair(current,make_pair(temp,1)));

                        }

                        //
                        // ASSIGNMENT: PROCESS ALL THE NON PUNCTUATION WORDS
                        // INSERTING THESE WORDS INTO THE DATA STRUCTURE
                        //
                    }
                    current = word_next;
                }

                //increase count of last word
                data.find(current)->second.second++;
            }
            else {
                assert(window == 3);

                string current = "NULL";
                string word_next = "NULL";
                string word_next2 = "NULL";

                while (ReadNextWord(istr_words,word_next2)) {
                    // skip the quotation marks (not used for this part)
                    if (word_next2 == "\"") continue;

                    if (current == "NULL") {
                        if (word_next == "NULL") {
                            MY_MAP temp;
                            OuterData.insert(make_pair(word_next2,make_pair(temp,0)));
                        }
                        else {
                            MY_MAP data;
                            MINI_MAP temp;
                            data.insert(make_pair(word_next,make_pair(temp,0)));
                            OuterData.insert(make_pair(word_next2,make_pair(data,0)));
                        }
                    }
                    else {



                        //if found current in BIG_MAP
                        if (OuterData.find(current) != OuterData.end()) {
                            BIG_MAP::iterator a = (OuterData.find(current));
                            //if found word_next in MY_MAP
                            if (a->second.first.find(word_next) != a->second.first.end()) {
                                MY_MAP::iterator b = (a->second.first.find(word_next));
                                //if found word_next2 in MINI_MAP       //increment all

                                if (b->second.first.find(word_next2) != b->second.first.end()) {
                                    (b->second.first.find(word_next2)->second)++;
                                    b->second.second++;

                                    a->second.second++;

                                }
                                //if havent found word_next2 in MINIMAP //add word_next2 then increment rest
                                else {

                                    b->second.first.insert(make_pair(word_next2,1));
                                    b->second.second++;
                                    a->second.second++;
                                }
                            }
                            //if havent found word_next in MY_MAP //add word_next
                            else {

                                MINI_MAP temp;
                                temp.insert(make_pair(word_next2, 1));
                                a->second.first.insert(make_pair(word_next, make_pair(temp,1)));
                                //added



                                a->second.second++;

                            }
                        }
                        else {

                            MINI_MAP temp;
                            MY_MAP data;
                            temp.insert(make_pair(word_next2, 1));
                            data.insert(make_pair(word_next,make_pair(temp,1)));
                            OuterData.insert(make_pair(current,make_pair(data,1)));


                        }


                    }
                    current = word_next;
                    word_next = word_next2;

                }

                //increase count of last word
                OuterData.find(current)->second.second++;
                OuterData.find(current)->second.first.find(word_next)->second.second++;



            }
        }

        // print the portion of the map structure with the choices for the
        // next word given a particular sequence.
        else if (command == "print") {

            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            if (window == 2) {
                assert(sentence.size() == 1);
                MY_MAP::iterator word = (data.find(sentence[0]));
                assert (word != data.end());
                cout << word->first << " (" <<  data.find(sentence[0])->second.second  << ")" << endl;

                for (MINI_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                    cout << word->first << " " << it->first << " (" << it->second << ")" << endl;

                }
                cout << endl;
            }
            else if (window == 3 && sentence.size() == 1) {
                //has seg faults
                //rewrite to go from current but ignore the 3rd value
                BIG_MAP::iterator word = (OuterData.find(sentence[0]));
                assert (word != OuterData.end());
                cout << word->first << " (" <<  OuterData.find(sentence[0])->second.second  << ")" << endl;

                for (MY_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                    cout << word->first << " " << it->first << " (" << it->second.second << ")" << endl;

                }
                cout << endl;
            }
            else if (window == 3 && sentence.size() == 2) {

                //same cept multiword
                BIG_MAP::iterator word = (OuterData.find(sentence[0]));
                assert (word != OuterData.end());
                cout << sentence[0] << " " << sentence[1] << " (" <<  OuterData.find(sentence[0])->second.first.find(sentence[1])->second.second  << ")" << endl;
                MY_MAP::iterator it = OuterData.find(sentence[0])->second.first.find(sentence[1]);
                for (MINI_MAP::iterator small = ((it->second).first).begin(); small != ((it->second).first).end(); small++) {
                    cout << sentence[0] << " " << sentence[1] << " " << small->first << " (" << small->second << ")" << endl;
                }

                cout << endl;

            }
            else {
                cerr << "error: you cannot print this many words with this small of window" << endl;
                exit(1);
            }


        }

        // generate the specified number of words


        else if (command == "generate") {
            std::vector<std::string> sentence = ReadQuotedWords(std::cin);
            // how many additional words to generate
            int length;
            std::cin >> length;
            std::string selection_method;
            std::cin >> selection_method;
            bool random_flag;
            if (selection_method == "random") {
                random_flag = true;
            } else {
                assert (selection_method == "most_common");
                random_flag = false;
            }

            if (window == 2) {
				assert(sentence.size() == 1);
                MY_MAP::iterator word = (data.find(sentence[0]));
                assert (word != data.end());
                cout << word->first << " ";
                MTRand a;
                for (int i = 0; i < length; i++) {
                    pair<string,int> max;
                    if(random_flag == false) {
                        for (MINI_MAP::iterator it = ((word->second).first).begin(); it != ((word->second).first).end(); it++) {
                            if ( it->second > max.second) {
                                max = make_pair(it->first,it->second);
                            }
                            else if (it->second == max.second) {
                                assert(it->first != max.first);
                                if (it->first < max.first) {
                                    max = make_pair(it->first,it->second);
                                }

                            }
                        }
                    }
                    else {
                        int rand = a.randInt(word->second.first.size() - 1);
                        MINI_MAP::iterator it = ((word->second).first).begin();
                        int j = 0;
                        while (j < rand) {
                            it++;
                            j++;
                        }
                        max = make_pair(it->first,it->second);
                    }
                    cout << max.first;
                    if (i != length - 1) {
                        cout << " ";
                    }
                    word = data.find(max.first);


                }
                cout << endl << endl;
            }


            else if(window == 3 && sentence.size() == 1){
//same as before cept with the outer map and ignoring the smallest
				  BIG_MAP::iterator word = (OuterData.find(sentence[0]));
                assert (word != OuterData.end());
                cout << word->first << " ";
                MTRand a;

		pair<string,int> max;
			//MY_MAP::iterator it
                for (int i = 0; i < length; i++) {
					
                    
									pair<string,int> maxb;
				
                    if(random_flag == false) {
						
						
						if (max.second == 0){
						
							MY_MAP::iterator it = ((word->second).first).begin();
						
                        while (it != ((word->second).first).end()) {
                            if ( it->second.second > max.second) {
                                max = make_pair(it->first,it->second.second);
                            }

                            else if (it->second.second == max.second) {
                                assert(it->first != max.first);
                                if (it->first < max.first) {
                                    max = make_pair(it->first,it->second.second);
                                }

                            }
							 it++;
                        }
						
								}
						
					
						 for (MINI_MAP::iterator itr_small = (((word->second).first).find(max.first)->second.first).begin(); itr_small != (((word->second).first).find(max.first)->second).first.end(); itr_small++) {
						
                            if ( itr_small->second > maxb.second) {
								
                                maxb = make_pair(itr_small->first,itr_small->second);
								
                            }
                            else if (itr_small->second == maxb.second) {
								
                                assert(itr_small->first != maxb.first);
                                if (itr_small->first < maxb.first) {
                                    maxb = make_pair(itr_small->first,itr_small->second);
                                }

                            }
						
                        }
						 
                    }
                    else {
                        int rand = a.randInt(word->second.first.size() - 1);
						
                        MY_MAP::iterator it = ((word->second).first).begin();
                        int j = 0;
						
                        while (j < rand) {
                            it++;
                            j++;
                        }
						
                        max = make_pair(it->first,it->second.second);
//cout << max.first << endl;
                    }
					
                    cout << max.first;
                    if (i != length - 1) {
                        cout << " ";
                    }
					
                    word = OuterData.find(max.first);
					if (random_flag == false)
					max = make_pair(word->second.first.find(maxb.first)->first,maxb.second);
	
                }
                cout << endl << endl;
				
            }
			else if(window == 3 && sentence.size() == 2) {
		//same cept multiword
				
			}
			 else {
                cerr << "error: you cannot generate this words with this small of window" << endl;
                exit(1);
            }
//word + following words in frequency
            //length is amount, random is random from frequency
            //check for previously used and full.

            //
            // ASSIGNMENT: ADD YOUR COMMANDS HERE
            //


        } else if (command == "quit") {
            break;
        } else {
            std::cout << "WARNING: Unknown command: " << command << std::endl;
        }
    }
    return 0;
}
