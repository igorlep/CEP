//---------------------------------------------------------------------------

#pragma hdrstop
#include "SomeFunctions.h"
#include "RBF.h"
#include "ReferenceBook.h"
#include "DBTreeViewF.h"
#include "UsersWorkFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//***************************************************
// - Фрейм Для работы с пользователями и их правами *
//***************************************************
// -- фрейм пользователей
bool __fastcall CanDelUser(TComponent* owner,FBDatabase *fbDB,AnsiString id,AnsiString &str)
{
FBQuery * fbRQ=fbDB->getFBQ_T1();
AnsiString sql;

  str=="";
  if(fbRQ==NULL) return false;
  sql="select r.ROLE_NAME from USERS_TABLE u";
  sql+=" left outer join ROLES_TABLE r on r.ID=u.U_ROLE_ID";
  sql+=" where u.U_ID="+id;
  if(fbRQ->execQuery("CanDelUser",sql)==false) return false;
  if(fbRQ->getQuery()->FieldByName("ROLE_NAME")->AsString == "администратор")
    str="Нельзя удалить пользователя \"Администратор\"";
  else str="Нельзя удалить запись с кодом \""+id+"\"";
  return false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formUsersSelectSQL(bool viewDelRecords)
{
AnsiString sql;

  sql="select * from USERS_TABLE";
  if(getUser()->getURole()!="администратор")
    sql+=" where U_ID="+getUser()->getUID();
  else if(viewDelRecords==false) sql+=" where (DEL_FLG is NULL or DEL_FLG=0)";
  return sql;
}
//---------------------------------------------------------------------------
TRBFrame * __fastcall createUsersFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
AnsiString tableInfo="USERS_TABLE,Пользователи,U_ID,U_LOGIN,Пользователи,";
AnsiString fieldsInfo;
int role_poz;

  if(getUser()->getURole()=="администратор")
  { fieldsInfo="DEL_FLG,DEL_FLG,,Center,1,1,1,0,;";
    fieldsInfo+="U_ID,U_ID,Код,Center,1,1,1,0,;";
    fieldsInfo+="U_NAME,U_NAME,Фамилия И.О.,Left,0,1,1,0,;";
    fieldsInfo+="U_LOGIN,U_LOGIN,Псевдоним,Left,0,1,1,0,;";
    fieldsInfo+="U_PASSWORD,U_PASSWORD,Пароль,Left,1,0,0,0,;";
    fieldsInfo+="U_ROLE_ID,U_ROLE_ID,Роль пользователя,Center,0,1,1,0,;";
    fieldsInfo+="U_MAIL,U_MAIL,Адрес электронной почты,Left,0,1,1,0,";
    role_poz=4;
  }
  else
  { fieldsInfo+="U_ID,U_ID,Код,Center,1,1,1,0,;";
    fieldsInfo+="U_NAME,U_NAME,Фамилия И.О.,Left,0,1,1,0,;";
    fieldsInfo+="U_LOGIN,U_LOGIN,Псевдоним,Left,1,1,1,0,;";
    fieldsInfo+="U_PASSWORD,U_PASSWORD,Пароль,Left,1,0,0,0,;";
    fieldsInfo+="U_ROLE_ID,U_ROLE_ID,Роль пользователя,Center,1,1,1,0,;";
    fieldsInfo+="U_MAIL,U_MAIL,Адрес электронной почты,Left,0,1,1,0,";
    role_poz=3;
  }
AnsiString contentsInfo="";
bool tvViewAble=false,
     needsSearchingForModels=false;

TRBFrame *urbf;

  try
  {
    urbf=new TRBFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,contentsInfo,tvViewAble,needsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("createUsersFrame - Ошибка создания TRBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  urbf->Parent=workPanel;
  urbf->gcHeader->Caption="Справочник Пользователей";
  urbf->getRB_GC()->canDelRecord=CanDelUser;
  urbf->formSelectSQL=formUsersSelectSQL;

  //Убираем лишние элементы меню
  urbf->rbMoveRecord->Visible=false;
  urbf->rbSelectRecord->Visible=false;
  //Добавляем нужные элементы меню
  urbf->rbSeparator3->Visible=true;
  urbf->rbInputPassword->Visible=true;
  urbf->rbViewPassword->Visible=true;

  if(getUser()->getURole()!="администратор")
  { urbf->rbAddRecord->Visible=false;
    urbf->rbDelRecord->Visible=false;
    urbf->rbUpdate->Visible=false;
    urbf->rbViewDelRecords->Visible=false;
    urbf->rbSeparator2->Visible=false;
    urbf->rbViewPassword->Visible=false;
  }
  //Настраиваем механизм выбора роли пользователя
  AnsiString sql;
  sql="select * from ROLES_TABLE";
  FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
  TpFIBDataSet *DS=fbDS->getDataSet();
  if(fbDS->execSelectSQL("createUsersFrame",sql)==true)
  { urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->KeyList->Clear();
    urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->PickList->Clear();
    DS->First();
    while(!DS->Eof)
    { urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->KeyList->Add(DS->FieldByName("ID")->AsString);
      urbf->getRB_GC()->getGridFrame()->workGrid->Columns->Items[role_poz]->PickList->Add(DS->FieldByName("ROLE_NAME")->AsString);
      DS->Next();
    }
    DS->Close();
  }
  return urbf;
}
//---------------------------------------------------------------------------
// -- фрейм с деревом прав пользователей
TDBTreeViewFrame * __fastcall createUsersTVFrame(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,TPanel *wPanel)
{
TDBTreeViewFrame *urTV;
AnsiString structTableInfo="MENU_ITEMS_TABLE,Права пользователей,MI_ID,MI_TITLE,2";

  try
  {
    urTV=new TDBTreeViewFrame(Owner,wPanel,iniFN,fbDB,structTableInfo);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TDBTreeViewFrame"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
//  urTV->DBTreeViewHeader->ColorFrom=clInactiveCaption;
//  urTV->DBTreeViewHeader->ColorTo=clWhite;
//  urTV->DBTreeViewHeader->Font->Color=clNavy;
  return urTV;
}
//---------------------------------------------------------------------------
