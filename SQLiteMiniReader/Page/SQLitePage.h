//
// @Project : SQLiteMiniReader
// @File    : SQLitePage.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_PAGE_H_
#define _SQLITE_PAGE_H_

#include "../Tools/SQLiteTools.h"
#include <string.h>

//第一页数据起始偏移量
#define FIRST_PAGE_OFFSET            0x64
//页类型偏移量
#define PAGE_TYPE_OFFSET             0x00
//第一个自由块偏移量
#define FIRST_FREE_BLOCK_OFFSET      0x01
//本页单元数量偏移量
#define CELLS_COUNT_OFFSET           0x03
//最后一个单元偏移量
#define LAST_CELL_OFFSET             0x05
//碎片字节数偏移量
#define FRAGMENTEDED_BYTES_OFFSET    0x07
//单元列表偏移量
#define CELL_LIST_OFFSET             0x08

#define LAST_RIGHT_PAGE_INDEX_OFFSET 0x08

#define PAGE_CELL_LIST_OFFSET        0x0C

//SQLite页类型
enum SQLitePageType
{
    //错误(无效)页
    SPT_ErrorPage = 0x00,
    //内部索引页
    SPT_InteriorIndexPage = 0x02,
    //内部页
    SPT_InteriorPage = 0x05,
    //叶子索引页
    SPT_LeafIndexPage = 0x0A,
    //叶子页
    SPT_LeafPage = 0x0D
};

//SQLite页对象基类
class SQLitePage
{
protected:
    //基础偏移
    USHORT m_BaseOffset;
    //页索引
    USHORT m_PageIndex;
    //页大小
    USHORT m_PageSize;
    //页类型
    BYTE   m_PageType;
    //页数据
    BYTE * m_PageData;
    //第一个自由块
    USHORT m_FirstFreeBlock;
    //单元数量
    USHORT m_CellsCount;
    //(逻辑顺序上)最后一个单元偏移
    USHORT m_LastCellOffset;
    //碎片字节数
    USHORT m_FragmentedBytes;

    //原始数据的指针
    const BYTE * m_RawData;

public:
    /************************************************************************
    * 功能：构造方法，初始化类。
    * 参数：
    *       bytes     [IN] -> SQLite数据库文件(从头开始的)数据。
    *       pageSize  [IN] -> 一个Page数据的字节数。
    *       pageIndex [IN] -> 当前Page的索引(从1开始)。
    * 返回：无。
    * 备注：给定的数据必须完整，超过一个Page的数据会自动忽略。
    ************************************************************************/
    SQLitePage(IN const BYTE * bytes, IN USHORT pageSize, IN USHORT pageIndex);

    /************************************************************************
    * 功能：虚析构方法，释放数据。
    * 参数：无。
    * 返回：无。
    ************************************************************************/
    virtual ~SQLitePage();

    /************************************************************************
    * 功能：当前页是否是错误页。
    * 参数：无。
    * 返回：是错误页返回TRUE，否则返回FALSE。
    ************************************************************************/
    BOOL IsErrorPage();

    /************************************************************************
    * 功能：获取当前页的类型。
    * 参数：无。
    * 返回：当前页的类型。
    ************************************************************************/
    SQLitePageType GetPageType();
};

#endif