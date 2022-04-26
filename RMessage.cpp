//---------------------------------------------------------------------------
//Класс для обработки поступающей электронной почты

#include <vcl.h>
#pragma hdrstop

#include "Folder.h"
#include "ZipBuilder.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "RMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall RMessage::RMessage(TComponent *owner,AnsiString workD,FBDatabase *fbdb)
{
  Owner=owner;
  workDir=workD;
  if(!DirectoryExists(workDir)) CreateDir(workDir);
  fb_db=fbdb;

  pop3Host="";
  pop3UserID="";
  pop3Password="";
//  pop3AttachFilePath="";
  userID="";
  UName="";

  fbRQ=NULL;
  fbWQ=NULL;
  fbRDS=NULL;
  fbWDS=NULL;

  folderID="";
  RContents=NULL;
  AttachedFiles=NULL;
  afID=NULL;

  ZipBuilder=NULL;
  pop3=NULL;

  try
  {
    fbRQ=newFBQuery(Owner,"Message::Message",fb_db->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта fbRQ.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  RQ=fbRQ->getQuery();
  try
  {
    fbWQ=newFBQuery(Owner,"Message::Message",fb_db->getDatabase(),false);
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта fbWQ.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  WT=fbWQ->getTransaction();
  try
  {
    fbRDS=newFBDataSet(Owner,"Message::Message",fb_db->getDatabase(),true);
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта fbRDS.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  RDS=fbRDS->getDataSet();
  try
  {
    fbWDS=newFBDataSet(Owner,"Message::Message",fb_db->getDatabase(),WT,WT,false);
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта fbWDS.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  WDS=fbWDS->getDataSet();

  try
  {
    RContents=new TStringList();
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта RContents\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  try
  {
    AttachedFiles=new TStringList();
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта AttachedFiles\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  try
  {
    afID=new TStringList();
  }
  catch (Exception &exc)
  { MessageDlg("Message::init:Ошибка создания объекта afID\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  try
  {
    ZipBuilder=new TZipBuilder(Owner);
  }
  catch (Exception &exc)
  { MessageDlg("Message::Message:Ошибка создания объекта ZipBuilder\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  try
  {
    pop3=new TIdPOP3(Owner);
  }
  catch (Exception &exc)
  { MessageDlg("Message::Message:Ошибка создания объекта TNMSMTP\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  init();
}
//---------------------------------------------------------------------------
__fastcall RMessage::~RMessage(void)
{
  if(RContents!=NULL){ delete RContents; RContents=NULL; }
  if(AttachedFiles!=NULL)
  { RemovingAttachedFiles();
    delete AttachedFiles; AttachedFiles=NULL;
  }
  if(afID!=NULL){ delete afID; afID=NULL;}

  if(ZipBuilder!=NULL){ delete ZipBuilder; ZipBuilder=NULL;}
  if(pop3!=NULL){ if(pop3->Connected()==true) pop3->Disconnect(); delete pop3; pop3=NULL;}
  if(fbRQ!=NULL){ delete fbRQ; fbRQ=NULL; }
  if(fbWQ!=NULL){ delete fbWQ; fbWQ=NULL; }
  if(fbRDS!=NULL){ delete fbRDS; fbRDS=NULL; }
  if(fbWDS!=NULL){ delete fbWDS; fbWDS=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall RMessage::init(void)
{
  RContents->Clear();
  RemovingAttachedFiles();
  AttachedFiles->Clear();
  afID->Clear();

  mID="";
  SenderAddress="";
  RSubject="";
  ReceivedDate=0.0;
  ReceivedTime=0.0;
}
//---------------------------------------------------------------------------
bool __fastcall RMessage::connectToHost(void)
{
  if(pop3->Connected()==false)
  { pop3->Host=pop3Host;
    pop3->UserId=pop3UserID;
    pop3->Password=pop3Password;
    try
    {
      pop3->Connect();
    }
    catch (Exception &exc)
    { MessageDlg("Ошибка подключения к серверу\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      mMailCount=-1;
      return false;
    }
  }
  mMailCount=pop3->CheckMessages();
  return true;
}
//---------------------------------------------------------------------------
void __fastcall RMessage::disconnectFromHost(void)
{
  if(pop3->Connected()==true) pop3->Disconnect();
}
//---------------------------------------------------------------------------
int __fastcall RMessage::getMailCount(void)
{
  return mMailCount;
}
//---------------------------------------------------------------------------
int __fastcall RMessage::checkMailCount(void)
{
 connectToHost();
 disconnectFromHost();
 return mMailCount;
}
//---------------------------------------------------------------------------
void __fastcall RMessage::receiveMessages(void)
{
int i,p,PartCount;
AnsiString filePath=workDir+"Temp\\";
AnsiString FileName;
TIdMessage *IdMsg=NULL;
TIdAttachment *IdAttach;
TIdText *IdText;

  if(connectToHost()==false) return;

  try
  {
    IdMsg=new TIdMessage(Owner);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TIdMessage\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    if(pop3->Connected()) pop3->Disconnect();
    return;
  }

  if(mMailCount > 0)
  { for(i=1; i<=mMailCount; i++)
    { init();
      IdMsg->Clear();
      if(pop3->Retrieve(i,IdMsg))
      { SenderAddress=IdMsg->From->Address;
        RSubject=IdMsg->Subject;
        PartCount=IdMsg->MessageParts->Count;
        for(p=0; p<PartCount; p++)
        { if(IdMsg->MessageParts->Items[p]->ClassNameIs("TIdAttachment"))
          { IdAttach=(TIdAttachment*)IdMsg->MessageParts->Items[p];
            FileName=IdAttach->FileName;
            AnsiString ext=ExtractFileExt(FileName);
            if(ext.IsEmpty())
            { ext=".txt";
              FileName=IntToStr(p)+ext;
            }
            FileName=filePath+FileName;
            if(FileExists(FileName)) DeleteFile(FileName);
            IdAttach->SaveToFile(FileName);
            AttachedFiles->Add(FileName);
          }
          else if(IdMsg->MessageParts->Items[p]->ClassNameIs("TIdText"))
          { IdText=dynamic_cast<TIdText *>(IdMsg->MessageParts->Items[p]);
            if(IdText) RContents->Add(IdText->Body->Text);
          }
        }
        WriteToBase();
        for(int i=0; i<AttachedFiles->Count; i++) DeleteFile(AttachedFiles->Strings[i]);
      }
    }
    for(i=mMailCount; i>0; i--) pop3->Delete(i);
  }
  if(IdMsg != NULL){ delete IdMsg; IdMsg=NULL;}
  mMailCount=pop3->CheckMessages();
  if(pop3->Connected()) pop3->Disconnect();
}
//---------------------------------------------------------------------------
void __fastcall RMessage::setAttachedFilesList(TStringList *af)
{
  AttachedFiles->Clear();
  AttachedFiles->AddStrings(af);
}
//---------------------------------------------------------------------------
void __fastcall RMessage::ReadFromBase(AnsiString m_ID)
{
  init();
  mID=m_ID;
  if(!mID.IsEmpty())
  { AnsiString sql;
    sql="select m.*, u.U_MAIL, u.U_NAME from RECEIVED_MESSAGES_TABLE m";
    sql+=" left outer join USERS_TABLE u on u.U_ID=m.USER_ID";
    sql+=" where M_ID="+mID;
    if(fbRQ->execQuery("RMessage::ReadFromBase",sql)==false) return;
    if(RQ->RecordCount > 0)
    { folderID=RQ->FieldByName("FOLDER_ID")->AsString;
      userID=RQ->FieldByName("USER_ID")->AsString;
      UName=RQ->FieldByName("U_NAME")->AsString;
      SenderAddress=RQ->FieldByName("SENDER_ADDRESS")->AsString;;
      RSubject=RQ->FieldByName("RM_SUBJECT")->AsString;
      ReceivedDate=RQ->FieldByName("RECEIVED_DATE")->AsDate;
      ReceivedTime=RQ->FieldByName("RECEIVED_TIME")->AsTime;
    }
    //Читаем содержание сообщения
    RContents->Clear();
    AnsiString content="";
    sql="select NUM,CONTENT from RMESSAGE_CONTENTS_TABLE";
    sql+=" where M_ID="+mID+" order by NUM";
    if(fbRDS->execSelectSQL("RMessage::ReadFromBase",sql)==false) return;
    RDS->First();
    while(!RDS->Eof)
    { content+=RDS->FieldByName("CONTENT")->AsString;
      RDS->Next();
    }
    RContents->Text=content;
    //Читаем информацию о присоединенных файлах
    sql="select * from RATTACHED_FILES_TABLE where RM_ID="+mID;
    if(fbRDS->execSelectSQL("RMessage::ReadFromBase",sql)==false) return;
    RDS->First();
    while(!RDS->Eof)
    { AnsiString fn=RDS->FieldByName("FILE_NAME")->AsString;
      fn=workDir+"Temp\\"+fn;
      AttachedFiles->Add(fn);
      afID->Add(RDS->FieldByName("RAF_ID")->AsString);
      RDS->Next();
    }
    RDS->Close();
  }
}
//---------------------------------------------------------------------------
bool __fastcall RMessage::WriteToBase(void)
{
AnsiString sql;
AnsiString m_ID;
int f,n;
AnsiString subStr;
AnsiString content;

  if(folderID.IsEmpty())
  { MessageDlg("Message::WriteToBase:Не указана папка, в которую должно быть записано сообщение",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto err;
  }
  if(userID.IsEmpty())
  { MessageDlg("Message::WriteToBase:Не указан пользователь",mtWarning,TMsgDlgButtons() << mbOK,0);
    goto err;
  }
  ReceivedDate=TDateTime().CurrentDate();
  ReceivedTime=TDateTime().CurrentTime();
  //Стартуем транзакцию
  WT->StartTransaction();
  //Генерируем значение M_ID
  m_ID=getNewKeyValue(fb_db,"RECEIVED_MESSAGES_TABLE");
  if(m_ID.IsEmpty()) goto err;
  //Делаем запись в таблице MESSAGES_TABLE
  sql="insert into RECEIVED_MESSAGES_TABLE(M_ID,FOLDER_ID,SENDER_ADDRESS,\
RM_SUBJECT,RECEIVED_DATE,RECEIVED_TIME,USER_ID,EXAMINED_FLG) values(";
  sql+=m_ID+",";
  sql+="'"+folderID+"',";
  if(!SenderAddress.IsEmpty()) sql+="'"+SenderAddress+"',";
  else sql+="NULL,";
  if(!RSubject.IsEmpty()) sql+="'"+RSubject+"',";
  else sql+="NULL,";
  sql+="'"+ReceivedDate.DateString()+"',";
  sql+="'"+ReceivedTime.TimeString()+"',";
  sql+=userID+",0)";
  if(fbWQ->execQuery("RMessage::WriteToBase",sql)==false) goto err;
  mID=m_ID;
  //Заносим содержание сообщения
  content=RContents->Text;
  if(!content.IsEmpty())
  { f=1;n=1;
    subStr=content.SubString(f,200);
    do
    { sql="insert into RMESSAGE_CONTENTS_TABLE(M_ID,NUM,CONTENT) values(";
      sql+=mID+",";
      sql+=IntToStr(n)+",";
      sql+="'"+subStr+"')";
      if(fbWQ->execQuery("RMessage::WriteToBase",sql)==false) goto err;
      n++;
      f+=200;
      subStr=content.SubString(f,200);
    }while(!subStr.IsEmpty());
  }
  //Записываем в базу присоединенные фалы
  if(AttachedFiles->Count > 0)
  { WDS->Close();
    WDS->SQLs->SelectSQL->Clear();
    WDS->SQLs->InsertSQL->Clear();
    sql="select * from RATTACHED_FILES_TABLE where RM_ID="+mID;
    WDS->SQLs->SelectSQL->Add(sql);
    sql="insert into RATTACHED_FILES_TABLE(RAF_ID,RM_ID,ATTACHED_FILE,FILE_NAME) values(?RAF_ID,?RM_ID,?ATTACHED_FILE,?FILE_NAME)";
    WDS->SQLs->InsertSQL->Add(sql);
    try
    {
      WDS->Open();
    }
    catch(const Exception &exc)
    { MessageDlg("Message::WriteToBase: Ошибка - WDS->Open()\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
      goto err;
    }
    for(int i=0; i<AttachedFiles->Count; i++)
    { AnsiString fName=AttachedFiles->Strings[i];
      if(FileExists(fName))
      { AnsiString zipFileName;
        unsigned short h,m,s,ms;
        ReceivedTime.DecodeTime(&h,&m,&s,&ms);
        zipFileName=ExtractFilePath(fName)+AnsiString().sprintf("%02d%02d%02d%02d",h,m,s,ms)+".zip";;
        //Архивируем файл
        ZipBuilder->ZipFileName=zipFileName;
        ZipBuilder->FSpecArgs->Clear();
        ZipBuilder->FSpecArgs->Add(fName);
        ZipBuilder->Add();
        if(ZipBuilder->ErrCode != 0)
        { MessageDlg("Ошибка архивирования файла - "+fName+".\n Код ошибки - "+IntToStr(ZipBuilder->ErrCode)+".\n"+ZipBuilder->Message,mtError,TMsgDlgButtons() << mbOK,0);
          goto err;
        }
        //Генерируем значение RAF_ID
        AnsiString rafID=getNewKeyValue(fb_db,"RATTACHED_FILES_TABLE");
        if(rafID.IsEmpty()) goto err;
        //Делаем запись в таблицу RATTACHED_FILES_TABLE
        WDS->Append();
        WDS->FieldByName("RAF_ID")->AsString=rafID;
        WDS->FieldByName("RM_ID")->AsString=mID;
        WDS->FieldByName("FILE_NAME")->AsString=ExtractFileName(fName);
        try
        {
          (static_cast<TBlobField*>(WDS->FieldByName("ATTACHED_FILE")))->LoadFromFile(zipFileName);
        }
        catch(const Exception &exc)
        { MessageDlg("Ошибка загрузки файла в Blob поле.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
          goto err;
        }
        try
        {
          WDS->Post();
        }
        catch(const Exception &exc)
        { MessageDlg("Ошибка WDS->Post()\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
          WDS->Close();
          goto err;
        }
        //Удаляем zip файл
        DeleteFile(zipFileName);
      }
      else
      { MessageDlg("Не найден файл - "+fName,mtError,TMsgDlgButtons() << mbOK,0);
        WDS->Close();
        goto err;
      }
    }
    WDS->Close();
  }
  WT->Commit();
  return true;
err:
  if(WT->InTransaction) WT->Rollback();
  return false;
}
//---------------------------------------------------------------------------
void __fastcall RMessage::DeleteFromBase(void)
{
  if(!mID.IsEmpty())
  { AnsiString sql;
    //Стартуем транзакцию
    WT->StartTransaction();
    //Удаляем запись о сообщении
    sql="delete from RECEIVED_MESSAGES_TABLE where M_ID="+mID;
    if(fbWQ->execQuery("Message::DeleteFromToBase",sql)==false) goto err;
    WT->Commit();
  }
  init();
  return;
err:
  if(WT->InTransaction) WT->Rollback();
  return;
}
//---------------------------------------------------------------------------
//Извлечение файла из базы
AnsiString __fastcall RMessage::ExtractFile(AnsiString fileName,AnsiString af_id)
{
AnsiString zipFileName;
unsigned short y,m,d,h,mn,s,ms;

  Date().DecodeDate(&y,&m,&d);
  Time().DecodeTime(&h,&mn,&s,&ms);
  AnsiString extractDir=workDir+"ExtractDir\\";
  if(!DirectoryExists(extractDir)) CreateDir(extractDir);
  zipFileName=extractDir+AnsiString().sprintf("%02d%02d%02d%02d",m,h,mn,s)+".zip";
  //Извлекаем документ в zip файл
  WDS->Close();
  WDS->SQLs->SelectSQL->Clear();
  AnsiString sql="select * from RATTACHED_FILES_TABLE where RAF_ID="+af_id;
  WDS->SQLs->SelectSQL->Add(sql);
  try
  {
    WDS->Open();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка - WDS->Open()\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return "";
  }
  WDS->First();
  if(WDS->Eof) return "";
  AnsiString oldFileName=WDS->FieldByName("FILE_NAME")->AsString;
  (static_cast<TBlobField*>(WDS->FieldByName("ATTACHED_FILE")))->SaveToFile(zipFileName);
  WDS->Close();
  //Извлекаем документ из zip файла
  if(unzipFiles(zipFileName)==true)
  { //Удаляем zip файл
    DeleteFile(zipFileName);
    oldFileName=ExtractFileDir(zipFileName)+"\\"+ExtractFileName(oldFileName);
    RenameFile(oldFileName,fileName);
    return fileName;
  }
  else return "";
}
//---------------------------------------------------------------------------
void __fastcall RMessage::ExtractAllFiles()
{
AnsiString sql;

  if(mID.IsEmpty()) return;
  sql="select RAF_ID,FILE_NAME from RATTACHED_FILES_TABLE where RM_ID="+mID;
  if(fbRDS->execSelectSQL("RMessage::ExtractAllFiles",sql)==false) return;
  RDS->First();
  while(!RDS->Eof)
  { AnsiString afid=RDS->FieldByName("RAF_ID")->AsString;
    AnsiString fn=RDS->FieldByName("FILE_NAME")->AsString;
    AnsiString filename=workDir+"Temp\\"+fn;
    if(ExtractFile(filename,afid).IsEmpty())
      MessageDlg("Ошибка извлечения файла - "+filename,mtError,TMsgDlgButtons() << mbOK,0);
    RDS->Next();
  }
  RDS->Close();
}
//---------------------------------------------------------------------------
bool __fastcall RMessage::unzipFiles(AnsiString zipFileName)
{
  ZipBuilder->ZipFileName=zipFileName;
  ZipBuilder->ExtrBaseDir=ExtractFilePath(zipFileName);
  try
  {
    ZipBuilder->Extract();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка извлечения файло из zip архива "+zipFileName+"\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall RMessage::UnloadFile(int af_num, TJvSaveDialog *dlg)
{
AnsiString sql;
AnsiString raf_id=afID->Strings[af_num];

  sql="select FILE_NAME from RATTACHED_FILES_TABLE where RAF_ID="+raf_id;
  if(fbRQ->execQuery("Message::UnloadFile",sql)==false) return;
  AnsiString fileName=RQ->FieldByName("FILE_NAME")->AsString;
  RQ->Close();
  dlg->FileName=fileName;
  if(dlg->Execute()==true) ExtractFile(dlg->FileName,raf_id);
}
//---------------------------------------------------------------------------
//Открытие для просмотра присоединенного фала
void __fastcall RMessage::PreviewAttachedFile(int af_num, AnsiString uid)
{
AnsiString sql;
AnsiString fileName;
AnsiString raf_id=afID->Strings[af_num];

  sql="select FILE_NAME from RATTACHED_FILES_TABLE where RAF_ID="+raf_id;
  if(fbRQ->execQuery("RMessage::PreviewAttachedFile",sql)==false) return;
  AnsiString previewDir=workDir+"PreviewFiles\\";
  if(!DirectoryExists(previewDir)) CreateDir(previewDir);
  fileName=previewDir+ExtractFileName(RQ->FieldByName("FILE_NAME")->AsString);
  RQ->Close();
  //Если этот файл уже просматривали, то удаляем его
  bool del_flg;
  if(FileExists(fileName)) del_flg=DeleteFile(fileName);
  else del_flg=true;
  if(del_flg)
  { sql="delete from RPREVIEW_FILES_TABLE";
    sql+=" where RATTACHED_FILE_ID="+raf_id;
    sql+=" and USER_ID="+uid;
    fbWQ->execQuery("Message::PreviewAttachedFile",sql);
  }
  ExtractFile(fileName,raf_id);
  if(!FileExists(fileName)) return;

  //Блокируем файл до момента его открытия для просмотра
  int hf=FileOpen(fileName,fmShareExclusive);

  //Заносим информацию в таблицу RPREVIEW_FILES_TABLE
  //Стартуем транзакцию
  WT->StartTransaction();
  //Генерируем новое значение ключа для таблицы RPREVIEW_FILES_TABLE
  AnsiString pfID=getNewKeyValue(fb_db,"RPREVIEW_FILES_TABLE");
  if(pfID.IsEmpty()) goto err;
  //  Делаем запись
  sql="insert into RPREVIEW_FILES_TABLE(PF_ID,RATTACHED_FILE_ID,FILE_NAME,USER_ID) values(";
  sql+=pfID+",";
  sql+=raf_id+",";
  sql+="'"+fileName+"',";
  sql+=uid+")";
  if(fbWQ->execQuery("Message::PreviewAttachedFile",sql)==false) goto err;
  //Завершаем транзакцию
  WT->Commit();
  //Разблокируем файл
  FileClose(hf);
  //Загружаем выбранный документ для просмотра
  if(FileExists(fileName))
  { HINSTANCE h=ShellExecute(NULL, NULL, fileName.c_str(), NULL, NULL, SW_RESTORE);
    HINSTANCE errh=HINSTANCE(32);
    if(h<=errh)
    { MessageDlg("Ошибка запуска приложения. Номер ошибки - "+IntToStr(h),mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }
  }
  return;
err:
  if(WT->InTransaction) WT->Rollback();
}
//---------------------------------------------------------------------------
bool __fastcall RMessage::renameAttachedFile(int af_num, AnsiString newFilename)
{
AnsiString sql;
AnsiString raf_id=afID->Strings[af_num];
bool ret=false;
AnsiString zipFileName;

  ExtractFile(newFilename,raf_id);
  unsigned short h,m,s,ms;
  Time().DecodeTime(&h,&m,&s,&ms);
  zipFileName=ExtractFilePath(newFilename)+AnsiString().sprintf("%02d%02d%02d%02d",h,m,s,ms)+".zip";;
  //Архивируем файл
  ZipBuilder->ZipFileName=zipFileName;
  ZipBuilder->FSpecArgs->Clear();
  ZipBuilder->FSpecArgs->Add(newFilename);
  ZipBuilder->Add();
  if(ZipBuilder->ErrCode != 0)
  { MessageDlg("Ошибка архивирования файла - "+newFilename+".\n Код ошибки - "+IntToStr(ZipBuilder->ErrCode)+".\n"+ZipBuilder->Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }

  WDS->Close();
  WDS->SQLs->SelectSQL->Clear();
  WDS->SQLs->UpdateSQL->Clear();
  sql="select * from RATTACHED_FILES_TABLE where RAF_ID="+raf_id;
  WDS->SQLs->SelectSQL->Add(sql);
  sql="update RATTACHED_FILES_TABLE set";
  sql+=" ATTACHED_FILE=?ATTACHED_FILE,";
  sql+="FILE_NAME=?FILE_NAME";
  sql+=" where RAF_ID="+raf_id;
  WDS->SQLs->UpdateSQL->Add(sql);
  try
  {
    WDS->Open();
  }
  catch(const Exception &exc)
  { MessageDlg("Message::WriteToBase: Ошибка - WDS->Open()\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  WDS->Edit();
  try
  {
    (static_cast<TBlobField*>(WDS->FieldByName("ATTACHED_FILE")))->LoadFromFile(zipFileName);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка загрузки файла в Blob поле.\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    goto end;
  }
  WDS->FieldByName("FILE_NAME")->AsString=ExtractFileName(newFilename);
  try
  {
    WDS->Post();
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка WDS->Post()\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    WDS->Close();
    goto end;
  }
  WDS->Close();
  if(WDS->UpdateTransaction->InTransaction) WDS->UpdateTransaction->Commit();
  ret=true;
end:
  if(FileExists(newFilename)) DeleteFile(newFilename);
  if(FileExists(zipFileName)) DeleteFile(zipFileName);
  return ret;
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall RMessage::RemovingAttachedFiles(void)
{
  if(AttachedFiles!=NULL)
  { for(int i=0; i<AttachedFiles->Count; i++)
      if(FileExists(AttachedFiles->Strings[i]))
        DeleteFile(AttachedFiles->Strings[i]);
  }
}
//---------------------------------------------------------------------------
