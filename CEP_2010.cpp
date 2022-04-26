//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "MainCAP_2010.h"
//---------------------------------------------------------------------------
USEFORM("MainCAP_2010.cpp", MainCAP_2010Form);
USEFORM("AdjustConnection.cpp", AdjustConnectionForm);
USEFORM("SelectBase.cpp", SelectBaseForm);
USEFORM("Entry.cpp", EntryForm);
USEFORM("FoldersTreeF.cpp", FoldersTreeFrame); /* TFrame: File Type */
USEFORM("ColumnsConfig.cpp", ColumnsConfigForm);
USEFORM("GridF.cpp", GridFrame); /* TFrame: File Type */
USEFORM("ContentF.cpp", ContentFrame); /* TFrame: File Type */
USEFORM("ContentConfig.cpp", ContentConfigForm);
USEFORM("TreeViewF.cpp", TreeViewFrame); /* TFrame: File Type */
USEFORM("GridContentF.cpp", GridContentFrame); /* TFrame: File Type */
USEFORM("NextGridF.cpp", NextGridFrame); /* TFrame: File Type */
USEFORM("ChangePass.cpp", ChangePassForm);
USEFORM("RBF.cpp", RBFrame); /* TFrame: File Type */
USEFORM("ReferenceBook.cpp", RBForm);
USEFORM("UniversalSelectPeriod.cpp", UniversalSelectPeriodForm);
USEFORM("AddEditSD.cpp", AddEditSDForm);
USEFORM("AddEditPublisher.cpp", AddEditPublisherForm);
USEFORM("AddEditNMP.cpp", AddEditNMPForm);
USEFORM("RP_Form.cpp", RPForm);
USEFORM("P_NMP_F.cpp", P_NMP_Frame); /* TFrame: File Type */
USEFORM("RBWorksF.cpp", RBWorksFrame); /* TFrame: File Type */
USEFORM("AddEditTemplate.cpp", AddEditTemplateForm);
USEFORM("MultiSelectSD.cpp", MultiSelectSDForm);
USEFORM("CopyTemplates.cpp", CopyTemplatesForm);
USEFORM("SelectPeriod.cpp", SelectPeriodForm);
USEFORM("RemovingTemplates.cpp", RemovingTemplatesForm);
USEFORM("ReCalc.cpp", ReCalcForm);
USEFORM("TemplatesEditF.cpp", TemplatesEditFrame); /* TFrame: File Type */
USEFORM("AddEditExits.cpp", AddEditExitsForm);
USEFORM("AddEditFacts.cpp", AddEditFactsForm);
USEFORM("GetDate.cpp", GetDateForm);
USEFORM("OrderingNMPF.cpp", OrderingNMPFrame); /* TFrame: File Type */
USEFORM("ArrivalNMPF.cpp", ArrivalNMPFrame); /* TFrame: File Type */
USEFORM("MessageParametersForm.cpp", MessageParametersForm);
USEFORM("ShipmentNMPF.cpp", ShipmentNMPFrame); /* TFrame: File Type */
USEFORM("SelectRicipient.cpp", SelectRicipientForm);
USEFORM("AddEditMessage.cpp", AddEditMessageForm);
USEFORM("MessagesWorkF.cpp", MessagesWorkFrame); /* TFrame: File Type */
USEFORM("ReturnNMPfromSDF.cpp", ReturnNMPfromSDFrame); /* TFrame: File Type */
USEFORM("LoadDataAboutReturnNMPFromSD.cpp", LoadDataAboutReturnNMPFromSDForm);
USEFORM("ReturnNMPtoPublishersF.cpp", ReturnNMPtoPublishersFrame); /* TFrame: File Type */
USEFORM("SpisanieNMPF.cpp", SpisanieNMPFrame); /* TFrame: File Type */
USEFORM("VozvratNMPF.cpp", VozvratNMPFrame); /* TFrame: File Type */
USEFORM("UnloadDataForBookKeepingF.cpp", UnloadDataForBookKeepingFrame); /* TFrame: File Type */
USEFORM("FormApplicationsForNMPFromSDF.cpp", FormApplicationsForNMPFromSDFrame); /* TFrame: File Type */
USEFORM("LoadDataFromApplications.cpp", LoadDataFromApplicationsForm);
USEFORM("AddEditApplicationForNMP.cpp", AddEditApplicationForNMPForm);
USEFORM("FormApplicationsForNMPToPublishersF.cpp", FormApplicationsForNMPToPublishersFrame); /* TFrame: File Type */
USEFORM("BackUpBaseF.cpp", BackUpBaseFrame); /* TFrame: File Type */
USEFORM("ArchivingBDF.cpp", ArchivingBDFrame); /* TFrame: File Type */
USEFORM("RestoreBaseF.cpp", RestoreBaseFrame); /* TFrame: File Type */
USEFORM("RestoreDBF.cpp", RestoreDBFrame); /* TFrame: File Type */
USEFORM("DBTreeViewF.cpp", DBTreeViewFrame); /* TFrame: File Type */
USEFORM("UsersWorkF.cpp", UsersWorkFrame); /* TFrame: File Type */
USEFORM("ProgrammSettingsF.cpp", ProgrammSettingsFrame); /* TFrame: File Type */
USEFORM("AddEditPP.cpp", AddEditPPForm);
USEFORM("IncomsFromRealizationNMPF.cpp", IncomsFromRealizationNMPFrame); /* TFrame: File Type */
USEFORM("ReferenceOnMovementOfGoodsF.cpp", ReferenceOnMovementOfGoodsFrame); /* TFrame: File Type */
USEFORM("PreparingBaseForNewPeriodF.cpp", PreparingBaseForNewPeriodFrame); /* TFrame: File Type */
USEFORM("DictationOnPaymentF.cpp", DictationOnPaymentFrame); /* TFrame: File Type */
USEFORM("AddRecordsToRool.cpp", AddRecordsToRoolForm);
USEFORM("GoodsRepotrF.cpp", GoodsRepotrFrame); /* TFrame: File Type */
USEFORM("ReferenceOnMGForSDF.cpp", ReferenceOnMGForSDFrame); /* TFrame: File Type */
USEFORM("FinancialResultsForPubF.cpp", FinancialResultsForPubFrame); /* TFrame: File Type */
USEFORM("FinancialResultsForSDF.cpp", FinancialResultsForSDFrame); /* TFrame: File Type */
USEFORM("ViewNMPBySD.cpp", ViewNMPBySDForm);
USEFORM("ExchangeDataF.cpp", ExchangeDataFrame); /* TFrame: File Type */
USEFORM("UnloadDataToSDF.cpp", UnloadDataToSDFrame); /* TFrame: File Type */
USEFORM("ReceivedMessagesF.cpp", ReceivedMessagesFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
#define MUTEX_NAME "CEP_2010"
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
HANDLE hMutex=0;

  try
  { //Проверяем, не запущено ли уже приложение?
    hMutex = OpenMutex( MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
    if(hMutex)
    { MessageDlg("Программа \"Розница газетно-журнальной продукции\" уже запущенна",mtWarning,TMsgDlgButtons() << mbOK,0);
      return 0;
    }
    hMutex = CreateMutex(NULL, false, "CEP_2010");

    Application->Initialize();
    Application->Title = "Розница газетно-журнальной продукции";
    Application->CreateForm(__classid(TMainCAP_2010Form), &MainCAP_2010Form);
                 if(MainCAP_2010Form->initMainForm()==true) Application->Run();
    else MainCAP_2010Form->Close();
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
  }
  catch (...)
  {
    try
    {
      throw Exception("");
    }
    catch (Exception &exception)
    {
      Application->ShowException(&exception);
    }
  }
  if(hMutex)
  { ReleaseMutex(hMutex);
    CloseHandle(hMutex);
  }
  return 0;
}
//---------------------------------------------------------------------------
