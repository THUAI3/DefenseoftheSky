from Parameters import *
import sys
import json
import time
import shlex
import subprocess

detectors = []
processors = []

subpro = []

errMsg = ['','']

log = {}
logPerRound = []

# DEBUG

def convertByte(jsonStr):
    msgLen = len(jsonStr)
    msg = msgLen.to_bytes(4, byteorder='big', signed=True)
    msg += bytes(jsonStr, encoding="utf8")
    return msg

def sendMsg(jsonStr, goal):
    jsonObj = json.loads(jsonStr)
    #print("======== Send To %d ========" % jsonObj['AI'])
    #print(json.dumps(jsonObj, sort_keys=True, indent=4, separators=(',', ': ')))
    #print("============================")
    #print("goal = %d" % goal)
    subpro[goal].stdin.buffer.write(convertByte(jsonStr))
    subpro[goal].stdin.buffer.flush()

def receiveMsg(AI):
    readBuffer = subpro[AI].stdout.buffer
    dataLen = int.from_bytes(readBuffer.read(4), byteorder='big', signed=True)
    data = readBuffer.read(dataLen)
    return json.loads(data)

def logInitState():
    initState = {
        'mapWidth': MapWidth,
        'mapHeight': MapHeight,
        'landPrice': LandPrice,
        'buildings': [], # [(x0,y0),(x1,y1),...]
        'pollutionComponentNum':PollutionComponentNum,
        'maxRoundNum': MaxRound,
        'maxRangeNum': MaxRangeNum,
        'processorRangeCost': ProcessorRangeCost,
        'processorTypeCost':ProcessorTypeCost,
        'detectorRangeCost':DetectorRangeCost,
        'tipsterCost': TipsterCost,
        'pollutionProfit': PollutionProfit,
        'pollutionMap': PollutionMap.tolist(),
        'pollutionMap0': PollutionMap0.tolist(),
        'pollutionMap1': PollutionMap1.tolist(),
        'scores': Scores,
        'moneys': Moneys,
    }
    errMsg = ['','']
    for i in range(MapWidth):
        for j in range(MapHeight):
            if BuildingMap[i][j]:
                initState['buildings'].append((i,j))

    log[-1] = initState

def sendInitState(AI):
    initState = {
        'mapWidth': MapWidth,
        'mapHeight': MapHeight,
        'landPrice': LandPrice,
        'buildings': [], # [(x0,y0),(x1,y1),...]
        'pollutionComponentNum':PollutionComponentNum,
        'maxRoundNum': MaxRound,
        'maxRangeNum': MaxRangeNum,
        'processorRangeCost': ProcessorRangeCost,
        'processorTypeCost':ProcessorTypeCost,
        'detectorRangeCost':DetectorRangeCost,
        'tipsterCost': TipsterCost,
        'pollutionProfit': PollutionProfit,
        'AI': AI,
        'errMsg':errMsg[AI],
    }
    errMsg[AI] = ''
    for i in range(MapWidth):
        for j in range(MapHeight):
            if BuildingMap[i][j]:
                initState['buildings'].append((i,j))
    sendMsg(json.dumps(initState), AI)

def sendRoundState(AI):
    pollutionM = PollutionMap0 if AI == 0 else PollutionMap1
    state = {
        'AI': AI,
        'pollution': pollutionM.tolist(),
        'lands': [], #所有的地皮的详细信息
        'moneys': Moneys,
        'scores': Scores,
        'detectors': [], # 所有场内的检测设备
        'processors': [], # 所有场内的处理设备
        'errMsg':errMsg[AI],
    }
    errMsg[AI] = ''

    print(Moneys)

    for i in range(MapWidth):
        state['lands'].append([])
        for j in range(MapHeight):
            state['lands'][i].append(Lands[i][j].toJsonObj())

    for detector in detectors:
        state['detectors'].append(detector.toJsonObj())

    for processor in processors:
        state['processors'].append(processor.toJsonObj()) 
        
    sendMsg(json.dumps(state), AI)

def validate(msg,AI):
    # msg = {
    #     'detector':{
    #         'pos':(x,y),
    #         'rangeType':0/1/2,
    #     }, 
    #     'tipster':{
    #         'pos':(x,y),
    #     },
    #     'bid':{
    #         'pos':(x,y),
    #         'bidPrice': money,
    #     },
    #     'processor':{
    #         'pos':(x,y),
    #         'rangeType':0/1/2,
    #         'processingType':0/1/2..,
    #     },
    # }
    try:
        msgObj = json.loads(msg)
    except:
        errMsg[AI] = "json can't decode msg str"
        return False
    
    try:
        assert 'detector' in msgObj, "detector operation not found"
        assert (msgObj['detector'] is None) or ('pos' in msgObj['detector'] and 'rangeType' in msgObj['detector']), 'detector operation invalid'
        assert 'tipster' in msgObj, "tipster operation not found"
        assert (msgObj['tipster'] is None) or ('pos' in msgObj['tipster']), 'tipster operation invalid'
        assert 'bid' in msgObj, "bid operation not found"
        assert (msgObj['bid'] is None) or ('pos' in msgObj['bid'] and 'bidPrice' in msgObj['bid']), 'bid operation invalid'
        assert 'processor' in msgObj, "processor operation not found"
        assert (msgObj['processor'] is None) or ('pos' in msgObj['processor'] and 'rangeType' in msgObj['processor'] and 'processingType' in msgObj['processor']), 'processing operation invalid' 
    except AssertionError as Argument:
        errMsg[AI] = Argument
        return False

    try:
        if not msgObj['detector'] is None:
            assert type(msgObj['detector']['pos']) == type([]), 'detector pos invalid'
            assert len(msgObj['detector']['pos']) == 2, 'detector pos invalid'
            assert type(operation['detector']['pos'][0]) == type(0), "detector pos invalid"
            assert type(operation['detector']['pos'][1]) == type(0), "detector pos invalid"
            assert type(msgObj['detector']['rangeType']) == type(0), 'detector rangeType invalid'
        if not msgObj['tipster'] is None:
            assert type(msgObj['tipster']['pos']) == type([]), 'tipster pos invalid'
            assert len(msgObj['tipster']['pos']) == 2, 'tipster pos invalid'
            assert type(operation['tipster']['pos'][0]) == type(0), "tipster pos invalid"
            assert type(operation['tipster']['pos'][1]) == type(0), "tipster pos invalid"
        
        if not msgObj['bid'] is None:
            assert type(msgObj['bid']['pos']) == type([]), 'bid pos invalid'
            assert len(msgObj['bid']['pos']) == 2, 'bid pos invalid'
            assert type(operation['bid']['pos'][0]) == type(0), "bid pos invalid"
            assert type(operation['bid']['pos'][1]) == type(0), "bid pos invalid"
            assert type(msgObj['bid']['bidPrice']) == type(0), 'bid bidPrice invalid'
        if not msgObj['processor'] is None:
            assert type(msgObj['processor']['pos']) == type([]), 'processor pos invalid'
            assert len(msgObj['processor']['pos']) == 2, 'processor pos invalid'
            assert type(operation['processor']['pos'][0]) == type(0), "processor pos invalid"
            assert type(operation['processor']['pos'][1]) == type(0), "processor pos invalid"
            assert type(msgObj['processor']['rangeType']) == type(0), 'processor rangeType invalid'
            assert type(msgObj['processor']['processingType']) == type(0), 'processor processingType invalid'        
    except AssertionError as Argument:
        errMsg[AI] = Argument
        return False
    return True

def construct(operation,AI):
    if not operation['processor'] is None:
        pos = operation['processor']['pos']
        rangeType = operation['processor']['rangeType']
        processingType = operation['processor']['processingType']
        if 0<=pos[0] and pos[0] <= MapWidth-1 and 0<=pos[1] and pos[1] <= MapHeight-1:
            land = Lands[pos[0]][pos[1]]
            if land.owner == AI and not land.occupied:
                if 0<=rangeType and rangeType< MaxRangeNum and processingType and processingType<PollutionComponentNum:
                    cost = ProcessorRangeCost[rangeType] + ProcessorTypeCost[processingType]
                    if Moneys[AI] >= cost:
                        Moneys[AI] -= cost
                        land.occupied = True
                        processors.append(Processor(tuple(pos),rangeType,processingType,AI))
                        logPerRound.append((1,AI,tuple(pos),rangeType,processingType))

def bid(operation, AI):
    if not operation['bid'] is None:
        pos = operation['bid']['pos']
        bidPrice = operation['bid']['bidPrice']
        if 0<=pos[0] and pos[0] <= MapWidth-1 and 0<=pos[1] and pos[1] <= MapHeight-1:
            land = Lands[pos[0]][pos[1]]
            if land.owner == -1 and (land.bidOnly == AI or land.bidOnly == -1) and land.bidder != AI and bidPrice > land.bid and bidPrice % int(0.1*LandPrice) == 0:
                land.bid = bidPrice
                land.bidder = AI
                land.round = 6
                logPerRound.append((2,AI,tuple(pos),bidPrice))
                
                
def tipster(operation, AI):
    if not operation['tipster'] is None:
        pos = operation['tipster']['pos']
        pM = PollutionMap0 if AI==0 else PollutionMap1
        if Moneys[AI] >= TipsterCost:
            Moneys[AI] -= TipsterCost
        else:
            return
        if np.sum(PollutionMap-pM) == 0:
            return
        
        deltaM = PollutionMap - pM
        tmp = []
        for i in range(MapWidth):
            for j in range(MapHeight):
                if deltaM[i][j] > 0:
                    tmp.append((i,j))
        tmp.sort(key=lambda x: abs(x[0]-pos[0])+abs(x[1]-pos[1]))
        pM[tmp[0][0]][tmp[0][1]] = PollutionMap[tmp[0][0]][tmp[0][1]]
        logPerRound.append((3, AI, tuple(pos)))
        logPerRound.append((4, AI, tuple(tmp[0])))

def launch(operation, AI):
    if not operation['detector']:
        pos = operation['detector']['pos']
        rangeType = operation['detector']['rangeType']
        if 0<=pos[0] and pos[0] <= MapWidth-1 and 0<=pos[1] and pos[1] <= MapHeight-1:
            land = Lands[pos[0]][pos[1]]
            if not land.filled:
                if 0<=rangeType and rangeType< MaxRangeNum:
                    cost = DetectorRangeCost[rangeType]
                    if Moneys[AI] >= cost:
                        Moneys[AI] -= cost
                        detectors.append(Detector(pos,rangeType,AI))
                        land.filled = True
                        pM = PollutionMap0 if AI == 0 else PollutionMap1
                        deltaM = PollutionMap - pM
                        
                        logPerRound.append((6, AI, tuple(pos)))

                        for i in range(MapWidth):
                            for j in range(MapHeight):
                                if deltaM[i][j] > 0 and detectors[-1].cover((i,j)):
                                    pM[i][j] = PollutionMap[i][j]
                                    logPerRound.append((7, AI, (i,j)))
                                
                            
        
def treatPollution(AI):
    pM = PollutionMap0 if AI == 0 else PollutionMap1
    for i in range(MapWidth):
        for j in range(MapHeight):
            if pM[i][j] > 0:
                components = 0
                for processor in processors:
                    if processor.cover((i,j)):
                        components |= 1 << processor.processingType
                if (components & pM[i][j]) == pM[i][j]:
                    profit = LandPrice
                    for k in range(PollutionComponentNum):
                        if pM[i][j] & (1<<k) == (1<<k):
                            profit += PollutionProfit[k]
                    Moneys[AI] += profit
                    Scores[AI] += profit
                    PollutionMap0[i][j] = 0
                    PollutionMap1[i][j] = 0
                    PollutionMap[i][j] = 0
                    logPerRound.append((10,AI,(i,j),profit))
                
def bidUpdate():
    for i in range(MapWidth):
        for j in range(MapHeight):
            land = Lands[i][j]
            if land.owner == -1 and (land.bidder == 0 or land.bidder == 1):
                land.round -= 1
                logPerRound.append((7,(i,j),land.round))
                if land.round == 0:
                    if Moneys[land.bidder] >= land.bid:
                        Moneys[land.bidder] -= land.bid
                        land.owner = land.bidder
                        logPerRound.append((8,(i,j),land.owner,land.bid))
                    else:
                        logPerRound.append((9,(i,j),land.bidder))
                        land.bidOnly = 1-land.bidder
                        land.bidder = -1
                        land.bid = LandPrice-1
                        

def endGame():
    if np.sum(PollutionMap) == 0:
        return True
    money = max(Moneys)
    tmp = [TipsterCost, min(DetectorRangeCost), min(ProcessorRangeCost) + min(ProcessorTypeCost)]
    if money < min(tmp):
        return True
    return False                                              


def main():
    global log,logPerRound,subpro
    #启动进程
    try:
        subpro.append(subprocess.Popen(shlex.split(sys.argv[1]), stdout=subprocess.PIPE,\
                stdin=subprocess.PIPE, universal_newlines=True))
        subpro.append(subprocess.Popen(shlex.split(sys.argv[2]), stdout=subprocess.PIPE,\
                stdin=subprocess.PIPE, universal_newlines=True))
    except Exception as e:
        for pro in subpro:
            try:
                pro.terminate()
            except:
                pass
        print(e)
        sys.exit()
    # 初始化，给出地图信息
    sendInitState(0)
    sendInitState(1)

    logInitState()    

    for round in range(MaxRound):
        AI = round % 2
        # 向AI发送当前的局面信息
        sendRoundState(AI)
        # 等待并接收AI的操作信息
        time.sleep(2)
        # 执行AI的操作消息
        msg = receiveMsg(AI)

        if validate(msg,AI):
            # 治理设备建造操作结算
            construct(msgObj,AI)

            # 地皮标记操作结算
            bid(msgObj,AI)

            
            # 设置新的检测设备
            launch(msgObj,AI)

            # 使用技能操作结算
            tipster(msgObj,AI)
    
        # 地皮竞拍结果结算
        bidUpdate()
        
        # 污染源治理结算
        treatPollution(AI)
        
        log[round] = logPerRound
        logPerRound = []

        # 判断游戏是否结束
        if endGame():
            break

    # 保存录像文件


if __name__ == '__main__':
    # 配置环境等等初始化放在这里
    main()