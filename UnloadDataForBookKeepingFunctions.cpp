//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include <ADODB.hpp>
#include "DifferentFunctions.h"
#include "DirectWorkExcel.h"
#include "SomeFunctions.h"
#include "GridF.h"
#include "Folder.h"
#include "MessageParametersForm.h"
#include "RBWorkFunctions.h"
#include "UnloadDataForBookKeepingF.h"
#include "UnloadDataForBookKeepingFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// ����� - �������� ������ ��� ����������� ����
// - ������������ select SQL
AnsiString __fastcall formUnloadDataForBookSelectSQL(int type,TDateTime dt1,TDateTime dt2)
{
AnsiString sql="";

  if(type==1) //����������� ��� �� ����������
  { sql="select P_ID,P_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA,SUMMA_S_NDS";
    sql+=" from ARRIVAL_NMP_FROM_PUB(";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"')";
/*
    sql="select p.p_id,p.p_name,'��� �� ������ ��� '||e.nds_stavka||'%' as NMP_NAME,";
    sql+="e.nds_stavka,sum(e.e_q) as quant,sum(e.e_q*e.e_price) as summa,";
    sql+="floor(sum(e.e_q*e.e_price*(100+e.nds_stavka))+0.5)/100 as summa_s_nds";
    sql+=" from EXITS_TABLE e,nmp_table n,publishers_table p";
    sql+=" where n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" group by p.p_id,p.p_name,e.nds_stavka";
*/
  }
  else if(type==2) //�������� ��� � ��������
  { sql="select PO_ID,PO_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA";
    sql+=" from SHIPMENT_NMP_TO_SD(";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"')";
/*
    sql="select po.po_id,po.po_name,'��� �� ������ ��� '||e.nds_stavka||'%' as NMP_NAME,";
    sql+="e.nds_stavka,sum(f.fact_q) as quant,sum(f.fact_q*e.e_price) as summa";
    sql+=" from facts_table f,subdivisions_table s,post_offices_table po,";
    sql+="exits_table e,nmp_table n";
    sql+=" where s.sd_id=f.sd_id and po.po_id=s.po_id and e.e_id=f.e_id";
    sql+=" and n.nmp_id=e.nmp_id";
    sql+=" and e.e_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" group by po.po_id,po.po_name,e.nds_stavka";
*/
  }
  else if(type==3) //������� ��� �� ���������
  { sql="select PO_ID,PO_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA";
    sql+=" from RETURNS_FROM_SD_FOR_BOOK(";
    sql+="'"+dt1.DateString()+"',";
    sql+="'"+dt2.DateString()+"')";
/*
    sql="select po.po_id,po.po_name,'��� �� ������ ��� '||e.nds_stavka||'%' as NMP_NAME,";
    sql+="e.nds_stavka,sum(f.return_q) as quant,sum(f.return_q*e.e_price) as summa";
    sql+=" from facts_table f,subdivisions_table s,post_offices_table po,";
    sql+="exits_table e,nmp_table n";
    sql+=" where s.sd_id=f.sd_id and po.po_id=s.po_id and e.e_id=f.e_id";
    sql+=" and n.nmp_id=e.nmp_id";
    sql+=" and e.r_date between '"+dt1.DateString()+"'";
    sql+=" and '"+dt2.DateString()+"'";
    sql+=" and f.return_q is not null";
    sql+=" group by po.po_id,po.po_name,e.nds_stavka";
*/
  }
  else if(type==4) //������� ��� ����������
  { sql="select P_ID,P_NAME,INVOICE_TYPE,NMP_NAME,NDS_STAVKA,QUANT,SUMMA,SUMMA_S_NDS";
    sql+=" from RETURNS_TO_PUB_FOR_BOOK(";
    sql+="'"+dt2.DateString()+"')";
/*
    sql="select p.p_id,p.p_name,n.INVOICE_TYPE,'��� �� ������ ��� '||e.nds_stavka||'%' as NMP_NAME,";
    sql+="e.nds_stavka,sum(e.q_return) as quant,sum(e.q_return*e.e_price) as summa,";
    sql+="floor(sum(e.q_return*e.e_price*(100+e.nds_stavka))+0.5)/100 as summa_s_nds";
    sql+=" from EXITS_TABLE e,nmp_table n,publishers_table p";
    sql+=" where n.nmp_id=e.nmp_id and p.p_id=n.p_id";
    sql+=" and e.date_return='"+dt2.DateString()+"'";
    sql+=" group by n.INVOICE_TYPE,p.p_id,p.p_name,NDS_STAVKA";
    sql+=" having sum(e.q_return)<>0";
*/
  }
  return sql;
}
//---------------------------------------------------------------------------
// - �������� ������ � ������������ �������
TGridFrame * __fastcall cteateUnloadDataForBookPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="EXITS_TABLE,������� ������� ���,P_ID,P_NAME,,";

  fieldsInfo ="P_ID,P_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,������������ ����������,Left,1,1,1,0,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,������������ ���,Left,1,1,1,0,;";
  if(type==4)
  { fieldsInfo+="INVOICE_TYPE,INVOICE_TYPE,��� ���������,Center,1,1,1,0,;";
    iniSN+="_1";
  }
  fieldsInfo+="NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,1,0,;";
  fieldsInfo+="QUANT,QUANT,����������,Right,1,1,0,0,#;";
  fieldsInfo+="SUMMA,SUMMA,����� ��� ���,Right,1,1,0,0,#0.00;";
  fieldsInfo+="SUMMA_S_NDS,SUMMA_S_NDS,����� � ���,Right,1,1,0,0,#0.00";

  footerFieldsInfo ="QUANT,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(g_f!=NULL)
  { g_f->additionalAdjustmentsForGrid=nmpAdditionalAdjustments;
    g_f->initFrame();
    g_f->Parent=workPanel;
    g_f->setColumnsConfigFieldEditInGridVisible(false);
  }
  return g_f;
}
//---------------------------------------------------------------------------
TGridFrame * __fastcall cteateUnloadDataForBookSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type)
{
TGridFrame *g_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="FACTS_TABLE,������� ������������ ���,PO_ID,PO_NAME,,";

  fieldsInfo ="PO_ID,PO_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="PO_NAME,PO_NAME,������������ ��������,Left,1,1,1,0,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,������������ ���,Left,1,1,1,0,;";
  fieldsInfo+="NDS_STAVKA,NDS_STAVKA,������ ���,Center,1,1,1,0,;";
  fieldsInfo+="QUANT,QUANT,����������,Right,1,1,0,0,#;";
  fieldsInfo+="SUMMA,SUMMA,����� ��� ���,Right,1,1,0,0,#0.00";
//  fieldsInfo+="floor(sum(f.fact_q*e.e_price*(100+e.nds_stavka)),SUMMA_S_NDS,����� � ���,Right,1,1,0,0,#0.00";

  footerFieldsInfo ="QUANT,fvtSum,#,,Right;";
  footerFieldsInfo+="SUMMA,fvtSum,#0.00,,Right";
//  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    g_f=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateUnloadDataForBookKeepingFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
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

//������������ ��������� � ������� ��� ����������� ����
// - �������� ������� ������������ ��������� � �������
void createMessage(TComponent *owner,AnsiString iniFileName,int type)
{
TMessageParametersForm *MPF;
TStringList *sl;
TDateTime date1, date2;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  date1=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getDate1();
  date2=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getDate2();
  sl=new TStringList;
  if(type==1) //����������� ��� �� ����������
  { sl->Add(SetHeader("����������� �������-���������� ��������� �� �����������",date1,date2));
    MPF->setSubject("����������� ��� �� �����������");
  }
  else if(type==2) //�������� ��� � �������������
  { sl->Add(SetHeader("������� �������-���������� ��������� � �������������",date1,date2));
    MPF->setSubject("������� ��� � �������������");
  }
  else if(type==3) //������� ��� �� �������������
  { sl->Add(SetHeader("������� �������-���������� ��������� �� �������������",date1,date2));
    MPF->setSubject("������� ��� �� �������������");
  }
  else if(type==4) //������� ��� �����������
  { sl->Add(SetHeader("������� �������-���������� ��������� �����������",date1,date2));
    MPF->setSubject("������� ��� �����������");
  }
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { AnsiString subject=MPF->subjectEdit->Text,
               content=MPF->contentMemo->Text;
    bool replyFLG=MPF->replyCheckbox->Checked,
         delFilesFLG=MPF->delFilesCheckbox->Checked;

    AnsiString dirName=getAppDir()+"Out\\ToBookKeeping\\";
    FBDatabase *fbDB;
    AnsiString monthFolderID;
    TDBGridEh *sdGrid;
    TpFIBDataSet *sdDS;

    fbDB=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getFBDataBase();

    //������� ������ ��������
    unsigned short y,m,d;
    date2.DecodeDate(&y,&m,&d);
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(y)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(m)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    //��������� ������� ������ ����� � ������� MESSAGES_FOLDERS_TABLE
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �����������","",y,m);
    bool resultFLG=false;
    if(!monthFolderID.IsEmpty())
    { //1.��������� dbf ����� � ������������ �������
      AnsiString fileOut=createDBFFileForBookKeeping(owner,type,dirName);
      if(!fileOut.IsEmpty())
      { //2.������� ��������� ��� �������� � ������������� � ������������� ������
        TStringList *fileNames;
        fileNames=new TStringList;
        if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
        AnsiString eMail=getConstValue(owner,fbDB,"BOOKKEEPING_EMAIL",date2);
        addMessageToBase(owner,fbDB,monthFolderID,"����������� ����",eMail,subject,content,fileNames,replyFLG,delFilesFLG);
        delete fileNames;
        resultFLG=true;
        if(type==2) lockOrderingData(fbDB,date1,date2);
      }
    }
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
  delete sl;
  delete MPF;
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � �������
AnsiString __fastcall createDBFFileForBookKeeping(TComponent *owner,int type,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn=getAppDir()+"Out\\ToBookKeeping\\Templates\\";
AnsiString tableName;
unsigned short y,m,d;
TDateTime op_date;
TpFIBDataSet *ds;
TADOConnection *ADOConnection;
TADOQuery *ADO_Q;
AnsiString sql;

  //���������� ��������
  op_date=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getDate2();
  ds=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getDS();
  ADOConnection=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getADOConnection();
  ADO_Q=(static_cast<TUnloadDataForBookKeepingFrame*>(owner))->getADOQuery();
  if(type==1) tableName="p";
  else if(type==2) tableName="o";
  else if(type==3) tableName="vs";
  else if(type==4) tableName="vp";
  fileIn+=tableName+".dbf";
  if(!FileExists(fileIn))
  { MessageDlg("����������� ����� � �������� - "+fileIn,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  op_date.DecodeDate(&y,&m,&d);
  tableName+="_"+AnsiString().sprintf("%02d%02d",y-2000,m)+".dbf";
  fileOut+=tableName;

  //�������� ������
  if(templateCopy(fileIn,fileOut)== false) return "";

  //�������������� ADO �����������
  ADOConnection->Connected=false;
  AnsiString path=ExtractFilePath(fileOut);
  AnsiString str;
  str="Provider=MSDASQL.1;";
  str+="Persist Security Info=False;";
  str+="Data Source=dBASE Files;";
  str+="Mode=ReadWrite;";
  str+="Extended Properties=\"DSN=dBASE Files;";
  str+="DBQ="+path+";";
  str+="DefaultDir="+dirName+";";
  str+="DriverId=533;FIL=dBase 5.0;MaxBufferSize=2048;PageTimeout=5;\";";
  str+="Initial Catalog="+path;
  ADOConnection->ConnectionString=str;
  try
  {
    ADOConnection->Connected=true;
  }
  catch (Exception &exc)
  { MessageDlg("������ ������������� ADO �����������\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }

  //��������� ������ � dbf ����
  ds->First();
  while(!ds->Eof)
  { ADO_Q->Close();
    ADO_Q->SQL->Clear();
    if(type==1)
    { sql="insert into "+tableName+"(OP_DATE,P_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA,SUM_S_NDS) values(";
      sql+="'"+op_date.DateString()+"',";
      sql+="'"+ds->FieldByName("P_NAME")->AsString+"',";
      sql+="'"+ds->FieldByName("NMP_NAME")->AsString+"',";
      sql+=ds->FieldByName("NDS_STAVKA")->AsString+",";
      sql+=ds->FieldByName("QUANT")->AsString+",";
      sql+=ds->FieldByName("SUMMA")->AsString+",";
      sql+=ds->FieldByName("SUMMA_S_NDS")->AsString+")";
    }
    else if(type==2)
    { sql="insert into "+tableName+"(OP_DATE,SD_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA) values(";
      sql+="'"+op_date.DateString()+"',";
      sql+="'"+ds->FieldByName("PO_NAME")->AsString+"',";
      sql+="'"+ds->FieldByName("NMP_NAME")->AsString+"',";
      sql+=ds->FieldByName("NDS_STAVKA")->AsString+",";
      sql+=ds->FieldByName("QUANT")->AsString+",";
      sql+=ds->FieldByName("SUMMA")->AsString+")";
    }
    else if(type==3)
    { sql="insert into "+tableName+"(OP_DATE,SD_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA) values(";
      sql+="'"+op_date.DateString()+"',";
      sql+="'"+ds->FieldByName("PO_NAME")->AsString+"',";
      sql+="'"+ds->FieldByName("NMP_NAME")->AsString+"',";
      sql+=ds->FieldByName("NDS_STAVKA")->AsString+",";
      sql+=ds->FieldByName("QUANT")->AsString+",";
      sql+=ds->FieldByName("SUMMA")->AsString+")";
    }
    else if(type==4)
    { sql="insert into "+tableName+"(OP_DATE,P_NAME,NMP_NAME,NDS_STAVKA,QUANT,SUMMA,SUM_S_NDS,INV_TYPE) values(";
      sql+="'"+op_date.DateString()+"',";
      sql+="'"+ds->FieldByName("P_NAME")->AsString+"',";
      sql+="'"+ds->FieldByName("NMP_NAME")->AsString+"',";
      sql+=ds->FieldByName("NDS_STAVKA")->AsString+",";
      sql+=ds->FieldByName("QUANT")->AsString+",";
      sql+=ds->FieldByName("SUMMA")->AsString+",";
      sql+=ds->FieldByName("SUMMA_S_NDS")->AsString+",";
      sql+="'"+ds->FieldByName("INVOICE_TYPE")->AsString+"')";
    }
    ADO_Q->SQL->Add(sql);
    try
    {
      ADO_Q->ExecSQL();
    }
    catch (Exception &exc)
    { MessageDlg("������ - "+sql+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      ADOConnection->Connected=false;
      return "";
    }
    ds->Next();
  }
  ADOConnection->Connected=false;
  return fileOut;
}
//---------------------------------------------------------------------------
void  __fastcall lockOrderingData(class FBDatabase *fbDB,TDateTime date1,TDateTime date2)
{
FBQuery *fbWQ=fbDB->getFBQ_T2();
TpFIBTransaction *WT=fbWQ->getTransaction();
AnsiString sql;

  sql="update FACTS_TABLE set LOCK_DATA=1";
  sql+=" where E_ID in";
  sql+="(select E_ID from exits_table where";
  sql+=" e_date between '"+date1.DateString()+"'";
  sql+=" and '"+date2.DateString()+"')";
  WT->StartTransaction();
  if(fbWQ->execQuery("lockOrderingData",sql)==false)
  { WT->Rollback();
    return;
  }
  WT->Commit();
}
//---------------------------------------------------------------------------

