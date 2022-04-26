//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "GridContentF.h"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxExportRTF.hpp"
#include "frxExportXLS.hpp"
#include "SpisanieNMPF.h"
#include "VozvratNMPF.h"
#include "MessageParametersForm.h"
#include "DirectWorkExcel.h"
#include "ReturnNMPtoPublishersF.h"
#include "ReturnNMPtoPublishersFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvProgressBar"
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//����� ��� ������ ����������
// - ������� ������������ sql �������
AnsiString __fastcall formPublishersSelectSQL(TDateTime date1,TDateTime date2,int typeNMP)
{
AnsiString sql;

  sql= "select p_id,p_name,p_official_name,p_address,p_inn_kpp,";
  sql+="p_bank_requisitions,p_phone,p_email,reason,return_q,q_return,q_lost,";
  sql+="sum_return,sum_lost,sum_vsego";
  sql+=" from GET_RETURNS_NMP_TO_PUBLISHERS(";
  if(typeNMP==0)
    sql+="0,";
  else
    sql+="1,";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ ��� ������ ����������
TGridContentFrame * __fastcall createPublisherSelectFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridContentFrame *gc_f;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString contentsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� ���,P_ID,P_NAME,,";

  fieldsInfo= "P_ID,P_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,������������,Left,1,1,1,0,;";
  fieldsInfo+="P_OFFICIAL_NAME,P_OFFICIAL_NAME,������ ������������,Left,1,0,0,0,;";
  fieldsInfo+="P_ADDRESS,P_ADDRESS,�����,Left,1,0,0,0,;";
  fieldsInfo+="P_INN_KPP,P_INN_KPP,��� (���),Center,1,0,0,0,;";
  fieldsInfo+="P_BANK_REQUISITIONS,P_BANK_REQUISITIONS,��������� �����,Left,1,0,0,0,;";
  fieldsInfo+="P_PHONE,P_PHONE,�������,Left,1,0,0,0,;";
  fieldsInfo+="P_EMAIL,P_EMAIL,����������� �����,Left,1,0,0,0,;";
  fieldsInfo+="REASON,REASON,��������� ��� ������� ���������,Left,1,0,0,0,;";
  fieldsInfo+="RETURN_Q,RETURN_Q,����������|�� �����,Right,1,1,1,0,#;";
  fieldsInfo+="Q_RETURN,Q_RETURN,����������|����������|���-��,Right,1,1,1,0,#;";
  fieldsInfo+="SUM_RETURN,SUM_RETURN,����������|����������|����� ��� ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="SUM_VSEGO,SUM_VSEGO,����������|����������|����� � ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="Q_LOST,Q_LOST,������|���-��,Right,1,1,1,0,#;";
  fieldsInfo+="SUM_LOST,SUM_LOST,������|����� ��� ���,Right,1,1,1,0,#0.00";

  contentsInfo= "P_OFFICIAL_NAME,������ ������������;";
  contentsInfo+="P_ADDRESS,�����;";
  contentsInfo+="P_INN_KPP,��� (���);";
  contentsInfo+="P_BANK_REQUISITIONS,��������� �����;";
  contentsInfo+="P_PHONE,�������;";
  contentsInfo+="P_EMAIL,����������� �����;";
  contentsInfo+="REASON,��������� ��� ������� ���������";

  footerFieldsInfo ="RETURN_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="Q_RETURN,fvtSum,#,,Right;";
  footerFieldsInfo+="SUM_RETURN,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUM_VSEGO,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="Q_LOST,fvtSum,#,,Right;";
  footerFieldsInfo+="SUM_LOST,fvtSum,#0.00,,Right";

  try
  {
    gc_f=new TGridContentFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,contentsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createFactsFrame - ������ �������� TGridContentFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gc_f!=NULL)
  {
    gc_f->Parent=workPanel;
  }
  return gc_f;
}
//---------------------------------------------------------------------------
//������� �������� ������ ���
TFrame * __fastcall createReturnNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int typeNMP)
{
TFrame* nmp_f;

  if(typeNMP==0)
  { TSpisanieNMPFrame *sf=new TSpisanieNMPFrame(Owner,iniFN,iniSN,fbDB);
    if(sf!=NULL)
    { sf->Parent=workPanel;
      sf->initFrame();
      (static_cast<TReturnNMPtoPublishersFrame*>(Owner))->setNMPGridFrame(sf->getGridFrame());
      nmp_f=sf;
    }
  }
  else
  { TVozvratNMPFrame *vf=new TVozvratNMPFrame(Owner,iniFN,iniSN,fbDB);
    if(vf!=NULL)
    { vf->Parent=workPanel;
      vf->initFrame();
      (static_cast<TReturnNMPtoPublishersFrame*>(Owner))->setNMPGridFrame(vf->getGridFrame());
      nmp_f=vf;
    }
  }
  return nmp_f;
}
//---------------------------------------------------------------------------
//������� ��� ������ � ��������� ����-12 � ����� ����-2
// - ������ ���������
void __fastcall printTorg12(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *pDS,TpFIBDataSet *nmpDS,int kind)
{
TfrxReport *frxReport;
TfrxDBDataset * frxDS;
TfrxXLSExport * frxXLSExport;
AnsiString workDir=getAppDir();
TDBGridEh *pGrid,*nmpGrid;
TDateTime date1, date2;
AnsiString p_id;
FBDataSet *fbDS=NULL;
AnsiString sql;
bool spisaneFLG;

  if(nmpDS->RecordCount==0) return;

  frxReport=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxReport;
  frxDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxDS;
  frxXLSExport=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxXLSExport;
  pGrid=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getPGrid();
  if(pGrid==NULL) return;
  nmpGrid=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getNMPGrid();
  if(nmpGrid==NULL) return;
  date1=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();

  //��������� �� ��������
  if((static_cast<TReturnNMPtoPublishersFrame*>(owner))->typeComboBox->ItemIndex==0)
  { if(frxReport->LoadFromFile(workDir+"Reports\\torg12s.fr3")==false)
    { MessageDlg("�� ���� ��������� ���� � �������: "+workDir+"Reports\\Torg12s.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
    spisaneFLG=true;
  }
  //��������� �� �������
  else
  { if(frxReport->LoadFromFile(workDir+"Reports\\torg12v.fr3")==false)
    { MessageDlg("�� ���� ��������� ���� � �������: "+workDir+"Reports\\Torg12v.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
    spisaneFLG=false;
  }

  try
  {
    fbDS=newFBDataSet(owner,"printTorg12",db,true);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(kind==1) //������ ���������
  { (static_cast<TReturnNMPtoPublishersFrame*>(owner))->setFullPrintFLG(true);
    frxXLSExport->FileName="��������� - ���� 12.xls";

    nmpGrid->Visible=false;
    if(pGrid->SelectedRows->Count==0) pGrid->Selection->Rows->CurrentRowSelected=true;
    for(int i=0; i<pGrid->SelectedRows->Count; ++i)
    { pDS->GotoBookmark((void *)pGrid->SelectedRows->Items[i].c_str());
      p_id=pDS->FieldByName("P_ID")->AsString;
      sql="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
      sql+="e.q_return,e.nds_stavka,";
      sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,";
      sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,";
      sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds";
      sql+=" from exits_table e,nmp_table n";
      sql+=" where n.nmp_id=e.nmp_id";
//      sql+=" and (e.r_date between '"+date1.DateString()+"'";
//      sql+=" and '"+date2.DateString()+"'";
      sql+=" and e.date_return='"+date2.DateString()+"'";
      sql+=" and n.p_id="+p_id;
      sql+=" and e.q_return is not NULL and e.q_return<>0";
      sql+=" and n.invoice_type='�'";
      if(spisaneFLG==true)
        sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
      else
        sql+=" and n.ret_must_flg=1";
      sql+=" order by n.nmp_name||' �'||e.e_number";
      if(fbDS->execSelectSQL("printTorg12",sql)==false)
      { delete fbDS;
        return;
      }
      frxDS->DataSet=fbDS->getDataSet();
      frxReport->DataSet=NULL;
      if(i==0) frxReport->PrepareReport(true);
      else frxReport->PrepareReport(false);
    }
    nmpGrid->Visible=true;
    frxReport->ShowPreparedReport();
  }
  else //���������� ���������
  { (static_cast<TReturnNMPtoPublishersFrame*>(owner))->setFullPrintFLG(false);
    frxXLSExport->FileName="��������� - ���� 12 (���������).xls";

    p_id=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getPublisherID();
    if(p_id.IsEmpty()) return;

    if(nmpGrid->SelectedRows->Count==0) nmpGrid->Selection->Rows->CurrentRowSelected=true;
    AnsiString s;
    for(int i=0; i<nmpGrid->SelectedRows->Count; ++i)
    { nmpDS->GotoBookmark((void *)nmpGrid->SelectedRows->Items[i].c_str());
      if(i==0) s="(";
      else s+=",";
      s+=nmpDS->FieldByName("E_ID")->AsString;
    }
    s+=")";
    sql="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="e.q_return,e.nds_stavka,";
    sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,";
    sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,";
    sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds";
    sql+=" from exits_table e,nmp_table n";
    sql+=" where n.nmp_id=e.nmp_id";
//    sql+=" and (e.r_date between '"+date1.DateString()+"'";
//    sql+=" and '"+date2.DateString()+"'";
    sql+=" and e.date_return='"+date2.DateString()+"'";
    sql+=" and n.p_id="+p_id;
    sql+=" and e.q_return is not NULL and e.q_return<>0";
    sql+=" and n.invoice_type='�'";
    if(spisaneFLG==true)
      sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
    else
      sql+=" and n.ret_must_flg=1";
    sql+=" and e.e_id in "+s;
    sql+=" order by n.nmp_name||' �'||e.e_number";
    if(fbDS->execSelectSQL("printTorg12",sql)==false)
    { delete fbDS;
      return;
    }
    frxDS->DataSet=fbDS->getDataSet();
    frxReport->DataSet=NULL;
    frxReport->ShowReport(true);
  }
  delete fbDS;
}
//---------------------------------------------------------------------------
// - ������ ���� ����-2
void __fastcall printTorg2(TComponent *owner,int kind)
{
AnsiString fileOut=getAppDir()+"PreviewFiles\\";
FBDatabase *fbDB;
FBQuery *fbWQ;

  fileOut=createTorg2File(owner,kind,fileOut);
  if(fileOut.IsEmpty()) return;

  //��������� ���� �� ������� ��� �������� ��� ���������
  int hf=FileOpen(fileOut,fmShareExclusive);

  fbDB=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getFB_DB();
  fbWQ=fbDB->getFBQ_T2();
  //������� ���������� � ������� PREVIEW_FILES_TABLE
  //�������� ����������
  //���������� ����� �������� ����� ��� ������� PREVIEW_FILES_TABLE
  AnsiString pfID=getNewKeyValue(fbDB,"PREVIEW_FILES_TABLE");
  if(pfID.IsEmpty()) return;
  AnsiString u_id=getUser()->getUID();
  //  ������ ������
  AnsiString sql="insert into PREVIEW_FILES_TABLE(PF_ID,ATTACHED_FILE_ID,FILE_NAME,USER_ID) values(";
  sql+=pfID+",0,";
  sql+="'"+fileOut+"',";
  sql+=u_id+")";
  if(fbWQ->execQuery("printTorg2",sql)==false) return;
  //������������ ����
  FileClose(hf);
  //��������� ��������� �������� ��� ���������
  if(FileExists(fileOut))
  { HINSTANCE h=ShellExecute(NULL, NULL, fileOut.c_str(), NULL, NULL, SW_RESTORE);
    HINSTANCE errh=HINSTANCE(32);
    if(h<=errh)
    { MessageDlg("������ ������� ����������. ����� ������ - "+IntToStr(h),mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
  }
}
//---------------------------------------------------------------------------
// - ������ ���� ����-16
void __fastcall printTorg16(TComponent *owner,FBDatabase *fbDB)
{
TfrxReport *frxReport;
TfrxDBDataset * frxDS;
TfrxXLSExport * frxXLSExport;
AnsiString workDir=getAppDir();
TDateTime date1,date2;
AnsiString p_id;
AnsiString sql;
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
unsigned short y,m,d;

  p_id=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getPublisherID();
  date1=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();
  sql="select n.NMP_NAME||' �'||e.e_number as name_nmp,e.e_price,";
  sql+="e.nds_stavka,e.q_lost,e.q_lost*e.e_price as lost_summa";
  sql+=" from EXITS_TABLE e,NMP_TABLE n,PUBLISHERS_TABLE p";
  sql+=" where n.nmp_id=e.nmp_id and p.p_id=n.p_id";
  sql+=" and e.date_return='"+date2.DateString()+"'";
  sql+=" and p.P_ID="+p_id;
  sql+=" and e.q_lost is not NULL and e.q_lost <> 0";
  if(fbDS->execSelectSQL("printTorg16",sql)==false) return;
  if(fbDS->getDataSet()->RecordCount==0)
  { MessageDlg("������ ��� ���������� ������������� �����������",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxReport=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxReport;
  frxDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxDS;
  frxXLSExport=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->frxXLSExport;
  if(frxReport->LoadFromFile(workDir+"Reports\\torg16.fr3")==false)
  { MessageDlg("�� ���� ��������� ���� � �������: "+workDir+"Reports\\Torg16.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }
  frxXLSExport->FileName="����� ����-16.xls";
  frxDS->DataSet=fbDS->getDataSet();
  frxReport->ShowReport(true);
  fbDS->getDataSet()->Close();
}
//---------------------------------------------------------------------------

// - �������� ������� ������������ ��������� � ��������� ����-12 ��� ���� ����-2
void createMessage(TComponent *owner,AnsiString iniFileName,int type,int kind)
// type=1 - �������,���������� ��������
// type=2 - �������, ���������� ��������
// type=3 - ����-2
// kind=1 - ������ ���������, kind=2 - ���������� ���������
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *pBar,*nmpBar;
TDateTime date1, date2;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  pBar=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->pBar;
  nmpBar=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->nmpBar;
  date1=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();
  AnsiString str;
  if(type==1) str="��������� �� �������� �������-���������� ���������";
  else if(type==1) str="��������� �� ������� �������-���������� ���������";
  else if(type==3) str="��� ����-2 � ����������� �� ���������� � �������� ��� ������ ���������";
  str+=", �� ������ � "+date1.DateString()+" �� "+date2.DateString();
  sl->Add(str);
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(type==1)
  { if(kind==1) //������ ���������
      MPF->setSubject("��������� \"����-12\" �� �������� ���");
    else //���������� ���������
      MPF->setSubject("��������� \"����-12\" �� �������� ��� (���������)");
  }
  else if(type==2)
  { if(kind==1) //������ ���������
      MPF->setSubject("��������� \"����-12\" �� ������� ���");
    else //���������� ���������
      MPF->setSubject("��������� \"����-12\" �� ������� ��� (���������)");
  }
  else if(type==3)
  { if(kind==1) //������ ���������
      MPF->setSubject("��� \"����-2\"");
    else //���������� ���������
      MPF->setSubject("��� \"����-2\" (���������)");
  }
  if(MPF->ShowModal()==mrOk)
  { pBar->Visible=true;
    nmpBar->Visible=true;
    (static_cast<TReturnNMPtoPublishersFrame*>(owner))->Repaint();
    bool flg=createMessagesForPublishers(owner,type,kind,MPF->getSubject(),MPF->getConten(),MPF->getReplyFLG(),MPF->getDelFilesFLG());
    pBar->Visible=false;
    nmpBar->Visible=false;
    if(flg==true) MessageDlg("��������� ������������",mtInformation,TMsgDlgButtons() << mbOK,0);
    else MessageDlg("��� ������������ ��������� �������� ������",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
  delete sl;
  delete MPF;
}
//---------------------------------------------------------------------------
// - ������� ������������ ���������
bool __fastcall createMessagesForPublishers(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG)
{
AnsiString dirName=getAppDir()+"Out\\ToPublishers\\";
unsigned short year, month, d;
AnsiString monthFolderID="";
TDateTime date;
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *lGrid;
TpFIBDataSet *lDS;
TProgressBar *pBar;
AnsiString rName,rMail;
AnsiString fileOut="";
bool retFLG=false;
AnsiString mID="";

  date=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();
  lDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getLDS();
  lGrid=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getLGrid();
  pBar=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->pBar;
  rName=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientName();
  rMail=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientMail();

  date.DecodeDate(&year,&month,&d);
  //������� ������ ��������
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(year)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(month)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);

  //��������� ������� ������ ����� � ������� MESSAGES_FOLDERS_TABLE
  if(type==1)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �����������","��������� \"����-12\" - ��������",year,month);
  else if(type==2)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �����������","��������� \"����-12\" - �������",year,month);
  else if(type==3)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �����������","���� \"����-2\"",year,month);
  if(monthFolderID.IsEmpty()) return retFLG;

  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
  pBar->Max=lGrid->SelectedRows->Count;
  pBar->Position=0;
  if(kind==1)
  { for(int i=0; i<lGrid->SelectedRows->Count; ++i)
    { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
      //1.��������� Excel ����� � ��������� ���� 12
      if(type==1 || type==2) //��������� � ��������� ����-12
        fileOut=createTorg12File(owner,type,kind,dirName);
      else if(type==3) //��������� � ����� ����-2
        fileOut=createTorg2File(owner,kind,dirName);
      //2.������� ��������� ��� �������� � ������������� � ������������� ������
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      rName=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientName();
      rMail=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientMail();
      mID=addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
      delete fileNames;
      pBar->StepIt();
    }
  }
  else
  { //1.��������� Excel ����� � ��������� ���� 12
    if(type==1 || type==2) //��������� � ��������� ����-12 (���������)
      fileOut=createTorg12File(owner,type,kind,dirName);
    else if(type==3) //��������� � ����� ����-2 (���������)
      fileOut=createTorg2File(owner,kind,dirName);
    //2.������� ��������� ��� �������� � ������������� � ������������� ������
    TStringList *fileNames;
    fileNames=new TStringList;
    if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
    rName=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientName();
    rMail=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientMail();
    mID=addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
    delete fileNames;
    pBar->StepIt();
  }
  pBar->Position=0;
  if(!fileOut.IsEmpty() && !mID.IsEmpty()) retFLG=true;
  return retFLG;
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � ��������� ����-12
AnsiString __fastcall createTorg12File(TComponent *owner,int type,int kind,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn;
unsigned short y,m,d;
DirectWorkExcel *de;
TDateTime date1,date2;
FBDatabase *fbDB=getFBDataBase();
FBDataSet *fbDS;
AnsiString ricipientID;
AnsiString ricipientName;
TpFIBDataSet *lDS,*rDS;
TDBGridEh *rGrid;
TProgressBar *nmpBar;

  if(type==1)
    fileIn=getAppDir()+"Out\\ToPublishers\\Templates\\Torg12s.xls";
  else
    fileIn=getAppDir()+"Out\\ToPublishers\\Templates\\Torg12v.xls";

  lDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getLDS();
  rDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRDS();
  rGrid=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRGrid();
  nmpBar=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->nmpBar;
  date1=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();
  ricipientID=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientID();
  ricipientName=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRicipientName();
  date2.DecodeDate(&y,&m,&d);
  if(type==1)
    fileOut+="ns";
  else
    fileOut+="nv";
  //�������� ������
  fileOut+="12_"+AnsiString().sprintf("%d-%02d_%03d",y,m,StrToInt(ricipientID))+".xls";
  if(templateCopy(fileIn,fileOut)==false) return "";

  //������� Excel ����
  try
  {
    de=new DirectWorkExcel(fileOut,1);
  }
  catch(const Exception &exc)
  { MessageDlg("������ �������� ������� DirectWorkExcel\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  if(de->openFile(false)==false)
  { MessageDlg("������ �������� Excel �����",mtWarning,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  de->setSheetNumber(1);

  try
  {
    fbDS=newFBDataSet(owner,"createTorg12File",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }

  //������� ������ � Excel ����
  //  ���������
  // - �����������
  AnsiString s=getConstValue(owner,fbDB,"ORGANIZATION_NAME",date2);
  s+=getConstValue(owner,fbDB,"ORGANIZATION_ADDRESS",date2);
  s+=getConstValue(owner,fbDB,"BANK_REQUISITIONS",date2);
  de->setCellValue(7,"B",s);
  // - ����������� �������������
  s=getConstValue(owner,fbDB,"SS_NAME",date2);
  de->setCellValue(10,"B",s);
  // - ���������������
  if(!lDS->FieldByName("P_OFFICIAL_NAME")->IsNull) s=lDS->FieldByName("P_OFFICIAL_NAME")->AsString;
  else s=lDS->FieldByName("P_NAME")->AsString;
  if(!lDS->FieldByName("P_ADDRESS")->IsNull)
    s+=", "+lDS->FieldByName("P_ADDRESS")->AsString;
  if(!lDS->FieldByName("P_INN_KPP")->IsNull)
    s+=", "+lDS->FieldByName("P_INN_KPP")->AsString;
  if(!lDS->FieldByName("P_PHONE")->IsNull)
    s+=", "+lDS->FieldByName("P_PHONE")->AsString;
  if(!lDS->FieldByName("P_BANK_REQUISITIONS")->IsNull)
    s+=", "+lDS->FieldByName("P_BANK_REQUISITIONS")->AsString;
  de->setCellValue(13,"J",s);
  // - ����������
  de->setCellValue(19,"J",s);
//  if(!lDS->FieldByName("P_OFFICIAL_NAME")->IsNull) s=lDS->FieldByName("P_OFFICIAL_NAME")->AsString;
//  else s=lDS->FieldByName("P_NAME")->AsString;
  // - ���������
  s=getConstValue(owner,fbDB,"MAIN_ORGANIZATION",date2);
  de->setCellValue(17,"J",s);
  // - ��������� ��� ������� ��������
  if(!lDS->FieldByName("REASON")->IsNull)
  { s=lDS->FieldByName("REASON")->AsString;
    de->setCellValue(21,"J",s);
  }
  // - ����
  s=getConstValue(owner,fbDB,"OKPO",date2);
  de->setCellValue(8,"CM",s);
  // - ����
  s=getConstValue(owner,fbDB,"OKDP",date2);
  de->setCellValue(11,"CM",s);
  // - ����� ���������
  if(type==1) s="�_";
  else s="�_";
  s+=ricipientID+"_"+AnsiString().sprintf("%02d-%d",m,y);
  if(kind==2) s+=" - ���.";
  de->setCellValue(31,"AT",s);
  // - ���� ���������
  unsigned short y1,m1,d1;
  date2.DecodeDate(&y1,&m1,&d1);
  m1++; if(m1>12){ m1=1; y1++;}
  de->setCellValue(31,"BC",TDateTime(y1,m1,10).DateString());

  // ��������� �����
  AnsiString sql;
  AnsiString p_id=lDS->FieldByName("P_ID")->AsString;
  if(kind==1)
  { sql ="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="e.q_return,e.nds_stavka,";
    sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,"; //����� ��� ����� ���
    sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,"; //����� � ���
    sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds"; //����� ���
    sql+=" from exits_table e,nmp_table n";
    sql+=" where n.nmp_id=e.nmp_id";
    sql+=" and e.date_return='"+date2.DateString()+"'";
    sql+=" and n.p_id="+p_id;
    sql+=" and e.q_return is not NULL and e.q_return<>0";
    sql+=" and n.invoice_type='�'";
    if(type==1)
      sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
    else
      sql+=" and n.ret_must_flg=1";
    sql+=" order by n.nmp_name||' �'||e.e_number";
    if(fbDS->execSelectSQL("createTorg12File",sql)==false)
    { delete fbDS;
      return "";
    }
  }
  else
  { AnsiString s;
    if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
    for(int i=0; i<rGrid->SelectedRows->Count; ++i)
    { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
      if(i==0) s="(";
      else s+=",";
      s+=rDS->FieldByName("E_ID")->AsString;
    }
    s+=")";
    sql="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="e.q_return,e.nds_stavka,";
    sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,";
    sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,";
    sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds";
    sql+=" from exits_table e,nmp_table n";
    sql+=" where n.nmp_id=e.nmp_id";
    sql+=" and e.date_return='"+date2.DateString()+"'";
    sql+=" and e.q_return is not NULL and e.q_return<>0";
    sql+=" and n.invoice_type='�'";
    if(type==1)
      sql+=" and (n.ret_must_flg is NULL or n.ret_must_flg=0)";
    else
      sql+=" and n.ret_must_flg=1";
    sql+=" and e.e_id in "+s;
    sql+=" order by n.nmp_name||' �'||e.e_number";
    if(fbDS->execSelectSQL("createTorg12File",sql)==false)
    { delete fbDS;
      return "";
    }
  }
  int ns=37;
  int quant=0;
  double summa=0.0,summa_nds=0.0,summa_s_nds=0.0;
  rDS=fbDS->getDataSet();
  rDS->Last();
  nmpBar->Max=rDS->RecordCount;
  rDS->First();
  nmpBar->Position=0;
  while(!rDS->Eof)
  { putTorg12Str(ns,de,rDS);
    quant+=rDS->FieldByName("Q_RETURN")->AsInteger;
    summa+=rDS->FieldByName("SUMMA")->AsFloat;
    summa_nds+=rDS->FieldByName("SUMMA_NDS")->AsFloat;
    summa_s_nds+=rDS->FieldByName("SUMMA_S_NDS")->AsFloat;
    ns++;
    rDS->Next();
    nmpBar->StepIt();
  }
  delete fbDS;
  nmpBar->Position=0;

  // ������
  de->deleteRow(ns);
  de->setCellValue(ns,"BB",IntToStr(quant));
  de->setCellValue(ns,"BO",AnsiString().sprintf("%.02lf",summa));
  de->setCellValue(ns,"CF",AnsiString().sprintf("%.02lf",summa_nds));
  de->setCellValue(ns,"CN",AnsiString().sprintf("%.02lf",summa_s_nds));
  // - ���������� ����� ��������
  de->setCellValue(ns+4,"M",i_to_str(ns-37));
  // - ����� � ��� ��������
  de->setCellValue(ns+21,"Q",f_to_str(summa_s_nds));
  // - ������ �������� (���������)
  s=getConstValue(owner,fbDB,"LEADER_JOB_TITLE",date2);
  de->setCellValue(ns+26,"P",s);
  // - ������ �������� (���)
  s=getConstValue(owner,fbDB,"LEADER_FIO",date2);
  de->setCellValue(ns+26,"AH",s);
  // - ������� (�������) ��������� (���)
  s=getConstValue(owner,fbDB,"GBOOK_FIO",date2);
  de->setCellValue(ns+28,"AH",s);
  // - ������ ����� �������� (���������)
  s=getConstValue(owner,fbDB,"WORKMAN_JOB_TITLE",date2);
  de->setCellValue(ns+30,"P",s);
  // - ������ ����� �������� (���)
  s=getConstValue(owner,fbDB,"WORKMAN_FIO",date2);
  de->setCellValue(ns+30,"AH",s);
  date2.DecodeDate(&y,&m,&d);
  char *mes[12]={"������","�������","�����","������","���","����","����","�������","��������","�������","������","�������"};
  // - ����
  if(type==1) s="10";
  else s=IntToStr(d);
  de->setCellValue(ns+33,"I",s);
  // - �����
  if(type==1)
  { m++;
    if(m>12){ m=1; y++;}
  }
  de->setCellValue(ns+33,"O",AnsiString(mes[m-1]));
  // - ���
  de->setCellValue(ns+33,"S",IntToStr(y));

  //��������� ����
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------
// - ������� ������ ������ � Excel ���� � ��������� ����-12
void putTorg12Str(int ns, class DirectWorkExcel *de, TDataSet *DS)
{
  de->insertRow(ns);
  de->copyRange("A"+IntToStr(ns+1),"CT"+IntToStr(ns+1));
  de->pasteRange("A"+IntToStr(ns));
  de->setCellValue(ns,"A",IntToStr(ns-36));
  de->setCellValue(ns,"C",DS->FieldByName("NAME_NMP")->AsString);
  de->setCellValue(ns,"BB",DS->FieldByName("Q_RETURN")->AsString);
  de->setCellValue(ns,"BE",DS->FieldByName("E_PRICE")->AsString);
  de->setCellValue(ns,"BO",DS->FieldByName("SUMMA")->AsString);
  de->setCellValue(ns,"BY",DS->FieldByName("NDS_STAVKA")->AsString);
  de->setCellValue(ns,"CF",DS->FieldByName("SUMMA_NDS")->AsString);
  de->setCellValue(ns,"CN",DS->FieldByName("SUMMA_S_NDS")->AsString);
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � ��������� ����-2
AnsiString __fastcall createTorg2File(TComponent *owner,int kind,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn;
unsigned short y,m,d;
DirectWorkExcel *de;
TDateTime date1,date2;
FBDatabase *fbDB=getFBDataBase();
FBDataSet *fbDS;
AnsiString pID;
TpFIBDataSet *lDS,*rDS;
TDBGridEh *rGrid;
char *mes[12]={"������","�������","�����","������","���","����","����","�������","��������","�������","������","�������"};
AnsiString sql;

  fileIn=getAppDir()+"Out\\ToPublishers\\Templates\\Torg_2.xls";

  lDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getLDS();
  rDS=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRDS();
  rGrid=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getRGrid();
  date1=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate1();
  date2=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getDate2();
  pID=(static_cast<TReturnNMPtoPublishersFrame*>(owner))->getPublisherID();
  date2.DecodeDate(&y,&m,&d);
  fileOut+="torg2_"+AnsiString().sprintf("%d-%02d_%03d",y,m,StrToInt(pID));
  if(kind==1)
    fileOut+=".xls";
  else
    fileOut+="���.xls";
  //�������� ������
  if(templateCopy(fileIn,fileOut)==false) return "";

  //������� Excel ����
  try
  {
    de=new DirectWorkExcel(fileOut,1);
  }
  catch(const Exception &exc)
  { MessageDlg("������ �������� ������� DirectWorkExcel\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  if(de->openFile(false)==false)
  { MessageDlg("������ �������� Excel �����",mtWarning,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  //������� ������� DataSet
  try
  {
    fbDS=newFBDataSet(owner,"createTorg12File",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }

  //������� ������ � Excel ����
  //�������� 1
  de->setSheetNumber(1);
  AnsiString s=getConstValue(owner,fbDB,"ORGANIZATION_NAME",date2);
  de->setCellValue("ORG_NAME",s);
  de->setCellValue("SUB_DIV_NAME","��� �������������� ������");
  s=pID+"_"+AnsiString().sprintf("%02d",m)+"-"+AnsiString().sprintf("%d",y);
  if(kind==2) s+="���.";
  de->setCellValue("ACT_NUM",s);
  unsigned short y1,m1,d1;
  date2.DecodeDate(&y1,&m1,&d1);
  m1++; if(m1>12){m1=1; y1++;}
  d1=10;
  TDateTime actDate=TDateTime(y1,m1,d1);
  s=actDate.DateString();
  de->setCellValue("ACT_DATE",s);
  s=getConstValue(owner,fbDB,"LEADER_JOB_TITLE",actDate);
  de->setCellValue("LEADER_JOB_TITLE",s);
  s=getConstValue(owner,fbDB,"LEADER_FIO",actDate);
  de->setCellValue("LEADER_FIO",s);
  de->setCellValue("LEADER_DATE_D",IntToStr(d1));
  de->setCellValue("LEADER_DATE_M",AnsiString(mes[m1-1]));
  de->setCellValue("LEADER_DATE_Y",IntToStr(y1));
  de->setCellValue("PLACE_ACCEPT_GOODS","��� �������������� ������, "+getConstValue(owner,fbDB,"ORGANIZATION_NAME",date2));
//  de->setCellValue("ACT_DATE_D",IntToStr(d));
//  de->setCellValue("ACT_DATE_M",AnsiString(mes[m-1]));
//  de->setCellValue("ACT_DATE_Y",IntToStr(y));
  if(!lDS->FieldByName("P_OFFICIAL_NAME")->IsNull) s=lDS->FieldByName("P_OFFICIAL_NAME")->AsString;
  else s=lDS->FieldByName("P_NAME")->AsString;
  if(!lDS->FieldByName("P_ADDRESS")->IsNull)
    s+=", "+lDS->FieldByName("P_ADDRESS")->AsString;
  if(!lDS->FieldByName("P_INN_KPP")->IsNull)
    s+=", "+lDS->FieldByName("P_INN_KPP")->AsString;
  if(!lDS->FieldByName("P_PHONE")->IsNull)
    s+=", "+lDS->FieldByName("P_PHONE")->AsString;
  if(!lDS->FieldByName("P_BANK_REQUISITIONS")->IsNull)
    s+=", "+lDS->FieldByName("P_BANK_REQUISITIONS")->AsString;
  de->setCellValue("CONSIGNOR",s);
  de->setCellValue("PRODUCER",s);
  de->setCellValue("SUPPLIER",s);
  if(!lDS->FieldByName("REASON")->IsNull)
  { s=lDS->FieldByName("REASON")->AsString;
    AnsiString dog_n=s.SubString(1,s.Pos(" �� ")-1);
    if(dog_n.Pos("�") > 0) dog_n=dog_n.SubString(dog_n.Pos("�")+1,dog_n.Length());
    de->setCellValue("D_NUMBER",dog_n);
    s=s.SubString(s.Pos("�� ")+3,s.Length());
    if(s.Pos("�") > 0) s=s.SubString(1,s.Pos("�")-1);
    TDateTime dog_date=TDateTime(s);
    unsigned short y2,m2,d2;
    dog_date.DecodeDate(&y2,&m2,&d2);
    de->setCellValue("D_DAY",IntToStr(d2));
    de->setCellValue("D_MONTH",AnsiString(mes[m2-1]));
    de->setCellValue("D_YEAR",IntToStr(y2));
  }
  //�������� 2,3
  if(kind==1)
  { sql ="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="sum(f.return_q) as return_q,e.e_q,e.q_return,e.nds_stavka";
    sql+=" from facts_table f";
    sql+=" left outer join exits_table e on e.e_id=f.e_id";
    sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
    sql+=" where e.date_return='"+date2.DateString()+"'";
    sql+=" and n.p_id="+pID;
    sql+=" and e.q_return is not NULL and e.q_return<>0";
    sql+=" and n.invoice_type='�'";
    sql+=" group by n.nmp_name,e.e_number,e.e_price,e.nds_stavka,e.e_q,e.q_return";
    sql+=" order by n.nmp_name||' �'||e.e_number";
    if(fbDS->execSelectSQL("createTorg12File",sql)==false)
    { delete fbDS;
      return "";
    }
  }
  else
  { sql ="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="sum(f.return_q) as return_q,e.e_q,e.q_return,e.nds_stavka";
    sql+=" from facts_table f";
    sql+=" left outer join exits_table e on e.e_id=f.e_id";
    sql+=" left outer join nmp_table n on n.nmp_id=e.nmp_id";
    sql+=" where n.invoice_type='�'";
    if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
    for(int i=0; i<rGrid->SelectedRows->Count; ++i)
    { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
      if(i==0) sql+=" and (";
      else sql+=" or ";
      sql+="e.E_ID="+rDS->FieldByName("E_ID")->AsString;
    }
    sql+=")";
    sql+=" group by n.nmp_name,e.e_number,e.e_price,e.nds_stavka,e.e_q,e.q_return";
    sql+=" order by n.nmp_name||' �'||e.e_number";
    if(fbDS->execSelectSQL("createTorg12File",sql)==false)
    { delete fbDS;
      return "";
    }
  }
  rDS=fbDS->getDataSet();
  de->setSheetNumber(2);
  int ns=40,dr=0;
  int q1=0, q2=0, q3=0;
  double s1=0.0, s2=0.0, s3=0.0;
  rDS->First();
  while(!rDS->Eof)
  { putTorg2Str(ns,dr,de,rDS);
    q1+=rDS->FieldByName("E_Q")->AsInteger;
    s1+=getSumma1(rDS);
    q2+=rDS->FieldByName("E_Q")->AsInteger-rDS->FieldByName("Q_RETURN")->AsInteger;
    s2+=getSumma2(rDS);
    q3+=rDS->FieldByName("Q_RETURN")->AsInteger;
    s3+=getSumma3(rDS);
    dr++;
    rDS->Next();
  }
  //�����
  int row=ns+dr;
  de->setCellValue(row,"A","�����:");
  de->setCellValue(row,"BS",IntToStr(q1));
  de->setCellValue(row,"CQ",AnsiString().sprintf("%.02lf",s1));
  de->setCellValue(row,"DR",IntToStr(q2));
  de->setCellValue(row,"EF",AnsiString().sprintf("%.02lf",s2));
  de->setCellValue(row,"FO",IntToStr(q3));
  de->setCellValue(row,"FW",AnsiString().sprintf("%.02lf",s3));
  //�������� 4
  de->setSheetNumber(3);
  s=getConstValue(owner,fbDB,"TORG2_CHAIRMAN_JT",date2);
  de->setCellValue("CHAIRMAN_JOB_TITLE",s);
  s=getConstValue(owner,fbDB,"TORG2_CHAIRMAN_FIO",date2);
  de->setCellValue("CHAIRMAN_FIO",s);

  s=getConstValue(owner,fbDB,"TORG2_MEMBER1_JT",date2);
  de->setCellValue("MEMBER1_JOB_TITLE",s);
  s=getConstValue(owner,fbDB,"TORG2_MEMBER1_FIO",date2);
  de->setCellValue("MEMBER1_FIO",s);

  s=getConstValue(owner,fbDB,"TORG2_MEMBER2_JT",date2);
  de->setCellValue("MEMBER2_JOB_TITLE",s);
  s=getConstValue(owner,fbDB,"TORG2_MEMBER2_FIO",date2);
  de->setCellValue("MEMBER2_FIO",s);

  s=getConstValue(owner,fbDB,"TORG2_MEMBER3_JT",date2);
  de->setCellValue("MEMBER3_JOB_TITLE",s);
  s=getConstValue(owner,fbDB,"TORG2_MEMBER3_FIO",date2);
  de->setCellValue("MEMBER3_FIO",s);
  //��������� ����
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------
// - ������� ������ ������ � Excel ���� � ����� ����-2
void putTorg2Str(int ns,int dr,class DirectWorkExcel *de,TDataSet *DS)
{
int row=ns+dr;

  if(dr > 10)
  { de->insertRow(row);
    de->copyRange("A"+IntToStr(row+1),"HF"+IntToStr(row+1));
    de->pasteRange("A"+IntToStr(row));
  }
  // �������� 2
  de->setCellValue(row,"A",DS->FieldByName("NAME_NMP")->AsString);
  de->setCellValue(row,"BS",DS->FieldByName("E_Q")->AsString);
  double price_s_nds=getPriceS_NDS(DS);
  de->setCellValue(row,"CD",AnsiString().sprintf("%.02lf",price_s_nds));
  de->setCellValue(row,"CQ",AnsiString().sprintf("%.02lf",getSumma1(DS)));
  // �������� 3
  int d=DS->FieldByName("E_Q")->AsInteger-DS->FieldByName("Q_RETURN")->AsInteger;
  de->setCellValue(row,"DR",IntToStr(d));
  de->setCellValue(row,"DY",AnsiString().sprintf("%.02lf",price_s_nds));
  de->setCellValue(row,"EF",AnsiString().sprintf("%.02lf",getSumma2(DS)));
  de->setCellValue(row,"FO",DS->FieldByName("Q_RETURN")->AsString);
  de->setCellValue(row,"FW",AnsiString().sprintf("%.02lf",getSumma3(DS)));
}
//---------------------------------------------------------------------------
double getPriceS_NDS(TDataSet *DS)
{
double price=DS->FieldByName("E_PRICE")->AsFloat; //���� ��� ���
int nds_stavka=DS->FieldByName("NDS_STAVKA")->AsInteger; //������ ���
double price_s_nds=floor(price*(100+nds_stavka)+0.5)/100;

  return price_s_nds;
}
//---------------------------------------------------------------------------
double getSumma1(TDataSet *DS)
{
int e_q=DS->FieldByName("E_Q")->AsInteger; //�����������
double price=DS->FieldByName("E_PRICE")->AsFloat; //���� ��� ���
int nds_stavka=DS->FieldByName("NDS_STAVKA")->AsInteger; //������ ���
double summa=floor(e_q*price*(100+nds_stavka)+0.5)/100;

  return summa;
}
//---------------------------------------------------------------------------
double getSumma2(TDataSet *DS)
{
  return getSumma1(DS)-getSumma3(DS);
}
//---------------------------------------------------------------------------
double getSumma3(TDataSet *DS)
{
int q_r=DS->FieldByName("Q_RETURN")->AsInteger; //�������
double price=DS->FieldByName("E_PRICE")->AsFloat; //���� ��� ���
int nds_stavka=DS->FieldByName("NDS_STAVKA")->AsInteger; //������ ���
double summa=floor(q_r*price*(100+nds_stavka)+0.5)/100;

  return summa;
}
//---------------------------------------------------------------------------

