//---------------------------------------------------------------------------

#pragma hdrstop
#include "RBWorkFunctions.h"
#include "ArrivalNMPFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// - ����� - ����������� ��� �� �����������
// -- ����� �����������
AnsiString __fastcall formArrivalPublishersSearchSQL(TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  sql="select distinct p.P_ID from PUBLISHERS_TABLE p,nmp_table n on,exits_table e";
  sql+=" where n.p_id=p.p_id and e.nmp_id=n.nmp_id";
  sql+=" and e.e_date between '"+dt1.DateString()+"'";
  sql+=" and '"+dt2.DateString()+"'";
//  sql+=" and (p.DEL_FLG is NULL or p.DEL_FLG=0)";
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formArrivalPublishersSelectSQL(TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  sql="select p.p_id,p.p_name,p.p_official_name,p.p_address,p.p_inn_kpp,";
  sql+="p.p_bank_requisitions,p.p_phone,p.p_email,p.INVOICE_TYPE,p.REASON,";
  sql+="sum(e.e_q) as SUM_FACT_Q,";
  sql+="sum(e.e_q*e.e_price) as SUMMA,";
  sql+="sum(e.e_q*e.e_price*(100+e.nds_stavka)/100) as VSEGO";
  sql+=" from publishers_table p,nmp_table n,exits_table e";
  sql+=" where n.p_id=p.p_id and e.nmp_id=n.nmp_id";
  sql+=" and e.e_date between '"+dt1.DateString()+"'";
  sql+=" and '"+dt2.DateString()+"'";
//  sql+=" and (p.DEL_FLG is NULL or p.DEL_FLG=0)";
  sql+=" group by p.p_id,p.p_name,p.p_official_name,p.p_address,p.p_inn_kpp,";
  sql+="p.p_bank_requisitions,p.p_phone,p.p_email,p.INVOICE_TYPE,p.REASON";
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateArrivalPublishersFtame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *p_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� ���,P_ID,P_NAME,,";

  fieldsInfo ="p.P_ID,P_ID,���,Center,1,1,1,1,;";
  fieldsInfo+="p.P_NAME,P_NAME,������������,Left,1,1,1,1,;";
  fieldsInfo+="sum(e.e_q),SUM_FACT_Q,��������|���-��,Right,1,1,0,0,;";
  fieldsInfo+="sum(e.e_q*e.e_price),SUMMA,��������|����� ��� ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="sum(e.e_q*e.e_price*(100+e.nds_stavka)/100),VSEGO,��������|����� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="p.P_OFFICIAL_NAME,P_OFFICIAL_NAME,����������� ������������,Left,1,0,0,0,;";
  fieldsInfo+="p.P_ADDRESS,P_ADDRESS,�������� �����,Left,1,0,0,0,;";
  fieldsInfo+="p.P_INN_KPP,P_INN_KPP,���/���,Left,1,0,0,0,;";
  fieldsInfo+="p.P_BANK_REQUISITIONS,P_BANK_REQUISITIONS,���������� ���������,Left,1,0,0,0,;";
  fieldsInfo+="p.P_PHONE,P_PHONE,�������,Left,1,0,0,0,;";
  fieldsInfo+="p.P_EMAIL,P_EMAIL,����������� �����,Left,1,0,0,0,;";
  fieldsInfo+="REASON,REASON,��������� ��� ������� ���������,Left,1,0,0,0,";

  contentsInfo= "P_OFFICIAL_NAME,����������� ������������;";
  contentsInfo+="P_ADDRESS,�������� �����;";
  contentsInfo+="P_INN_KPP,���/���;";
  contentsInfo+="P_BANK_REQUISITIONS,���������� ���������;";
  contentsInfo+="P_PHONE,�������;";
  contentsInfo+="P_EMAIL,����������� �����;";
  contentsInfo+="REASON,��������� ��� ������� ���������";

  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="VSEGO,fvtSum,#0.00,,Right";

  try
  {
    p_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  p_f->setEdit(SEdit);
  p_f->setConditionSBox(ConditionSBox);
  p_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  //�������������� ���������
  p_f->gcHeader->ColorFrom=Graphics::clMoneyGreen;
  p_f->gcHeader->ColorTo=clWhite;
  p_f->gcHeader->Font->Color=clTeal;
  p_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clMoneyGreen;
  p_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  p_f->getContentFrame()->contentHeader->Font->Color=clTeal;
  //��������� ����
  p_f->columnsConfig->Visible=true;
  return p_f;
}
//---------------------------------------------------------------------------
// -- ����� ���
AnsiString __fastcall formArrivalNMPSearchSQL(AnsiString pID,bool selectForParentID,bool groupOnPublishing,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  sql="select distinct n.NMP_ID,e.E_PRICE as nmp_price";
  if(groupOnPublishing==false) sql+=",e.E_ID";
  sql+=" from NMP_TABLE n,exits_table e";
  sql+=" where e.nmp_id=n.nmp_id";
  sql+=" and e.e_date between '"+dt1.DateString()+"'";
  sql+=" and '"+dt2.DateString()+"'";
  if(selectForParentID==true && !pID.IsEmpty()) sql+=" and n.p_id="+pID;
//  sql+=" and(n.DEL_FLG is NULL or n.DEL_FLG=0)";
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formArrivalNMPSelectSQL(AnsiString pID,bool selectForPublisher,bool groupOnPublishing,TDateTime dt1,TDateTime dt2)
{
AnsiString sql;

  if(groupOnPublishing==false)
  { sql+="select distinct n.nmp_id,n.p_id,n.nmp_name||' �'||e.e_number as NMP_NAME,";
    sql+="e.e_id,e.e_number,e.e_price as nmp_price,e.nds_stavka,n.nmp_type,";
    sql+="n.invoice_type,n.ret_must_flg,n.SALE_TIME,p.P_NAME,";
    sql+="e.e_q as sum_fact_q,";
    sql+="e.e_q*e.e_price as summa,";
    sql+="e.e_q*e.e_price*(100+e.nds_stavka)/100 as vsego,";
    sql+="e.e_q*e.e_price*(100+e.nds_stavka)/100-e.e_q*e.e_price as summa_nds";
    sql+=" from exits_table e,nmp_table n,publishers_table p";
    sql+=" where n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(selectForPublisher==true && !pID.IsEmpty()) sql+=" and n.p_id="+pID;
//    sql+=" and(n.DEL_FLG is NULL or n.DEL_FLG=0)";
  }
  else
  { sql+="select n.nmp_id,n.p_id,n.nmp_name,";
    sql+="e.e_price as nmp_price,e.nds_stavka,n.nmp_type,n.invoice_type,";
    sql+="n.ret_must_flg,n.SALE_TIME,p.P_NAME,";
    sql+="sum(e.e_q) as sum_fact_q,";
    sql+="sum(e.e_q*e.e_price) as summa,";
    sql+="sum(e.e_q*e.e_price*(100+e.nds_stavka)/100) as vsego,";
    sql+="sum(e.e_q*e.e_price*(100+e.nds_stavka)/100)-sum(e.e_q*e.e_price) as summa_nds";
    sql+=" from exits_table e,nmp_table n,publishers_table p";
    sql+=" where n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    if(selectForPublisher==true && !pID.IsEmpty()) sql+=" and n.p_id="+pID;
//    sql+=" and(n.DEL_FLG is NULL or n.DEL_FLG=0)";
    sql+=" group by n.nmp_id,n.p_id,n.nmp_name,e.e_price,e.nds_stavka,n.nmp_type,";
    sql+="n.invoice_type,n.ret_must_flg,n.SALE_TIME,p.p_name";
  }
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createArrivalNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox,bool groupOnPublishing)
{
TNextGridFrame *nmp_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="NMP_TABLE,�������-���������� ���������,NMP_ID,NMP_NAME,,";


  if(groupOnPublishing==false)
  { fieldsInfo ="n.NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
    fieldsInfo+="n.NMP_NAME||' �'||e.E_NUMBER,NMP_NAME,������������,Left,1,1,1,1,;";
    fieldsInfo+="e.e_id,e_id,��� �������,Left,1,0,0,0,;";
    fieldsInfo+="e.e_q,SUM_FACT_Q,��������,Right,1,1,0,0,;";
    fieldsInfo+="e.e_price,NMP_PRICE,����,Right,1,1,0,0,#0.00;";
    fieldsInfo+="e.e_Q*e.e_price,SUMMA,����� ��� ���,Right,1,0,0,0,#0.00;";
    fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,0,0,;";
    fieldsInfo+="e.e_Q*e.e_price*(100+e.nds_stavka/100-e.e_Q*e.e_price,SUMMA_NDS,����� ���,Right,1,0,0,0,#0.00;";
    fieldsInfo+="e.e_Q*e.e_price*(100+e.nds_stavka)/100,VSEGO,����� � ���,Right,1,1,0,0,#0.00;";
    fieldsInfo+="n.NMP_TYPE,NMP_TYPE,��� ���,Center,1,0,0,0,;";
    fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,�������� ��������,Center,1,0,0,0,;";
    fieldsInfo+="n.SALE_TIME,SALE_TIME,���� ����������,Center,1,0,0,0,;";
    fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,��� ���������,Center,1,0,0,0,;";
    fieldsInfo+="n.P_ID,P_ID,��� ����������,Center,1,0,0,0,;";
    fieldsInfo+="p.P_NAME,P_NAME,������������ ����������,Left,1,0,0,0,";
  }
  else
  { fieldsInfo ="n.NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
    fieldsInfo+="n.NMP_NAME,NMP_NAME,������������,Left,1,1,1,1,;";
    fieldsInfo+="sum(e.e_q),SUM_FACT_Q,��������,Right,1,1,0,0,;";
    fieldsInfo+="e.e_price,NMP_PRICE,����,Right,1,1,0,0,#0.00;";
    fieldsInfo+="sum(e.e_Q*e.e_price),SUMMA,����� ��� ���,Right,1,0,0,0,#0.00;";
    fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,0,0,;";
    fieldsInfo+="sum(e.e_Q*e.e_price)*(100+e.nds_stavka/100)-sum(e.e_Q*e.e_price),SUMMA_NDS,����� ���,Right,1,0,0,0,#0.00;";
    fieldsInfo+="sum(e.e_Q*e.e_price*(100+e.nds_stavka)/100),VSEGO,����� � ���,Right,1,1,0,0,#0.00;";
    fieldsInfo+="n.NMP_TYPE,NMP_TYPE,��� ���,Center,1,0,0,0,;";
    fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,�������� ��������,Center,1,0,0,0,;";
    fieldsInfo+="n.SALE_TIME,SALE_TIME,���� ����������,Center,1,0,0,0,;";
    fieldsInfo+="n.INVOICE_TYPE,INVOICE_TYPE,��� ���������,Center,1,0,0,0,;";
    fieldsInfo+="n.P_ID,P_ID,��� ����������,Center,1,0,0,0,;";
    fieldsInfo+="p.P_NAME,P_NAME,������������ ����������,Left,1,0,0,0,";
  }

  contentsInfo= "P_ID,��� ����������;";
  contentsInfo+="P_NAME,������������ ����������;";
  contentsInfo+="NMP_TYPE,��� ���;";
  contentsInfo+="RET_MUST_FLG,�������� ��������,";
  contentsInfo+="SALE_TIME,���� ����������;";
  contentsInfo+="INVOICE_TYPE,��� ���������";

  footerFieldsInfo ="SUM_FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_NDS,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="VSEGO,fvtSum,#0.00,,Right";

  try
  {
    nmp_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createNMPFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  nmp_f->setEdit(SEdit);
  nmp_f->setConditionSBox(ConditionSBox);
  nmp_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  nmp_f->gcHeader->ColorFrom=Graphics::clMoneyGreen;
  nmp_f->gcHeader->ColorTo=clWhite;
  nmp_f->gcHeader->Font->Color=clTeal;
  nmp_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clMoneyGreen;
  nmp_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  nmp_f->getContentFrame()->contentHeader->Font->Color=clTeal;
  //�������������� ���������
  nmp_f->getGridFrame()->additionalAdjustmentsForGrid=nmpAdditionalAdjustments;
  nmpAdditionalAdjustments(nmp_f->getGCFrame()->getGridFrame());
  //��������� ����
  nmp_f->columnsConfig->Visible=true;
  nmp_f->additional1->Caption="�������� �� ��������������";
  nmp_f->additional1->Visible=true;
  return nmp_f;
}
//---------------------------------------------------------------------------
