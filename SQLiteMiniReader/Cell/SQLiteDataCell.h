//
// @Project : SQLiteMiniReader
// @File    : SQLiteDataCell.h
// @Date    : 2016-02-20
// @Author  : wPgg1es
//

#ifndef _SQLITE_DATA_CELL_H_
#define _SQLITE_DATA_CELL_H_

#include "SQLiteCell.h"

//SQLite数据单元类
class SQLiteDataCell : public SQLiteCell
{
private:
    /************************************************************************
    * 功能：私有构造方法，初始化类。
    * 参数：无。
    * 返回：无。
    * 备注：给定的数据必须完整，超过一个Cell的数据会自动忽略。
    ************************************************************************/
    SQLiteDataCell() : SQLiteCell(NULL, NULL, 0, 0){};
public:
    /************************************************************************
    * 功能：析构方法，释放资源。
    * 参数：无。
    * 返回：无。
    ************************************************************************/
    ~SQLiteDataCell(){};

    /************************************************************************
    * 功能：获取当前单元在所在表中的RowID。
    * 参数：无。
    * 返回：RowID。
    ************************************************************************/
    ULONG GetRowID();

    /************************************************************************
    * 功能：获取当前单元的字段列数。
    * 参数：无。
    * 返回：字段列数。
    ************************************************************************/
    ULONG GetColumnCount();

    /************************************************************************
    * 功能：获取指定列的字段类型。
    * 参数：
    *       index [IN] -> 指定列字段索引。
    * 返回：SQLite数据类型。
    * 备注：根据数据来判断的，不准确，仅供参考。
    *       这里返回布尔型也有可能是数字，只不过数字可能是0或1。
    ************************************************************************/
    SQLiteDataType GetTpye(IN int index);

    /************************************************************************
    * 功能：获取指定字段的布尔型数据。
    * 参数：
    *       index [IN] -> 指定的字段索引。
    * 返回：布尔值。
    * 备注：小于256并且的非0为TRUE，否则为FALSE。
    ************************************************************************/
    BOOL GetBoolean(IN int index);

    /************************************************************************
    * 功能：获取指定字段的整数型数据。
    * 参数：
    *       index [IN] -> 指定的字段索引。
    * 返回：转换后的整数。
    ************************************************************************/
    LONG GetInteger(IN int index);

    /************************************************************************
    * 功能：获取指定字段的浮点型数据。
    * 参数：
    *       index [IN] -> 指定的字段索引。
    * 返回：转换后的浮点数。
    ************************************************************************/
    double GetReal(IN int index);

    /************************************************************************
    * 功能：获取指定字段的文本型数据。
    * 参数：
    *       index     [IN]  -> 指定的字段索引。
    *       bufferPtr [OUT] -> 存储返回数据的指针。
    * 返回：文本长度。
    * 注意：bufferPtr 只需要一个字符串指针，函数内申请空间，使用完要释放掉。
    ************************************************************************/
    UINT GetText(IN int index, OUT char * & bufferPtr);

    /************************************************************************
    * 功能：获取指定字段的二进制型数据。
    * 参数：
    *       index     [IN]  -> 指定的字段索引。
    *       bufferPtr [OUT] -> 存储返回数据的指针。
    * 返回：二进制型数据长度。
    * 注意：bufferPtr 只需要一个BYTE指针，函数内申请空间，使用完要释放掉。
    ************************************************************************/
    UINT GetBLOB(IN int index, OUT BYTE * & bufferPtr);
};

#endif
