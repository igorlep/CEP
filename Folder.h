//---------------------------------------------------------------------------
//Класс "Папка" - для работы с таблицей FOLDERS_TABLE

#ifndef FolderH
#define FolderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
class Folder
{
private:
  FBQuery *fbRQ;
  TpFIBQuery *RQ;
  FBQuery *fbWQ;
  TpFIBTransaction *WT;
  FBDataSet *fbDS;
  TpFIBDataSet *DS;
  AnsiString tableName;
public:
  __fastcall Folder(TComponent* owner,TpFIBDatabase *db,AnsiString tName);
  __fastcall ~Folder(void);
  //функция возвращает идентификатор корневой папки
  AnsiString __fastcall getRootFolderID(void);
  //функция, возвращающая значение поля FOLDER_NUMBER
  AnsiString __fastcall getFolderNumber(AnsiString folderID);
  //Функция проверяет наличие записи по коду родителя и наименованию папки
  //Если запись отсутствует, то, если параметр createRecord == true, создает запись
  //Возвращает код записи
  AnsiString __fastcall checkRecordInFoldersTable(AnsiString parentID, AnsiString folderName,bool createRecord);
  //функция переименования папки
  bool __fastcall renameFolder(AnsiString folderID,AnsiString newFolderName);
  //функция удаления записи из таблицы tableName
  bool __fastcall removeFolder(AnsiString folderID);
  //Функция возвращает полное имя папки, начиная с коренвой
  AnsiString getFolderFullName(AnsiString folderID);
  //Функция возвращает код папки по ее полному имени
  AnsiString getFolderID(AnsiString fullFolderName);
};
//---------------------------------------------------------------------------
//Функция создания экземпляра Folder
Folder *newFolder(TComponent* owner,TpFIBDatabase *db,AnsiString tName);
//---------------------------------------------------------------------------

#endif
