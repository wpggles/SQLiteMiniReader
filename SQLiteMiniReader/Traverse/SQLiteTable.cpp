//
// @Project : SQLiteMiniReader
// @File    : SQLiteTable.cpp
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#include "SQLiteTable.h"

/************************************************************************
* 功能：格式化SQL语句，转换为小写，去除其中的重复空格，删除制表和换行符
*       这类空白字符。
* 参数：无。
*       sqlString [IN_OUT] -> 要格式化的SQL语句。
* 返回：无。
* 备注：此操作是在原字符串的基础上进行直接修改，会影响原字符串。
*       对于使用单引号区分大小写的部分不会转换为小写。
************************************************************************/
void SQLiteTable::FormatSql(IN_OUT char * sqlString)
{
    //因为是为了格式化SQL，去除其中的重复空格，删除制表和换行符这类空白字符，所以在元字符串上操作。
    //原字符串扫描位置指针
    char * orgPos = sqlString;
    //当前字符串写入位置指针
    char * curPos = sqlString;
    //指示上一个字符是否有空白字符
    BOOL lastIsSpace = FALSE;
    //指示上一个字符是否是分割字符(',' , '(' , ')')
    BOOL lastIsSplite = TRUE;
    //指示当前字符串是否是分割字符(',' , '(' , ')')
    BOOL currentIsSplite = FALSE;
    //分号标志，对于分号内的大小写不转换
    BOOL semicolonFlag = FALSE;

    //循环遍历源字符串
    while (*orgPos)
    {
        //判断是否是空白字符
        if (IS_SPACE_CHAR(*orgPos))
        {
            //修改空白字符标记
            lastIsSpace = TRUE;
            //指针自增
            orgPos++;
            //不往下执行，继续循环判断下一个
            continue;
        }
        //判断当前是否是分割字符
        currentIsSplite = IS_SPLITE_CHAR(*orgPos);
        //如果执行到这里，说明当前的这个不是空白字符，所以要判断一下之前有没有空白字符，如果之前有空白字符，看情况是否添加，并修改标记
        if (lastIsSpace)
        {
            //修改标记
            lastIsSpace = FALSE;

            //一下这个if结构是对上面这个对段落注释的精简写法
            if (*orgPos != ',' && (!lastIsSplite || !currentIsSplite) && 
                ((!lastIsSplite && currentIsSplite && *orgPos == '(') || 
                (lastIsSplite && !currentIsSplite && *(curPos - 1) == ')') || 
                (!lastIsSplite && !currentIsSplite)))
            {
                *curPos = ' ';
                curPos++;
            }
        }
        //记录当前是否是分隔符来作为下一次判断上一个是否是分隔符的依据
        lastIsSplite = currentIsSplite;
        
        //执行到这里只需要进行赋值
        *curPos = *orgPos;
        //判断是否出现单引号
        if (*curPos == '\'')
        {
            semicolonFlag = !semicolonFlag;
        }
        //如果不在单引号内就将所有大写字母转换为小写
        else if (!semicolonFlag && (*curPos >= 0x41 && *curPos <= 0x5A))
        {
            *curPos += 0x20;
        }
        //指针指向下一个位置
        orgPos++;
        curPos++;
        
    }
    //再格式化后的字符串末尾增加字符串结束符
    *curPos = 0;

}

/************************************************************************
* 功能：初始化指定索引的字段属性。
* 参数：
*       index    [IN] -> 指定初始化字段属性的索引。
*       feildSql [IN] -> 用于初始化的SQL语句片段。
* 返回：无。
* 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
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
* 功能：处理主键语句设置相应的字段属性。
* 参数：
*       setence [IN] -> 主键语句。
* 返回：已经设置的字段属性的数量。
* 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
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
* 功能：处理外键语句设置相应的字段属性。
* 参数：
*       setence [IN] -> 外键语句。
* 返回：当前语句中的外键是否设置成功。
* 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
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
* 功能：处理唯一键值语句设置相应的字段属性。
* 参数：
*       setence [IN] -> 唯一键值语句。
* 返回：已经设置的字段属性的数量。
* 备注：此操必须在成员变量m_Fields已经申请空间的情况下。
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
* 功能：构造方法，初始化类。
* 参数：
*       tableCell [IN] -> 指定表的单元。
* 返回：无。
************************************************************************/
SQLiteTable::SQLiteTable(IN SQLiteTableCell * tableCell)
{
    //初始化成员属性
	char * createSql = NULL;
    m_FieldCount = 0;
    m_Fields = NULL;
	tableCell->GetObjectName(m_TableName);
	tableCell->GetCreateSQL(createSql);

	if (!createSql)
	{
		return;
	}
    //取从第一个'('开始的字符串
    char * tmpPtrPos = strchr(createSql, '(');
    //如果这个字符串不存在直接返回
    if (tmpPtrPos == NULL)
    {
		if (createSql)
		{
			delete[] createSql;
		}
        return;
    }

    //计算字符串长度，从第一个'('之后开始
    int tmpLen = strlen(tmpPtrPos + 1);

    //为目标sql分配空间
    char * sql = new char[tmpLen];
    //拷贝到新的空间中
    memcpy(sql, (void *)(tmpPtrPos + 1), tmpLen);
    //完整的sql结尾有对应开头的')'，所以正好用来存储字符串结束符
    sql[tmpLen - 1] = 0;
    //格式化SQL语句字符串
    FormatSql(sql);
    //////////////////////////////////////////////////////////////////////////
    //从这里开始处理截取的sql字符串
    //////////////////////////////////////////////////////////////////////////
    //字段语句数量，因为至少一条，所以从1开始
    int setenceCount = 1;
    //括号标志
    int bracketFlag = 0;
    //临时指向当前处理的字段语句开始位置
    char * tmpSetenceStart = sql;
    //临时指针，用于遍历SQL字符串
    tmpPtrPos = sql;
    //一直遍历到字符串结尾
    while (*tmpPtrPos)
    {
        //进入括号标志+1
        if (*tmpPtrPos == '(')
        {
            bracketFlag++;
        }
        //离开括号标志-1
        else if (*tmpPtrPos == ')')
        {
            bracketFlag--;
        }
        //当不在括号内(即括号标志位0)，再进行分割
        else if (!bracketFlag && *tmpPtrPos == ',')
        {
            //语句+1
            setenceCount++;
            //将此置为0，为了方便检查当前语句是否是字段声明语句
            *tmpPtrPos = 0;
            //////////////////////////////////////////////////////////////////////////
            //如果不是外键声明语句也不是联合主键声明语句，说明是字段声明语句
            if (!IS_FOREIGN_KEY_SETENCE(tmpSetenceStart) && !IS_CONSTRAINT_SETENCE(tmpSetenceStart) && !IS_UNIQUE_SETENCE(tmpSetenceStart))
            {
                //字段计数+1
                m_FieldCount++;
            }
            //////////////////////////////////////////////////////////////////////////
            //再还原结束为止为','
            *tmpPtrPos = ',';
            //指向下一个语句的开始
            tmpSetenceStart = tmpPtrPos + 1;
        }
        //指向下一个字符
        tmpPtrPos++;
    }
    //单独处理最后一句，因为它是以字符串结束符为结尾，跟上面的循环判断结尾','不同
    if (!IS_FOREIGN_KEY_SETENCE(tmpSetenceStart) && !IS_CONSTRAINT_SETENCE(tmpSetenceStart) && !IS_UNIQUE_SETENCE(tmpSetenceStart))
    {
        //字段计数+1
        m_FieldCount++;
    }
    //////////////////////////////////////////////////////////////////////////
    //到这里我们知道了所有语句以及字段声明命语句的数量
    //////////////////////////////////////////////////////////////////////////
    //申请出存储字段的空间
    m_Fields = new SqlFeild[m_FieldCount];
    //////////////////////////////////////////////////////////////////////////
    //字段索引
    int feildsIndex = 0;
    //语句索引，最后一句单独处理，所以这里从1开始
    int setenceIndex = 1;
    //指向一个语句的开始，方便后边处理单条语句
    tmpSetenceStart = sql;
    //括号标示
    bracketFlag = 0;
    //字符指针，用于遍历字符串
    tmpPtrPos = sql;
    //循环遍历语句
    while (setenceIndex < setenceCount)
    {
        //进入括号标志+1
        if (*tmpPtrPos == '(')
        {
            bracketFlag++;
        }
        //离开括号标志-1
        else if (*tmpPtrPos == ')')
        {
            bracketFlag--;
        }
        //当不在括号内(即括号标志位0)，且遇到分割符','，说明遍历完一条语句没开始处理
        else if (!bracketFlag && *tmpPtrPos == ',')
        {
            //索引增加
            setenceIndex++;
            //增加字符串结束符，方便处理
            *tmpPtrPos = 0;
            //如果是外键语句则进行外键语句的处理
            if (IS_FOREIGN_KEY_SETENCE(tmpSetenceStart))
            {
                HandleForeignKeySetence(tmpSetenceStart);
            }
            //如果是主键语句则进行主键语句的处理
            else if (IS_CONSTRAINT_SETENCE(tmpSetenceStart))
            {
                HandlePrimaryKeySetence(tmpSetenceStart);
            }
            //如果是唯一键值语句则进行唯一键值语句的处理
            else if (IS_UNIQUE_SETENCE(tmpSetenceStart))
            {
                HandleUniqueKeySetence(tmpSetenceStart);
            }
            //如果不是外键声明语句也不是联合主键声明语句，说明是字段声明语句
            else
            {
                InitSqlFeild(feildsIndex++, tmpSetenceStart);
            }
            //指向下一个语句的开始
            tmpSetenceStart = ++tmpPtrPos;
        }
        //指向下一个字符
        tmpPtrPos++;
    }
    //单独处理最后一句，因为它是以字符串结束符为结尾，跟上面的循环判断结尾','不同
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
    //释放处理sql而申请的空间
    delete[] sql;

	if (createSql)
	{
		delete[] createSql;
	}
}



/************************************************************************
* 功能：析构方法，释放数据。
* 参数：无。
* 返回：无。
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
* 功能：获取表名。
* 参数：无。
* 返回：表名。
* 备注：返回值不可修改，也不用单独释放。
************************************************************************/
const char * SQLiteTable::GetTableName()
{
	return m_TableName;
}

/************************************************************************
* 功能：获取字段数量。
* 参数：无。
* 返回：字段数量。
************************************************************************/
UINT SQLiteTable::GetFieldCount()
{
    return m_FieldCount;
}

/************************************************************************
* 功能：获取指定字段的字段名称。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：字段名称的字符串。
* 备注：返回值不可修改，也不用单独释放。
************************************************************************/
const char * SQLiteTable::GetFieldName(IN UINT index)
{
    return m_Fields[index].FieldName;
}

/************************************************************************
* 功能：获取指定字段的字段类型。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：字段类型的字符串。
* 备注：返回值不要修改，也不要单独释放。
************************************************************************/
const char * SQLiteTable::GetTypeName(IN UINT index)
{
    return m_Fields[index].TypeName;
}

/************************************************************************
* 功能：获取指定字段在SQLite中的近似类型。
* 参数：
*       index [IN] -> 指定字段的索引。
* 返回：指定字段在SQLite中的近似类型。
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
* 功能：获取指定字段的属性。
* 参数：
*       index      [IN] -> 指定字段的索引。
*       properties [IN] -> 要检查的属性(除SFP_NOP外可以使用'|'连接)。
* 返回：指定的属性存在返回TRUE，只要有一个不存在就返回FALSE。
* 注意：这里properties的属性传递SQLiteFeildProperty类型即可。
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

