//---------------------------------------------------------------------------

#ifndef ShipmentNMPFunctionsH
#define ShipmentNMPFunctionsH
//---------------------------------------------------------------------------
//#include "RBF.h"
//#include "ReferenceBook.h"
#include "DBClasses.h"
#include "NextGridF.h"
//#include "P_NMP_F.h"
//#include "RP_Form.h"
//#include "MultiSelectSD.h"
//#include "GetDate.h"
//#include "Message.h"
//---------------------------------------------------------------------------
// - ����� - �������� ��� � �������������
// -- ����� �������������
AnsiString __fastcall formShipmentSubdivisionsSelectSQL(bool groupOnPostOffices,TDateTime dt1,TDateTime dt2);
TNextGridFrame * __fastcall cteateShipmentSubdivisionsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox);
void __fastcall subdivisionsAdditionalAdjustments(TGridFrame *gf);
TNextGridFrame * __fastcall cteateShipmentPostOfficesFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox);
// -- ����� ����������� � ������������� ���
AnsiString __fastcall formShipmentNMPSelectSQL(AnsiString sdID,bool groupOnPostOffices,bool groupOnPublishers,TDateTime dt1,TDateTime dt2);
AnsiString __fastcall formShipmentNMPSearchSQL(AnsiString pID,bool groupOnPostOffices,bool groupOnPublishers,TDateTime dt1,TDateTime dt2);
TNextGridFrame * __fastcall createShipmentNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox);
TNextGridFrame * __fastcall createShipmentPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox);

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

