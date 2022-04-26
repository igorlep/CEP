//---------------------------------------------------------------------------

#ifndef DifferentFunctionsH
#define DifferentFunctionsH
//---------------------------------------------------------------------------
#include "DBClasses.h"
//---------------------------------------------------------------------------
//������� ����������� ���������� ��� ������
unsigned short __fastcall lastDayOfMonth(unsigned short y, unsigned short m);
//������ ������
TDateTime __fastcall beginOfMonth(TDateTime date);
//����� ������
TDateTime __fastcall endOfMonth(TDateTime date);
//��������� ������� ��� ������ � �����
AnsiString __fastcall getMonthYearString(TDateTime date);
TDateTime __fastcall getDateFromMonthYearString(AnsiString myStr);

//������� ���������� ��������� �� ������ � �������������
AnsiString __fastcall getSubstring(AnsiString str, char r, int n);

//������� ��� ������ � �������� ������
// - ��������� ������ �������� ��������� ���� ��� tableName
AnsiString __fastcall getNewKeyValue(FBDatabase * fb_db, AnsiString tableName);
AnsiString __fastcall getNewKeyValue(FBQuery * fbWQ, AnsiString tableName);
void __fastcall setGenerator(FBQuery *fbWQ,AnsiString tableName,AnsiString genName,AnsiString keyName);

// - ��������� �������� ���� fieldName ������� tableName �� �������� keyData ��������� ���� keyFieldName
//AnsiString __fastcall getDataForFieldName(FBDatabase * fbdb,AnsiString tableName,AnsiString keyFieldName,AnsiString keiData, AnsiString fieldName);

//������� ��� ������ � �������� ��������
AnsiString __fastcall getConstName(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
AnsiString __fastcall getConstTitle(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
AnsiString __fastcall getConstValue(TComponent* owner,FBDatabase * fb_db,AnsiString identifer,TDateTime date=0);
int __fastcall getConstType(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
int __fastcall getConstKind(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);

void  __fastcall setConstValue(FBDatabase * fb_db,AnsiString identifer, AnsiString constValue);
bool  __fastcall setPeriodicalConstValue(FBDatabase * fb_db,AnsiString const_id,int &value_id,AnsiString constValue,TDateTime constDate);
void  __fastcall delPeriodicalConstValue(FBDatabase * fb_db,AnsiString value_id);

//������� �������� ����������� �������������� ������ �� ���������� ���� ��� ����������� ������������
bool checkPossibileEditingData(FBDatabase * fb_db,TDateTime actualDate,int userID);

//������� ��� ������ � �������������� �����������
AnsiString __fastcall getPeriodicalParamValue(FBDatabase * fb_db,AnsiString tableInfo,AnsiString keyID,TDateTime forDate);

//������� �������������� ����� � ������ ��������
//������� �������������� ������������� ����� � ������������ �������� ���. � ���.
AnsiString f_to_str(double d);
AnsiString i_to_str(int i);

//�������, ������������ ���������� ������������� - GUID
AnsiString __fastcall genGUID(void);

//������� ������������ ������ ���������
AnsiString SetHeader(AnsiString str, TDateTime date1, TDateTime date2);

#endif
