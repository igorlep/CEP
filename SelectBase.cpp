//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "SecretWord.h"
#include "AdjustConnection.h"
#include "SelectBase.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SmrtPanel"
#pragma link "Placemnt"
#pragma link "FIBDatabase"
#pragma link "pFIBDatabase"
#pragma link "FIBDatabase"
#pragma link "pFIBDatabase"
#pragma link "JvAnimTitle"
#pragma link "JvComponent"
#pragma link "FIBQuery"
#pragma link "pFIBQuery"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TSelectBaseForm::TSelectBaseForm(TComponent* Owner,AnsiString iniFN,AnsiString wd,AnsiString pName)
        : TForm(Owner),iniFileName(iniFN),workDir(wd)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="SelectBaseForm";
  programNameLabel->Caption=pName;
  DBNames=new TStringList;
  TIniFile *ini = new TIniFile(iniFileName);
  TopPanel->Height=ini->ReadInteger("SelectBaseForm","TopPanel_Height",38);
  DBIndex=ini->ReadInteger("SelectBaseForm","DBIndex",0);
  delete ini;
  EditFLG=false;
  ACF=NULL;
  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    sw=NULL;
  }
  LoadDBNamesList();
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::FormShow(TObject *Sender)
{
  if(DBNamesBox->Items->Count > 0)
  { if(DBNamesBox->Items->Count > DBIndex) DBNamesBox->ItemIndex=DBIndex;
    else DBNamesBox->ItemIndex=0;
    ConnectButton->SetFocus();
  }
  else AddButton->SetFocus();
  setStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::FormDestroy(TObject *Sender)
{
  TIniFile *ini = new TIniFile(iniFileName);
  ini->WriteInteger("SelectBaseForm","TopPanel_Height",TopPanel->Height);
  ini->WriteInteger("SelectBaseForm","DBIndex",DBIndex);
  if(EditFLG)
  { if(ini->SectionExists("DBAliases")) ini->EraseSection("DBAliases");
    if(DBNames->Count > 0)
    { for(int i=0; i<DBNames->Count; ++i)
      { AnsiString str=DBNames->Strings[i];
        AnsiString DBAlias=str.SubString(1,str.Pos("=")-1);
        AnsiString DBName=str.SubString(str.Pos("=")+1,str.Length());
        ini->WriteString("DBAliases",DBAlias,DBName);
      }
    }
  }
  delete ini;
  delete DBNames;
  if(sw!=NULL) delete sw;
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::LoadDBNamesList(void)
{
  DBNamesBox->Clear();
  DBNames->Clear();
  TIniFile *ini = new TIniFile(iniFileName);
  ini->ReadSection("DBAliases",DBNamesBox->Items); //считываем псевдонимы баз данных (все что слева от знака '=')
  ini->ReadSectionValues("DBAliases",DBNames);     //считываем значения
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::CancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::setStatusBar(void)
{
AnsiString DBName;

  StatusBar->SimpleText="";
  DBIndex=-1;
  if(DBNames->Count > 0)
  { DBName=DBNames->Strings[DBNamesBox->ItemIndex];
    AnsiString s=DBName.SubString(DBName.Pos("=")+1,DBName.Length());
    if(s.Pos(" -user ")>0) s=s.SubString(0,s.Pos(" -user "));
    StatusBar->SimpleText=" "+s;
    DBIndex=DBNamesBox->ItemIndex;
  }
  if(DBNamesBox->Items->Count > 0) ConnectButton->Enabled=true;
  else ConnectButton->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::DBNamesBoxClick(TObject *Sender)
{
  setStatusBar();
}
//---------------------------------------------------------------------------
bool __fastcall TSelectBaseForm::SaveDBNameToList(bool AddFLG)
{
  if(ACF==NULL) return false;
  AnsiString DBName=ACF->aliasEdit->Text+"=";
  DBName+=ACF->serverNameBox->Text+":";
  DBName+=ACF->dbFullNameBox->Text;
  DBName+=" -user "+ACF->userNameEdit->Text;
  DBName+=" -pas "+sw->encodeWord(ACF->userPasswordEdit->Text);
  int i;
  for(i=0; i<DBNames->Count; ++i)
  { AnsiString s1=DBName;
    if(s1.Pos(" -user ")>0) s1=s1.SubString(0,s1.Pos(" -user "));
    AnsiString s2=DBNames->Strings[i];
    if(s2.Pos(" -user ")>0) s2=s2.SubString(0,s2.Pos(" -user "));
    if(s1==s2) break;
  }
  if(i<DBNames->Count)
  { DBNames->Delete(i);
    DBNamesBox->Items->Delete(i);
  }
  else if(!AddFLG)
  { DBNames->Delete(DBNamesBox->ItemIndex);
    DBNamesBox->Items->Delete(DBNamesBox->ItemIndex);
  }
  DBNamesBox->Items->Add(ACF->aliasEdit->Text);
  DBNames->Add(DBName);
  EditFLG=true;
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::EditButtonClick(TObject *Sender)
{
  try
  {
    ACF=new TAdjustConnectionForm(this,iniFileName,workDir,DBNames->Strings[DBNamesBox->ItemIndex]);
  }
  catch(Exception &exception)
  { MessageDlg("Ошибка создания объекта TAdjustConnectionForm\n"+exception.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  AnsiString alias="";
  do
  { if(ACF->ShowModal()==mrOk)
    { if(SaveDBNameToList(false)==true)
      { alias=ACF->aliasEdit->Text;
        break;
      }
    }
    else break;
  }while(1);
  if(ACF!=NULL){ delete ACF; ACF=NULL;}
  if(DBNamesBox->Count > 0)
  { if(!alias.IsEmpty())
    { int i;
      for(i=0; i < DBNamesBox->Count; i++)
        if(DBNamesBox->Items->Strings[i]==alias) break;
      if(i<DBNamesBox->Count) DBNamesBox->ItemIndex=i;
      else DBNamesBox->ItemIndex=0;
    }
  }
  setStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::AddButtonClick(TObject *Sender)
{
  try
  {
    ACF=new TAdjustConnectionForm(this,iniFileName,workDir,"");
  }
  catch(Exception &exception)
  { MessageDlg("Ошибка создания объекта TAdjustConnectionForm\n"+exception.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  AnsiString alias="";
  do
  { if(ACF->ShowModal()==mrOk)
    { if(SaveDBNameToList(true)==true)
      { alias=ACF->aliasEdit->Text;
        break;
      }
    }
    else break;
  }while(1);
  if(ACF!=NULL){ delete ACF; ACF=NULL;}
  if(DBNames->Count > 0)
  { if(!alias.IsEmpty())
    { int i;
      for(i=0; i<DBNamesBox->Count; i++)
        if(DBNamesBox->Items->Strings[i]==alias) break;
      if(i<DBNamesBox->Count) DBNamesBox->ItemIndex=i;
      else DBNamesBox->ItemIndex=0;
    }
  }
  setStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::DelButtonClick(TObject *Sender)
{
  if(MessageDlg("Вы действительно хотите удалить текущий псевдоним базы данных?",mtConfirmation,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
    return;
  int i=DBNamesBox->ItemIndex;
  DBNamesBox->Items->Delete(i);
  DBNames->Delete(i);
  EditFLG=true;
  if(DBNamesBox->Items->Count > 0)
  { DBNamesBox->ItemIndex=0;
    DBNamesBox->SetFocus();
  }
  else AddButton->SetFocus();
  setStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TSelectBaseForm::ConnectButtonClick(TObject *Sender)
{
FBDatabase *dataBase;

  try
  {
    dataBase=new FBDatabase(static_cast<void*>(NULL));
  }
  catch(const Exception &exc)
  {
    MessageDlg("Ошибка создания объекта FBDatabase\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  dataBase->init(DBNames->Strings[DBNamesBox->ItemIndex]);
  //Пробуем подключиться к выбранной базе
  if(dataBase->connect()==false)
  { delete dataBase;
    Close();
    ModalResult=mrCancel;
  }
  else
  { delete dataBase;
    Close();
    ModalResult=mrOk;
  }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TSelectBaseForm::getInitString(void)
{
  if(DBNamesBox->Count==0) return "";
  return DBNames->Strings[DBNamesBox->ItemIndex];
}
//---------------------------------------------------------------------------

