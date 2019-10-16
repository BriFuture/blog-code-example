## Interface SD Communicate Protocol

1. registration() -->  客户端发送注册消息

>> Loop Race Leg

2. leg_start() <----  独立子场（leg）开始消息。

>> Loop Round   裁判程序给各参赛选手广播的当前会和的游戏相关的全部信息

3.  round()  <----

4. action()   ---->

<< Loop Round end

5.  leg_end()

<< Loop Race Leg end

6. game_over()

> 注：服务器和客户端启动顺序不确定

### Message

小写字符串。格式：消息长度+消息体

#### registration : 客户端发送的注册消息

```json
00158{"msg_name": "registration", "msg_data": {"team_id": 1000, "team_name": "test_demo"}}
```

#### leg_start: leg 开始消息

裁判程序给各参赛选手广播地图相关信息，以及选手的分组信息。

vision : 默认视野4，即视野范围9*9的方形空间

```json
{
    "msg_name": "leg_start",
    "msg_data": {
        "map": {
            "width": 15,
            "height": 15,
            "vision": 3,
            "meteor": [
                {"x": 1, "y": 1},
                {"x": 1, "y": 4}
            ],
            "tunnel": [
                {"x": 3, "y": 1, "direction": "down"},
                {"x": 3, "y": 4, "direction": "down"}
            ],
            "wormhole": [
                {"name": "a","x": 4, "y": 1},
                {"name": "A", "x": 4, "y": 4 }
            ]

        },
        "teams" : [
            {"id": 1001, "players": [0, 1, 2, 3], "force": "beat"},
            {"id": 1002, "players": [4, 5, 6, 7], "force": "think"},

        ]
    }
}
```


#### round

裁判程序给各参赛选手广播的当前会和的游戏相关的全部信息。

```json
{
    "msg_name": "round",
    "msg_data": {
        "round_id": 2,
        "mode": "beat",
        "power": [
            {"x":5,"y":2, "point":1},
            {"x":5,"y":5, "point":2}
        ],
        "players": [
            {
                "id":0,"score":0,"sleep":0,"team":1001,"x":0,"y":1
            }
        ],
        "teams": [
            {"id":1001,"point":0, "remain_life":2},
            {"id":1002,"point":0, "remain_life":3}

        ]
    }
}
```


#### action

参赛选手反馈在本回合准备执行的动作。每个参赛选手每回合只反馈一条消息，包含所有player的动作请求。

```json
{
    "msg_name": "action",
    "msg_data": {
        "round_id": 2,
        "actions": [
            {"team": 1002, "player_id": 5, "move": ["up"]}
        ]
    }
}
```

#### leg_end

```json
{
    "msg_name" : "leg_end",
    "msg_data" : {
      "teams" : [
         {
            "id" : 1001,				
            "point" : 770             
         },
         {
            "id" : 1002,
            "point" : 450
         }
      ]
   }

}
```

#### game_over

```json
{
   "msg_name" : "game_over"
}
```