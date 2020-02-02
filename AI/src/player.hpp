#pragma once
#include "../include/client/client.h"

const int dx[3][9] = {{0, 0, 0, 1, -1, 0, 0, 2, -2},
				   {0, 0, 0, 1, -1, 1, 1, -1, -1},
				   {0, 1, 1, -1, -1, 2, 2, -2, -2}};
const int dy[3][9] = {{0, 1, -1, 0, 0, 2, -2, 0, 0},
				   {0, 1, -1, 0, 0, 1, -1, 1, -1},
				   {0, 1, -1, 1, -1, 2, -2, 2, -2}};

int pollutionPosNum;
bool buildingMap[WIDTH][HEIGHT];
bool knowMap[WIDTH][HEIGHT];
std::vector<std::pair<int,int>>control[WIDTH][HEIGHT][MAXRANGENUM];

int stateNum = 0;

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
	opt->setTipster(WIDTH>>1, HEIGHT>>1);
}

