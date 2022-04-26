//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "DirectWorkExcel.h"
#include "SomeFunctions.h"
#include "NextGridF.h"
#include "MessageParametersForm.h"
#include "FormApplicationsForNMPToPublishersF.h"
#include "FormApplicationsForNMPToPublishersFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// ����� �������������
// -- ������������ Select �������
AnsiString __fastcall formPublishersSelectSQL(TDateTime dt)
{
AnsiString sql="";

  sql="select p_id,p_name,p_official_name,p_address,p_inn_kpp,p_bank_requisitions,p_phone,p_email,sum_temp_q";
  sql+=" from GET_PUBLISHERS_FOR_APPICATION(";
  sql+="'"+dt.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteatePublishersFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TNextGridFrame *p_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="PUBLISHERS_TABLE,���������� ���,P_ID,P_NAME,,";

  fieldsInfo ="P_ID,P_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="P_NAME,P_NAME,������������ ����������,Left,1,1,1,0,;";
  fieldsInfo+="P_OFFICIAL_NAME,P_OFFICIAL_NAME,����������� ������������,Left,1,0,0,0,;";
  fieldsInfo+="P_ADDRESS,P_ADDRESS,�����,Left,1,0,0,0,;";
  fieldsInfo+="P_INN_KPP,P_INN_KPP,���/���,Left,1,0,0,0,;";
  fieldsInfo+="P_BANK_REQUISITIONS,P_BANK_REQUISITIONS,���������� ���������,Left,1,0,0,0,;";
  fieldsInfo+="P_PHONE,P_PHONE,�������,Left,1,0,0,0,;";
  fieldsInfo+="P_EMAIL,P_EMAIL,����������� �����,Left,1,1,1,0,;";
  fieldsInfo+="SUM_TEMP_Q,SUM_TEMP_Q,��������,Right,1,1,0,0,#";

  contentFieldsInfo ="P_OFFICIAL_NAME,����������� ������������;";
  contentFieldsInfo+="P_ADDRESS,�����;";
  contentFieldsInfo+="P_INN_KPP,���/���;";
  contentFieldsInfo+="P_BANK_REQUISITIONS,���������� ���������;";
  contentFieldsInfo+="P_PHONE,�������";

  footerFieldsInfo ="SUM_TEMP_Q,fvtSum,#,,Right";

  try
  {
    p_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteatePublishersFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  p_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  p_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //�������������� ���������
  p_f->gcHeader->ColorFrom=cl3DDkShadow;
  p_f->gcHeader->ColorTo=clWhite;
  p_f->gcHeader->Font->Color=clNavy;
  p_f->getContentFrame()->contentHeader->ColorFrom=cl3DDkShadow;
  p_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  p_f->getContentFrame()->contentHeader->Font->Color=clNavy;
  //��������� ����
  p_f->columnsConfig->Visible=true;
  return p_f;
}
//---------------------------------------------------------------------------
// ����� � ���������� ���
// -- ������������ Search �������
AnsiString __fastcall formAppNMPToPubSearchSQL(AnsiString p_id)
{
AnsiString sql;

  sql="select distinct n.NMP_ID,n.NMP_NAME";
  sql+=" from NMP_TABLE n";
  sql+=" where (n.del_flg is null or n.del_flg=0)";
  sql+=" and n.P_ID=p_id";
  sql+=" and n.comprise_of_template=1";
  return sql;
}
//---------------------------------------------------------------------------
// -- ������������ Select �������
AnsiString __fastcall formAppNMPToPubSelectSQL(AnsiString p_id,TDateTime tempDate)
{
AnsiString sql;

  sql="select NMP_ID,NMP_NAME,nmp_price,stavka_nds,nmp_type,ret_must_flg,";
  sql+="sale_time,invoice_type,TEMP_Q,FACT_Q";
  sql+=" from GET_APPLICATION_FOR_NMP_TO_PUB(";
  sql+=p_id+",";
  sql+="'"+tempDate.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createAppNMPToPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *ng_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="NMP_TABLE,���������� ���,NMP_ID,NMP_NAME,,";

  fieldsInfo ="NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,������������ �������,Left,1,1,1,1,;";
  fieldsInfo+="NMP_PRICE,NMP_PRICE,����,Right,1,0,0,0,#0.00;";
  fieldsInfo+="STAVKA_NDS,STAVKA_NDS,������ ���,Center,1,0,0,0,#;";
  fieldsInfo+="NMP_TYPE,NMP_TYPE,��� �������,Center,1,0,0,0,;";
  fieldsInfo+="RET_MUST_FLG,RET_MUST_FLG,�������� ��������,Center,1,0,0,0,;";
  fieldsInfo+="SALE_TIME,SALE_TIME,���� ����������,Center,1,0,0,0,;";
  fieldsInfo+="INVOICE_TYPE,INVOICE_TYPE,��� ����������,Center,1,0,0,0,;";
  fieldsInfo+="FACT_Q,FACT_Q,����������|���������� ������,Center,1,1,1,0,#;";
  fieldsInfo+="TEMP_Q,TEMP_Q,����������|������� ������,Center,1,1,1,0,#";

  contentFieldsInfo ="NMP_PRICE,����;";
  contentFieldsInfo+="STAVKA_NDS,������ ���;";
  contentFieldsInfo+="NMP_TYPE,��� �������;";
  contentFieldsInfo+="RET_MUST_FLG,�������� ��������;";
  contentFieldsInfo+="SALE_TIME,���� ����������;";
  contentFieldsInfo+="INVOICE_TYPE,��� ����������";

  footerFieldsInfo ="FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="TEMP_Q,fvtSum,#,,Right";

  try
  {
    ng_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateAppNMPFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  ng_f->setEdit(SEdit);
  ng_f->setConditionSBox(ConditionSBox);
  ng_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  ng_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //�������������� ���������
  ng_f->getGridFrame()->additionalAdjustmentsForGrid=appToPubAdditionalAdjustments;
  appToPubAdditionalAdjustments(ng_f->getGridFrame());
  ng_f->gcHeader->ColorFrom=cl3DDkShadow;
  ng_f->gcHeader->ColorTo=clWhite;
  ng_f->gcHeader->Font->Color=clNavy;
  ng_f->getContentFrame()->contentHeader->ColorFrom=cl3DDkShadow;
  ng_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  ng_f->getContentFrame()->contentHeader->Font->Color=clNavy;
  //��������� ����
  ng_f->columnsConfig->Visible=true;
  return ng_f;
}
//---------------------------------------------------------------------------
void __fastcall appToPubAdditionalAdjustments(TGridFrame *gf)
{
int poz;

  poz=gf->findPozitionColumnInGrid("RET_MUST_FLG");
  if(poz!=0)
  { poz-=1;
    //����������� �������� ����������� ���� �������
    gf->workGrid->Columns->Items[poz]->KeyList->Clear();
    gf->workGrid->Columns->Items[poz]->PickList->Clear();
    gf->workGrid->Columns->Items[poz]->KeyList->Add("0");
    gf->workGrid->Columns->Items[poz]->PickList->Add("���");
    gf->workGrid->Columns->Items[poz]->KeyList->Add("1");
    gf->workGrid->Columns->Items[poz]->PickList->Add("��");
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

//������� ����������� ���������
// - �������� ������� ������������ ��������� � �������� ������ �� ��� �� ����� ������
void createMessageToPub(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *pBar,*nmpBar;
TDateTime tempDate;
AnsiString period;
bool flg=false;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  pBar=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->pBar;
  nmpBar=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->nmpBar;
  tempDate=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getTempDate();
  period=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->monthBox->Text;
  period+=" "+(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->yearEdit->Text;
  period+=" ����";
  AnsiString str;
  str="������ �� �������� �������-���������� ��������� �� "+period;
  sl->Add(str);
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  MPF->setSubject("������ �� �������� ���");
  if(MPF->ShowModal()==mrOk)
  { pBar->Visible=true;
    nmpBar->Visible=true;
    (static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->Repaint();
    flg=createMessagesForPublishers(owner,tempDate,period,MPF->getSubject(),MPF->getConten(),MPF->getReplyFLG(),MPF->getDelFilesFLG());
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
bool __fastcall createMessagesForPublishers(TComponent *owner,TDateTime date,AnsiString period,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG)
{
AnsiString dirName=getAppDir()+"Out\\ToPublishers\\";
unsigned short year, month, d;
AnsiString monthFolderID="";
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *lGrid;
TpFIBDataSet *lDS,*rDS;
TProgressBar *pBar;
AnsiString rName,rMail;
AnsiString fileOut="";
bool retFLG=false;
AnsiString mID;

  lDS=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getLDS();
  lGrid=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getLGrid();
  rDS=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRDS();
  pBar=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->pBar;

  date.DecodeDate(&year,&month,&d);
  //������� ������ ��������
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(year)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(month)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);

  //��������� ������� ������ ����� � ������� MESSAGES_FOLDERS_TABLE
  monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �����������","������ �� �������� ���",year,month);
  if(monthFolderID.IsEmpty()) return retFLG;
  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
  pBar->Max=lGrid->SelectedRows->Count;
  pBar->Position=0;
  for(int i=0; i<lGrid->SelectedRows->Count; ++i)
  { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
    if(rDS->RecordCount!=0)
    { //1.��������� Excel ����� � �������� ������
      fileOut=createApplicationsForNMPToPublisher(owner,date,period,dirName);
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      rName=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRicipientName();
      rMail=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRicipientMail();
      mID=addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
      delete fileNames;
    }
    pBar->StepIt();
  }
  if(!fileOut.IsEmpty() && !mID.IsEmpty()) retFLG=true;
  return retFLG;
}

//---------------------------------------------------------------------------
// - ������� ������������ ����� � �������� ������
AnsiString __fastcall createApplicationsForNMPToPublisher(TComponent *owner,TDateTime tempDate,AnsiString period,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn;
unsigned short y,m,d;
DirectWorkExcel *de;
FBDatabase *fbDB=getFBDataBase();
FBDataSet *fbDS;
AnsiString ricipientID;
AnsiString ricipientName;
TpFIBDataSet *lDS,*rDS;
TProgressBar *nmpBar;

  fileIn=getAppDir()+"Out\\ToPublishers\\Templates\\ApplForNMP.xls";
  lDS=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getLDS();
  nmpBar=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->nmpBar;
  ricipientID=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRicipientID();
  ricipientName=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRicipientName();
  tempDate.DecodeDate(&y,&m,&d);
  fileOut+="a_"+AnsiString().sprintf("%d-%02d_%02d",y,m,StrToInt(ricipientID))+".xls";

  //�������� ������
  if(templateCopy(fileIn,fileOut)==false) return "";
  //������� ���������� ��� ������ � Excel ������ � �������������� ��� ��� ������
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
    delete de;
    return "";
  }
  de->setSheetNumber(1);
  //������� ������ � Excel ����
  //  ���������
  AnsiString s="������ �� �������-���������� ��������� ��\n";
  s+=getConstValue(owner,fbDB,"ORGANIZATION_NAME",tempDate)+"\n";
  s+="������������ - "+ricipientName;
  s+="\n�� "+period;
  de->setCellValue(1,"A",s);
  //  ��������� �����
  //    ������� ��������������� DataSet
  try
  {
    fbDS=newFBDataSet(owner,"createTorg12File",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    delete de;
    return "";
  }
  //    ��������� ���
  AnsiString sql;
  sql="select nmp_name,temp_q from GET_APPLICATION_FOR_NMP_TO_PUB(";
  sql+=lDS->FieldByName("P_ID")->AsString+",";
  sql+="'"+tempDate.DateString()+"')";
  sql+=" order by NMP_NAME";
  if(fbDS->execSelectSQL("createApplicationsForNMPFromSD",sql)==false)
  { delete de;
    delete fbDS;
    return "";
  }
  rDS=fbDS->getDataSet();
  int ns=5;
  int npp=1;
  int quant=0;
  rDS->Last();
  nmpBar->Max=rDS->RecordCount;
  nmpBar->Position=0;
  rDS->First();
  while(!rDS->Eof)
  { de->insertRow(ns);
    de->copyRange(de->Cell_A1(ns+1,"A"),de->Cell_A1(ns+1,"I"));
    de->pasteRange(de->Cell_A1(ns,"A"));
    de->setCellValue(ns,"A",IntToStr(npp));
    de->setCellValue(ns,"C",rDS->FieldByName("NMP_NAME")->AsString);
    de->setCellValue(ns,"I",rDS->FieldByName("TEMP_Q")->AsString);
    quant+=rDS->FieldByName("TEMP_Q")->AsInteger;
    ns++; npp++;
    rDS->Next();
    nmpBar->StepIt();
    if((GetAsyncKeyState(VK_ESCAPE) << 1)!=0)
    { MessageDlg("���������� �������� ������?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0);
      if(MessageDlg("���������� �������� ������?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
      { //��������� ����
        delete fbDS;
        de->closeFile();
        delete de;
        return "";
      }
    }
  }
  delete fbDS;
  nmpBar->Position=0;
  //������
  de->deleteRow(ns);
  de->setCellValue(ns,"I",quant);
  de->setCellValue(ns+2,"A",getConstValue(owner,fbDB,"WORKMAN_JOB_TITLE",tempDate));
  de->setCellValue(ns+2,"G",getConstValue(owner,fbDB,"WORKMAN_FIO",tempDate));
  //��������� ����
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------

//����� ��� �������������� ������
AnsiString __fastcall formAppSelectSQL(AnsiString nmp_id,TDateTime tDate)
{
AnsiString sql;

  sql="select T_ID,SD_ID,SD_NAME,TEMP_Q,FACT_Q";
  sql+=" from GET_SUBDIVISIONS_FOR_APP(";
  sql+=nmp_id+",";
  sql+="'"+tDate.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
TGridFrame * __fastcall createAppFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame *af=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="TEMPLATE_TABLE,������� ��������,T_ID,T_ID,,";

  fieldsInfo= "T_ID,T_ID,��� ������,Center,1,1,1,0,;";
  fieldsInfo+="SD_NAME,SD_NAME,�������������,Left,1,1,1,0,;";
  fieldsInfo+="FACT_Q,FACT_Q,��������|���������� ������,Righr,1,1,0,0,;";
  fieldsInfo+="TEMP_Q,TEMP_Q,��������|������� ������,Righr,0,1,0,0,";

  footerFieldsInfo ="FACT_Q,fvtSum,#,,Right;";
  footerFieldsInfo+="TEMP_Q,fvtSum,#,,Right";

  try
  {
    af=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createAppFrame - ������ �������� TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(af!=NULL)
  { af->initFrame();
    af->Parent=workPanel;
    af->setColumnsConfigFieldEditInGridVisible(false);
  }
  //�������������� ���������
  af->additionalAdjustmentsForGrid=appAdditionalAdjustments;
  appAdditionalAdjustments(af);
  return af;
}
//---------------------------------------------------------------------------
void __fastcall appAdditionalAdjustments(TGridFrame *gf)
{
  int poz=gf->findPozitionColumnInGrid("TEMP_Q");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
}
//---------------------------------------------------------------------------

//������ �������� ���������
void __fastcall PrintTirVed(TComponent *owner,TDateTime tempDate)
{
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *rGrid;
TpFIBDataSet *rDS;
FBDataSet *fbDS=NULL;
TfrxReport *frxReport;
TfrxDBDataset *frxDS;
AnsiString sql;
AnsiString fName=getAppDir()+"Reports\\TirVedByNMPReport.fr3";
bool printFLG=false;

  rGrid=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRGrid();
  rDS=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->getRDS();
  if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
  frxReport=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->frxReport;
  frxDS=(static_cast<TFormApplicationsForNMPToPublishersFrame*>(owner))->frxDS;

  try
  {
    fbDS=newFBDataSet(owner,"PrintTirVed",fbDB->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }

  frxReport->Clear();
  if(frxReport->LoadFromFile(fName)==false)
  { MessageDlg("�� ��������� ���� � �������� ������ - "+fName,mtWarning,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  for(int i=0; i<rGrid->SelectedRows->Count; ++i)
  { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
    if(!rDS->FieldByName("TEMP_Q")->IsNull)
    { AnsiString nmpid=rDS->FieldByName("NMP_ID")->AsString;
      sql="select SD_ID,SD_NAME,TEMP_Q,TRACT";
      sql+=" from TIR_VED_BY_NMP(";
      sql+=nmpid+",";
      sql+="'"+tempDate.DateString()+"')";
      if(fbDS->execSelectSQL("PrintTirVed",sql)==false) goto end;
      frxDS->DataSet=fbDS->getDataSet();
      if(i==0) frxReport->PrepareReport(true);
      else frxReport->PrepareReport(false);
      printFLG=true;
    }
  }
  if(printFLG==true) frxReport->ShowPreparedReport();
end: if(fbDS!=NULL) delete fbDS;
}
//---------------------------------------------------------------------------

