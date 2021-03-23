#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace std;

struct Team {
	string name;
	int seed;

	Team(string _name, int _seed) : name(_name), seed(_seed) {}
};

string getWinner(Team* team1, Team* team2) {
	int seedDiff = team2->seed - team1->seed;
	int boundary = 50 + 3 * seedDiff;

	if (rand() % 100 > boundary) return team2->name;
	return team1->name;

}

void generateGuess(vector<pair<string, string>>& matchups, unordered_map<string, Team*>& teams, ofstream& ofs) {
	if (matchups.size() == 1) {
		string winner = getWinner(teams[matchups[0].first], teams[matchups[0].second]);

		if (winner == matchups[0].first) {
			ofs << matchups[0].second << "," << 2 << endl;
			ofs << matchups[0].first << "," << 1 << endl;
		}
		else {
			ofs << matchups[0].first << "," << 2 << endl;
			ofs << matchups[0].second << "," << 1 << endl;
		}

		return;
	}

	vector<pair<string, string>> nextMatchups;

	for (int i = 0; i < matchups.size(); i = i + 2) {
		pair<string, string> matchup = matchups[i];
		string winner = getWinner(teams[matchup.first], teams[matchup.second]);

		if (winner == matchup.first) {
			ofs << matchup.second << "," << matchups.size() * 2 << endl;
		}
		else {
			ofs << matchup.first << "," << matchups.size() * 2 << endl;
		}

		pair<string, string> nextMatchup{ winner, "" };

		matchup = matchups[i + 1];
		winner = getWinner(teams[matchup.first], teams[matchup.second]);

		if (winner == matchup.first) {
			ofs << matchup.second << "," << matchups.size() * 2 << endl;
		}
		else {
			ofs << matchup.first << "," << matchups.size() * 2 << endl;
		}

		nextMatchup.second = winner;

		nextMatchups.push_back(nextMatchup);
	}

	generateGuess(nextMatchups, teams, ofs);
}

int main() {
	ifstream fin;
	fin.open("data/teams.csv");
	
	// Store all the team info
	unordered_map<string, Team*> teams;

	vector<string> data;
	while (fin.good()) {
		string line;
		getline(fin, line);

		string word;
		stringstream s(line);
		while (getline(s, word, ',')) {
			data.push_back(word);
		}

		teams[data[0]] = new Team(data[0], stoi(data[1]));

		data.clear();
	}

	
	// Store all the matchups
	vector<pair<string, string>> matchups;

	fin.clear();
	fin.seekg(0, ios::beg);
	
	pair<string, string> matchup{ "", "" };

	while (fin.good()) {
		string line;
		getline(fin, line);

		string teamName;
		stringstream s(line);
		getline(s, teamName, ',');

		if (matchup.first.empty()) matchup.first = teamName;
		else {
			matchup.second = teamName;
			matchups.push_back(matchup);
			matchup.first = matchup.second = "";
		}
	}

	// NOW TO THE GOOD STUFF
	ofstream fout("data/result.csv");

	srand(time(NULL));
	generateGuess(matchups, teams, fout);

	fout.close();
}