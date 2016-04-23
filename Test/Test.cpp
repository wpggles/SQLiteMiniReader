// Test.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include "../SQLiteMiniReader/SQLiteMiniReader.h"
using namespace std;

BOOL ProcTable(SQLiteTable * table, void * pVoid)
{
	cout << "GetTableName : " << table->GetTableName() << endl;
	return TRUE;
}

BOOL ProcCell(SQLiteRecord * record, void * pVoid)
{
	cout << "--Field count : " << record->GetFieldCount() << endl;
	for (UINT i = 0; i < record->GetFieldCount(); ++i)
	{
		cout << record->GetFieldName(i) << "(" << record->GetFeildTypeName(i) << ") : ";

		switch (record->GetFeildSimilarType(i))
		{
			case SDT_BOOLEAN:
				cout << record->GetBoolean(i);
				break;
			case SDT_INTEGER:
				cout << record->GetInteger(i);
				break;
			case SDT_REAL:
				cout << record->GetReal(i);
				break;
			case SDT_TEXT:
			{
				char * tmpChars = NULL;
				cout << "(" << record->GetText(i, tmpChars) << ") ";
				if (tmpChars)
				{
					cout << tmpChars;
					delete[] tmpChars;
				}
				break;
			}
			case SDT_BLOB:
			{
				BYTE * tmpBytes = NULL;
				cout << "(" << record->GetBLOB(i, tmpBytes) << ")BLOB";
				if (tmpBytes)
				{
					delete[] tmpBytes;
				}
				break;
			}
			case SDT_DBNULL:
				cout << "SDT_DBNULL";
				break;
			case SDT_UNKNOWN:
				if (!_stricmp("datetime", record->GetFeildTypeName(i)))
				{
					char * tmpChars = NULL;
					cout << "(" << record->GetText(i, tmpChars) << ") ";
					if (tmpChars)
					{
						cout << tmpChars;
						delete[] tmpChars;
					}
				}
				else
				{
					cout << "SDT_UNKNOWN";
				}
			break;
		}
		cout << endl;
	}
	cout << "--RowID : " << record->GetRowID() << endl;
	cout << "================================" << endl;
	return TRUE;
}

int main()
{
	SQLiteMiniReader sqliteReader("test.db");

	SQLiteFileHeader * info = sqliteReader.GetFileInfo();
	cout << "info.GetHeaderString() : " << info->GetHeaderString() << endl;
	cout << "info.GetPageSize() : " << info->GetPageSize() << endl;

	cout << "info.GetFileFormatWriteVersion() : " << info->GetFileFormatWriteVersion() << endl;
	cout << "info.GetFileFormatReadVersion() : " << info->GetFileFormatReadVersion() << endl;
	cout << "info.GetFileChangeCount() : " << info->GetFileChangeCount() << endl;
	cout << "info.GetPageCount() : " << info->GetPageCount() << endl;
	cout << "info.GetTotalFreelistPages() : " << info->GetTotalFreelistPages() << endl;
	cout << "info.GetSchemaCookie() : " << info->GetSchemaCookie() << endl;
	cout << "info.GetSchemaFormatNumber() : " << info->GetSchemaFormatNumber() << endl;
	cout << "info.GetDefaultPageCacheSize() : " << info->GetDefaultPageCacheSize() << endl;
	cout << "info.GetRootPageNumber() : " << info->GetRootPageNumber() << endl;
	cout << "info.GetTextEncoding() : " << info->GetTextEncoding() << endl;
	cout << "info.GetUserVersion() : " << info->GetUserVersion() << endl;
	cout << "info.GetIncrementalVacuumMode() : " << info->GetIncrementalVacuumMode() << endl;
	cout << "info.GetApplicationID() : " << info->GetApplicationID() << endl;
	cout << "info.GetVersionValidForNumber() : " << info->GetVersionValidForNumber() << endl;
	cout << "info.GetVersion() : " << info->GetVersion() << endl;
	cout << "info.GetMajorVersion() : " << info->GetMajorVersion() << endl;
	cout << "info.GetMinorVersion() : " << info->GetMinorVersion() << endl;
	cout << "info.GetReleaseNumber() : " << info->GetReleaseNumber() << endl;

	cout << "---------------TABLE---------------" << endl;
	cout << "Table Count : " << sqliteReader.QueryTableCount() << endl;
	cout << "-----------------------------------" << endl;
	sqliteReader.QueryTable(ProcTable, NULL);

	cout << "---------------UserInfo---------------" << endl;
	cout << "UserInfo Record Count : " << sqliteReader.QueryRecordCount("userinfo") << endl;
	cout << "--------------------------------------" << endl;
	sqliteReader.QueryRecord("userinfo", ProcCell, NULL);

	cout << "---------------Project---------------" << endl;
	cout << "Project Record Count : " << sqliteReader.QueryRecordCount("project") << endl;
	cout << "-------------------------------------" << endl;
	sqliteReader.QueryRecord("project", ProcCell, NULL);

	cout << "---------------PerTendering---------------" << endl;
	cout << "PerTendering Record Count : " << sqliteReader.QueryRecordCount("PerTendering") << endl;
	cout << "------------------------------------------" << endl;
	sqliteReader.QueryRecord("PerTendering", ProcCell, NULL);

	getchar();
	return 0;
}

