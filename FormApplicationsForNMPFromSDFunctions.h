//---------------------------------------------------------------------------
#ifndef FormApplicationsForNMPFromSDFunctionsH
#define FormApplicationsForNMPFromSDFunctionsH
//---------------------------------------------------------------------------
#include "DBClasses.h"
//---------------------------------------------------------------------------
// - Фрейм - формирование заявок на ГЖП от подразделений
// -- фрейм подразделений
AnsiString __fastcall formSubdivisionsSelectSQL(TDateTime dt);
TNextGridFrame * __fastcall cteateSubdivisionsFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel);
// -- фрейм с заказанной ГЖП
AnsiString __fastcall formAppNMPSearchSQL(void);
AnsiString __fastcall formAppNMPSelectSQL(AnsiString sd_id,TDateTime tempDate);
TNextGridFrame * __fastcall createAppNMPFrame(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,FBDatabase *fbDB,TPanel *workPanel,TEdit *SEdit,TJvXPCheckbox *ConditionSBox);
void __fastcall appNMPAdditionalAdjustments(TGridFrame *gf);
//Функции формироания сообщений
// - головная функция формирования сообщений с шаблоном заявки на поставку ГЖП на новый период
void createMessage(TComponent *owner,AnsiString iniFileName);
// - функция формирования сообщений
bool __fastcall createMessagesForSubdivisions(TComponent *owner,TDateTime date,AnsiString period,AnsiString subject,AnsiString content,bool replyFLG,bool delFilesFLG);
// - функция формирования файла с шаблоном заявки
AnsiString __fastcall createApplicationsForNMPFromSD(TComponent *owner,TDateTime tempDate,AnsiString period,AnsiString dirName);

#endif

/*Шаблон для создания фрейма со справочником

//Строка описания таблицы:
AnsiString tableInfo="п1,п2,п3,п4,п5,п6";
// где:
// п1 - наименование_таблицы_со_справочником,
// п2 - заголовок_справочника (выводится в заголовке окна справочника),
// п3 - наименование_ключевого_поля,
// п4 - наименование_поля_просмотра, (поля, по которому производится отбор записи)
// п5 - заголовок_корневой_папки_справочника,
// п6 - заголовок_папки_для_удаленных_записей

//Строка с описанием полей:
AnsiString fieldsInfo="оп1;оп2;...опN";
// где: оп1;оп2;оп3;...опN - описатель поля 1, оп2 - описатель поля2, опN - описатель поля N
// формат описателя поля:
// оп=п1,п2,п3,п4,п5,п6,п7,п8
// где:
// п1 - наименование_поля_в_SQL_запросе,
// п2 - наименование_поля_в_DataSet-е,
// п3 - заголовок_поля_в_Gride-е,
// п4 - расположение_поля_в_Gride-е, (Left,Center,Right)
// п5 - флаг_"Только_чтение",
// п6 - флаг_"Отображать_в_Gride-е,
// п7 - флаг_"Заголовок_поля_в_Grid-е_являетя_кнопкой"
// п8 - флаг_"Возможен поиск по модели"
// п9 - формат вывода в Grid-е

//Строка с описанием Footer полей
AnsiString footerFieldsInfo="оп1,оп2,...опN";
// где: оп1;оп2;оп3;...опN - описатель поля 1, оп2 - описатель поля2, опN - описатель поля N
// формат описателя поля:
// оп=п1,п2,п3,п4,п5
// где:
// п1 - наименование поля, для которого будет выведено значение в строке Footer
// п2 - тип значения (fvtAvg,fvtCount,fvtFieldValue,fvtStaticText,fvtSum)
// п3 - формат вывода
// п4 - значение
// п4 - положение

//Строка с описанием полей, отображаемых в области содержания записи
AnsiString contentsInfo="оп1;оп2;...опN";
// формат описателя поля:
// оп=п1,п2
// где:
// п1 - наименование_поля,
// п2 - заголовок_поля

//Шаблон функции создания фрейма справочника
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
bool searchAble=;      //флаг "Поск записи по значению активного поля"
bool directEdit=;      //флаг "Редактирование в Grid-е"
bool tvViewAble=;      //флаг "Отображать область выбора папки"
bool contentViewAble=; //флаг "Отображать область содержимого записи"

TRBFrame *RBFrame;

  try
  {
    RBFrame=new TReferenceBookFrame(TComponent* Owner,AnsiString OwnerN,\
FBDatabase *fb_DB,AnsiString iniFN,AnsiString tableInf,AnsiString fieldsInf,\
AnsiString contentsInf,bool TVViewAble,bool NeedsSearchingForModels);
  }
  catch (Exception &exc)
  {
    MessageDlg("Ошибка создания TRBFrame\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
    return NULL;
  }
  return RBFrame;
}

*/

