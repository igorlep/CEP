//---------------------------------------------------------------------------
//Набор функций к разделу меню "Аналитические отчеты"
#include <math.h>
#pragma hdrstop

#include "GridF.h"
#include "NextGridF.h"
#include "AnaliticalReportsFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// Фрейм - Дохожы от реализации ГЖП
// - формирование select SQL
AnsiString __fastcall formIncomsFromRealizationSelectSQL(AnsiString pid,TDateTime dt1,TDateTime dt2)
{
AnsiString sql="";

  sql="select * from INCOM_FROM_REALIZATION_NMP(";
  sql+=pid+",";
  sql+="'"+dt1.DateString()+"',";
  sql+="'"+dt2.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formIncomsFromRealizationSearchSQL(AnsiString pid,TDateTime dt1,TDateTime dt2)
{
AnsiString sql="";

  sql="select e.NMP_ID from EXITS_TABLE e";
  sql+=" left outer join NMP_TABLE n on n.nmp_id=e.nmp_id";
  sql+=" where n.p_id="+pid;
  sql+=" and e.date_return>='"+dt1.DateString()+"'";
  sql+=" and e.date_return<='"+dt2.DateString()+"'";
  return sql;
}
//---------------------------------------------------------------------------
TGridFrame * __fastcall cteateIncomsFromRealizationFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="EXITS_TABLE,Таблица выходов ГЖП,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,Код,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,Издание,Left,1,1,1,1,;";
  fieldsInfo+="C1,C1,Получено|коли- чество,Right,1,1,0,0,#;";
  fieldsInfo+="C2,C2,Получено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,Списано и возвращено|коли- чество,Right,1,1,0,0,#;";
  fieldsInfo+="C4,C4,Списано и возвращено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,Реализовано|коли- чество,Right,1,1,0,0,#;";
  fieldsInfo+="C6,C6,Реализовано|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,Реализовано|сумма в ценах реализации с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,Списано на убытки|коли- чество,Right,1,1,0,0,#;";
  fieldsInfo+="C9,C9,Списано на убытки|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,Цена с НДС|поставки,Center,1,1,0,0,#0.00;";
  fieldsInfo+="C11,C11,Цена с НДС|реализации,Center,1,1,0,0,#0.00;";
  fieldsInfo+="C12,C12,Наценка|(руб.),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C13,C13,Наценка|%,Center,1,1,0,0,#0.0;";
  fieldsInfo+="C14,C14,Доход (без учета убытков) (руб.),Right,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#0,,Right;";
  footerFieldsInfo+="C2,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C3,fvtSum,#0,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#0,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C8,fvtSum,#0,,Right;";
  footerFieldsInfo+="C9,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C12,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C14,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->setSEdit(SEdit);
    g_f->setConditionSBox(ConditionSBox);
    g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------

//******************************************************
// - Фрейм - Справка о товародвижении (по поставщикам) *
//******************************************************
//  - фрейм с результатами товародвижения
AnsiString __fastcall formMovementOfGoodsSelectSQL(AnsiString pid,TDateTime dt1,TDateTime dt2,int period,int resultType)
{
AnsiString sql="";

  if(resultType==1)
  { sql="select * from REFERENCE_ON_MOVEMENT_OF_GOODS(";
    sql+=pid+",";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"',";
    sql+=IntToStr(period)+")";
  }
  else
  { sql="select * from REFERENCE_ON_GOODSMOVEMENT_SD(";
    sql+=pid+",";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"',";
    sql+=IntToStr(period)+")";
  }
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formMovementOfGoodsSearchSQL(AnsiString pid,TDateTime dt1,TDateTime dt2,int period)
{
AnsiString sql="";

  sql="select distinct n.NMP_ID";
  sql+=" from NMP_TABLE n,EXITS_TABLE e";
  sql+=" where e.nmp_id=n.nmp_id";
  sql+=" and n.p_id="+pid;
  if(period==0)
  { sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==1)
  { sql+=" and e.e_date<'"+dt1.DateString()+"'";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==2)
  { sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==3)
  {
    sql+=" and e.r_date>'"+dt2.DateString()+"'";
  }
  return sql;
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall cteateReferenceOnMovementOfGoodsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="EXITS_TABLE,Таблица выходов ГЖП,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,Код,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,Издание,Left,1,1,1,1,;";
  fieldsInfo+="PERIOD,PERIOD,Период,Center,1,1,1,0,;";
  fieldsInfo+="C1,C1,Получено|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,Получено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,Списано или возвращено|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,Списано или возвращено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,Продано|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C6,C6,Продано|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,Продано|сумма в ценах реализации с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,Средняя цена реализации с НДС,Center,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#,,Right;";
  footerFieldsInfo+="C2,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C3,fvtSum,#,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->setSEdit(SEdit);
    g_f->setConditionSBox(ConditionSBox);
    g_f->additionalAdjustmentsForGrid=movementOfGoodsAdditionalAdjustments;
    g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------
void __fastcall movementOfGoodsAdditionalAdjustments(TGridFrame *gf)
{
int poz;

  poz=gf->findPozitionColumnInGrid("PERIOD");
  if(poz!=0)
  { poz-=1;
    //Настраиваем механизм отображения периода
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add(0);
    gf->workGrid->Columns->Items[poz]->PickList->Add("предыдущий");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(1);
    gf->workGrid->Columns->Items[poz]->PickList->Add("текущий");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(2);
    gf->workGrid->Columns->Items[poz]->PickList->Add("следующий");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(3);
    gf->workGrid->Columns->Items[poz]->PickList->Add("возвращено в ТП");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(4);
    gf->workGrid->Columns->Items[poz]->PickList->Add("получено в ТП");
  }
}
//---------------------------------------------------------------------------

//*******************************************************************
// - Фрейм - Справка о товародвижении по подразделениям (почтамтам) *
//*******************************************************************
//  - фрейм - подразделения (почтамты)
AnsiString __fastcall formSD_POSelectSQL(bool groupOnPostOffices,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPostOffices==false)
  { sql+="select s.sd_id,s.sd_name,s.sd_email,s.sd_type,p.po_id,p.po_name";
    sql+=" from SUBDIVISIONS_TABLE s,POST_OFFICES_TABLE p";
    sql+=" where p.po_id=s.po_id";
  }
  else
  { sql+="select p.po_id,p.po_name";
    sql+=" from POST_OFFICES_TABLE p";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TNextGridFrame *sd_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo="";
AnsiString contentFieldsInfo="";

  tableInfo="SUBDIVISIONS_TABLE,Подразделения,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,Наименование подразделения,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_EMAIL,SD_EMAIL,Электронный адрес,Left,1,0,1,0,;";
  fieldsInfo+="s.SD_TYPE,SD_TYPE,Тип подразделения,Left,1,0,1,0,;";
  fieldsInfo+="p.PO_ID,PO_ID,Код почтамта,Center,1,0,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,Наименование почтамта,Left,1,0,1,0,";

  contentFieldsInfo ="SD_TYPE,Тип подразделения;";
  contentFieldsInfo+="SD_EMAIL,Электронный адрес;";
  contentFieldsInfo+="PO_ID,Код почтамта;";
  contentFieldsInfo+="PO_NAME,Наименование почтамта";

  try
  {
    sd_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  sd_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  sd_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //Дополнительные настройки
  sd_f->gcHeader->ColorFrom=clInactiveCaption;
  sd_f->gcHeader->ColorTo=clSkyBlue;
  sd_f->gcHeader->Font->Color=clNavy;
  sd_f->getContentFrame()->contentHeader->ColorFrom=clInactiveCaption;
  sd_f->getContentFrame()->contentHeader->ColorTo=clSkyBlue;
  sd_f->getContentFrame()->contentHeader->Font->Color=clNavy;
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
TNextGridFrame * __fastcall cteatePOFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TNextGridFrame *po_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo="";
AnsiString contentFieldsInfo="";

  tableInfo="POST_OFFICES_TABLE,Почтамты,PO_ID,PO_NAME,,";

  fieldsInfo ="p.PO_ID,PO_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,Наименование почтамта,Left,1,1,1,0,";

  try
  {
    po_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  //Дополнительные настройки
  po_f->gcHeader->ColorFrom=clInactiveCaption;
  po_f->gcHeader->ColorTo=clSkyBlue;
  po_f->gcHeader->Font->Color=clNavy;
  return po_f;
}
//---------------------------------------------------------------------------
//  - фрейм с результатами товародвижения
AnsiString __fastcall formGMForSDSelectSQL(AnsiString id,TDateTime dt1,TDateTime dt2,int period,bool forPostOffice)
{
AnsiString sql="";

  if(forPostOffice==false) //товародвижение по подразделению
  { sql="select NMP_ID,NMP_NAME,PERIOD,C1,C2,C3,C4,C5,C6,C7,C8";
    sql+=" from REFERENCE_ON_GM_FOR_SD(";
    sql+=id+",";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"',";
    sql+=IntToStr(period)+")";
  }
  else  //товародвижение по почтамту
  { sql="select NMP_ID,NMP_NAME,PERIOD,C1,C2,C3,C4,C5,C6,C7,C8";
    sql+=" from REFERENCE_ON_GN_FOR_PO(";
    sql+=id+",";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"',";
    sql+=IntToStr(period)+")";
  }
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formGMRorSDSearchSQL(AnsiString id,TDateTime dt1,TDateTime dt2,int period,bool forPostOffice)
{
AnsiString sql="";

  sql="select distinct n.NMP_ID";
  sql+=" from NMP_TABLE n,EXITS_TABLE e,FACTS_TABLE f,";
  if(forPostOffice==false)
  { sql+="SUBDIVISIONS_TABLE s";
    sql+=" where e.nmp_id=n.nmp_id and f.e_id=e.e_id and s.sd_id=f.sd_id";
    sql+=" and s.sd_id="+id;
  }
  else
  {  sql+="POST_OFFICES_TABLE p";
     sql+=" where e.nmp_id=n.nmp_id and f.e_id=e.e_id and s.sd_id=f.sd_id";
     sql+=" and s.sd_po="+id;
  }
  if(period==0)
  { sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==1)
  { sql+=" and e.e_date<'"+dt1.DateString()+"'";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==2)
  { sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
  }
  else if(period==3)
  { sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and e.date_return>'"+dt2.DateString()+"'";
  }
  return sql;
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall cteateGMForSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="FACTS_TABLE,Таблица разнаряжения ГЖП,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,Издание,Left,1,1,1,1,;";
  fieldsInfo+="PERIOD,PERIOD,Период,Center,1,1,1,0,;";
  fieldsInfo+="C1,C1,Получено|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,Получено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,Списано или возвращено|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,Списано или возвращено|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,Продано|коли- чество,Right,1,1,0,0,#0;";
  fieldsInfo+="C6,C6,Продано|сумма в ценах поставки с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,Продано|сумма в ценах реализации с НДС,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,Средняя цена реализации с НДС,Center,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#,,Right;";
  footerFieldsInfo+="C2,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C3,fvtSum,#,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->setSEdit(SEdit);
    g_f->setConditionSBox(ConditionSBox);
    g_f->additionalAdjustmentsForGrid=movementOfGoodsAdditionalAdjustments;
    g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------

//**************************************************
//* - Фрейм - Финансовые результаты по поставщикам *
//**************************************************
AnsiString __fastcall formFRForPubSelectSQL(TDateTime dt1,TDateTime dt2,int result_type)
{
AnsiString sql="";

  sql="select P_ID,P_NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10 from FR_FOR_PUBLISHERS(";
  sql+="'"+dt1.DateString()+"',";
  sql+="'"+dt2.DateString()+"',";
  sql+=IntToStr(result_type)+")";
  return sql;
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall cteateFRForPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,Финансовые результаты по поставщикам,P_ID,P_NAME,,";

  fieldsInfo ="P_ID,P_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,Издание,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,Количество|получено,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,Количество|списано (возвращено),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,Количество|потери,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,В ценах постаки (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,В ценах постаки (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,В ценах постаки (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,В ценах продажи (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,В ценах продажи (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,В ценах продажи (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,Доход (без учета потерь),Right,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#,,Right;";
  footerFieldsInfo+="C2,fvtSum,#,,Right;";
  footerFieldsInfo+="C3,fvtSum,#,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C8,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C9,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C10,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateFRForPubFrame(- Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------

//***************************************************************
//* Фрейм - Финансовые результаты по подразделениям (почтамтам) *
//***************************************************************
AnsiString __fastcall formFRForSDSelectSQL(TDateTime dt1,TDateTime dt2,bool groupOnPostOffices)
{
AnsiString sql="";

  if(groupOnPostOffices==false)
  { sql="select SD_ID,SD_NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10 from FR_FOR_SUBDIVISIONS(";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"')";
  }
  else
  { sql="select PO_ID,PO_NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,C10 from FR_FOR_POSTOFFICES(";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"')";
  }
  return sql;
}
//---------------------------------------------------------------------------
//Фрейм - по подразделениям
class TGridFrame * __fastcall createFRForSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,Финансовые результаты по подразделениям,SD_ID,SD_NAME,,";

  fieldsInfo ="SD_ID,SD_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="SD_NAME,SD_NAME,Подразделение,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,Количество|получено,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,Количество|списано (возвращено),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,Количество|потери,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,В ценах постаки (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,В ценах постаки (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,В ценах постаки (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,В ценах продажи (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,В ценах продажи (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,В ценах продажи (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,Доход (без учета потерь),Right,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#,,Right;";
  footerFieldsInfo+="C2,fvtSum,#,,Right;";
  footerFieldsInfo+="C3,fvtSum,#,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C8,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C9,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C10,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateFRForPubFrame(- Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------
//Фрейм - по почтамтам
class TGridFrame * __fastcall createFRForPOFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,Финансовые результаты по почтамтам,PO_ID,PO_NAME,,";

  fieldsInfo ="PO_ID,PO_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="PO_NAME,PO_NAME,Почтамт,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,Количество|получено,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,Количество|списано (возвращено),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,Количество|потери,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,В ценах постаки (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,В ценах постаки (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,В ценах постаки (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,В ценах продажи (без НДС)|получено,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,В ценах продажи (без НДС)|списано (возвращено),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,В ценах продажи (без НДС)|реализовано,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,Доход (без учета потерь),Right,1,1,0,0,#0.00";

  footerFieldsInfo ="C1,fvtSum,#,,Right;";
  footerFieldsInfo+="C2,fvtSum,#,,Right;";
  footerFieldsInfo+="C3,fvtSum,#,,Right;";
  footerFieldsInfo+="C4,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C5,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C6,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C7,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C8,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C9,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="C10,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateFRForPubFrame(- Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------

