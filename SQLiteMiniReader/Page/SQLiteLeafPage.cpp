//
// @Project : SQLiteMiniReader
// @File    : SQLiteLeafPage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteLeafPage.h"

/************************************************************************
* 功能：获取当前页单元数量。
* 参数：无。
* 返回：当前页的单元数量。
************************************************************************/
USHORT SQLiteLeafPage::GetCellsCount()
{
    return m_CellsCount;
}

/************************************************************************
* 功能：获取当前页指定的单元。
* 参数：
*       index [IN] -> 要获取的单元索引。
* 返回：单元对象，如果索引不对或对象创建失败返回NULL。
* 备注：返回的单元要手动释放。
************************************************************************/
SQLiteCell * SQLiteLeafPage::GetCell(IN int index)
{
    if (index >= m_CellsCount || index < 0)
    {
        return NULL;
    }
    //记录当前要返回的单元偏移
    int offset = SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + CELL_LIST_OFFSET + index * 2);
    //计算当前单元前一个单元的偏移
    int prevOffset = index == 0 ? m_PageSize : SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + CELL_LIST_OFFSET + (index - 1) * 2);
    //返回创建的单元对象
    return new SQLiteCell(m_PageData + offset, m_RawData, m_PageSize, prevOffset - offset);
}
