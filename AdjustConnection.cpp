//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "SecretWord.h"
#include "AdjustConnection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "SmrtPanel"
#pragma link "SmrtPanel"
#pragma link "SmrtPanel"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "SmrtPanel"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//TAdjustConnectionForm *AdjustConnectionForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAdjustConnectionForm::TAdjustConnectionForm(TComponent* Owner,AnsiString iniFN,AnsiString wd,AnsiString initStr)
        : TForm(Owner),iniFileName(iniFN),workDir(wd),initString(initStr)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AdjustConnectionForm";

  if(FileExists(workDir+"\\Edit1.ini")) serverNameBox->Items->LoadFromFile(workDir+"Edit1.ini");
  if(FileExists(workDir+"\\Edit2.ini")) dbFullNameBox->Items->LoadFromFile(workDir+"Edit2.ini");
  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
    return;
  }
  analysesInitString();
}
//---------------------------------------------------------------------------
__fastcall TAdjustConnectionForm::~TAdjustConnectionForm(void)
{
  if(sw!=NULL) delete sw;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TAdjustConnectionForm::analysesInitString(void)
{
  if(!initString.IsEmpty())
  { int len;
    //Псевдоним базы
    if(initString.Pos("=") > 0)
      aliasEdit->Text=initString.SubString(0,initString.Pos("=")-1);
    else aliasEdit->Text="";
    //Сервер
    if(initString.Pos(":") > 0)
      serverNameBox->Text=initString.SubString(initString.Pos("=")+1,initString.Pos(":")-initString.Pos("=")-1);
    else serverNameBox->Text="localhost";
    if(serverNameBox->Text.UpperCase()=="LOCALHOST") localRadioButton->Checked=true;
    else remoteRadioButton->Checked=true;
    //Полное имя файла с базой
    if(initString.Pos(" -user ") > 0)
    { if(initString.Pos(":") > 0) len=initString.Pos(" -user ")-initString.Pos(":")-1;
      else len=initString.Pos(" -user ")-1;
    }
    else len=initString.Length()-initString.Pos(":");
    dbFullNameBox->Text=initString.SubString(initString.Pos(":")+1,len);
    //Имя пользователя
    userNameEdit->Text="";
    if(initString.Pos(" -user ") > 0)
    {
      int p1=initString.Pos(" -pas");
      int p2=initString.Pos(" -user");
      len=p1-p2-7;
      userNameEdit->Text=initString.SubString(p2+7,len);
    }
    //Пароль
    userPasswordEdit->Text="";
    if(initString.Pos(" -pas") > 0)
    { len=initString.Length()-initString.Pos(" -pas")-5;
      userPas=sw->decodeWord(initString.SubString(initString.Pos(" -pas")+6,len));
      userPasswordEdit->Text=userPas;
    }
  }
  else
  { aliasEdit->Text="";
    serverNameBox->Text="localhost";
    localRadioButton->Checked=true;
    userNameEdit->Text="";
    userPasswordEdit->Text="";
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAdjustConnectionForm::FormShow(TObject *Sender)
{
  aliasEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAdjustConnectionForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  serverNameBox->Items->SaveToFile(workDir+"Edit1.ini");
  dbFullNameBox->Items->SaveToFile(workDir+"Edit2.ini");
}
//---------------------------------------------------------------------------
void __fastcall TAdjustConnectionForm::selectDBButtonClick(TObject *Sender)
{
  OpenDialog->InitialDir=ExtractFileDir(dbFullNameBox->Text);
  if(OpenDialog->Execute()==true) dbFullNameBox->Text=OpenDialog->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TAdjustConnectionForm::CancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TAdjustConnectionForm::SaveButtonClick(TObject *Sender)
{
  if(aliasEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Описание базы данных\" должно быть заполнено",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(serverNameBox->Text.IsEmpty())
  { MessageDlg("Поле \"cетевое имя сервера или IP\" должно быть заполнено",mtConfirmation,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(dbFullNameBox->Text.IsEmpty())
  { MessageDlg("Поле \"Полное имя файла с базой данных на сервере\" должно быть заполнено",mtConfirmation,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(userNameEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Логин\" должно быть заполнено",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(userPasswordEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Пароль\" должно быть заполнено",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(serverNameBox->Items->Text.Pos(serverNameBox->Text)==0) serverNameBox->Items->Add(serverNameBox->Text);
  if(dbFullNameBox->Items->Text.Pos(dbFullNameBox->Text)==0) dbFullNameBox->Items->Add(dbFullNameBox->Text);
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TAdjustConnectionForm::userPasswordEditKeyPress(
      TObject *Sender, char &Key)
{
  if(Key==VK_RETURN){ SaveButton->OnClick(this); Key=0;}
}
//---------------------------------------------------------------------------

