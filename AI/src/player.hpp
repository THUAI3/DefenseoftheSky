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
	估值参数
*/
const int isDetected = 54;//探索过的地方的估值参数

/*
	初始确定
*/
int pollutionPosNum;//总污染源
bool buildingMap[WIDTH][HEIGHT];//建筑物地图
std::vector<std::pair<int,int>>control[WIDTH][HEIGHT][MAXRANGENUM];//范围控制点

/*
	回合更新
*/
int stateNum = 0;
bool knowMap[WIDTH][HEIGHT];//可以确定是否有污染源的点
bool filledMap[WIDTH][HEIGHT];//是否可放置检测设备,true代表不可放置

/*
	计算用
*/
int valueMap[WIDTH][HEIGHT][MAXRANGENUM];//估值地图 
int tmpMap[WIDTH][HEIGHT];//临时变量地图


int getMax(int x, int y){
	return x > y? x : y;
}

int getAbs(int x){
	return x >= 0? x: -x;
}

bool checkInMap(int x, int y){
	return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

void init(Parameters* parameters, State* state){
	srand((unsigned)time(NULL));
	for(int i = 0; i < WIDTH; ++i)for(int j = 0; j < HEIGHT; ++j)buildingMap[i][j] = false;
	int sz = parameters->buildings.size();
	for(int i = 0; i < sz; ++i){
		buildingMap[parameters->buildings[i].first][parameters->buildings[i].second] = true;
	}

	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			for(int k = 0; k < MAXRANGENUM; ++k){
				for(int index = 0; index < 9; ++index){
					int x = i+dx[k][index];
					int y = j+dy[k][index];
					if(!checkInMap(x, y))continue;
					if(buildingMap[x][y])continue;
					if(getMax(getAbs(dx[k][index]), getAbs(dy[k][index])) == 2 && 
						buildingMap[(i+x)>>1][(i+y)>>1])continue;
					control[i][j][k].push_back(std::make_pair(x, y));
				}
			}
		}
	}

	for(int i = 0; i < WIDTH; ++i)for(int j = 0; j < WIDTH; ++j)knowMap[i][j] = false;
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(state->pollution[i][j])knowMap[i][j] = true;
			if(buildingMap[i][j])knowMap[i][j] = true;
		}
	}
	return;
}

void updateState(State* state){
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(state->lands[i][j].filled)filledMap[i][j] = true;
			else filledMap[i][j] = false;
		}
	}
	return;
}

void calculateDetectorValue(){
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(knowMap[i][j])tmpMap[i][j] = isDetected;
			else tmpMap[i][j] = 0;
		}
	}
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(filledMap[i][j])continue;
			for(int k = 0; k < MAXRANGENUM; ++k){
				int sz = control[i][j][k].size();
				for(int index = 0; index < sz; ++index){
					int x = control[i][j][k][index].first;
					int y = control[i][j][k][index].second;
					if(!knowMap[x][y])tmpMap[x][y]++;
				}
			}
		}
	}

	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(filledMap[i][j])continue;
			for(int k = 0; k < MAXRANGENUM; ++k){
				valueMap[i][j][k] = 0;
				int sz = control[i][j][k].size();
				for(int index = 0; index < sz; ++index){
					int x = control[i][j][k][index].first;
					int y = control[i][j][k][index].second;
					valueMap[i][j][k] += tmpMap[x][y];
				}
			}
		}
	}
	return;
}

bool checkDetector(int x, int y, int range){
	int sz = control[x][y][range].size();
	int controlNum = 0;
	for(int i = 0; i < sz; ++i){
		int controlX = control[x][y][range][i].first;
		int controlY = control[x][y][range][i].second;
		if(!knowMap[controlX][controlY])controlNum++;
	}
	double probability = 1.0;
	return false;
}

void detectorOperation(Parameters* parameters,
					   State* state,
					   Operations *opt){
	calculateDetectorValue();
	int detectorX = -1;
	int detectorY = -1;
	int detectorRange = -1;
	int minValue = inf;
	for(int i = 0; i < WIDTH; ++i){
		for(int j = 0; j < HEIGHT; ++j){
			if(filledMap[i][j])continue;
			for(int k = 0; k < MAXRANGENUM; ++k){
				if(minValue > valueMap[i][j][k]){
					minValue = valueMap[i][j][k];
					detectorY = i;
					detectorY = j;
					detectorRange = k;
				}
			}
		}
	}
	if(detectorX == -1)return;
	if(checkDetector(detectorX, detectorY, detectorRange)){
		opt->setDetector(detectorX, detectorY, detectorRange);
	}return;
}

void getOperations(Parameters* parameters,
				   State* state,
				   Operations *opt){
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
	if(stateNum == 1)init(parameters, state);
	updateState(state);
	detectorOperation(parameters, state, opt);
	opt->setTipster(WIDTH >> 1, HEIGHT >> 1);
}


