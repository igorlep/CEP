//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "SomeFunctions.h"
#include "SelectRicipient.h"
#include "AddEditMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "JvArrowButton"
#pragma link "JvBevel"
#pragma link "JvDialogs"
#pragma link "JvDotNetControls"
#pragma link "JvExComCtrls"
#pragma link "JvExExtCtrls"
#pragma link "JvExStdCtrls"
#pragma link "JvListView"
#pragma link "JvMemo"
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "JvXPCheckCtrls"
#pragma resource "*.dfm"
//TAddEditMessageForm *AddEditMessageForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TAddEditMessageForm::TAddEditMessageForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb,Message *m)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb),mes(m)
{
TIniFile* ini;

  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="AddEditMessageForm";

  ini=new TIniFile(iniFileName);
  initialDir=ini->ReadString("AddEditMessageForm","InitialDir",getAppDir()+"Out\\");
  delete ini;
  if(DirectoryExists(initialDir)==false) CreateDir(initialDir);

  fileNames=new TStringList;
//  restoreFileNames=new TStringList;

  if(mes->getM_ID().IsEmpty()) Caption="Новое сообщение";
  else Caption="Редактирование сообщения"; Caption="Редактирование сообщения";
  rNameEdit->Text=mes->getRecipientName();
  rMailEdit->Text=mes->getRecipientAddress();
  subjectEdit->Text=mes->getSubject();
  contentMemo->Lines->Text=mes->getContents();
  replyCheckbox->Checked=mes->getReplyFLG();
  delFilesCheckbox->Checked=mes->getDelAttachedFilesFLG();
  fileNames->AddStrings(mes->getAttachedFilesList());
  mes->ExtractAllFiles();
  
  //Заполняем afLV и список fileNames
  afLV->Clear();
  for(int i=0; i<fileNames->Count; i++)
  { AnsiString fn=ExtractFileName(fileNames->Strings[i]);
    TListItem *li=afLV->Items->Add();
    li->Caption=fn;
    if(ExtractFileExt(fn).Pos("zip") || ExtractFileExt(fn).Pos("rar") || ExtractFileExt(fn).Pos("arj"))
      li->ImageIndex=0;
    else if(ExtractFileExt(fn).Pos("xls"))
      li->ImageIndex=1;
    else if(ExtractFileExt(fn).Pos("doc"))
      li->ImageIndex=2;
    else if(ExtractFileExt(fn).Pos("pdf"))
      li->ImageIndex=3;
    else li->ImageIndex=4;
  }
}
//---------------------------------------------------------------------------
__fastcall TAddEditMessageForm::~TAddEditMessageForm(void)
{
TIniFile* ini;

  ini=new TIniFile(iniFileName);
  ini->WriteString("AddEditMessageFrame","InitialDir",initialDir);
  delete ini;

  delete fileNames;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TAddEditMessageForm::choiceRicipient(int choice_place)
{
TSelectRicipientForm *SRF;

  try
  {
    SRF=new TSelectRicipientForm(this,iniFileName,fbDB,choice_place);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания формы SelectRicipientForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(SRF->ShowModal()==mrOk)
  { rNameEdit->Text=SRF->getRicipientName();
    rMailEdit->Text=SRF->getRicipientEMail();
  }
  delete SRF;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TAddEditMessageForm::cancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
//Выбор подразделения
void __fastcall TAddEditMessageForm::subdivisionsItemClick(TObject *Sender)
{
  choiceRicipient(1);
}
//---------------------------------------------------------------------------
//Выбор поставщика
void __fastcall TAddEditMessageForm::publishersItemClick(TObject *Sender)
{
  choiceRicipient(2);
}
//---------------------------------------------------------------------------
//Добавить вложение
void __fastcall TAddEditMessageForm::addAttachedFileItemClick(
      TObject *Sender)
{
  fileOpenDialog->InitialDir=initialDir;
  if(fileOpenDialog->Execute()==true)
  { initialDir=ExtractFileDir(fileOpenDialog->FileName);
    for(int i=0; i<fileOpenDialog->Files->Count; ++i)
    { AnsiString FName=fileOpenDialog->Files->Strings[i];
      if(FileExists(FName))
      { AnsiString fn=ExtractFileName(FName);
        fileNames->Add(FName);
        TListItem *li=afLV->Items->Add();
        li->Caption=fn;
        if(ExtractFileExt(fn).Pos("zip") || ExtractFileExt(fn).Pos("rar") || ExtractFileExt(fn).Pos("arj"))
          li->ImageIndex=0;
        else if(ExtractFileExt(fn).Pos("xls"))
          li->ImageIndex=1;
        else if(ExtractFileExt(fn).Pos("doc") || ExtractFileExt(fn).Pos("rtf"))
          li->ImageIndex=2;
        else if(ExtractFileExt(fn).Pos("pdf"))
          li->ImageIndex=3;
        else li->ImageIndex=4;
      }
    }
  }
}
//---------------------------------------------------------------------------
//Удаление элемента из списка вложений
void __fastcall TAddEditMessageForm::delAttachedFileItemClick(TObject *Sender)
{
  if(afLV->Items->Count==0) return;
  while(afLV->SelCount > 0)
  { AnsiString fn=fileNames->Strings[afLV->ItemIndex];
    if(FileExists(fn)) DeleteFile(fn);
    fileNames->Delete(afLV->ItemIndex);
    afLV->Items->Delete(afLV->ItemIndex);
  }
}
//---------------------------------------------------------------------------
//Очистить список вложений
void __fastcall TAddEditMessageForm::clearAttachedListItemClick(TObject *Sender)
{
  for(int i=0; i<fileNames->Count; i++)
  { AnsiString fn=fileNames->Strings[i];
    if(FileExists(fn)) DeleteFile(fn);
  }
  afLV->Clear();
  fileNames->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TAddEditMessageForm::afMenuPopup(TObject *Sender)
{
  if(afLV->Items->Count==0)
  { delAttachedFileItem->Enabled=false;
    clearAttachedListItem->Enabled=false;
  }
  else
  { delAttachedFileItem->Enabled=true;
    clearAttachedListItem->Enabled=true;
  }
}
//---------------------------------------------------------------------------
//Сохранение сообщения
void __fastcall TAddEditMessageForm::saveButtonClick(TObject *Sender)
{
  if(rNameEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Наименование получателя\" должно быть заполнено",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(rMailEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Адрес EMail\" должно быть заполнено",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(subjectEdit->Text.IsEmpty())
  { MessageDlg("Поле \"Тема\" должно быть заполнено",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(contentMemo->Text.IsEmpty())
  { MessageDlg("Поле \"Содержание\" должно быть заполнено",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  if(contentMemo->Text.Length() > 240)
  { MessageDlg("Длина поля \"Содержание\" не должна превышать 240 символов",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  mes->setRecipientName(rNameEdit->Text);
  mes->setRecipientAddress(rMailEdit->Text);
  mes->setSubject(subjectEdit->Text);
  mes->setContents(contentMemo->Text);
  mes->setReplyFLG(replyCheckbox->Checked);
  mes->setDelAttachedFilesFLG(delFilesCheckbox->Checked);
  mes->setAttachedFilesList(fileNames);
  if(mes->WriteToBase()==false) return;

  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------

