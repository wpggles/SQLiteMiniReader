//
// @Project : SQLiteMiniReader
// @File    : SQLiteInteriorPage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteInteriorPage.h"

/************************************************************************
* 功能：获取当前内部页指向的叶子页的数量。
* 参数：无。
* 返回：当前内部页指向的叶子页的数量。
************************************************************************/
USHORT SQLiteInteriorPage::GetPageCount()
{
    return m_CellsCount;
}

/************************************************************************
* 功能：获取当前内部页指向的叶子页所在的页索引。
* 参数：
*       index [IN] -> 当前内部页中的叶子页索引。
* 返回：当前内部页指向的叶子页所在的页索引。
************************************************************************/
UINT SQLiteInteriorPage::GetPageIndex(IN int index)
{
    //如果不在有效范围内直接返回0
    if (index < 0 || index >= m_CellsCount)
    {
        return 0;
    }
    //单独处理最后一个，因为位置不同
    if (index == m_CellsCount - 1)
    {
        return SQLiteTools::BigEndianInt(m_PageData + m_BaseOffset + LAST_RIGHT_PAGE_INDEX_OFFSET);
    }
    return SQLiteTools::BigEndianInt(m_PageData + SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + PAGE_CELL_LIST_OFFSET + index * 2));
}

/************************************************************************
* 功能：获取当前内部页指向的叶子页的起始索引值。
* 参数：
*       index [IN] -> 当前内部页中的叶子的起始索引值。
* 返回：当前内部页指向的叶子页的起始索引值。
************************************************************************/
ULONG SQLiteInteriorPage::GetCompareIndex(IN int index)
{
    //如果不在有效范围内直接返回0
    if (index < 0 || index >= m_CellsCount)
    {
        return 0;
    }
    //如果是第一个，直接返回1
    if (index == 0)
    {
        return 1;
    }
    int useByteCount;
    return SQLiteTools::VolatileLong(m_PageData + SQLiteTools::BigEndianShort(m_PageData + m_BaseOffset + PAGE_CELL_LIST_OFFSET + (index - 1) * 2) + 4, &useByteCount);
}
