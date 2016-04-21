//
// @Project : SQLiteMiniReader
// @File    : SQLiteMiniReader.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteMiniReader.h"

/************************************************************************
* 功能：初始化数据。
* 参数：
*       dbFileName [IN] -> 要查询的SQLite数据库路径。
* 返回：无。
************************************************************************/
SQLiteMiniReader::SQLiteMiniReader(const char * dbFileName)
{
    m_Data = NULL;
    m_SQLiteFileHeaderInfo = NULL;
    std::ifstream in;
    in.open(dbFileName, std::ios::in | std::ios::binary | std::ios::ate);
    m_DataLenght = (UINT)in.tellg();
    if (m_DataLenght <= 100)
    {
        in.close();
        return;
    }
    m_Data = new BYTE[m_DataLenght];
    in.seekg(std::ios::beg);
    in.read((char *)m_Data, m_DataLenght);
    in.close();
    m_SQLiteFileHeaderInfo = new SQLiteFileHeader(m_Data);
}

/************************************************************************
* 功能：析构函数释放资源。
* 参数：无。
* 返回：无。
************************************************************************/
SQLiteMiniReader::~SQLiteMiniReader()
{
    if (m_Data != NULL)
    {
        delete[] m_Data;
        m_Data = NULL;
    }
    if (m_SQLiteFileHeaderInfo != NULL)
    {
        delete m_SQLiteFileHeaderInfo;
        m_SQLiteFileHeaderInfo = NULL;
    }
}

/************************************************************************
* 功能：获取数据库文件头信息。
* 参数：无。
* 返回：数据库文件头信息。
* 备注：不要手动释放，SQLiteReader释放时会自动释放。
************************************************************************/
SQLiteFileHeader * SQLiteMiniReader::GetFileInfo()
{
    return m_SQLiteFileHeaderInfo;
}

/************************************************************************
* 功能：查找指定表单元的对象。
* 参数：
*       tableName [IN] -> 要查找的表名。
*       pageIndex [IN] -> 从第几页开始查找。
* 返回：查找到的表单元对象，查找不到返回NULL。
* 备注：此函数会递归查找，所以首次调用pageIndex应该为1。
************************************************************************/
SQLiteTableCell * SQLiteMiniReader::FindTableCell(const char * tableName, UINT pageIndex)
{
    //指向查找到的表单元
    SQLiteTableCell * queryTableCell = NULL;
    //创建页对象
    SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
    //获取页类型，如果是叶子页则开始是查找数据
    if (page->GetPageType() == SPT_LeafPage)
    {
        //转换为叶子页处理
        SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
        //获取单元数量
        int count = leafPage->GetCellsCount();
        //循环遍历所有表对象
        for (int i = 0; i < count && queryTableCell == NULL; ++i)
        {
            //获取表对象
            SQLiteTableCell * tableCell = (SQLiteTableCell *)leafPage->GetCell(i);
            //存储表名
            char * tableNameBuffer;
            tableCell->GetObjectName(tableNameBuffer);
            //如果是表并且表名是要查找的，则返回对应的表对象
            if (tableCell->GetTableType() == STT_Table && !_stricmp(tableNameBuffer, tableName))
            {
                queryTableCell = tableCell;
            }
            else
            {
                delete tableCell;
            }
            //释放掉表名空间
            delete[] tableNameBuffer;
        }
    }
    //如果是内部页则遍历当前内部页的页指针
    else if (page->GetPageType() == SPT_InteriorPage)
    {
        //转换为内部页处理
        SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
        //获取内部页指向的叶子页(内部页)数量
        int count = interiorPage->GetPageCount();
        //循环递归遍历查找
        for (int i = 0; i < count && queryTableCell == NULL; ++i)
        {
            queryTableCell = FindTableCell(tableName, interiorPage->GetPageIndex(i));
        }
    }
    //删除创建的页数据
    delete page;

    return queryTableCell;
}

/************************************************************************
* 功能：遍历指定表下的单元。
* 参数：
*       pageIndex        [IN]     -> 从第几页开始遍历。
*       handleRecordFunc [IN]     -> 处理记录数据的回调函数。
*       record           [IN]     -> 要被处理的记录数据。
*       pVoid            [IN_OUT] -> 用户自定义值。
* 返回：继续遍历返回TRUE，否则返回FALSE，受回调函数控制。
************************************************************************/
BOOL SQLiteMiniReader::TraverseCell(IN UINT pageIndex, IN HandleRecordCallback handleRecordFunc, IN SQLiteRecord * record, IN_OUT void * pVoid)
{
    //标记是否继续遍历
    BOOL continueTravers = TRUE;
    //创建页对象
    SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
    //获取页类型，如果是叶子页则开始是查找数据
    if (page->GetPageType() == SPT_LeafPage)
    {
        //转换成叶子页对象处理
        SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
        int count = leafPage->GetCellsCount();
        
        for (int i = 0; i < count && continueTravers; ++i)
        {
            SQLiteDataCell * dataCell = (SQLiteDataCell *)leafPage->GetCell(i);
            record->SetTmpDataCell(dataCell);
            continueTravers = handleRecordFunc(record, pVoid);

            delete dataCell;
        }
    }
    //如果是内部页则遍历当前内部页的页指针
    else if (page->GetPageType() == SPT_InteriorPage)
    {
        SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
        int count = interiorPage->GetPageCount();
        for (int i = 0; i < count && continueTravers; ++i)
        {
            continueTravers = TraverseCell(interiorPage->GetPageIndex(i), handleRecordFunc, record, pVoid);
        }
    }
    delete page;
    return continueTravers;
}

/************************************************************************
* 功能：查询指定表下所有数据。
* 参数：
*       tableName        [IN]     -> 要查询的表名。
*       handleRecordFunc [IN]     -> 处理记录数据的回调函数。
*       pVoid            [IN_OUT] -> 用户自定义值。
* 返回：查询成功返回TRUE，否则返回FALSE。
* 备注：此函数会嵌套循环调用回调函数，所以注意流程处理。
************************************************************************/
BOOL SQLiteMiniReader::QueryRecord(IN const char * tableName, IN HandleRecordCallback handleRecordFunc, IN_OUT void * pVoid)
{
    //从第1页开始查询指定的表
    SQLiteTableCell * queryTableCell = FindTableCell(tableName, 1);
    //查不到直接返回
    if (queryTableCell == NULL)
    {
        return FALSE;
    }
    //创建用于传参的对象
    SQLiteRecord sqliteHandleCell(queryTableCell);
	if (!sqliteHandleCell.m_SQLiteTable->HasNoTypeFeild())
	{
		//遍历对象下的记录
		TraverseCell((UINT)queryTableCell->GetRootPageIndex(), handleRecordFunc, &sqliteHandleCell, pVoid);
	}
    //删除表对象
    delete queryTableCell;
    return TRUE;
}

/************************************************************************
* 功能：遍历所有的表。
* 参数：
*       pageIndex       [IN]     -> 从第几页开始遍历。
*       handleTableFunc [IN]     -> 处理表数据的回调函数。
*       pVoid           [IN_OUT] -> 用户自定义值。
* 返回：继续遍历返回TRUE，否则返回FALSE，受回调函数控制。
************************************************************************/
BOOL SQLiteMiniReader::TraverseTable(IN UINT pageIndex, IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid)
{
	//标记是否继续遍历
	BOOL continueTravers = TRUE;
	//创建页对象
	SQLitePage * page = new SQLitePage(m_Data, m_SQLiteFileHeaderInfo->GetPageSize(), pageIndex);
	//获取页类型，如果是叶子页则开始是查找数据
	if (page->GetPageType() == SPT_LeafPage)
	{
		//转换成叶子页对象处理
		SQLiteLeafPage * leafPage = (SQLiteLeafPage *)page;
		int count = leafPage->GetCellsCount();

		for (int i = 0; i < count && continueTravers; ++i)
		{
			SQLiteTableCell * tableCell = (SQLiteTableCell *)leafPage->GetCell(i);

			if (tableCell->GetTableType() == STT_Table)
			{
				SQLiteTable * table = new SQLiteTable(tableCell);
				if (!table->HasNoTypeFeild())
				{
					continueTravers = handleTableFunc(table, pVoid);
				}
				delete table;
			}
			delete tableCell;
		}
	}
	//如果是内部页则遍历当前内部页的页指针
	else if (page->GetPageType() == SPT_InteriorPage)
	{
		SQLiteInteriorPage * interiorPage = (SQLiteInteriorPage *)page;
		int count = interiorPage->GetPageCount();
		for (int i = 0; i < count && continueTravers; ++i)
		{
			continueTravers = TraverseTable(interiorPage->GetPageIndex(i), handleTableFunc, pVoid);
		}
	}
	delete page;
	return continueTravers;
}

/************************************************************************
* 功能：遍历查询所有的表。
* 参数：
*       handleTableFunc [IN]     -> 处理表数据的回调函数。
*       pVoid           [IN_OUT] -> 用户自定义值。
* 返回：查询成功返回TRUE，否则返回FALSE。
* 备注：此函数会嵌套循环调用回调函数，所以注意流程处理。
************************************************************************/
void SQLiteMiniReader::QueryTable(IN HandleTableCallback handleTableFunc, IN_OUT void * pVoid)
{
	TraverseTable(1, handleTableFunc, pVoid);
}
