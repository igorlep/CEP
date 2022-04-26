//---------------------------------------------------------------------------

#pragma hdrstop
#include "NextGridF.h"
#include "DBTreeViewF.h"
#include "AddEditPPFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//****************************
// - Фрейм истории параметра *
//****************************
bool __fastcall cannDelHistoryParam(TComponent* owner,FBDatabase *fbDB,AnsiString id,AnsiString &str)
{

  return true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formHistorySelectSQL(AnsiString identifier)
{
AnsiString sql="";

  sql="select ID,CONST_ID,CONST_VALUE,CONST_DATE";
  sql+=" from CONST_VALUES";
  sql+=" where CONST_ID=(";
  sql+="select ID from CONSTANTS_TABLE where IDENTIFIER='"+identifier+"'";
  sql+=")";

  return sql;
}
//---------------------------------------------------------------------------
class TNextGridFrame * __fastcall createHistoryFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workP,int const_kind)
{
TNextGridFrame *gf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentInfo="";

  tableInfo="CONST_VALUES,История параметра,ID,CONST_VALUE,,";

  if(const_kind==1) //периодическая константа
  { fieldsInfo= "ID,ID,Код,Center,1,1,1,0,;";
    fieldsInfo+="CONST_ID,CONST_ID,Код параметра,Center,1,0,0,0,;";
    fieldsInfo+="CONST_VALUE,CONST_VALUE,Значение,Left,0,1,1,0,;";
    fieldsInfo+="CONST_DATE,CONST_DATE,Дата начала действия,Center,0,1,1,0,";
  }
  else
  { fieldsInfo= "ID,ID,Код,Center,1,1,1,0,;";
    fieldsInfo+="CONST_ID,CONST_ID,Код параметра,Center,1,0,0,0,;";
    fieldsInfo+="CONST_VALUE,CONST_VALUE,Значение,Left,0,1,1,0,";
  }
  try
  {
    gf=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workP,tableInfo,fieldsInfo,contentInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createHistoryRFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  gf->gcHeader->Visible=false;
  gf->getGCFrame()->canDelRecord=cannDelHistoryParam;
  //Настраиваем меню
  gf->addRecord->Visible=true;
  gf->editRecord->Visible=true;
  gf->delRecord->Visible=true;
  gf->separator1->Visible=true;
  return gf;
}
//---------------------------------------------------------------------------
PeriodicalParam *createPeriodicalParam(TComponent* owner,AnsiString iniFN,FBDatabase *fbdb,AnsiString ppIdent,TLabel *lab,TJvComboEdit *ed,TDateTime ppDt)
{
PeriodicalParam *pp=NULL;

  try
  {
    pp=new PeriodicalParam(owner,iniFN,fbdb,ppIdent,lab,ed,ppDt);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания объекта PeriodicalParam-"+ppIdent+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  return pp;
}
//---------------------------------------------------------------------------

