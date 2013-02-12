//---------------------------------------------------------------------------

#ifndef VclLocalizerH
#define VclLocalizerH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "TinyXML.h"

//---------------------------------------------------------------------------
//
// Memory operations used: malloc, realloc, free : [ stdlib ]
//
// NotSaved: TStringGrid
// Exclude: TToolBar        - No useful text fields
// TTypeKind { tkUnknown, tkInteger, tkChar, tkEnumeration, tkFloat, tkString, tkSet, tkClass, tkMethod, tkWChar, tkLString, tkWString, tkVariant, tkArray, tkRecord, tkInterface, tkInt64, tkDynArray };
//---------------------------------------------------------------------------
#ifndef DEFCALL
#define DEFCALL __stdcall
#endif
//---------------------------------------------------------------------------
#define OBJDATA         "Data"
#define OBJTXTTEXT      "Text"
#define OBJTXTCAPTION   "Caption"
#define OBJTXTCOMMAT    "CommaText"
#define LOCTYPESTR      "string"
#define LOCOBJTYPE      "Type"
#define LOCOBJNAME      "Name"
#define LOCOBJVALUE     "Value"
#define LOCOBJNAMEF     "Name"
#define LOCLANGID       "LangId"
#define LOCCLASSCOMP    "Class"
#define LOCPROPCOMPN    "Property"
#define LOCLANGDESCR    "Description"
#define LOCABASENODE    "AppLocalization"
#define LOCFORMSNODE    "ApplicationForms"
#define LOCSTRINGNODE   "LocalizedStrings"

AnsiString ExtractTypeName(PTypeInfo TypeInf);
AnsiString ExtractPropName(PPropInfo PropInf);
AnsiString ExtractPropTypeName(PPropInfo PropInf);
//---------------------------------------------------------------------------
class CLocalizer
{
 private:
   TStringList   *LangsList;
   TStringList   *ExcludeList;
   TiXmlDocument *XmlLocFile;

   void DEFCALL SetDefaults(void);
   bool DEFCALL Initialize(void);
   bool DEFCALL UnInitialize(void);
   int  DEFCALL BuildLanguagesList(void);                                  // Returns length of the list
   int  DEFCALL SaveComponentText(TiXmlNode *RootNode, TObject *CurObj, TiXmlNode **ChildNode);
   int  DEFCALL LoadComponentText(TiXmlNode *RootNode, TObject *CurObj);
   TiXmlNode* DEFCALL FindFormNode(TiXmlNode* FormsRoot, LPSTR FormName);
   TiXmlNode* DEFCALL GetLangRootNode(LPSTR NodeName, LPSTR CodePage, LPSTR LangName, LPSTR LangDescr, DWORD LangID);
   TiXmlNode* DEFCALL GetLangFormsRoot(LPSTR LangName);
   TiXmlNode* DEFCALL GetLocStringsRoot(LPSTR LangName);

   static AnsiString DEFCALL ExtractTypeName(PTypeInfo TypeInf);
   static AnsiString DEFCALL ExtractPropName(PPropInfo PropInf);
   static AnsiString DEFCALL ExtractPropTypeName(PPropInfo PropInf);

 public:
   int   DEFCALL GetLangCount(void);
   int   DEFCALL LoadLocFile(LPSTR LocFile);                          // Returns Lang count ( -1 = ERROR )
   int   DEFCALL SaveLocFile(LPSTR LocFile);
   int   DEFCALL SetLocDocument(LPSTR XmlLocDocument);
   int   DEFCALL SetAppLanguage(LPSTR LangName);                      // Returns number of failed component names
   int   DEFCALL SetAppLanguage(int LangIndex){return SetAppLanguage(GetLangName(LangIndex));}
   int   DEFCALL BuildCurrentLanguage(LPSTR LangName, LPSTR LangDescr, LPSTR CodePage, DWORD LangID);       // Returns number of components saved
   int   DEFCALL GetLangIndexByName(LPSTR LangName);
   int   DEFCALL GetLangIndexByID(DWORD LangID);
   DWORD DEFCALL GetLangID(int LangIndex);
   DWORD DEFCALL GetLangID(LPSTR LangName){return GetLangID(GetLangIndexByName(LangName));}
   LPSTR DEFCALL GetLangName(int LangIndex);
   LPSTR DEFCALL GetLangDescription(int LangIndex);
   LPSTR DEFCALL GetLangDescription(LPSTR LangName){return GetLangDescription(GetLangIndexByName(LangName));}
   LPSTR DEFCALL GetLocString(int LangIndex, LPSTR StrName){return GetLocString(GetLangName(LangIndex), StrName);}
   LPSTR DEFCALL GetLocString(LPSTR LangName, LPSTR StrName);
   int   DEFCALL SetLocString(LPSTR StrName, LPSTR StrValue, LPSTR CodePage, LPSTR LangName, LPSTR LangDescr, DWORD LangID);
   int   DEFCALL SetLocString(LPSTR StrName, LPSTR StrValue, LPSTR CodePage, int LangIndex, LPSTR LangDescr, DWORD LangID){return SetLocString(StrName,StrValue,CodePage, GetLangName(LangIndex),LangDescr,LangID);}
   void  DEFCALL ClearExcludeList(void);
   void  DEFCALL RemoveFromExcludeList(LPSTR ComponentName);
   void  DEFCALL AddToExcludeList(LPSTR ComponentName);
   bool  DEFCALL IsInExcludeList(LPSTR ComponentName);

	   DEFCALL ~CLocalizer();
 	   DEFCALL CLocalizer(void);
};
typedef CLocalizer* PCLocalizer;
//---------------------------------------------------------------------------
#endif
