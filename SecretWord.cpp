//����� ��� ������ � �������� (����������� � �������������)
#include <vcl.h>
#pragma hdrstop
#include "SecretWord.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//****************************************************************
// ����� "��������� �����" - ����������� � ������������� ������� *
//****************************************************************
__fastcall SecretWord::SecretWord(void)
{
  static char buf[130]={'1','2','3','4','5','6','7','8','9','0',\
'A','B','C','D','E','F','G','H','I','J','K','L','(',\
'M','N','O','P','Q','R','S','T','V','U','W','X','Y','Z','(',\
'a','b','c','d','e','f','g','h','i','j','k','l','(',\
'm','n','o','p','q','r','s','t','v','u','x','x','y','z','(',\
'�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','(',\
'�','�','�','�','�','�','�','�','�','�','�','�','�','�','(',\
'�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','�','(',\
'�','�','�','�','�','�','�','�','�','�','�','�','�','�','('};
  symbols=buf;
  p=new Graphics::TBitmap();
  p->LoadFromResourceName(reinterpret_cast<int>(HInstance), "MP");
}
//---------------------------------------------------------------------------
__fastcall SecretWord::~SecretWord(void)
{
  delete p;
}
//---------------------------------------------------------------------------
AnsiString __fastcall SecretWord::selectMap(void)
{
  randomize();
  short m=random(10);
  AnsiString ret;
  switch(m)
  {
    case 0: ret="A"; break;
    case 1: ret="B"; break;
    case 2: ret="C"; break;
    case 3: ret="D"; break;
    case 4: ret="E"; break;
    case 5: ret="F"; break;
    case 6: ret="G"; break;
    case 7: ret="H"; break;
    case 8: ret="I"; break;
    case 9: ret="K"; break;
  }
  return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall SecretWord::selectWay(void)
{
  randomize();
  short m=random(4);
  AnsiString ret;
  switch(m)
  {
    case 0: ret="A"; break;
    case 1: ret="B"; break;
    case 2: ret="C"; break;
    case 3: ret="D"; break;
  }
  return ret;
}
//---------------------------------------------------------------------------
AnsiString __fastcall SecretWord::generateSymbol(void)
{
  return AnsiString(symbols[random(130)]);
}
//---------------------------------------------------------------------------
//����������� ������� ������
AnsiString __fastcall SecretWord::encodeWord(AnsiString str)
{
  AnsiString ret;
  if(str.Length() > 50) str=str.SubString(0,50);
  int a=0, b=0;
  //Select a map
  ret=selectMap();
  if(ret.SubString(1,1)=="A") {a=30;}
  //Stay on the first point
  else if(ret.SubString(1,1)=="B") {a=0;}
  else if(ret.SubString(1,1)=="C") {a=15;}
  else if(ret.SubString(1,1)=="D") {a=45;}
  else if(ret.SubString(1,1)=="E") {a=60;}
  else if(ret.SubString(1,1)=="F") {a=60;b=16;}
  else if(ret.SubString(1,1)=="G") {a=15;b=16;}
  else if(ret.SubString(1,1)=="H") {a=45;b=16;}
  else if(ret.SubString(1,1)=="I") {a=30;b=16;}
  else if(ret.SubString(1,1)=="K") {a=0;b=16;}
  //Select a way of reading bmps
  ret+=selectWay();

  int num=1;
  if(ret.SubString(2,1)=="A") //������ ������, ������ ����
  { for(int y=b; y<=(b+15); y++)//������ ����
    { for(int x=a; x<=(a+14); x++)//������ ������
      { if(p->Canvas->Pixels[x][y]!=clWhite)
        { if(num<=str.Length()) ret+=str.SubString(num,1);
          else ret+="(";
          num++;
        }
        else
        { AnsiString s=generateSymbol();
          ret+=s;
        }
      }
    }
  }
  else if(ret.SubString(2,1)=="B") //C����� ����, c����� ������
  { for(int x=a; x<=(a+14); x++)//������ ������
    { for(int y=b; y<=(b+15); y++)//������ ����
      { if(p->Canvas->Pixels[x][y]!=clWhite)
        { if(num<=str.Length()) ret+=str.SubString(num,1);
          else ret+="(";
          num++;
        }
        else
        { AnsiString s=generateSymbol();
          ret+=s;
        }
      }
    }
  }
  else if(ret.SubString(2,1)=="C") //C���� �������, ������ ����
  { for(int x=(a+14); x>=a; x--)//C���� �������
    { for(int y=b; y<=(b+15); y++)//������ ����
      { if(p->Canvas->Pixels[x][y]!=clWhite)
        { if(num<=str.Length()) ret+=str.SubString(num,1);
          else ret+="(";
          num++;
        }
        else
        { AnsiString s=generateSymbol();
          ret+=s;
        }
      }
    }
  }
  else if(ret.SubString(2,1)=="D") //C����� ����, c���� �������
  { for(int y=b; y<=(b+15); y++)//������ ����
    { for(int x=(a+14); x>=a; x--)//C���� �������
      { if(p->Canvas->Pixels[x][y]!=clWhite)
        { if(num<=str.Length()) ret+=str.SubString(num,1);
          else ret+="(";
          num++;
        }
        else
        { AnsiString s=generateSymbol();
          ret+=s;
        }
      }
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
//������������� ������� ������
AnsiString __fastcall SecretWord::decodeWord(AnsiString str)
{
  AnsiString ret="";
  AnsiString pwd=str.SubString(3,240);
  int a=0, b=0;
  if(str.SubString(1,1)=="A") {a=30;}
  //Stay on the first point
  else if(str.SubString(1,1)=="B") {a=0;}
  else if(str.SubString(1,1)=="C") {a=15;}
  else if(str.SubString(1,1)=="D") {a=45;}
  else if(str.SubString(1,1)=="E") {a=60;}
  else if(str.SubString(1,1)=="F") {a=60;b=16;}
  else if(str.SubString(1,1)=="G") {a=15;b=16;}
  else if(str.SubString(1,1)=="H") {a=45;b=16;}
  else if(str.SubString(1,1)=="I") {a=30;b=16;}
  else if(str.SubString(1,1)=="K") {a=0;b=16;}

  int num=1;
  if(str.SubString(2,1)=="A") //������ ������, ������ ����
  { for(int y=b; y<=(b+15); y++)//������ ����
    { for(int x=a; x<=(a+14); x++)//������ ������
      { if(p->Canvas->Pixels[x][y]!=clWhite && pwd.SubString(num,1)!="(")
          ret+=pwd.SubString(num,1);
        num++;
      }
    }
  }
  else if(str.SubString(2,1)=="B") //C����� ����, c����� ������
  { for(int x=a; x<=(a+14); x++)//������ ������
    { for(int y=b; y<=(b+15); y++)//������ ����
      { if(p->Canvas->Pixels[x][y]!=clWhite && pwd.SubString(num,1)!="(")
          ret+=pwd.SubString(num,1);
        num++;
      }
    }
  }
  else if(str.SubString(2,1)=="C") //C���� �������, ������ ����
  { for(int x=(a+14); x>=a; x--)//C���� �������
    { for(int y=b; y<=(b+15); y++)//������ ����
      { if(p->Canvas->Pixels[x][y]!=clWhite && pwd.SubString(num,1)!="(")
          ret=ret+pwd.SubString(num,1);
        num++;
      }
    }
  }
  else if(str.SubString(2,1)=="D") //C����� ����, c���� �������
  { for(int y=b; y<=(b+15); y++)//������ ����
    { for(int x=(a+14); x>=a; x--)//C���� �������
      { if(p->Canvas->Pixels[x][y]!=clWhite && pwd.SubString(num,1)!="(")
          ret=ret+pwd.SubString(num,1);
        num++;
      }
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
