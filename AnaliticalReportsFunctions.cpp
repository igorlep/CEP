//---------------------------------------------------------------------------
//����� ������� � ������� ���� "������������� ������"
#include <math.h>
#pragma hdrstop

#include "GridF.h"
#include "NextGridF.h"
#include "AnaliticalReportsFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// ����� - ������ �� ���������� ���
// - ������������ select SQL
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

  tableInfo="EXITS_TABLE,������� ������� ���,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,�������,Left,1,1,1,1,;";
  fieldsInfo+="C1,C1,��������|����- ������,Right,1,1,0,0,#;";
  fieldsInfo+="C2,C2,��������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,������� � ����������|����- ������,Right,1,1,0,0,#;";
  fieldsInfo+="C4,C4,������� � ����������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,�����������|����- ������,Right,1,1,0,0,#;";
  fieldsInfo+="C6,C6,�����������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,�����������|����� � ����� ���������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,������� �� ������|����- ������,Right,1,1,0,0,#;";
  fieldsInfo+="C9,C9,������� �� ������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,���� � ���|��������,Center,1,1,0,0,#0.00;";
  fieldsInfo+="C11,C11,���� � ���|����������,Center,1,1,0,0,#0.00;";
  fieldsInfo+="C12,C12,�������|(���.),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C13,C13,�������|%,Center,1,1,0,0,#0.0;";
  fieldsInfo+="C14,C14,����� (��� ����� �������) (���.),Right,1,1,0,0,#0.00";

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
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
// - ����� - ������� � �������������� (�� �����������) *
//******************************************************
//  - ����� � ������������ ��������������
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

  tableInfo="EXITS_TABLE,������� ������� ���,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,�������,Left,1,1,1,1,;";
  fieldsInfo+="PERIOD,PERIOD,������,Center,1,1,1,0,;";
  fieldsInfo+="C1,C1,��������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,��������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,������� ��� ����������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,������� ��� ����������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,�������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C6,C6,�������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,�������|����� � ����� ���������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,������� ���� ���������� � ���,Center,1,1,0,0,#0.00";

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
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
    //����������� �������� ����������� �������
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add(0);
    gf->workGrid->Columns->Items[poz]->PickList->Add("����������");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(1);
    gf->workGrid->Columns->Items[poz]->PickList->Add("�������");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(2);
    gf->workGrid->Columns->Items[poz]->PickList->Add("���������");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(3);
    gf->workGrid->Columns->Items[poz]->PickList->Add("���������� � ��");
    gf->workGrid->Columns->Items[poz]->KeyList->Add(4);
    gf->workGrid->Columns->Items[poz]->PickList->Add("�������� � ��");
  }
}
//---------------------------------------------------------------------------

//*******************************************************************
// - ����� - ������� � �������������� �� �������������� (���������) *
//*******************************************************************
//  - ����� - ������������� (��������)
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

  tableInfo="SUBDIVISIONS_TABLE,�������������,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,������������ �������������,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_EMAIL,SD_EMAIL,����������� �����,Left,1,0,1,0,;";
  fieldsInfo+="s.SD_TYPE,SD_TYPE,��� �������������,Left,1,0,1,0,;";
  fieldsInfo+="p.PO_ID,PO_ID,��� ��������,Center,1,0,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,������������ ��������,Left,1,0,1,0,";

  contentFieldsInfo ="SD_TYPE,��� �������������;";
  contentFieldsInfo+="SD_EMAIL,����������� �����;";
  contentFieldsInfo+="PO_ID,��� ��������;";
  contentFieldsInfo+="PO_NAME,������������ ��������";

  try
  {
    sd_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  sd_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  sd_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //�������������� ���������
  sd_f->gcHeader->ColorFrom=clInactiveCaption;
  sd_f->gcHeader->ColorTo=clSkyBlue;
  sd_f->gcHeader->Font->Color=clNavy;
  sd_f->getContentFrame()->contentHeader->ColorFrom=clInactiveCaption;
  sd_f->getContentFrame()->contentHeader->ColorTo=clSkyBlue;
  sd_f->getContentFrame()->contentHeader->Font->Color=clNavy;
  sd_f->getGridFrame()->additionalAdjustmentsForGrid=subdivisionsAdditionalAdjustments;
  subdivisionsAdditionalAdjustments(sd_f->getGridFrame());
  //��������� ����
  sd_f->columnsConfig->Visible=true;
  return sd_f;
}
//---------------------------------------------------------------------------
void __fastcall subdivisionsAdditionalAdjustments(TGridFrame *gf)
{
int poz=gf->findPozitionColumnInGrid("SD_TYPE");

  if(poz!=0)
  { poz-=1;
    //����������� �������� ����������� ���� �������������
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add("1");
    gf->workGrid->Columns->Items[poz]->PickList->Add("�������");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("2");
    gf->workGrid->Columns->Items[poz]->PickList->Add("��������� �����");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("3");
    gf->workGrid->Columns->Items[poz]->PickList->Add("�����");
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

  tableInfo="POST_OFFICES_TABLE,��������,PO_ID,PO_NAME,,";

  fieldsInfo ="p.PO_ID,PO_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="p.PO_NAME,PO_NAME,������������ ��������,Left,1,1,1,0,";

  try
  {
    po_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateTempPublishersFtame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  //�������������� ���������
  po_f->gcHeader->ColorFrom=clInactiveCaption;
  po_f->gcHeader->ColorTo=clSkyBlue;
  po_f->gcHeader->Font->Color=clNavy;
  return po_f;
}
//---------------------------------------------------------------------------
//  - ����� � ������������ ��������������
AnsiString __fastcall formGMForSDSelectSQL(AnsiString id,TDateTime dt1,TDateTime dt2,int period,bool forPostOffice)
{
AnsiString sql="";

  if(forPostOffice==false) //�������������� �� �������������
  { sql="select NMP_ID,NMP_NAME,PERIOD,C1,C2,C3,C4,C5,C6,C7,C8";
    sql+=" from REFERENCE_ON_GM_FOR_SD(";
    sql+=id+",";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"',";
    sql+=IntToStr(period)+")";
  }
  else  //�������������� �� ��������
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

  tableInfo="FACTS_TABLE,������� ������������ ���,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,�������,Left,1,1,1,1,;";
  fieldsInfo+="PERIOD,PERIOD,������,Center,1,1,1,0,;";
  fieldsInfo+="C1,C1,��������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,��������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C3,C3,������� ��� ����������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,������� ��� ����������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,�������|����- ������,Right,1,1,0,0,#0;";
  fieldsInfo+="C6,C6,�������|����� � ����� �������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,�������|����� � ����� ���������� � ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,������� ���� ���������� � ���,Center,1,1,0,0,#0.00";

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
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
//* - ����� - ���������� ���������� �� ����������� *
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

  tableInfo="PUBLISHERS_TABLE,���������� ���������� �� �����������,P_ID,P_NAME,,";

  fieldsInfo ="P_ID,P_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,�������,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,����������|��������,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,����������|������� (����������),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,����������|������,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,����� (��� ����� ������),Right,1,1,0,0,#0.00";

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
  { MessageDlg("cteateFRForPubFrame(- ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
//* ����� - ���������� ���������� �� �������������� (���������) *
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
//����� - �� ��������������
class TGridFrame * __fastcall createFRForSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� ���������� �� ��������������,SD_ID,SD_NAME,,";

  fieldsInfo ="SD_ID,SD_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="SD_NAME,SD_NAME,�������������,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,����������|��������,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,����������|������� (����������),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,����������|������,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,����� (��� ����� ������),Right,1,1,0,0,#0.00";

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
  { MessageDlg("cteateFRForPubFrame(- ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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
//����� - �� ���������
class TGridFrame * __fastcall createFRForPOFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� ���������� �� ���������,PO_ID,PO_NAME,,";

  fieldsInfo ="PO_ID,PO_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="PO_NAME,PO_NAME,�������,Left,1,1,1,0,;";
  fieldsInfo+="C1,C1,����������|��������,Right,1,1,0,0,#0;";
  fieldsInfo+="C2,C2,����������|������� (����������),Right,1,1,0,0,#0;";
  fieldsInfo+="C3,C3,����������|������,Right,1,1,0,0,#0;";
  fieldsInfo+="C4,C4,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C5,C5,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C6,C6,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C7,C7,� ����� ������� (��� ���)|��������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C8,C8,� ����� ������� (��� ���)|������� (����������),Right,1,1,0,0,#0.00;";
  fieldsInfo+="C9,C9,� ����� ������� (��� ���)|�����������,Right,1,1,0,0,#0.00;";
  fieldsInfo+="C10,C10,����� (��� ����� ������),Right,1,1,0,0,#0.00";

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
  { MessageDlg("cteateFRForPubFrame(- ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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

