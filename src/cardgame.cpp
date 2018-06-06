//============================================================================
// Name        : cardgame.cpp
// Author      : Xiao Li
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================



#include <iostream>
#include "CardManager.h"
#include <map>
#include <algorithm>
using namespace std;

int main(int argc, const char * argv[]) {
	char pOrq;
	int numberOfplayers;
	string playerName;
	vector<string> playerNames;
	int bestCardStyle = 10;
//	int highValue=1;
	string winner;
	CardManager manager;
	bool isRun = true;
	manager.refreshAllCards();
	map<string, int> playerStyleValue;
	while (isRun) {
		cout << "Do you wish to play poker or quit(p or q)? " << endl;
		cin >> pOrq;
		if (pOrq == 'p' || pOrq == 'P') {
			cout << "How many players are playing? " << endl;
			cin >> numberOfplayers;
			for (int i = 0; i < numberOfplayers; i++) {
				cout << "Enter player name: " << endl;
				cin >> playerName;
				playerNames.push_back(playerName);

				VectorCard myCards = manager.randomSelectCards(5);
				GamerNormal gamer(myCards, playerNames[i]);
				gamer.showMyCardsStyle();
				cout << endl;
				playerStyleValue[playerNames[i]] = gamer.judgeCardsStyle();
			}
			map<string, int>::const_iterator iter;
			for (iter = playerStyleValue.begin(); iter != playerStyleValue.end();
					iter++) {
				if (iter->second < bestCardStyle) {
					bestCardStyle = iter->second;    //iter->first is the playerName,iter->second is the score
					winner = iter->first;    //now need to find the smallest cardStyle; smallest will win?? wired anyway.
				}
				else if(iter->second == bestCardStyle){
					winner ="no exist!";
				}
			}
			cout << " The winner is:" << winner << endl;
		}
		else if(pOrq == 'Q' || pOrq == 'q') {
		            cout<<" Quit game."<<endl;
		            isRun = false;
		        }
		        else{
		            cout<<"Unknown Command!!"<<endl;
		        }
	}

	return 0;
}


