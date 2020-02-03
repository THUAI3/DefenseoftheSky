#include "client.h"

Parameters::Parameters(){}
Parameters::~Parameters(){}
void Parameters::Debug(FILE* fp){
	fprintf(fp, "============Parameters begin=============\n");
	fprintf(fp, "num = %d\n", num);
	fprintf(fp, "mapWidth = %d\n", mapWidth);
	fprintf(fp, "mapHeight = %d\n", mapHeight);
	fprintf(fp, "landPrice = %d\n", landPrice);
	fprintf(fp, "pollutionComponentNum = %d\n", pollutionComponentNum);
	fprintf(fp, "maxRoundNum = %d\n", maxRoundNum);
	fprintf(fp, "maxRangeNum = %d\n", maxRangeNum);
	fprintf(fp, "tipsterCost = %d\n", tipsterCost);
	fprintf(fp, "buildings: \n");
	for(int i = 0; i < buildings.size(); ++i){
		fprintf(fp,"        (%d, %d)\n", buildings[i].first, buildings[i].second);
	}
	fprintf(fp, "processorRangeCost: \n");
	for(int i = 0; i < processorRangeCost.size(); ++i){
		fprintf(fp, "        %d\n", processorRangeCost[i]);
	}
	fprintf(fp, "processorTypeCost: \n");
	for(int i = 0; i < processorTypeCost.size(); ++i){
		fprintf(fp, "        %d\n", processorTypeCost[i]);
	}
	fprintf(fp, "detectorRangeCost: \n");
	for(int i = 0; i < detectorRangeCost.size(); ++i){
		fprintf(fp, "        %d\n", detectorRangeCost[i]);
	}
	fprintf(fp, "pollutionProfit: \n");
	for(int i = 0; i < pollutionProfit.size(); ++i){
		fprintf(fp, "        %d\n", pollutionProfit[i]);
	}
	fprintf(fp, "============Parameters   end=============\n\n");
	fflush(fp);
}

Processor::Processor(int x, int y, int rangeType, 
				   int processingType ,int owner){
	this->pos = std::make_pair(x, y);
	this->rangeType = rangeType;
	this->processingType = processingType;
	this->owner = owner;
}
Processor::~Processor(){}

Detector::Detector(int x, int y, 
				   int rangeType, int owner){
	this->pos = std::make_pair(x, y);
	this->rangeType = rangeType;
	this->owner = owner;
}
Detector::~Detector(){}

Land::Land(){}
Land::~Land(){}

State::State(){}
State::~State(){}
void State::clear(){
	detectors.clear();
	processors.clear();
}

void State::Debug(FILE* fp){
	fprintf(fp, "============State begin=============\n");
	fprintf(fp, "num = %d\n", num);
	fprintf(fp, "money[0] = %d, money[1] = %d\n", money[0], money[1]);
	fprintf(fp, "score[0] = %d, score[1] = %d\n", score[0], score[1]);
	fprintf(fp, "pollution: \n");
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			fprintf(fp, "%d ", pollution[i][j]);
		}fprintf(fp, "\n");
	}
	fprintf(fp, "lands: \n");
	int x, y;
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(lands[i][j].owner != -1){
				x = lands[i][j].owner;
				y = lands[i][j].occupied;
			}
			else if(lands[i][j].bidOnly != -1){
				x = lands[i][j].bidOnly;
				y = lands[i][j].occupied;
			}else{
				x = lands[i][j].bid;
				if(lands[i][j].round != -1)y = lands[i][j].bidder*10+lands[i][j].round;
				else y = lands[i][j].bidder;
			}
			fprintf(fp, "(%5d, %5d) ", x, y);
		}fprintf(fp, "\n");
	}
	fprintf(fp, "detectors: \n");
	for(int i = 0; i < detectors.size(); ++i){
		fprintf(fp, "pos = (%d, %d), rangeType = %d, owner = %d\n", 
			detectors[i].pos.first, detectors[i].pos.second,
			detectors[i].rangeType, detectors[i].owner);
	}
	fprintf(fp, "processors: \n");
	for(int i = 0; i < processors.size(); ++i){
		fprintf(fp, "pos = (%d, %d), rangeType = %d, processingType = %d, owner = %d\n",
				processors[i].pos.first, processors[i].pos.second,
				processors[i].rangeType, processors[i].processingType,
				processors[i].owner);
	}
	fprintf(fp, "============State   end=============\n\n");
	fflush(fp);
}

Operations::Operations(){
	clear();
}
Operations::~Operations(){}
void Operations::clear(){
	tipsterX = -1;
	tipsterY = -1;
	detectorX = -1;
	detectorY = -1;
	bidX = -1; bidY = -1;
	processorX = -1;
	processorY = -1;
	detectorRangeType = -1;
	processorProcessingType = -1;
	processorRangeType = -1;
	bidPrice = -1;
}
void Operations::Debug(FILE* fp){
	fprintf(fp, "============Opt begin=============\n");
	if(tipsterX != -1 || tipsterY != -1){
		fprintf(fp, "tipster: \n");
		fprintf(fp, "    pos = (%d, %d)\n", tipsterX, tipsterY);
	}
	if(detectorX != -1 || detectorY != -1 || detectorRangeType != -1){
		fprintf(fp, "detector: \n");
		fprintf(fp, "    pos = (%d, %d)\n", detectorX, detectorY);
		fprintf(fp, "    rangeType = %d\n", detectorRangeType);
	}
	if(processorX != -1 || processorY != -1 || processorProcessingType != -1 | processorRangeType != -1){
		fprintf(fp, "processor: \n");
		fprintf(fp, "    pos = (%d, %d)\n", processorX, processorY);
		fprintf(fp, "    rangeType = %d\n", processorRangeType);
		fprintf(fp, "    processingType = %d\n", processorProcessingType);
	}
	if(bidX != -1 || bidY != -1 || bidPrice != -1){
		fprintf(fp, "bid: \n");
		fprintf(fp, "    pos = (%d, %d)\n", bidX, bidY);
		fprintf(fp, "    bidPrice = %d\n", bidPrice);
	}
	fprintf(fp, "============Opt   end=============\n\n");
	fflush(fp);
}
void Operations::setTipster(int x, int y){
	tipsterX = x;
	tipsterY = y;
}
void Operations::setDetector(int x, int y, int rangeType){
	detectorX = x;
	detectorY = y;
	detectorRangeType = rangeType;
}
void Operations::setProcessor(int x, int y, int rangeType, int processingType){
	processorX = x;
	processorY = y;
	processorProcessingType = processingType;
	processorRangeType = rangeType;
}
void Operations::setBid(int x, int y, int bidPrice){
	bidX = x;
	bidY = y;
	this->bidPrice = bidPrice;
}

Client::Client(){
	parameters = new Parameters();
	state = new State();
	opt = new Operations();
}
Client::~Client(){
	delete parameters;
	delete state;
	delete opt;
}
bool Client::init(){
	std::string initStr = read();
	root.clear();
	if(!reader.parse(initStr, root))return false;
	parameters->num = root["AI"].asInt();
	parameters->mapWidth = root["mapWidth"].asInt();
	parameters->mapHeight = root["mapHeight"].asInt();
	parameters->landPrice = root["landPrice"].asInt();
	parameters->pollutionComponentNum = root["pollutionComponentNum"].asInt();
	parameters->maxRangeNum = root["maxRangeNum"].asInt();
	parameters->maxRoundNum = root["maxRoundNum"].asInt();
	parameters->tipsterCost = root["tipsterCost"].asInt();
	int sz = root["buildings"].size();
	for(int i = 0; i < sz; ++i){
		parameters->buildings.push_back(std::make_pair(root["buildings"][i][0].asInt(), root["buildings"][i][1].asInt()));
	}
	sz = root["processorRangeCost"].size();
	for(int i = 0; i < sz; ++i){
		parameters->processorRangeCost.push_back(root["processorRangeCost"][i].asInt());
	}
	sz = root["processorTypeCost"].size();
	for(int i = 0; i < sz; ++i){
		parameters->processorTypeCost.push_back(root["processorRangeCost"][i].asInt());
	}
	sz = root["detectorRangeCost"].size();
	for(int i = 0; i < sz; ++i){
		parameters->detectorRangeCost.push_back(root["detectorRangeCost"][i].asInt());
	}
	sz = root["pollutionProfit"].size();
	for(int i = 0; i < sz; ++i){
		parameters->pollutionProfit.push_back(root["pollutionProfit"][i].asInt());
	}
	return true;
}
bool Client::stateInfo(FILE* fp){
	std::string stateStr = read();
	root.clear();
	if(!reader.parse(stateStr, root))return false;
	state->clear();
	state->num = root["AI"].asInt();
	state->money[0] = root["moneys"][0].asInt();
	state->score[0] = root["scores"][0].asInt();
	state->money[1] = root["moneys"][1].asInt();
	state->score[1] = root["scores"][1].asInt();
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			state->pollution[i][j] = root["pollution"][i][j].asInt();
		}
	}
	fflush(fp);
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			Json::Value tmp = root["lands"][i][j];
			int x = tmp["pos"][0].asInt();
			int y = tmp["pos"][1].asInt();
			state->lands[x][y].owner = tmp["owner"].asInt();
			state->lands[x][y].occupied = tmp["occupied"].asInt();
			state->lands[x][y].bid = tmp["bid"].asInt();
			state->lands[x][y].bidder = tmp["bidder"].asInt();
			state->lands[x][y].round = tmp["round"].asInt();
			state->lands[x][y].bidOnly = tmp["bidOnly"].asInt();
			state->lands[x][y].filled = tmp["filled"].asInt();
		}
	}
	int sz = root["detectors"].size();
	for(int i = 0; i < sz; ++i){
		Json::Value tmp = root["detectors"][i];
		state->detectors.push_back(
			Detector(tmp["pos"][0].asInt(), tmp["pos"][1].asInt(), 
					 tmp["rangeType"].asInt(), tmp["owner"].asInt()));
	}
	sz = root["processors"].size();
	for(int i = 0; i < sz; ++i){
		Json::Value tmp = root["processors"][i];
		state->processors.push_back(
			Processor(tmp["pos"][0].asInt(), tmp["pos"][1].asInt(),
					  tmp["rangeType"].asInt(), tmp["processingType"].asInt(), 
					  tmp["owner"].asInt()));
	}
	return true;
}
void Client::sendOpt(FILE* fp){
	root.clear();
	if(opt->tipsterX != -1 || opt->tipsterY != -1){
		root["tipster"]["pos"][0] = opt->tipsterX;
		root["tipster"]["pos"][1] = opt->tipsterY;
	}else root["tipster"] = Json::Value();

	if(opt->detectorX != -1 || opt->detectorY != -1 || opt->detectorRangeType != -1){
		root["detector"]["pos"][0] = opt->detectorX;
		root["detector"]["pos"][1] = opt->detectorY;
		root["detector"]["rangeType"] = opt->detectorRangeType;
	}else root["detector"] = Json::Value();

	if(opt->processorX != -1 || opt->processorY != -1 || 
		opt->processorProcessingType != -1 | opt->processorRangeType != -1){
		root["processor"]["pos"][0] = opt->processorX;
		root["processor"]["pos"][1] = opt->processorY;
		root["processor"]["rangeType"] = opt->processorRangeType;
		root["processor"]["processingType"] = opt->processorProcessingType;
	}else root["processor"] = Json::Value();

	if(opt->bidX != -1 || opt->bidY != -1 || opt->bidPrice != -1){
		root["bid"]["pos"][0] = opt->bidX;
		root["bid"]["pos"][1] = opt->bidY;
		root["bid"]["bidPrice"] = opt->bidPrice;
	}else root["bid"] = Json::Value();

	std::string jsonStr = fw.write(root);
	sendLen((int)(jsonStr.length()));
	printf("%s", jsonStr.c_str());
	fflush(stdout);
}
std::string Client::read(){
	char tmp[4];
    std::string msg;
    while(true){
        scanf("%4c", tmp);
        int len = (unsigned int)((((unsigned int)tmp[3]) & 255) |
                                ((((unsigned int)tmp[2]) & 255) << 8) |
                                ((((unsigned int)tmp[1]) & 255) << 16) |
                                ((((unsigned int)tmp[0]) & 255) << 24));
        for (int i = 0; i < len; i ++) msg += getchar();
        break;
    }return msg;
}
void Client::sendLen(int len){
    unsigned char tmp[4];
    tmp[0] = (unsigned char)(len);
    tmp[1] = (unsigned char)(len >> 8);
    tmp[2] = (unsigned char)(len >> 16);
    tmp[3] = (unsigned char)(len >> 24);
    for(int i = 0; i < 4; i++)printf("%c", tmp[3 - i]);
}
