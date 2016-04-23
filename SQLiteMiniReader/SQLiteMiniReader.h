//
// @Project : SQLiteMiniReader
// @File    : SQLiteMiniReader.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_MINI_READER_H_
#define _SQLITE_MINI_READER_H_

#include <fstream>
#include "FileHeader/SQLiteFileHeader.h"
#include "Traverse/SQLiteTable.h"
#include "Traverse/SQLiteRecord.h"
#include "Cell/SQLiteDataCell.h"
#include "Cell/SQLiteTableCell.h"
#include "Page/SQLitePage.h"
#include "Page/SQLiteLeafPage.h"
#include "Page/SQLiteInteriorPage.h"

/************************************************************************
* 功能：遍历表的回调函数定义。
* 参数：
*       table [IN]     -> 当前传递的要处理的表数据。
*       pVoid [IN_OUT] -> 用户自定义值。
* 返回：继续查找下一条数据返回TRUE，否则返回FALSE。
************************************************************************/
typedef BOOL(*HandleTableCallback)(IN SQLiteTable * table, IN_OUT void * pVoid);

/************************************************************************
* 功能：遍历记录的回调函数定义。
* 参数：
*       record [IN]     -> 当前传递的要处理的单元记录。
*       pVoid  [IN_OUT] -> 用户自定义值。
* 返回：继续查找下一条数据返回TRUE，否则返回FALSE。
************************************************************************/
typedef BOOL(*HandleRecordCallback)(IN SQLiteRecord * record, IN_OUT void * pVoid);


//SQLite数据库读取处理类
class SQLiteMiniReader
{
private:
    //存储SQLite数据库文件中的数据
    BYTE * m_Data;
    //SQLite数据库文件的大小
    UINT m_DataLenght;
    //SQLite数据库文件头信息
    SQLiteFileHeader * m_SQLiteFileHeaderInfo;

private:
    /************************************************************************
    * 功能：查找指定表单元的对象。
    * 参数：
    *       tableName [IN] -> 要查找的表名。
    *       pageIndex [IN] -> 从第几页开始查找。
    * 返回：查找到的表单元对象，查找不到返回NULL。
    * 备注：此函数会递归查找，所以首次调用pageIndex应该为1。
    ************************************************************************/
    SQLiteTableCell * FindTableCell(IN const char * tableName, IN UINT pageIndex);

    /************************************************************************
    * 功能：遍历指定表下的单元。
    * 参数：
    *       pageIndex        [IN]     -> 从第几页开始遍历。
    *       handleRecordFunc [IN]     -> 处理记录数据的回调函数。
    *       record           [IN]     -> 要被处理的记录数据。
    *       pVoid            [IN_OUT] -> 用户自定义值。
    * 返回：继续遍历返回TRUE，否则返回FALSE，受回调函数控制。
    ************************************************************************/
    BOOL TraverseCell(IN UINT pageIndex, IN HandleRecordCallback handleRecordFunc, IN SQLiteRecord * record, IN_OUT void * pVoid);

    /************************************************************************
    * 功能：遍历所有的表。
    * 参数：
    *       pageIndex       [IN]     -> 从第几页开始遍历。
    *       handleTableFunc [IN]     -> 处理表数据的回调函数。
    *       table           [IN]     -> 要被处理的表数据。
    *       pVoid           [IN_OUT] -> 用户自定义值。
    * 返回：继续遍历返回TRUE，否则返回FALSE，受回调函数控制。
    ************************************************************************/
    BOOL TraverseTable(IN UINT pageIndex, IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid);

    /************************************************************************
    * 功能：遍历指定表下的单元数量。
    * 参数：
    *       pageIndex [IN] -> 从第几页开始遍历。
    * 返回：单元的数量。
    ************************************************************************/
    int TraverseCellCount(IN UINT pageIndex);

    /************************************************************************
    * 功能：遍历所有的表统计表的数量。
    * 参数：
    *       pageIndex [IN] -> 从第几页开始遍历。
    * 返回：表的数量。
    ************************************************************************/
    int TraverseTableCount(IN UINT pageIndex);

public:
    /************************************************************************
    * 功能：初始化数据。
    * 参数：
    *       dbFileName [IN] -> 要查询的SQLite数据库路径。
    * 返回：无。
    ************************************************************************/
    SQLiteMiniReader(IN const char * dbFileName);

    /************************************************************************
    * 功能：析构函数释放资源。
    * 参数：无。
    * 返回：无。
    ************************************************************************/
    ~SQLiteMiniReader();

    /************************************************************************
    * 功能：获取数据库文件头信息。
    * 参数：无。
    * 返回：数据库文件头信息。
    * 备注：不要手动释放，SQLiteMiniReader释放时会自动释放。
    ************************************************************************/
    SQLiteFileHeader * GetFileInfo();

    /************************************************************************
    * 功能：遍历查询指定表下所有数据。
    * 参数：
    *       tableName        [IN]     -> 要查询的表名。
    *       handleRecordFunc [IN]     -> 处理记录数据的回调函数。
    *       pVoid            [IN_OUT] -> 用户自定义值。
    * 返回：查询成功返回TRUE，否则返回FALSE。
    * 备注：此函数会嵌套循环调用回调函数，所以注意流程处理。
    ************************************************************************/
    BOOL QueryRecord(IN const char * tableName, IN HandleRecordCallback handleRecordFunc, IN_OUT void * pVoid);

	/************************************************************************
	* 功能：遍历查询所有的表。
	* 参数：
	*       handleTableFunc [IN]     -> 处理表数据的回调函数。
	*       pVoid           [IN_OUT] -> 用户自定义值。
	* 返回：查询成功返回TRUE，否则返回FALSE。
	* 备注：此函数会嵌套循环调用回调函数，所以注意流程处理。
	************************************************************************/
	void QueryTable(IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid);

    /************************************************************************
    * 功能：遍历查询指定表下数据的数量。
    * 参数：
    *       tableName [IN] -> 要查询的表名。
    * 返回：数据的数量, -1说明没有此表或不支持此表。
    ************************************************************************/
    int QueryRecordCount(IN const char * tableName);

    /************************************************************************
    * 功能：遍历查询数据库中表的数量。
    * 参数：无。
    * 返回：表的数量。
    * 备注：返回的数量是所能处理的表的数量，对于无类型等表直接忽略。
    ************************************************************************/
    int QueryTableCount();
};

#endif
