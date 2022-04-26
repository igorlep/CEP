//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include "RBWorkFunctions.h"
#include "DifferentFunctions.h"
#include "Folder.h"
#include "DirectWorkExcel.h"
#include "SomeFunctions.h"
#include "MessageParametersForm.h"
#include "ShipmentNMPFunctions.h"
#include "ReturnNMPfromSDF.h"
#include "ReturnNMPfromSDFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// - ����� - ������� ��� �� �������������
// -- ����� �������������
AnsiString __fastcall formReturnSubdivisionsSelectSQL(bool groupOnPostOffices,bool showAllRecords,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPostOffices==false)
  { sql="select s.sd_id,s.sd_name,s.sd_email,s.sd_type,p.po_id,p.po_name,";
    sql+="sum(f.RETURN_Q) as SUM_RETURN_Q,";
    sql+="sum(f.RETURN_Q*e.e_price) as SUM_RETURN";
    sql+=" from facts_table f,exits_table e,subdivisions_table s,";
    sql+="post_offices_table p";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and p.po_id=s.po_id";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(showAllRecords==false) sql+=" and f.return_q <> 0";
    sql+=" group by s.sd_id,s.sd_name,s.sd_email,s.sd_type,p.po_id,p.po_name";
  }
  else
  { sql="select p.po_id,p.po_name,";
    sql+="sum(f.RETURN_Q) as SUM_RETURN_Q,";
    sql+="sum(f.RETURN_Q*e.e_price) as SUM_RETURN";
    sql+=" from facts_table f,exits_table e,subdivisions_table s,";
    sql+="post_offices_table p";
    sql+=" where e.e_id=f.e_id and s.sd_id=f.sd_id and p.po_id=s.po_id";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(showAllRecords==false) sql+=" and f.return_q <> 0";
    sql+=" group by p.po_id,p.po_name";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateReturnSubdivisionsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *sd_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="SUBDIVISIONS_TABLE,�������������,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,������������ �������������,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_EMAIL,SD_EMAIL,����������� �����,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_TYPE,SD_TYPE,��� �������������,Left,1,0,1,0,;";
  fieldsInfo+="p.PO_ID,PO_ID,��� ��������,Center,1,0,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,������������ ��������,Left,1,0,1,0,;";
//  fieldsInfo+="SUM_FACT_Q,SUM_FACT_Q,�����- ������,Right,1,1,0,0,#;";
  fieldsInfo+="sum(f.RETURN_Q),SUM_RETURN_Q,����������|���-��,Right,1,1,0,0,#;";
  fieldsInfo+="sum(f.RETURN_Q*e.e_price),SUM_RETURN,����������|����� ��� ���,Right,1,1,0,0,#0.00";

  contentFieldsInfo ="SD_TYPE,��� �������������;";
  contentFieldsInfo+="PO_ID,��� ��������;";
  contentFieldsInfo+="PO_NAME,������������ ��������";

//  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo ="SUM_RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUM_RETURN,fvtSum,#0.00,,Right";

  try
  {
    sd_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateReturnSubdivisionsFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  sd_f->setEdit(SEdit);
  sd_f->setConditionSBox(ConditionSBox);
  sd_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  sd_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //�������������� ���������
  sd_f->gcHeader->ColorFrom=Graphics::clCream;
  sd_f->gcHeader->ColorTo=clWhite;
  sd_f->gcHeader->Font->Color=clMaroon;
  sd_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clCream;
  sd_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  sd_f->getContentFrame()->contentHeader->Font->Color=clMaroon;
  sd_f->getGridFrame()->additionalAdjustmentsForGrid=subdivisionsAdditionalAdjustments;
  subdivisionsAdditionalAdjustments(sd_f->getGridFrame());
  //��������� ����
  sd_f->columnsConfig->Visible=true;
  return sd_f;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateReturnPostOfficesFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *po_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="POST_OFFICES_TABLE,��������,PO_ID,PO_NAME,,";

  fieldsInfo ="PO_ID,PO_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="PO_NAME,PO_NAME,������������ ��������,Left,1,1,1,0,;";
  fieldsInfo+="SUM_RETURN_Q,SUM_RETURN_Q,����������|���-��,Right,1,1,0,0,#;";
  fieldsInfo+="SUM_RETURN,SUM_RETURN,����������|����� ��� ���,Right,1,1,0,0,#0.00";

//  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo ="SUM_RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUM_RETURN,fvtSum,#0.00,,Right";

  try
  {
    po_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateReturnPostOfficesFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  po_f->setEdit(SEdit);
  po_f->setConditionSBox(ConditionSBox);
  //�������������� ���������
  po_f->gcHeader->ColorFrom=Graphics::clCream;
  po_f->gcHeader->ColorTo=clWhite;
  po_f->gcHeader->Font->Color=clMaroon;
  return po_f;
}
//---------------------------------------------------------------------------
// -- ����� ������������ �� ������������� ���
AnsiString __fastcall formReturnNMPfromSDSearchSQL(AnsiString sdID,bool showAllRecords,bool groupOnPublishers,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPublishers==false)
  { sql="select distinct n.NMP_ID,n.nmp_name||', �'||e.e_number as nmp_name";
    sql+=" from facts_table f";
    sql+=" left outer join exits_table e on e.e_id=f.e_id";
    sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
    sql+=" left outer join publishers_table p on p.p_id=n.p_id";
    sql+=" where e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and f.sd_id="+sdID;
    if(showAllRecords==false) sql+=" and f.return_q <> 0";
    sql+=" and(n.DEL_FLG is NULL or n.DEL_FLG=0)";
  }
  else
  { sql="select distinct p.P_ID";
    sql+=" from PUBLISHERS_TABLE p";
    sql+=" left outer join nmp_table n on n.p_id=p.p_id";
    sql+=" left outer join exits_table e on e.nmp_id=n.nmp_id";
    sql+=" left outer join facts_table f on f.e_id=e.e_id";
    sql+=" where e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and f.sd_id="+sdID;
    if(showAllRecords==false) sql+=" and f.return_q <> 0";
    sql+=" and(p.DEL_FLG is NULL or p.DEL_FLG=0)";
  }
  return sql;

}
//---------------------------------------------------------------------------
AnsiString __fastcall formReturnNMPfromSDSelectSQL(AnsiString sdID,bool groupOnPostOffices,bool groupOnPublishers,bool showAllRecords,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(sdID==NULL || sdID.IsEmpty()) return "";
  if(groupOnPublishers==false && groupOnPostOffices==false)
  { sql="select p.p_id,p.p_name,n.nmp_id,n.nmp_name||', �'||e.e_number as nmp_name,";
    sql+="e.e_id,e.e_price,e.nds_stavka,n.invoice_type,f.fact_id,e.e_date as FACT_DATE,";
    sql+="f.fact_q,n.sale_time,e.r_date as RETURN_DATE,f.return_q,";
    sql+="f.real_price,";
    sql+="f.return_q*e.e_price as return_summa,";
    sql+="case when f.return_q <> 0 and f.fact_q <> 0 then floor(f.return_q*1000/f.fact_q+0.5)/10";
    sql+=" else NULL end as return_percent";
    sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p";
    sql+=" where f.sd_id="+sdID;
    sql+="and e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(showAllRecords==false) sql+=" and f.return_q <> 0";
  }
  else if(groupOnPublishers==false && groupOnPostOffices==true)
  { sql="select p.p_id,p.p_name,n.nmp_id,n.nmp_name||', �'||e.e_number as nmp_name,";
    sql+="e.e_id,e.e_price,e.nds_stavka,n.invoice_type,e.e_date as FACT_DATE,n.sale_time,";
    sql+="sum(f.fact_q) as fact_q,";
    sql+="sum(f.return_q) as return_q,";
    sql+="sum(f.return_q*e.e_price) as return_summa,";
    sql+="case when sum(f.return_q) <> 0 and sum(f.fact_q) <> 0 then floor(sum(f.return_q)*1000/sum(f.fact_q)+0.5)/10";
    sql+=" else NULL end as return_percent";
    sql+=" from facts_table f,subdivisions_table s,post_offices_table po,";
    sql+="exits_table e,nmp_table n,publishers_table p";
    sql+=" where s.sd_id=f.sd_id and po.po_id=s.po_id";
    sql+=" and e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and po.po_id="+sdID;
    if(showAllRecords==false) sql+=" and f.return_q > 0";
    sql+=" group by p.p_id,p.p_name,n.nmp_id,n.nmp_name,e.e_number,e.e_id,";
    sql+="e.e_price,e.nds_stavka,n.invoice_type,e.e_date,n.sale_time";
//    sql+=" having sum(f.fact_q) is not null and sum(f.fact_q) <> 0";
  }
  else if(groupOnPublishers==true && groupOnPostOffices==false)
  { sql="select p.p_id,p.p_name,";
    sql+="sum(f.FACT_Q) as fact_q,";
    sql+="sum(f.RETURN_Q) as return_q,";
    sql+="sum(f.RETURN_Q*e.e_price) as return_summa";
    sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p";
    sql+=" where e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and f.sd_id="+sdID;
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(showAllRecords==false) sql+=" and f.return_q > 0";
    sql+=" group by p.p_id,p.p_name";
//    sql+=" having sum(f.fact_q) is not null and sum(f.fact_q) <> 0";
  }
  else if(groupOnPublishers==true && groupOnPostOffices==true)
  { sql="select p.p_id,p.p_name,";
    sql+="sum(f.FACT_Q) as fact_q,";
    sql+="sum(f.RETURN_Q) as return_q,";
    sql+="sum(f.RETURN_Q*e.e_price) as return_summa";
    sql+=" from facts_table f,subdivisions_table s,";
    sql+=" exits_table e,nmp_table n,publishers_table p";
    sql+=" where s.sd_id=f.sd_id and e.e_id=f.e_id and n.nmp_id=e.nmp_id";
    sql+=" and p.p_id=n.p_id and s.po_id="+sdID;
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(showAllRecords==false) sql+=" and f.return_q > 0";
    sql+=" group by p.p_id,p.p_name";
//    sql+=" having sum(f.fact_q) is not null and sum(f.fact_q) <> 0";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createReturnNMP_SDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *nmp_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="FACTS_TABLE,�������-���������� ���������,FACT_ID,NMP_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,��� ����������,Center,1,0,0,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,������������ ����������,Left,1,0,0,0,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,��� �������,Center,1,1,1,1,;";
  fieldsInfo+="n.nmp_name||' �'||e.e_number,NMP_NAME,�������,Left,1,1,1,1,;";
  fieldsInfo+="e.E_ID,E_ID, ��� �������,Center,1,0,0,0,#0.00;";
  fieldsInfo+="e.E_PRICE,E_PRICE,���� ��� ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,1,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,��� ���������,Center,1,0,1,0,;";
  fieldsInfo+="f.FACT_ID,FACT_ID,�����������|���,Center,1,0,0,0,;";
  fieldsInfo+="e.E_DATE,FACT_DATE,�����������|����,Center,1,1,0,0,;";
  fieldsInfo+="f.FACT_Q,FACT_Q,�����������|����- ������,Right,1,1,0,0,;";
  fieldsInfo+="n.SALE_TIME,SALE_TIME,������ ���������� � ����,Center,1,1,1,0,;";
  fieldsInfo+="e.R_DATE,RETURN_DATE,���������� �� �����|����,Center,1,1,1,0,;";
  fieldsInfo+="f.RETURN_Q,RETURN_Q,���������� �� �����|����- ������,Right,0,1,1,0,;";
  fieldsInfo+="f.RETURN_Q*e.E_PRICE,RETURN_SUMMA,���������� �� �����|�����,Right,1,0,0,0,#0.00;";
  fieldsInfo+="f.REAL_PRICE,REAL_PRICE,���� ���������� � ���,Right,0,1,1,0,#0.00;";
  fieldsInfo+="f.RETURN_Q/f.FACT_Q,RETURN_PERCENT,������� ��������,Right,1,1,0,0,#0.00";

  contentsInfo= "P_ID,��� ����������;";
  contentsInfo+="P_NAME,������������ ����������;";
  contentsInfo+="INVOICE_TYPE,��� ���������";

  footerFieldsInfo ="FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_SUMMA,fvtSum,#0.00,,Right";

  try
  {
    nmp_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createReturnNMPfromSDFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  nmp_f->setEdit(SEdit);
  nmp_f->setConditionSBox(ConditionSBox);
  nmp_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  nmp_f->gcHeader->ColorFrom=Graphics::clCream;
  nmp_f->gcHeader->ColorTo=clWhite;
  nmp_f->gcHeader->Font->Color=clMaroon;
  nmp_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clCream;
  nmp_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  nmp_f->getContentFrame()->contentHeader->Font->Color=clMaroon;
  //�������������� ���������
  nmp_f->getGridFrame()->additionalAdjustmentsForGrid=returnNMPAdditionalAdjustments;
  returnNMPAdditionalAdjustments(nmp_f->getGridFrame());
  //��������� ����
  nmp_f->columnsConfig->Visible=true;
  return nmp_f;
}
//---------------------------------------------------------------------------
void __fastcall returnNMPAdditionalAdjustments(TGridFrame *gf)
{
int poz;

//  poz=gf->findPozitionColumnInGrid("RETURN_DATE");
//  if(poz>0)
//  { poz-=1;
//    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
//  }
  poz=gf->findPozitionColumnInGrid("RETURN_Q");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
  poz=gf->findPozitionColumnInGrid("REAL_PRICE");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
  poz=gf->findPozitionColumnInGrid("INVOICE_TYPE");
  if(poz!=0)
  { poz-=1;
    //����������� �������� ����������� ���� ���������
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add("�");
    gf->workGrid->Columns->Items[poz]->PickList->Add("����-12");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("�");
    gf->workGrid->Columns->Items[poz]->PickList->Add("����-12�");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("�");
    gf->workGrid->Columns->Items[poz]->PickList->Add("����-2");
  }
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createReturnNMP_POFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *nmp_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="NMP_TABLE,�������-���������� ���������,NMP_ID,NMP_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,��� ����������,Center,1,0,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,������������ ����������,Left,1,0,1,1,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,��� �������,Center,1,1,1,1,;";
  fieldsInfo+="n.nmp_name||' �'||e.e_number,NMP_NAME,�������,Left,1,1,1,1,;";
  fieldsInfo+="e.E_PRICE,E_PRICE,���� ��� ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,0,0,;";
  fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,��� ���������,Center,1,0,1,0,;";
  fieldsInfo+="e.E_DATE,FACT_DATE,�����������|����,Center,1,1,0,0,;";
  fieldsInfo+="sum(f.FACT_Q),FACT_Q,�����������|����������,Right,1,1,0,0,;";
  fieldsInfo+="n.SALE_TIME,SALE_TIME,������ ���������� � ����,Center,1,1,1,0,;";
  fieldsInfo+="sum(f.RETURN_Q),RETURN_Q,���������� �� �����|����������,Right,1,1,1,0,;";
  fieldsInfo+="sum(f.RETURN_Q*e.E_PRICE),RETURN_SUMMA,���������� �� �����|�����,Right,1,1,0,0,#0.00;";
  fieldsInfo+="sum(f.RETURN_Q)/sum(f.FACT_Q),RETURN_PERCENT,������� ��������,Right,1,1,0,0,#0.00";

  contentsInfo= "P_ID,��� ����������;";
  contentsInfo+="P_NAME,������������ ����������;";
  contentsInfo+="INVOICE_TYPE,��� ���������";

  footerFieldsInfo ="FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_SUMMA,fvtSum,#0.00,,Right";

  try
  {
    nmp_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createReturnNMPfromSDFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  nmp_f->setEdit(SEdit);
  nmp_f->setConditionSBox(ConditionSBox);
  nmp_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  nmp_f->gcHeader->ColorFrom=Graphics::clCream;
  nmp_f->gcHeader->ColorTo=clWhite;
  nmp_f->gcHeader->Font->Color=clMaroon;
  nmp_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clCream;
  nmp_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  nmp_f->getContentFrame()->contentHeader->Font->Color=clMaroon;
  //�������������� ���������
//  nmp_f->getGridFrame()->additionalAdjustmentsForGrid=returnNMPAdditionalAdjustments;
//  returnNMPAdditionalAdjustments(nmp_f->getGridFrame());
  //��������� ����
  nmp_f->columnsConfig->Visible=true;
  return nmp_f;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createReturnPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *p_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo="";
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� ���,P_ID,P_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,��� ����������,Center,1,1,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,������������ ����������,Left,1,1,1,1,;";
  fieldsInfo+="sum(f.FACT_Q),FACT_Q,�����������,Right,1,1,0,0,;";
  fieldsInfo+="sum(f.RETURN_Q),RETURN_Q,����������,Right,1,1,0,0,;";
  fieldsInfo+="sum(f.RETURN_Q*e.e_price),RETURN_SUMMA,����� ��� ���,Right,1,1,0,0,#0.00";

  footerFieldsInfo ="FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="RETURN_SUMMA,fvtSum,#0.00,,Right";

  try
  {
    p_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createReturnPFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  p_f->setEdit(SEdit);
  p_f->setConditionSBox(ConditionSBox);
  p_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  p_f->gcHeader->ColorFrom=Graphics::clCream;
  p_f->gcHeader->ColorTo=clWhite;
  p_f->gcHeader->Font->Color=clMaroon;
  return p_f;
}
//---------------------------------------------------------------------------

//������������ ��������� � ��������� �� ��������
// - �������� ������� ������������ ������� �� �������� ���
void createTemplateMessages(TComponent *owner,AnsiString iniFileName,int kind)
{
TMessageParametersForm *MPF=NULL;
TStringList *sl=NULL;
TDateTime date1, date2;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  date1=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
  sl=new TStringList;
  if(kind==1) //������ ������
  { sl->Add(SetHeader("�������� �������-���������� ���������",date1,date2));
    MPF->setSubject("������ �� �������� �������-���������� ���������");
  }
  else //���������� ������
  { sl->Add(SetHeader("�������������� �������� �������-���������� ���������",date1,date2));
    MPF->setSubject("������������� ������ �� �������� �������-���������� ���������");
  }
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { AnsiString subject=MPF->subjectEdit->Text,
               content=MPF->contentMemo->Text;
    bool replyFLG=MPF->replyCheckbox->Checked,
         delFilesFLG=MPF->delFilesCheckbox->Checked;
    (static_cast<TReturnNMPfromSDFrame*>(owner))->Repaint();
    AnsiString dirName=getAppDir()+"Out\\ToRaions\\";
    TProgressBar *sdBar,*nmpBar;
    FBDatabase *fbDB;
    AnsiString monthFolderID;
    TDBGridEh *sdGrid;
    TpFIBDataSet *sdDS;

    fbDB=(static_cast<TReturnNMPfromSDFrame*>(owner))->getFBDataBase();
    //������� ������ ��������
    unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(y)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(m)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    //��������� ������� ������ ����� � ������� MESSAGES_FOLDERS_TABLE
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �������������","������� �� �������� ���",y,m);
    if(monthFolderID==NULL) goto end;

    sdBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->sdBar;
    nmpBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->nmpBar;
    sdBar->Visible=true;
    nmpBar->Visible=true;

    sdGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getLGrid();
    sdDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->getLDS();

    if(sdGrid->SelectedRows->Count==0) sdGrid->Selection->Rows->CurrentRowSelected=true;
    bool resultFLG=true;
    if(kind==1) //������ ������
    { sdBar->Max=sdGrid->SelectedRows->Count;
      sdBar->Position=0;
      for(int i=0; i<sdGrid->SelectedRows->Count; ++i)
      { sdDS->GotoBookmark((void *)sdGrid->SelectedRows->Items[i].c_str());
        AnsiString sdName=sdDS->FieldByName("SD_NAME")->AsString;
        AnsiString sdMail=sdDS->FieldByName("SD_EMAIL")->AsString;
        //1.��������� Excel ����� � ������ ��������
        AnsiString fileOut=createTemplateFile(owner,kind,dirName);
        if(fileOut==NULL || fileOut.IsEmpty())
        { resultFLG=false;
          break;
        }
        //2.������� ��������� ��� �������� � ������������� � ������������� ������
        TStringList *fileNames;
        fileNames=new TStringList;
        if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
        addMessageToBase(owner,fbDB,monthFolderID,sdName,sdMail,subject,content,fileNames,replyFLG,delFilesFLG);
        delete fileNames;
        sdBar->StepIt();
      }
    }
    else  //��������� � ���������� ��������
    { sdBar->Max=1;
      sdBar->Position=0;
      AnsiString sdName=sdDS->FieldByName("SD_NAME")->AsString;
      AnsiString sdMail=sdDS->FieldByName("SD_EMAIL")->AsString;
      //1.��������� Excel ����� ���������� ��������
      AnsiString fileOut=createTemplateFile(owner,kind,dirName);
      if(!fileOut.IsEmpty())
      { //2.������� ��������� ��� �������� � ������������� � ������������� ������
        TStringList *fileNames;
        fileNames=new TStringList;
        if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
        addMessageToBase(owner,fbDB,monthFolderID,sdName,sdMail,subject,content,fileNames,replyFLG,delFilesFLG);
        delete fileNames;
      }
      else resultFLG=false;
      sdBar->StepIt();
    }
    sdBar->Position=0;
    sdBar->Visible=false;
    nmpBar->Visible=false;
    AnsiString str;
    if(resultFLG==true)
    { Folder *f;
      try
      { f=new Folder(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE");
        str="��������� ������������ � ��������� � ����� \""+f->getFolderFullName(monthFolderID)+"\".";
        delete f;
      }
      catch(...)
      {
        str="��������� ������������";
      }
    }
    else str="��� ������������ ��������� �������� ������";
    MessageDlg(str,mtInformation,TMsgDlgButtons() << mbOK,0);
  }
end:
  if(sl!=NULL) delete sl;
  if(MPF!=NULL) delete MPF;
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � ��������
AnsiString __fastcall createTemplateFile(TComponent *owner,int kind,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn=getAppDir()+"Out\\ToRaions\\Templates\\";
unsigned short y,m,d;
DirectWorkExcel *de=NULL;
TDateTime date1,date2;
FBDatabase *fbDB=getFBDataBase();
AnsiString ricipientID;
AnsiString ricipientName;
TpFIBDataSet *rDS;
TDBGridEh *rGrid;
FBQuery *fbRQ=NULL;
TProgressBar *nmpBar;
AnsiString period;
int sq=0,  //����� ������������� ���������
    npp=1, //���������� ����� ������
    ns=6;  //����� ������ ������ ��������� �����

  //������� ��������������� �����������
  try
  {
    fbRQ=newFBQuery(owner,"createTemplateFile",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("createTemplateFile: ������ �������� ������� FBQuery"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(fbRQ!=NULL) delete fbRQ;
    return "";
  }
  //���������� ��������
  date1=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
  period=getMonthYearString(date2);
  rDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRDS();
  rGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRGrid();
  nmpBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->nmpBar;
  ricipientID=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientID();
  ricipientName=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientName();
  date2.DecodeDate(&y,&m,&d);
  fileOut+="rt_"+AnsiString().sprintf("%d-%02d_%02d",y,m,StrToInt(ricipientID))+".xls";
  if(kind==1) fileIn+="Temp_1_n.xls";
  else fileIn+="Temp_1_s_n.xls";


  //�������� ������
  if(templateCopy(fileIn,fileOut)== false) return "";

  //������� Excel ����
  try
  {
    de=new DirectWorkExcel(fileOut,1);
  }
  catch(const Exception &exc)
  { MessageDlg("������ �������� ������� DirectWorkExcel\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(fbRQ!=NULL) delete fbRQ;
    if(de!=NULL) delete de;
    return "";
  }
  if(de->openFile(false)==false)
  { MessageDlg("������ �������� Excel �����",mtWarning,TMsgDlgButtons() << mbOK,0);
    if(fbRQ!=NULL) delete fbRQ;
    if(de!=NULL) delete de;
    return "";
  }
  de->setSheetNumber(1);

  //������� ������ � Excel ����
  //  ���������
  de->setCellValue(3,"A",ricipientID);
  de->setCellValue(3,"B",ricipientName);
  de->setCellValue(3,"G",period);
  //  ������
  if(kind==1)   // ������ ������
  { nmpBar->Max=rDS->RecordCount;
    nmpBar->Position=0;
    rDS->First();
    while(!rDS->Eof)
    { if(putStr(ns,npp,ricipientID,date1,de,rDS,fbRQ)==false) break;
      npp++;
      sq+=rDS->FieldByName("FACT_Q")->AsInteger;
      rDS->Next();
      nmpBar->StepIt();
    }
  }
  else          // ���������� ������
  { if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
    nmpBar->Max=rGrid->SelectedRows->Count;
    nmpBar->Position=0;
    for(int i=0; i<rGrid->SelectedRows->Count; ++i)
    { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
      if(putStr(ns,npp,ricipientID,date1,de,rDS,fbRQ)==false) break;
      npp++;
      sq+=rDS->FieldByName("FACT_Q")->AsInteger;
      nmpBar->StepIt();
    }
  }
  nmpBar->Position=0;
  // ������
  de->setMergeCells("A"+IntToStr(ns+npp),"E"+IntToStr(ns+npp));
  de->setAlignment("A"+IntToStr(ns+npp),"E"+IntToStr(ns+npp),"RIGHT","CENTER",0);
  de->setCellValue(ns+npp,"A","�����:");
  de->setCellValue(ns+npp,"F",IntToStr(sq));
  de->setCellValue(ns+npp,"G","=����(G7:G"+IntToStr(ns+npp-1)+")");
  de->setCellValue(ns+npp,"I","=����(I7:I"+IntToStr(ns+npp-1)+")");
  de->setBorderCells("A7","L"+IntToStr(ns+npp));
  de->setColorCells("A7","F"+IntToStr(ns+npp),15);
  de->setColorCells("I7","L"+IntToStr(ns+npp),15);
  TFont *f;
  f=new TFont();
  de->getFontCells("A"+IntToStr(ns),"A"+IntToStr(ns),f);
  f->Style=TFontStyles()<<fsBold;
  de->setFontCells("A"+IntToStr(ns+npp),"I"+IntToStr(ns+npp),f);
  delete f;
  de->setWrapText("B"+IntToStr(ns),"B"+IntToStr(ns+npp), true);
  de->setSheetLocked("MAR",true);

  //��������� �������
  if(fbRQ!=NULL) delete fbRQ;
  if(de!=NULL)
  { de->closeFile();
    delete de;
  }
  return fileOut;
}
//---------------------------------------------------------------------------
// - ������� ������ ������ � Excel ���� � ��������
bool putStr(int ns,int npp,AnsiString sdID,TDateTime date,class DirectWorkExcel *de, TDataSet *ds, FBQuery *fbRQ)
{
AnsiString s="";

  de->setCellValue(ns+npp,"A",IntToStr(npp));
  de->setCellValue(ns+npp,"B",ds->FieldByName("NMP_NAME")->AsString);
  de->setCellValue(ns+npp,"C",ds->FieldByName("E_PRICE")->AsString);
  de->setCellValue(ns+npp,"D",ds->FieldByName("FACT_ID")->AsString);
  de->setCellValue(ns+npp,"E",ds->FieldByName("FACT_DATE")->AsString);
  de->setCellValue(ns+npp,"F",ds->FieldByName("FACT_Q")->AsString);
  if(ds->FieldByName("RETURN_Q")->AsInteger > 0)
    de->setCellValue(ns+npp,"G",ds->FieldByName("RETURN_Q")->AsString);

  //���� ���� ���������� ����������� � �� ����� ��������� ����
  if(fabs(ds->FieldByName("REAL_PRICE")->AsFloat) > 0.00001 && fabs(ds->FieldByName("E_PRICE")->AsFloat-ds->FieldByName("REAL_PRICE")->AsFloat) > 0.00001)
    s=AnsiString().sprintf("%.2lf",ds->FieldByName("REAL_PRICE")->AsFloat);
  else //����� ���� ���� ���������� � ���������� ��������
  { TpFIBQuery *rq=fbRQ->getQuery();
    AnsiString sql;
    rq->Close();
    rq->SQL->Clear();
    sql="select REAL_PRICE";
    sql+=" from GET_REAL_PRICE(";
    sql+=sdID+",";
    sql+=ds->FieldByName("NMP_ID")->AsString+",";
    sql+="'"+date.DateString()+"')";
    if(fbRQ->execQuery("createTemplateFile",sql)==false) return false;
    if(!rq->FieldByName("REAL_PRICE")->IsNull)
      s=AnsiString().sprintf("%.2lf",rq->FieldByName("REAL_PRICE")->AsFloat);
  }
  if(!s.IsEmpty())
  { de->setCellValue(ns+npp,"H",s);
    de->setCellValue(ns+npp,"L",s);
  }
  de->setCellValue(ns+npp,"I","=����(G"+IntToStr(ns+npp)+"<>0;C"+IntToStr(ns+npp)+"*G"+IntToStr(ns+npp)+";\" \")");
  de->setCellValue(ns+npp,"J","=����(F"+IntToStr(ns+npp)+" < G"+IntToStr(ns+npp)+";\"!\";\" \")");
  de->setCellValue(6+npp,"K","=����(H"+IntToStr(ns+npp)+" <> L"+IntToStr(ns+npp)+";\"!\";\" \")");
  return true;
}
//---------------------------------------------------------------------------

