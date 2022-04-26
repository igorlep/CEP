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
#include "DictationOnPaymentF.h"
#include "DocumentsFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvProgressBar"
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//****************************************************
//* ������ �� ������ �� ��� � ������������ �� ������ *
//****************************************************
//����� ��� ������ � �������������� �� ������
// - ������� ������������ select sql �������
AnsiString __fastcall formDictationsSelectSQL(TDateTime date)
{
AnsiString sql="";

  sql="select d_id,p_id,p_name,reason,counts_on_payment,supply_s,defect_s,";
  sql+="write_off_s,payment_s";
  sql+=" from GET_PAYMENTS_FOR_PUB(";
  sql+=" '"+date.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ � ��������������
TGridFrame * __fastcall createDictationsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* df=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="DICTATION_ON_PAYMENT_TABLE,������ �� ���������,P_ID,D_DATE,,";

  fieldsInfo ="D_ID,D_ID,���,Center,1,0,0,0,;";
  fieldsInfo+="P_ID,P_ID,���������|���,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,���������|������������,Left,1,1,1,0,;";
  fieldsInfo+="REASON,REASON,���������,Left,1,1,1,0,;";
  fieldsInfo+="COUNTS_ON_PAYMENT,COUNTS_ON_PAYMENT,������ ������,Left,0,1,1,0,;";
  fieldsInfo+="SUPPLY_S,SUPPLY_S,����� ��������,Right,0,1,1,0,#0.00;";
  fieldsInfo+="DEFECT_S,DEFECT_S,����� �����,Right,0,1,1,0,#0.00;";
  fieldsInfo+="WRITE_OFF_S,WRITE_OFF_S,����� ��������,Right,1,1,1,0,#0.00;";
  fieldsInfo+="PAYMENT_S,PAYMENT_S,����� � ������,Right,1,1,1,0,#0.00";

  footerFieldsInfo ="SUPPLY_S,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="DEFECT_S,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="WRITE_OFF_S,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="PAYMENT_S,fvtSum,#0.00,,Right";

  try
  {
    df=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createDictationsFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(df!=NULL)
  { df->initFrame();
    df->getWorkDataSet()->SQLs->InsertSQL->Clear();
    df->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    df->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    AnsiString sql;
    sql="update DICTATION_ON_PAYMENT_TABLE set COUNTS_ON_PAYMENT=?COUNTS_ON_PAYMENT";
    sql+=",SUPPLY_S=?SUPPLY_S,DEFECT_S=?DEFECT_S";
    sql+=" where D_ID=?OLD_D_ID";
    df->getWorkDataSet()->SQLs->UpdateSQL->Add(sql);
    df->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    df->Parent=workPanel;
  }
  return df;
}
//---------------------------------------------------------------------------

//����� ��� ���������� ������� � ������
AnsiString __fastcall formPubSelectSQL(TDateTime dDate)
{
AnsiString sql;

  sql="select p.p_id,p.p_name,p.reason from publishers_table p";
  sql+=" where p.p_id not in";
  sql+=" (select d.p_id from DICTATION_ON_PAYMENT_TABLE d where";
  sql+=" d.d_date='"+dDate.DateString()+"')";
  sql+=" and (p.del_flg is null or p.del_flg=0)";
  sql+=" order by p.p_name";
  return sql;
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall createAddRecordsToRoolFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *af=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� �����������,P_ID,P_NAME,,";

  fieldsInfo= "p.P_ID,P_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,���������,Left,1,1,1,0,;";
  fieldsInfo+="p.REASON,REASON,��������� ��� �������,Left,1,1,0,0,";

  try
  {
    af=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createAppFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(af!=NULL)
  { af->initFrame();
    af->Parent=workPanel;
  }
  return af;
}
//---------------------------------------------------------------------------

//������� ��� ������ � ��������������
// - ������ ������������
void __fastcall printDictation(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *pDS,TpFIBDataSet *dDS)
{
//
}
//---------------------------------------------------------------------------
/*
// - �������� ������� ������������ ��������� � ��������������
void createMessage(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *pBar,*dBar;
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
AnsiString dirName=getWorkDir()+"Out\\ToPublishers\\";
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
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"��� �����������","��������� \"����-12\" - ��������",year,month);
  else if(type==2)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"��� �����������","��������� \"����-12\" - �������",year,month);
  else if(type==3)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"��� �����������","���� \"����-2\"",year,month);
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
  return "";
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � ������������
AnsiString __fastcall createDictationFile(TComponent *owner,AnsiString dirName)
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
    fileIn=getWorkDir()+"Out\\ToPublishers\\Templates\\Torg12s.xls";
  else
    fileIn=getWorkDir()+"Out\\ToPublishers\\Templates\\Torg12v.xls";

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
  // - ���������
  s=getConstValue(owner,fbDB,"MAIN_ORGANIZATION",date2);
  de->setCellValue(17,"J",s);
  // - ����������
  if(!lDS->FieldByName("P_OFFICIAL_NAME")->IsNull) s=lDS->FieldByName("P_OFFICIAL_NAME")->AsString;
  else s=lDS->FieldByName("P_NAME")->AsString;
  de->setCellValue(19,"J",s);
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
  de->setCellValue(31,"BC",date2.DateString());

  // ��������� �����
  AnsiString sql;
  AnsiString p_id=lDS->FieldByName("P_ID")->AsString;
  if(kind==1)
  { sql ="select distinct n.nmp_name||' �'||e.e_number as name_nmp,e.e_price,";
    sql+="e.q_return,n.stavka_nds,";
    sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,"; //����� ��� ����� ���
    sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,"; //����� � ���
    sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds"; //����� ���
    sql+=" from exits_table e,nmp_table n";
    sql+=" where n.nmp_id=e.nmp_id";
    sql+=" and e.r_date between '"+date1.DateString()+"'";
    sql+=" and '"+date2.DateString()+"'";
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
    sql+="e.q_return,n.stavka_nds,";
    sql+="floor(e.e_price*e.q_return*100+0.5)/100 as summa,";
    sql+="floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100 as summa_s_nds,";
    sql+="(floor(e.q_return*e.e_price*(100+e.nds_stavka)+0.5)/100-floor(e.e_price*e.q_return*100+0.5)/100) as summa_nds";
    sql+=" from exits_table e,nmp_table n";
    sql+=" where n.nmp_id=e.nmp_id";
    sql+=" and e.r_date between '"+date1.DateString()+"'";
    sql+=" and '"+date2.DateString()+"'";
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
  s=getConstValue(owner,fbDB,"TORG_12_SENDER_JT",date2);
  de->setCellValue(ns+30,"P",s);
  // - ������ ����� �������� (���)
  s=getConstValue(owner,fbDB,"TORG_12_SENDER_FIO",date2);
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
  return "";
}
//---------------------------------------------------------------------------
*/

//******************
//* �������� ����� *
//******************
// ������� ������������ select sql ������� ��� ������ � ������� �������
AnsiString __fastcall formLeftSelectSQL(TDateTime date1,TDateTime date2,int type)
//type=1 - ����� � �������� �� �����������, 2-����� c ��������� �� �������������
{
AnsiString sql="";

  if(type==1) sql=formArrivalFromPubSelectSQL(date1,date2);
  else sql=formReturnFromSubSelectSQL(date1,date2);
  return sql;
}
//---------------------------------------------------------------------------
AnsiString __fastcall formRihgtSelectSQL(TDateTime date1,TDateTime date2,int type)
//type=1 - ����� � ��������� � �������������, 2-����� c ��������� �����������
{
AnsiString sql="";

  if(type==1) sql=formShipmentToSubSelectSQL(date1,date2);
  else sql=formReturnToPubSelectSQL(date1,date2);
  return sql;
}
//---------------------------------------------------------------------------
//������� �������� ������ � ������� �������
class TGridFrame * __fastcall createLeftFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type,TJvNavPanelHeader *header,TStatusBar *bar)
//type=1 - ����� � �������� �� �����������, 2-����� c ��������� �� �������������
{
TGridFrame *f;

  if(type==1) f=createArrivalFromPubFrame(Owner,iniFN,iniSN+"L1",fbDB,workPanel);
  else f=createReturnFromSDFrame(Owner,iniFN,iniSN+"L2",fbDB,workPanel);
  if(f!=NULL)
  { f->setHeader(header);
    f->setHeaderCaption(f->getTableTitle());
    f->setStatusBar(bar);
  }
  return f;
}
//---------------------------------------------------------------------------
class TGridFrame * __fastcall createRightFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type,TJvNavPanelHeader *header,TStatusBar *bar)
//type=1 - ����� � ��������� � �������������, 2-����� c ��������� �����������
{
TGridFrame *f;

  if(type==1) f=createShipmentToSubFrame(Owner,iniFN,iniSN+"R1",fbDB,workPanel);
  else f=createReturnToPubFrame(Owner,iniFN,iniSN+"R2",fbDB,workPanel);
  if(f!=NULL)
  { f->setHeader(header);
    f->setHeaderCaption(f->getTableTitle());
    f->setStatusBar(bar);
  }
  return f;
}
//---------------------------------------------------------------------------

//����� - ������ �� �����������
// - ������� ������������ select sql �������
AnsiString __fastcall formArrivalFromPubSelectSQL(TDateTime date1,TDateTime date2)
{
AnsiString sql="";

  sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA_S_NDS from GET_GM_FOR_AS(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  sql+=" where D_TYPE=1";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ � �������� �� �����������
class TGridFrame * __fastcall createArrivalFromPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* af=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,�������� �� �����������,ID,NAME,,";

  fieldsInfo ="ID,ID,���������|���,Center,1,1,0,0,;";
  fieldsInfo+="NAME,NAME,���������|������������,Left,1,1,1,0,;";
  fieldsInfo+="DOC_DATE,DOC_DATE,��������|����,Center,1,1,1,0,;";
  fieldsInfo+="DOC_NUMBER,DOC_NUMBER,��������|�����,Center,1,1,1,0,;";
  fieldsInfo+="SUMMA,SUMMA,�����|��� ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="SUMMA_S_NDS,SUMMA_S_NDS,�����|� ���,Right,1,1,1,0,#0.00";

  footerFieldsInfo ="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    af=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createArrivalFromPubFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(af!=NULL)
  { af->initFrame();
    af->getWorkDataSet()->SQLs->InsertSQL->Clear();
    af->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    af->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    af->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    af->Parent=workPanel;
  }
  return af;
}
//---------------------------------------------------------------------------
//����� - �������� � �������������
// - ������� ������������ select sql �������
AnsiString __fastcall formShipmentToSubSelectSQL(TDateTime date1,TDateTime date2)
{
AnsiString sql="";

  sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA_S_NDS";
  sql+=" from GET_GM_FOR_AS(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  sql+=" where D_TYPE=2";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ � ��������� � �������������
class TGridFrame * __fastcall createShipmentToSubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* sf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="SUBDIVISIONS_TABLE,���������� � �������������,ID,NAME,,";

  fieldsInfo ="ID,ID,�������������|���,Center,1,1,0,0,;";
  fieldsInfo+="NAME,NAME,�������������|������������,Left,1,1,1,0,;";
  fieldsInfo+="DOC_DATE,DOC_DATE,��������|����,Center,1,1,1,0,;";
  fieldsInfo+="DOC_NUMBER,DOC_NUMBER,��������|�����,Center,1,1,1,0,;";
  fieldsInfo+="SUMMA,SUMMA,�����|��� ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="SUMMA_S_NDS,SUMMA_S_NDS,�����|� ���,Right,1,1,1,0,#0.00";

  footerFieldsInfo ="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    sf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createShipmentToSubFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(sf!=NULL)
  { sf->initFrame();
    sf->getWorkDataSet()->SQLs->InsertSQL->Clear();
    sf->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    sf->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    sf->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    sf->Parent=workPanel;
  }
  return sf;
}
//---------------------------------------------------------------------------

//����� - ������� �� �������������
// - ������� ������������ select sql �������
AnsiString __fastcall formReturnFromSubSelectSQL(TDateTime date1,TDateTime date2)
{
AnsiString sql="";

  sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA_S_NDS";
  sql+=" from GET_GM_FOR_RETURN(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  sql+=" where D_TYPE=1";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ � ��������� �� �������������
class TGridFrame * __fastcall createReturnFromSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* rsf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="SUBDIVISIONS_TABLE,���������� �� �������������,ID,NAME,,";

  fieldsInfo ="ID,ID,�������������|���,Center,1,1,0,0,;";
  fieldsInfo+="NAME,NAME,�������������|������������,Left,1,1,1,0,;";
  fieldsInfo+="DOC_DATE,DOC_DATE,��������|����,Center,1,1,1,0,;";
  fieldsInfo+="DOC_NUMBER,DOC_NUMBER,��������|�����,Center,1,1,1,0,;";
  fieldsInfo+="SUMMA,SUMMA,�����|��� ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="SUMMA_S_NDS,SUMMA_S_NDS,�����|� ���,Right,1,1,1,0,#0.00";

  footerFieldsInfo ="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    rsf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createReturnFromSDFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(rsf!=NULL)
  { rsf->initFrame();
    rsf->getWorkDataSet()->SQLs->InsertSQL->Clear();
    rsf->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    rsf->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    rsf->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    rsf->Parent=workPanel;
  }

  return rsf;
}
//---------------------------------------------------------------------------
//����� - ������� �����������
// - ������� ������������ select sql �������
AnsiString __fastcall formReturnToPubSelectSQL(TDateTime date1,TDateTime date2)
{
AnsiString sql="";

  sql="select ID,NAME,DOC_DATE,DOC_NUMBER,SUMMA,SUMMA_S_NDS from";
  sql+=" GET_GM_FOR_RETURN(";
  sql+="'"+date1.DateString()+"',";
  sql+="'"+date2.DateString()+"')";
  sql+=" where D_TYPE=2";
  return sql;
}
//---------------------------------------------------------------------------
// - ������� �������� ������ � ��������� �����������
class TGridFrame * __fastcall createReturnToPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* rpf=NULL;

AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="PUBLISHERS_TABLE,���������� �����������,ID,NAME,,";

  fieldsInfo ="ID,ID,���������|���,Center,1,1,0,0,;";
  fieldsInfo+="NAME,NAME,���������|������������,Left,1,1,1,0,;";
  fieldsInfo+="DOC_DATE,DOC_DATE,��������|����,Center,1,1,1,0,;";
  fieldsInfo+="DOC_NUMBER,DOC_NUMBER,��������|�����,Center,1,1,1,0,;";
  fieldsInfo+="SUMMA,SUMMA,�����|��� ���,Right,1,1,1,0,#0.00;";
  fieldsInfo+="SUMMA_S_NDS,SUMMA_S_NDS,�����|� ���,Right,1,1,1,0,#0.00";

  footerFieldsInfo ="SUMMA,fvtSum,#0.00,,Right;";
  footerFieldsInfo+="SUMMA_S_NDS,fvtSum,#0.00,,Right";

  try
  {
    rpf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createReturnToPubFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(rpf!=NULL)
  { rpf->initFrame();
    rpf->getWorkDataSet()->SQLs->InsertSQL->Clear();
    rpf->getWorkDataSet()->SQLs->DeleteSQL->Clear();
    rpf->getWorkDataSet()->SQLs->UpdateSQL->Clear();
    rpf->getWorkDataSet()->SQLs->RefreshSQL->Clear();
    rpf->Parent=workPanel;
  }
  return rpf;
}
//---------------------------------------------------------------------------

