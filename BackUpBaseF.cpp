//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DBClasses.h"
#include "BackUpBaseF.h"
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
#pragma link "Placemnt"
#pragma link "ToolEdit"
#pragma link "ZipBuilder"
#pragma link "JvXPCheckCtrls"
#pragma resource "*.dfm"
//TBackUpBaseFrame *BackUpBaseFrame;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TBackUpBaseFrame::TBackUpBaseFrame(TComponent* Owner,AnsiString wd,AnsiString iniFN,FBDatabase *fbdb,TPanel *workPanel,TStatusPanel *statPanel)
        : TFrame(Owner),workDir(wd),iniFileName(iniFN),fbDB(fbdb),statusPanel(statPanel)
{
  Parent=workPanel;
  server=fbDB->getServer();
  dbFullName=fbDB->getDBFullName();
  DBNameHeader->Caption="���� ������ - "+server+dbFullName;
}
//---------------------------------------------------------------------------
__fastcall TBackUpBaseFrame::~TBackUpBaseFrame(void)
{
  saveProperties();
  SetCurrentDir(workDir);
}
//---------------------------------------------------------------------------
void __fastcall TBackUpBaseFrame::initFrame(void)
{
  Label1->Width=Width-32;
  Label2->Width=Width-32;
  Label3->Width=Width-32;
  BackUpFilenameEdit->Width=Width-32;
  BackUpDirEdit->Width=Width-32;
  ZipFilenameEdit->Width=Width-32;
  ArchButton->Left=Width-16-ArchButton->Width;

  loadProperties();
  BackUpFilenameEdit->SetFocus();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TBackUpBaseFrame::loadProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  BackUpFilenameEdit->FileName=ini->ReadString(server+"BackUpBaseF","BackUpFileName","");
  BackUpDirEdit->Text=ini->ReadString(server+"BackUpBaseF","BackUpNetDirectory","");
  ZipFilenameEdit->FileName=ini->ReadString(server+"BackUpBaseF","ZipFileName","");
  unsigned short y,m,d;
  Date().DecodeDate(&y,&m,&d);
  ZipFilenameEdit->FileName=ExtractFileDir(ZipFilenameEdit->FileName)+"\\"+AnsiString().sprintf("%02d%02d%02d",y-2000,m,d)+".zip";
  backUpDateTime=ini->ReadDateTime(server+"BackUpBaseF","backUpDateTime",0);
  delete ini;
  if(int(backUpDateTime)==0)
    statusPanel->Text="���� ���������� ������������� ����������";
  else
    statusPanel->Text="���� � ����� ���������� ������������� - "+backUpDateTime.DateTimeString();
}
//---------------------------------------------------------------------------
void __fastcall TBackUpBaseFrame::saveProperties(void)
{
TIniFile *ini = new TIniFile(iniFileName);

  ini->WriteString(server+"BackUpBaseF","BackUpFileName",BackUpFilenameEdit->FileName);
  ini->WriteString(server+"BackUpBaseF","BackUpNetDirectory",BackUpDirEdit->Text);
  ini->WriteString(server+"BackUpBaseF","ZipFileName",ZipFilenameEdit->FileName);
  ini->WriteDateTime(server+"BackUpBaseF","backUpDateTime",backUpDateTime);
  delete ini;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TBackUpBaseFrame::BackUpFilenameEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action)
{
  BackUpDirEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TBackUpBaseFrame::BackUpDirEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action)
{
  if(ExtractFileDir(ZipFilenameEdit->FileName).Pos(":")==false)
    ZipFilenameEdit->FileName=Name+ZipFilenameEdit->FileName;
  ZipFilenameEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TBackUpBaseFrame::ZipFilenameEditAfterDialog(TObject *Sender,AnsiString &Name,bool &Action)
{
  ArchButton->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TBackUpBaseFrame::ArchButtonClick(TObject *Sender)
{
  ProgressBar->Visible=true;
  ProgressBar->Position=0;

  if(BackUpFilenameEdit->FileName.IsEmpty())
  { MessageDlg("�� ������� ��� BackUp �����",mtError,TMsgDlgButtons() << mbOK,0);
    BackUpFilenameEdit->SetFocus();
    return;
  }
  if(BackUpDirEdit->Text.IsEmpty())
  { MessageDlg("�� ������ ������� ���� � BackUp �����",mtError,TMsgDlgButtons() << mbOK,0);
    BackUpDirEdit->SetFocus();
    return;
  }
  if(ZipFilenameEdit->FileName.IsEmpty())
  { MessageDlg("�� ������� ��� zip ������",mtError,TMsgDlgButtons() << mbOK,0);
    ZipFilenameEdit->SetFocus();
    return;
  }

  //������������� ������� �������� BackUp �����
  BackupService->ServerName=server.SubString(0,server.Length()-1);
  BackupService->LoginPrompt=false;
  BackupService->Params->Clear();
  BackupService->Params->Add("user_name="+fbDB->getUserName());
  BackupService->Params->Add("password="+fbDB->getUserPassword());
  BackupService->Protocol=TCP;
  BackupService->Active = true;
  BackupService->Verbose = true;
  BackupService->DatabaseName=dbFullName;
  BackupService->BackupFile->Clear();
  BackupService->BackupFile->Add(BackUpFilenameEdit->FileName);

  RichEdit->Lines->Clear();
  RichEdit->SetFocus();
  TColor color=RichEdit->DefAttributes->Color;
  int size=RichEdit->DefAttributes->Size;
  RichEdit->SelAttributes->Color=clRed;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("�������� Backup ����� ���� ������...");
  RichEdit->SelAttributes->Color=color;
  RichEdit->SelAttributes->Size=size;
  RichEdit->Repaint();

  if(protocolShowBox->Checked==false)
  { ProgressBar->Visible=true;
    ProgressBar->Position=0;
  }

  //�����
  try
  { AnsiString Text;
    BackupService->ServiceStart();
    while(!BackupService->Eof)
    { Text=BackupService->GetNextLine();
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
  { MessageDlg("������ �������� BackUp �����\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }

  RichEdit->SelAttributes->Color=clBlue;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("Backup ���� ���� ������ ������! ��� ����� - "+BackUpFilenameEdit->FileName);
  RichEdit->Repaint();
  BackupService->Active = false;

  if(ProgressBar->Visible)
  { ProgressBar->Position=0;
    ProgressBar->Visible=false;
  }
  WaitingGradient->Visible=true;
  WaitingGradient->Active=true;

  //���������� ��������� BackUp ����
  AnsiString zipFN=ZipFilenameEdit->FileName;
  AnsiString backupFN=BackUpDirEdit->Text+"\\"+ExtractFileName(BackUpFilenameEdit->FileName);
  if(FileExists(backupFN))
  { RichEdit->SelAttributes->Color=clRed;
    RichEdit->SelAttributes->Size=10;
    RichEdit->Lines->Add("������������� Backup �����...");
    RichEdit->Repaint();
    RichEdit->SelAttributes->Color=clBlue;
    RichEdit->SelAttributes->Size=10;
    if(FileExists(zipFN)) DeleteFile(zipFN);
    ZipBuilder->ZipFileName=zipFN;
    ZipBuilder->FSpecArgs->Clear();
    ZipBuilder->FSpecArgs->Add(backupFN);
    ZipBuilder->Add();
    if(ZipBuilder->ErrCode != 0)
    { MessageDlg("������ �������� ������. ��� ������ - "+IntToStr(ZipBuilder->ErrCode)+".\n"+ZipBuilder->Message,mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }

    //��������� ����������� ���������� ������
    ZipBuilder->ZipFileName=zipFN;
    ZipBuilder->FSpecArgs->Clear();
    ZipBuilder->ExtrOptions.Clear();
    ZipBuilder->ExtrOptions << ExtrTest;
    ZipBuilder->Extract();
    if(ZipBuilder->SuccessCnt != 1)
    { MessageDlg("������ ������������ ���������� ������. �������� ���������� ���������!",mtError,TMsgDlgButtons() << mbOK,0);
      return;
    }

    RichEdit->Lines->Add("����� ���� ������ ������ � ��������� � ����� - "+zipFN);
    RichEdit->Repaint();

    WaitingGradient->Active=false;
    WaitingGradient->Visible=false;

    //�������� BackUp �����
    if(FileExists(backupFN))
    { RichEdit->SelAttributes->Color=clRed;
      RichEdit->SelAttributes->Size=10;
      RichEdit->Lines->Add("�������� BackUp �����...");
      RichEdit->Repaint();
      DeleteFile(backupFN);
      RichEdit->SelAttributes->Color=clBlue;
      RichEdit->SelAttributes->Size=10;
      if(!FileExists(backupFN))
        RichEdit->Lines->Add("���� ������!");
      else
        MessageDlg("�� ���� ������� BackUp ���� - "+backupFN,mtError,TMsgDlgButtons() << mbOK,0);
      RichEdit->Lines->Add("\n");
    }
  }
  RichEdit->SelAttributes->Color=clBlue;
  RichEdit->SelAttributes->Size=10;
  RichEdit->Lines->Add("�������� ���������!");
  RichEdit->Repaint();

  RichEdit->SelAttributes->Color=color;
  RichEdit->SelAttributes->Size=size;
  backUpDateTime=TDateTime().CurrentDateTime();
  statusPanel->Text="���� � ����� ���������� ������������� - "+backUpDateTime.DateTimeString();
}
//---------------------------------------------------------------------------

