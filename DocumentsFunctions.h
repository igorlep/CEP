//---------------------------------------------------------------------------

#ifndef DocumentsFunctionsH
#define DocumentsFunctionsH
//---------------------------------------------------------------------------
#include "GridF.h"
//---------------------------------------------------------------------------
//****************************************************
//* ������ �� ������ �� ��� � ������������ �� ������ *
//****************************************************
//����� � ��������������
// - ������� ������������ select sql �������
AnsiString __fastcall formDictationsSelectSQL(TDateTime date);
// - ������� �������� ������ � �������������
class TGridFrame * __fastcall createDictationsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);

//����� ��� ���������� ������� � ������
AnsiString __fastcall formPubSelectSQL(TDateTime dDate);
class TGridFrame * __fastcall createAddRecordsToRoolFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);

//������� ��� ������ �������������� �� ������
// - ������ ������������
void __fastcall printDictation(TComponent *owner,TpFIBDatabase *db,TpFIBDataSet *pDS,TpFIBDataSet *dDS);
// - �������� ������� ������������ ��������� � ������������� �� ������
//void createMessage(TComponent *owner,AnsiString iniFileName);
// - ������� ������������ ���������
//bool __fastcall createMessagesForPublishers(TComponent *owner,int type,int kind,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG);
// - ������� ������������ ����� � �������������
//AnsiString __fastcall createDictationFile(TComponent *owner,AnsiString dirName);

//******************
//* �������� ����� *
//******************
// ������� ������������ select sql ������� ��� ������ � ������� �������
AnsiString __fastcall formLeftSelectSQL(TDateTime date1,TDateTime date2,int type);
AnsiString __fastcall formRihgtSelectSQL(TDateTime date1,TDateTime date2,int type);
//������� �������� ������ � ������� �������
class TGridFrame * __fastcall createLeftFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type,TJvNavPanelHeader *header,TStatusBar *bar);
class TGridFrame * __fastcall createRightFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,int type,TJvNavPanelHeader *header,TStatusBar *bar);

//����� - ������ �� �����������
// - ������� ������������ select sql �������
AnsiString __fastcall formArrivalFromPubSelectSQL(TDateTime date1,TDateTime date2);
// - ������� �������� ������ � ��������
class TGridFrame * __fastcall createArrivalFromPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);
//����� - �������� � �������������
// - ������� ������������ select sql �������
AnsiString __fastcall formShipmentToSubSelectSQL(TDateTime date1,TDateTime date2);
// - ������� �������� ������ � ���������
class TGridFrame * __fastcall createShipmentToSubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);

//����� - ������� �� �������������
// - ������� ������������ select sql �������
AnsiString __fastcall formReturnFromSubSelectSQL(TDateTime date1,TDateTime date2);
// - ������� �������� ������ � ��������� �� �������������
class TGridFrame * __fastcall createReturnFromSDFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);
//����� - ������� �����������
// - ������� ������������ select sql �������
AnsiString __fastcall formReturnToPubSelectSQL(TDateTime date1,TDateTime date2);
// - ������� �������� ������ � ���������
class TGridFrame * __fastcall createReturnToPubFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);


#endif

/*������ ��� �������� ������ �� ������������

//������ �������� �������:
AnsiString tableInfo="�1,�2,�3,�4,�5,�6";
// ���:
// �1 - ������������_�������_��_������������,
// �2 - ���������_����������� (��������� � ��������� ���� �����������),
// �3 - ������������_���������_����,
// �4 - ������������_����_���������, (����, �� �������� ������������ ����� ������)
// �5 - ���������_��������_�����_�����������,
// �6 - ���������_�����_���_���������_�������

//������ � ��������� �����:
AnsiString fieldsInfo="��1;��2;...��N";
// ���: ��1;��2;��3;...��N - ��������� ���� 1, ��2 - ��������� ����2, ��N - ��������� ���� N
// ������ ��������� ����:
// ��=�1,�2,�3,�4,�5,�6,�7,�8
// ���:
// �1 - ������������_����_�_SQL_�������,
// �2 - ������������_����_�_DataSet-�,
// �3 - ���������_����_�_Gride-�,
// �4 - ������������_����_�_Gride-�, (Left,Center,Right)
// �5 - ����_"������_������",
// �6 - ����_"����������_�_Gride-�,
// �7 - ����_"���������_����_�_Grid-�_�������_�������"
// �8 - ����_"�������� ����� �� ������"
// �9 - ������ ������ � Grid-�

//������ � ��������� Footer �����
AnsiString footerFieldsInfo="��1,��2,...��N";
// ���: ��1;��2;��3;...��N - ��������� ���� 1, ��2 - ��������� ����2, ��N - ��������� ���� N
// ������ ��������� ����:
// ��=�1,�2,�3,�4,�5
// ���:
// �1 - ������������ ����, ��� �������� ����� �������� �������� � ������ Footer
// �2 - ��� �������� (fvtAvg,fvtCount,fvtFieldValue,fvtStaticText,fvtSum)
// �3 - ������ ������
// �4 - ��������
// �4 - ���������

//������ � ��������� �����, ������������ � ������� ���������� ������
AnsiString contentsInfo="��1;��2;...��N";
// ������ ��������� ����:
// ��=�1,�2
// ���:
// �1 - ������������_����,
// �2 - ���������_����

//������ ������� �������� ������ �����������
TReferenceBookFrame * __fastcall create???RB_Frame(TComponent* Owner,TPanel *workPanel,AnsiString iniFileName,FBDatabase *fbDB)
{
AnsiString tableInfo=",,,,,";
AnsiString fieldsInfo =",,,,,,;";
           fieldsInfo+=",,,,,,;";
           fieldsInfo+=",,,,,,;";
           fieldsInfo+=",,,,,,;";
           ...
           fieldsInfo+=",,,,,,";
AnsiString contentsInfo= ",;";
           contentsInfo+=",;";
           contentsInfo+=",;";
           ...
           contentsInfo+=",";
bool searchAble=;      //���� "���� ������ �� �������� ��������� ����"
bool directEdit=;      //���� "�������������� � Grid-�"
bool tvViewAble=;      //���� "���������� ������� ������ �����"
bool contentViewAble=; //���� "���������� ������� ����������� ������"

TRBFrame *RBFrame;

  try
  {
    RBFrame=new TReferenceBookFrame(TComponent* Owner,AnsiString OwnerN,\
FBDatabase *fb_DB,AnsiString iniFN,AnsiString tableInf,AnsiString fieldsInf,\
AnsiString contentsInf,bool TVViewAble,bool NeedsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("������ �������� TRBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  return RBFrame;
}

*/

