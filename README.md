# SoftChat
一个简陋粗暴的IM程序，用Qt写的，Tcp长连接服务器，使用信号和信号槽的连接方式使得每个类之间拿到实例的机会少之又少，但是这么写出来给我的感觉代码的耦合度略高，但是像其他的Chat项目中使用的全局变量就各个地方分割的很明确，看来有时候降低耦合度也会增加代码的冗余程度。
1. MySQL数据库 5.7.19-log MySQL Community Server，vs2015，Qt5.9.1 32位。
2. 在Server项目中的server.cpp的构造函数中有一个连接本地（127.0.0.1）一个叫做softchat的MySQL数据库的操作，所以如果想要连接自己的数据库的话，那里应该要改一下。
3. 本地使用的MySQL数据库中有一个叫做acount的表，有三个字段，用来存储用户的id，name（昵称）和password（密码），类型都是char，长度分别是10，50，50。其他的表都是程序自动生成的，friend(id)的表就表示拥有这个id的用户所拥有的好友的id，用来请求好友列表。
4. 客户端在一开始的时候连接的是本地的地址，因为本地开发服务器跑在本地上，如果要连接到外面的服务器的话，要修改一下data.cpp里的init函数里的socket的连接操作。
5. 然后就是这个项目很简陋，很不具有实用性，仅拿来练手。顺便说一句，Qt的信号槽机制的确非常好用。
6. 项目中并没有包含相应的Qt库，所以如果要运行代码的话，要自己的配vs的Qt库。
