//
// @Project : SQLiteMiniReader
// @File    : SQLiteRecord.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteRecord.h"

/************************************************************************
* 功能：私有构造。
* 参数：
*       tableCell [IN] -> 表单元对象。
* 返回：无。
************************************************************************/
SQLiteRecord::SQLiteRecord(IN SQLiteTableCell * tableCell)
{
	m_TmpDataCell = NULL;
	m_SQLiteTable = new SQLiteTable(tableCell);
}

/************************************************************************
* 功能：设置临时用于传递的一条数据记录。
* 参数：
*       cell [IN] -> 数据单元。
* 返回：无。
* 备注：函数不会维护传递的数据单元，销毁由调用者处理。
************************************************************************/
void SQLiteRecord::SetTmpDataCell(IN SQLiteDataCell * cell)
{
    m_TmpDataCell = cell;
}

/************************************************************************
* 功能：私有析构。
* 参数：无。
* 返回：无。
************************************************************************/
SQLiteRecord::~SQLiteRecord()
{
    if (m_SQLiteTable)
    {
        delete m_SQLiteTable;
    }
}

/************************************************************************
* 功能：获取字段数量。
* 参数：无。
* 返回：字段数量。
************************************************************************/
UINT SQLiteRecord::GetFieldCount()
{
    return m_SQLiteTable->GetFieldCount();
}

/************************************************************************
* 功能：获取指定字段的字段名称。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：字段名称的字符串。
* 备注：返回值不可修改，也不用单独释放。
************************************************************************/
const char * SQLiteRecord::GetFieldName(IN UINT index)
{
    return m_SQLiteTable->GetFieldName(index);
}

/************************************************************************
* 功能：获取指定字段的字段类型。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：字段类型的字符串。
* 备注：返回值不要修改，也不要单独释放。
************************************************************************/
const char * SQLiteRecord::GetFeildTypeName(IN UINT index)
{
    return m_SQLiteTable->GetTypeName(index);
}

/************************************************************************
* 功能：获取指定字段在SQLite中的近似类型。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：指定字段在SQLite中的近似类型。
************************************************************************/
SQLiteDataType SQLiteRecord::GetFeildSimilarType(IN UINT index)
{
    return m_SQLiteTable->GetSimilarType(index);
}

/************************************************************************
* 功能：获取指定字段的属性。
* 参数：
*       index      [IN] -> 指定字段的索引。
*       properties [IN] -> 要检查的属性(除SFP_NOP外可以使用'|'连接)。
* 返回：指定的属性存在返回TRUE，只要有一个不存在就返回FALSE。
* 注意：这里properties的属性传递SQLiteFeildProperty类型即可。
************************************************************************/
BOOL SQLiteRecord::GetFeildProperty(IN UINT index, IN int properties)
{
    return m_SQLiteTable->GetFeildProperty(index, properties);
}

/************************************************************************
* 功能：获取当前单元在所在表中的RowID。
* 参数：无。
* 返回：RowID。
************************************************************************/
ULONG SQLiteRecord::GetRowID()
{
    return m_TmpDataCell ? m_TmpDataCell->GetRowID() : 0;
}

/************************************************************************
* 功能：获取指定字段的布尔型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：布尔值。
* 备注：小于256并且的非0为TRUE，否则为FALSE。
************************************************************************/
BOOL SQLiteRecord::GetBoolean(IN int index)
{
    return m_TmpDataCell ? m_TmpDataCell->GetBoolean(index) : 0;
}

/************************************************************************
* 功能：获取指定字段的整数型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：转换后的整数。
************************************************************************/
LONG SQLiteRecord::GetInteger(IN int index)
{

    if (m_TmpDataCell)
    {
        LONG result = m_TmpDataCell->GetInteger(index);
        //因为SQLite为了节省空间，对于主键是自增的字段默认采用的是rowid
        if (!result)
        {
            if (m_SQLiteTable->GetFeildProperty(index, SFP_PRIMARY_KEY) ||
                m_SQLiteTable->GetFeildProperty(index, SFP_AUTOINCREMENT))
            {
                return m_TmpDataCell->GetRowID();
            }
        }
        else
        {
            return result;
        }
    }
    return 0;
}

/************************************************************************
* 功能：获取指定字段的浮点型数据。
* 参数：
*       index [IN] -> 指定的字段索引。
* 返回：转换后的浮点数。
************************************************************************/
double SQLiteRecord::GetReal(IN int index)
{
    return m_TmpDataCell ? m_TmpDataCell->GetReal(index) : 0;
}

/************************************************************************
* 功能：获取指定字段的文本型数据。
* 参数：
*       index     [IN]  -> 指定的字段索引。
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：文本长度。
* 注意：bufferPtr 只需要一个字符串指针，函数内申请空间，使用完要释放掉。
************************************************************************/
UINT SQLiteRecord::GetText(IN int index, OUT char * & bufferPtr)
{
    return m_TmpDataCell ? m_TmpDataCell->GetText(index, bufferPtr) : 0;
}

/************************************************************************
* 功能：获取指定字段的二进制型数据。
* 参数：
*       index     [IN]  -> 指定的字段索引。
*       bufferPtr [OUT] -> 存储返回数据的指针。
* 返回：二进制型数据长度。
* 注意：bufferPtr 只需要一个BYTE指针，函数内申请空间，使用完要释放掉。
************************************************************************/
UINT SQLiteRecord::GetBLOB(IN int index, OUT BYTE * & bufferPtr)
{
    return m_TmpDataCell ? m_TmpDataCell->GetBLOB(index, bufferPtr) : 0;
}
