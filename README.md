# CS1950UStencil

Todo:

Game

Enemy

- [x] chasing enemy - pathfinding
- [x] shooting enemy - shoot player within a range? 直线或是抛物线？

Time

- [x] 60 seconds
- [ ] score if survive 60 seconds

Health

- [x] health for player
- [ ] health pack maybe?

Pathfinding

- [ ] funnel algorithm



Engine

collision

- [x] layer



map generator?

在一定范围内，创建一些长方体，其中一个房间是出生房间，另一个房间是逃离的房间，只要主角逃出就算胜利。地图中会随机分布一些enemy。

simple version

所有房间都在同一高度，房间的高度也完全相同。



需要的信息：

（1）game world的大小

（2）每个房间的最小空间（长宽至少需要多少）

（3）需要多少enemy

进行的操作（game feature？）

（1）按xz轴分空间，分出不同的空间。构成一棵树

（2）在每个空间里放房间

（3）连接相邻节点，把dungeon连接起来



bug

collision normal check
