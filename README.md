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
