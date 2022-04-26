//---------------------------------------------------------------------------
//Класс для обработки поступающей электронной почты
#ifndef RMessageH
#define RMessageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DBClasses.h"
#include "ZipBuilder.h"
#include <IdPOP3.hpp>
#include "JvDialogs.hpp"
//---------------------------------------------------------------------------
class RMessage
{
private:
  TComponent* Owner;
  AnsiString workDir;
  FBDatabase *fb_db;
  FBQuery *fbRQ,*fbWQ;
  TpFIBQuery *RQ;
  TpFIBTransaction *WT;
  FBDataSet *fbRDS,*fbWDS;
  TpFIBDataSet *RDS,*WDS;
  AnsiString mID;
  AnsiString folderID;
  AnsiString userID;
  AnsiString UName;
  AnsiString SenderAddress;
  AnsiString RSubject;
  TStringList *RContents;
  TStringList *AttachedFiles;
  TStringList *afID;
  TDateTime ReceivedDate;
  TDateTime ReceivedTime;
  TZipBuilder *ZipBuilder;
  TIdPOP3 *pop3;
  AnsiString pop3Host;
  AnsiString pop3UserID;
  AnsiString pop3Password;
  int mMailCount;
  void __fastcall RemovingAttachedFiles(void);

public:
  __fastcall RMessage(TComponent* owner,AnsiString workD,FBDatabase *fbdb);
  __fastcall ~RMessage(void);
  void __fastcall init(void);
  bool __fastcall connectToHost(void);
  void __fastcall disconnectFromHost(void);
  int __fastcall getMailCount(void);
  int __fastcall checkMailCount(void);
  void __fastcall receiveMessages(void);
  void __fastcall setAttachedFilesList(TStringList *af);
  void __fastcall ReadFromBase(AnsiString m_ID);
  bool __fastcall WriteToBase(void);
  void __fastcall DeleteFromBase(void);
  AnsiString __fastcall ExtractFile(AnsiString fileName,AnsiString af_id);
  void __fastcall ExtractAllFiles();
  bool __fastcall unzipFiles(AnsiString zipFileName);
  void __fastcall UnloadFile(int af_num, TJvSaveDialog *dlg);
  void __fastcall PreviewAttachedFile(int af_num, AnsiString uid);
  bool __fastcall renameAttachedFile(int af_num, AnsiString newFiename);

  AnsiString __fastcall getM_ID(void){ return mID; }
  TStringList * __fastcall getContentsList(void){ return RContents;}
  TStringList * __fastcall getAttachedFilesList(void){ return AttachedFiles;}
  TStringList * __fastcall getAD_IDList(void){ return afID;}
  void __fastcall setPOP3Host(AnsiString host){ pop3Host=host; };
  void __fastcall setPOP3UserID(AnsiString uID){ pop3UserID=uID; };
  void __fastcall setPOP3Password(AnsiString pwd){ pop3Password=pwd; };
//  void __fastcall setPOP3AttachFilePath(AnsiString path){ pop3AttachFilePath=path; };
  void __fastcall setSenderAddress(AnsiString sa){ SenderAddress=sa; };
  void __fastcall setRSubject(AnsiString s){ RSubject=s; };
  AnsiString __fastcall getRSubject(void){ return RSubject; }
//  void __fastcall setRContent(AnsiString c){ RContent=c; };
  void __fastcall setUserID(AnsiString uID){ userID=uID; };
  void __fastcall setUName(AnsiString un){ UName=un; };
  void __fastcall setFolderID(AnsiString fid){ folderID=fid; };
};
//---------------------------------------------------------------------------

#endif
