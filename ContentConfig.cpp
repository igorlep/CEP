//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "ContentConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "kbmMemTable"
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "JvCheckListBox"
#pragma link "JvDotNetControls"
#pragma link "JvExCheckLst"
#pragma link "JvxCheckListBox"
#pragma resource "*.dfm"
//TContentConfigForm *ContentConfigForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TContentConfigForm::TContentConfigForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString tableTitle,AnsiString FI,AnsiString CI)
        : TForm(Owner),iniFileName(iniFN),iniSectionName(iniSN),fieldsInfo(FI),contentsInfo(CI)
{
TIniFile *ini;

  if(contentsInfo==NULL) contentsInfo="";
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="ContentConfigForm_"+iniSectionName;
  ini=new TIniFile(iniFileName);
  leftPanel->Width=ini->ReadInteger("ContentConfigForm_"+iniSectionName,"leftPanel_Width",180);
  delete ini;
  header->Caption=tableTitle;
  loadRightBox();
  loadLeftBox();
}
//---------------------------------------------------------------------------
__fastcall TContentConfigForm::~TContentConfigForm(void)
{
TIniFile *ini;

  ini=new TIniFile(iniFileName);
  ini->WriteInteger("ContentConfigForm_"+iniSectionName,"leftPanel_Width",leftPanel->Width);
  delete ini;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TContentConfigForm::getContentsInfo(void)
{
AnsiString newContentsInfo="";

  for(int i=0; i<rightBox->Items->Count; i++)
  { AnsiString str=rightBox->Items->Strings[i];
    AnsiString fieldName=str.SubString(1,str.Pos(" - ")-1);
    AnsiString fieldTitle=str.SubString(str.Pos(" - ")+3,str.Length()-str.Pos(" - ")-2);
    if(i>0) newContentsInfo+=";";
    newContentsInfo+=fieldName+","+fieldTitle;
  }
  return newContentsInfo;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TContentConfigForm::loadRightBox(void)
{
AnsiString subString;
int n=1;

  rightBox->Clear();
  if(contentsInfo==NULL || contentsInfo.IsEmpty()) return;
  while((subString=getSubstring(contentsInfo,';',n))!=NULL)
  { AnsiString fieldName=getSubstring(subString,',',1);
    AnsiString fieldTitle=getSubstring(subString,',',2);
    rightBox->Items->Add(fieldName+" - "+fieldTitle);
    n++;
  }
  rightBox->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TContentConfigForm::loadLeftBox(void)
{
AnsiString subString;
int n=1;

  leftBox->Clear();
  if(fieldsInfo==NULL || fieldsInfo.IsEmpty()) return;
  while((subString=getSubstring(fieldsInfo,';',n))!=NULL)
  { AnsiString fieldName=getSubstring(subString,',',2);
    if(fieldName!="DEL_FLG" && fieldName!="REC_TYPE" && contentsInfo.Pos(fieldName)==0)
    { AnsiString fieldTitle=getSubstring(subString,',',3);
      leftBox->Items->Add(fieldName+" - "+fieldTitle);
    }
    n++;
  }
  leftBox->ItemIndex=0;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TContentConfigForm::CancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TContentConfigForm::SaveButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TContentConfigForm::toRightButtonClick(TObject *Sender)
{
AnsiString str;

  if(leftBox->Items->Count==0) return;
  if(leftBox->ItemIndex<0)
  { leftBox->ItemIndex=0;
    leftBox->Selected[0]=true;
  }
  str=leftBox->Items->Strings[leftBox->ItemIndex];
  rightBox->Items->Insert(rightBox->ItemIndex,str);
  leftBox->Items->Delete(leftBox->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TContentConfigForm::toLeftButtonClick(TObject *Sender)
{
AnsiString str;

  if(rightBox->Items->Count==0) return;
  if(rightBox->ItemIndex<0)
  { rightBox->ItemIndex=0;
    rightBox->Selected[0]=true;
  }
  str=rightBox->Items->Strings[rightBox->ItemIndex];
  leftBox->Items->Insert(leftBox->ItemIndex,str);
  rightBox->Items->Delete(rightBox->ItemIndex);
}
//---------------------------------------------------------------------------

