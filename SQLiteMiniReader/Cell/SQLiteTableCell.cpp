//
// @Project : SQLiteMiniReader
// @File    : SQLiteTableCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTableCell.h"

/************************************************************************
* 功能：获取指定字段的文本型数据。
* 参数：
*       index     [IN]  -> 指定的字段索引。
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：文本长度。
* 注意：bufferPtr 只需要一个字符串指针，函数内申请空间，使用完要释放掉。
************************************************************************/
UINT SQLiteTableCell::GetText(IN int index, OUT char * & bufferPtr)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        bufferPtr = NULL;
        return 0;
    }
    UINT len = TEXT_LENGHT(m_Types[index]);
    if (len == 0)
    {
        bufferPtr = NULL;
        return 0;
    }
    bufferPtr = new char[len + 1];
    memcpy(bufferPtr, m_Data[index], len);
    bufferPtr[len] = 0;
    return len;
}

/************************************************************************
* 功能：获取指定字段的整数型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：转换后的整数。
************************************************************************/
LONG SQLiteTableCell::GetInteger(IN int index)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        return 0;
    }
    int len = (int)m_Types[index];
    if (len >= 8 || len <= 0)
    {
        return len == 9 ? 1 : 0;
    }
    switch (len)
    {
    case 6:
        len += 1;
    case 5:
        len += 1;
    }
    return SQLiteTools::BigEndianNumber(m_Data[index], len);
}

/************************************************************************
* 功能：获取当前Table的ID。
* 参数：无。
* 返回：当前Table的ID。
************************************************************************/
ULONG SQLiteTableCell::GetTableID()
{
    return m_RowID;
}

/************************************************************************
* 功能：获取当前Table类型。
* 参数：无。
* 返回：当前Table类型。
************************************************************************/
SQLiteTableType SQLiteTableCell::GetTableType()
{
    switch (m_Data[0][1])
    {
        case 'a':
            return STT_Table;
        case 'n':
            return STT_Index;
        case 'r':
            return STT_Trigger;
        case 'i':
            return STT_View;
    }
    return STT_UNKNOWN;
}

/************************************************************************
* 功能：获取当前Table的对象名称。
* 参数：
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：当前Table的对象名称的长度。
************************************************************************/
UINT SQLiteTableCell::GetObjectName(OUT char * & bufferPtr)
{
    return GetText(1, bufferPtr);
}

/************************************************************************
* 功能：获取当前Table相关联的对象名称。
* 参数：
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：当前Table相关联的对象名称的长度。
* 备注：如果是表或视图，则与当前对象名称相同。
*       如果是索引或触发器对象，此字段值为与其相关的表名。
************************************************************************/
UINT SQLiteTableCell::GetRelatedObjectName(OUT char * & bufferPtr)
{
    return GetText(2, bufferPtr);
}

/************************************************************************
* 功能：获取当前Table对应的数据的根页索引。
* 参数：无。
* 返回：当前Table对应的数据的根页索引。
************************************************************************/
ULONG SQLiteTableCell::GetRootPageIndex()
{
    return GetInteger(3);
}

/************************************************************************
* 功能：获取当前Table创建时使用的SQL语句。
* 参数：
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：当前Table创建时使用的SQL语句的长度。
************************************************************************/
UINT SQLiteTableCell::GetCreateSQL(OUT char * & bufferPtr)
{
    return GetText(4, bufferPtr);
}
