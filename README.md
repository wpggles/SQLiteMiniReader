# SQLiteMiniReader
A mini reader of query(traverse) SQLite3 data from database file.

---

##概述
- SQLiteMiniReader 是一个简易的SQLite数据库读取(遍历)的类。
- 按照SQLite3的数据结构，使用C++实现，可以在Windows、Linux、MacOS上编译。
- 目标实现一个超小型的SQLite遍历，不包括增删改，针对数据表遍历，忽略对索引、视图等。
- 不包含sql语句处理，只是通过回调函数遍历所有数据，使用简单。
- 每个方法和属性都有比较完整的注释，觉得不好的地方自己改吧。

##使用
- 参考Test中的测试工程，先阅读其中的ReadMe.txt。

##注意
- 因为像NULL这类的宏在SQLiteTools中重新定义，可能在部分编译器编译下出现警告。
- 表名、字段名除非在建表语句中使用单引号引起来，否则全部转换为小写字符处理，在比较字段名时要注意。

##修复BUG：
2016-04-24  
[+]增加获取表数量的方法。  
[+]增加获取指定表的数据数量的方法。  
[*]修改方法名GetFeildProperty为CheckFeildProperty。  
[*]更新测试工程和类结构图。

2016-04-22  
[*]修正因为Linux下与Windows下部分函数名不同造成的报错，以及NULL相关的警告。

2016-04-21  
[*]修复二进制数据（BLOB类型）读取的数据缺失，数据长度不正确的BUG。