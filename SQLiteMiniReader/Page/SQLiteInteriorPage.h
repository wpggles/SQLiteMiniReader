//
// @Project : SQLiteMiniReader
// @File    : SQLiteInteriorPage.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_INTERIOR_PAGE_H_
#define _SQLITE_INTERIOR_PAGE_H_

#include "SQLitePage.h"

//SQLite叶子页索引类
class SQLiteInteriorPage : public SQLitePage
{
private:
    /************************************************************************
    * 功能：构造方法，初始化类。
    * 参数：无。
    * 返回：无。
    * 备注：给定的数据必须完整，超过一个Page的数据会自动忽略。
    ************************************************************************/
    SQLiteInteriorPage() : SQLitePage(NULL, 0, 0){};

public:
    /************************************************************************
    * 功能：虚析构方法，释放数据。
    * 参数：无。
    * 返回：无。
    ************************************************************************/
    ~SQLiteInteriorPage(){};

    /************************************************************************
    * 功能：获取当前内部页指向的叶子页的数量。
    * 参数：无。
    * 返回：当前内部页指向的叶子页的数量。
    ************************************************************************/
    USHORT GetPageCount();

    /************************************************************************
    * 功能：获取当前内部页指向的叶子页所在的页索引。
    * 参数：
    *       index [IN] -> 当前内部页中的叶子页索引。
    * 返回：当前内部页指向的叶子页所在的页索引。
    ************************************************************************/
    UINT GetPageIndex(IN int index);

    /************************************************************************
    * 功能：获取当前内部页指向的叶子页的起始索引值。
    * 参数：
    *       index [IN] -> 当前内部页中的叶子的起始索引值。
    * 返回：当前内部页指向的叶子页的起始索引值。
    ************************************************************************/
    ULONG GetCompareIndex(IN int index);
};

#endif
