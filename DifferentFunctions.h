//---------------------------------------------------------------------------

#ifndef DifferentFunctionsH
#define DifferentFunctionsH
//---------------------------------------------------------------------------
#include "DBClasses.h"
//---------------------------------------------------------------------------
//Функция определения последнего дня месяца
unsigned short __fastcall lastDayOfMonth(unsigned short y, unsigned short m);
//Начало месяца
TDateTime __fastcall beginOfMonth(TDateTime date);
//Конец месяца
TDateTime __fastcall endOfMonth(TDateTime date);
//Различные функции для работы с датой
AnsiString __fastcall getMonthYearString(TDateTime date);
TDateTime __fastcall getDateFromMonthYearString(AnsiString myStr);

//Функция извлечения подстроки из строки с разделителями
AnsiString __fastcall getSubstring(AnsiString str, char r, int n);

//Функции для работы с таблицей данных
// - Генерация нового значения ключевого поля для tableName
AnsiString __fastcall getNewKeyValue(FBDatabase * fb_db, AnsiString tableName);
AnsiString __fastcall getNewKeyValue(FBQuery * fbWQ, AnsiString tableName);
void __fastcall setGenerator(FBQuery *fbWQ,AnsiString tableName,AnsiString genName,AnsiString keyName);

// - Получение значения поля fieldName таблицы tableName по значению keyData ключевого поля keyFieldName
//AnsiString __fastcall getDataForFieldName(FBDatabase * fbdb,AnsiString tableName,AnsiString keyFieldName,AnsiString keiData, AnsiString fieldName);

//Функции для работы с таблицей констант
AnsiString __fastcall getConstName(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
AnsiString __fastcall getConstTitle(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
AnsiString __fastcall getConstValue(TComponent* owner,FBDatabase * fb_db,AnsiString identifer,TDateTime date=0);
int __fastcall getConstType(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);
int __fastcall getConstKind(TComponent* owner,FBDatabase * fb_db,AnsiString identifer);

void  __fastcall setConstValue(FBDatabase * fb_db,AnsiString identifer, AnsiString constValue);
bool  __fastcall setPeriodicalConstValue(FBDatabase * fb_db,AnsiString const_id,int &value_id,AnsiString constValue,TDateTime constDate);
void  __fastcall delPeriodicalConstValue(FBDatabase * fb_db,AnsiString value_id);

//Функция проверки возможности редактирования данных на актуальную дату для конкретного пользователя
bool checkPossibileEditingData(FBDatabase * fb_db,TDateTime actualDate,int userID);

//Функции для работы с париодическими параметрами
AnsiString __fastcall getPeriodicalParamValue(FBDatabase * fb_db,AnsiString tableInfo,AnsiString keyID,TDateTime forDate);

//Функции преобразования чисел в строку прописью
//Функция преобразования вещественного числа в числительное прописью руб. и коп.
AnsiString f_to_str(double d);
AnsiString i_to_str(int i);

//Функция, генерирующая уникальный идентификатор - GUID
AnsiString __fastcall genGUID(void);

//Функция формирования строки заголовка
AnsiString SetHeader(AnsiString str, TDateTime date1, TDateTime date2);

#endif
