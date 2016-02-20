//
// @Project : SQLiteMiniReader
// @File    : SQLitePage.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLitePage.h"

/************************************************************************
* 功能：构造方法，初始化类。
* 参数：
*       bytes     [IN] -> SQLite数据库文件(从头开始的)数据。
*       pageSize  [IN] -> 一个Page数据的字节数。
*       pageIndex [IN] -> 当前Page的索引(从1开始)。
* 返回：无。
* 备注：给定的数据必须完整，超过一个Page的数据会自动忽略。
************************************************************************/
SQLitePage::SQLitePage(IN const BYTE * bytes, IN USHORT pageSize, IN USHORT pageIndex) : m_RawData(bytes)
{
    //如果是第一个page，因为开头含有100字节的文件头数据，所以要计算一个基础的偏移
    m_BaseOffset = pageIndex == 1 ? FIRST_PAGE_OFFSET : 0;
    bytes += pageSize * (pageIndex - 1);
    m_PageIndex = pageIndex;
    m_PageSize = pageSize;
   
    m_PageType = *(bytes + m_BaseOffset);

    m_PageData = new BYTE[m_PageSize];
    memcpy(m_PageData, bytes, m_PageSize);

    //判断如果是错误页就不再往下判断了
    if (IsErrorPage())
    {
        return;
    }

    m_FirstFreeBlock = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + FIRST_FREE_BLOCK_OFFSET);
    m_CellsCount = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + CELLS_COUNT_OFFSET);
    m_LastCellOffset = SQLiteTools::BigEndianShort(bytes + m_BaseOffset + LAST_CELL_OFFSET);
    m_FragmentedBytes = (USHORT)*(bytes + m_BaseOffset + FRAGMENTEDED_BYTES_OFFSET);
    
    //如果是内部页
    if (GetPageType() == SPT_InteriorPage)
    {
        //因为左右侧子叶不记在单原页数量之内
        m_CellsCount += 1;
    }
}

/************************************************************************
* 功能：虚析构方法，释放数据。
* 参数：无。
* 返回：无。
************************************************************************/
SQLitePage::~SQLitePage()
{
    delete [] m_PageData;
}

/************************************************************************
* 功能：当前页是否是错误页。
* 参数：无。
* 返回：是错误页返回TRUE，否则返回FALSE。
************************************************************************/
BOOL SQLitePage::IsErrorPage()
{
    return GetPageType() == SPT_ErrorPage;
}

/************************************************************************
* 功能：获取当前页的类型。
* 参数：无。
* 返回：当前页的类型。
************************************************************************/
SQLitePageType SQLitePage::GetPageType()
{
    switch (m_PageType)
    {
        case 0x02:
            return SPT_InteriorIndexPage;
        case 0x05:
            return SPT_InteriorPage;
        case 0x10:
            return SPT_LeafIndexPage;
        case 0x0D:
            return SPT_LeafPage;
        default:
            return SPT_ErrorPage;
    }
}
