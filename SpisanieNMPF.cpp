//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "GridF.h"
#include "GridContentF.h"
#include "ReturnNMPtoPublishersF.h"
#include "ViewNMPBySD.h"
#include "SpisanieNMPFunctions.h"
#include "SpisanieNMPF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPCheckCtrls"
#pragma link "JvXPCore"
#pragma resource "*.dfm"
//TSpisanieNMPFrame *SpisanieNMPFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TSpisanieNMPFrame::TSpisanieNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbdb)
        : TFrame(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  iniSectionName=iniSN+"_SpisanieNMPFrame";
  parent=static_cast<TReturnNMPtoPublishersFrame*>(Owner);
  nmpFrame=NULL;
  fillFLG=false;
  ST_1->Caption="0.0";
  ST_2->Caption="0.0";
  ST_3->Caption="0.0";
  ST_4->Caption="0.0";
}
//---------------------------------------------------------------------------
__fastcall TSpisanieNMPFrame::~TSpisanieNMPFrame(void)
{
  if(nmpFrame!=NULL) delete nmpFrame;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::initFrame(void)
{
  nmpFrame=createSpisanieNMPFrame(this,iniFileName,iniSectionName,fbDB,nmpPanel);
  if(nmpFrame!=NULL)
  { nmpF=nmpFrame->getGridFrame();
    nmpF->setSEdit(parent->SEdit);
    nmpF->setConditionSBox(parent->conditionSBox);
    nmpF->workGrid->PopupMenu=nmpGridMenu;
    nmpF->workGrid->OnMouseDown=parent->nmpGridMouseDown;
    nmpF->workGrid->OnKeyDown=workGridKeyDown;
    nmpF->workGrid->OnDblClick=nmpGridDblClick;
    nmpF->setHeader(parent->nmpHeader);
    nmpF->workSource->OnDataChange=parent->nmpSourceDataChange;
    nmpF->setStatusBar(parent->nmpStatusBar);
    nmpF->workSource->OnStateChange=nmpSourceStateChange;
    nmpF->getWorkDataSet()->BeforePost=nmpDataSetBeforePost;
    nmpF->getWorkDataSet()->AfterPost=nmpDataSetAfterPost;
    nmpF->workGrid->OnGetCellParams=nmpGridGetCellParams;
    additionalAdjustment();
  }
}
//---------------------------------------------------------------------------
TpFIBDataSet * __fastcall TSpisanieNMPFrame::getWorkDataSet(void)
{
  if(nmpF!=NULL) return nmpF->getWorkDataSet();
  else return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::loadDS(void)
{
  loadNMPDS();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::loadSearchDS(void)
{
  loadNMPSearchDS();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::setFocus(void)
{
  if(nmpF!=NULL) nmpF->workGrid->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::reLoadDS(void)
{
AnsiString e_id=nmpF->getFieldDataOfSelectedRecord("E_ID");

  loadNMPDS();
  nmpF->getWorkDataSet()->Locate("E_ID",e_id,TLocateOptions().Clear());
  nmpF->workGrid->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::forwardSearch(void)
{
  if(nmpF!=NULL) nmpF->forwardSearch();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::backwardSearch(void)
{
  if(nmpF!=NULL) nmpF->backwardSearch();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TSpisanieNMPFrame::loadNMPDS(void)
{
  if(nmpF!=NULL)
  { nmpF->workSource->DataSet->Close();
    TGridContentFrame *pF=parent->getPublishersFrame();
    if(pF->getGridFrame()->workSource->DataSet->Active==false || pF->getGridFrame()->workSource->DataSet->RecordCount==0) return;
    bool selectForPublisher=parent->selectForPublisherChecked();
    AnsiString pID=parent->getPublisherID();
    bool delDistrRecords=delDistrRecordsBox->Checked;
    AnsiString sql=formNMPSelectSQL(selectForPublisher,pID,delDistrRecords,getStavkaNDS(),parent->getDate1(),parent->getDate2());
    nmpFrame->loadDS(sql);
    getSum1();
  }
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::loadNMPSearchDS(void)
{
AnsiString pID;

  if(nmpF==NULL || nmpF->getWorkDataSet()->RecordCount==0) return;
  pID=parent->getPublisherID();
  bool selectForPublisher=parent->selectForPublisherChecked();
  bool delDistrRecords=delDistrRecordsBox->Checked;
  int stavkaNDS=getStavkaNDS();
  AnsiString sql=formNMPSearchSQL(selectForPublisher,pID,delDistrRecords,stavkaNDS,parent->getDate1(),parent->getDate2());
  nmpF->initSearchSQL(sql);
  nmpF->loadSearchingDS();
}
//---------------------------------------------------------------------------
int __fastcall TSpisanieNMPFrame::getStavkaNDS(void)
{
  if(NDSBox->ItemIndex==1) return 0;
  else if(NDSBox->ItemIndex==2) return 10;
  else if(NDSBox->ItemIndex==3) return 18;
  else return -1;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::getSum1(void)
{
  if(fillFLG==true) return;

  ST_1->Caption="";
  FBQuery *fbRQ;
  try
  {
    fbRQ=newFBQuery(this,"TSpisanieNMPFrame::getSum1",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  int stavka_nds=getStavkaNDS();
  AnsiString sql;
  sql="select sum(e.r_q*e.e_price) as s1,";
  sql+="sum(e.q_return*e.e_price) as s2,";
  sql+="sum(e.q_lost*e.e_price) as s3";
  sql+=" from exits_table e,nmp_table n";
  sql+=" where n.nmp_id=e.nmp_id";
  sql+=" and e.r_date between '"+parent->getDate1().DateString()+"'";
  sql+=" and '"+parent->getDate2().DateString()+"'";
  sql+=" and (e.date_return is null";
  sql+=" or (e.date_return between '"+parent->getDate1().DateString()+"'";
  sql+=" and '"+parent->getDate2().DateString()+"'))";
  if(stavka_nds>=0)
    sql+=" and e.nds_stavka="+IntToStr(stavka_nds);
  if(parent->selectForPublisherChecked()==true)
    sql+=" and n.p_id="+parent->getPublisherID();
  sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
  if(fbRQ->execQuery("TSpisanieNMPFrame::getSum1",sql)==false)
  { delete fbRQ;
    return;
  }
  double st1=0.0,st2,st3;
  if(!fbRQ->getQuery()->FieldByName("s1")->IsNull) st1=fbRQ->getQuery()->FieldByName("s1")->AsFloat;
  if(!fbRQ->getQuery()->FieldByName("s2")->IsNull) st2=fbRQ->getQuery()->FieldByName("s2")->AsFloat;
  if(!fbRQ->getQuery()->FieldByName("s3")->IsNull) st3=fbRQ->getQuery()->FieldByName("s3")->AsFloat;
  delete fbRQ;
  ST_1->Caption=AnsiString().sprintf("%.02lf",st1);
  ST_2->Caption=AnsiString().sprintf("%.02lf",st2);
  ST_3->Caption=AnsiString().sprintf("%.02lf",st3);
  getSum4();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::getSum2(void)
{
  if(fillFLG==true) return;

  ST_2->Caption="";
  FBQuery *fbRQ;
  try
  {
    fbRQ=newFBQuery(this,"TSpisanieNMPFrame::getSum2",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  int stavka_nds=getStavkaNDS();
  AnsiString sql;
  sql="select sum(e.q_return*e.e_price) as summa";
  sql+=" from exits_table e,nmp_table n";
  sql+=" where n.nmp_id=e.nmp_id";
  sql+=" and e.date_return between '"+parent->getDate1().DateString()+"'";
  sql+=" and '"+parent->getDate2().DateString()+"'";
  sql+=" and e.q_return is not null";
  if(stavka_nds>=0)
    sql+=" and e.nds_stavka="+IntToStr(stavka_nds);
  if(parent->selectForPublisherChecked()==true)
    sql+=" and n.p_id="+parent->getPublisherID();
  sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
  if(fbRQ->execQuery("TSpisanieNMPFrame::getSum2",sql)==false)
  { delete fbRQ;
    return;
  }
  double st2=0.0;
  if(!fbRQ->getQuery()->FieldByName("summa")->IsNull) st2=fbRQ->getQuery()->FieldByName("summa")->AsFloat;
  delete fbRQ;
  ST_2->Caption=AnsiString().sprintf("%.02lf",st2);
  getSum4();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::getSum3(void)
{
  if(fillFLG==true) return;

  ST_3->Caption="";
  FBQuery *fbRQ;
  try
  {
    fbRQ=newFBQuery(this,"TSpisanieNMPFrame::getSum3",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  int stavka_nds=getStavkaNDS();
  AnsiString sql;
  sql="select sum(e.q_lost*e.e_price) as summa";
  sql+=" from exits_table e,nmp_table n";
  sql+=" where n.nmp_id=e.nmp_id";
  sql+=" and e.date_return between '"+parent->getDate1().DateString()+"'";
  sql+=" and '"+parent->getDate2().DateString()+"'";
  sql+=" and e.q_lost is not null";
  if(stavka_nds>=0)
    sql+=" and e.nds_stavka="+IntToStr(stavka_nds);
  if(parent->selectForPublisherChecked()==true)
    sql+=" and n.p_id="+parent->getPublisherID();
  sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
  if(fbRQ->execQuery("TSpisanieNMPFrame::getSum3",sql)==false)
  { delete fbRQ;
    return;
  }
  double st3=0.0;
  if(!fbRQ->getQuery()->FieldByName("summa")->IsNull) st3=fbRQ->getQuery()->FieldByName("summa")->AsFloat;
  delete fbRQ;
  ST_3->Caption=AnsiString().sprintf("%.02lf",st3);
  getSum4();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::getSum4(void)
{
  if(fillFLG==true) return;

  double st4=StrToFloat(ST_1->Caption)-StrToFloat(ST_2->Caption)-StrToFloat(ST_3->Caption);
  ST_4->Caption=AnsiString().sprintf("%.02lf",st4);
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::additionalAdjustment(void)
{
int p=nmpF->findPozitionColumnInGrid("RETURN_Q");

  if(p > 0)
  { p--;
    nmpF->workGrid->Columns->Items[p]->OnGetCellParams=nmpGridColumns8GetCellParams;
  }
  p=nmpF->findPozitionColumnInGrid("Q_RETURN");
  if(p > 0)
  { p--;
    nmpF->workGrid->Columns->Items[p]->OnGetCellParams=nmpGridColumns9GetCellParams;
  }
  p=nmpF->findPozitionColumnInGrid("REAL_RP");
  if(p > 0)
  { p--;
    nmpF->workGrid->Columns->Items[p]->OnGetCellParams=nmpGridColumns10GetCellParams;
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TSpisanieNMPFrame::workPanelResize(TObject *Sender)
{
int p=nmpButtonsPanel->Width;

  ST_4->Left=p-ST_4->Width-5;
  p=ST_4->Left-2;
  labelST_4->Left=p-labelST_4->Width;
  p=labelST_4->Left-8;
  ST_3->Left=p-ST_3->Width-5;
  p=ST_3->Left-2;
  labelST_3->Left=p-labelST_3->Width;
  p=labelST_3->Left-8;
  ST_2->Left=p-ST_2->Width;
  p=ST_2->Left-2;
  labelST_2->Left=p-labelST_2->Width;
  p=labelST_2->Left-8;
  ST_1->Left=p-ST_1->Width;
  p=ST_1->Left-2;
  labelST_1->Left=p-labelST_1->Width;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpSourceStateChange(TObject *Sender)
{
  if(fillFLG==true) return;
  if(nmpF->workGrid->DataSource->DataSet->State==dsEdit)
  { nmpSaveItem->Enabled=true;
    nmpCancelItem->Enabled=true;
  }
  else
  { nmpSaveItem->Enabled=false;
    nmpCancelItem->Enabled=false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpDataSetBeforePost(TDataSet *DataSet)
{
  if(fillFLG==false)
  { int q_return=DataSet->FieldByName("Q_RETURN")->AsInteger;
    if(q_return==0)
    { DataSet->FieldByName("Q_RETURN")->Clear();
//      DataSet->FieldByName("DATE_RETURN")->Clear();
    }
    else
    { //unsigned short y,m,d;
//      TDateTime date_return;
//      if(DataSet->FieldByName("DATE_RETURN")->IsNull)
//      { parent->getDate2().DecodeDate(&y,&m,&d);
//        date_return=TDateTime(y,m,lastDayOfMonth(y,m));
//        DataSet->FieldByName("DATE_RETURN")->AsDateTime=date_return;
//      }
//      else
//      { date_return=DataSet->FieldByName("DATE_RETURN")->AsDateTime;
//        date_return.DecodeDate(&y,&m,&d);
//        TDateTime dr=TDateTime(y,m,lastDayOfMonth(y,m));
//        if(date_return!=dr) DataSet->FieldByName("DATE_RETURN")->AsDateTime=dr;
//      }

      int eq=DataSet->FieldByName("E_Q")->AsInteger;      //разнаряжено
      int rq=DataSet->FieldByName("RETURN_Q")->AsInteger; //возвращено на склад
      int ql=DataSet->FieldByName("Q_LOST")->AsInteger;   //потери
      int qr=DataSet->FieldByName("Q_RETURN")->AsInteger; //возвращено поставщику
      if(qr > eq)
        DataSet->FieldByName("Q_RETURN")->AsInteger=eq;
      if(ql > rq)
        DataSet->FieldByName("Q_LOST")->AsInteger=rq;
    }
    int lost=DataSet->FieldByName("Q_LOST")->AsInteger;
    if(lost==0) DataSet->FieldByName("LOST_FLG")->AsInteger=0;
    else DataSet->FieldByName("LOST_FLG")->AsInteger=1;
    DataSet->FieldByName("DATE_RETURN")->AsDateTime=getDateReturnToPublisher(DataSet->FieldByName("DATE_RETURN")->AsDateTime);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridDblClick(TObject *Sender)
{
TpFIBDataSet *nmpDS=nmpF->getWorkDataSet();
AnsiString eid=nmpF->getFieldDataOfSelectedRecord("E_ID");

  if(nmpF->workGrid->Columns->Items[nmpF->workGrid->Col-1]->FieldName=="Q_LOST")
  { int rq=0,  //возвращено на склад
        qr=0,  //возвращено поставщику
        mbr=0, //может быть возвращено
        ql=0;  //потери
    if(nmpF->getFieldDataOfSelectedRecord("RETURN_Q")!=NULL && !nmpF->getFieldDataOfSelectedRecord("RETURN_Q").IsEmpty())
      rq=StrToInt(nmpF->getFieldDataOfSelectedRecord("RETURN_Q"));
    if(nmpF->getFieldDataOfSelectedRecord("Q_RETURN")!=NULL && !nmpF->getFieldDataOfSelectedRecord("Q_RETURN").IsEmpty())
      qr=StrToInt(nmpF->getFieldDataOfSelectedRecord("Q_RETURN"));
    if(nmpF->getFieldDataOfSelectedRecord("MAY_BE_RETURN")!=NULL && !nmpF->getFieldDataOfSelectedRecord("MAY_BE_RETURN").IsEmpty())
      mbr=StrToInt(nmpF->getFieldDataOfSelectedRecord("MAY_BE_RETURN"));
    qr=rq;
    if(qr>mbr)
    { qr=mbr;
      ql=rq-mbr;
    }
    nmpDS->Edit();
    nmpDS->FieldByName("Q_RETURN")->AsInteger=qr;
    nmpDS->FieldByName("Q_LOST")->AsInteger=ql;
    nmpDS->Post();
  }
  else if(nmpF->workGrid->Columns->Items[nmpF->workGrid->Col-1]->FieldName=="Q_RETURN")
  { int qe=0; //разнаряжено
    int rq=0; //возвращено на склад
    int qr=0; //возвращено поствщику
    int ql=0; //потери
    qe=StrToInt(nmpF->getFieldDataOfSelectedRecord("E_Q"));
    if(nmpF->getFieldDataOfSelectedRecord("RETURN_Q")!=NULL && !nmpF->getFieldDataOfSelectedRecord("RETURN_Q").IsEmpty())
      rq=StrToInt(nmpF->getFieldDataOfSelectedRecord("RETURN_Q"));
    if(nmpF->getFieldDataOfSelectedRecord("Q_RETURN")!=NULL && !nmpF->getFieldDataOfSelectedRecord("Q_RETURN").IsEmpty())
      qr=StrToInt(nmpF->getFieldDataOfSelectedRecord("Q_RETURN"));
    if(nmpF->getFieldDataOfSelectedRecord("Q_LOST")!=NULL && !nmpF->getFieldDataOfSelectedRecord("Q_LOST").IsEmpty())
      ql=StrToInt(nmpF->getFieldDataOfSelectedRecord("Q_LOST"));
    if(ql != 0)
    { qr+=ql;
      if(qr > qe)
      { ql=qr-qe;;
        qr=qe;
      }
      else ql=0;
    }
    else qr=rq;

    nmpDS->Edit();
    nmpDS->FieldByName("Q_RETURN")->AsInteger=qr;
    nmpDS->FieldByName("Q_LOST")->AsInteger=ql;
    nmpDS->Post();
  }
  nmpDS->Locate("E_ID",eid,TLocateOptions().Clear());
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpDataSetAfterPost(TDataSet *DataSet)
{
AnsiString e_id=nmpF->getFieldDataOfSelectedRecord("E_ID");

  getSum2();
  parent->reloadP_DS();
  nmpF->getWorkDataSet()->Locate("E_ID",e_id,TLocateOptions().Clear());
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridColumns8GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params)
{
TpFIBDataSet *ds=nmpF->getWorkDataSet();
int rq=ds->FieldByName("RETURN_Q")->AsInteger;       //возвращено на склад
int qr=ds->FieldByName("Q_RETURN")->AsInteger;       //возвращено поставщику
int mbr=ds->FieldByName("MAY_BE_RETURN")->AsInteger; //может быть возвращено

  //если на складе > чем может быть списано
  if(rq > mbr) Params->Font->Color=clRed;
  //иначе если на складе < чем может быть списано и возвращено поставщику < чем может быть списано
  else if(rq<mbr && qr<mbr) Params->Font->Color=clGreen;
  else Params->Font->Color=clWindowText;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridColumns9GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params)
{
TpFIBDataSet *ds=nmpF->getWorkDataSet();
int rq=ds->FieldByName("RETURN_Q")->AsInteger;      //возвращено на склад
int qr=ds->FieldByName("Q_RETURN")->AsInteger;      //списано
int mbr=ds->FieldByName("MAY_BE_RETURN")->AsInteger; //может быть возвращено

  //если списано > чем может быть списано
  if(qr > mbr) Params->Font->Color=clRed;
  //если списано > чем на складе
  else if(qr > rq) Params->Font->Color=clBlue;
  //если списано < чем может быть списано
  else if(qr < mbr) Params->Font->Color=clGreen;
  //если списано = чем может быть списано
  else Params->Font->Color=clWindowText;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridColumns10GetCellParams(TObject *Sender,bool EditMode,TColCellParamsEh *Params)
{
TpFIBDataSet *ds=nmpF->getWorkDataSet();
double rp=ds->FieldByName("REMOVE_PERCENT")->AsFloat; //допустимый процент списания
double rrp=ds->FieldByName("REAL_RP")->AsFloat;       //действительный процент списания

  if(rrp-rp > 0.5) Params->Font->Color=clRed;
  else Params->Font->Color=clWindowText;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::NDSBoxChange(TObject *Sender)
{
  reLoadDS();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::delDistrRecordsBoxClick(TObject *Sender)
{
  reLoadDS();
}
//---------------------------------------------------------------------------
//Заполнение полей Q_RETURN, DATE_RETURN и NDS_STAVKA таблицы EXITS_TABLE
//на основании данных по возврату из подразделений
void __fastcall TSpisanieNMPFrame::fill_NMP_DSButtonClick(TObject *Sender)
{
AnsiString sql;
AnsiString date_return;
TpFIBDataSet *nmpDS=nmpF->getWorkDataSet();
int rn=nmpDS->RecNo;
FBQuery *fbWQ=fbDB->getFBQ_T2();
bool selectForPublisher=parent->selectForPublisherChecked();
AnsiString p_id=parent->getPublisherID();
TGridContentFrame *pF=parent->getPublishersFrame();
//unsigned short y,m,d;

//  parent->getDate2().DecodeDate(&y,&m,&d);
//  date_return=TDateTime(y,m,lastDayOfMonth(y,m)).DateString();
  date_return=parent->getDate2();
  //Чистим поля Q_RETURN, DATE_RETURN таблицы EXITS_TABLE
  sql="update EXITS_TABLE e";
  sql+=" set e.Q_RETURN=NULL,e.DATE_RETURN=NULL";
  sql+=" where e.DATE_RETURN='"+date_return+"'";
  if(NDSBox->ItemIndex > 0) sql+=" and e.NDS_STAVKA="+IntToStr(getStavkaNDS());
  if(selectForPublisher==false)
    sql+=" and e.nmp_id in (select n.nmp_id from nmp_table n where (n.ret_must_flg is NULL or n.ret_must_flg=0))";
  else
  { sql+=" and e.nmp_id in";
    sql+="(select n.nmp_id from nmp_table n left outer join publishers_table p on p.p_id=n.p_id where p.p_id="+p_id+" and (n.ret_must_flg is NULL or n.ret_must_flg=0))";
  }
  if(fbWQ->execQuery("TSpisanieNMPFrame::fill_NMP_DSButtonClick",sql)==false) return;

  nmpF->Visible=false;
  pF->Visible=false;
  parent->nmpBar->Visible=true;
  parent->nmpBar->Max=nmpDS->RecordCount;
  parent->nmpBar->Position=0;
  //Первый проход
  // Если RETURN_Q <= MAY_BE_RETURN делаем Q_RERURN = RETURN_Q
  // Иначе Q_RERURN = MAY_BE_RETURN
  fillFLG=true;
  nmpDS->First();
  while(!nmpDS->Eof)
  { if(nmpDS->FieldByName("RET_MUST_FLG")->AsInteger==0)
    { int may_be_ret=nmpDS->FieldByName("MAY_BE_RETURN")->AsInteger;
      int return_q=nmpDS->FieldByName("RETURN_Q")->AsInteger; //возврашено на склад
      int q_return; //возвращено поставщику
      if(return_q <= may_be_ret) q_return=return_q;
      else q_return=may_be_ret;

      int q_lost=0; //потери
      if(q_return < return_q) q_lost=return_q-q_return;

      sql="update EXITS_TABLE set Q_RETURN="+IntToStr(q_return);
      sql+=",DATE_RETURN='"+date_return+"'";
      if(q_lost > 0)
      { sql+=",Q_LOST="+IntToStr(q_lost);
        sql+=",LOST_FLG=1";
      }
      sql+=" where E_ID="+nmpDS->FieldByName("E_ID")->AsString;
      if(fbWQ->execQuery("TSpisanieNMPFrame::fill_NMP_DSButtonClick",sql)==false) return;
    }
    nmpDS->Next();
    parent->nmpBar->StepIt();
  }
  //Второй проход
  parent->nmpBar->Position=0;


  parent->nmpBar->Visible=false;
  fillFLG=false;
  pF->Visible=true;
  nmpF->Visible=true;
  parent->reloadP_DS();
  loadNMPDS();
  nmpDS->RecNo=rn;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpSaveItemClick(TObject *Sender)
{
  if(nmpF->getWorkDataSet()->State==dsEdit) nmpF->getWorkDataSet()->Post();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpCancelItemClick(TObject *Sender)
{
  if(nmpF->getWorkDataSet()->State==dsEdit) nmpF->getWorkDataSet()->Cancel();
}
//---------------------------------------------------------------------------
//Настройка колонок
void __fastcall TSpisanieNMPFrame::columnsConfigItemClick(TObject *Sender)
{
  nmpF->columnsConfigClick(Sender);
  additionalAdjustment();
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::workGridKeyDown(TObject *Sender, WORD &Key,TShiftState Shift)
{
TpFIBDataSet *ds=nmpFrame->getGridFrame()->getWorkDataSet();

  if(Key==VK_DOWN && ds->RecNo==ds->RecordCount){ Key=0; return; }
}
//---------------------------------------------------------------------------
//Взаимозачет
void __fastcall TSpisanieNMPFrame::changeItemClick(TObject *Sender)
{
TpFIBDataSet *ds=nmpF->getWorkDataSet();
TDBGridEh *grid=nmpF->workGrid;
FBQuery *fbWQ;
TpFIBTransaction *T;
AnsiString eid_l=""; //код записи для строки с потерями
int ql=0;            //потери, которые надо списать
int nds_l;           //ставка НДС для строки с потерями
double price_l;      //цена ГЖП для строки с потерями
double sum_l;        //сумма потерь
AnsiString sql;

  fbWQ=newFBQuery(this,"TSpisanieNMPFrame::changeItemClick",fbDB->getDatabase(),true);
  if(fbWQ==NULL) goto end;
  T=fbWQ->getTransaction();
  //Ищем строку, в которой потери не равны нулю
  for(int i=0; i<grid->SelectedRows->Count; ++i)
  { ds->GotoBookmark((void *)grid->SelectedRows->Items[i].c_str());
    AnsiString qlost=nmpF->getFieldDataOfSelectedRecord("Q_LOST");
    if(qlost.IsEmpty()) ql=0;
    else ql=StrToInt(qlost);
    if(ql>0)
    { eid_l=nmpF->getFieldDataOfSelectedRecord("E_ID");
      nds_l=StrToInt(nmpF->getFieldDataOfSelectedRecord("NDS_STAVKA"));
      price_l=StrToFloat(nmpF->getFieldDataOfSelectedRecord("E_PRICE"));
      sum_l=ql*price_l;
      break;
    }
  }
  if(ql==0) goto end;
  T->StartTransaction();
  //Ликвидируем потери в найденной строки, увеличивая списание в строках-спонсорах
  for(int i=0; i<grid->SelectedRows->Count; ++i)
  { ds->GotoBookmark((void *)grid->SelectedRows->Items[i].c_str());
    if(nmpF->getFieldDataOfSelectedRecord("E_ID")==eid_l) continue;
    else
    { int mbq=StrToInt(nmpF->getFieldDataOfSelectedRecord("MAY_BE_RETURN")); //может быть списано
      int qr=StrToInt(nmpF->getFieldDataOfSelectedRecord("Q_RETURN"));       //возвращено поставщику
      int nds_m=StrToInt(nmpF->getFieldDataOfSelectedRecord("NDS_STAVKA"));  //ставка НДС в текущей строке
      double price_m=StrToFloat(nmpF->getFieldDataOfSelectedRecord("E_PRICE")); //цена ГЖП в текущей строке
      if(nds_m!=nds_l) continue; //перераспледеление возможно, если ставки НДС совпадают

      //Проверяем, может ли текущая строка быть спонсором
      int mr=mbq-qr; //можно списать дополнительно
      if(mr <= 0) continue;

      double sum_m=mr*price_m;   //может быть списания дополнительно
      double sum_s;              //списываем дополнительно
      if(sum_m >= sum_l) sum_s=sum_l;
      else sum_s=sum_m;
      int rl=int(floor(sum_s/price_l+0.5));
      ql-=rl;            //корректируем значение потерь
      sum_l-=rl*price_l; // и суммы потерь
      //Корректируем значение - возвращено поставщику
      qr+=int(floor(sum_s/price_m));
      AnsiString eid=nmpF->getFieldDataOfSelectedRecord("E_ID");
      sql="update EXITS_TABLE set Q_RETURN="+IntToStr(qr);
      sql+=" where E_ID="+eid;
      if(fbWQ->execQuery("TSpisanieNMPFrame::changeItemClick",sql)==false)
      { T->Rollback();
        goto end;
      }
      if(ql<=0) break;
    }
  }
  //Корректируем значение потерь
  sql="update EXITS_TABLE set Q_LOST="+IntToStr(ql);
  if(ql==0) sql+=",LOST_FLG=0";
  sql+=" where E_ID="+eid_l;
  if(fbWQ->execQuery("TSpisanieNMPFrame::changeItemClick",sql)==false) T->Rollback();
end:
  if(T->InTransaction) T->Commit();
  if(fbWQ!=NULL) delete fbWQ;
  parent->reloadP_DS();
  loadNMPDS();
  if(!eid_l.IsEmpty()) ds->Locate("E_ID",eid_l,TLocateOptions().Clear());
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridMenuPopup(TObject *Sender)
{
TDBGridEh *grid=nmpF->workGrid;

  if(grid->SelectedRows->Count >= 2) changeItem->Visible=true;
  else changeItem->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TSpisanieNMPFrame::nmpGridGetCellParams(TObject *Sender,TColumnEh *Column,TFont *AFont,TColor &Background,TGridDrawState State)
{
AnsiString str=nmpF->getFieldDataOfSelectedRecord("LOST_FLG");
//AnsiString dr=nmpF->getFieldDataOfSelectedRecord("DATE_RETURN");

  if(str!=NULL && !str.IsEmpty())
  { int lostflg=StrToInt(str);
    if(lostflg==1) Background=clBtnFace;
  }
//  if(!dr.IsEmpty())
//  { TDateTime date_ret=TDateTime(dr);
//    unsigned short y,m,d;
//    parent->getDate2().DecodeDate(&y,&m,&d);
//    TDateTime date_return=TDateTime(y,m,lastDayOfMonth(y,m));
//    if(date_return!=date_ret)
//    { Background=clBtnFace;
//      AFont->Color=clGray;
//    }
//  }
}
//---------------------------------------------------------------------------
//Показать количество возвращенной на склад ГЖП по подразделениям
void __fastcall TSpisanieNMPFrame::viewBySDItemClick(TObject *Sender)
{
TViewNMPBySDForm *VSD;
AnsiString id=nmpF->getFieldDataOfSelectedRecord("E_ID");;
AnsiString nmp_name=nmpF->getFieldDataOfSelectedRecord("NAME_NMP");

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

