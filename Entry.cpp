//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Registry.hpp>
#include "SecretWord.h"
#include "DBClasses.h"
#include "Entry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SmrtPanel"
#pragma link "DBGridEh"
#pragma link "FIBDatabase"
#pragma link "FIBDataSet"
#pragma link "pFIBDatabase"
#pragma link "pFIBDataSet"
#pragma link "RxRichEd"
#pragma link "FIBQuery"
#pragma link "pFIBQuery"
#pragma link "JvAnimTitle"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "JvDotNetControls"
#pragma link "JvEdit"
#pragma link "JvExStdCtrls"
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "JvXPCheckCtrls"
#pragma resource "*.dfm"
TEntryForm *EntryForm;
//---------------------------------------------------------------------------
__fastcall TEntryForm::TEntryForm(TComponent* Owner,AnsiString iniFN,AnsiString initDBString,AnsiString pName)
        : TForm(Owner),iniFileName(iniFN)
{
  programNameLabel->Caption=pName;
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="EntryForm";
  //Считываем данные из ini файла
  TIniFile *ini = new TIniFile(iniFileName);
  userIndex=ini->ReadInteger("EntryForm","UserIndex",0);
  delete ini;

  try
  {
    fbDB=new FBDatabase(Owner);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта FBDatabase\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fbDB=NULL;
    return;
  }
  fbDB->init(initDBString);
  if(fbDB->connect()==false) return;

  try
  {
    sw=new SecretWord();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }

  //Формируем список пользователей
  FBDataSet *fbUDS=newFBDataSet(Owner,"TEntryForm::TEntryForm",fbDB->getDatabase(),true);
  if(fbUDS==NULL) return;
  TpFIBDataSet *UsersDataSet=fbUDS->getDataSet();
  RQ=newFBQuery(Owner,"TEntryForm::TEntryForm",fbDB->getDatabase(),true);
  if(RQ==NULL) return;
  WQ=newFBQuery(Owner,"TEntryForm::TEntryForm",fbDB->getDatabase(),false);
  if(WQ==NULL) return;

  AnsiString sql="select U_ID from USERS_TABLE order by U_LOGIN";
  if(fbUDS->execSelectSQL("TEntryForm::TEntryForm",sql)==false) return;

  //Если список пользователей пуст, создаем пользователя "Администратор" с паролем "password"
  if(UsersDataSet->RecordCount == 0)
  { AnsiString u_role_id="";
    AnsiString sql;
    //Проверяем есть ли в таблице ROLES_TABLE роль с именем "администратор"
    sql="select ID from ROLES_TABLE where ROLE_NAME='администратор'";
    if(RQ->execQuery("TEntryForm::TEntryForm",sql)==false) return;
    if(RQ->getQuery()->RecordCount > 0)
      u_role_id=RQ->getQuery()->FieldByName("ID")->AsString;
    RQ->getQuery()->Close();

    //Создаем пользователя Администратор с паролем password
    AnsiString passwd=sw->encodeWord("password");
    sql="insert into USERS_TABLE(U_ID,U_NAME,U_LOGIN,U_PASSWORD,U_ROLE_ID,DEL_FLG) values(";
    sql+="GEN_ID(GEN_USERS_TABLE_ID,1),'Администратор','Администратор'";
    sql+=",'"+passwd+"'";
    if(u_role_id.IsEmpty()) sql+=",NULL,0)";
    else sql+=","+u_role_id+",0)";
    if(WQ->execQuery("TEntryForm::TEntryForm",sql)==false)
    { WQ->getTransaction()->Rollback();
      return;
    }
    UsersDataSet->Close();
    UsersDataSet->Open();
  }

  UsersDataSet->Last();
  int uCount=UsersDataSet->RecordCount;
  try
  {
    PCU=new PCUser*[uCount];
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта PCUser*["+IntToStr(uCount)+"]\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    PCU=NULL;
    return;
  }
  for(int i=0; i<uCount; i++) PCU[i]=NULL;

  UsersDataSet->First();
  int i=0;
  while(!UsersDataSet->Eof)
  { try
    {
      PCU[i]=new PCUser(fbDB);
    }
    catch(const Exception &exc)
    { MessageDlg("Ошибка создания объекта PCU["+IntToStr(i)+"]\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      PCU[i]=NULL;
      return;
    }
    PCU[i]->getDataFromUsersTable(UsersDataSet->FieldByName("U_ID")->AsString);
    UsersBox->Items->Add(PCU[i]->getULogin());
    i++;
    UsersDataSet->Next();
  }
  UsersDataSet->Close();

  if(UsersBox->Items->Count > userIndex) UsersBox->ItemIndex=userIndex;
  else UsersBox->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::FormShow(TObject *Sender)
{
  PasswordEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Action=caFree;
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::FormDestroy(TObject *Sender)
{
  if(sw!=NULL) delete sw;
  if(PCU!=NULL)
  { for(int i=0; i<UsersBox->Items->Count; i++) if(PCU[i]!=NULL) delete PCU[i];
    delete []PCU;
  }
  if(WQ!=NULL) delete WQ;
  if(RQ!=NULL) delete RQ;
  if(fbUDS!=NULL) delete fbUDS;
  if(fbDB!=NULL) delete fbDB;
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::UsersBoxChange(TObject *Sender)
{
//  if(PCU[UsersBox->ItemIndex]->getUPassword().IsEmpty())
//  { MessageDlg("У выбранного пользователя отсутствует пароль.\nВход в программу без пароля невозможен!!!\nОбратитесь к администратору программы.",mtWarning,TMsgDlgButtons() << mbOK,0);
//    return;
//  }
  PasswordEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::PasswordEditKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==VK_RETURN){ EntryButton->OnClick(this); Key=0;}
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::EntryButtonClick(TObject *Sender)
{
  if(PasswordEdit->Text.Length()>50) PasswordEdit->Text=PasswordEdit->Text.SubString(0,50);
  AnsiString pw=PCU[UsersBox->ItemIndex]->getUPassword();
  if(pw.Length()>50) pw=sw->decodeWord(pw);
  if(PasswordEdit->Text==pw)
  { TIniFile *ini = new TIniFile(iniFileName);
    ini->WriteInteger("EntryForm","UserIndex",UsersBox->ItemIndex);
    ini->WriteBool("EntryForm","loadingFLG",loadingFLG->Checked);
    Close();
    ModalResult=mrOk;
  }
  else
  { MessageDlg("Ошибка входа в программу - \"Неправильный пароль\"",mtError,TMsgDlgButtons() << mbOK,0);
    PasswordEdit->Text="";
    PasswordEdit->SetFocus();
  }
}
//---------------------------------------------------------------------------
PCUser * __fastcall TEntryForm::getSelectedUser(void)
{
  if(UsersBox->Items->Count > 0) return PCU[UsersBox->ItemIndex];
  else return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TEntryForm::loadingFLGClick(TObject *Sender)
{
  PasswordEdit->SetFocus();
}
//---------------------------------------------------------------------------

