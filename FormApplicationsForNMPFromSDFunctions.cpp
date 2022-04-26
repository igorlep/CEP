//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop
//#include "DifferentFunctions.h"
//#include "GridF.h"
//#include "Folder.h"
//#include "RBWorkFunctions.h"
#include "DirectWorkExcel.h"
#include "SomeFunctions.h"
#include "MessageParametersForm.h"
#include "ShipmentNMPFunctions.h"
#include "FormApplicationsForNMPFromSDF.h"
#include "FormApplicationsForNMPFromSDFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
// ����� �������������
// -- ������������ Select �������
AnsiString __fastcall formSubdivisionsSelectSQL(TDateTime dt)
{
AnsiString sql;

  sql="select sd_id,sd_name,sd_email,sd_type,po_id,po_name,sum_temp_q";
  sql+=" from GET_SD_FOR_APPLICATION(";
  sql+="'"+dt.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall cteateSubdivisionsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TNextGridFrame *sd_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="SUBDIVISIONS_TABLE,�������������,SD_ID,SD_NAME,,";

  fieldsInfo ="SD_ID,SD_ID,���,Center,1,1,1,0,;";
  fieldsInfo+="SD_NAME,SD_NAME,������������ �������������,Left,1,1,1,0,;";
  fieldsInfo+="SD_EMAIL,SD_EMAIL,����������� �����,Left,1,1,1,0,;";
  fieldsInfo+="SD_TYPE,SD_TYPE,��� �������������,Left,1,0,1,0,;";
  fieldsInfo+="PO_ID,PO_ID,��� ��������,Center,1,0,1,0,;";
  fieldsInfo+="PO_NAME,PO_NAME,������������ ��������,Left,1,0,1,0,;";
  fieldsInfo+="SUM_TEMP_Q,SUM_TEMP_Q,��������,Right,1,1,0,0,#";

  contentFieldsInfo ="SD_TYPE,��� �������������;";
  contentFieldsInfo+="PO_ID,��� ��������;";
  contentFieldsInfo+="PO_NAME,������������ ��������";

  footerFieldsInfo ="SUM_TEMP_Q,fvtSum,#,,Right";

  try
  {
    sd_f=new TNextGridFrame(Owner,iniFN,iniSN,fbDB,workPanel,tableInfo,fieldsInfo,contentFieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("cteateSubdivisionsFrame - ������ �������� TNextGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  sd_f->getGridFrame()->setColumnsConfigFieldEditInGridVisible(false);
  sd_f->getGridFrame()->setColumnsConfigFieldSearchAbleVisible(false);
  //�������������� ���������
  sd_f->gcHeader->ColorFrom=Graphics::clCream;
  sd_f->gcHeader->ColorTo=clWhite;
  sd_f->gcHeader->Font->Color=clOlive;
  sd_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clCream;
  sd_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  sd_f->getContentFrame()->contentHeader->Font->Color=clOlive;
  sd_f->getGridFrame()->additionalAdjustmentsForGrid=subdivisionsAdditionalAdjustments;
  subdivisionsAdditionalAdjustments(sd_f->getGridFrame());
  //��������� ����
  sd_f->columnsConfig->Visible=true;
  return sd_f;
}
//---------------------------------------------------------------------------
// ����� � ���������� ���
// -- ������������ Search �������
AnsiString __fastcall formAppNMPSearchSQL(void)
{
AnsiString sql;

  sql="select distinct n.NMP_ID,n.NMP_NAME";
  sql+=" from NMP_TABLE n";
  sql+=" left outer join PUBLISHERS_TABLE p on p.P_ID=n.P_ID";
  sql+=" where (p.del_flg is null or p.del_flg=0)";
  sql+=" and (n.del_flg is null or n.del_flg=0)";
  sql+=" and n.COMPRISE_OF_TEMPLATE=1";
  return sql;
}
//---------------------------------------------------------------------------
// -- ������������ Select �������
AnsiString __fastcall formAppNMPSelectSQL(AnsiString sd_id,TDateTime tempDate)
{
AnsiString sql;

  sql="select T_ID,TEMP_Q,P_ID,P_NAME,NMP_ID,NMP_NAME,FACT_Q";
  sql+=" from GET_APPLICATION_FOR_NMP_FROM_SD(";
  sql+=sd_id+",";
  sql+="'"+tempDate.DateString()+"')";
  return sql;
}
//---------------------------------------------------------------------------
TNextGridFrame * __fastcall createAppNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox)
{
TNextGridFrame *ng_f=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;
AnsiString contentFieldsInfo="";

  tableInfo="NMP_TABLE,���������� ���,NMP_ID,NMP_NAME,,";

  fieldsInfo ="T_ID,T_ID,��� �������,Center,1,0,0,0,;";
  fieldsInfo+="P_ID,P_ID,��� ����������,Center,1,0,0,0,;";
  fieldsInfo+="P_NAME,P_NAME,������������ ����������,Left,1,0,0,0,;";
  fieldsInfo+="NMP_ID,NMP_ID,���,Center,1,1,1,1,;";
  fieldsInfo+="NMP_NAME,NMP_NAME,������������ �������,Left,1,1,1,1,;";
  fieldsInfo+="FACT_Q,FACT_Q,����������|���������� ������,Center,1,1,1,0,#;";
  fieldsInfo+="TEMP_Q,TEMP_Q,����������|������� ������,Center,0,1,1,0,#";

  contentFieldsInfo ="P_ID,��� ����������;";
  contentFieldsInfo+="P_NAME,������������ ����������";

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
  ng_f->getGridFrame()->additionalAdjustmentsForGrid=appNMPAdditionalAdjustments;
  appNMPAdditionalAdjustments(ng_f->getGridFrame());
  ng_f->gcHeader->ColorFrom=Graphics::clCream;
  ng_f->gcHeader->ColorTo=clWhite;
  ng_f->gcHeader->Font->Color=clOlive;
  ng_f->getContentFrame()->contentHeader->ColorFrom=Graphics::clCream;
  ng_f->getContentFrame()->contentHeader->ColorTo=clWhite;
  ng_f->getContentFrame()->contentHeader->Font->Color=clOlive;
  //��������� ����
  ng_f->saveEdit->Visible=true;
  ng_f->cancelEdit->Visible=true;
  ng_f->separator4->Visible=true;
  ng_f->columnsConfig->Visible=true;

  ng_f->getGridFrame()->getWorkDataSet()->SQLs->DeleteSQL->Clear();
  ng_f->getGridFrame()->getWorkDataSet()->SQLs->UpdateSQL->Clear();
  ng_f->getGridFrame()->getWorkDataSet()->SQLs->InsertSQL->Clear();
  AnsiString sql="update TEMPLATE_TABLE set TEMP_Q=?TEMP_Q where T_ID=?OLD_T_ID";
  ng_f->getGridFrame()->getWorkDataSet()->SQLs->UpdateSQL->Add(sql);
  return ng_f;
}
//---------------------------------------------------------------------------
void __fastcall appNMPAdditionalAdjustments(TGridFrame *gf)
{
  int poz=gf->findPozitionColumnInGrid("TEMP_Q");
  if(poz>0)
  { poz-=1;
    gf->workGrid->Columns->Items[poz]->Font->Style=TFontStyles()<< fsBold;
  }
}
//---------------------------------------------------------------------------

//������� ����������� ���������
// - �������� ������� ������������ ��������� � �������� ������ ��� �� ����� ������
void createMessage(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *sdBar,*nmpBar;
TDateTime tempDate;
AnsiString period;
bool flg=true;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("������ �������� ������� TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  sdBar=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->sdBar;
  nmpBar=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->nmpBar;
  tempDate=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getTempDate();
  period=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->monthBox->Text;
  period+=" "+(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->yearEdit->Text;
  period+=" ����";
  AnsiString str;
  str="������ ������ �� �������� �������-���������� ��������� �� "+period;
  sl->Add(str);
  str="����� ��������� � ��������� � ����� �������� ����";
  sl->Add(str);
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  MPF->setSubject("������ ������ �� �������� ���");
  if(MPF->ShowModal()==mrOk)
  { sdBar->Visible=true;
    nmpBar->Visible=true;
    (static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->Repaint();
    flg=createMessagesForSubdivisions(owner,tempDate,period,MPF->getSubject(),MPF->getConten(),MPF->getReplyFLG(),MPF->getDelFilesFLG());
    sdBar->Visible=false;
    nmpBar->Visible=false;
    if(flg==true) MessageDlg("��������� ������������",mtInformation,TMsgDlgButtons() << mbOK,0);
    else MessageDlg("��� ������������ ��������� �������� ������",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
  delete sl;
  delete MPF;
}
//---------------------------------------------------------------------------
// - ������� ������������ ���������
bool __fastcall createMessagesForSubdivisions(TComponent *owner,TDateTime date,AnsiString period,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG)
{
AnsiString dirName=getAppDir()+"Out\\ToRaions\\";
unsigned short year, month, d;
AnsiString monthFolderID="";
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *lGrid;
TpFIBDataSet *lDS;
TProgressBar *sdBar;
AnsiString rName,rMail;
AnsiString fileOut="";
bool retFLG=false;
AnsiString mID;

  lDS=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getLDS();
  lGrid=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getLGrid();
  sdBar=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->sdBar;

  date.DecodeDate(&year,&month,&d);
  //������� ������ ��������
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(year)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(month)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);

  //��������� ������� ������ ����� � ������� MESSAGES_FOLDERS_TABLE
  monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","��� �������������","������� ������ �� �������� ���",year,month);
  if(monthFolderID.IsEmpty()) return retFLG;
  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
  sdBar->Max=lGrid->SelectedRows->Count;
  sdBar->Position=0;
  for(int i=0; i<lGrid->SelectedRows->Count; ++i)
  { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
    //1.��������� Excel ����� � �������� ������
    fileOut=createApplicationsForNMPFromSD(owner,date,period,dirName);
    TStringList *fileNames;
    fileNames=new TStringList;
    if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
    rName=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getRicipientName();
    rMail=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getRicipientMail();
    mID=addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
    delete fileNames;
    sdBar->StepIt();
  }
  if(!fileOut.IsEmpty() && !mID.IsEmpty()) retFLG=true;
  return retFLG;
}
//---------------------------------------------------------------------------
// - ������� ������������ ����� � �������� ������
AnsiString __fastcall createApplicationsForNMPFromSD(TComponent *owner,TDateTime tempDate,AnsiString period,AnsiString dirName)
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

  fileIn=getAppDir()+"Out\\ToRaions\\Templates\\Temp_2.xls";
  lDS=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getLDS();
  nmpBar=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->nmpBar;
  ricipientID=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getRicipientID();
  ricipientName=(static_cast<TFormApplicationsForNMPFromSDFrame*>(owner))->getRicipientName();
  tempDate.DecodeDate(&y,&m,&d);
  fileOut+="at_"+AnsiString().sprintf("%d-%02d_%02d",y,m,StrToInt(ricipientID))+".xls";

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
    return "";
  }
  de->setSheetNumber(1);
  //������� ������ � Excel ����
  //  ���������
  AnsiString s=lDS->FieldByName("SD_ID")->AsString;
  de->setCellValue(3,"A",s);
  s=lDS->FieldByName("SD_NAME")->AsString;
  de->setCellValue(3,"C",s);
  de->setCellValue(3,"D",period);
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
  sql="select T_ID,TEMP_Q,P_ID,P_NAME,NMP_ID,NMP_NAME,FACT_Q";
  sql+=" from GET_APPLICATION_FOR_NMP_FROM_SD(";
  sql+=lDS->FieldByName("SD_ID")->AsString+",";
  sql+="'"+tempDate.DateString()+"')";
  sql+=" order by P_NAME,NMP_NAME";
  if(fbDS->execSelectSQL("createApplicationsForNMPFromSD",sql)==false)
  { delete fbDS;
    delete de;
    return "";
  }
  rDS=fbDS->getDataSet();
  int sq=0,ns=6,npp=1,sn=1;
  AnsiString p_name="";
  rDS->Last();
  nmpBar->Max=rDS->RecordCount;
  nmpBar->Position=0;
  rDS->First();
  while(!rDS->Eof)
  { if(p_name!=rDS->FieldByName("P_NAME")->AsString)
    { p_name=rDS->FieldByName("P_NAME")->AsString;
      de->setMergeCells(de->Cell_A1(ns+npp,"A"),de->Cell_A1(ns+npp,"F"));
      TFont *font;
      font=new TFont();
      de->getFontCells(de->Cell_A1(ns+npp,"A"),de->Cell_A1(ns+npp,"F"),font);
      font->Style= TFontStyles() << fsBold;
      de->setFontCells(de->Cell_A1(ns+npp,"A"),de->Cell_A1(ns+npp,"F"),font);
      delete font;
      de->setCellValue(ns+npp,"A",p_name);
      npp++;
    }
    AnsiString nmp_id=rDS->FieldByName("NMP_ID")->AsString;
    AnsiString nmp_name=rDS->FieldByName("NMP_NAME")->AsString;
    int fact_q=rDS->FieldByName("FACT_Q")->AsInteger;
    AnsiString temp_q=rDS->FieldByName("TEMP_Q")->AsString;
    //��������� c�����
    de->setCellValue(ns+npp,"A",IntToStr(sn));
    de->setCellValue(ns+npp,"B",nmp_id);
    de->setCellValue(ns+npp,"C",nmp_name);
    if(fact_q > 0) de->setCellValue(ns+npp,"D",IntToStr(fact_q));
    else  de->setCellValue(ns+npp,"D","");
    if(!temp_q.IsEmpty()) de->setCellValue(ns+npp,"E",temp_q);
    else if(fact_q != 0) de->setCellValue(ns+npp,"E",IntToStr(fact_q));
    s="=����(�(E"+IntToStr(ns+npp)+">0;ABS(E"+IntToStr(ns+npp)+"-D"+IntToStr(ns+npp)+")>0);E"+IntToStr(ns+npp)+"-D"+IntToStr(ns+npp)+";\" \")";
    de->setCellValue(ns+npp,"F",s);
    de->setColorCells(de->Cell_A1(ns+npp,"F"),de->Cell_A1(ns+npp-1,"F"),15);
    sq+=fact_q;
    rDS->Next();
    npp++; sn++;
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
  de->setCellValue(ns+npp,"C","�����:");
  de->setCellValue(ns+npp,"D",IntToStr(sq));
  de->setCellValue(ns+npp,"E","=����(E"+IntToStr(ns+1)+":E"+IntToStr(ns+npp-1)+")");
  de->setCellValue(ns+npp,"F","=����(F"+IntToStr(ns+1)+":F"+IntToStr(ns+npp-1)+")");
  de->setBorderCells(de->Cell_A1(ns+1,"A"),de->Cell_A1(ns+npp-1,"F"));
  de->setColorCells(de->Cell_A1(ns+1,"A"),de->Cell_A1(ns+npp-1,"D"),15);
  de->setSheetLocked("MAR",true);

  //��������� ����
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------

