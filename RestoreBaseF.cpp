//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SomeFunctions.h"
#include "RestoreBaseF.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExComCtrls"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvProgressBar"
#pragma link "JvWaitingGradient"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "ToolEdit"
#pragma link "ZipBuilder"
#pragma link "JvComponent"
#pragma link "JvExComCtrls"
#pragma link "JvExControls"
#pragma link "JvNavigationPane"
#pragma link "JvProgressBar"
#pragma link "JvWaitingGradient"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "ToolEdit"
#pragma link "ZipBuilder"
#pragma link "JvXPCheckCtrls"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma resource "*.dfm"
//TRestoreBaseFrame *RestoreBaseFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TRestoreBaseFrame::TRestoreBaseFrame(TComponent* Owner,AnsiString wd,AnsiString iniFN,FBDatabase *fbdb,TPanel *workPanel,TStatusPanel *statPanel)
        : TFrame(Owner),workDir(wd),iniFileName(iniFN),fbDB(fbdb),statusPanel(statPanel)
{
  Parent=workPanel;
  server=fbDB->getServer();
  dbFullName=fbDB->getDBFullName();
  DBNameHeader->Caption="База данных - "+server+dbFullName;
}
//---------------------------------------------------------------------------
__fastcall TRestoreBaseFrame::~TRestoreBaseFrame(void)
{
  saveProperties();
  SetCurrentDir(workDir);
}
//---------------------------------------------------------------------------
void __fastcall TRestoreBaseFrame::initFrame(void)
{
  Label1->Width=Width-32;
  Label2->Width=Width-32;
  Label3->Width=Width-32;
  Label4->Width=Width-32;
  zipFilenameEdit->Width=Width-32;
  backupDirectoryEdit->Width=Width-32;
  dbNetDirectoryEdit->Width=Width-32;
  dbCopyDirectoryEdit->Width=Width-32;
  RestoreButton->Left=Width-16-RestoreButton->Width;

  loadProperties();
  zipFilenameEdit->SetFocus();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TRestoreBaseFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  zipFilenameEdit->Text=ini->ReadString(server+"BackUpBaseF","ZipFileName","");
  backupFN=ExtractFileName(ini->ReadString(server+"BackUpBaseF","BackUpFileName",""));
  backupDirectoryEdit->Text=ini->ReadString(server+"BackUpBaseF","BackUpNetDirectory","");
  backupFileName=backupDirectoryEdit->Text+"\\"+backupFN;
  dbNetDirectoryEdit->Text=ini->ReadString(server+"RestoreBaseF","dbNetDirectory","");
  dbCopyDirectoryEdit->Text=ini->ReadString(server+"RestoreBaseF","dbCopyDirectory","");
  restoreDateTime=ini->ReadDateTime(server+"RestoreBaseF","restoreDateTime",0);
  delete ini;
  if(int(restoreDateTime)==0)
    statusPanel->Text="Дата последнего восстановления базы данных неизвестна";
  else
    statusPanel->Text="Дата и время последнего восстановления базы данных - "+restoreDateTime.DateTimeString();
}
//---------------------------------------------------------------------------
void __fastcall TRestoreBaseFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteString(server+"RestoreBaseF","dbNetDirectory",dbNetDirectoryEdit->Text);
  ini->WriteString(server+"RestoreBaseF","dbCopyDirectory",dbCopyDirectoryEdit->Text);
  ini->WriteDateTime(server+"RestoreBaseF","restoreDateTime",restoreDateTime);
  delete ini;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
//Восстановление базы
void __fastcall TRestoreBaseFrame::RestoreButtonClick(TObject *Sender)
{
  if(MessageDlg("Восстановление базы данных возможно только в монопольном режиме.\nВы уверены, что никто кроме Вас не работет с базой?",mtWarning,TMsgDlgButtons() << mbOK << mbCancel,0)==mrCancel)
    return;
  if(backupDirectoryEdit->Text.IsEmpty())
  { MessageDlg("Не указана папка для BackUp файла",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(dbNetDirectoryEdit->Text.IsEmpty() || !DirectoryExists(dbNetDirectoryEdit->Text))
  { if(dbNetDirectoryEdit->Text.IsEmpty())
      MessageDlg("Не указан сетевой путь к базе данных",mtError,TMsgDlgButtons() << mbOK,0);
    else
      MessageDlg("Каталог \""+dbNetDirectoryEdit->Text+"\" отсутствует",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  if(dbCopyDirectoryEdit->Text.IsEmpty() || !DirectoryExists(dbCopyDirectoryEdit->Text))
  { if(dbCopyDirectoryEdit->Text.IsEmpty())
      MessageDlg("Не указана папка для копии базы данных",mtError,TMsgDlgButtons() << mbOK,0);
    else
      MessageDlg("Каталог \""+dbCopyDirectoryEdit->Text+"\" отсутствует",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  RichEdit->Lines->Clear();
  RichEdit->SetFocus();
  TColor color=RichEdit->DefAttributes->Color;
  int size=RichEdit->DefAttributes->Size;
  RichEdit->SelAttributes->Color=clRed;
  RichEdit->SelAttributes->Size=10;

  WaitingGradient->Visible=true;
  WaitingGradient->Active=true;

  //Распаковываем архив
  if(!zipFilenameEdit->Text.IsEmpty() && FileExists(zipFilenameEdit->Text))
  { RichEdit->Lines->Add("Извлечение BackUp файла из архива...");
    ZipBuilder->ZipFileName=zipFilenameEdit->Text;
    ZipBuilder->FSpecArgs->Clear();
    AnsiString fn=backupFN;
    ZipBuilder->FSpecArgs->Add(fn);
    ZipBuilder->ExtrBaseDir=backupDirectoryEdit->Text;
    ZipBuilder->ExtrOptions.Clear();
    ZipBuilder->ExtrOptions << ExtrOverWrite;
    ZipBuilder->Extract();
    if(ZipBuilder->SuccessCnt != 1)
    { MessageDlg("Ошибка извлечения BackUp файла",mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }

    RichEdit->SelAttributes->Color=clBlue;
    RichEdit->SelAttributes->Size=10;
    RichEdit->Lines->Add("BackUp файл извлечен!");
    RichEdit->Repaint();
  }

  if(!FileExists(backupFileName))
  { MessageDlg("BackUP файла \""+backupFileName+"\" отсутствует",mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  WaitingGradient->Active=false;
  WaitingGradient->Visible=false;

  bool connectFLG=fbDB->isConnected();
  if(connectFLG==true) closeBase();

  ProgressBar->Visible=true;

  //Сохраняем старую базу
  RichEdit->SelAttributes->Color=clRed;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("Сохраняем старую базу данных");
  RichEdit->Repaint();

  AnsiString oldDBFileName, dbFileName;
  dbFileName=dbNetDirectoryEdit->Text+"\\"+fbDB->getBaseName();
  unsigned short y,m,d;
  Date().DecodeDate(&y,&m,&d);
  oldDBFileName=dbCopyDirectoryEdit->Text+"\\"+fbDB->getBaseName().SubString(0,fbDB->getBaseName().Pos(".")-1)+AnsiString().sprintf("%4d%02d%02d",y,m,d)+".old";

  int hDB, hOldDB;
  hDB=FileOpen(dbFileName,fmOpenRead);
  if(hDB < 0)
  { MessageDlg("Ошибка открытия файла - "+dbFileName,mtError,TMsgDlgButtons() << mbOK,0);
    RichEdit->Lines->Clear();
    return;
  }
  if(FileExists(oldDBFileName)) DeleteFile(oldDBFileName);
  hOldDB=FileCreate(oldDBFileName);
  if(hOldDB < 0)
  { MessageDlg("Ошибка открытия файла - "+oldDBFileName,mtError,TMsgDlgButtons() << mbOK,0);
    RichEdit->Lines->Clear();
    return;
  }
  int dbFileLength=FileSeek(hDB,0,2);
  ProgressBar->Max=dbFileLength/32768+1;
  char buf[32768];
  FileSeek(hDB,0,0);
  ProgressBar->Position=0;
  int rb=0;
  while((rb=FileRead(hDB,buf,sizeof(buf))) > 0)
  { if(FileWrite(hOldDB,buf,rb) < 0)
    { MessageDlg("Ошибка копирования файла - ",mtError,TMsgDlgButtons() << mbOK,0);
      RichEdit->Lines->Clear();
      return;
    }
    ProgressBar->StepIt();
  }
  FileClose(hDB);
  FileClose(hOldDB);

  RichEdit->SelAttributes->Color=clBlue;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("Старая база находится в файле - "+oldDBFileName);
  RichEdit->Repaint();

  ProgressBar->Position=0;
  ProgressBar->Max=1100;
  ProgressBar->Visible=false;

  //Восстановление базы из BuckUp фала
  RichEdit->SelAttributes->Color=clRed;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("Восстановление базы данных из BackUp файла...");
  RichEdit->SelAttributes->Color=color;
  RichEdit->SelAttributes->Size=size;
  RichEdit->Repaint();

  //Инициируем сервис
  RestoreService->ServerName=server.SubString(0,server.Length()-1);
  RestoreService->LoginPrompt=false;
  RestoreService->Params->Clear();
  RestoreService->Params->Add("user_name="+fbDB->getUserName());
  RestoreService->Params->Add("password="+fbDB->getUserPassword());
  RestoreService->Protocol=TCP;
  RestoreService->Active=true;
  RestoreService->Verbose = true;
  RestoreService->PageSize=8192;
  RestoreService->BufferSize=32000;
  RestoreService->DatabaseName->Clear();
  RestoreService->DatabaseName->Add(dbFullName);
  RestoreService->BackupFile->Clear();
//  RestoreService->BackupFile->Add(backUpFilenameEdit->Text);
  RestoreService->BackupFile->Add(backupFileName);

  if(protocolShowBox->Checked==false)
  { ProgressBar->Visible=true;
    ProgressBar->Position=0;
  }
  try
  { AnsiString Text;
    RestoreService->ServiceStart();
    while(!RestoreService->Eof)
    { Text=RestoreService->GetNextLine();
      if(protocolShowBox->Checked)
      { RichEdit->Lines->Add(Text);
        RichEdit->Repaint();
        RichEdit->SetFocus();
        RichEdit->SelStart=RichEdit->Text.Length();
        RichEdit->SelLength=0;
      }
      else
      { ProgressBar->StepIt();
        ProgressBar->Refresh();
      }
    }
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка восстановления из BackUp файла\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    RestoreService->Active = false;
    ProgressBar->Position=0;
    return;
  }
  RestoreService->Active = false;

  RichEdit->SelAttributes->Color=clBlue;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("База данных восстановлена!");
  RichEdit->Repaint();

  if(ProgressBar->Visible)
  { ProgressBar->Position=0;
    ProgressBar->Visible=false;
  }

  //Удаление BackUp файла
  if(!zipFilenameEdit->Text.IsEmpty() && FileExists(backupFileName))
  { RichEdit->SelAttributes->Color=clRed;
    RichEdit->SelAttributes->Size=10;
    RichEdit->Lines->Add("Удаление BackUp файла...");
    RichEdit->Repaint();
    DeleteFile(backupFileName);
    RichEdit->SelAttributes->Color=clBlue;
    RichEdit->SelAttributes->Size=10;
    if(!FileExists(backupFileName))
      RichEdit->Lines->Add("Файл удален!");
    else
      MessageDlg("Не могу удалить BackUp файл - "+backupFileName,mtError,TMsgDlgButtons() << mbOK,0);
    RichEdit->Lines->Add("\n");
    RichEdit->SelAttributes->Color=clBlue;
    RichEdit->SelAttributes->Size=10;
    RichEdit->Lines->Add("Операция завершена!");
    RichEdit->Repaint();
  }

  RichEdit->SelAttributes->Color=color;
  RichEdit->SelAttributes->Size=size;

  if(connectFLG==true) openBase();
  restoreDateTime=TDateTime().CurrentDateTime();
  statusPanel->Text="Дата и время последнего восстановлени базы данных - "+restoreDateTime.DateTimeString();
}
//---------------------------------------------------------------------------
void __fastcall TRestoreBaseFrame::ZipBuilderMessage(TObject *Sender,int ErrCode,AnsiString Message)
{
  RichEdit->Lines->Add(Message);
  RichEdit->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TRestoreBaseFrame::backupDirectoryEditChange(TObject *Sender)
{
  backupFileName=backupDirectoryEdit->Text+"\\"+backupFN;
}
//---------------------------------------------------------------------------

