#pragma once
#include "../include/client/client.h"

/*
	常量
*/
const int dx[3][9] = {{0, 0, 0, 1, -1, 0, 0, 2, -2},
				   {0, 0, 0, 1, -1, 1, 1, -1, -1},
				   {0, 1, 1, -1, -1, 2, 2, -2, -2}};
const int dy[3][9] = {{0, 1, -1, 0, 0, 2, -2, 0, 0},
				   {0, 1, -1, 0, 0, 1, -1, 1, -1},
				   {0, 1, -1, 1, -1, 2, -2, 2, -2}};
const int inf = 0x7fffffff;

/*
	参数
*/
const double rate = 0.4;

const double interval1 = 0.9;
const double interval2 = 0.5;
const double speedUpProbability = 0.2;

/*
	初始化
*/
std::vector<std::pair<int,int>>control[WIDTH][HEIGHT][MAXRANGENUM];
bool buildingsMap[WIDTH][HEIGHT];
int expectProfit = 0;
int baseMoney = 0;
FILE *fp;

/*
	回合更新
*/
int stateNum = 0;
int otherStateProfit = 0;
int otherExpectPollution = 0;
int otherLastMoney;
int myMoney;
bool knowMap[WIDTH][HEIGHT];
int initialPollution[WIDTH][HEIGHT];
std::vector<std::pair<int,int>>myLands;

/*
	计算用变量
*/
double bidValueMap[WIDTH][HEIGHT];
double bidMaxValueMap[WIDTH][HEIGHT];
double bidMyValueMap[WIDTH][HEIGHT];
int bidPollutionMap[WIDTH][HEIGHT];

bool checkInMap(int x, int y){
	return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

int getAbs(int x){
	return x > 0? x : -x;
}

int getMax(int x, int y){
	return x > y? x : y;
}

int getDis(int a, int b, int x, int y){
	return getAbs(a-x)+getAbs(b-y);
}

int getProfit(Parameters* parameters, int p){
	int ans = 0;
	for(int i = 0; i < parameters->pollutionComponentNum; ++i){
		if((p>>i)&1)ans += parameters->pollutionProfit[i];
	}return ans;
}

int getBitCount(int bit){
	int ans = 0;
	while(bit){
		if(bit&1)ans++;
		bit >>= 1;
	}return ans;
}

int getLegalLandPrice(double price, int Base){
	return (int)(price/Base)*Base;
}

double getRand(){//[0,1)精确到两位 伪随机
	return (double)(rand()%100)/100.0;
}

void init(Parameters* parameters, State* state){
	srand((unsigned)time(NULL));
	for(int i = 0; i < WIDTH; ++i)for(int j = 0; j < HEIGHT; ++j)buildingsMap[i][j] = false;
	int sz = parameters->buildings.size();
	for(int i = 0; i < sz; ++i){
		buildingsMap[parameters->buildings[i].first][parameters->buildings[i].second] = true;
	}
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			for(int k = 0; k < MAXRANGENUM; ++k){
				for(int index = 0; index < 9; ++index){
					int x = i+dx[k][index];
					int y = j+dy[k][index];
					if(!checkInMap(x, y))continue;
					if(buildingsMap[x][y])continue;
					if(getMax(getAbs(dx[k][index]), getAbs(dy[k][index])) == 2 &&
						buildingsMap[(i+x)>>1][(j+y)>>1])continue;
					control[i][j][k].push_back(std::make_pair(x, y));
				}
			}
		}
	}
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(buildingsMap[i][j])knowMap[i][j] = true;
			else if(state->pollution[i][j])knowMap[i][j] = true;
			else knowMap[i][j] = false;
		}
	}
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			initialPollution[i][j] = state->pollution[i][j];
		}
	}
	otherLastMoney = state->money[1-parameters->num];
	for(int i = 1; i < (1<<parameters->pollutionComponentNum); ++i){
		int tmp = 0;
		for(int j = 0; j < parameters->pollutionComponentNum; ++j){
			if((i>>j)&1)tmp += parameters->pollutionProfit[j];
		}
		expectProfit += tmp;
	}
	expectProfit = (int)(expectProfit/((1<<parameters->pollutionComponentNum)-1));
	int maxRangeCost = -inf;
	int maxTypeCost = -inf;
	for(int i = 0; i < parameters->maxRangeNum; ++i){
		maxRangeCost = getMax(maxRangeCost, parameters->processorRangeCost[i]);
	}
	for(int i = 0; i < parameters->pollutionComponentNum; ++i){
		maxTypeCost = getMax(maxTypeCost, parameters->processorTypeCost[i]);
	}
	baseMoney = maxRangeCost+maxTypeCost;
	maxRangeCost = -inf;
	for(int i = 0; i < parameters->maxRangeNum; ++i){
		maxRangeCost = getMax(maxRangeCost, parameters->detectorRangeCost[i]);
	}
	baseMoney += maxRangeCost;
	return;
}

void updateState(Parameters* parameters, State* state){
	if(state->myDetectorX != -1){
		int x = state->myDetectorX;
		int y = state->myDetectorY;
		int range = state->myDetectorRange;
		int sz = control[x][y][range].size();
		for(int i = 0; i < sz; ++i){
			knowMap[control[x][y][range][i].first][control[x][y][range][i].second] = true;
		}
		sz = state->myDetectorCheckPos.size();
		for(int i = 0; i < sz; ++i){
			x = state->myDetectorCheckPos[i].first;
			y = state->myDetectorCheckPos[i].second;
			initialPollution[x][y] = state->myDetectorCheckPollution[i];
		}
	}

	int centerX = state->tipsterX;
	int centerY = state->tipsterY;
	if(centerX != -1){
		int checkX = state->tipsterCheckX;
		int checkY = state->tipsterCheckY;
		int checkPollution = state->tipsterCheckPollution;
		if(checkX != -1){
			knowMap[checkX][checkY] = true;
			initialPollution[checkX][checkY] = checkPollution;
			int dis = getDis(centerX, centerY, checkX, checkY);
			for(int i = 0; i < WIDTH; ++i){
				for(int j = 0; j < HEIGHT; ++j){
					if(getDis(i, j, centerX, centerY) < dis)knowMap[i][j] = true;
				}
			}
		}else{
			for(int i = 0; i < WIDTH; ++i)for(int j = 0; j < HEIGHT; ++j)knowMap[i][j] = true;
		}
	}

	int sz = state->profitPos.size();
	for(int i = 0; i < sz; ++i){
		int x = state->profitPos[i].first;
		int y = state->profitPos[i].second;
		initialPollution[x][y] = 0;
	}

	otherStateProfit = 0;
	if(state->otherProcessorX != -1){
		int x = state->otherProcessorX;
		int y = state->otherProcessorY;
		int range = state->otherProcessorRange;
		sz = control[x][y][range].size();
		for(int i = 0; i < sz; ++i){
			int controlX = control[x][y][range][i].first;
			int controlY = control[x][y][range][i].second;
			if(initialPollution[controlX][controlY] != 0 && state->pollution[controlX][controlY] == 0){
				initialPollution[controlX][controlY] = 0;
				otherExpectPollution++;
				int profit = 0;
				for(int j = 0; j < parameters->pollutionComponentNum; ++j){
					if((initialPollution[controlX][controlY]>>j)&1)profit += parameters->pollutionProfit[j];
				}
				otherStateProfit += profit;
			}
		}
		int tmp = state->money[1-parameters->num]-otherLastMoney-otherStateProfit;
		if(tmp != 0){
			if(tmp%expectProfit == 0)otherExpectPollution += (tmp/expectProfit);
			else otherExpectPollution += ((int)(tmp/expectProfit)+1);
		}
	}

	myLands.clear();
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(state->lands[i][j].owner == parameters->num && !state->lands[i][j].occupied){
				myLands.push_back(std::make_pair(i, j));
			}
		}
	}

	myMoney = state->money[parameters->num];
	return;
}

void construct(Parameters* parameters, State* state, Operations* opt){
	int sz = myLands.size();
	if(sz == 0)return;
	int ansX = -1;
	int ansY = -1;
	int ansRange = -1;
	int ansType = -1;
	int ansCost = -1;
	double maxProfit = -inf;
	for(int i = 0; i < sz; ++i){
		int x = myLands[i].first;
		int y = myLands[i].second;
		for(int range = 0; range < parameters->maxRangeNum; ++range){
			for(int type = 0; type < parameters->pollutionComponentNum; ++type){
				int cost = parameters->processorRangeCost[range]+parameters->processorTypeCost[type];
				if(myMoney < cost)continue;
				double tmp = (double)(-cost);
				int lim = control[x][y][range].size();
				for(int index = 0; index < lim; ++index){
					int nowX = control[x][y][range][index].first;
					int nowY = control[x][y][range][index].second;
					if(initialPollution[nowX][nowY] == 0)continue;
					if(!((state->pollution[nowX][nowY]>>type)&1))continue;
					double landProfit = (double)(getProfit(parameters, initialPollution[nowX][nowY]));
					int bitCount = getBitCount(state->pollution[nowX][nowY]);
					bitCount--;
					while(bitCount){landProfit *= rate; bitCount--;}
					tmp += landProfit;
				}
				if(maxProfit < tmp){
					maxProfit = tmp;
					ansX = x; ansY = y;
					ansRange = range;
					ansType = type;
					ansCost = cost;
				}
			}
		}
	}
	if(ansX != -1){
		opt->setProcessor(ansX, ansY, ansRange, ansType);
		myMoney -= ansCost;
	}return;
}

void bid(Parameters* parameters, State* state, Operations* opt){
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			//消去自己的bid的影响
		}
	}
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(state->lands[i][j].owner == -1 && state->lands[i][j].bidder == parameters->num){
				myMoney -= state->lands[i][j].bid;
			}
		}
	}

	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			bidValueMap[i][j] = -inf;
			bidMaxValueMap[i][j] = -inf;
			bidMyValueMap[i][j] = -inf;
			for(int range = 0; range < parameters->maxRangeNum; ++range){
				int sz = control[i][j][range].size();
				for(int type = 0; type < parameters->pollutionComponentNum; ++type){
					int cost = parameters->processorRangeCost[range]+parameters->processorTypeCost[type];
					double tmpValue = (double)(-cost);
					double tmpMaxValue = tmpValue;
					double tmpMyValue = tmpValue;
					for(int index = 0; index < sz; ++index){
						int nowX = control[i][j][range][index].first;
						int nowY = control[i][j][range][index].second;
						if(initialPollution[nowX][nowY] == 0)continue;
						if(!((state->pollution[nowX][nowY]>>type)&1))continue;

						double landProfit = parameters->pollutionProfit[type];
						tmpMaxValue += landProfit;

						int bitCount = getBitCount(state->pollution[nowX][nowY]);
						bitCount--;
						while(bitCount){landProfit *= rate; bitCount--;}
						tmpValue += landProfit;

						landProfit = (double)(getProfit(parameters, initialPollution[nowX][nowY]));
						bitCount = getBitCount(state->pollution[nowX][nowY]);
						bitCount--;
						while(bitCount){landProfit *= rate; bitCount--;}
						tmpMyValue += landProfit;
					}
					if(bidValueMap[i][j] < tmpValue)bidValueMap[i][j] = tmpValue;
					if(bidMaxValueMap[i][j] < tmpMaxValue)bidMaxValueMap[i][j] = tmpMaxValue;
					if(bidMyValueMap[i][j] < tmpMyValue)bidMyValueMap[i][j] = tmpMyValue;
				}
			}
		}
	}

	fprintf(fp, "bidValueMap: \n");
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			fprintf(fp, "%.0lf ", bidValueMap[i][j]);
		}fprintf(fp, "\n");
	}
	fprintf(fp, "bidMaxValueMap: \n");
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			fprintf(fp, "%.0lf ", bidMaxValueMap[i][j]);
		}fprintf(fp, "\n");
	}
	fprintf(fp, "bidMyValueMap: \n");
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			fprintf(fp, "%.0lf ", bidMyValueMap[i][j]);
		}fprintf(fp, "\n");
	}

	int otherBidX = -1;
	int otherBidY = -1;
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(state->lands[i][j].owner == -1 && state->lands[i][j].bidOnly == -1){
				if(state->lands[i][j].bidder == 1-parameters->num && state->lands[i][j].round == 1){
					otherBidX = i;
					otherBidY = j;
				}
			}
		}
	}

	fprintf(fp, "myMoney = %d\n", myMoney);
	bool optFlag = false;
	if(otherBidX != -1){
		int otherBidValue = getLegalLandPrice(bidValueMap[otherBidX][otherBidY], (int)(parameters->landPrice*0.1));
		int otherBidMaxValue = getLegalLandPrice(bidValueMap[otherBidX][otherBidY], (int)(parameters->landPrice*0.1));
		int otherBidPrice = state->lands[otherBidX][otherBidY].bid;
		fprintf(fp, "otherBidValue = %d\n", otherBidValue);
		fprintf(fp, "otherBidMaxValue = %d\n", otherBidMaxValue);
		fprintf(fp, "otherBidPrice = %d\n", otherBidPrice);
		if(otherBidPrice < otherBidValue){
			int resultPrice;
			if(getRand() < speedUpProbability)resultPrice = otherBidPrice;
			else resultPrice = otherBidPrice+parameters->landPrice*0.1;
			if(myMoney-resultPrice >= baseMoney){
				optFlag = true;
				myMoney -= resultPrice;
				opt->setBid(otherBidX, otherBidY, resultPrice);
			}
		}else if(otherBidPrice < otherBidMaxValue){
			double p = 1.0;
			int tmp = (otherBidMaxValue-otherBidValue)/(parameters->landPrice*0.1);
			while(tmp > 0){p *= interval1; tmp--;}
			int resultPrice = -1;
			if(getRand() < p)resultPrice = otherBidPrice+parameters->landPrice*0.1;
			if(resultPrice != -1 && myMoney-resultPrice >= baseMoney){
				optFlag = true;
				myMoney -= resultPrice;
				opt->setBid(otherBidX, otherBidY, resultPrice);
			}
		}else{
			double p = 1.0;
			int tmp = (otherBidMaxValue-otherBidValue)/(parameters->landPrice*0.1);
			while(tmp > 0){p *= interval1; tmp--;}
			tmp = (otherBidPrice-otherBidMaxValue)/(parameters->landPrice*0.1);
			while(tmp > 0){p *= interval2; tmp--;}
			int resultPrice = -1;
			if(getRand() < p)resultPrice = otherBidPrice+parameters->landPrice*0.1;
			if(resultPrice != -1 && myMoney-resultPrice >= baseMoney){
				optFlag = true;
				myMoney -= resultPrice;
				opt->setBid(otherBidX, otherBidY, resultPrice);
			}
		}
	}
	if(!optFlag){
		int bidX = -1;
		int bidY = -1;
		int maxAns = -inf;
		for(int i = 0; i < WIDTH; ++i){
			for(int j = 0; j < HEIGHT; ++j){
				if(state->lands[i][j].owner == -1 && state->lands[i][j].bidOnly != 1-parameters->num){
					int baseBidPrice = -1;
					if(state->lands[i][j].bidder == parameters->num)continue;
					if(state->lands[i][j].bidder == -1)baseBidPrice = parameters->landPrice;
					else baseBidPrice = state->lands[i][j].bid+parameters->landPrice*0.1;
					int v = bidMyValueMap[i][j]+(bidValueMap[i][j]-baseBidPrice);
					if(v > maxAns){
						maxAns = v;
						bidX = i;
						bidY = j;
					}
				}
			}
		}
		fprintf(fp, "bidX = %d\n", bidX);
		fprintf(fp, "bidY = %d\n", bidY);
		if(bidX != -1){
			fprintf(fp, "bidPrice = %d\n", state->lands[bidX][bidY].bid);
			int resultPrice = -1;
			if(state->lands[bidX][bidY].bidder == -1)resultPrice = parameters->landPrice;
			else resultPrice = state->lands[bidX][bidY].bid+parameters->landPrice*0.1;
			if(myMoney-resultPrice >= baseMoney){
				if(resultPrice < bidValueMap[bidX][bidY]){
					optFlag = true;
					myMoney -= resultPrice;
					opt->setBid(bidX, bidY, resultPrice);
				}else if(resultPrice < bidMaxValueMap[bidX][bidY]){
					double p = 1.0;
					int tmp = (bidMaxValueMap[bidX][bidY]-bidValueMap[bidX][bidY])/(parameters->landPrice*0.1);
					while(tmp >= 0){p *= interval1; tmp--;}
					if(getRand() < p){
						optFlag = true;
						myMoney -= resultPrice;
						opt->setBid(bidX, bidY, resultPrice);
					}
				}else{
					double p = 1.0;
					int tmp = (bidMaxValueMap[bidX][bidY]-bidValueMap[bidX][bidY])/(parameters->landPrice*0.1);
					while(tmp >= 0){p *= interval1; tmp--;}
					tmp = (resultPrice-bidMaxValueMap[bidX][bidY])/(parameters->landPrice*0.1);
					while(tmp >= 0){p *= interval2; tmp--;}
					if(getRand() < p){
						optFlag = true;
						myMoney -= resultPrice;
						opt->setBid(bidX, bidY, resultPrice);
					}
				}
			}
		}
	}
	fflush(fp);
	return;
}

void getOperations(Parameters* parameters,
				   State* state,
				   Operations* opt,
				   FILE* f){
	/*
	@parameters 
		member variables(public):
			AI编号: int num;                                   
			地图宽度: int mapWidth;
			地图长度: int mapHeight;
			初始地价: int landPrice;
			污染气体数目: int pollutionComponentNum;
			最大回合数: int maxRoundNum;
			覆盖范围种类数: int maxRangeNum;
			情报价格: int tipsterCost;
			建筑物: std::vector<std::pair<int,int>>buildings;
			治理设备各种范围价格: std::vector<int>processorRangeCost;
			治理设备各种类型价格: std::vector<int>processorTypeCost;
			检测设备各种范围价格: std::vector<int>detectorRangeCost;
			污染气体治理收入: std::vector<int>pollutionProfit;
	@state
		member variables(public):
			行动AI编号: int num;
	    	双方钱数: int money[PLAYER];
	    	双方分数: int score[PLAYER];
	    	污染源: int pollution[WIDTH][HEIGHT];
	    	地皮情况: Land lands[WIDTH][HEIGHT];
	    	放置检测设备情况: std::vector<Detector>detectors;
	    	放置治理设备情况: std::vector<Processor>processors;
			
		以下变量如果上两回合无该操作，则对应的容器大小为0或者变量为-1
	    	我方情报贩子的中心点:int tipsterX, tipsterY;
    		我方情报贩子侦测情报位置和污染情况:int tipsterCheckX, tipsterCheckY, tipsterCheckPollution;
    		我方检测设备放置位置和范围类型:int myDetectorX, myDetectorY, myDetectorRange;
    		我方治理设备放置位置和范围类型、气体类型:
    			int myProcessorX, myProcessorY, myProcessorRange, myProcessorType;
    		我方标价位置和标价:int myBidX, myBidY, myBidPrice;
    		对方标价位置和标价:int otherBidX, otherBidY, otherBidPrice;
    		对方检测设备放置位置和范围类型:int otherDetectorX, otherDetectorY, otherDetectorRange;
    		对方治理设备放置位置,范围类型,气体类型:int otherProcessorX, otherProcessorY, otherProcessorRange, otherProcessorType;
    		我方检测设备侦测到的污染源位置:std::vector<std::pair<int,int>>myDetectorCheckPos;
    		我方检测设备侦测到的污染源组成:std::vector<int>myDetectorCheckPollution;
    		我方获得收益的点:std::vector<std::pair<int,int>>profitPos;
	@opt
		member functions(public):
			@x 获取情报的中心位置x坐标,对应mapWidth那一维,下标从0开始,以下同理
			@y 获取情报的中心位置y坐标,对应mapHeight那一维,下标从0开始,以下同理
			void setTipster(int x, int y):设置本回合使用情报的中心位置，如果本回合不使用，请勿调用
			
			@x 放置检测设备的位置x坐标
			@y 放置检测设备的位置y坐标
			@rangeType 放置检测设备的检测范围类型,对应maxRangeNum,下标从0开始,以下同理
    		void setDetector(int x, int y, int rangeType):设置本回合放置的检测设备，如果本回合不使用，请勿调用
    		@x 放置治理设备的位置x坐标
    		@y 放置治理设备的位置y坐标
    		@rangeType 放置治理设备的治理范围类型
    		@processingType 放置治理设备的治理气体类型,对应pollutionComponentNum,下标从0开始
    		void setProcessor(int x, int y, int rangeType, int processingType):设置本回合放置的治理设备，
    		如果本回合不使用，请勿调用
			@x 地皮竞价的位置x坐标
			@y 地皮竞价的位置y坐标
			@bidPrice 本回合对该地皮的报价，要求bidPrice大于上一次报价且为landPrice*0.1的整数倍
    		void setBid(int x, int y, int bidPrice):设置本回合的地皮报价信息，如果本回合不使用，请勿调用
    @覆盖类型:DeltaPos = [
    		[(0,0),(0,1),(0,-1),(1,0),(-1,0),(0,2),(0,-2),(2,0),(-2,0)],     十字
    		[(0,0),(0,1),(0,-1),(1,0),(-1,0),(1,1),(1,-1),(-1,1),(-1,-1)],   区域
    		[(0,0),(1,1),(1,-1),(-1,1),(-1,-1),(2,2),(2,-2),(-2,2),(-2,-2)], 斜十字
		]
	*/
	stateNum++;
	if(stateNum == 1){
		init(parameters, state);
		fp = f;
	}
	updateState(parameters, state);
	construct(parameters, state, opt);
	bid(parameters, state, opt);

	int x, y, range;
	int cnt = 0;
	while(true){
		x = rand()%WIDTH;
		y = rand()%HEIGHT;
		if(!state->lands[x][y].filled)break;
		cnt++;
		if(cnt >= 10)break;
	}
	if(!state->lands[x][y].filled){
		int p = rand()%10;
		if(p > 4){
			range = rand()%parameters->maxRangeNum;
			opt->setDetector(x, y, range);
			myMoney -= parameters->detectorRangeCost[range];
		}
	}

	if(myMoney >= 3000){
		int p = rand()%10;
		if(p > 6)opt->setTipster(WIDTH>>1, HEIGHT>>1);
	}return;
}