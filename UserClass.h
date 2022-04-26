//---------------------------------------------------------------------------
//Файл - UserClasses.h
//***************************************************************
// Набор классов для работы с объектом - Пользователь программы *
//***************************************************************
// Для работы с объектом "Пользователь программы" в базе данных должны быть следующие таблицы:
// USERS_TABLE  //таблица с пользователями программы
//    U_ID        SMALLINT NOT NULL,
//    U_NAME      VARCHAR(50),
//    U_LOGIN     VARCHAR(20),
//    U_PASSWORD  VARCHAR(242),
//    U_ROLE_ID   SMALLINT,
//    U_MAIL      VARCHAR(50),
//    REC_TYPE    SMALLINT
// ROLES_TABLE  //таблица с наименованими ролей
//    ID         SMALLINT NOT NULL,
//    ROLE_NAME  VARCHAR(25)
//
#ifndef UserClassH
#define UserClassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DBClasses.h"
//---------------------------------------------------------------------------
//Класс пользователь программы
class PCUser
{
private:
  FBDatabase *db;
  FBQuery *fbRQ;
  AnsiString uID;
  AnsiString uName;
  AnsiString uLogin;
  AnsiString uPassword;
  AnsiString uRoleID;
  AnsiString uRole;
  AnsiString netName;
  AnsiString netComputerName;
  AnsiString uMail;
  class SecretWord *sw;
public:
  __fastcall PCUser(FBDatabase *dataBase);
  __fastcall ~PCUser(void);
  void __fastcall getDataFromUsersTable(AnsiString id);
  AnsiString __fastcall encodePasword(AnsiString password);
  AnsiString __fastcall decodePasword(AnsiString codeString);
  AnsiString __fastcall getUID(void){ return uID; }
  AnsiString __fastcall getUName(void){ return uName; }
  AnsiString __fastcall getULogin(void){ return uLogin; }
  AnsiString __fastcall getUPassword(void){ return uPassword;}
  AnsiString __fastcall getURoleID(void){ return uRoleID; }
  AnsiString __fastcall getURole(void){ return uRole; }
  AnsiString __fastcall getUMail(void){ return uMail; }
  AnsiString __fastcall getNetName(void){ return netName; }
  AnsiString __fastcall getNetComputerName(void){ return netComputerName; }
};
//---------------------------------------------------------------------------
#endif
