//---------------------------------------------------------------------------
//Класс "Папка" - для работы с таблицей FOLDERS_TABLE *

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DifferentFunctions.h"
#include "GUID.h"
#include "Folder.h"
//---------------------------------------------------------------------------
__fastcall Folder::Folder(TComponent* owner,TpFIBDatabase *db,AnsiString tName)
{
  tableName=tName;
  fbRQ=NULL;
  fbWQ=NULL;
  fbDS=NULL;
  try
  {
    fbRQ=newFBQuery(owner,"Folder::Folder",db,true);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта fbRQ"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fbRQ=NULL;
    return;
  }
  RQ=fbRQ->getQuery();
  try
  {
    fbWQ=newFBQuery(owner,"Folder::Folder",db,false);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта fbWQ"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fbWQ=NULL;
    return;
  }
  WT=fbWQ->getTransaction();
  try
  {
    fbDS=newFBDataSet(owner,"Folder::Folder",db,true);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта fbDS"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fbDS=NULL;
    return;
  }
  DS=fbDS->getDataSet();
}
//---------------------------------------------------------------------------
__fastcall Folder::~Folder(void)
{
  if(fbRQ!=NULL){ delete fbRQ; fbRQ=NULL;}
  if(fbWQ!=NULL){ delete fbWQ; fbWQ=NULL;}
  if(fbDS!=NULL){ delete fbDS; fbDS=NULL;}
}
//---------------------------------------------------------------------------
//функция возвращает идентификатор корневой папки
AnsiString __fastcall Folder::getRootFolderID(void)
{
AnsiString sql;
AnsiString folderID=" ";

  if(fbRQ==NULL) return folderID;
  sql="select FOLDER_ID from "+tableName;
  sql+=" where PARENT_ID is NULL";
  if(fbRQ->execQuery("Folder::getRootFolderID",sql)==true && RQ->RecordCount > 0)
    folderID=RQ->FieldByName("FOLDER_ID")->AsString;
  return folderID;
}
//---------------------------------------------------------------------------
//функция, возвращающая значение поля FOLDER_NUMBER
AnsiString __fastcall Folder::getFolderNumber(AnsiString folderID)
{
AnsiString sql;
AnsiString folderNumber=" ";

  sql="select FOLDER_NUMBER from MESSAGES_FOLDERS_TABLE";
  sql+=" where FOLDER_ID='"+folderID+"'";
  if(fbRQ->execQuery("Folder::getFolderNumber",sql)==true && RQ->RecordCount > 0)
    folderNumber=RQ->FieldByName("FOLDER_NUMBER")->AsString;
  return folderNumber;
}
//---------------------------------------------------------------------------
//Функция проверяет наличие записи по коду родителя и наименованию папки
//Если запись отсутствует, то, если параметр createRecord == true, создает запись
//Возвращает код записи
AnsiString __fastcall Folder::checkRecordInFoldersTable(AnsiString parentID, AnsiString folderName,bool createRecord)
{
AnsiString sql;
AnsiString folderID="";
AnsiString folderNumber="";

  if(fbRQ==NULL || fbWQ==NULL || fbDS==NULL) goto ret;
  sql="select FOLDER_ID from "+tableName;
  sql+=" where FOLDER_NAME='"+folderName+"'";
  if(parentID!="0") sql+=" and PARENT_ID='"+parentID+"'";
  else sql+=" and PARENT_ID is NULL";
  if(fbRQ->execQuery("Folder::checkRecordInFoldersTable",sql)==false) goto ret;
  if(RQ->RecordCount > 0) //папка есть
    folderID=RQ->FieldByName("FOLDER_ID")->AsString;
  else if(createRecord==true) //создаем папку
  {//Определяем номер FOLDER_NUMBER, создаваемой папки
    if(parentID=="0") folderNumber="0";
    else
    { sql="select FOLDER_NUMBER from "+tableName;
      sql+=" where PARENT_ID='"+parentID+"'";
      if(fbDS->execSelectSQL("Folder::checkRecordInFoldersTable",sql)==false) goto ret;
      int number=0;
      AnsiString fn;
      int p;
      DS->First();
      while(!DS->Eof)
      { fn=DS->FieldByName("FOLDER_NUMBER")->AsString;
        p=fn.Length();
        while(p!=0 && fn.c_str()[p-1]!='.') p--;
        fn=fn.SubString(p+1,fn.Length()-p);
        int n=StrToInt(fn);
        if(n > number) number=n;
        DS->Next();
      }
      number++;
      fn=getFolderNumber(parentID);
      if(fn.IsEmpty()) goto ret;
      AnsiString n=AnsiString().sprintf("%02d",number);
      if(fn=="0") folderNumber=n;
      else folderNumber=fn+"."+n;
    }
    //Создаем запись
    AnsiString fID=guid().genGUID();
    sql="insert into "+tableName+"(FOLDER_ID,PARENT_ID,FOLDER_NAME,FOLDER_NUMBER) values(";
    sql+="'"+fID+"',";
    if(parentID!="0") sql+="'"+parentID+"',";
    else sql+="NULL,";
    sql+="'"+folderName+"','"+folderNumber+"')";
    WT->StartTransaction();
    if(fbWQ->execQuery("Folder::checkRecordInFoldersTable",sql)==false)
    { WT->Rollback();
      goto ret;
    }
    WT->Commit();
    folderID=fID;
  }
ret:
  return folderID;
}
//---------------------------------------------------------------------------
//функция переименования папки
bool __fastcall Folder::renameFolder(AnsiString folderID,AnsiString newFolderName)
{
AnsiString sql;
bool ret=false;

  if(fbWQ==NULL) return ret;
  sql="update "+tableName+" set";
  sql+=" FOLDER_NAME='"+newFolderName+"'";
  sql+=" where FOLDER_ID='"+folderID+"'";
  WT->StartTransaction();
  if(fbWQ->execQuery("Folder::renameFolder",sql)==false) WT->Rollback();
  else{ WT->Commit(); ret=true; }
  return ret;
}
//---------------------------------------------------------------------------
//функция удаления записи из таблицы tableName
bool __fastcall Folder::removeFolder(AnsiString folderID)
{
AnsiString sql;
bool flg=false;

  if(fbWQ==NULL) goto ret;
  sql="delete from "+tableName+" where FOLDER_ID='"+folderID+"'";
  WT->StartTransaction();
  if(fbWQ->execQuery("Folder::removeFolder",sql)==false)
  { WT->Rollback();
    goto ret;
  }
  WT->Commit();
  flg=true;
ret:
  return flg;
}
//---------------------------------------------------------------------------
//Функция возвращает полное имя папки, начиная с коренвой
AnsiString Folder::getFolderFullName(AnsiString folderID)
{
AnsiString name="";
AnsiString sql;

  if(fbRQ==NULL) return name;
  if(folderID==NULL || folderID.IsEmpty()) return name;
  do
  { sql="select FOLDER_NAME from "+tableName;
    sql+=" where FOLDER_ID='"+folderID+"'";
    if(fbRQ->execQuery("Folder::getFolderFullName",sql)==true && RQ->RecordCount > 0)
      name=RQ->FieldByName("FOLDER_NAME")->AsString+"\\"+name;
    else return "";
    sql="select PARENT_ID from "+tableName;
    sql+=" where FOLDER_ID='"+folderID+"'";
    if(fbRQ->execQuery("Folder::getFolderFullName",sql)==true && RQ->RecordCount > 0)
      folderID=RQ->FieldByName("PARENT_ID")->AsString;
    else return "";
  }while(folderID!=NULL && !folderID.IsEmpty());
  return name;
}
//---------------------------------------------------------------------------
  //Функция возвращает код папки по ее полному имени
AnsiString Folder::getFolderID(AnsiString fullFolderName)
{
AnsiString name="";
AnsiString sql;
AnsiString folderID;
int n=1;

  folderID=getRootFolderID();
  if(folderID==NULL) return NULL;
  while((name=getSubstring(fullFolderName,'\\',n))!=NULL)
  { folderID=checkRecordInFoldersTable(folderID,name,false);
    if(folderID.IsEmpty()) break;
    n++;
  }
  return folderID;
}
//---------------------------------------------------------------------------

//Функция создания экземпляра Folder
Folder *newFolder(TComponent* owner,TpFIBDatabase *db,AnsiString tName)
{
  try
  {
    return new Folder(owner,db,tName);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта Folder\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
}
//---------------------------------------------------------------------------

