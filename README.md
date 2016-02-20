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
- 通过调用QueryTable遍历所有的数据表。
- 通过调用QueryRecord遍历指定表下的所有数据。
- 例子：

		#include "../SQLiteMiniReader/SQLiteMiniReader.h"

		BOOL ProcTable(SQLiteTable * table, void * pVoid)
		{
			cout << "GetTableName : " << table->GetTableName() << endl;
			return TRUE;
		}

		BOOL ProcCell(SQLiteRecord * record, void * pVoid)
		{
			cout << "Record count : " << record->GetFieldCount() << endl;
			for (UINT i = 0; i < record->GetFieldCount(); ++i)
			{
				cout<<record->GetFieldName(i) << "(" << record->GetFeildTypeName(i) << ") : ";

				switch (record->GetFeildSimilarType(i))
				{
					case SDT_BOOLEAN:
						cout << record->GetBoolean(i);
						break;
					case SDT_INTEGER:
						cout << record->GetInteger(i);
						break;
					case SDT_REAL:
						cout << record->GetReal(i);
						break;
					case SDT_TEXT:
					{
						char * tmpChars = NULL;
						cout << "(" << record->GetText(i, tmpChars) << ")";
						if (tmpChars)
						{
							cout << tmpChars;
							delete[] tmpChars;
						}
						break;
					}
					case SDT_BLOB:
					{
						BYTE * tmpBytes = NULL;
						cout << "(" << record->GetBLOB(i, tmpBytes) << ")BLOB";
						if (tmpBytes)
						{
							delete[] tmpBytes;
						}
						break;
					}
					case SDT_DBNULL:
						cout << "SDT_DBNULL";
						break;
					case SDT_UNKNOWN:
						cout << "SDT_UNKNOWN";
						break;
				}
				cout << endl;
			}
			cout << "RowID : " << record->GetRowID() << endl;
			return TRUE;
		}

		int main()
		{
			//打开SQLite数据库test.db
			SQLiteMiniReader sqliteReader("test.db");
			//通过回调函数ProcTable遍历所有的表（忽略视图、索引等）。
			sqliteReader.QueryTable(ProcTable, NULL);
			//通过回调函数ProcCell遍历表"test_table1"所有的数据。
			sqliteReader.QueryRecord("test_table1", ProcCell, NULL);
			return 0;
		}

##注意
- 因为像NULL这类的宏在SQLiteTools中重新定义，可能在部分编译器编译下出现警告。
