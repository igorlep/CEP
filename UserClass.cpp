//---------------------------------------------------------------------------
//Файл - UserClass.cpp
//********************************************************
// Классы для работы с объектом - Пользователь программы *
//********************************************************

#include "SecretWord.h"
#pragma hdrstop
#include "UserClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//*******************************
// Класс пользователь программы *
//*******************************
__fastcall PCUser::PCUser(FBDatabase *dataBase)
{
  db=dataBase;
  uID=""; uName=""; uLogin=""; uPassword=""; uRoleID=""; uRole=""; netName="";
  netComputerName=""; uMail="";

  fbRQ=newFBQuery((TComponent*)NULL,"PCUser::PCUser",db->getDatabase(),true);
  if(fbRQ==NULL) return;

  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }
}
//---------------------------------------------------------------------------
__fastcall PCUser::~PCUser(void)
{
  if(sw!=NULL){ delete sw; sw=NULL;}
  if(fbRQ!=NULL) delete fbRQ;
}
//---------------------------------------------------------------------------
void __fastcall PCUser::getDataFromUsersTable(AnsiString id)
{
TpFIBQuery *q=fbRQ->getQuery();

  AnsiString sql="select u.*, r.ROLE_NAME";
  sql+=" from USERS_TABLE u";
  sql+=" left outer join ROLES_TABLE r on r.ID=u.U_ROLE_ID";
  sql+=" where U_ID="+id;
  if(fbRQ->execQuery("PCUser::getDataFromUsersTable",sql)==false) return;
  if(q->RecordCount > 0)
  { uID=id;
    uName=q->FieldByName("U_NAME")->AsString;
    uLogin=q->FieldByName("U_LOGIN")->AsString;
    uPassword=decodePasword(q->FieldByName("U_PASSWORD")->AsString);
    uRoleID=q->FieldByName("U_ROLE_ID")->AsString;
    uRole=q->FieldByName("ROLE_NAME")->AsString;
    uMail=q->FieldByName("U_MAIL")->AsString;
    char szBuf[128];
    szBuf[0] = '\0';
    unsigned long dwSize=128;
    netName="";
    if(GetUserName(szBuf,&dwSize)) netName=AnsiString(szBuf);
    szBuf[0] = '\0';
    netComputerName="";
    if(GetComputerName(szBuf, &dwSize)) netComputerName=AnsiString(szBuf);
  }
  else
  { MessageDlg("Пользователь с кодом "+id+" не найден!",mtWarning,TMsgDlgButtons() << mbOK,0);
    uID="";uName="";uLogin="";uPassword="";uRoleID="";uRole="";netName=""; uMail="";netComputerName="";
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall PCUser::encodePasword(AnsiString password)
{
  return sw->encodeWord(password);
}
//---------------------------------------------------------------------------
AnsiString __fastcall PCUser::decodePasword(AnsiString codeString)
{
  if(codeString.Length()>50) return sw->decodeWord(codeString);
  else return codeString;
}
//---------------------------------------------------------------------------


