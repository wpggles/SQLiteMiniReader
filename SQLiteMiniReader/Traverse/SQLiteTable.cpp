//
// @Project : SQLiteMiniReader
// @File    : SQLiteTable.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTable.h"

/************************************************************************
* ���ܣ���ʽ��SQL��䣬ת��ΪСд��ȥ�����е��ظ��ո�ɾ���Ʊ�ͻ��з�
*       ����հ��ַ���
* �������ޡ�
*       sqlString [IN_OUT] -> Ҫ��ʽ����SQL��䡣
* ���أ��ޡ�
* ��ע���˲�������ԭ�ַ����Ļ����Ͻ���ֱ���޸ģ���Ӱ��ԭ�ַ�����
*       ����ʹ�õ��������ִ�Сд�Ĳ��ֲ���ת��ΪСд��
************************************************************************/
void SQLiteTable::FormatSql(IN_OUT char * sqlString)
{
    //��Ϊ��Ϊ�˸�ʽ��SQL��ȥ�����е��ظ��ո�ɾ���Ʊ�ͻ��з�����հ��ַ���������Ԫ�ַ����ϲ�����
    //ԭ�ַ���ɨ��λ��ָ��
    char * orgPos = sqlString;
    //��ǰ�ַ���д��λ��ָ��
    char * curPos = sqlString;
    //ָʾ��һ���ַ��Ƿ��пհ��ַ�
    BOOL lastIsSpace = FALSE;
    //ָʾ��һ���ַ��Ƿ��Ƿָ��ַ�(',' , '(' , ')')
    BOOL lastIsSplite = TRUE;
    //ָʾ��ǰ�ַ����Ƿ��Ƿָ��ַ�(',' , '(' , ')')
    BOOL currentIsSplite = FALSE;
    //�ֺű�־�����ڷֺ��ڵĴ�Сд��ת��
    BOOL semicolonFlag = FALSE;

    //ѭ������Դ�ַ���
    while (*orgPos)
    {
        //�ж��Ƿ��ǿհ��ַ�
        if (IS_SPACE_CHAR(*orgPos))
        {
            //�޸Ŀհ��ַ����
            lastIsSpace = TRUE;
            //ָ������
            orgPos++;
            //������ִ�У�����ѭ���ж���һ��
            continue;
        }
        //�жϵ�ǰ�Ƿ��Ƿָ��ַ�
        currentIsSplite = IS_SPLITE_CHAR(*orgPos);
        //���ִ�е����˵����ǰ��������ǿհ��ַ�������Ҫ�ж�һ��֮ǰ��û�пհ��ַ������֮ǰ�пհ��ַ���������Ƿ���ӣ����޸ı��
        if (lastIsSpace)
        {
            //�޸ı��
            lastIsSpace = FALSE;

            //һ�����if�ṹ�Ƕ���������Զ���ע�͵ľ���д��
            if (*orgPos != ',' && (!lastIsSplite || !currentIsSplite) && 
                ((!lastIsSplite && currentIsSplite && *orgPos == '(') || 
                (lastIsSplite && !currentIsSplite && *(curPos - 1) == ')') || 
                (!lastIsSplite && !currentIsSplite)))
            {
                *curPos = ' ';
                curPos++;
            }
        }
        //��¼��ǰ�Ƿ��Ƿָ�������Ϊ��һ���ж���һ���Ƿ��Ƿָ���������
        lastIsSplite = currentIsSplite;
        
        //ִ�е�����ֻ��Ҫ���и�ֵ
        *curPos = *orgPos;
        //�ж��Ƿ���ֵ�����
        if (*curPos == '\'')
        {
            semicolonFlag = !semicolonFlag;
        }
        //������ڵ������ھͽ����д�д��ĸת��ΪСд
        else if (!semicolonFlag && (*curPos >= 0x41 && *curPos <= 0x5A))
        {
            *curPos += 0x20;
        }
        //ָ��ָ����һ��λ��
        orgPos++;
        curPos++;
        
    }
    //�ٸ�ʽ������ַ���ĩβ�����ַ���������
    *curPos = 0;

}

/************************************************************************
* ���ܣ���ʼ��ָ���������ֶ����ԡ�
* ������
*       index    [IN] -> ָ����ʼ���ֶ����Ե�������
*       feildSql [IN] -> ���ڳ�ʼ����SQL���Ƭ�Ρ�
* ���أ��ޡ�
* ��ע���˲ٱ����ڳ�Ա����m_Fields�Ѿ�����ռ������¡�
************************************************************************/
void SQLiteTable::InitSqlFeild(IN int index, IN const char * feildSql)
{
    int len = strlen(feildSql) + 1;
    char * sql = new char[len];
    memcpy(sql, feildSql, len);
    char * tmpPosStartPtr = sql;
    char * tmpPosEndPtr = *tmpPosStartPtr == '\'' ? strchr(tmpPosStartPtr + 1, '\'') + 1 : strchr(tmpPosStartPtr, ' ');
	if (!tmpPosEndPtr)
	{
		tmpPosEndPtr = sql + len;
	}
    *tmpPosEndPtr = 0;
    len = strlen(tmpPosStartPtr) + 1;
    m_Fields[index].FieldName = new char[len];
    memcpy(m_Fields[index].FieldName, tmpPosStartPtr, len);

    tmpPosStartPtr = tmpPosEndPtr + 1;
    tmpPosEndPtr = strchr(tmpPosStartPtr, ')');
    if (tmpPosEndPtr == NULL)
    {
        tmpPosEndPtr = strchr(tmpPosStartPtr, ' ');
        if (tmpPosEndPtr)
        {
            *tmpPosEndPtr = 0;
        }
    }
    else
    {
        tmpPosEndPtr++;
        *tmpPosEndPtr = 0;
    }
    
    len = strlen(tmpPosStartPtr) + 1;
    m_Fields[index].TypeName = new char[len];
    memcpy(m_Fields[index].TypeName, tmpPosStartPtr, len);

    delete[] sql;
    m_Fields[index].NotNull = IS_NOT_NULL_FIELD(feildSql);
    m_Fields[index].IsPrimaryKey = IS_PRIMARY_KEY_FIELD(feildSql);
    m_Fields[index].IsAutoincrementKey = IS_AUTOINCREMENT_FIELD(feildSql);
    m_Fields[index].IsUniqueKey = IS_UNIQUE_FIELD(feildSql);
}

/************************************************************************
* ���ܣ������������������Ӧ���ֶ����ԡ�
* ������
*       setence [IN] -> ������䡣
* ���أ��Ѿ����õ��ֶ����Ե�������
* ��ע���˲ٱ����ڳ�Ա����m_Fields�Ѿ�����ռ������¡�
************************************************************************/
int SQLiteTable::HandlePrimaryKeySetence(IN const char * setence)
{
    int count = 0;
    const char * feildName = strchr(setence, '(');
    const char * feildNameEnd = NULL;
    if (!feildName)
    {
        return 0;
    }
    feildName += 1;

    do
    {
        feildNameEnd = strchr(feildName, ',');
        if (!feildNameEnd)
        {
            feildNameEnd = strchr(feildName, ')');
        }
        feildNameEnd -= 1;
        int len = feildNameEnd - feildName + 1;

        for (UINT i = 0; i < m_FieldCount; ++i)
        {
            if (!_strnicmp(m_Fields[i].FieldName, feildName, len) && !m_Fields[i].FieldName[len])
            {
                m_Fields[i].IsPrimaryKey = TRUE;
                count++;
                break;
            }
        }
        feildName = feildNameEnd + 2;
    } while (*(feildNameEnd + 1) != ')');

    return count;
}

/************************************************************************
* ���ܣ�����������������Ӧ���ֶ����ԡ�
* ������
*       setence [IN] -> �����䡣
* ���أ���ǰ����е�����Ƿ����óɹ���
* ��ע���˲ٱ����ڳ�Ա����m_Fields�Ѿ�����ռ������¡�
************************************************************************/
BOOL SQLiteTable::HandleForeignKeySetence(IN const char * setence)
{
    const char * feildName = strchr(setence, '(');
    const char * feildNameEnd = strchr(setence, ')');

    if (!feildName || !feildNameEnd)
    {
        return FALSE;
    }
    
    feildName += 1;
    feildNameEnd -= 1;

    int len = feildNameEnd - feildName + 1;

    for (UINT i = 0; i < m_FieldCount; ++i)
    {
        if (!_strnicmp(m_Fields[i].FieldName, feildName, len) && !m_Fields[i].FieldName[len])
        {
            m_Fields[i].IsForeignKey = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

/************************************************************************
* ���ܣ�����Ψһ��ֵ���������Ӧ���ֶ����ԡ�
* ������
*       setence [IN] -> Ψһ��ֵ��䡣
* ���أ��Ѿ����õ��ֶ����Ե�������
* ��ע���˲ٱ����ڳ�Ա����m_Fields�Ѿ�����ռ������¡�
************************************************************************/
BOOL SQLiteTable::HandleUniqueKeySetence(IN const char * setence)
{
    int count = 0;
    const char * feildName = strchr(setence, '(');
    const char * feildNameEnd = NULL;
    if (!feildName)
    {
        return 0;
    }
    feildName += 1;

    do
    {
        feildNameEnd = strchr(feildName, ',');
        if (!feildNameEnd)
        {
            feildNameEnd = strchr(feildName, ')');
        }
        feildNameEnd -= 1;
        int len = feildNameEnd - feildName + 1;

        for (UINT i = 0; i < m_FieldCount; ++i)
        {
            if (!_strnicmp(m_Fields[i].FieldName, feildName, len) && !m_Fields[i].FieldName[len])
            {
                m_Fields[i].IsUniqueKey = TRUE;
                count++;
                break;
            }
        }
        feildName = feildNameEnd + 2;
    } while (*(feildNameEnd + 1) != ')');

    return count;
}

/************************************************************************
* ���ܣ����췽������ʼ���ࡣ
* ������
*       tableCell [IN] -> ָ����ĵ�Ԫ��
* ���أ��ޡ�
************************************************************************/
SQLiteTable::SQLiteTable(IN SQLiteTableCell * tableCell)
{
    //��ʼ����Ա����
	char * createSql = NULL;
    m_FieldCount = 0;
    m_Fields = NULL;
	tableCell->GetObjectName(m_TableName);
	tableCell->GetCreateSQL(createSql);

	if (!createSql)
	{
		return;
	}
    //ȡ�ӵ�һ��'('��ʼ���ַ���
    char * tmpPtrPos = strchr(createSql, '(');
    //�������ַ���������ֱ�ӷ���
    if (tmpPtrPos == NULL)
    {
		if (createSql)
		{
			delete[] createSql;
		}
        return;
    }

    //�����ַ������ȣ��ӵ�һ��'('֮��ʼ
    int tmpLen = strlen(tmpPtrPos + 1);

    //ΪĿ��sql����ռ�
    char * sql = new char[tmpLen];
    //�������µĿռ���
    memcpy(sql, (void *)(tmpPtrPos + 1), tmpLen);
    //������sql��β�ж�Ӧ��ͷ��')'���������������洢�ַ���������
    sql[tmpLen - 1] = 0;
    //��ʽ��SQL����ַ���
    FormatSql(sql);
    //////////////////////////////////////////////////////////////////////////
    //�����￪ʼ�����ȡ��sql�ַ���
    //////////////////////////////////////////////////////////////////////////
    //�ֶ������������Ϊ����һ�������Դ�1��ʼ
    int setenceCount = 1;
    //���ű�־
    int bracketFlag = 0;
    //��ʱָ��ǰ������ֶ���俪ʼλ��
    char * tmpSetenceStart = sql;
    //��ʱָ�룬���ڱ���SQL�ַ���
    tmpPtrPos = sql;
    //һֱ�������ַ�����β
    while (*tmpPtrPos)
    {
        //�������ű�־+1
        if (*tmpPtrPos == '(')
        {
            bracketFlag++;
        }
        //�뿪���ű�־-1
        else if (*tmpPtrPos == ')')
        {
            bracketFlag--;
        }
        //������������(�����ű�־λ0)���ٽ��зָ�
        else if (!bracketFlag && *tmpPtrPos == ',')
        {
            //���+1
            setenceCount++;
            //������Ϊ0��Ϊ�˷����鵱ǰ����Ƿ����ֶ��������
            *tmpPtrPos = 0;
            //////////////////////////////////////////////////////////////////////////
            //�����������������Ҳ������������������䣬˵�����ֶ��������
            if (!IS_FOREIGN_KEY_SETENCE(tmpSetenceStart) && !IS_CONSTRAINT_SETENCE(tmpSetenceStart) && !IS_UNIQUE_SETENCE(tmpSetenceStart))
            {
                //�ֶμ���+1
                m_FieldCount++;
            }
            //////////////////////////////////////////////////////////////////////////
            //�ٻ�ԭ����ΪֹΪ','
            *tmpPtrPos = ',';
            //ָ����һ�����Ŀ�ʼ
            tmpSetenceStart = tmpPtrPos + 1;
        }
        //ָ����һ���ַ�
        tmpPtrPos++;
    }
    //�����������һ�䣬��Ϊ�������ַ���������Ϊ��β���������ѭ���жϽ�β','��ͬ
    if (!IS_FOREIGN_KEY_SETENCE(tmpSetenceStart) && !IS_CONSTRAINT_SETENCE(tmpSetenceStart) && !IS_UNIQUE_SETENCE(tmpSetenceStart))
    {
        //�ֶμ���+1
        m_FieldCount++;
    }
    //////////////////////////////////////////////////////////////////////////
    //����������֪������������Լ��ֶ���������������
    //////////////////////////////////////////////////////////////////////////
    //������洢�ֶεĿռ�
    m_Fields = new SqlFeild[m_FieldCount];
    //////////////////////////////////////////////////////////////////////////
    //�ֶ�����
    int feildsIndex = 0;
    //������������һ�䵥���������������1��ʼ
    int setenceIndex = 1;
    //ָ��һ�����Ŀ�ʼ�������ߴ��������
    tmpSetenceStart = sql;
    //���ű�ʾ
    bracketFlag = 0;
    //�ַ�ָ�룬���ڱ����ַ���
    tmpPtrPos = sql;
    //ѭ���������
    while (setenceIndex < setenceCount)
    {
        //�������ű�־+1
        if (*tmpPtrPos == '(')
        {
            bracketFlag++;
        }
        //�뿪���ű�־-1
        else if (*tmpPtrPos == ')')
        {
            bracketFlag--;
        }
        //������������(�����ű�־λ0)���������ָ��','��˵��������һ�����û��ʼ����
        else if (!bracketFlag && *tmpPtrPos == ',')
        {
            //��������
            setenceIndex++;
            //�����ַ��������������㴦��
            *tmpPtrPos = 0;
            //����������������������Ĵ���
            if (IS_FOREIGN_KEY_SETENCE(tmpSetenceStart))
            {
                HandleForeignKeySetence(tmpSetenceStart);
            }
            //������������������������Ĵ���
            else if (IS_CONSTRAINT_SETENCE(tmpSetenceStart))
            {
                HandlePrimaryKeySetence(tmpSetenceStart);
            }
            //�����Ψһ��ֵ��������Ψһ��ֵ���Ĵ���
            else if (IS_UNIQUE_SETENCE(tmpSetenceStart))
            {
                HandleUniqueKeySetence(tmpSetenceStart);
            }
            //�����������������Ҳ������������������䣬˵�����ֶ��������
            else
            {
                InitSqlFeild(feildsIndex++, tmpSetenceStart);
            }
            //ָ����һ�����Ŀ�ʼ
            tmpSetenceStart = ++tmpPtrPos;
        }
        //ָ����һ���ַ�
        tmpPtrPos++;
    }
    //�����������һ�䣬��Ϊ�������ַ���������Ϊ��β���������ѭ���жϽ�β','��ͬ
    if (IS_FOREIGN_KEY_SETENCE(tmpSetenceStart))
    {
        HandleForeignKeySetence(tmpSetenceStart);
    }
    else if (IS_CONSTRAINT_SETENCE(tmpSetenceStart))
    {
        HandlePrimaryKeySetence(tmpSetenceStart);
    }
    else if (IS_UNIQUE_SETENCE(tmpSetenceStart))
    {
        HandleUniqueKeySetence(tmpSetenceStart);
    }
    else
    {
        InitSqlFeild(feildsIndex++, tmpSetenceStart);
    }

    //////////////////////////////////////////////////////////////////////////
    //�ͷŴ���sql������Ŀռ�
    delete[] sql;

	if (createSql)
	{
		delete[] createSql;
	}
}



/************************************************************************
* ���ܣ������������ͷ����ݡ�
* �������ޡ�
* ���أ��ޡ�
************************************************************************/
SQLiteTable::~SQLiteTable()
{
	if (m_TableName)
	{
		delete[] m_TableName;
	}
    if (m_Fields)
    {
        while (m_FieldCount--)
        {
            if (m_Fields[m_FieldCount].FieldName)
            {
                delete[] m_Fields[m_FieldCount].FieldName;
            }
            if (m_Fields[m_FieldCount].TypeName)
            {
                delete[] m_Fields[m_FieldCount].TypeName;
            }
        }
        delete[] m_Fields;
    }
}

/************************************************************************
* ���ܣ���ȡ������
* �������ޡ�
* ���أ�������
* ��ע������ֵ�����޸ģ�Ҳ���õ����ͷš�
************************************************************************/
const char * SQLiteTable::GetTableName()
{
	return m_TableName;
}

/************************************************************************
* ���ܣ���ȡ�ֶ�������
* �������ޡ�
* ���أ��ֶ�������
************************************************************************/
UINT SQLiteTable::GetFieldCount()
{
    return m_FieldCount;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ֶ����ơ�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ��ֶ����Ƶ��ַ�����
* ��ע������ֵ�����޸ģ�Ҳ���õ����ͷš�
************************************************************************/
const char * SQLiteTable::GetFieldName(IN UINT index)
{
    return m_Fields[index].FieldName;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε��ֶ����͡�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ��ֶ����͵��ַ�����
* ��ע������ֵ��Ҫ�޸ģ�Ҳ��Ҫ�����ͷš�
************************************************************************/
const char * SQLiteTable::GetTypeName(IN UINT index)
{
    return m_Fields[index].TypeName;
}

/************************************************************************
* ���ܣ���ȡָ���ֶ���SQLite�еĽ������͡�
* ������
*       index [IN] -> ָ���ֶε�������
* ���أ�ָ���ֶ���SQLite�еĽ������͡�
************************************************************************/
SQLiteDataType SQLiteTable::GetSimilarType(IN UINT index)
{
    if (strstr(m_Fields[index].TypeName, "bool") != NULL)
    {
        return SQLiteDataType::SDT_BOOLEAN;
    }
    else if (strstr(m_Fields[index].TypeName, "int") != NULL)
    {
        return SQLiteDataType::SDT_INTEGER;
    }
    else if (strstr(m_Fields[index].TypeName, "real") != NULL || strstr(m_Fields[index].TypeName, "double") != NULL || strstr(m_Fields[index].TypeName, "float") != NULL || strstr(m_Fields[index].TypeName, "decimal") != NULL)
    {
        return SQLiteDataType::SDT_REAL;
    }
    else if (strstr(m_Fields[index].TypeName, "char") != NULL || !strncmp(m_Fields[index].TypeName, "text", 4) || !strncmp(m_Fields[index].TypeName, "clob", 4))
    {
        return SQLiteDataType::SDT_TEXT;
    }
    else if (!strcmp(m_Fields[index].TypeName, "blob"))
    {
        return SQLiteDataType::SDT_BLOB;
    }
    return SQLiteDataType::SDT_UNKNOWN;
}

/************************************************************************
* ���ܣ���ȡָ���ֶε����ԡ�
* ������
*       index      [IN] -> ָ���ֶε�������
*       properties [IN] -> Ҫ��������(��SFP_NOP�����ʹ��'|'����)��
* ���أ�ָ�������Դ��ڷ���TRUE��ֻҪ��һ�������ھͷ���FALSE��
* ע�⣺����properties�����Դ���SQLiteFeildProperty���ͼ��ɡ�
************************************************************************/
BOOL SQLiteTable::GetFeildProperty(IN UINT index, IN int properties)
{
    return !((properties == SFP_NOP && (m_Fields[index].NotNull || m_Fields[index].IsPrimaryKey || m_Fields[index].IsAutoincrementKey || m_Fields[index].IsForeignKey)) || 
        (SFP_PRIMARY_KEY & properties && !m_Fields[index].IsPrimaryKey) || 
        (SFP_AUTOINCREMENT & properties && !m_Fields[index].IsAutoincrementKey) || 
        (SFP_NOT_NULL & properties && !m_Fields[index].NotNull) || 
        (SFP_FOREIGN_KEY & properties && !m_Fields[index].IsForeignKey) || 
        (SFP_UNIQUE_KEY & properties && !m_Fields[index].IsUniqueKey)
        );
}

