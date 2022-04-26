//---------------------------------------------------------------------------

#include "RBWorkFunctions.h"
#pragma hdrstop
#include "AddEditTemplate.h"
#include "MultiSelectSD.h"
#include "TWFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//*************************************
//* Фрейм для редактирования шаблонов *
//*************************************
// -- фрейм поставщиков
AnsiString __fastcall formTempPublishersSearchSQL(bool viewDelRecords,bool selectToPeriod,TDateTime tempDate)
{
AnsiString sql;
bool whereFLG=false;

  sql="select distinct p.P_ID from PUBLISHERS_TABLE p";
  if(selectToPeriod==true)
  { sql+=" left outer join nmp_table n on n.p_id=p.p_id";
    sql+=" left outer join template_table t on t.nmp_id=n.nmp_id";
    sql+=" where t.temp_date='"+tempDate.DateString()+"'";
    whereFLG=true;
  }
  if(viewDelRecords==false)
  { if(whereFLG==false) sql+=" where ";
    else sql+=" and ";
    sql+="(p.DEL_FLG is NULL or p.DEL_FLG=0)";
  }
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formTempPublishersSelectSQL(bool veiwDelRecords,bool selectToPeriod,TDateTime tempDate)
{
AnsiString sql;
bool whereFLG=false;

  sql="select distinct p.* from PUBLISHERS_TABLE p";
  if(selectToPeriod==true)
  { sql+=" left outer join nmp_table n on n.p_id=p.p_id";
    sql+=" left outer join template_table t on t.nmp_id=n.nmp_id";
    sql+=" where t.temp_date='"+tempDate.DateString()+"'";
    whereFLG=true;
  }
  if(veiwDelRecords==false)
  { if(whereFLG==false) sql+=" where ";
    else sql+=" and ";
    sql+="(p.DEL_FLG is NULL or p.DEL_FLG=0)";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createTempPublishersFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *p_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;

  tableInfo="PUBLISHERS_TABLE,Поставщики ГЖП,P_ID,P_NAME,,";

  fieldsInfo= "p.DEL_FLG,DEL_FLG,,Center,1,1,1,0,;";
  fieldsInfo+="p.P_ID,P_ID,Код,Center,1,1,1,1,;";
  fieldsInfo+="p.P_NAME,P_NAME,Наименование,Left,1,1,1,1,;";
  fieldsInfo+="p.P_OFFICIAL_NAME,P_OFFICIAL_NAME,Официальное наименование,Left,1,0,0,0,;";
  fieldsInfo+="p.P_ADDRESS,P_ADDRESS,Почтовый адрес,Left,1,0,0,0,;";
  fieldsInfo+="p.P_INN_KPP,P_INN_KPP,ИНН/КПП,Left,1,0,0,0,;";
  fieldsInfo+="p.P_BANK_REQUISITIONS,P_BANK_REQUISITIONS,Банковские реквизиты,Left,1,0,0,0,;";
  fieldsInfo+="p.P_PHONE,P_PHONE,Телефон,Left,1,0,0,0,;";
  fieldsInfo+="p.P_EMAIL,P_EMAIL,Электронный адрес,Left,1,0,0,0,;";
//  fieldsInfo+="p.INVOICE_TYPE,INVOICE_TYPE,Тип накладной,Center,1,1,1,0,;";
  fieldsInfo+="REASON,REASON,Основание для выписки накладной,Left,1,0,0,0,";

  contentsInfo= "P_OFFICIAL_NAME,Официальное наименование;";
  contentsInfo+="P_ADDRESS,Почтовый адрес;";
  contentsInfo+="P_INN_KPP,ИНН/КПП;";
  contentsInfo+="P_BANK_REQUISITIONS,Банковские реквизиты;";
  contentsInfo+="P_PHONE,Телефон;";
  contentsInfo+="P_EMAIL,Электронный адрес;";
  contentsInfo+="REASON,Основание для выписки накладной";

  try
  {
    p_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  p_f->setEdit(SEdit);
  p_f->setConditionSBox(ConditionSBox);
  p_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  //Дополнительные настройки
//  p_f->getGridFrame()->additionalAdjustmentsForGrid=publishersAdditionalAdjustments;
//  publishersAdditionalAdjustments(p_f->getGCFrame()->getGridFrame());
  //Настройка меню
  p_f->columnsConfig->Visible=true;
  return p_f;
}
//---------------------------------------------------------------------------
// -- фрейм ГЖП
AnsiString __fastcall formTempNMPSearchSQL(AnsiString pID,bool selectForPublisher,bool viewDelRecords,bool selectToPeriod,TDateTime tempDate)
{
AnsiString sql;
bool whereFLG=false;

  sql="select distinct n.NMP_ID from NMP_TABLE n";
  if(selectToPeriod==true)
  { sql+=" left outer join template_table t on t.nmp_id=n.nmp_id";
    sql+=" where t.temp_date='"+tempDate.DateString()+"'";
    whereFLG=true;
  }
  if(viewDelRecords==false)
  { if(whereFLG==false){ sql+=" where "; whereFLG=true; }
    else sql+=" and ";
    sql+="(n.DEL_FLG is NULL or n.DEL_FLG=0)";
  }
  if(selectForPublisher==true && pID!=NULL && !pID.IsEmpty())
  { if(whereFLG==false) sql+=" where ";
    else sql+=" and ";
    sql+="n.P_ID="+pID;
  }
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formTempNMPSelectSQL(AnsiString pID,bool selectForPublisher,bool viewDelRecords,bool selectToPeriod,TDateTime tempDate)
{
AnsiString sql;
bool whereFLG=false;

  sql="select distinct n.*,p.P_NAME,r.REMOVE_PERCENT from NMP_TABLE n";
  sql+=" left outer join PUBLISHERS_TABLE p on p.P_ID=n.P_ID";
  sql+=" left outer join REMOVE_PERCENT_TABLE r on r.NMP_ID=n.NMP_ID and r.RP_DATE=(select max(rp_date) from remove_percent_table where nmp_id=n.nmp_id and rp_date<='"+tempDate.DateString()+"')";
  if(selectToPeriod==true)
  { sql+=" left outer join template_table t on t.nmp_id=n.nmp_id";
    sql+=" where t.temp_date='"+tempDate.DateString()+"'";
    whereFLG=true;
  }
  if(viewDelRecords==false)
  { if(whereFLG==false){ sql+=" where "; whereFLG=true; }
    else sql+=" and ";
    sql+="(n.DEL_FLG is NULL or n.DEL_FLG=0)";
  }
  if(selectForPublisher==true && pID!=NULL && !pID.IsEmpty())
  { if(whereFLG==false){ sql+=" where "; whereFLG=true; }
    else sql+=" and ";
    sql+="n.P_ID="+pID;
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createTempNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *nmp_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;

  tableInfo="NMP_TABLE,Газетно-журнальная продукция,NMP_ID,NMP_NAME,,";

  fieldsInfo= "n.DEL_FLG,DEL_FLG,,Center,1,1,1,0,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,Код,Center,1,1,1,1,;";
  fieldsInfo+="n.NMP_NAME,NMP_NAME,Наименование,Left,1,1,1,1,;";
  fieldsInfo+="n.NMP_PRICE,NMP_PRICE,Цена ГЖП,Right,1,0,0,0,#0.00;";
  fieldsInfo+="n.STAVKA_NDS,STAVKA_NDS,Ставка НДС,Center,1,0,0,0,;";
  fieldsInfo+="n.NMP_TYPE,NMP_TYPE,Вид ГЖП,Center,1,1,1,0,;";
  fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,Подлежит возврату,Center,1,0,0,0,;";
  fieldsInfo+="n.SALE_TIME,SALE_TIME,Срок реализации,Center,1,0,0,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,Тип накладной,Center,1,0,0,0,;";
  fieldsInfo+="n.P_ID,P_ID,Код поставщика,Center,1,0,0,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,Наименование поставщика,Left,1,0,0,0,;";
  fieldsInfo+="r.REMOVE_PERCENT,REMOVE_PERCENT,Процент списания,Center,1,1,1,0,";

  contentsInfo= "P_ID,Код поставщика;";
  contentsInfo+="P_NAME,Наименование поставщика;";
  contentsInfo+="NMP_PRICE,Цена ГЖП;";
  contentsInfo+="STAVKA_NDS,Ставка НДС;";
  contentsInfo+="INVOICE_TYPE,Тип накладной";

  try
  {
    nmp_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  nmp_f->setEdit(SEdit);
  nmp_f->setConditionSBox(ConditionSBox);
  nmp_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  //Дополнительные настройки
  nmp_f->getGCFrame()->getGridFrame()->additionalAdjustmentsForGrid=nmpAdditionalAdjustments;
  nmpAdditionalAdjustments(nmp_f->getGCFrame()->getGridFrame());
  //Настройка меню
  nmp_f->columnsConfig->Visible=true;
  return nmp_f;
}
//---------------------------------------------------------------------------
// -- фрейм шаблонов
bool __fastcall CanDelTemplate(TComponent* owner,FBDatabase *fbDB,AnsiString id,AnsiString &str)
{
  return true;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formTempSelectSQL(TDateTime tempDate,TDateTime oldTempDate,TDateTime fDate1,TDateTime fDate2,AnsiString nmpID)
{
AnsiString sql="";

  sql="select T_ID,TEMP_Q,OLD_TEMP_Q,SD_NAME,CALC_Q from GET_FROM_TEMPLATE_TABLE(";
  sql+="'"+tempDate.DateString()+"',";
  sql+="'"+oldTempDate.DateString()+"',";
  sql+="'"+fDate1.DateString()+"',";
  sql+="'"+fDate2.DateString()+"',";
  sql+=nmpID+")";

  return sql;
}
//---------------------------------------------------------------------------
TGridFrame * __fastcall createTempFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *t_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="TEMPLATE_TABLE,Шаблоны для разнаряжения ГЖП,T_ID,T_ID,,";

  fieldsInfo= "T_ID,T_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="SD_NAME,SD_NAME,Подразделение,Left,1,1,1,0,;";
  fieldsInfo+="TEMP_Q,TEMP_Q,Количество за период|текущий,Center,0,1,1,0,;";
  fieldsInfo+="OLD_TEMP_Q,OLD_TEMP_Q,Количество за период|преды- дущий,Center,1,1,1,0,;";
  fieldsInfo+="CALC_Q,CALC_Q,Расчет,Center,1,1,1,0,";

  footerFieldsInfo= "TEMP_Q,fvtSum,#,,Center;";
  footerFieldsInfo+="OLD_TEMP_Q,fvtSum,#,,Center;";
  footerFieldsInfo+="CALC_Q,fvtSum,#,,Center";

  try
  {
    t_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(t_f!=NULL)
  { AnsiString sql;
    t_f->initFrame();
    t_f->getWorkDataSet()->SQLs->InsertSQL->Clear();
    t_f->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    sql="delete from TEMPLATE_TABLE where T_ID=?OLD_T_ID";
    t_f->getWorkDataSet()->SQLs->DeleteSQL->Add(sql);
    t_f->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    sql="update TEMPLATE_TABLE set TEMP_Q=?TEMP_Q where T_ID=?OLD_T_ID";
    t_f->getWorkDataSet()->SQLs->UpdateSQL->Add(sql);
    t_f->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    t_f->Parent=workPanel;
    t_f->setColumnsConfigFieldEditInGridVisible(false);
    t_f->setColumnsConfigFieldSearchAbleVisible(false);
  }
  return t_f;
}
//---------------------------------------------------------------------------
int __fastcall AddEditTemplate(TComponent* Owner,AnsiString iniFN,FBDatabase *fbDB,AnsiString &t_id,AnsiString nmp_id,AnsiString sd_id,TDateTime tempDate)
{
TAddEditTemplateForm *AET;
int modalResult=mrCancel;

  try
  {
    AET=new TAddEditTemplateForm(Owner,iniFN,fbDB,t_id,nmp_id,sd_id,tempDate);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания формы TAddEditTemplateForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return modalResult;
  }
  modalResult=AET->ShowModal();
  delete AET;
  return modalResult;
}
//---------------------------------------------------------------------------
TMultiSelectSDForm * __fastcall createMultiSelectSDForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,AnsiString pTN,AnsiString pid,TDateTime dt)
{
TMultiSelectSDForm *msForm;

  try
  {
    msForm=new TMultiSelectSDForm(Owner,iniFN,iniSN,fbDB,pTN,pid,dt);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TMultiSelectSDForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }

  AnsiString tableInfo="SUBDIVISIOMS_TABLE,Подразделения,SD_ID,SD_NAME,,";
  AnsiString sql;
  sql="select s.SD_ID,s.SD_NAME from SUBDIVISIONS_TABLE s where not exists";
  if(pTN=="TEMPLATE_TABLE")
  { sql+="(select SD_ID from template_table where NMP_ID="+pid;
    sql+=" and TEMP_DATE='"+dt.DateString()+"'";
    sql+=" and sd_id=s.sd_id) and (s.DEL_FLG is NULL or s.DEL_FLG=0)";
  }
  else if(pTN=="FACTS_TABLE")
  { sql+="(select SD_ID from facts_table f where f.E_ID="+pid;
    sql+=" and sd_id=s.sd_id) and (s.DEL_FLG is NULL or s.DEL_FLG=0)";
  }
  sql+=" order by s.SD_NAME";
  AnsiString
    fieldsInfo= "s.SD_ID,SD_ID,Код,Center,1,1,1,0;";
    fieldsInfo+="s.SD_NAME,SD_NAME,Подразделение,Left,1,1,1,0";
  TGridFrame *gf;
  try
  {
    gf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gf!=NULL)
  { gf->initFrame();
    gf->Parent=msForm->workPanel;
    gf->setStatusBar(msForm->getStatusBar());
    gf->initSelectSQL(sql);
    gf->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    gf->getWorkDataSet()->SQLs->InsertSQL->Clear();
    gf->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    gf->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    gf->loadDS();
    msForm->setGridFrame(gf);
  }
  return msForm;
}
//---------------------------------------------------------------------------
