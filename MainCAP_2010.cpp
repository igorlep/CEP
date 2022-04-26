//---------------------------------------------------------------------------
//Главная форма программы "Розница книжной продукции" (версия 2010 года)
#include <vcl.h>
#pragma hdrstop
#include "ChTreeView.h"
#include "SomeFunctions.h"
#include "SelectBase.h"
#include "Entry.h"
#include "RBWorksF.h"
#include "TemplatesEditF.h"
#include "OrderingNMPF.h"
#include "ArrivalNMPF.h"
#include "ShipmentNMPF.h"
#include "ReceivedMessagesF.h"
#include "MessagesWorkF.h"
#include "ReturnNMPfromSDF.h"
#include "ReturnNMPtoPublishersF.h"
#include "UnloadDataForBookKeepingF.h"
#include "FormApplicationsForNMPFromSDF.h"
#include "FormApplicationsForNMPToPublishersF.h"
#include "ArchivingBDF.h"
#include "RestoreDBF.h"
#include "UsersRightsF.h"
#include "ProgrammSettingsF.h"
#include "IncomsFromRealizationNMPF.h"
#include "ReferenceOnMovementOfGoodsF.h"
#include "PreparingBaseForNewPeriodF.h"
#include "DictationOnPaymentF.h"
#include "GoodsRepotrF.h"
#include "ReferenceOnMGForSDF.h"
#include "FinancialResultsForPubF.h"
#include "FinancialResultsForSDF.h"
#include "ExchangeDataF.h"
#include "UnloadDataToSDF.h"
#include "MainCAP_2010.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TMainCAP_2010Form *MainCAP_2010Form;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TMainCAP_2010Form::TMainCAP_2010Form(TComponent* Owner)
        : TForm(Owner)
{
char szEnvVariable[MAX_PATH];

  GetEnvironmentVariable("USERPROFILE",szEnvVariable,MAX_PATH);
  userprofileDir=AnsiString(szEnvVariable)+"\\CEP\\";
  if(!DirectoryExists(userprofileDir)) CreateDir(userprofileDir);
  iniFileName=userprofileDir+"CEP_2010.INI";
  appDir=ExtractFilePath(Application->ExeName);
  fbDB=NULL;
  user=NULL;
  workFrame=NULL;
  miActive=NULL;
  TRegistry *Reg;
  fontDPI=96;
  try
  {
    Reg=new TRegistry(KEY_READ);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TRegistry\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return;
  }
  try
  { Reg->RootKey = HKEY_LOCAL_MACHINE;
    if (Reg->OpenKey("\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\FontDPI",false))
    { fontDPI=Reg->ReadInteger("LogPixels");
      Reg->CloseKey();
    }
  }
  __finally
  {
    delete Reg;
  }
}
//---------------------------------------------------------------------------
bool __fastcall TMainCAP_2010Form::initMainForm(void)
{
AnsiString progName="Розница газетно-журнальной продукции";

  //Подключение к базе
  TSelectBaseForm *SBF=NULL;
  try
  {
    SBF=new TSelectBaseForm(this,iniFileName,userprofileDir,progName);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания TSelectBaseForm\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  AnsiString initDBString="";
  if(SBF->ShowModal()==mrOk)
    initDBString=SBF->getInitString();
  delete SBF;
  if(initDBString.IsEmpty()) return false;

  //Авторизация
  TEntryForm *EF=NULL;
  try
  {
    EF=new TEntryForm(Owner,iniFileName,initDBString,progName);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания объекта TEntryForm"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    EF=NULL;
    return false;
  }
  AnsiString u_id="";
  if(EF->ShowModal()==mrOk) u_id=EF->getSelectedUser()->getUID();
  delete EF;
  if(u_id.IsEmpty()) return false;

  //Создаем объект FBDatabase и инициируем его
  try
  {
    fbDB=new FBDatabase(this);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта FBDatabase\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  fbDB->init(initDBString);
  //Создаем объект PCUser
  try
  {
    user=new PCUser(fbDB);
  }
  catch(const Exception &exc)
  { MessageDlg("Ошибка создания объекта PCUser\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    user=NULL;
    return false;
  }
  //Открываем базу и определяем параметры выбранного пользователя
  if(openBase(fbDB, u_id)==false)
  { MessageDlg("Ошибка подключения к базе данных",mtError,TMsgDlgButtons() << mbOK,0);
    return false;
  }
  //Инициируем пункты меню
  TIniFile *ini = new TIniFile(iniFileName);
  miActiveName=ini->ReadString("MainCEP_2010Form","miActiveName_"+user->getUID(),"");
  delete ini;
  setMenuItems(MainMenu->Items);
  //
  setMainStatusBar();

  DecimalSeparator='.';
  DateSeparator='.';
  ShortDateFormat="dd.mm.yyyy";
  ShortTimeFormat="hh:mm:ss";

  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TMainCAP_2010Form::openBase(FBDatabase *fbdb, AnsiString userID)
{
  if(fbdb==NULL) return false;
  //Подключаемся к базе
  if(fbdb->connect()==false) return false;
  //Читаем информацию о пользователе
  user->getDataFromUsersTable(userID);
  //Запускаем поток, удаляющий файлы, открытые для прсмотра
//  try
//  {
//    tpf=new TerminatePreviewFilesThread(false,fbDB,userID);
//  }
//  catch(const Exception &exc)
//  { MessageDlg("Ошибка создания объекта TerminatePreviewFilesThread\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
//    tpf=NULL;
//    return false;
//  }
  return true;
}
//---------------------------------------------------------------------------
void __fastcall TMainCAP_2010Form::closeBase(void)
{
  if(fbDB==NULL) return;
  //Завершаем поток, удаляющий файлы, открытые для прсмотра
//  if(tpf!=NULL)
//  { tpf->Terminate();
//    while(terminatePreviewFilesThreadFGG==true) Sleep(200);
//    delete tpf; tpf=NULL;
//  }
  //Завершаем подключение к базе данных
  if(fbDB->isConnected()) fbDB->disconnect();
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TMainCAP_2010Form::destroyFrames(void)
{
  if(workFrame!=NULL){ delete workFrame; workFrame=NULL; }
}
//---------------------------------------------------------------------------
void __fastcall TMainCAP_2010Form::setMainStatusBar(void)
{
int p=7;

  if(fontDPI > 96) p=9;
  MainStatusBar->Panels->Items[0]->Text=fbDB->getDBAlias();
  MainStatusBar->Panels->Items[0]->Width=(MainStatusBar->Panels->Items[0]->Text.Length())*p;
  MainStatusBar->Panels->Items[1]->Text=user->getUName()+","+user->getURole();
  MainStatusBar->Panels->Items[1]->Width=(MainStatusBar->Panels->Items[1]->Text.Length())*p;
  MainStatusBar->Panels->Items[2]->Text="Компьютер - "+user->getNetComputerName();
  MainStatusBar->Panels->Items[2]->Width=(MainStatusBar->Panels->Items[2]->Text.Length())*p;
}
//---------------------------------------------------------------------------
void __fastcall TMainCAP_2010Form::setMenuItems(TMenuItem *mi)
{
AnsiString uid=user->getUID();

  for(int i=0; i<mi->Count; i++)
  { AnsiString name=mi->Items[i]->Name;
    CheckStyle cs=checkUserRightForMenuName(this,fbDB,uid,name);
    if(cs==csChecked || cs==csCheckedgray)
    { mi->Items[i]->Visible=true;
      if(mi->Items[i]->Name==miActiveName) miActive=mi->Items[i];
    }
    else mi->Items[i]->Visible=false;
    setMenuItems(mi->Items[i]);

  }
  if(user->getURole()=="администратор" && rbWorksItem->Visible==false) rbWorksItem->Visible=true;
  if(exitItem->Visible==false) exitItem->Visible=true;
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TMainCAP_2010Form::FormShow(TObject *Sender)
{
  if(miActive!=NULL) miActive->Click();
}
//---------------------------------------------------------------------------
void __fastcall TMainCAP_2010Form::FormDestroy(TObject *Sender)
{
  if(user!=NULL && miActiveName!=NULL)
  { TIniFile *ini = new TIniFile(iniFileName);
    ini->WriteString("MainCEP_2010Form","miActiveName_"+user->getUID(),miActiveName);
    delete ini;
  }
  destroyFrames();
  closeBase();
  if(user!=NULL){ delete user; user=NULL; }
  if(fbDB!=NULL){ delete fbDB; fbDB=NULL; }
}
//---------------------------------------------------------------------------

//***************
//* Справочники *
//***************
//Работа со справочниками
void __fastcall TMainCAP_2010Form::rbWorksItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TRBWorksFrame") return;
  destroyFrames();
  try
  {
    workFrame=new TRBWorksFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма rbWorkF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TRBWorksFrame*>(workFrame))->initFrame();
    miActiveName="rbWorksItem";
  }
}
//---------------------------------------------------------------------------
//Выгрузка справочников для синхронизации данных с подразделениями
void __fastcall TMainCAP_2010Form::rbSinchroItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TExchangeDataFrame" && (static_cast<TExchangeDataFrame*>(workFrame))->getType()=="rbSinchro") return;
  destroyFrames();
  try
  {
    workFrame=new TExchangeDataFrame(this,iniFileName,fbDB,"rbSinchro");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма rbSinchroF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TExchangeDataFrame*>(workFrame))->initFrame();
    miActiveName="rbSinchroItem";
  }
}
//---------------------------------------------------------------------------

//Работа с данными - Формирование заявок на ГЖП - Формирование заявок на ГЖП от подразделений
void __fastcall TMainCAP_2010Form::formApplicationsForNMPToSubdivisionsItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TFormApplicationsForNMPFromSDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TFormApplicationsForNMPFromSDFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма FormApplicationsForNMPFromSDFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TFormApplicationsForNMPFromSDFrame*>(workFrame))->initFrame();
    miActiveName="formApplicationsForNMPToSubdivisionsItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Формирование заявок на ГЖП - Формирование заявок на ГЖП для поставщиков
void __fastcall TMainCAP_2010Form::formApplicationsForNMPToPublishersItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TFormApplicationsForNMPToPublishersFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TFormApplicationsForNMPToPublishersFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма FormApplicationsForNMPToPublishersFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TFormApplicationsForNMPToPublishersFrame*>(workFrame))->initFrame();
    miActiveName="formApplicationsForNMPToPublishersItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Шаблоны - Редактирование
void __fastcall TMainCAP_2010Form::templatesEditItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TTemplatesEditFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TTemplatesEditFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма TemplatesEditF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TTemplatesEditFrame*>(workFrame))->initFrame();
    miActiveName="templatesEditItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Поступление и отгрузка ГЖП - Разнаряжение газетно-журнальной продукции
void __fastcall TMainCAP_2010Form::orderingNMPClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TOrderingNMPFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TOrderingNMPFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма OrderingNMPF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TOrderingNMPFrame*>(workFrame))->initFrame();
    miActiveName="orderingNMP";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Поступление и отгрузка ГЖП - Поступление ГЖП от поставщиков
void __fastcall TMainCAP_2010Form::arrivalNMPItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TArrivalNMPFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TArrivalNMPFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма ArrivalNMPF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TArrivalNMPFrame*>(workFrame))->initFrame();
    miActiveName="arrivalNMPItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Поступление и отгрузка ГЖП - Отгрузка ГЖП подразделениям
void __fastcall TMainCAP_2010Form::shipmentNMPItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TShipmentNMPFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TShipmentNMPFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма ShipmentNMPF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TShipmentNMPFrame*>(workFrame))->initFrame();
    miActiveName="shipmentNMPItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Поступление и отгрузка ГЖП - Выгрузка данных в подразделение
void __fastcall TMainCAP_2010Form::unloadDataToSDItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TUnloadDataToSDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TUnloadDataToSDFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма UnloadDataToSDPF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TUnloadDataToSDFrame*>(workFrame))->initFrame();
    miActiveName="unloadDataToSDItem";
  }
}
//---------------------------------------------------------------------------

//Работа с данными - Возврат ГЖП - Возврат ГЖП из подразделений
void __fastcall TMainCAP_2010Form::returnNMPFromSubdivisionsItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TReturnNMPfromSDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TReturnNMPfromSDFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма ReturnNMPfromSDFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TReturnNMPfromSDFrame*>(workFrame))->initFrame();
    miActiveName="returnNMPFromSubdivisionsItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Возврат ГЖП - Возврат ГЖП поставщикам
void __fastcall TMainCAP_2010Form::returnNMPToPublishersItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TReturnNMPtoPublishersFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TReturnNMPtoPublishersFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма ReturnNMPtoPublishersFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TReturnNMPtoPublishersFrame*>(workFrame))->initFrame();
    miActiveName="returnNMPToPublishersItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Выгрузка данных для бухгалтерии УФПС
void __fastcall TMainCAP_2010Form::sendDataToMainOfficeItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TUnloadDataForBookKeepingFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TUnloadDataForBookKeepingFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма UnloadDataForBookKeepingFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TUnloadDataForBookKeepingFrame*>(workFrame))->initFrame();
    miActiveName="sendDataToMainOfficeItem";
  }
}
//---------------------------------------------------------------------------

//Работа с данными - Документы - Рапоряжения на платеж поставщикам за газетно-журнальную продукцию
void __fastcall TMainCAP_2010Form::dictationOnPaymentItemClick(
      TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TDictationOnPaymentFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TDictationOnPaymentFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма DictationOnPaymentFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TDictationOnPaymentFrame*>(workFrame))->initFrame();
    miActiveName="dictationOnPaymentItem";
  }
}
//---------------------------------------------------------------------------
//Работа с данными - Документы - Товарный отчет
void __fastcall TMainCAP_2010Form::goodsReportItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TGoodsRepotrFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TGoodsRepotrFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма GoodsRepotrFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TGoodsRepotrFrame*>(workFrame))->initFrame();
    miActiveName="goodsReportItem";
  }
}
//---------------------------------------------------------------------------

//Аналитический отчет - Доходы от реализации ГЖП
void __fastcall TMainCAP_2010Form::incomsItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TIncomsFromRealizationNMPFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TIncomsFromRealizationNMPFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма IncomsFromRealizationNMPFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TIncomsFromRealizationNMPFrame*>(workFrame))->initFrame();
    miActiveName="incomsItem";
  }
}
//---------------------------------------------------------------------------
//Аналитический отчет - Справка о товародвижени (по постаащикам)
void __fastcall TMainCAP_2010Form::referenceOnMovementOfGoodsItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TReferenceOnMovementOfGoodsFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TReferenceOnMovementOfGoodsFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма ReferenceOnMovementOfGoodsFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TReferenceOnMovementOfGoodsFrame*>(workFrame))->initFrame();
    miActiveName="referenceOnMovementOfGoodsItem";
  }
}
//---------------------------------------------------------------------------
//Аналитический отчет - Справка о товародвижени (по подразделениям)
void __fastcall TMainCAP_2010Form::referenceOnMGForSDItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TReferenceOnMGForSDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TReferenceOnMGForSDFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма ReferenceOnMGForSDFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TReferenceOnMGForSDFrame*>(workFrame))->initFrame();
    miActiveName="referenceOnMGForSDItem";
  }
}
//---------------------------------------------------------------------------
//Финансовые результаты (по поставщикам)
void __fastcall TMainCAP_2010Form::financialResultsForPubItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TFinancialResultsForPubFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TFinancialResultsForPubFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма FinancialResultsForPubFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TFinancialResultsForPubFrame*>(workFrame))->initFrame();
    miActiveName="financialResultsForPubItem";
  }
}
//---------------------------------------------------------------------------
//Финансовые результаты (по подразднениям)
void __fastcall TMainCAP_2010Form::financialResultsForSDItemClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TFinancialResultsForSDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TFinancialResultsForSDFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма FinancialResultsForSDFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TFinancialResultsForSDFrame*>(workFrame))->initFrame();
    miActiveName="financialResultsForSDItem";
  }
}
//---------------------------------------------------------------------------

//Сообщения
//Сообщения - Полученные
void __fastcall TMainCAP_2010Form::receivedMessagesItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TReceivedMessagesFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TReceivedMessagesFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма ReceivedMessagesFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TReceivedMessagesFrame*>(workFrame))->initFrame();
    miActiveName="receivedMessagesItem";
  }
}
//---------------------------------------------------------------------------
//Сообщения - Отправленные
void __fastcall TMainCAP_2010Form::sentMessagesItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TMessagesWorkFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TMessagesWorkFrame(this,iniFileName,fbDB);
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма MessagesWorkFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TMessagesWorkFrame*>(workFrame))->initFrame();
    miActiveName="sentMessagesItem";
  }
}
//---------------------------------------------------------------------------

//Параметры настройки программы
void __fastcall TMainCAP_2010Form::adjustmentProgramParametersClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TProgrammSettingsFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TProgrammSettingsFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма ProgrammSettingsFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TProgrammSettingsFrame*>(workFrame))->initFrame();
    miActiveName="adjustmentProgramParameters";
  }
}
//---------------------------------------------------------------------------

//Администрирование - Обслуживание базы - Архивироание базы данных
void __fastcall TMainCAP_2010Form::archivingBDItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TArchivingBDFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TArchivingBDFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма ArchivingBDFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TArchivingBDFrame*>(workFrame))->initFrame();
    miActiveName="archivingBDItem";
  }
}
//---------------------------------------------------------------------------
//Администрирование - Обслуживание базы - Восстановление базы из ахива
void __fastcall TMainCAP_2010Form::recoverBDItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TRestoreDBFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TRestoreDBFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма RestoreDBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TRestoreDBFrame*>(workFrame))->initFrame();
    miActiveName="recoverBDItem";
  }
}
//---------------------------------------------------------------------------
//Администрирование - Обслуживание базы - Подготовка базы на новый период
void __fastcall TMainCAP_2010Form::preparingBaseForNewPeriodClick(TObject *Sender)
{
 if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TPreparingBaseForNewPeriodFrame") return;
  destroyFrames();

  try
  {
    workFrame=new TPreparingBaseForNewPeriodFrame(this,iniFileName,fbDB);
  }
  catch(Exception &exc)
  { MessageDlg("Ошибка создания фрейма PreparingBaseForNewPeriodFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TPreparingBaseForNewPeriodFrame*>(workFrame))->initFrame();
    miActiveName="reparingBaseForNewPeriod";
  }
}
//---------------------------------------------------------------------------
//Администрирование - Подготовка ключей настройки БД на подразделение
void __fastcall TMainCAP_2010Form::prepareKeysItemClick(TObject *Sender)
{
  if(workFrame!=NULL && workFrame->ClassName().operator AnsiString()=="TExchangeDataFrame" && (static_cast<TExchangeDataFrame*>(workFrame))->getType()=="prepareKeys") return;
  destroyFrames();
  try
  {
    workFrame=new TExchangeDataFrame(this,iniFileName,fbDB,"prepareKeys");
  }
  catch (Exception &exc)
  { MessageDlg("Ошибка создания фрейма rbSinchroF\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    workFrame=NULL;
  }
  if(workFrame!=NULL)
  { workFrame->Parent=workPanel;
    (static_cast<TExchangeDataFrame*>(workFrame))->initFrame();
    miActiveName="prepareKeysItem";
  }
}
//---------------------------------------------------------------------------

//Выход
void __fastcall TMainCAP_2010Form::exitItemClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

