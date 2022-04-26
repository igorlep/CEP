//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "DBClasses.h"
#include "DifferentFunctions.h"
#include "SomeFunctions.h"
#include "DirectWorkExcel.h"
#include "LoadDataAboutReturnNMPFromSD.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "JvExMask"
#pragma link "JvToolEdit"
#pragma link "JvCheckListBox"
#pragma link "JvExCheckLst"
#pragma link "JvExComCtrls"
#pragma link "JvStatusBar"
#pragma link "JvProgressBar"
#pragma resource "*.dfm"
//TLoadDataAboutReturnNMPFromSDForm *LoadDataAboutReturnNMPFromSDForm;
//---------------------------------------------------------------------------
//**********
//* public *
//**********
__fastcall TLoadDataAboutReturnNMPFromSDForm::TLoadDataAboutReturnNMPFromSDForm(TComponent* Owner,AnsiString iniFN,FBDatabase *fbdb)
        : TForm(Owner),iniFileName(iniFN),fbDB(fbdb)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection="LoadDataAboutReturnNMPFromSDForm";
  workDir=getAppDir(); //SomeFunctions.h
  workPath=workDir+"In\\";
  if(!DirectoryExists(workPath)) CreateDir(workPath);
  directoryEdit->InitialDir=workPath;
  directoryEdit->Text=workPath;
  fileMask="rt_????-??_??.xls";
  fillCheckListBox(workPath+fileMask);
}
//---------------------------------------------------------------------------

//***********
//* private *
//***********
void __fastcall TLoadDataAboutReturnNMPFromSDForm::fillCheckListBox(AnsiString fm)
{
TSearchRec sr;
int atr=32;

  checkListBox->Clear();
  if(FindFirst(fm,atr,sr)==0)
  { do
      if ((sr.Attr & atr) == sr.Attr)
        checkListBox->Checked[checkListBox->Items->Add(sr.Name)]=true;
    while (FindNext(sr)==0);
    FindClose(sr);
  }
}
//---------------------------------------------------------------------------

//*************
//* published *
//*************
void __fastcall TLoadDataAboutReturnNMPFromSDForm::FormShow(
      TObject *Sender)
{
  checkListBox->SetFocus();
  if(checkListBox->Items->Count > 0)
  { checkListBox->ItemIndex=0;
    checkListBox->OnClick(this);
  }
}
//---------------------------------------------------------------------------
void __fastcall TLoadDataAboutReturnNMPFromSDForm::cancelButtonClick(
      TObject *Sender)
{
  SetCurrentDir(workDir);
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TLoadDataAboutReturnNMPFromSDForm::directoryEditAfterDialog(
      TObject *Sender, AnsiString &Name, bool &Action)
{
  workPath=Name+"\\";
  fillCheckListBox(workPath+fileMask);
}
//---------------------------------------------------------------------------
void __fastcall TLoadDataAboutReturnNMPFromSDForm::checkListBoxClick(
      TObject *Sender)
{
  if(checkListBox->Items->Count==0) return;
  AnsiString fn=checkListBox->Items->Strings[checkListBox->ItemIndex];
  unsigned short y;
  unsigned short m;
  unsigned short sd_id;
  TDateTime dt;
  FBQuery *fbRQ=fbDB->getFBQ_T1();
  AnsiString sql;

  //rt_????-??_??.xls
  y=StrToInt(fn.SubString(4,4));
  m=StrToInt(fn.SubString(9,2));
  sd_id=StrToInt(fn.SubString(12,2));
  dt=TDateTime(y,m,lastDayOfMonth(y,m));
  sql="select SD_NAME from SUBDIVISIONS_TABLE where SD_ID="+IntToStr(sd_id);
  if(fbRQ->execQuery("TLoadDataAboutReturnNMPFromSDForm::checkListBoxClick",sql)==false) return;
  statusBar->Panels->Items[0]->Text=dt.DateString();
  statusBar->Panels->Items[1]->Text=fbRQ->getQuery()->FieldByName("SD_NAME")->AsString;
  statusBar->Repaint();
  fbRQ->getQuery()->Close();
}
//---------------------------------------------------------------------------
//Выбор всех файлов
void __fastcall TLoadDataAboutReturnNMPFromSDForm::checkAllItemClick(
      TObject *Sender)
{
  for(int i=0; i<checkListBox->Items->Count; ++i) checkListBox->Checked[i]=true;
}
//---------------------------------------------------------------------------
//Отмена выбора
void __fastcall TLoadDataAboutReturnNMPFromSDForm::uncheckAllItemClick(
      TObject *Sender)
{
  for(int i=0; i<checkListBox->Items->Count; ++i) checkListBox->Checked[i]=false;
}
//---------------------------------------------------------------------------
//Загрузка данных
void __fastcall TLoadDataAboutReturnNMPFromSDForm::loadButtonClick(
      TObject *Sender)
{
AnsiString sql;
DirectWorkExcel *de=NULL;
FBQuery *fbWQ=fbDB->getFBQ_T2();

  //Проверим, есть ли выбранные для загрузки файлы
  int sf=0;
  for(int i=0; i<checkListBox->Items->Count; ++i)
    if(checkListBox->Checked[i]==true) sf++;

  if(sf==0)
  { MessageDlg("Нет выбранных для загрузки файлов",mtWarning,TMsgDlgButtons() << mbOK,0);
    return;
  }

  //Создаем нужные каталоги
  AnsiString archPath=workDir+"In\\";
  if(!DirectoryExists(archPath)) CreateDir(archPath);
  archPath+="Archive\\";
  if(!DirectoryExists(archPath)) CreateDir(archPath);
  archPath+="FromRaions\\";
  if(!DirectoryExists(archPath)) CreateDir(archPath);

  totalBar->Max=sf;
  totalBar->Position=0;
  //Загрузка данных
  for(int i=0; i<checkListBox->Items->Count; ++i)
  { if(checkListBox->Checked[i]==true)
    { AnsiString fileIn=workPath+checkListBox->Items->Strings[i];
      // Создаем инструмент для работы с Excel файлами
      try
      {
        de=new DirectWorkExcel(fileIn,1);
      }
      catch(const Exception &exc)
      { MessageDlg("Ошибка создания объекта DirectWorkExcel\n"+exc.Message,mtError,TMsgDlgButtons() << mbOK,0);
        return;
      }
      if(de->openFile(true)==false)
      { MessageDlg("Ошибка открытия Excel файла - "+fileIn,mtWarning,TMsgDlgButtons() << mbOK,0);
        if(de!=NULL) delete de;
        return;
      }
      de->setSheetNumber(1);
      //Считываем код подразделения, его наименование и отчетный период
      AnsiString sd_id=de->getCellValue(3,"A");
      AnsiString sd_name=de->getCellValue(3,"B");
      TDateTime op_date=getDateFromMonthYearString(de->getCellValue(3,"G"));
      unsigned short y,m,d;
      op_date.DecodeDate(&y,&m,&d);
      TDateTime date1=TDateTime(y,m,1), date2=TDateTime(y,m,lastDayOfMonth(y,m));
      statusBar->Panels->Items[0]->Text=de->getCellValue(3,"G");
      statusBar->Panels->Items[1]->Text=sd_id+" - "+sd_name;
      statusBar->Repaint();
      //Подсчитываем количество строк
      int ns=7;
      while(!de->getCellValue(ns,"A").IsEmpty()) ns++;
      stringsBar->Max=ns-7;
      stringsBar->Position=0;
      ns=7;
      //Стартуем транзакцию
      fbWQ->getTransaction()->StartTransaction();
      //Заносим данные в базу
      GetAsyncKeyState(VK_ESCAPE);
      while(!de->getCellValue(ns,"A").IsEmpty())
      { stringsBar->StepIt();
        //считываем значение поля "Код разнарядки"
        AnsiString fact_id=de->getCellValue(ns,"D");
        if(!fact_id.IsEmpty())
        { //считываем значения полей "Списать"
          int return_q=0;
          AnsiString s=de->getCellValue(ns,"G");
          if(!s.IsEmpty()) return_q=StrToInt(s);
          //считываем значения полей "Цена реализации"
          double real_price=0.0;
          s=de->getCellValue(ns,"H");
          if(!s.IsEmpty()) real_price=StrToFloat(s);
          //заносим данные
          sql="update FACTS_TABLE";
          if(return_q>0) sql+=" set RETURN_Q="+IntToStr(return_q);
          else sql+=" set RETURN_Q=0";
          if(fabs(real_price)>0.0001) sql+=", REAL_PRICE="+AnsiString().sprintf("%.2lf",real_price);
          else sql+=", REAL_PRICE=0.0";
          sql+=" where FACT_ID="+fact_id;
          if(fbWQ->execQuery("TLoadDataAboutReturnNMPFromSDForm::loadButtonClick",sql)==false)
          { if(de!=NULL) delete de;
            fbWQ->getTransaction()->Rollback();
            return;
          }
        }
        statusBar->Panels->Items[2]->Text="№ "+de->getCellValue(ns,"A")+" "+de->getCellValue(ns,"B");
        statusBar->Repaint();
        ns++;
        if((GetAsyncKeyState(VK_ESCAPE) << 1)!=0)
        { MessageDlg("Прекратить обработку файла?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0);
          if(MessageDlg("Прекратить обработку файла?",mtConfirmation,TMsgDlgButtons() << mbYes << mbNo,0) == mrYes)
          { //Закрываем Excel файл
            de->closeFile();
            //Откатываем транзакцию
            fbWQ->getTransaction()->Rollback();
            return;
          }
        }
      }
      if(fbWQ->getTransaction()->InTransaction) fbWQ->getTransaction()->Commit();
      //Закрываем входной файл
      if(de!=NULL)
      { de->closeFile();
        delete de; de=NULL;
      }
    }
    totalBar->StepIt();
  }
  stringsBar->Position=0;
  totalBar->Position=0;
  //Переносим выбранные файлы в папку workPath в папку Archive
  if(workPath!=archPath)
  { for(int i=0; i<checkListBox->Items->Count; ++i)
    { if(checkListBox->Checked[i]==true)
      { AnsiString fileIn=workPath+checkListBox->Items->Strings[i];
        AnsiString fn=archPath+ExtractFileName(fileIn);
        if(FileExists(fn)) DeleteFile(fn);
        RenameFile(fileIn,fn);
      }
    }
  }
  //Обновляем список доступных для загрузки файлов
  fillCheckListBox(workPath+fileMask);
}
//---------------------------------------------------------------------------

