//---------------------------------------------------------------------------

#include <math.h>
#pragma hdrstop

#include "SecretWord.h"
#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "Folder.h"
#include "Message.h"
#include "RMessage.h"
#include "ShipmentNMPF.h"
#include "ReturnNMPfromSDF.h"
#include "MessageParametersForm.h"
#include "DirectWorkExcel.h"
#include "SomeFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBGridEh"
//---------------------------------------------------------------------------
//********************************************
//* Функции для обмена данными между формами *
//********************************************
FBDatabase * __fastcall getFBDataBase(void)
{
  return MainCAP_2010Form->getFBDatabase();
}
//---------------------------------------------------------------------------
PCUser * __fastcall getUser(void)
{
  return MainCAP_2010Form->getUser();
}
//---------------------------------------------------------------------------
TMainMenu * __fastcall getMainMenu(void)
{
  return MainCAP_2010Form->getMainMenu();
}
//---------------------------------------------------------------------------
TMainCAP_2010Form * __fastcall getMainForm(void)
{
  return MainCAP_2010Form;
}
//---------------------------------------------------------------------------
TStatusBar * __fastcall getMainStatusBar(void)
{
  return MainCAP_2010Form->getMainStatusBar();
}
//---------------------------------------------------------------------------
TFrame * __fastcall getMainWorkFrame(void)
{
  return MainCAP_2010Form->getWorkFrame();
}
//---------------------------------------------------------------------------
AnsiString __fastcall getAppDir(void)
{
  return MainCAP_2010Form->getAppDir();
}
//---------------------------------------------------------------------------
AnsiString __fastcall getUserWorkDir(void)
{
  return MainCAP_2010Form->getUserWorkDir();
}
//---------------------------------------------------------------------------
void __fastcall closeBase()
{
  MainCAP_2010Form->closeBase();
}
//---------------------------------------------------------------------------
void __fastcall openBase()
{
FBDatabase *fbdb=MainCAP_2010Form->getFBDatabase();
AnsiString userID=MainCAP_2010Form->getUser()->getUID();

  MainCAP_2010Form->openBase(fbdb,userID);
}
//---------------------------------------------------------------------------

//Функция, определяет дату возврата нереализованного издания из подразделения на склад
TDateTime __fastcall getReturnDate(TComponent* owner,FBDatabase *fbDB,TDateTime e_date,AnsiString nmp_id)
{
FBQuery *fbRQ=newFBQuery(owner,"getReturnDate",fbDB->getDatabase(),true);
AnsiString sql;

  if(fbRQ==NULL) return 0;

  //Выясняем срок реализации для издания
  sql="select SALE_TIME from NMP_TABLE";
  sql+=" where nmp_id="+nmp_id;
  if(fbRQ->execQuery("",sql)==false)
  { delete fbRQ;
    return 0;
  }
  int sale_time=0;
  if(fbRQ->getQuery()->RecordCount > 0 && !fbRQ->getQuery()->FieldByName("SALE_TIME")->IsNull) sale_time=fbRQ->getQuery()->FieldByName("SALE_TIME")->AsInteger;
  delete fbRQ;
  return e_date+sale_time;
}
//---------------------------------------------------------------------------
//Функция, определяет дату возврата нереализованного издания поставщику
TDateTime __fastcall getDateReturnToPublisher(TDateTime r_date)
{
unsigned short y,m,d;
TDateTime date_return;

  if(int(r_date)==0) return 0;
  r_date.DecodeDate(&y,&m,&d);
  return TDateTime(y,m,lastDayOfMonth(y,m));
}
//---------------------------------------------------------------------------

/*
TDateTime __fastcall getWriteOffDate(TComponent* owner,FBDatabase *fbDB,TDateTime fact_date,AnsiString e_id)
{
FBQuery *fbRQ=newFBQuery(owner,"getWriteOffDate",fbDB->getDatabase(),true);
AnsiString sql;

  if(fbRQ==NULL) return 0;

  //Выясняем срок реализации для издания
  sql="select n.SALE_TIME from EXITS_TABLE e";
  sql+=" left outer join NMP_TABLE n on n.NMP_ID=e.NMP_ID";
  sql+=" where e.e_id="+e_id;
  if(fbRQ->execQuery("",sql)==false)
  { delete fbRQ;
    return 0;
  }
  int sale_time=0;
  if(fbRQ->getQuery()->RecordCount > 0 && !fbRQ->getQuery()->FieldByName("SALE_TIME")->IsNull) sale_time=fbRQ->getQuery()->FieldByName("SALE_TIME")->AsInteger;
  delete fbRQ;
  return fact_date+sale_time;
}
//---------------------------------------------------------------------------
*/
//Возвращает 3-ю статусную панель гланого окна
TStatusPanel * __fastcall getStatusPanelMainWindow(void)
{
  return getMainStatusBar()->Panels->Items[3];
}
//---------------------------------------------------------------------------
//Выводит текст в 3-ю StatusPanel MainStatusBar
void __fastcall set3MainStatusBar(AnsiString text)
{
  getMainStatusBar()->Panels->Items[3]->Text=text;
}
//---------------------------------------------------------------------------

//Функции для работы с таблицами foldersTableName и messagesTableName
// - функция удаления записи из таблицы foldersTableName
bool __fastcall removeFolder(TComponent *Owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString messagesTableName,AnsiString folderID)
{
FBQuery *fbRQ=NULL;
TpFIBQuery *RQ;
AnsiString sql;
bool flg=false;
Folder *folder=newFolder(Owner,db,foldersTableName);

  if(folder==NULL) goto ret;
  fbRQ=newFBQuery(Owner,"removeFolder",db,false);
  if(fbRQ==NULL) goto ret;
  RQ=fbRQ->getQuery();

  //Ищем ссылки на удаляемую папку в таблице foldersTableName
  sql="select count(FOLDER_ID) as COUNT_RECORD from "+foldersTableName;
  sql+=" where PARENT_ID='"+folderID+"'";
  if(fbRQ->execQuery("removeFolder",sql)==false) goto ret;
  if(RQ->FieldByName("COUNT_RECORD")->AsInteger > 0)
  { MessageDlg("Имеются ссылки на папку в таблице "+foldersTableName+".\nУдаление невозможно.",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto ret;
  }
  //Ищем ссылки на удаляемую папку в таблице messagesTableName
  sql="select count(M_ID) as COUNT_RECORD from "+messagesTableName;
  sql+=" where FOLDER_ID='"+folderID+"'";
  if(fbRQ->execQuery("removeFolder",sql)==false) goto ret;
  if(RQ->FieldByName("COUNT_RECORD")->AsInteger > 0)
  { MessageDlg("Имеются ссылки на папку в таблице "+messagesTableName+".\nУдаление невозможно.",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto ret;
  }
  if(folder->removeFolder(folderID)==false) goto ret;
  flg=true;
ret:
  if(fbRQ!=NULL) delete fbRQ;
  if(folder!=NULL) delete folder;
  return flg;
}
//---------------------------------------------------------------------------
//Функция проверяет наличие папки по полному наименованию папки
//Если запись отсутствует, то, если параметр createRecord == true, создает запись
//Возвращает код записи
AnsiString __fastcall checkFolderInMessagesTable(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString fullFolderName,bool createRecord)
{
AnsiString folderID="",fID;
Folder *folder=newFolder(owner,db,foldersTableName);

  if(folder==NULL) goto ret;
  fID=folder->getFolderID(fullFolderName);
  if(fID.IsEmpty())
  { //Создаем папку
    int n=1;
    AnsiString pID=folder->getRootFolderID(),name;
    while((name=getSubstring(fullFolderName,'\\',n))!=NULL)
    { pID=folder->checkRecordInFoldersTable(pID,name,createRecord);
      if(pID.IsEmpty()) break;
      n++;
    }
  }
  folderID=fID;
ret:
  if(folder!=NULL) delete folder;
  return folderID;
}
//---------------------------------------------------------------------------
//Функция проверяет и если нет, то создает, папки с номером года и месяца
// Если folderName_2 не пусто, то папки создаются внутри folderName_2
// иначе внутри folderName_1
// Возвращает идентификатор папки с номером месяца
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderName_1,AnsiString folderName_2,int year,int month)
{
AnsiString fullFolderName;

 fullFolderName=folderName_1;
 if(folderName_2!=NULL && !folderName_2.IsEmpty()) fullFolderName+="\\"+folderName_2;
 return checkYearAndMonthFolders(owner,db,foldersTableName,fullFolderName,year,month);
}
//---------------------------------------------------------------------------
//Функция проверяет и если нет, то создаем, папки 3-го и 4-го уровня с номером года и месяца
AnsiString checkYearAndMonthFolders(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString fullFolderName,int year,int month)
{
AnsiString folderID=" ",fID;
Folder *folder=newFolder(owner,db,foldersTableName);

  if(folder==NULL) goto ret;
  fID=folder->getFolderID(fullFolderName);
  //Проверяем наличие папки с номером года
  fID=folder->checkRecordInFoldersTable(fID,IntToStr(year),true);
  if(fID.IsEmpty())
  { MessageDlg("Отсутствует папка \""+IntToStr(year)+"\" в таблице "+foldersTableName,mtWarning,TMsgDlgButtons() << mbOK,0);
    goto ret;
  }
  //Проверяем наличие папки с номером месяца
  fID=folder->checkRecordInFoldersTable(fID,IntToStr(month),true);
  if(fID.IsEmpty())
  { MessageDlg("Отсутствует папка \""+IntToStr(month)+"\" в таблице "+foldersTableName,mtWarning,TMsgDlgButtons() << mbOK,0);
    goto ret;
  }
  folderID=fID;
ret:
  if(folder!=NULL) delete folder;
  return folderID;
}
//---------------------------------------------------------------------------
//Функци возвращает полный путь папки
AnsiString getFullNameFilder(TComponent *owner,TpFIBDatabase *db,AnsiString foldersTableName,AnsiString folderID)
{
AnsiString folderName="";
Folder *folder=newFolder(owner,db,foldersTableName);

  if(folder==NULL) goto ret;;
  folderName=folder->getFolderFullName(folderID);
ret:
  if(folder!=NULL) delete folder;
  return folderName;
}
//---------------------------------------------------------------------------

// Функции создания сообщений
// - функция создания записи в таблице MESSAGES_TABLE об отправке сообщения
AnsiString __fastcall addMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString rName,AnsiString rMail,AnsiString subject,AnsiString content,TStringList *attachedFiles,bool reply,bool delFiles)
{
Message *mes;
AnsiString mID="";

  try
  {
    mes=new Message(owner,getAppDir(),fbDB);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта Message\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return mID;
  }
  mes->setSMTPHost(getConstValue(owner,fbDB,"SMTP_HOST"));
  mes->setSMTPUserID(getConstValue(owner,fbDB,"SMTP_USER_ID"));
  AnsiString pass=getConstValue(owner,fbDB,"SMTP_PASSWORD");
  SecretWord sw; pass=sw.decodeWord(pass);
  mes->setSMTPPassword(pass);
  mes->setSendUserAddress(getUser()->getUMail());
//  mes->setSendUserAddress(getConstValue(owner,fbDB,"SEND_USER_ADDRESS"));
  mes->setFolderID(folderID);
  mes->setUserID(getUser()->getUID());
  mes->setUserName(getUser()->getULogin());
  mes->setUserMail(getUser()->getUMail());
  mes->setRecipientName(rName);
  mes->setRecipientAddress(rMail);
  mes->setSubject(subject);
  mes->setContents(content);
  mes->setAttachedFilesList(attachedFiles);
  mes->setReplyFLG(reply);
  mes->setDelAttachedFilesFLG(delFiles);
  if(mes->WriteToBase()==true) mID=mes->getM_ID();
  delete mes;
  return mID;
}
//---------------------------------------------------------------------------
// - функция создания записи в таблице RECEIVED_MESSAGES_TABLE о получении сообщении
/*
AnsiString __fastcall addRMessageToBase(TComponent *owner,FBDatabase *fbDB,AnsiString folderID,AnsiString sMail,AnsiString subject,AnsiString content,TStringList *attachedFiles,bool delOnRead)
{
RMessage *mes;
AnsiString mID="";

  try
  {
    mes=new RMessage(owner,getAppDir(),fbDB,getConstValue(owner,fbDB,"POP3_HOST",0));
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта RMessage\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return mID;
  }
  mes->setFolderID(folderID);
  mes->setSenderAddress(sMail);
  mes->setRSubject(subject);
  mes->setRContents(content);
  mes->setAttachedFilesList(attachedFiles);
  mes->setPOP3DeleteOnRead(delOnRead);
  mes->setUserID(getUser()->getUID());
  mes->setUserName(getUser()->getULogin());
  if(mes->WriteToBase()==true) mID=mes->getM_ID();
  delete mes;
  return mID;
}
//---------------------------------------------------------------------------
*/

//Функция копирования шаблона
bool __fastcall templateCopy(AnsiString fileIn, AnsiString fileOut)
{
int fin, fout;

  fin=FileOpen(fileIn,fmOpenRead);
  if(fin < 0)
  { MessageDlg("Ошибка открытия файла - "+fileIn,mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  if(FileExists(fileOut)) DeleteFile(fileOut);
  fout=FileCreate(fileOut);
  if(fout < 0)
  { MessageDlg("Ошибка копирования шаблона файла "+fileOut,mtError,TMsgDlgButtons() << mbOK,0);
    FileClose(fin);
    return false;
  }
  int len=FileSeek(fin,0,2);
  FileSeek(fin,0,0);
  char *buf;
  buf=new char[len+1];
  int readLen=FileRead(fin,buf,len);
  FileClose(fin);
  FileWrite(fout,buf,readLen);
  FileClose(fout);
  delete []buf;
  return true;
}
//---------------------------------------------------------------------------

//Функции для работы с накладной Торг-13
// - печать накладной
void __fastcall printTorg13(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *lDS,TpFIBDataSet *rDS,int type,int kind)
{
TfrxReport *frxReport;
TfrxDBDataset * frxDS;
TfrxRTFExport * frxRTFExport;
TfrxXLSExport * frxXLSExport;
AnsiString workDir=getAppDir();
TDBGridEh *lGrid,*rGrid;
TDateTime date1, date2;
AnsiString sd_id;

  if(rDS->RecordCount==0) return;

  if(type==1) //накладная на отгрузку ГЖП со склада в почтамты
  { frxReport=(static_cast<TShipmentNMPFrame*>(owner))->frxReport;
    frxDS=(static_cast<TShipmentNMPFrame*>(owner))->frxDS;
    frxXLSExport=(static_cast<TShipmentNMPFrame*>(owner))->frxXLSExport;
    lGrid=(static_cast<TShipmentNMPFrame*>(owner))->getLGrid();
    rGrid=(static_cast<TShipmentNMPFrame*>(owner))->getRGrid();
    if(frxReport->LoadFromFile(workDir+"Reports\\torg13.fr3")==false)
    { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\Torg13.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
    if(kind==1) //полная накладная
    { //(static_cast<TShipmentNMPFrame*>(owner))->setFullPrintFLG(true);
      frxXLSExport->FileName="Накладная - Торг 13.xls";
    }
    else //выборочная накладная
    { //(static_cast<TShipmentNMPFrame*>(owner))->setFullPrintFLG(false);
      frxXLSExport->FileName="Накладная - Торг 13 (выборочно).xls";
      date1=(static_cast<TShipmentNMPFrame*>(owner))->getDate1();
      date2=(static_cast<TShipmentNMPFrame*>(owner))->getDate2();
      sd_id=(static_cast<TShipmentNMPFrame*>(owner))->getRicipientID();
    }
  }
  else //накладная на возврат ГЖП из почтамтов на склад
  { frxReport=(static_cast<TReturnNMPfromSDFrame*>(owner))->frxReport;
    frxDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->frxDS;
    frxXLSExport=(static_cast<TReturnNMPfromSDFrame*>(owner))->frxXLSExport;
    lGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getLGrid();
    rGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRGrid();
    if(frxReport->LoadFromFile(workDir+"Reports\\torg13-vozvrat.fr3")==false)
    { MessageDlg("Не могу загрузить файл с отчетом: "+workDir+"Reports\\Torg13-vozvrat.fr3",mtWarning,TMsgDlgButtons() << mbOK,0);
      return;
    }
    if(kind==1) //полная накладная
    { (static_cast<TReturnNMPfromSDFrame*>(owner))->setFullPrintFLG(true);
      frxXLSExport->FileName="Накладная - Торг 13 возврат.xls";
    }
    else
    { (static_cast<TReturnNMPfromSDFrame*>(owner))->setFullPrintFLG(false);
      frxXLSExport->FileName="Накладная - Торг 13 возврат (выборочно).xls";
      date1=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate1();
      date2=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
      sd_id=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientID();
    }
  }

  if(kind==1)
  { frxDS->DataSet=rDS;
    rGrid->Visible=false;
    if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
    for(int i=0; i<lGrid->SelectedRows->Count; ++i)
    { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
      if(i==0) frxReport->PrepareReport(true);
      else frxReport->PrepareReport(false);
    }
    rGrid->Visible=true;
    frxReport->ShowPreparedReport();
  }
  else
  { FBDataSet *fbDS=NULL;
    AnsiString sql;
    try
    {
      fbDS=newFBDataSet(owner,"printTorg13",db,true);
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка создания объекта FBDataSet.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
    if(type==1)
    { sql="select n.nmp_name||', №'||e.e_number as nmp_name,n.nmp_id,e.e_price,";
      sql+="e.nds_stavka,";
      sql+="sum(f.FACT_Q) as quant,";
      sql+="sum(f.FACT_Q*e.e_price) as summa";
      sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p,";
      sql+="subdivisions_table s";
      if((static_cast<TShipmentNMPFrame*>(owner))->groupOnPostOffices->Checked==true)
        sql+=",POST_OFFICES_TABLE po";
      sql+=" where e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
      sql+=" and s.sd_id=f.sd_id and ";
      if((static_cast<TShipmentNMPFrame*>(owner))->groupOnPostOffices->Checked==false)
        sql+="s.sd_id="+sd_id;
      else
        sql+="po.po_id="+sd_id;
      sql+=" and e.E_DATE between '"+date1.DateString()+"'";
      sql+=" and '"+date2.DateString()+"' and (";
      if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
      for(int i=0; i<rGrid->SelectedRows->Count; ++i)
      { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
        if(i>0) sql+=" or ";
        sql+="e.E_ID="+rDS->FieldByName("E_ID")->AsString;
      }
      sql+=") group by n.nmp_name,e.e_number,n.nmp_id,e.nds_stavka,e.e_price";
      sql+=" order by n.nmp_name,e.e_number";
    }
    else
    { sql="select n.nmp_name||', №'||e.e_number as nmp_name,n.nmp_id,e.e_price,";
      sql+="e.nds_stavka,";
      sql+="sum(f.RETURN_Q) as RETURN_Q,";
      sql+="sum(f.RETURN_Q*e.e_price) as return_summa";
      sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p,";
      sql+="subdivisions_table s";
      if((static_cast<TShipmentNMPFrame*>(owner))->groupOnPostOffices->Checked==true)
        sql+=",POST_OFFICES_TABLE po";
      sql+=" where e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
      sql+=" and s.sd_id=f.sd_id and ";
      if((static_cast<TShipmentNMPFrame*>(owner))->groupOnPostOffices->Checked==false)
        sql+="s.sd_id="+sd_id;
      else
        sql+="po.po_id="+sd_id;
      sql+=" and e.R_DATE between '"+date1.DateString()+"'";
      sql+=" and '"+date2.DateString()+"' and (";
      if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
      for(int i=0; i<rGrid->SelectedRows->Count; ++i)
      { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
        if(i>0) sql+=" or ";
        sql+="e.E_ID="+rDS->FieldByName("E_ID")->AsString;
      }
      sql+=") group by n.nmp_name,e.e_number,n.nmp_id,e.nds_stavka,e.e_price";
      sql+=" order by n.nmp_name,e.e_number";
    }
    if(fbDS->execSelectSQL("printTorg13",sql)==false)
    { delete fbDS;
      return;
    }
    frxDS->DataSet=fbDS->getDataSet();
    frxReport->ShowReport(true);
    if(fbDS!=NULL) delete fbDS;
  }
}
//---------------------------------------------------------------------------
// - головная функция формирования сообщений с накладной Торг-13
void createTorg13Message(TComponent *owner,AnsiString iniFileName,int type,int kind)
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *sdBar,*nmpBar;
TDateTime date1, date2;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  if(type==1) //накладная на отгрузку ГЖП
  { sdBar=(static_cast<TShipmentNMPFrame*>(owner))->sdBar;
    nmpBar=(static_cast<TShipmentNMPFrame*>(owner))->nmpBar;
    date1=(static_cast<TShipmentNMPFrame*>(owner))->getDate1();
    date2=(static_cast<TShipmentNMPFrame*>(owner))->getDate2();
    if(kind==1) //полная накладная
    { sl->Add("Накладная на отгрузку газетно-журнальной продукции за период с "+date1.DateString()+" по "+date2.DateString());
      MPF->setSubject("Накладная \"Торг-13\" на отрузку ГЖП");
    }
    else //выборочная накладная
    { sl->Add("Дополнительная накладная на отгрузку газетно-журнальной продукции за период с "+date1.DateString()+" по "+date2.DateString());
      MPF->setSubject("Накладная \"Торг-13\" на отрузку ГЖП (выборочно)");
    }
  }
  else  //накладная на возврат ГЖП
  { sdBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->sdBar;
    nmpBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->nmpBar;
    date1=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate1();
    date2=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
    if(kind==1) //полная накладная
    { sl->Add("Накладная на возврат газетно-журнальной продукции за период с "+date1.DateString()+" по "+date2.DateString());
      MPF->setSubject("Накладная \"Торг-13\" на возврат ГЖП");
    }
    else //выборочная накладная
    { sl->Add("Дополнительная накладная на возврат газетно-журнальной продукции за период с "+date1.DateString()+" по "+date2.DateString());
      MPF->setSubject("Накладная \"Торг-13\" на возврат ГЖП (выборочно)");
    }
  }
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { sdBar->Visible=true;
    nmpBar->Visible=true;
    (static_cast<TShipmentNMPFrame*>(owner))->Repaint();
    bool flg=createTorg13MessagesForSubdivisions(owner,type,kind,MPF->getSubject(),MPF->getConten(),MPF->getReplyFLG(),MPF->getDelFilesFLG());
    sdBar->Visible=false;
    nmpBar->Visible=false;
    if(flg==true) MessageDlg("Сообщения сформированы",mtInformation,TMsgDlgButtons() << mbOK,0);
    else MessageDlg("При формировании сообщений возникли ошибки",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
  delete sl;
  delete MPF;
}
//---------------------------------------------------------------------------
// - функция формирования сообщений
bool __fastcall createTorg13MessagesForSubdivisions(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG)
{
AnsiString dirName=getAppDir()+"Out\\ToRaions\\";
unsigned short year, month, d;
AnsiString monthFolderID;
TDateTime date;
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *lGrid;//,*rGrid;
TpFIBDataSet *lDS;//,*rDS;
TProgressBar *sdBar;//,*nmpBar;
AnsiString rName,rMail;

  if(type==1)
  { date=(static_cast<TShipmentNMPFrame*>(owner))->getDate2();
    lDS=(static_cast<TShipmentNMPFrame*>(owner))->getLDS();
    lGrid=(static_cast<TShipmentNMPFrame*>(owner))->getLGrid();
    sdBar=(static_cast<TShipmentNMPFrame*>(owner))->sdBar;
    rName=(static_cast<TShipmentNMPFrame*>(owner))->getRicipientName();
    rMail=(static_cast<TShipmentNMPFrame*>(owner))->getRicipientMail();
  }
  else
  { date=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
    lDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->getLDS();
//    rDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRDS();
    lGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getLGrid();
//    rGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRGrid();
    sdBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->sdBar;
//    nmpBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->nmpBar;
    rName=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientName();
    rMail=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientMail();
  }
  date.DecodeDate(&year,&month,&d);
  //Создаем нужные каталоги
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(year)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);
  dirName+=IntToStr(month)+"\\";
  if(!DirectoryExists(dirName)) CreateDir(dirName);

  //Проверяем наличие нужных папок в таблице MESSAGES_FOLDERS_TABLE
  if(type==1)
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","Для подразделений","Накладные \"Торг-13\" - поступление",year,month);
  else
    monthFolderID=checkYearAndMonthFolders(owner,fbDB->getDatabase(),"MESSAGES_FOLDERS_TABLE","Для подразделений","Накладные \"Торг-13\" - возврат",year,month);
  if(monthFolderID.IsEmpty()) return false;

  if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
  sdBar->Max=lGrid->SelectedRows->Count;
  sdBar->Position=0;
  if(kind==1) //Сообщения с накладной Торг-13
  { for(int i=0; i<lGrid->SelectedRows->Count; ++i)
    { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
      //1.Формируем Excel файла с накладной Торг 13
      AnsiString fileOut=createTorg13File(owner,type,kind,dirName);
      //2.Создаем сообщения для отправки в подразделение с прикрепленным файлом
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
      delete fileNames;
      sdBar->StepIt();
    }
  }
  else  //Сообщение с накладной Торг-13 (выборочно)
  { //1.Формируем Excel файла с накладной Торг 13
    AnsiString fileOut=createTorg13File(owner,type,kind,dirName);
    //2.Создаем сообщения для отправки в подразделение с прикрепленным файлом
    TStringList *fileNames;
    fileNames=new TStringList;
    if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
    addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
    delete fileNames;
    sdBar->StepIt();
  }
  sdBar->Position=0;
  return true;
}
//---------------------------------------------------------------------------
// - функция формирования файла с накладной Торг-13
AnsiString __fastcall createTorg13File(TComponent *owner,int type,int kind,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn=getAppDir()+"Out\\ToRaions\\Templates\\Torg_13n.xls";
unsigned short y,m,d;
DirectWorkExcel *de;
TDateTime date1,date2;
FBDatabase *fbDB=getFBDataBase();
AnsiString ricipientID;
AnsiString ricipientName;
TpFIBDataSet *rDS;
TDBGridEh *rGrid;
TProgressBar *nmpBar;

  if(type==1)
  { rDS=(static_cast<TShipmentNMPFrame*>(owner))->getRDS();
    rGrid=(static_cast<TShipmentNMPFrame*>(owner))->getRGrid();
    nmpBar=(static_cast<TShipmentNMPFrame*>(owner))->nmpBar;
    date1=(static_cast<TShipmentNMPFrame*>(owner))->getDate1();
    date2=(static_cast<TShipmentNMPFrame*>(owner))->getDate2();
    ricipientID=(static_cast<TShipmentNMPFrame*>(owner))->getRicipientID();
    ricipientName=(static_cast<TShipmentNMPFrame*>(owner))->getRicipientName();
  }
  else
  { rDS=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRDS();
    rGrid=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRGrid();
    nmpBar=(static_cast<TReturnNMPfromSDFrame*>(owner))->nmpBar;
    date1=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate1();
    date2=(static_cast<TReturnNMPfromSDFrame*>(owner))->getDate2();
    ricipientID=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientID();
    ricipientName=(static_cast<TReturnNMPfromSDFrame*>(owner))->getRicipientName();
  }
  date2.DecodeDate(&y,&m,&d);
  if(type==1)
    fileOut+="no";
  else
    fileOut+="nv";
  fileOut+="13_"+AnsiString().sprintf("%d-%02d_%02d",y,m,StrToInt(ricipientID))+".xls";
  //Копируем шаблон
  if(templateCopy(fileIn,fileOut)== false) return "";

  //Создаем Excel файл
  try
  {
    de=new DirectWorkExcel(fileOut,1);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта DirectWorkExcel\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  if(de->openFile(false)==false)
  { MessageDlg("Ошибка открытия Excel файла",mtWarning,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  de->setSheetNumber(1);

  //Заносим данные в Excel файл
  //  заголовок
  AnsiString s=getConstValue(owner,fbDB,"ORGANIZATION_NAME",date2);
  s+=getConstValue(owner,fbDB,"ORGANIZATION_ADDRESS",date2);
  de->setCellValue(5,"A",s);
  if(type==1) s="от_";
  else s="воз_";
  s+=ricipientID+"_"+AnsiString().sprintf("%02d-%d",m,y);
  if(kind==2) s+=" - доп.";
  de->setCellValue(13,"K",s);
  s=getConstValue(owner,fbDB,"SS_NAME",date2);
  if(type==1)
  { de->setCellValue(13,"S",date2.DateString());
    de->setCellValue(20,"A",s);
    de->setCellValue(20,"H",ricipientName);
  }
  else
  { unsigned short y1,m1,d1;
    date2.DecodeDate(&y1,&m1,&d1);
    m1++; if(m1>12){ m1=1; y1++;}
    de->setCellValue(13,"S",TDateTime(y1,m1,10).DateString());
    de->setCellValue(20,"A",ricipientName);
    de->setCellValue(20,"H",s);
  }
  //  данные
  int ns=26;
  int quant=0;
  double summa=0.0;
  if(kind==1)
  { nmpBar->Max=rDS->RecordCount;
    nmpBar->Position=0;
    rDS->First();
    while(!rDS->Eof)
    { putStr(type,ns,de,rDS);
      if(type==1)
      { quant+=rDS->FieldByName("QUANT")->AsInteger;
        summa+=rDS->FieldByName("SUMMA")->AsFloat;
      }
      else
      { quant+=rDS->FieldByName("RETURN_Q")->AsInteger;
        summa+=rDS->FieldByName("RETURN_SUMMA")->AsFloat;
      }
      ns++;
      rDS->Next();
      nmpBar->StepIt();
    }
  }
  else
  { if(rGrid->SelectedRows->Count==0) rGrid->Selection->Rows->CurrentRowSelected=true;
    nmpBar->Max=rGrid->SelectedRows->Count;
    nmpBar->Position=0;
    for(int i=0; i<rGrid->SelectedRows->Count; ++i)
    { rDS->GotoBookmark((void *)rGrid->SelectedRows->Items[i].c_str());
      putStr(type,ns,de,rDS);
      if(type==1)
      { quant+=rDS->FieldByName("QUANT")->AsInteger;
        summa+=rDS->FieldByName("SUMMA")->AsFloat;
      }
      else
      { quant+=rDS->FieldByName("RETURN_Q")->AsInteger;
        summa+=rDS->FieldByName("RETURN_SUMMA")->AsFloat;
      }
      ns++;
      nmpBar->StepIt();
    }
  }
  nmpBar->Position=0;
  // подвал
  de->deleteRow(ns);
  de->setCellValue(ns,"R",IntToStr(quant));
  de->setCellValue(ns,"AD",AnsiString().sprintf("%.02lf",summa));
  AnsiString molJT=getConstValue(owner,fbDB,"MOL_JOB_TITLE",date2);
  AnsiString molFIO=getConstValue(owner,fbDB,"MOL_FIO",date2);
  if(type==1)
  { de->setCellValue(ns+2,"B",molJT);
    de->setCellValue(ns+2,"P",molFIO);
  }
  else
  { de->setCellValue(ns+9,"B",molJT);
    de->setCellValue(ns+9,"P",molFIO);
  }
  de->setCellValue(ns+7,"A",f_to_str(floor(summa*100+0.5)/100));

  //Закрываем файл
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------
// - функция вывода строки в Excel файл с накладной Торг-13
void __fastcall putStr(int type,int ns, DirectWorkExcel *de, TDataSet *DS)
{
  de->insertRow(ns);
  de->copyRange("A"+IntToStr(ns+1),"AG"+IntToStr(ns+1));
  de->pasteRange("A"+IntToStr(ns));
  de->setCellValue(ns,"A",DS->FieldByName("NMP_NAME")->AsString);
  de->setCellValue(ns,"D",DS->FieldByName("NMP_ID")->AsString);
  if(type==1)
  { de->setCellValue(ns,"R",DS->FieldByName("QUANT")->AsString);
    de->setCellValue(ns,"AD",DS->FieldByName("SUMMA")->AsString);
  }
  else
  { de->setCellValue(ns,"R",DS->FieldByName("RETURN_Q")->AsString);
    de->setCellValue(ns,"AD",DS->FieldByName("RETURN_SUMMA")->AsString);
  }
  de->setCellValue(ns,"AA",DS->FieldByName("E_PRICE")->AsString);
  de->setCellValue(ns,"AG",DS->FieldByName("NDS_STAVKA")->AsString);
}
//---------------------------------------------------------------------------

//функции для работы с набором прав пользователя
// - проверка права ползователя на пункт меню по коду меню
CheckStyle __fastcall checkUserRightForMenuItem(TComponent *owner,FBDatabase *fbDB,AnsiString uID,AnsiString miID)
{
FBQuery *fbRQ;
AnsiString sql;
CheckStyle cs=csUnknown;

  try
  {
    fbRQ=new FBQuery(owner,fbDB->getDatabase(),true);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return cs;
  }
  sql="select CHECKED_STYLE from USERS_RIGHTS_TABLE";
  sql+=" where USER_ID="+uID;
  sql+=" and R_ID='"+miID+"'";
  if(fbRQ->execQuery("checkUserRightForMenuItem",sql)==true)
    cs=CheckStyle(fbRQ->getQuery()->FieldByName("CHECKED_STYLE")->AsInteger);
  delete fbRQ;
  return cs;
}
//---------------------------------------------------------------------------
// - проверка права ползователя на пункт меню по имени меню
CheckStyle __fastcall checkUserRightForMenuName(TComponent *owner,FBDatabase *fbDB,AnsiString uID,AnsiString miName)
{
FBQuery *fbRQ;
AnsiString sql;
CheckStyle cs=csUnknown;

  try
  {
    fbRQ=new FBQuery(owner,fbDB->getDatabase(),true);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта FBQuery\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return cs;
  }
  sql="select CHECKED_STYLE from USERS_RIGHTS_TABLE";
  sql+=" where USER_ID="+uID;
  sql+=" and MENU_NAME='"+miName+"'";
  if(fbRQ->execQuery("checkUserRightForMenuItem",sql)==true)
    cs=CheckStyle(fbRQ->getQuery()->FieldByName("CHECKED_STYLE")->AsInteger);
  delete fbRQ;
  return cs;
}
//---------------------------------------------------------------------------

