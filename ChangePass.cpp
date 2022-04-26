//---------------------------------------------------------------------------

#include <vcl.h>
#include "DBClasses.h"
#include "SecretWord.h"
#pragma hdrstop
#include "ChangePass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FIBDatabase"
#pragma link "pFIBDatabase"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//TChangePassForm *ChangePassForm;
//---------------------------------------------------------------------------
__fastcall TChangePassForm::TChangePassForm(TComponent* Owner, AnsiString id, TpFIBDatabase *db)
        : TForm(Owner)
{
  ID=id;
  database=db;

  if(ID.IsEmpty())
  { MessageDlg("Ошибка ID="+ID,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }

  try
  {
    fbQ=new FBQuery(Owner,db,false);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fbQ=NULL;
    return;
  }
  Q=fbQ->getQuery();
  AnsiString sql="select U_NAME,U_PASSWORD from USERS_TABLE where U_ID="+ID;
  if(fbQ->execQuery("TChangePassForm::TChangePassForm",sql)==false) return;
  OldPassWD=sw->decodeWord(Q->FieldByName("U_PASSWORD")->AsString);
  AnsiString str="Пользователь: "+Q->FieldByName("U_NAME")->AsString;
  Name->Caption=str;
  Q->Close();
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::FormDestroy(TObject *Sender)
{
  if(sw!=NULL) delete sw;
  if(fbQ!=NULL) delete fbQ;
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::FormShow(TObject *Sender)
{
  if(OldPassWD.IsEmpty()) NewPassword1->SetFocus();
  else OldPassword->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::OldPasswordKeyPress(TObject *Sender,
      char &Key)
{
  if(Key==VK_RETURN){ NewPassword1->SetFocus(); Key=0;}
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::NewPassword1KeyPress(TObject *Sender,
      char &Key)
{
  if(Key==VK_RETURN){ NewPassword2->SetFocus(); Key=0;}
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::SaveButtonClick(TObject *Sender)
{
  if(OldPassWD!=OldPassword->Text)
  { MessageDlg("Направильный старый пароль",mtError,TMsgDlgButtons() << mbOK,0);
    OldPassword->Text="";
    OldPassword->SetFocus();
    return;
  }
  else if(NewPassword1->Text!=NewPassword2->Text)
  { MessageDlg("\"Новый пароль\" не совпадает с \"Повторите пароль\"",mtError,TMsgDlgButtons() << mbOK,0);
    NewPassword1->Text=""; NewPassword2->Text="";
    NewPassword1->SetFocus();
    return;
  }
  else if(NewPassword1->Text.IsEmpty())
  { MessageDlg("Пароль не может быть пустым",mtError,TMsgDlgButtons() << mbOK,0);
    NewPassword1->SetFocus();
    return;
  }
  else if(OldPassWD==NewPassword1->Text) return;
  else
  { AnsiString new_pw=sw->encodeWord(NewPassword1->Text);
    AnsiString sql="update USERS_TABLE set U_PASSWORD='"+new_pw+"' where U_ID="+ID;
    if(fbQ->execQuery("TChangePassForm::SaveButtonClick",sql)==false) return;
  }
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TChangePassForm::CancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------

