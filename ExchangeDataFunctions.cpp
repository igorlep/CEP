//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop

#include "DBClasses.h"
#include "GridF.h"
#include "SomeFunctions.h"
#include "DirectWorkExcel.h"
#include "MessageParametersForm.h"
#include "SecretWord.h"
#include "UnloadDataToSDF.h"
#include "ExchangeDataF.h"
#include "ExchangeDataFunctions.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//**********************************
//* Фрейм для выборв подразделений *
//**********************************
// - функция формирования select sql запроса
AnsiString __fastcall formED_SDSelectSQL(void)
{
AnsiString sql="";

  sql ="select s.SD_ID,s.SD_NAME,SD_EMAIL from SUBDIVISIONS_TABLE s";
  sql+=" where PROG_FLG=1";

  return sql;
}
//---------------------------------------------------------------------------
// - функция создания фрейма
class TGridFrame * __fastcall createED_SDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* gf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
//AnsiString footerFieldsInfo;

  tableInfo="SUBDIVISIONS_TABLE,Список подразделений,SD_ID,SD_NAME,,";

  fieldsInfo ="s.SD_ID,SD_ID,Код,Center,1,1,1,0,;";
  fieldsInfo+="s.SD_NAME,SD_NAME,Наименование,Left,1,1,1,0,;";
  fieldsInfo+="s.SD_EMAIL,SD_EMAIL,Адрес электронной почты,Left,1,1,1,0,;";

  try
  {
    gf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,NULL);
  }
  catch (Exception &exc)
  { MessageDlg("createOrderingSinchroSDFrame - Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(gf!=NULL)
  { gf->initFrame();
    gf->Parent=workPanel;
  }

  return gf;
}
//---------------------------------------------------------------------------

//******************************
//* Синхронизация справочников *
//******************************
// - функция формирования файла синхронизации справочников
AnsiString __fastcall createRBSinchroFile(TComponent *owner,AnsiString dirName)
{
TDateTime date=Date();
TDateTime time=Time();
unsigned short y,m,d; date.DecodeDate(&y,&m,&d);
unsigned short h,mn,s,ms; time.DecodeTime(&h,&mn,&s,&ms);
AnsiString fileIn=getAppDir()+"Out\\ToRaions\\Templates\\TempRBSinchro.xls";
AnsiString fileOut=dirName+"rbSinchro_"+AnsiString().sprintf("%d%02d%02d_%02d%02d%02d",y,m,d,h,mn,s)+".xls";
DirectWorkExcel *de;
FBDatabase *fbDB=getFBDataBase();
FBDataSet *fbDS=fbDB->getFBDS_T1_T2();
TpFIBDataSet *ds=fbDS->getDataSet();
AnsiString sql;

  //Копируем шаблон
  if(templateCopy(fileIn,fileOut)== false) return "";
  //Создаеи Excel файл
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

  //Заносим данные
  AnsiString str="Файл синхронизации справочников от "+date.DateString()+" "+time.TimeString();
  de->setCellValue("A1",str);
  //Выгружаем данные справочника PUBLISHERS_TABLE
  str="1. Справочник \"Поставщики ГЖП\"";
  de->setCellValue("A2",str);
  str="P_ID";de->setCellValue("A3",str);
  str="P_NAME";de->setCellValue("B3",str);
  str="DEL_FLG";de->setCellValue("C3",str);
  int n=4;
  sql="select P_ID,P_NAME,DEL_FLG from PUBLISHERS_TABLE";
  sql+=" order by P_ID";
  if(fbDS->execSelectSQL("createRBSinchroFile",sql)==false) goto end;
  ds->First();
  while(!ds->Eof)
  { str=ds->FieldByName("P_ID")->AsString; de->setCellValue(n,"A",str);
    str=ds->FieldByName("P_NAME")->AsString; de->setCellValue(n,"B",str);
    int delflg=ds->FieldByName("DEL_FLG")->AsInteger;
    str=IntToStr(delflg); de->setCellValue(n,"C",str);
    ds->Next();
    n++;
  }
  n++;
  //Выгружаем данные справочника NMP_TABLE
  str="2. Справочник \"Газетно-журнальная продукция\"";
  de->setCellValue(n,"A",str); n++;
  str="NMP_ID";de->setCellValue(n,"A",str);
  str="P_ID";de->setCellValue(n,"B",str);
  str="NMP_NAME";de->setCellValue(n,"C",str);
  str="NMP_TYPE";de->setCellValue(n,"D",str);
  str="RET_MUST_FLG";de->setCellValue(n,"E",str);
  str="DEL_FLG";de->setCellValue(n,"F",str);
  n++;
  sql="select NMP_ID,P_ID,NMP_NAME,NMP_TYPE,RET_MUST_FLG,DEL_FLG from NMP_TABLE";
  sql+=" where P_ID is not null order by P_ID,NMP_ID";
  if(fbDS->execSelectSQL("createRBSinchroFile",sql)==false) goto end;
  ds->First();
  while(!ds->Eof)
  { str=ds->FieldByName("NMP_ID")->AsString; de->setCellValue(n,"A",str);
    str=ds->FieldByName("P_ID")->AsString; de->setCellValue(n,"B",str);
    str=ds->FieldByName("NMP_NAME")->AsString; de->setCellValue(n,"C",str);
    str=ds->FieldByName("NMP_TYPE")->AsString; de->setCellValue(n,"D",str);
    int rmflg=ds->FieldByName("RET_MUST_FLG")->AsInteger;
    str=IntToStr(rmflg); de->setCellValue(n,"E",str);
    int delflg=ds->FieldByName("DEL_FLG")->AsInteger;
    str=IntToStr(delflg); de->setCellValue(n,"F",str);
    ds->Next();
    n++;
  }
  //Закрываем файл для изменений
  de->setSheetLocked("_MAR_",true);
end:
  de->saveFile();
  //Закрываем файл
  de->closeFile();
  delete de;
  return fileOut;
}
//---------------------------------------------------------------------------
// - функция формирования сообщений для синхронизации справочников в подразделениях
void createRBSinchroMessages(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
AnsiString dirIn=getAppDir()+"Out\\ToRaions\\ОбменДанными\\";
AnsiString dirOut=getAppDir()+"Temp\\";
TDBGridEh *grid;
TpFIBDataSet *ds;
AnsiString fileName,fileIn,fileOut;
AnsiString monthFolderID;
AnsiString rName,rMail;
unsigned short year, month, d;
TStringList *sl;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  MPF->setSubject("Программа \"Розница ГЖП\"");
  sl->Add("Присоединенный файл содержит данные для синхронизации справочников.");
  sl->Add("Используйте его для выполнения соответствующей процедуры");
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { AnsiString subject=MPF->getSubject();
    AnsiString content=MPF->getConten();
    bool delFilesFLG=MPF->getDelFilesFLG();
    bool replyFLG=MPF->getReplyFLG();
    delete MPF; MPF=NULL;

    grid=(static_cast<TExchangeDataFrame*>(owner))->getSDFrame()->workGrid;
    ds=(static_cast<TExchangeDataFrame*>(owner))->getSDFrame()->getWorkDataSet();
    TRichEdit *richEdit=(static_cast<TExchangeDataFrame*>(owner))->richEdit;
    richEdit->SetFocus();
    TColor color=richEdit->DefAttributes->Color;
    int size=richEdit->DefAttributes->Size;

    if(!DirectoryExists(dirIn)) CreateDir(dirIn);
    if(!DirectoryExists(dirOut)) CreateDir(dirOut);

    richEdit->SelAttributes->Size=10;
    //Создаем файл с данними справочникоа
    richEdit->SelAttributes->Color=clRed;
    richEdit->Lines->Add("Создание файла с данными справочников...");
    richEdit->Repaint();
    fileIn=createRBSinchroFile(owner,dirIn);
    if(fileIn.IsEmpty()) goto end;
    fileName=ExtractFileName(fileIn);
    richEdit->SelAttributes->Color=clBlue;
    richEdit->Lines->Add("Файл создан!");
    richEdit->Lines->Add("");
    richEdit->Repaint();

    Date().DecodeDate(&year,&month,&d);
    monthFolderID=checkYearAndMonthFolders(owner,getFBDataBase()->getDatabase(),"MESSAGES_FOLDERS_TABLE","Для подразделений\\Обмен данными\\Синхронизация справочников",year,month);
    if(monthFolderID==NULL || monthFolderID.IsEmpty()) goto end;

    richEdit->SelAttributes->Color=clRed;
    richEdit->Lines->Add("Формирование сообщений для подразделений:");
    richEdit->SelAttributes->Color=clBlue;
    richEdit->Repaint();
    if(grid->SelectedRows->Count==0) grid->Selection->Rows->CurrentRowSelected=true;
    for(int i=0; i<grid->SelectedRows->Count; ++i)
    { ds->GotoBookmark((void *)grid->SelectedRows->Items[i].c_str());
      grid->SetFocus();
      AnsiString sdid=ds->FieldByName("SD_ID")->AsString;
      fileOut=dirOut+fileName.SubString(1,fileName.Pos(".")-1)+"_"+sdid+ExtractFileExt(fileName);
      if(templateCopy(fileIn,fileOut)==false) goto end;
      rName=ds->FieldByName("SD_NAME")->AsString;
      rMail=ds->FieldByName("SD_EMAIL")->AsString;

      //Создаем сообщения для отправки в подразделение с прикрепленным файлом
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      addMessageToBase(owner,getFBDataBase(),monthFolderID,rName,rMail,subject,content,fileNames,replyFLG,delFilesFLG);
      delete fileNames;
      richEdit->Lines->Add(rName+" - "+fileOut);
      richEdit->Repaint();
      richEdit->SetFocus();
      richEdit->SelStart=richEdit->Text.Length();
      richEdit->SelLength=0;
    }
    DeleteFile(fileIn);
    richEdit->Lines->Add("");
    richEdit->SelAttributes->Color=color;
    richEdit->SelAttributes->Size=size;
    richEdit->Lines->Add("Операция завершена");
    richEdit->Repaint();

    AnsiString s;
    if(grid->SelectedRows->Count==1) s="Сообщение сформировано и находятся в папке \"";
    else s="Сообщения сформированны и находятся в папке \"";
    MessageDlg(s+getFullNameFilder(owner,getFBDataBase()->getDatabase(),"MESSAGES_FOLDERS_TABLE",monthFolderID)+"\"",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
end:
  if(MPF!=NULL) delete MPF;
}
//---------------------------------------------------------------------------

//***************************************************
//* Подготовка ключей настройки БД на подразделение *
//***************************************************
//функия формирования файла с ключем настройки
AnsiString __fastcall createKeyFile(TComponent *owner,AnsiString dir_name,AnsiString sd_id,AnsiString sd_name)
{
AnsiString fileOut=dir_name+"key_"+sd_id+".txt";
AnsiString key=sd_id+","+sd_name;
SecretWord *sw;
int hout;

  try
  {
    sw=new SecretWord();
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта SecretWord.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  key=sw->encodeWord(key);
  delete sw;

  //Создаем файл с кодированным ключем
  if(FileExists(fileOut)) DeleteFile(fileOut);
  hout=FileCreate(fileOut);
  if(hout < 0) return "";
  FileWrite(hout,key.c_str(),key.Length());
  FileClose(hout);

  return fileOut;
}
//---------------------------------------------------------------------------
// - функция формирования сообщений с ключами настройки
void prepareKeysOnSDMessages(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
AnsiString dirOut=getAppDir()+"Out\\ToRaions\\ОбменДанными\\";
TDBGridEh *grid;
TpFIBDataSet *ds;
AnsiString fileOut;
unsigned short year, month, d;
AnsiString monthFolderID;
TStringList *sl;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  MPF->setSubject("Программа \"Розница ГЖП\"");
  sl->Add("Присоединенный файл содержит ключ настройки базы данных на подразделение.");
  sl->Add("Используйте его для выполнения соответствующей процедуры");
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { AnsiString subject=MPF->getSubject();
    AnsiString content=MPF->getConten();
    bool delFilesFLG=MPF->getDelFilesFLG();
    bool replyFLG=MPF->getReplyFLG();
    delete MPF; MPF=NULL;

    grid=(static_cast<TExchangeDataFrame*>(owner))->getSDFrame()->workGrid;
    ds=(static_cast<TExchangeDataFrame*>(owner))->getSDFrame()->getWorkDataSet();
    TRichEdit *richEdit=(static_cast<TExchangeDataFrame*>(owner))->richEdit;
    richEdit->SetFocus();
    TColor color=richEdit->DefAttributes->Color;
    int size=richEdit->DefAttributes->Size;

    if(!DirectoryExists(dirOut)) CreateDir(dirOut);

    Date().DecodeDate(&year,&month,&d);
    monthFolderID=checkFolderInMessagesTable(owner,getFBDataBase()->getDatabase(),"MESSAGES_FOLDERS_TABLE","Для подразделений\\Обмен данными\\Ключи настройки БД",true);
    if(monthFolderID==NULL || monthFolderID.IsEmpty()) goto end;

    richEdit->SelAttributes->Color=clRed;
    richEdit->Lines->Add("Формирование сообщений с ключами настройки для подразделений:");
    if(grid->SelectedRows->Count==0) grid->Selection->Rows->CurrentRowSelected=true;
    richEdit->Repaint();
    for(int i=0; i<grid->SelectedRows->Count; ++i)
    { ds->GotoBookmark((void *)grid->SelectedRows->Items[i].c_str());
      grid->SetFocus();
      AnsiString sd_id=ds->FieldByName("SD_ID")->AsString;
      AnsiString sd_name=ds->FieldByName("SD_NAME")->AsString;
      AnsiString sd_mail=ds->FieldByName("SD_EMAIL")->AsString;
      //Создаем файл с ключем настройки
      fileOut=createKeyFile(owner,dirOut,sd_id,sd_name);
      if(fileOut.IsEmpty()) goto end;
      //Создаем сообщения для отправки в подразделение с прикрепленным файлом
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      addMessageToBase(owner,getFBDataBase(),monthFolderID,sd_name,sd_mail,subject,content,fileNames,replyFLG,delFilesFLG);
      delete fileNames;
      richEdit->SelAttributes->Color=clBlue;
      richEdit->Lines->Add(sd_id+","+sd_name+" - "+fileOut);
      richEdit->Repaint();
      richEdit->SetFocus();
      richEdit->SelStart=richEdit->Text.Length();
      richEdit->SelLength=0;
    }
    richEdit->Lines->Add("");
    richEdit->SelAttributes->Color=color;
    richEdit->SelAttributes->Size=size;
    richEdit->Lines->Add("Операция завершена");
    richEdit->Repaint();

    AnsiString s;
    if(grid->SelectedRows->Count==1) s="Сообщение сформировано и находятся в папке \"";
    else s="Сообщения сформированны и находятся в папке \"";
    MessageDlg(s+getFullNameFilder(owner,getFBDataBase()->getDatabase(),"MESSAGES_FOLDERS_TABLE",monthFolderID)+"\"",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
end:
  if(MPF!=NULL) delete MPF;
}
//---------------------------------------------------------------------------

//*****************************************************
//* Выгрузка в подразделения данных о поступлении ГЖП *
//*****************************************************
// - функция формирования select sql запроса о поступивщей ГЖП
AnsiString __fastcall formED_NMPSelectSQL(AnsiString sdID,TDateTime date1,TDateTime date2)
{
AnsiString sql;

  sql ="select p.p_id,p.p_name,n.nmp_id,n.nmp_name,n.nmp_type,n.ret_must_flg,";
  sql+="f.fact_id,f.fact_q,";
  sql+="e.E_NUMBER,e.E_PRICE,e.E_DATE,e.R_DATE,e.NDS_STAVKA";
  sql+=" from facts_table f,exits_table e,nmp_table n,publishers_table p";
  sql+=" where e.e_id=f.e_id and n.nmp_id=e.nmp_id and p.p_id=n.p_id";
  sql+=" and f.SD_ID="+sdID;
  sql+=" and e.E_DATE between '"+date1.DateString()+"'";
  sql+=" and '"+date2.DateString()+"'";

  return sql;
}
//---------------------------------------------------------------------------
// - функция создания фрейма с поступившей ГЖП
class TGridFrame * __fastcall createED_NMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel)
{
TGridFrame* nmpf=NULL;
AnsiString tableInfo;
AnsiString fieldsInfo;
AnsiString footerFieldsInfo;

  tableInfo="FACTS_TABLE,Газетно-журнальная продукция,FACT_ID,FACT_Q,,";

  fieldsInfo ="p.P_ID,P_ID,Поставщик|код,Center,1,1,1,0,;";
  fieldsInfo+="p.P_NAME,P_NAME,Поставщик|наименование,Left,1,1,1,0,;";
  fieldsInfo+="n.NMP_ID,NMP_ID,Газетно-журнальная продукция|код,Center,1,1,1,0,;";
  fieldsInfo+="n.NMP_NAME,NMP_NAME,Газетно-журнальная продукция|наименование,Left,1,1,1,0,;";
  fieldsInfo+="n.NMP_TYPE,NMP_TYPE,Газетно-журнальная продукция|тип,Center,1,0,0,0,;";
  fieldsInfo+="n.RET_MUST_FLG,RET_MUST_FLG,Газетно-журнальная продукция|флаг возврата,Left,1,0,0,0,;";
  fieldsInfo+="e.E_NUMBER,E_NUMBER,Номер выхода,Left,1,1,1,0,;";
  fieldsInfo+="f.FACT_ID,FACT_ID,Разнарядка|код,Center,1,1,1,0,;";
  fieldsInfo+="f.FACT_Q,FACT_Q,Разнарядка|коли- чество,Right,1,1,1,0,#;";
  fieldsInfo+="e.E_PRICE,E_PRICE,Разнарядка|цена без НДС,Center,1,1,1,0,;";
  fieldsInfo+="e.NDS_STAVKA,NDS_STAVKA,Разнарядка|ставка НДС,Left,1,1,1,0,;";
  fieldsInfo+="e.E_DATE,E_DATE,Разнарядка|дата|разнаряжения,Center,1,1,1,0,;";
  fieldsInfo+="e.R_DATE,R_DATE,Разнарядка|дата|возврата,Center,1,1,1,0,";

  footerFieldsInfo="FACT_Q,fvtSum,#0,,Right";

  try
  {
    nmpf=new TGridFrame(Owner,iniFN,iniSN,fbDB,tableInfo,fieldsInfo,footerFieldsInfo);
  }
  catch (Exception &exc)
  { MessageDlg("createED_NMPFrame- Ошибка создания TGridFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  if(nmpf!=NULL)
  { nmpf->initFrame();
    nmpf->Parent=workPanel;
  }

  return nmpf;
}
//---------------------------------------------------------------------------
// - формирование сообщений с данными по отруженной в подразделения ГЖП
void createShipmentDataToSubdivisions(TComponent *owner,AnsiString iniFileName)
{
TMessageParametersForm *MPF;
TStringList *sl;
TProgressBar *sdBar;
TDateTime date1, date2;
FBDatabase *fbDB=getFBDataBase();
TDBGridEh *lGrid;
TpFIBDataSet *lDS;
AnsiString rName,rMail;
unsigned short year, month, d;
AnsiString monthFolderID;
AnsiString dirName=getAppDir()+"Out\\ToRaions\\";
AnsiString fileOut;

  try
  {
    MPF=new TMessageParametersForm(owner,iniFileName,"MessageParametersForm");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TMessageParametersForm.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  sl=new TStringList;
  sdBar=(static_cast<TUnloadDataToSDFrame*>(owner))->sdBar;
  date1=(static_cast<TUnloadDataToSDFrame*>(owner))->getDate1();
  date2=(static_cast<TUnloadDataToSDFrame*>(owner))->getDate2();
  lDS=(static_cast<TUnloadDataToSDFrame*>(owner))->getLDS();
  lGrid=(static_cast<TUnloadDataToSDFrame*>(owner))->getLGrid();

  sl->Add("Данные по отгруженной газетно-журнальной продукции за период с "+date1.DateString()+" по "+date2.DateString());
  MPF->setSubject("Данные по отруженной ГЖП");
  sl->Add("---");
  sl->Add(getUser()->getUName()+", "+getUser()->getUMail());
  MPF->setContent(sl);
  if(MPF->ShowModal()==mrOk)
  { sdBar->Visible=true;
//    (static_cast<TUnloadDataToSDFrame*>(owner))->Repaint();
    date2.DecodeDate(&year,&month,&d);
    //Создаем нужные каталоги
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(year)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    dirName+=IntToStr(month)+"\\";
    if(!DirectoryExists(dirName)) CreateDir(dirName);
    //Проверяем наличие нужных папок в таблице MESSAGES_FOLDERS_TABLE
    monthFolderID=checkYearAndMonthFolders(owner,getFBDataBase()->getDatabase(),"MESSAGES_FOLDERS_TABLE","Для подразделений\\Обмен данными\\Данные по отгруженной ГЖП",year,month);
    if(monthFolderID==NULL || monthFolderID.IsEmpty()) return;
    if(lGrid->SelectedRows->Count==0) lGrid->Selection->Rows->CurrentRowSelected=true;
    sdBar->Max=lGrid->SelectedRows->Count;
    sdBar->Position=0;
    for(int i=0; i<lGrid->SelectedRows->Count; ++i)
    { lDS->GotoBookmark((void *)lGrid->SelectedRows->Items[i].c_str());
      //1.Формируем Excel файла с данными
      AnsiString sd_id=lDS->FieldByName("SD_ID")->AsString;
      fileOut=createShipmentDataToSDFile(owner,fbDB,sd_id,dirName);
      rName=(static_cast<TUnloadDataToSDFrame*>(owner))->getRicipientName();
      rMail=(static_cast<TUnloadDataToSDFrame*>(owner))->getRicipientMail();
      //2.Создаем сообщения для отправки в подразделение с прикрепленным файлом
      TStringList *fileNames;
      fileNames=new TStringList;
      if(!fileOut.IsEmpty()) fileNames->Add(fileOut);
      addMessageToBase(owner,fbDB,monthFolderID,rName,rMail,MPF->getSubject(),MPF->getConten(),fileNames,MPF->getReplyFLG(),MPF->getDelFilesFLG());
      delete fileNames;
      sdBar->StepIt();
    }
    sdBar->Position=0;
    sdBar->Visible=false;
    if(!fileOut.IsEmpty()) MessageDlg("Сообщения сформированы",mtInformation,TMsgDlgButtons() << mbOK,0);
    else MessageDlg("При формировании сообщений возникли ошибки",mtInformation,TMsgDlgButtons() << mbOK,0);
  }
}
//---------------------------------------------------------------------------
// - формирование файла с отгруженными данными
AnsiString __fastcall createShipmentDataToSDFile(TComponent *owner,FBDatabase *fbDB,AnsiString sd_id,AnsiString dirName)
{
AnsiString fileOut=dirName;
AnsiString fileIn=getAppDir()+"Out\\ToRaions\\Templates\\ShipmentDataToSD.xls";
unsigned short y,m,d;
DirectWorkExcel *de=NULL;
//TDBGridEh *rGrid;
TpFIBDataSet *rDS;
TDateTime date1,date2;
AnsiString sql;
TProgressBar *nmpBar;
AnsiString sdID,sdName;
int n=5, rn;

  nmpBar=(static_cast<TUnloadDataToSDFrame*>(owner))->nmpBar;
  rDS=(static_cast<TUnloadDataToSDFrame*>(owner))->getRDS();
//  rGrid=(static_cast<TUnloadDataToSDFrame*>(owner))->getRGrid();
  date1=(static_cast<TUnloadDataToSDFrame*>(owner))->getDate1();
  date2=(static_cast<TUnloadDataToSDFrame*>(owner))->getDate2();
  date2.DecodeDate(&y,&m,&d);
  sdID=(static_cast<TUnloadDataToSDFrame*>(owner))->getRicipientID();
  sdName=(static_cast<TUnloadDataToSDFrame*>(owner))->getRicipientName();
  fileOut+="Данные по отгрузке ГЖП-"+AnsiString().sprintf("%d-%02d_%02d",y,m,StrToInt(sd_id))+".xls";
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
    fileOut="";
    goto end;
  }
  de->setSheetNumber(1);
  nmpBar->Visible=true;
//  (static_cast<TUnloadDataToSDFrame*>(owner))->Repaint();
  rn=rDS->RecNo;
  rDS->Last();
  nmpBar->Max=rDS->RecordCount;
  nmpBar->Position=0;
  //Выгружаем данные в файл
  //Заголовок
  de->setCellValue(1,"A","Данные по отгруженной газетно-журнальной продукции");
  de->setCellValue(2,"A",date1.DateString());
  de->setCellValue(2,"B",date2.DateString());
  de->setCellValue(3,"A",sdID);
  de->setCellValue(3,"B",sdName);
  //Табличная часть
  rDS->First();
  n=5;
  while(!rDS->Eof)
  { putstr(n,de,rDS);
    n++;
    nmpBar->StepIt();
    rDS->Next();
  }
  //Защищаем файл от изменений
  de->setSheetLocked("_MAR_",true);

  nmpBar->Position=0;
  nmpBar->Visible=false;
  rDS->RecNo=rn;
end:
  if(de!=NULL) delete de;
  return fileOut;
}
//---------------------------------------------------------------------------
// - Вывод строки в файл
void __fastcall putstr(int n,class DirectWorkExcel *de,TDataSet *ds)
{
  de->setCellValue(n,"A",ds->FieldByName("P_ID")->AsString);
  de->setCellValue(n,"B",ds->FieldByName("P_NAME")->AsString);
  de->setCellValue(n,"C",ds->FieldByName("NMP_ID")->AsString);
  de->setCellValue(n,"D",ds->FieldByName("NMP_NAME")->AsString);
  de->setCellValue(n,"E",ds->FieldByName("NMP_TYPE")->AsString);
  de->setCellValue(n,"F",ds->FieldByName("RET_MUST_FLG")->AsString);
  de->setCellValue(n,"G",ds->FieldByName("FACT_ID")->AsString);
  de->setCellValue(n,"H",ds->FieldByName("FACT_Q")->AsString);
  de->setCellValue(n,"I",ds->FieldByName("E_NUMBER")->AsString);
  de->setCellValue(n,"J",ds->FieldByName("E_PRICE")->AsString);
  de->setCellValue(n,"K",ds->FieldByName("NDS_STAVKA")->AsString);
  de->setCellValue(n,"L",ds->FieldByName("E_DATE")->AsString);
  de->setCellValue(n,"M",ds->FieldByName("R_DATE")->AsString);
}
//---------------------------------------------------------------------------

