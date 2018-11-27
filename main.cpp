#include <iostream>
#include <map>
#include <random>
#include <ctime>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct States //class definition
{
public:

    /*==== Variables ====*/
    string stateName; //name of the state
    int perCapitaIncome; //the income per person
    int population; //number of people in the state
    int medHouseIncome; //the median household income
    int numHouseholds; //total number of households in the states

    /*==== Constructors ====*/
    States();
};

States::States()
{
    stateName = "noName"; //initializes the state name
    perCapitaIncome = 0; //initializes the per capita income
    population = 0; //initializes the state's population
    medHouseIncome = 0; //initializes the median household income
    numHouseholds = 0; //initializes the state's total number of households
}

/*==== Program Functions ====*/
void RollDice(int, int);
void GetInfoFromFile(string, map<string,States>&);
void PrintAllStates(map<string,States>&);
vector<States> SearchForState(string, map<string,States> &);
void PrintStateData(States);

/*==== Random Numbers Function ====*/
mt19937 random_mt;
int Random(int min, int max)
{
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);
}


int main()
{
    cout << "1. Random Numbers\n";
    cout << "2. State Info\n";

    int option;
    cin >> option;

    if (option == 1)
    {
        int randomSeed;
        cout << "Random seed value: ";
        cin >> randomSeed;
        random_mt.seed(randomSeed);


        cout << "Number of times to roll the die: ";
        int numRolls;
        cin >> numRolls;

        cout << "Number of sides on this die: ";
        int numSides;
        cin >> numSides;

        cout << endl;

        // Roll Dice
        RollDice(numRolls,numSides);
    }
    else if (option == 2)
    {
        map<string,States> stateMap; //a map of state objects
        GetInfoFromFile("states.csv", stateMap); // Load the states

        cout << "1. Print all states" << endl;
        cout << "2. Search for a state" << endl;
        int menuChoice;
        cin >> menuChoice;

        if(menuChoice == 1)
            PrintAllStates(stateMap);
        else if(menuChoice == 2)
        {
            string name;
            cin >> name;
            vector<States> statesVec = SearchForState(name, stateMap);
            if(statesVec.empty())
                cout << "No match found for " << name << endl;
            else
                PrintStateData(statesVec.at(0));
        }
        else
            cout << "Please input a valid menu choice" << endl;
    }

    return 0;
}

void RollDice(int numberOfRolls, int numberOfSides)
{
    map<int, int> diceMap;

    for (int i = 1; i <= numberOfSides; i++)
    {
        diceMap.emplace(i, 0); //initializes all roll numbers to zero
    }

    for (int i = 0; i < numberOfRolls; i++)
    {
        int diceRoll = Random(1, numberOfSides); //this simulates a random dice roll
        auto iterator = diceMap.begin(); //this is an iterator that begins at the beginning of the map

        while(iterator != diceMap.end())
        {
            if(diceRoll == iterator->first) //if dice roll equal dice side number
            {
                iterator->second++; //increases the roll number of the appropriate number
                break;
            }
            iterator++;
        }

        //need a way to keep track of the total number of times a number has been rolled
    }
    for (int i = 1; i <= numberOfSides; i++)
    {
        cout << i << ":" << diceMap[i] << endl;
    }

}

void GetInfoFromFile(string fileName, map<string,States> &mapOfStates) {
    ifstream iFile(fileName);
    string lineFromFile;
    getline(iFile, lineFromFile); //simply gets rid of first line of unneeded info

    if (iFile.is_open())
    {
        int moduloCounter = 1;

        while (getline(iFile, lineFromFile)) //gets full line
        {
            States tempObject;
            string singleString;
            stringstream myStringStream(lineFromFile);

            for (unsigned int i = 0; i < 5; i++)
            {
                getline(myStringStream, singleString, ',');

                if (moduloCounter % 5 == 1) //checks to see if the string is state name
                {
                    tempObject.stateName = singleString;
                }

                if (moduloCounter % 5 == 2) //checks to see if the string is the theme name
                {
                    tempObject.perCapitaIncome = stoi(singleString);
                }

                if (moduloCounter % 5 == 3) //checks to see if the string is the set name
                {
                    tempObject.population = stoi(singleString);
                }

                if (moduloCounter % 5 == 4) //checks to see if the string is the mini figures
                {
                    tempObject.medHouseIncome = stoi(singleString);
                }

                if (moduloCounter % 5 == 0) //increments every sixth data point to complete the set
                {
                    tempObject.numHouseholds = stoi(singleString);

                    mapOfStates.emplace(tempObject.stateName, tempObject);
                    moduloCounter = 0;
                }

                moduloCounter++;

            } //end for

        }//end while
    }

    iFile.close();

}

void PrintAllStates(map<string,States> &mapOfStates)
{
    auto iterator = mapOfStates.begin(); //this is an iterator that begins at the beginning of the map
    while(iterator != mapOfStates.end())
    {
        PrintStateData(iterator->second);
        iterator++;
    }
}

vector<States> SearchForState(string stateName, map<string,States> &mapOfStates)
{
    vector<States> tempVec;
    auto iterator = mapOfStates.begin(); //this is an iterator that begins at the beginning of the map
    while(iterator != mapOfStates.end())
    {
        if(iterator->first.find(stateName) != string::npos)
        {
            tempVec.push_back(iterator->second); //this returns the state object
        }
        iterator++;
    }

    return tempVec;
}

void PrintStateData(States state)
{
    cout << state.stateName << endl;
    cout << "Population: " << state.population << endl;
    cout << "Per Capita Income: " << state.perCapitaIncome << endl;
    cout << "Median Household Income: " << state.medHouseIncome << endl;
    cout << "Number of Households: " << state.numHouseholds << endl;
}