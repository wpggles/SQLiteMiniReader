//
// @Project : SQLiteMiniReader
// @File    : SQLiteFileHeader.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteFileHeader.h"

/************************************************************************
* ���ܣ����췽��������SQLite���ݿ��ļ�ͷ�����ݡ�
* ������
*       data [IN] -> Ҫ������SQLite���ݿ��ļ�ͷ�����ݡ�
* ���أ��ޡ�
* ��ע���������������ʱ���ݿ��ļ���ͷ��ǰ100�ֽڡ�
************************************************************************/
SQLiteFileHeader::SQLiteFileHeader(IN const BYTE * data)
{
    //����ƫ�ƽ�����Ӧ�����ݸ�ֵ
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
* ���ܣ���ȡͷ��־�ַ�����
* �������ޡ�
* ���أ�ͷ��־�ַ�����
************************************************************************/
const char * SQLiteFileHeader::GetHeaderString()
{
    return m_HeaderString;
}

/************************************************************************
* ���ܣ���ȡҳ��С��
* �������ޡ�
* ���أ�ҳ��С��
************************************************************************/
USHORT SQLiteFileHeader::GetPageSize()
{
    return m_PageSize;
}

/************************************************************************
* ���ܣ���ȡ�ļ���ʽ(д)�汾��־��
* �������ޡ�
* ���أ��ļ���ʽ(д)�汾��־��
************************************************************************/
USHORT SQLiteFileHeader::GetFileFormatWriteVersion()
{
    return m_FileFormatWriteVersion;
}

/************************************************************************
* ���ܣ���ȡ�ļ���ʽ(��)�汾��־��
* �������ޡ�
* ���أ��ļ���ʽ(��)�汾��־��
************************************************************************/
USHORT SQLiteFileHeader::GetFileFormatReadVersion()
{
    return m_FileFormatReadVersion;
}

/************************************************************************
* ���ܣ���ȡ�ļ��޸Ĵ�����
* �������ޡ�
* ���أ��ļ��޸Ĵ�����
************************************************************************/
UINT   SQLiteFileHeader::GetFileChangeCount()
{
    return m_FileChangeCount;
}

/************************************************************************
* ���ܣ���ȡҳ������
* �������ޡ�
* ���أ�ҳ������
************************************************************************/
UINT   SQLiteFileHeader::GetPageCount()
{
    return m_PageCount;
}

/************************************************************************
* ���ܣ���ȡ����ҳ����ͷָ�롣
* �������ޡ�
* ���أ�����ҳ����ͷָ�롣
************************************************************************/
UINT   SQLiteFileHeader::GetFirstFreelistTrunkPage()
{
    return m_FirstFreelistTrunkPage;
}

/************************************************************************
* ���ܣ���ȡ����ҳ������
* �������ޡ�
* ���أ�����ҳ������
************************************************************************/
UINT   SQLiteFileHeader::GetTotalFreelistPages()
{
    return m_TotalFreelistPages;
}

/************************************************************************
* ���ܣ���ȡ���档
* �������ޡ�
* ���أ����档
************************************************************************/
UINT   SQLiteFileHeader::GetSchemaCookie()
{
    return m_SchemaCookie;
}

/************************************************************************
* ���ܣ���ȡ���ʽ��־��
* �������ޡ�
* ���أ����ʽ��־��
************************************************************************/
UINT   SQLiteFileHeader::GetSchemaFormatNumber()
{
    return m_SchemaFormatNumber;
}

/************************************************************************
* ���ܣ���ȡĬ��ҳ�����С��
* �������ޡ�
* ���أ�Ĭ��ҳ�����С��
************************************************************************/
UINT   SQLiteFileHeader::GetDefaultPageCacheSize()
{
    return m_DefaultPageCacheSize;
}

/************************************************************************
* ���ܣ���ȡ��ҳ��š�
* �������ޡ�
* ���أ���ҳ��š�
************************************************************************/
UINT   SQLiteFileHeader::GetRootPageNumber()
{
    return m_RootPageNumber;
}

/************************************************************************
* ���ܣ���ȡ�ı������־��
* �������ޡ�
* ���أ��ı������־��
************************************************************************/
UINT   SQLiteFileHeader::GetTextEncoding()
{
    return m_TextEncoding;
}

/************************************************************************
* ���ܣ���ȡ�û��汾��
* �������ޡ�
* ���أ��û��汾��
************************************************************************/
UINT   SQLiteFileHeader::GetUserVersion()
{
    return m_UserVersion;
}

/************************************************************************
* ���ܣ���ȡINCREMENTAL VACUUMģʽ��־��
* �������ޡ�
* ���أ�INCREMENTAL VACUUMģʽ��־��
************************************************************************/
UINT   SQLiteFileHeader::GetIncrementalVacuumMode()
{
    return m_IncrementalVacuumMode;
}

/************************************************************************
* ���ܣ���ȡӦ�ó���ID��
* �������ޡ�
* ���أ�Ӧ�ó���ID��
************************************************************************/
UINT   SQLiteFileHeader::GetApplicationID()
{
    return m_ApplicationID;
}

/************************************************************************
* ���ܣ���ȡ���ݿ��ļ����޸Ĵ�����
* �������ޡ�
* ���أ����ݿ��ļ����޸Ĵ�����
************************************************************************/
UINT   SQLiteFileHeader::GetVersionValidForNumber()
{
    return m_VersionValidForNumber;
}

/************************************************************************
* ���ܣ���ȡSQLite�汾����
* �������ޡ�
* ���أ�SQLite�汾����
************************************************************************/
UINT SQLiteFileHeader::GetVersion()
{
    return m_SQLiteVersionNumber.Version;
}

/************************************************************************
* ���ܣ���ȡSQLite�汾����ֺ�����汾��
* �������ޡ�
* ���أ�SQLite�汾����ֺ�����汾��
************************************************************************/
USHORT SQLiteFileHeader::GetMajorVersion()
{
    return m_SQLiteVersionNumber.MajorVersion;
}

/************************************************************************
* ���ܣ���ȡSQLite�汾����ֺ�ĸ��汾��
* �������ޡ�
* ���أ�SQLite�汾����ֺ�ĸ��汾��
************************************************************************/
USHORT SQLiteFileHeader::GetMinorVersion()
{
    return m_SQLiteVersionNumber.MinorVersion;
}

/************************************************************************
* ���ܣ���ȡSQLite�汾����ֺ�ķ����汾��
* �������ޡ�
* ���أ�SQLite�汾����ֺ�ķ����汾��
************************************************************************/
USHORT SQLiteFileHeader::GetReleaseNumber()
{
    return m_SQLiteVersionNumber.ReleaseNumber;
}

/************************************************************************
* ���ܣ���SQLite�汾�����в�֡�
* ������
*       version [IN]  -> Ҫ�����SQLite�汾����
*       x       [OUT] -> ���ص����汾�š�
*       y       [OUT] -> ���صĸ��汾�š�
*       z       [OUT] -> ���صķ����汾�š�
* ���أ��ޡ�
************************************************************************/
void SQLiteFileHeader::SplitSQLiteVersionNumber(IN UINT version, OUT USHORT * x, OUT USHORT * y, OUT USHORT * z)
{
    *x = version / 1000000;
    *y = version % 1000000 / 1000;
    *z = version % 1000;
}
