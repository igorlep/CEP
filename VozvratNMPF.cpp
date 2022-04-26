//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "GridF.h"
#include "GridContentF.h"
#include "ReturnNMPtoPublishersF.h"
#include "ViewNMPBySD.h"
#include "VozvratNMPFunctions.h"
#include "VozvratNMPF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TVozvratNMPFrame *VozvratNMPFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TVozvratNMPFrame::TVozvratNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName=iniSN+"_VozvratNMPFrame";
  parent=static_cast<TReturnNMPtoPublishersFrame*>(Owner);
  nmpFrame=NULL;
}
//---------------------------------------------------------------------------
__fastcall TVozvratNMPFrame::~TVozvratNMPFrame(void)
{
  if(nmpFrame!=NULL) delete nmpFrame;
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::initFrame(void)
{
  nmpFrame=createVozvratNMPFrame(this,iniFileName,iniSectionName,fbDB,nmpPanel);
  if(nmpFrame!=NULL)
  { nmpFrame->setSEdit(parent->SEdit);
    nmpFrame->setConditionSBox(parent->conditionSBox);
    nmpFrame->workGrid->PopupMenu=nmpGridMenu;
    nmpFrame->workGrid->OnMouseDown=parent->nmpGridMouseDown;
    nmpFrame->workGrid->OnKeyDown=workGridKeyDown;
    nmpFrame->setHeader(parent->nmpHeader);
    nmpFrame->workSource->OnDataChange=parent->nmpSourceDataChange;
    nmpFrame->setStatusBar(parent->nmpStatusBar);
    nmpFrame->workSource->OnStateChange=nmpSourceStateChange;
    nmpFrame->getWorkDataSet()->BeforePost=nmpDataSetBeforePost;
    nmpFrame->getWorkDataSet()->AfterPost=nmpDataSetAfterPost;
//    nmpFrame->workGrid->Columns->Items[7]->OnGetCellParams=nmpGridColumns7GetCellParams;
//    nmpFrame->workGrid->Columns->Items[8]->OnGetCellParams=nmpGridColumns8GetCellParams;
  }
}
//---------------------------------------------------------------------------
TpFIBDataSet * __fastcall TVozvratNMPFrame::getWorkDataSet(void)
{
  if(nmpFrame!=NULL) return nmpFrame->getWorkDataSet();
  else return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::loadDS(void)
{
  loadNMPDS();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::loadSearchDS(void)
{
  loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::setFocus(void)
{
  if(nmpFrame!=NULL) nmpFrame->workGrid->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::reLoadDS(void)
{
AnsiString e_id=nmpFrame->getFieldDataOfSelectedRecord("E_ID");

  loadNMPDS();
  nmpFrame->getWorkDataSet()->Locate("E_ID",e_id,TLocateOptions().Clear());
  nmpFrame->workGrid->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::forwardSearch(void)
{
  if(nmpFrame!=NULL) nmpFrame->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::backwardSearch(void)
{
  if(nmpFrame!=NULL) nmpFrame->backwardSearch();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TVozvratNMPFrame::loadNMPDS(void)
{
  if(nmpFrame!=NULL)
  { nmpFrame->workSource->DataSet->Close();
    TGridContentFrame *pF=parent->getPublishersFrame();
    if(pF->getGridFrame()->workSource->DataSet->Active==false || pF->getGridFrame()->workSource->DataSet->RecordCount==0) return;
    bool selectForPublisher=parent->selectForPublisherChecked();
    AnsiString pID=parent->getPublisherID();
    AnsiString sql=formVNMPSelectSQL(selectForPublisher,pID,parent->getDate1(),parent->getDate2());
    nmpFrame->initSelectSQL(sql);
    nmpFrame->loadDS();
  }
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::loadNMPSearchDS(void)
{
AnsiString pID;

  if(nmpFrame==NULL || nmpFrame->getWorkDataSet()->RecordCount==0) return;
  pID=parent->getPublisherID();
  bool selectForPublisher=parent->selectForPublisherChecked();
  AnsiString sql=formVNMPSearchSQL(selectForPublisher,pID,parent->getDate1(),parent->getDate2());
  nmpFrame->initSearchSQL(sql);
  nmpFrame->loadSearchingDS();
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TVozvratNMPFrame::nmpSourceStateChange(TObject *Sender)
{
  if(nmpFrame->workGrid->DataSource->DataSet->State==dsEdit)
  { nmpSaveItem->Enabled=true;
    nmpCancelItem->Enabled=true;
  }
  else
  { nmpSaveItem->Enabled=false;
    nmpCancelItem->Enabled=false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::nmpDataSetBeforePost(TDataSet *DataSet)
{
  int q_return=DataSet->FieldByName("Q_RETURN")->AsInteger;
  int return_q=DataSet->FieldByName("RETURN_Q")->AsInteger;
  if(q_return==0)
  { DataSet->FieldByName("Q_RETURN")->Clear();
    DataSet->FieldByName("DATE_RETURN")->Clear();
  }
  else
  { unsigned short y,m,d;
    parent->getDate2().DecodeDate(&y,&m,&d);
    AnsiString date_return=TDateTime(y,m,lastDayOfMonth(y,m)).DateString();
    DataSet->FieldByName("DATE_RETURN")->AsDateTime=date_return;
    if(DataSet->FieldByName("Q_RETURN")->AsInteger > DataSet->FieldByName("E_Q")->AsInteger)
      DataSet->FieldByName("Q_RETURN")->AsInteger=DataSet->FieldByName("E_Q")->AsInteger;
    if(q_return > return_q)
    { MessageDlg("\"Количество | возвратить\" не может быть больше \"Количество | на складе\"",mtWarning,TMsgDlgButtons() << mbOK,0);
      DataSet->FieldByName("Q_RETURN")->AsInteger=return_q;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::nmpDataSetAfterPost(TDataSet *DataSet)
{
//AnsiString e_id=nmpFrame->getFieldDataOfSelectedRecord("E_ID");

  parent->reloadP_DS();
//  nmpFrame->getWorkDataSet()->Locate("E_ID",e_id,TLocateOptions().Clear());
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::nmpSaveItemClick(TObject *Sender)
{
  if(nmpFrame->getWorkDataSet()->State==dsEdit) nmpFrame->getWorkDataSet()->Post();
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::nmpCancelItemClick(TObject *Sender)
{
  if(nmpFrame->getWorkDataSet()->State==dsEdit) nmpFrame->getWorkDataSet()->Cancel();
}
//---------------------------------------------------------------------------
//Настройка колонок
void __fastcall TVozvratNMPFrame::columnsConfigItemClick(TObject *Sender)
{
  nmpFrame->columnsConfigClick(Sender);
}
//---------------------------------------------------------------------------
//Заполнение полей Q_RETURN, DATE_RETURN и NDS_STAVKA таблицы EXITS_TABLE
//на основании данных по возврату из подразделений
void __fastcall TVozvratNMPFrame::fill_NMP_DSButtonClick(TObject *Sender)
{
AnsiString sql;
AnsiString date_return;
TpFIBDataSet *nmpDS=nmpFrame->getWorkDataSet();
int rn=nmpDS->RecNo;
FBQuery *fbWQ=fbDB->getFBQ_T2();
bool selectForPublisher=parent->selectForPublisherChecked();
AnsiString p_id=parent->getPublisherID();
TGridContentFrame *pF=parent->getPublishersFrame();

  date_return=parent->getDate2();
  //Чистим поля Q_RETURN, DATE_RETURN и NDS_STAVKA таблицы EXITS_TABLE
  sql="update EXITS_TABLE e";
  sql+=" set e.Q_RETURN=NULL,e.DATE_RETURN=NULL";
  sql+=" where e.DATE_RETURN='"+date_return+"'";
  if(selectForPublisher==false)
    sql+=" and e.nmp_id in (select n.nmp_id from nmp_table n where n.ret_must_flg=1)";
  else
  { sql+=" and e.nmp_id in";
    sql+="(select n.nmp_id from nmp_table n left outer join publishers_table p on p.p_id=n.p_id where p.p_id="+p_id+" and n.ret_must_flg=1)";
  }
  if(fbWQ->execQuery("TSpisanieNMPFrame::fill_NMP_DSButtonClick",sql)==false) return;

  nmpFrame->Visible=false;
  pF->Visible=false;
  parent->nmpBar->Visible=true;
  parent->nmpBar->Max=nmpDS->RecordCount;
  parent->nmpBar->Position=0;

//  fillFLG=true;
  nmpDS->First();
  while(!nmpDS->Eof)
  { if(nmpDS->FieldByName("RET_MUST_FLG")->AsInteger==1)
    { int q_return=nmpDS->FieldByName("RETURN_Q")->AsInteger;
      if(q_return != 0)
      { sql="update EXITS_TABLE set Q_RETURN="+IntToStr(q_return);
        sql+=",DATE_RETURN='"+date_return+"'";
        sql+=" where E_ID="+nmpDS->FieldByName("E_ID")->AsString;
        if(fbWQ->execQuery("TSpisanieNMPFrame::fill_NMP_DSButtonClick",sql)==false) return;
      }
    }
    nmpDS->Next();
    parent->nmpBar->StepIt();
  }
  parent->nmpBar->Position=0;
  parent->nmpBar->Visible=false;
  pF->Visible=true;
  nmpFrame->Visible=true;
//  fillFLG=false;
  parent->reloadP_DS();
  loadNMPDS();
  nmpDS->RecNo=rn;
}
//---------------------------------------------------------------------------
void __fastcall TVozvratNMPFrame::workGridKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{
TpFIBDataSet *ds=nmpFrame->getWorkDataSet();

  if(Key==VK_DOWN && ds->RecNo==ds->RecordCount){ Key=0; return; }
}
//---------------------------------------------------------------------------
//Показать количество возвращенной на склад ГЖП по подразделениям
void __fastcall TVozvratNMPFrame::viewBySDItemClick(TObject *Sender)
{
TViewNMPBySDForm *VSD;
AnsiString id=nmpFrame->getFieldDataOfSelectedRecord("E_ID");;
AnsiString nmp_name=nmpFrame->getFieldDataOfSelectedRecord("NAME_NMP");

  try
  {
    VSD=new TViewNMPBySDForm(this,iniFileName,fbDB,2,id,parent->getDate1(),parent->getDate2(),nmp_name);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания формы TViewNMPBySDForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  VSD->ShowModal();
  delete VSD;
}
//---------------------------------------------------------------------------

