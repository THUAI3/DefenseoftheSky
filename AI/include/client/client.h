#pragma once

#include "../jsoncpp/json/json.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>

const int WIDTH = 10;
const int HEIGHT = 10;
const int MAXRANGENUM = 3;
const int PLAYER = 2;

class Parameters{
public:
	int num;
	int mapWidth;
	int mapHeight;
	int landPrice;
	int pollutionComponentNum;
	int maxRoundNum;
	int maxRangeNum;
	int tipsterCost;
	std::vector<std::pair<int,int>>buildings;
	std::vector<int>processorRangeCost;
	std::vector<int>processorTypeCost;
	std::vector<int>detectorRangeCost;
	std::vector<int>pollutionProfit;

	Parameters();
	~Parameters();
	void Debug(FILE*);
};

class Processor{
public:
	std::pair<int, int>pos;
	int rangeType;
	int processingType;
	int owner;
	Processor(int, int, int, int, int);
	~Processor();
};

class Detector{
public:
	std::pair<int,int>pos;
	int rangeType;
	int owner;
	Detector(int, int, int, int);
	~Detector();
};

class Land{
public:
	int owner;
	int occupied;
	int bid;
	int bidder;
	int round;
	int bidOnly;
	Land();
	~Land();
};

class State{
public:
    int num;
    int money[PLAYER];
    int score[PLAYER];
    int pollution[WIDTH][HEIGHT];
    Land lands[WIDTH][HEIGHT];
    std::vector<Detector>detectors;
    std::vector<Processor>processors;
    State();
    ~State();
    void clear();
    void Debug(FILE*);
};

class Operations{
public:
    Operations();
    ~Operations();
    void clear();
    void Debug(FILE*);
    void setTipster(int, int);
    void setDetector(int, int, int);
    void setProcessor(int, int, int, int);
    void setBid(int, int, int);
private:
	int tipsterX, tipsterY;
	int detectorX, detectorY;
	int bidX, bidY;
	int processorX, processorY;
	int detectorRangeType;
	int processorRangeType;
	int processorProcessingType;
	int bidPrice;

	friend class Client;
};

class Client{
public:
	Parameters* parameters;
	State* state;
	Operations* opt;
	Client();
	~Client();
	bool init();
	bool stateInfo();
	void sendOpt(FILE*);
private:
	Json::Value root;
	Json::Reader reader;
	Json::FastWriter fw;
	std::string read();
	void sendLen(int len);
};

