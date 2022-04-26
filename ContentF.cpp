//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "ContentF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma resource "*.dfm"
//TContentFrame *ContentFrame;
//---------------------------------------------------------------------------
static int ContentFrameNumber=1;
//---------------------------------------------------------------------------
__fastcall TContentFrame::TContentFrame(TComponent* Owner,TGridFrame *gf,AnsiString contInfo)
        : TFrame(Owner),gridFrame(gf)
{
//contInfo - строка с наименованиям и заголовками полей, выводимых в contentMemo
// Формат строки: fieldName1,fieldTitle1;fieldName2,fieldTitle2;...

AnsiString name=this->Name+IntToStr(ContentFrameNumber);
bool rez=Owner->FindComponent(name);

  if(rez==true) ContentFrameNumber++;
  this->Name=this->Name+IntToStr(ContentFrameNumber);


  fieldsName=NULL;
  fieldsTitle=NULL;
  initContentFields(contInfo);
  clearContent();
}
//---------------------------------------------------------------------------
__fastcall TContentFrame::~TContentFrame(void)
{
  if(fieldsName!=NULL){ delete fieldsName; fieldsName=NULL;}
  if(fieldsTitle!=NULL){ delete fieldsTitle; fieldsTitle=NULL;}
}
//---------------------------------------------------------------------------
void __fastcall TContentFrame::initContentFields(AnsiString contentInfo)
{
  if(fieldsName!=NULL){ delete fieldsName; fieldsName=NULL;}
  if(fieldsTitle!=NULL){ delete fieldsTitle; fieldsTitle=NULL;}
  try
  {
    fieldsName=new TStringList();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта fieldsName\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fieldsName=NULL;
    return;
  }
  try
  {
    fieldsTitle=new TStringList();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта fieldsTitle\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    fieldsTitle=NULL;
    return;
  }
  if(contentInfo==NULL || contentInfo.IsEmpty()) return;
  AnsiString subString;
  int fn=0;
  while((subString=getSubstring(contentInfo,';',fn+1))!=NULL)
  { fieldsName->Add(getSubstring(subString,',',1));
    fieldsTitle->Add(getSubstring(subString,',',2));
    fn++;
  }
}
//---------------------------------------------------------------------------
void __fastcall TContentFrame::setContent(AnsiString headerString)
{
  clearContent();
  contentHeader->Caption=headerString;
  for(int i=0; i<fieldsName->Count; i++)
  { AnsiString st=fieldsTitle->Strings[i];
    AnsiString sn=fieldsName->Strings[i];
    AnsiString sd=gridFrame->getFieldDataOfSelectedRecord(sn);
    if(sd=="True") sd="да";
    else if(sd=="False") sd="нет";
    contentMemo->Lines->Add(st+" - "+sd);
  }
}
//---------------------------------------------------------------------------
void __fastcall TContentFrame::setContent(void)
{
  AnsiString viewFieldName=gridFrame->getViewFieldName();
  TpFIBDataSet *ds=gridFrame->getWorkDataSet();
  if(ds->Active)
    setContent(ds->FieldByName(viewFieldName)->AsString);
//  setContent(gridFrame->getFieldDataOfSelectedRecord(viewFieldName));
}
//---------------------------------------------------------------------------
void __fastcall TContentFrame::clearContent(void)
{
  contentHeader->Caption="";
  contentMemo->Lines->Clear();
}
//---------------------------------------------------------------------------


