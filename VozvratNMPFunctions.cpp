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
#include "VozvratNMPF.h"
#include "VozvratNMPFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvProgressBar"
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//Фрейм со списываемой газетно-журнальной продукцией
// - функция формирования select sql запроса
AnsiString __fastcall formVNMPSelectSQL(bool selectForPublisher,AnsiString pID,TDateTime date1,TDateTime date2)
{
AnsiString sql;

  sql ="select n.nmp_name||' №'||e.e_number as name_nmp,e.e_price,";
  sql+="n.invoice_type,e.r_date as return_date,e.e_q,";
  sql+="e.r_q as return_q,e.q_return,e.date_return,e.nds_stavka,";
  sql+="n.p_id,n.nmp_id,e.e_id,n.ret_must_flg,";
  sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,"; //сумма без учета НДС
  sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,"; //сумма с НДС
  sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds"; //сумма НДС
  sql+=" from exits_table e";
  sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
  sql+=" where e.r_date>='"+date1.DateString()+"'";
  sql+=" and e.r_date<='"+date2.DateString()+"'";
  if(selectForPublisher==true) sql+=" and n.p_id="+pID;
  sql+=" and n.ret_must_flg=1";
//  sql+=" group by n.nmp_name,e.e_number,e.e_price,n.stavka_nds,n.invoice_type,";
//  sql+="e.r_date,e.e_q,e.q_return,e.date_return,e.nds_stavka,n.p_id,";
//  sql+="n.nmp_id,e.e_id,n.ret_must_flg";
  return sql;
}
//---------------------------------------------------------------------------
// - функция формирования search sql запроса
AnsiString __fastcall formVNMPSearchSQL(bool selectForPublisher,AnsiString pID,TDateTime date1,TDateTime date2)
{
AnsiString sql;

  sql= "select distinct n.nmp_name||' №'||e.e_number as name_nmp";
  sql+=" from exits_table e";
  sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
  sql+=" where e.r_date>='"+date1.DateString()+"'";
  sql+=" and e.r_date<='"+date2.DateString()+"'";
  if(selectForPublisher==true) sql+=" and n.p_id="+pID;
  sql+=" and n.ret_must_flg=1";
  return sql;
}
//---------------------------------------------------------------------------
// - функция создания фрейма с возвращаемой ГЖП
TGridFrame * __fastcall createVozvratNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="EXITS_TABLE,Подлежащая спианию газетно-журнальная продукция,E_ID,NMP_NAME,,";

  fieldsInfo= "n.nmp_name||' №'||e.e_number,NAME_NMP,Наименование,Left,1,1,1,1,;";
  fieldsInfo+="e.E_PRICE,E_PRICE,Цена без НДС,Center,1,1,1,0,;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,Стака НДС,Center,1,1,1,0,;";
  fieldsInfo+="e.R_DATE,RETURN_DATE,Дата возврата,Center,1,1,1,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,Тип накладной,Center,1,1,1,0,;";
  fieldsInfo+="e.E_Q,E_Q,Количество|разна- ряжено,Right,1,1,1,0,#;";
  fieldsInfo+="e.R_Q,RETURN_Q,Количество|на складе,Right,1,1,1,0,#;";
  fieldsInfo+="e.Q_RETURN,Q_RETURN,Количество|возвратить,Right,0,1,1,0,#;";
  fieldsInfo+="e.DATE_RETURN,DATE_RETURN,Дата возврата поставщику,Center,0,0,0,0,;";
  fieldsInfo+="n.P_ID,P_ID,Код поставщика,Center,1,0,0,0,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,Код издания,Center,1,0,0,0,;";
  fieldsInfo+="e.E_ID,E_ID,Код выхода,Center,1,0,0,0,;";
  fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,Подлежит возврату,Center,1,0,0,0,;";
  fieldsInfo+="floor(e.e_price*(100+e.nds_stavka)+0.5)/100,SUMMA,Сумма без учета НДС,Right,1,0,0,0,#0.00;";
  fieldsInfo+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100,SUMMA_NDS,Сумма НДС,Right,1,0,0,0,#0.00;";
  fieldsInfo+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100,SUMMA_S_NDS,Сумма с НДС,Right,1,0,0,0,#0.00";

  footerFieldsInfo= "E_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="Q_RETURN,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_NDS,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
    g_f->additionalAdjustmentsForGrid=vozvratNMPAdditionalAdjustments;
    vozvratNMPAdditionalAdjustments(g_f);
  }
  return g_f;
}
//---------------------------------------------------------------------------
void __fastcall vozvratNMPAdditionalAdjustments(TGridFrame *gf)
{
int poz;

  poz=gf->findPozitionColumnInGrid("Q_RETURN");
  if(poz!=0)
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

