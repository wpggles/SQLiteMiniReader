//
// @Project : SQLiteMiniReader
// @File    : SQLiteFileHeader.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteFileHeader.h"

/************************************************************************
* 功能：构造方法，分析SQLite数据库文件头部数据。
* 参数：
*       data [IN] -> 要分析的SQLite数据库文件头部数据。
* 返回：无。
* 备注：所需分析的数据时数据库文件开头的前100字节。
************************************************************************/
SQLiteFileHeader::SQLiteFileHeader(IN const BYTE * data)
{
    //根据偏移进行相应的数据赋值
    memcpy(m_HeaderString, data + HEADER_STRING_OFFSET, sizeof(m_HeaderString));

    m_PageSize = SQLiteTools::BigEndianShort(data + PAGE_SIZE_OFFSET);
    m_FileFormatWriteVersion = *(data + FILE_FORMAT_WRITE_VERSION_OFFSET);
    m_FileFormatReadVersion = *(data + FILE_FORMAT_READ_VERSION_OFFSET);
    m_FileChangeCount = SQLiteTools::BigEndianInt(data + FILE_CHANGE_COUNT_OFFSET);
    m_PageCount = SQLiteTools::BigEndianInt(data + PAGE_COUNT_OFFSET);
    m_TotalFreelistPages = SQLiteTools::BigEndianInt(data + TOTAL_FREELIST_PAGES_OFFSET);
    m_SchemaCookie = SQLiteTools::BigEndianInt(data + SCHEMA_COOKIE_OFFSET);
    m_SchemaFormatNumber = SQLiteTools::BigEndianInt(data + SCHEMA_FORMAT_NUMBER_OFFSET);
    m_DefaultPageCacheSize = SQLiteTools::BigEndianInt(data + DEFAULT_PAGE_CACHE_SIZE_OFFSET);
    m_RootPageNumber = SQLiteTools::BigEndianInt(data + ROOT_PAGE_NUMBER_OFFSET);
    m_TextEncoding = SQLiteTools::BigEndianInt(data + TEXT_ENCODING_OFFSET);
    m_UserVersion = SQLiteTools::BigEndianInt(data + USER_VERSION_OFFSET);
    m_IncrementalVacuumMode = SQLiteTools::BigEndianInt(data + INCREMENTAL_VACUUM_MODE_OFFSET);
    m_ApplicationID = SQLiteTools::BigEndianInt(data + APPLICATION_ID_OFFSET);
    m_VersionValidForNumber = SQLiteTools::BigEndianInt(data + VERSION_VALID_FOR_NNUMBER_OFFSET);
    m_SQLiteVersionNumber.Version = SQLiteTools::BigEndianInt(data + SQLITE_VERSION_NUMBER_OFFSET);

    SplitSQLiteVersionNumber(
        m_SQLiteVersionNumber.Version,
        &m_SQLiteVersionNumber.MajorVersion,
        &m_SQLiteVersionNumber.MinorVersion,
        &m_SQLiteVersionNumber.ReleaseNumber
        );
}

/************************************************************************
* 功能：获取头标志字符串。
* 参数：无。
* 返回：头标志字符串。
************************************************************************/
const char * SQLiteFileHeader::GetHeaderString()
{
    return m_HeaderString;
}

/************************************************************************
* 功能：获取页大小。
* 参数：无。
* 返回：页大小。
************************************************************************/
USHORT SQLiteFileHeader::GetPageSize()
{
    return m_PageSize;
}

/************************************************************************
* 功能：获取文件格式(写)版本标志。
* 参数：无。
* 返回：文件格式(写)版本标志。
************************************************************************/
USHORT SQLiteFileHeader::GetFileFormatWriteVersion()
{
    return m_FileFormatWriteVersion;
}

/************************************************************************
* 功能：获取文件格式(读)版本标志。
* 参数：无。
* 返回：文件格式(读)版本标志。
************************************************************************/
USHORT SQLiteFileHeader::GetFileFormatReadVersion()
{
    return m_FileFormatReadVersion;
}

/************************************************************************
* 功能：获取文件修改次数。
* 参数：无。
* 返回：文件修改次数。
************************************************************************/
UINT   SQLiteFileHeader::GetFileChangeCount()
{
    return m_FileChangeCount;
}

/************************************************************************
* 功能：获取页数量。
* 参数：无。
* 返回：页数量。
************************************************************************/
UINT   SQLiteFileHeader::GetPageCount()
{
    return m_PageCount;
}

/************************************************************************
* 功能：获取空闲页链表头指针。
* 参数：无。
* 返回：空闲页链表头指针。
************************************************************************/
UINT   SQLiteFileHeader::GetFirstFreelistTrunkPage()
{
    return m_FirstFreelistTrunkPage;
}

/************************************************************************
* 功能：获取空闲页数量。
* 参数：无。
* 返回：空闲页数量。
************************************************************************/
UINT   SQLiteFileHeader::GetTotalFreelistPages()
{
    return m_TotalFreelistPages;
}

/************************************************************************
* 功能：获取表缓存。
* 参数：无。
* 返回：表缓存。
************************************************************************/
UINT   SQLiteFileHeader::GetSchemaCookie()
{
    return m_SchemaCookie;
}

/************************************************************************
* 功能：获取表格式标志。
* 参数：无。
* 返回：表格式标志。
************************************************************************/
UINT   SQLiteFileHeader::GetSchemaFormatNumber()
{
    return m_SchemaFormatNumber;
}

/************************************************************************
* 功能：获取默认页缓存大小。
* 参数：无。
* 返回：默认页缓存大小。
************************************************************************/
UINT   SQLiteFileHeader::GetDefaultPageCacheSize()
{
    return m_DefaultPageCacheSize;
}

/************************************************************************
* 功能：获取根页编号。
* 参数：无。
* 返回：根页编号。
************************************************************************/
UINT   SQLiteFileHeader::GetRootPageNumber()
{
    return m_RootPageNumber;
}

/************************************************************************
* 功能：获取文本编码标志。
* 参数：无。
* 返回：文本编码标志。
************************************************************************/
UINT   SQLiteFileHeader::GetTextEncoding()
{
    return m_TextEncoding;
}

/************************************************************************
* 功能：获取用户版本。
* 参数：无。
* 返回：用户版本。
************************************************************************/
UINT   SQLiteFileHeader::GetUserVersion()
{
    return m_UserVersion;
}

/************************************************************************
* 功能：获取INCREMENTAL VACUUM模式标志。
* 参数：无。
* 返回：INCREMENTAL VACUUM模式标志。
************************************************************************/
UINT   SQLiteFileHeader::GetIncrementalVacuumMode()
{
    return m_IncrementalVacuumMode;
}

/************************************************************************
* 功能：获取应用程序ID。
* 参数：无。
* 返回：应用程序ID。
************************************************************************/
UINT   SQLiteFileHeader::GetApplicationID()
{
    return m_ApplicationID;
}

/************************************************************************
* 功能：获取数据库文件被修改次数。
* 参数：无。
* 返回：数据库文件被修改次数。
************************************************************************/
UINT   SQLiteFileHeader::GetVersionValidForNumber()
{
    return m_VersionValidForNumber;
}

/************************************************************************
* 功能：获取SQLite版本数。
* 参数：无。
* 返回：SQLite版本数。
************************************************************************/
UINT SQLiteFileHeader::GetVersion()
{
    return m_SQLiteVersionNumber.Version;
}

/************************************************************************
* 功能：获取SQLite版本数拆分后的主版本。
* 参数：无。
* 返回：SQLite版本数拆分后的主版本。
************************************************************************/
USHORT SQLiteFileHeader::GetMajorVersion()
{
    return m_SQLiteVersionNumber.MajorVersion;
}

/************************************************************************
* 功能：获取SQLite版本数拆分后的副版本。
* 参数：无。
* 返回：SQLite版本数拆分后的副版本。
************************************************************************/
USHORT SQLiteFileHeader::GetMinorVersion()
{
    return m_SQLiteVersionNumber.MinorVersion;
}

/************************************************************************
* 功能：获取SQLite版本数拆分后的发布版本。
* 参数：无。
* 返回：SQLite版本数拆分后的发布版本。
************************************************************************/
USHORT SQLiteFileHeader::GetReleaseNumber()
{
    return m_SQLiteVersionNumber.ReleaseNumber;
}

/************************************************************************
* 功能：将SQLite版本数进行拆分。
* 参数：
*       version [IN]  -> 要处理的SQLite版本数。
*       x       [OUT] -> 返回的主版本号。
*       y       [OUT] -> 返回的副版本号。
*       z       [OUT] -> 返回的发布版本号。
* 返回：无。
************************************************************************/
void SQLiteFileHeader::SplitSQLiteVersionNumber(IN UINT version, OUT USHORT * x, OUT USHORT * y, OUT USHORT * z)
{
    *x = version / 1000000;
    *y = version % 1000000 / 1000;
    *z = version % 1000;
}
