//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include "DBClasses.h"
#include "SomeFunctions.h"
#include "GridContentF.h"
#include "frxClass.hpp" 
#include "frxDBSet.hpp" 
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
#include "SpisanieNMPF.h"
#include "SpisanieNMPFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvProgressBar"
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//Фрейм со списываемой газетно-журнальной продукцией
// - функция формирования select sql запроса
AnsiString __fastcall formNMPSelectSQL(bool selectForPublisher,AnsiString pID,bool delDistrRecords,int stavkaNDS,TDateTime date1,TDateTime date2)
{
AnsiString sql;

  sql="select n.nmp_name||' №'||e.e_number as name_nmp,e.e_price,";
  sql+="n.invoice_type,e.r_date as return_date,e.e_q,e.e_date,";
  sql+="(select REMOVE_PERCENT from GET_REMOVE_PERCENT(n.nmp_id,'"+date1.DateString()+"')) as remove_percent,";
  sql+="cast(floor(e.e_q*(select REMOVE_PERCENT from GET_REMOVE_PERCENT(n.nmp_id,'"+date1.DateString()+"'))+0.5)/100 as integer) as may_be_return,";
  sql+="e.r_q as return_q,e.q_return,e.date_return,e.nds_stavka,";
  sql+="n.p_id,n.nmp_id,e.e_id,n.ret_must_flg,";
  sql+="e.q_lost,e.lost_flg,";
  sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,";
  sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,";
  sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds,";
  sql+=" cast(e.q_return as float)/e.e_q*100 as real_rp";
  sql+=" from exits_table e,nmp_table n";
  sql+=" where n.nmp_id=e.nmp_id";
//  sql+=" and (e.r_date between '"+date1.DateString()+"'";
//  sql+=" and '"+date2.DateString()+"'";
//  sql+=" or e.date_return='"+date2.DateString()+"')";
  sql+=" and e.date_return='"+date2.DateString()+"'";
  if(selectForPublisher==true) sql+=" and n.p_id="+pID;
  if(stavkaNDS>=0) sql+=" and e.NDS_STAVKA="+IntToStr(stavkaNDS);
  if(delDistrRecords==true)
    sql+=" and((e.q_lost is not null and e.q_lost<>0) or e.q_return < cast(floor(e.e_q*(select REMOVE_PERCENT from GET_REMOVE_PERCENT(n.nmp_id,'"+date1.DateString()+"'))+0.5)/100 as integer))";
  sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
//  sql+=" group by n.nmp_name,e.e_number,e.e_price,n.stavka_nds,n.invoice_type,";
//  sql+="r.remove_percent,e.e_q,e.e_date,e.r_date,e.q_return,e.date_return,";
//  sql+="e.nds_stavka,n.p_id,n.nmp_id,e.e_id,n.ret_must_flg,e.q_lost,e.lost_flg";
  return sql;
}
//---------------------------------------------------------------------------
// - функция формирования search sql запроса
AnsiString __fastcall formNMPSearchSQL(bool selectForPublisher,AnsiString pID,bool delDistrRecords,int stavkaNDS,TDateTime date1,TDateTime date2)
{
AnsiString sql;

  sql= "select distinct n.nmp_name||' №'||e.e_number as name_nmp";
  sql+=" from exits_table e";
  sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
//  sql+=" where (e.r_date between '"+date1.DateString()+"'";
//  sql+=" and '"+date2.DateString()+"'";
//  sql+=" or e.date_return='"+date2.DateString()+"')";
  sql+=" where e.date_return='"+date2.DateString()+"'";
  if(selectForPublisher==true) sql+=" and n.p_id="+pID;
  if(stavkaNDS>=0) sql+=" and e.NDS_SATAVKA="+IntToStr(stavkaNDS);
  if(delDistrRecords==true)
    sql+=" and e.q_return is NULL or e.q_return < cast(floor(e.e_q*r.remove_percent+0.5)/100 as integer)";
  sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
  return sql;
}
//---------------------------------------------------------------------------
// - функция создания фрейма с возвращаемой ГЖП
TGridContentFrame * __fastcall createSpisanieNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridContentFrame *gc_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="EXITS_TABLE,Подлежащая спианию газетно-журнальная продукция,E_ID,NAME_NMP,,";

  fieldsInfo ="e.E_ID,E_ID,Код выхода,Center,1,0,0,0,;";
  fieldsInfo+="n.nmp_name||' №'||e.e_number,NAME_NMP,Наименование,Left,1,1,1,1,;";
  fieldsInfo+="e.E_PRICE,E_PRICE,Цена без НДС,Center,1,1,1,0,;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,Ставка НДС,Center,1,1,1,0,;";
  fieldsInfo+="e.R_DATE,RETURN_DATE,Дата возврата на склад,Center,1,1,1,0,;";
  fieldsInfo+="REMOVE_PERCENT,REMOVE_PERCENT,Допустимый процент списания,Center,1,1,0,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,Тип накладной,Center,1,0,1,0,;";
  fieldsInfo+="e.E_Q,E_Q,Количество|разна- ряжено,Right,1,1,1,0,#;";
  fieldsInfo+="MAY_BE_RETURN,MAY_BE_RETURN,Количество|может быть списано,Right,1,1,0,0,#;";
  fieldsInfo+="e.R_Q,RETURN_Q,Количество|на складе,Right,1,1,1,0,#;";
  fieldsInfo+="e.Q_RETURN,Q_RETURN,Количество|списать,Right,0,1,1,0,#;";
  fieldsInfo+="e.Q_LOST,Q_LOST,Количество|потери,Right,0,1,1,0,#;";
  fieldsInfo+="e.q_return/e.e_q*100,REAL_RP,Списано в %,Center,1,1,0,0,#0.0;";
  fieldsInfo+="e.LOST_FLG,LOST_FLG,Флаг потерь,Right,0,0,0,0,#;";
  fieldsInfo+="e.E_DATE,E_DATE,Дата разнаряжения,Right,1,0,0,0,#;";
  fieldsInfo+="e.DATE_RETURN,DATE_RETURN,Дата возврата поставщику,Center,0,0,0,0,;";
  fieldsInfo+="n.P_ID,P_ID,Код поставщика,Center,1,0,0,0,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,Код издания,Center,1,0,0,0,;";
  fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,Подлежит возврату,Center,1,0,0,0,;";
  fieldsInfo+="floor(e.q_return*e.e_price*100++0.5)/100,SUMMA,Списано|без учете НДС,Right,1,0,0,0,#0.00;";
  fieldsInfo+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100,SUMMA_NDS,Списано|сумма НДС,Right,1,0,0,0,#0.00;";
  fieldsInfo+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100,SUMMA_S_NDS,Списано|с учетом НДС,Right,1,0,0,0,#0.00";

  contentsInfo= "INVOICE_TYPE,Тип накладной;";
  contentsInfo+="E_DATE,Дата разнаряжения;";
  contentsInfo+="DATE_RETURN,Дата списания;";
  contentsInfo+="P_ID,Код поставщика;";
  contentsInfo+="NMP_ID,Код издания;";
  contentsInfo+="E_ID,Код выхода";

  footerFieldsInfo= "E_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="MAY_BE_RETURN,fvtSum,#,,Right;";
  footerFieldsInfo+="Q_RETURN,fvtSum,#,,Right;";
  footerFieldsInfo+="Q_LOST,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_NDS,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    gc_f=new TGridContentFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gc_f!=NULL)
  { gc_f->Parent=workPanel;
    gc_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
    gc_f->getGridFrame()->additionalAdjustmentsForGrid=spisanieNMPAdditionalAdjustments;
    spisanieNMPAdditionalAdjustments(gc_f->getGridFrame());
  }
  return gc_f;
}
//---------------------------------------------------------------------------
void __fastcall spisanieNMPAdditionalAdjustments(TGridFrame *gf)
{
int poz;

  poz=gf->findPozitionColumnInGrid("Q_RETURN");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
  poz=gf->findPozitionColumnInGrid("Q_LOST");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
  poz=gf->findPozitionColumnInGrid("INVOICE_TYPE");
  if(poz!=0)
  { poz-=1;
    //Настраиваем механизм отображения типа накладной
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add("н");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Торг-12");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("м");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Торг-12м");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("а");
    gf->workGrid->Columns->Items[poz]->PickList->Add("Торг-2");
  }
}
//---------------------------------------------------------------------------

