//---------------------------------------------------------------------------

#include "RBWorkFunctions.h"
#pragma hdrstop
#include "SomeFunctions.h"
#include "MessageParametersForm.h"
#include "ShipmentNMPF.h"
#include "ShipmentNMPFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// - Фрейм - отгрузка ГЖП в подразделения
// -- фрейм подразделений
AnsiString __fastcall formShipmentSubdivisionsSelectSQL(bool groupOnPostOffices,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPostOffices==false)
  { sql+="select s.sd_id,s.sd_name,s.sd_email,s.sd_type,p.po_id,p.po_name,";
    sql+="sum(f.fact_Q) as SUM_FACT_Q,";
    sql+="sum(f.FACT_Q*e.e_price) as SUMMA";
    sql+=" from FACTS_TABLE f,EXITS_TABLE e,SUBDIVISIONS_TABLE s,";
    sql+="POST_OFFICES_TABLE p";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and p.po_id=s.po_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" group by s.sd_id,s.sd_name,s.sd_email,s.sd_type,p.po_id,p.po_name";
  }
  else
  { sql+="select p.po_id,p.po_name,";
    sql+="sum(f.fact_Q) as SUM_FACT_Q,";
    sql+="sum(f.FACT_Q*e.e_price) as summa";
    sql+=" from FACTS_TABLE f,EXITS_TABLE e,SUBDIVISIONS_TABLE s,";
    sql+="POST_OFFICES_TABLE p";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and p.po_id=s.po_id";
    sql+=" and s.po_id is not null";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" group by p.po_id,p.po_name";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateShipmentSubdivisionsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *sd_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="SUBDIVISIONS_TABLE,Подразделения,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,Наименование подразделения,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_EMAIL,SD_EMAIL,Электронный адрес,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_TYPE,SD_TYPE,Тип подразделения,Left,1,0,1,0,;";
  fieldsInfo+="p.PO_ID,PO_ID,Код почтамта,Center,1,0,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,Наименование почтамта,Left,1,0,1,0,;";
  fieldsInfo+="sum(f.FACT_Q),SUM_FACT_Q,Отгружено|кол-во,Right,1,1,0,0,#;";
  fieldsInfo+="sum(f.FACT_Q*e.e_price),SUMMA,Отгружено|сумма без НДС,Right,1,1,0,0,#.00";

  contentFieldsInfo ="SD_TYPE,Тип подразделения;";
  contentFieldsInfo+="PO_ID,Код почтамта;";
  contentFieldsInfo+="PO_NAME,Наименование почтамта";

  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#.00,,Right";

  try
  {
    sd_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  sd_f->setEdit(SEdit);
  sd_f->setConditionSBox(ConditionSBox);
  sd_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  sd_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //Дополнительные настройки
  sd_f->gcHeader->ColorFrom=Graphics::clGray;
  sd_f->gcHeader->ColorTo=clWhite;
  sd_f->gcHeader->Font->Color=clBlack;
  sd_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clGray;
  sd_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  sd_f->getContentFrame()->contentHeader->Font->Color=clBlack;
  sd_f->getGridFrame()->additionalAdjustmentsForGrid=subdivisionsAdditionalAdjustments;
  subdivisionsAdditionalAdjustments(sd_f->getGridFrame());
  //Настройка меню
  sd_f->columnsConfig->Visible=true;
  return sd_f;
}
//---------------------------------------------------------------------------
void __fastcall subdivisionsAdditionalAdjustments(TGridFrame *gf)
{
int poz=gf->findPozitionColumnInGrid("SD_TYPE");

  if(poz!=0)
  { poz-=1;
    //Настраиваем механизм отображения типа подразделения
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add("1");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Почтамт");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("2");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Отделение связи");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("3");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Склад");
  }
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateShipmentPostOfficesFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *po_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="POST_OFFICES_TABLE,Почтамты,PO_ID,PO_NAME,,";

  fieldsInfo ="p.PO_ID,PO_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,Наименование почтамта,Left,1,1,1,0,;";
  fieldsInfo+="sum(f.FACT_Q),SUM_FACT_Q,Отправлено,Right,1,1,0,0,#;";
  fieldsInfo+="sum(f.FACT_Q*e.e_price),SUMMA,Отправлено|сумма без НДС,Right,1,1,0,0,#.00";

  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#.00,,Right";

  try
  {
    po_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  po_f->setEdit(SEdit);
  po_f->setConditionSBox(ConditionSBox);
  //Дополнительные настройки
  po_f->gcHeader->ColorFrom=Graphics::clGray;
  po_f->gcHeader->ColorTo=clWhite;
  po_f->gcHeader->Font->Color=clBlack;
  return po_f;
}
//---------------------------------------------------------------------------
// -- фрейм отгруженной в подразделения ГЖП
AnsiString __fastcall formShipmentNMPSearchSQL(AnsiString sdID,bool groupOnPostOffices,bool groupOnPublishers,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPublishers==false)
  { sql="select distinct n.NMP_ID,e.e_id";
    sql+=" from NMP_TABLE n,exits_table e,facts_table f";
    sql+=" where e.nmp_id=n.nmp_id and f.e_id=e.e_id";
    sql+=" and e.e_date>='"+dt1.DateString()+"'";
    sql+=" and e.e_date<='"+dt2.DateString()+"'";
    sql+=" and f.sd_id="+sdID;
  }
  else
  { sql="select distinct p.P_ID";
    sql+=" from publishers_table p";
    sql+=" left outer join nmp_table n on n.p_id=p.p_id";
    sql+=" left outer join exits_table e on e.nmp_id=n.nmp_id";
    sql+=" left outer join facts_table f on f.e_id=e.e_id";
    sql+=" where e.e_date>='"+dt1.DateString()+"'";
    sql+=" and e.e_date<='"+dt2.DateString()+"'";
    sql+=" and f.sd_id="+sdID;
  }
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formShipmentNMPSelectSQL(AnsiString sdID,bool groupOnPostOffices,bool groupOnPublishers,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPublishers==false && groupOnPostOffices==false)
  { sql="select p.p_id,p.p_name,n.nmp_id,n.nmp_name||' №'||e.e_number as NMP_NAME,";
    sql+="e.e_id,f.fact_id,e.e_price,e.nds_stavka,n.invoice_type,e.r_date as RETURN_DATE,";
    sql+="f.FACT_Q as quant,";
    sql+="f.FACT_Q*e.e_price as summa";
    sql+=" from facts_table f,exits_table e,nmp_table n";
    sql+=" left outer join publishers_table p on p.p_id=n.p_id";
    sql+=" where e.e_id=f.e_id and n.nmp_id=f.nmp_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and f.sd_id="+sdID;
  }
  else if(groupOnPublishers==false && groupOnPostOffices==true)
  { sql="select p.p_id,p.p_name,n.nmp_id,n.nmp_name||' №'||e.e_number as NMP_NAME,";
    sql+="e.e_id,e.e_price,e.nds_stavka,n.invoice_type,e.r_date as RETURN_DATE,";
    sql+="sum(f.FACT_Q) as quant,";
    sql+="sum(f.FACT_Q*e.e_price) as summa";
    sql+=" from facts_table f,exits_table e,subdivisions_table s,nmp_table n";
    sql+=" left outer join publishers_table p on p.p_id=n.p_id";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and n.nmp_id=f.nmp_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and s.po_id="+sdID;
    sql+=" group by p.p_id,p.p_name,n.nmp_id,n.nmp_name,e.e_number,e.e_price,";
    sql+="e.e_id,e.nds_stavka,n.invoice_type,e.r_date";
  }
  else if(groupOnPublishers==true && groupOnPostOffices==false)
  { sql+="select p.p_id,p.p_name,";
    sql+="sum(f.FACT_Q) as quant,";
    sql+="sum(f.FACT_Q*e.e_price) as summa";
    sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p";
    sql+=" where f.sd_id="+sdID;
    sql+=" and e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" group by p.p_id,p.p_name";
  }
  else if(groupOnPublishers==true && groupOnPostOffices==true)
  { sql="select p.p_id,p.p_name,";
    sql+="sum(f.FACT_Q) as quant,";
    sql+="sum(f.FACT_Q*e.e_price) as summa";
    sql+=" from facts_table f,exits_table e,subdivisions_table s,nmp_table n";
    sql+=" left outer join publishers_table p on p.p_id=n.p_id";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and n.nmp_id=f.nmp_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and s.po_id="+sdID;
    sql+=" group by p.p_id,p.p_name";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createShipmentNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *nmp_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="NMP_TABLE,Газетно-журнальная продукция,NMP_ID,NMP_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,Код поставщика,Center,1,0,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,Наименование поставщика,Left,1,0,1,1,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,Код издания,Center,1,1,1,1,;";
  fieldsInfo+="n.nmp_name||' №'||e.e_number,NMP_NAME,Наименование издания,Left,1,1,1,1,;";
  fieldsInfo+="e.E_ID,E_ID,Код вызода,Center,1,0,0,0,;";
  fieldsInfo+="f.FACT_ID,FACT_ID,Код разнарядки,Center,1,0,0,0,;";
  fieldsInfo+="e.E_PRICE,E_PRICE,Цена,Right,1,1,0,0,#0.00;";
  fieldsInfo+="sum(f.FACT_Q),QUANT,Отправлено,Right,1,1,0,0,;";
  fieldsInfo+="sum(f.FACT_Q*e.e_price),SUMMA,Сумма без НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,Ставка НДС,Center,1,1,0,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,Тип накладной,Center,1,0,1,0,;";
  fieldsInfo+="e.R_DATE,RETURN_DATE,Дата возврата,Center,1,1,0,0,;";

  contentsInfo= "P_ID,Код поставщика;";
  contentsInfo+="P_NAME,Наименование поставщика;";
  contentsInfo+="INVOICE_TYPE,Тип накладной";

  footerFieldsInfo ="QUANT,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right";

  try
  {
    nmp_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  nmp_f->setEdit(SEdit);
  nmp_f->setConditionSBox(ConditionSBox);
  nmp_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  nmp_f->gcHeader->ColorFrom=Graphics::clGray;
  nmp_f->gcHeader->ColorTo=clWhite;
  nmp_f->gcHeader->Font->Color=clBlack;
  nmp_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clGray;
  nmp_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  nmp_f->getContentFrame()->contentHeader->Font->Color=clBlack;
  //Дополнительные настройки
  nmp_f->getGridFrame()->additionalAdjustmentsForGrid=nmpAdditionalAdjustments;
  //Настройка меню
  nmp_f->columnsConfig->Visible=true;
  return nmp_f;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createShipmentPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *p_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo="";
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,Поставщики ГЖП,P_ID,P_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,Код поставщика,Center,1,1,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,Наименование поставщика,Left,1,1,1,1,;";
  fieldsInfo+="sum(f.FACT_Q),QUANT,Отправлено,Right,1,1,0,0,;";
  fieldsInfo+="sum(f.FACT_Q*e.e_price),SUMMA,Сумма без НДС,Right,1,1,0,0,#0.00";

  footerFieldsInfo ="QUANT,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right";

  try
  {
    p_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  p_f->setEdit(SEdit);
  p_f->setConditionSBox(ConditionSBox);
  p_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  p_f->gcHeader->ColorFrom=Graphics::clGray;
  p_f->gcHeader->ColorTo=clWhite;
  p_f->gcHeader->Font->Color=clBlack;
  return p_f;
}
//---------------------------------------------------------------------------

