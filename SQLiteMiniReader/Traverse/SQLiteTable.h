//
// @Project : SQLiteMiniReader
// @File    : SQLiteTable.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_TABLE_H_
#define _SQLITE_TABLE_H_

#include <string.h>
#include "../Cell/SQLiteTableCell.h"
#include "../Tools/SQLiteTools.h"

//判断是否是空白字符(换行符、回车符、制表符、空格)
#define IS_SPACE_CHAR(x) ((x) == '\r' || (x) == '\n' || (x) == '\t' || (x) == ' ')

//判断是否是SQL中的分割字符(',' , '(' , ')')
#define IS_SPLITE_CHAR(x) ((x) == ',' || (x) == '(' || (x) == ')')


//以下宏主要是通过比对关键字判断，其中有空格是因为为了防止有字段名重复

//判断是否是主键字段
#define IS_PRIMARY_KEY_FIELD(x) (strstr((x), "primary key") != NULL)
//判断是否是自增字段
#define IS_AUTOINCREMENT_FIELD(x) (strstr((x), " autoincrement") != NULL)
//判断是否是非空字段
#define IS_NOT_NULL_FIELD(x) (strstr((x), " not null") != NULL)
//判断是否是唯一键值字段
#define IS_UNIQUE_FIELD(x) (strstr((x), " unique ") != NULL)
//判断是否是外键语句
#define IS_FOREIGN_KEY_SETENCE(x) (strstr((x), "foreign key") != NULL)
//判断是否是联合主键语句
#define IS_CONSTRAINT_SETENCE(x) (strstr((x), "constraint ") != NULL)
//判断是否是唯一键值语句
#define IS_UNIQUE_SETENCE(x) (!_strnicmp((x), "unique ", 7) || !_strnicmp((x), "unique(", 7))


//字段属性
enum SQLiteFeildProperty
{
    //没有指定的属性
    SFP_NOP = 0,
    //主键
    SFP_PRIMARY_KEY = 1,
    //自增
    SFP_AUTOINCREMENT = 2,
    //非空
    SFP_NOT_NULL = 4,
    //外键
    SFP_FOREIGN_KEY = 8,
    //唯一
    SFP_UNIQUE_KEY = 16
};


typedef struct _SqlFeild
{
    //字段名称
    char * FieldName;
    //类型名称
    char * TypeName;
    //是否不允许NULL值
    BOOL NotNull;
    //是否是主键
    BOOL IsPrimaryKey;
    //是否是自增
    BOOL IsAutoincrementKey;
    //是否是外键
    BOOL IsForeignKey;
    //是否是唯一
    BOOL IsUniqueKey;
    //初始化
    _SqlFeild()
    {
        FieldName = NULL;
        TypeName = NULL;
        NotNull = FALSE;
        IsPrimaryKey = FALSE;
        IsAutoincrementKey = FALSE;
        IsForeignKey = FALSE;
        IsUniqueKey = FALSE;
    }
}
//SQL创建语句的字段属性
SqlFeild;

class SQLiteMiniReader;
class SQLiteRecord;

//SQL语句的创建对象
class SQLiteTable
{
private:
    //SQL创建语句的字段属性
    SqlFeild * m_Fields;
    //字段数量
    UINT m_FieldCount;
	//表名
	char * m_TableName;

	//为了调用私有构造，声明SQLiteReader为友元
	friend SQLiteMiniReader;
	friend SQLiteRecord;

private:
	/************************************************************************
	* 功能：构造方法，初始化类。
	* 参数：
	*       tableCell [IN] -> 指定表的单元。
	* 返回：无。
	************************************************************************/
	SQLiteTable(IN SQLiteTableCell * tableCell);

	/************************************************************************
	* 功能：析构方法，释放数据。
	* 参数：无。
	* 返回：无。
	************************************************************************/
	~SQLiteTable();

    /************************************************************************
    * 功能：格式化SQL语句，转换为小写，去除其中的重复空格，删除制表和换行符
    *       这类空白字符。
    * 参数：无。
    *       sqlString [IN_OUT] -> 要格式化的SQL语句。
    * 返回：无。
    * 备注：此操作是在原字符串的基础上进行直接修改，会影响原字符串。
    *       对于使用单引号区分大小写的部分不会转换为小写。
    ************************************************************************/
    void FormatSql(IN_OUT char * sqlString);

    /************************************************************************
    * 功能：初始化指定索引的字段属性。
    * 参数：
    *       index    [IN] -> 指定初始化字段属性的索引。
    *       feildSql [IN] -> 用于初始化的SQL语句片段。
    * 返回：无。
    * 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
    ************************************************************************/
    void InitSqlFeild(IN int index, IN const char * feildSql);

    /************************************************************************
    * 功能：处理主键语句设置相应的字段属性。
    * 参数：
    *       setence [IN] -> 主键语句。
    * 返回：已经设置的字段属性的数量。
    * 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
    ************************************************************************/
    int HandlePrimaryKeySetence(IN const char * setence);

    /************************************************************************
    * 功能：处理外键语句设置相应的字段属性。
    * 参数：
    *       setence [IN] -> 外键语句。
    * 返回：当前语句中的外键是否设置成功。
    * 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
    ************************************************************************/
    BOOL HandleForeignKeySetence(IN const char * setence);

    /************************************************************************
    * 功能：处理唯一键值语句设置相应的字段属性。
    * 参数：
    *       setence [IN] -> 唯一键值语句。
    * 返回：已经设置的字段属性的数量。
    * 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
    ************************************************************************/
    BOOL HandleUniqueKeySetence(IN const char * setence);

public:
	/************************************************************************
	* 功能：获取表名。
	* 参数：无。
	* 返回：表名。
	* 备注：返回值不可修改，也不用单独释放。
	************************************************************************/
	const char * GetTableName();

    /************************************************************************
    * 功能：获取字段数量。
    * 参数：无。
    * 返回：字段数量。
    ************************************************************************/
    UINT GetFieldCount();

    /************************************************************************
    * 功能：获取指定字段的字段名称。
    * 参数：
    *       index [IN] -> 指定字段的索引。
    * 返回：字段名称的字符串。
    * 备注：返回值不可修改，也不用单独释放。
    ************************************************************************/
    const char * GetFieldName(IN UINT index);

    /************************************************************************
    * 功能：获取指定字段的字段类型。
    * 参数：
    *       index [IN] -> 指定字段的索引。
    * 返回：字段类型的字符串。
    * 备注：返回值不要修改，也不要单独释放。
    ************************************************************************/
    const char * GetTypeName(IN UINT index);

    /************************************************************************
    * 功能：获取指定字段在SQLite中的近似类型。
    * 参数：
    *       index [IN] -> 指定字段的索引。
    * 返回：指定字段在SQLite中的近似类型。
    ************************************************************************/
    SQLiteDataType GetSimilarType(IN UINT index);

    /************************************************************************
    * 功能：获取指定字段的属性。
    * 参数：
    *       index      [IN] -> 指定字段的索引。
    *       properties [IN] -> 要检查的属性(除SFP_NOP外可以使用'|'连接)。
    * 返回：指定的属性存在返回TRUE，只要有一个不存在就返回FALSE。
    * 注意：这里properties的属性传递SQLiteFeildProperty类型即可。
    ************************************************************************/
    BOOL GetFeildProperty(IN UINT index, IN int properties);
};

#endif