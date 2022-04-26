//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "DifferentFunctions.h"
#include "UniversalSelectPeriod.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "JvComponent"
#pragma link "JvExControls"
#pragma link "JvXPButtons"
#pragma link "JvXPCore"
#pragma link "Placemnt"
#pragma link "JvNavigationPane"
#pragma link "CSPIN"
#pragma link "JvXPCheckCtrls"
#pragma link "ToolEdit"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
//TUniversalSelectPeriodForm *UniversalSelectPeriodForm;
//---------------------------------------------------------------------------
__fastcall TUniversalSelectPeriodForm::TUniversalSelectPeriodForm(TComponent* Owner,AnsiString iniFN,AnsiString iniSN,AnsiString caption,TDateTime &dt1,TDateTime &dt2)
        : TForm(Owner),iniFileName(iniFN),iniSectionName(iniSN),date1(dt1),date2(dt2)
{
  FormStorage->IniFileName=iniFileName;
  FormStorage->IniSection=iniSectionName;
  TIniFile* ini=new TIniFile(iniFileName);
  date1=ini->ReadDate(iniSectionName,"Date1",date1);
  date2=ini->ReadDate(iniSectionName,"Date2",date2);
  delete ini;

  header->Caption=caption;
  Month->Text="";
  if(date1==date2)
  { DayButton->Checked=true;
    Day->Date=date1;
    MonthCheckBox->Checked=false;
  }
  else
  { unsigned short y1,m1,d1, y2,m2,d2;
    char *mn[12]={"€нварь","февраль","март","апрель","май","июнь","июль","август","сент€брь","окт€брь","но€брь","декабрь"};
    date1.DecodeDate(&y1,&m1,&d1);
    date2.DecodeDate(&y2,&m2,&d2);
    if(y1==y2)
    { if(m1==m2)
      { if(d1==1)
        { if(d2==lastDayOfMonth(y2,m2))
          { MonthButton->Checked=true;
            Month->Text=AnsiString(mn[m1-1])+" "+IntToStr(y1);
            MonthPosition=m1;
            YearCheckBox->Checked=false;
          }
          else
          { DayButton->Checked=true;
            Day->Date=date2;
            MonthCheckBox->Checked=true;
          }
        }
        else
        { IntButton->Checked=true;
          IntDate1->Date=date1;
          IntDate2->Date=date2;
        }
      }
      else
      { if(date1==TDateTime(y1,1,1) && date2==TDateTime(y2,m2,lastDayOfMonth(y2,m2)))
        { MonthButton->Checked=true;
          YearCheckBox->Checked=true;
          Month->Text=AnsiString(mn[m2-1])+" "+IntToStr(y2);
          MonthPosition=m2;
        }
        else
        { IntButton->Checked=true;
          IntDate1->Date=date1;
          IntDate2->Date=date2;
        }
      }
    }
    else
    { IntButton->Checked=true;
      IntDate1->Date=date1;
      IntDate2->Date=date2;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::FormDestroy(TObject *Sender)
{
  TIniFile* ini=new TIniFile(iniFileName);
  ini->WriteDate(iniSectionName,"Date1",date1);
  ini->WriteDate(iniSectionName,"Date2",date2);
  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::JXPBCancelClick(
      TObject *Sender)
{
  Close();
  ModalResult=mrCancel;        
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::JXPBOKClick(TObject *Sender)
{
  if(MonthButton->Checked==true)
  { unsigned short y1,m1,d1=1, y2,m2,d2;
    m2=MonthPosition;
    AnsiString Year=Month->Text.SubString(Month->Text.Pos(" ")+1,4);
    y1=y2=StrToInt(Year);
    if(YearCheckBox->Checked==true) m1=1;
    else m1=m2;
    d2=lastDayOfMonth(y2,m2);
    date1=TDateTime(y1,m1,d1);
    date2=TDateTime(y2,m2,d2);
  }
  else if(DayButton->Checked==true)
  { date2=Day->Date;
    if(MonthCheckBox->Checked==true)
    { unsigned short y,m,d;
      date2.DecodeDate(&y,&m,&d);
      date1=TDateTime(y,m,1);
    }
    else date1=date2;
  }
  else
  { date1=IntDate1->Date;
    date2=IntDate2->Date;
  }
  Close();
  ModalResult=mrOk;        
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::MonthButtonClick(
      TObject *Sender)
{
  if(MonthButton->Checked==true && Month->Text.IsEmpty())
  { unsigned short y,m,d;
    char *mn[12]={"€нварь","февраль","март","апрель","май","июнь","июль","август","сент€брь","окт€брь","но€брь","декабрь"};
    date2.DecodeDate(&y,&m,&d);
    Month->Text=AnsiString(mn[m-1])+" "+IntToStr(y);
    MonthPosition=m;
  }
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::MonthUpDownDownClick(
      TObject *Sender)
{
unsigned short y,m,d;
char *mn[12]={"€нварь","февраль","март","апрель","май","июнь","июль","август","сент€брь","окт€брь","но€брь","декабрь"};
date2.DecodeDate(&y,&m,&d);

  m--;
  if(m==0){ m=12; y--;}
  Month->Text=AnsiString(mn[m-1])+" "+IntToStr(y);
  MonthPosition=m;
  date1=TDateTime(y,m,1);
  date2=TDateTime(y,m,lastDayOfMonth(y,m));
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::MonthUpDownUpClick(
      TObject *Sender)
{
unsigned short y,m,d;
char *mn[12]={"€нварь","февраль","март","апрель","май","июнь","июль","август","сент€брь","окт€брь","но€брь","декабрь"};
date2.DecodeDate(&y,&m,&d);

  m++;
  if(m > 12){ m=1; y++;}
  Month->Text=AnsiString(mn[m-1])+" "+IntToStr(y);
  MonthPosition=m;
  date1=TDateTime(y,m,1);
  date2=TDateTime(y,m,lastDayOfMonth(y,m));
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::DayButtonClick(TObject *Sender)
{
  if(DayButton->Checked==true && int(Day->Date)==0)
  {
    Day->Date=date2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TUniversalSelectPeriodForm::IntButtonClick(TObject *Sender)
{
  if(IntButton->Checked==true && (int(IntDate1->Date)==0 || int(IntDate1->Date)==0))
  { IntDate1->Date=date1;
    IntDate2->Date=date2;
  }
}
//---------------------------------------------------------------------------

