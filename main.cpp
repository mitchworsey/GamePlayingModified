/*
NAME: Mitch Worsey
STUDENT ID: 8370927291
EXTRA CREDIT VERSION
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sys/time.h>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <limits>
#include <ctime>

using namespace std;

struct State {
    int lives, total, scratch;
    bool hold;
    float prob;
    State(int l = 0, int t = 0, int s = 0, bool h = false, float p = 0.0) : 
	lives(l), total(t), scratch(s), hold(h), prob(p) { }
};

void print(vector<State> states){
    ofstream fout;
    fout.open("output.txt");
    for(int x = 0; x < states.size(); x++){
	if(states[x].scratch > 0 && states[x].scratch + states[x].total < 40){
	    fout << states[x].lives << " " << states[x].total << " " << states[x].scratch << " ";
	    if(states[x].hold)
		fout << "Yes ";
	    else
		fout << "No ";
	    fout << states[x].prob << endl;
	}
    }
    fout.close();
}

void APrint(vector<State> states){
    ofstream fout;
    fout.open("output.txt");
    fout << "Assuming I have X > 0 lives, a total score of 20 and a scratch score of Y < 20, " << 
		"the states for which I should yell hold are:" << endl << endl;
    vector< vector<int> > values;
    vector<int> XVals, YVals;

    for(int x = 0; x < states.size(); x++){
	if(states[x].scratch > 0 && states[x].scratch + states[x].total < 40){
	    if(states[x].lives > 0 && states[x].total == 20 && states[x].scratch < 20 && states[x].hold){
		fout << states[x].lives << " " << states[x].total << " " << states[x].scratch << " ";
		if(states[x].hold)
		    fout << "Yes ";
		else
		    fout << "No ";
		fout << states[x].prob << endl;
	    }
	}
    }
    fout.close();
}

void BPrint(vector<State> states){
    ofstream fout;
    fout.open("output.txt");
    float winProb = 0.0;
    for(int x = 0; x < states.size(); x++){
	if(states[x].scratch == 0 && states[x].total == 0 && states[x].lives == 5){
	    winProb = states[x].prob;
	    fout << "Start state: " << endl;
	    fout << states[x].lives << " " << states[x].total << " " << states[x].scratch << " ";
	    if(states[x].hold)
		fout << "Yes ";
	    else
		fout << "No ";
	    fout << states[x].prob << endl << endl;
	}
    }

    fout << "Expected profit = $500 x " << winProb << " = $" << setprecision(5) << 500*winProb << endl;
    fout << "Willing to pay at most $" << setprecision(5) << 500*winProb << 
		" to participate in this game in order to maximize expected profit." << endl;		

    fout.close();
}

void generateStates(vector<State> &states){
    for(int l = 5; l > 0; l--){
	for(int t = 0; t < 44; t++){
	    for(int s = 0; s+t < 44; s++)
		if(s + t < 40)
		    states.push_back(State(l, t, s, false, 0.0));
		else
		    states.push_back(State(l, t, s, true, 1.0));
	}
    }
}


void valueIteration(vector<State> &states){
    for(int i = 0; i < 100; i++){
	for(int s = 0; s < states.size(); s++){
	if(states[s].scratch + states[s].total < 40){
	    float holdProb = 0.0;
	    float noHoldProb = 0.0;
	    for(int r = 1; r <= 6; r++){
		if(r == 6){
		    for(int t = 0; t < states.size(); t++){
			if(states[t].lives == states[s].lives-2 &&
			   states[t].total == states[s].total+states[s].scratch &&
			   states[t].scratch == 0){
			    holdProb += (1.0/6.0) * states[t].prob;
			    //cout << "State: " << s << endl;
			}
			if(states[t].lives == states[s].lives-1 &&
			   states[t].total == states[s].total &&
			   states[t].scratch == 0){
			    noHoldProb += (1.0/6.0) * states[t].prob;
			    //cout << "State: " << s << endl;
			}
		    }
		}
		else if(r == 5){
		    if(states[s].lives < 5){
			for(int t = 0; t < states.size(); t++){
			    if(states[t].lives == states[s].lives && states[s].lives > 1 &&
			       states[t].total == states[s].total+states[s].scratch &&
			       states[t].scratch == 0){
				holdProb += (1.0/6.0) * states[t].prob;
				//cout << "State: " << s << endl;
			    }
			    if(states[t].lives == states[s].lives+1 &&
			       states[t].total == states[s].total &&
			       states[t].scratch == 0){
				noHoldProb += (1.0/6.0) * states[t].prob;
				//cout << "State: " << s << endl;
			    }
			}
		    }
		    else if(states[s].lives == 5){
			for(int t = 0; t < states.size(); t++){
			    if(states[t].lives == states[s].lives &&
			       states[t].total == states[s].total+states[s].scratch &&
			       states[t].scratch == 0){
				holdProb += (1.0/6.0) * states[t].prob;
				//cout << "State: " << s << endl;
			    }
			    if(states[t].lives == states[s].lives &&
			       states[t].total == states[s].total &&
			       states[t].scratch == 0){
				noHoldProb += (1.0/6.0) * states[t].prob;
				//cout << "State: " << s << endl;
			    }
			}
		    }
		}
		else{
		    for(int t = 0; t < states.size(); t++){
			if(states[t].lives == states[s].lives-1 &&
			   states[t].total == states[s].total+states[s].scratch &&
			   states[t].scratch == r){
			    holdProb += (1.0/6.0) * states[t].prob;
			    //cout << "State: " << s << " Roll: " << r << endl;
			}
			if(states[t].lives == states[s].lives &&
			   states[t].total == states[s].total &&
			   states[t].scratch == states[s].scratch+r){
			    noHoldProb += (1.0/6.0) * states[t].prob;
			    //cout << "State: " << s << " Roll: " << r << endl;
			}
		    }
		}
	    }
	    if(holdProb > noHoldProb){
		states[s].prob = holdProb;
		states[s].hold = true;
	    }
	    else{
		states[s].prob = noHoldProb;
		states[s].hold = false;
	    }
	}}
    }
}

int main(){
    vector<State> states;
    generateStates(states);

    valueIteration(states);

    //APrint(states);
    //BPrint(states);
    print(states);
    return 0;
}
