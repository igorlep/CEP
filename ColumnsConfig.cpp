//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "ColumnsConfig.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "Placemnt"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "DBGridEh"
#pragma link "kbmMemTable"
#pragma resource "*.dfm"
//TColumnsConfigForm *ColumnsConfigForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TColumnsConfigForm::TColumnsConfigForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString tableT,AnsiString fieldsInf)
        : TForm(Owner),iniFileName(iniFN),iniSectionName(iniSN),tableTitle(tableT),fieldsInfo(fieldsInf)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="ColumnsConfigForm_"+iniSectionName;
  header->Caption=tableTitle;
  loadTableFromStr();
}
//---------------------------------------------------------------------------
__fastcall TColumnsConfigForm::~TColumnsConfigForm(void)
{
  if(table->Active==true) table->Close();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TColumnsConfigForm::loadTableFromStr(void)
{
AnsiString subString;
int n=1,nk=1;

  if(table->Active==false) table->Open();
  while((subString=getSubstring(fieldsInfo,';',n))!=NULL)
  { AnsiString fieldName=getSubstring(subString,',',2);
    if(fieldName!="DEL_FLG" && fieldName!="REC_TYPE")
    { table->Append();
      table->FieldByName("Number")->AsInteger=nk;
      table->FieldByName("FieldTitle")->AsString=getSubstring(subString,',',3);
      int d=StrToInt(getSubstring(subString,',',5));
      table->FieldByName("EditInGrid")->AsBoolean=!bool(d);
      d=StrToInt(getSubstring(subString,',',6));
      table->FieldByName("GridViewAble")->AsBoolean=bool(d);
      d=StrToInt(getSubstring(subString,',',7));
      table->FieldByName("TitleButton")->AsBoolean=bool(d);
      d=StrToInt(getSubstring(subString,',',8));
      table->FieldByName("SearchAble")->AsBoolean=bool(d);
      if(getSubstring(subString,',',9)!=NULL)
        table->FieldByName("FormatStr")->Value=getSubstring(subString,',',9);
      table->Post();
      nk++;
    }
    n++;
  }
}
//---------------------------------------------------------------------------
void __fastcall TColumnsConfigForm::unLoadTableToStr(void)
{
AnsiString subString;
AnsiString newStr="";
int n=1;

  if(table->Active==false) return;
  table->First();
  while((subString=getSubstring(fieldsInfo,';',n))!=NULL)
  { if(n>1) newStr+=";";
    AnsiString fieldName=getSubstring(subString,',',2);
    if(fieldName=="DEL_FLG" || fieldName=="REC_TYPE") newStr+=subString;
    else
    { newStr+=getSubstring(subString,',',1)+",";
      newStr+=getSubstring(subString,',',2)+",";
      newStr+=getSubstring(subString,',',3)+",";
      newStr+=getSubstring(subString,',',4)+",";
      int p;
      if(table->FieldByName("EditInGrid")->AsBoolean==true) p=0;
      else p=1;
      newStr+=IntToStr(p)+',';
      if(table->FieldByName("GridViewAble")->AsBoolean==true) p=1;
      else p=0;
      newStr+=IntToStr(p)+',';
      if(table->FieldByName("TitleButton")->AsBoolean==true) p=1;
      else p=0;
      newStr+=IntToStr(p)+',';
      if(table->FieldByName("SearchAble")->AsBoolean==true) p=1;
      else p=0;
      newStr+=IntToStr(p)+',';
      newStr+=table->FieldByName("FormatStr")->AsString;
      table->Next();
    }
    n++;
  }
  fieldsInfo=newStr;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TColumnsConfigForm::CancelButtonClick(TObject *Sender)
{
  Close();
  ModalResult=mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TColumnsConfigForm::SaveButtonClick(TObject *Sender)
{
  unLoadTableToStr();

  Close();
  ModalResult=mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TColumnsConfigForm::tableEditInGridChange(TField *Sender)
{
  if(table->State==dsEdit && tableEditInGrid->AsBoolean==true)
    tableSearchAble->AsBoolean=false;
}
//---------------------------------------------------------------------------
void __fastcall TColumnsConfigForm::tableSearchAbleChange(TField *Sender)
{
  if(table->State==dsEdit && tableSearchAble->AsBoolean==true)
    tableEditInGrid->AsBoolean=false;
}
//---------------------------------------------------------------------------

