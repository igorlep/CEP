//---------------------------------------------------------------------------

#ifndef SomeFunctionsH
#define SomeFunctionsH
//---------------------------------------------------------------------------
#include "ChTreeView.h"
#include "MainCAP_2010.h"
#include "DBClasses.h"
//---------------------------------------------------------------------------
//********************************************
//* Функции для обмена данными между формами *
//********************************************
FBDatabase * __fastcall getFBDataBase(void);
PCUser * __fastcall getUser(void);
TMainMenu * __fastcall getMainMenu(void);
TMainCAP_2010Form * __fastcall getMainForm(void);
TStatusBar * __fastcall getMainStatusBar(void);
TFrame * __fastcall getMainWorkFrame(void);
AnsiString __fastcall getAppDir(void);
AnsiString __fastcall getUserWorkDir(void);
void __fastcall closeBase();
void __fastcall openBase();

//Дата возврата нереализованного издания на склад
TDateTime __fastcall getReturnDate(TComponent* owner,FBDatabase *fbDB,TDateTime e_date,AnsiString NMP_ID);
//Дата возврата нереализованного издания поставщику
TDateTime __fastcall getDateReturnToPublisher(TDateTime r_date);
//TDateTime __fastcall getWriteOffDate(TComponent* owner,FBDatabase *fbDB,TDateTime fact_date,AnsiString e_id);

//Возвращает 3-ю статусную панель гланого окна
TStatusPanel * __fastcall getStatusPanelMainWindow(void);
//Выводит текст в 3-ю StatusPanel MainStatusBar
void __fastcall set3MainStatusBar(AnsiString text);
//Функция, устанавливающая флаг создания потока, проверяющего дату запрета редактирования разнаряжения ГЖП
//void __fastcall setCreateLDOThread(bool flg);
//Функция, устанавливающая флаг создания потока, удаляющего файлы, открытые для просмотра
//void __fastcall setCreateTPFThread(bool flg);

//Функции для работы с таблицами foldersTableName и messagesTableName
// - функция удаления записи из таблицы foldersTableName
bool __fastcall removeFolder(TComponent *Owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString messagesTableName,AnsiString folderID);
//Функция проверяет наличие записи по полному и наименованию папки
//Если запись отсутствует, то, если параметр createRecord == true, создает запись
//Возвращает код записи
AnsiString __fastcall checkFolderInMessagesTable(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString foolFolderName,bool createRecord);
//Функция проверяет и если нет, то создаем, папки 3-го и 4-го уровня с номером года и месяца
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderName_1,AnsiString folderName_2,int year,int month);
//Функция проверяет и если нет, то создаем, папки 3-го и 4-го уровня с номером года и месяца
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString fullFolderName,int year,int month);
//Функци возвращает полный путь папки
AnsiString getFullNameFilder(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderID);

// Функции создания сообщений
// - функция создания записи о сообщении в таблице MESSAGES_TABLE
AnsiString __fastcall addMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString rName,AnsiString rMail,AnsiString subject,AnsiString content,TStringList *attachedFiles,bool reply,bool delFiles);
// - функция создания записи о сообщении в таблице RECEIVED_MESSAGES_TABLE
//AnsiString __fastcall addRMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString sMail,AnsiString subject,AnsiString content,TStringList *attachedFiles);

//Функция копирования шаблона
bool __fastcall templateCopy(AnsiString fin, AnsiString fout);

//Функции для работы с накладной Торг-13
// - печать накладной
void __fastcall printTorg13(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *lDS,TpFIBDataSet *rDS,int type,int kind);
// type=1 - накладная на отгрузку, type=2 - накладная на возврат
// kind=1 - полная накладная, kind=2 - выборочная накладная
// - головная функция формирования сообщений с накладной Торг-13
void createTorg13Message(TComponent *owner,AnsiString iniFileName,int type,int kind);
// type=1 - накладная на отгрузку, type=2 - накладная на возврат
// kind=1 - полная накладная, kind=2 - выборочная накладная
// - функция формирования сообщений
bool __fastcall createTorg13MessagesForSubdivisions(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG);
// - функция формирования файла с накладной Торг-13
AnsiString __fastcall createTorg13File(TComponent *owner,int type,int kind,AnsiString dirName);
// - функция вывода строки в Excel файл с накладной Торг-13
void __fastcall putStr(int type,int ns, class DirectWorkExcel *de, TDataSet *DS);

//функции для работы с набором прав пользователя
// - проверка права ползователя на пункт меню по коду меню
CheckStyle __fastcall checkUserRightForMenuItem(TComponent *owner,FBDatabase *fbDB,AnsiString iserID,AnsiString miID);
// - проверка права ползователя на пункт меню по имени меню
CheckStyle __fastcall checkUserRightForMenuName(TComponent *owner,FBDatabase *fbDB,AnsiString iserID,AnsiString miName);
#endif

