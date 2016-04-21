//
// @Project : SQLiteMiniReader
// @File    : SQLiteDataCell.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteDataCell.h"

/************************************************************************
* 功能：获取当前单元在所在表中的RowID。
* 参数：无。
* 返回：RowID。
************************************************************************/
ULONG SQLiteDataCell::GetRowID()
{
    return m_RowID;
}

/************************************************************************
* 功能：获取当前单元的字段列数。
* 参数：无。
* 返回：字段列数。
************************************************************************/
ULONG SQLiteDataCell::GetColumnCount()
{
    return m_ColumnCount;
}

/************************************************************************
* 功能：获取指定列的字段类型。
* 参数：
*       index [IN] -> 指定列字段索引。
* 返回：SQLite数据类型。
* 备注：根据数据来判断的，不准确，仅供参考。
*       这里返回布尔型也有可能是数字，只不过数字可能是0或1。
************************************************************************/
SQLiteDataType SQLiteDataCell::GetTpye(IN int index)
{
    if (!IS_VALOD_INDEX(index))
    {
        return SDT_UNKNOWN;
    }
    switch (m_Types[index])
    {
        case 0:
            return SDT_DBNULL;
        case 8:
        case 9:
            return SDT_BOOLEAN;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return SDT_INTEGER;
        case 7:
            return SDT_REAL;
        default:
            if (IS_BLOB_FIELD(m_Types[index]))
            {
                return SDT_BLOB;
            }
            else if (IS_TEXT_FIELD(m_Types[index]))
            {
                return SDT_TEXT;
            }
    }
    return SDT_UNKNOWN;
}

/************************************************************************
* 功能：获取指定字段的布尔型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：布尔值。
* 备注：小于256并且的非0为TRUE，否则为FALSE。
************************************************************************/
BOOL SQLiteDataCell::GetBoolean(IN int index)
{
    switch (m_Types[index])
    {
    case 1:
        return *m_Data[index] != 0;
    case 8:
        return TRUE;
    }
    return FALSE;
}

/************************************************************************
* 功能：获取指定字段的整数型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：转换后的整数。
************************************************************************/
LONG SQLiteDataCell::GetInteger(IN int index)
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
* 功能：获取指定字段的浮点型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：转换后的浮点数。
************************************************************************/
double SQLiteDataCell::GetReal(int index)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        return 0;
    }
    return SQLiteTools::BigEndianDouble(m_Data[index]);
}

/************************************************************************
* 功能：获取指定字段的文本型数据。
* 参数：
*       index     [IN]  -> 指定的字段索引。
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：文本长度。
* 注意：bufferPtr 只需要一个字符串指针，函数内申请空间，使用完要释放掉。
************************************************************************/
UINT SQLiteDataCell::GetText(IN int index, OUT char * & bufferPtr)
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
* 功能：获取指定字段的二进制型数据。
* 参数：
*       index     [IN]  -> 指定的字段索引。
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：二进制型数据长度。
* 注意：bufferPtr 只需要一个BYTE指针，函数内申请空间，使用完要释放掉。
************************************************************************/
UINT SQLiteDataCell::GetBLOB(IN int index, OUT BYTE * & bufferPtr)
{
    if (!IS_VALOD_INDEX(index) || IS_DBNULL(index))
    {
        bufferPtr = NULL;
        return 0;
    }
    UINT len = BLOB_LENGHT(m_Types[index]);
    if (len == 0)
    {
        bufferPtr = NULL;
        return 0;
    }
    bufferPtr = new BYTE[len];
    memcpy(bufferPtr, m_Data[index], len);
    return len;
}
