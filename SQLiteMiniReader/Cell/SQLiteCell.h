//
// @Project : SQLiteMiniReader
// @File    : SQLiteCell.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_CELL_H_
#define _SQLITE_CELL_H_

#include "../Tools/SQLiteTools.h"
#include <string.h>

//通过类型判断是否是文本字段
#define IS_TEXT_FIELD(x) ((x) >= 13 && ((x) % 2))
//通过类型判断是否是二进制数据字段
#define IS_BLOB_FIELD(x) ((x) >= 12 && !((x) % 2))

//通过类型判断文本长度
#define TEXT_LENGHT(x) (((UINT)(x) - 13) / 2)
//通过类型判断二进制数据长度
#define BLOB_LENGHT(x) (((UINT)(x) - 12) / 2)

//判断列索引是否有效
#define IS_VALOD_INDEX(x) ((x) >= 0 && (x) < (int)m_ColumnCount)

//判断类型是为为DBNULL
#define IS_DBNULL(x) (!m_Types[x])

//SQLite表类型
enum SQLiteTableType
{
    //未知
    STT_UNKNOWN,
    //数据表
    STT_Table,
    //索引
    STT_Index,
    //触发器
    STT_Trigger,
    //视图
    STT_View
};

//SQLite数据类型
enum SQLiteDataType
{
    //未知
    SDT_UNKNOWN,
    //NULL
    SDT_DBNULL,
    //布尔型
    SDT_BOOLEAN,
    //整型
    SDT_INTEGER,
    //浮点型
    SDT_REAL,
    //文本
    SDT_TEXT,
    //二进制数据
    SDT_BLOB
};

//SQLite单元类
class SQLiteCell
{
protected:
    //使用字节数
    ULONG   m_UseBytes;
    //在当前表中的ID
    ULONG   m_RowID;
    //列数量
    ULONG   m_ColumnCount;
    //类型列表
    ULONG * m_Types;
    //数据列表
    BYTE **     m_Data;

public:
	/************************************************************************
	* 功能：私有构造方法，初始化类。
	* 参数：
	*       data                  [IN] -> 要分析的一个Cell的数据。
	*       rawData               [IN] -> 原始数据，即数据库文件起始数据指
	*                                     针，用于溢出页处理。
	*       pageSize              [IN] -> 数据页大小，用于溢出页处理。
	*       cellSizeInCurrentPage [IN] -> 当前单元在当前页已经占用的长度，
	*                                     用于溢出页处理。
	* 返回：无。
	* 备注：
	*       1.给定的数据必须完整，超过一个Cell的数据会自动忽略(溢出页除外)。
	*       2.对于cellSizeInCurrentPage，可以用当前单元的前一个单元偏移减去
	*         当前单元的偏移，计算而出，对已第一个单元可以用页大小来减。
	************************************************************************/
	SQLiteCell(IN const BYTE * bytes, IN const BYTE * rawData, IN const UINT pageSize, IN const UINT cellSizeInCurrentPage);

    /************************************************************************
    * 功能：析构方法，释放资源。
    * 参数：无。
    * 返回：无。
    ************************************************************************/
    virtual ~SQLiteCell();
};

#endif
