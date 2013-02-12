//---------------------------------------------------------------------------


#pragma hdrstop

#include "VclLocalizer.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
AnsiString DEFCALL CLocalizer::ExtractTypeName(PTypeInfo TypeInf)
{
 BYTE SStrBuf[256];

 lstrcpyn((LPSTR)&SStrBuf,&TypeInf->Name[1],TypeInf->Name[0]+1);
 return (AnsiString)((LPSTR)&SStrBuf);
}
//---------------------------------------------------------------------------
AnsiString DEFCALL CLocalizer::ExtractPropName(PPropInfo PropInf)
{
 BYTE SStrBuf[256];

 lstrcpyn((LPSTR)&SStrBuf,&PropInf->Name[1],PropInf->Name[0]+1);
 return (AnsiString)((LPSTR)&SStrBuf);
}
//---------------------------------------------------------------------------
AnsiString DEFCALL CLocalizer::ExtractPropTypeName(PPropInfo PropInf)
{
 return ExtractTypeName((*PropInf->PropType));
}
//---------------------------------------------------------------------------
DEFCALL CLocalizer::~CLocalizer()
{
 UnInitialize();
}
//---------------------------------------------------------------------------
DEFCALL CLocalizer::CLocalizer(void)
{
 SetDefaults();
 Initialize();
}
//---------------------------------------------------------------------------
void  DEFCALL CLocalizer::SetDefaults(void)
{
 LangsList   = NULL;
 ExcludeList = NULL;
 XmlLocFile  = NULL;
}
//---------------------------------------------------------------------------
bool  DEFCALL CLocalizer::Initialize(void)
{
 UnInitialize();
 LangsList   = new TStringList();
 ExcludeList = new TStringList();
 XmlLocFile  = new TiXmlDocument();
 return true;
}
//---------------------------------------------------------------------------
bool  DEFCALL CLocalizer::UnInitialize(void)
{
 if(LangsList){delete(LangsList);LangsList=NULL;}
 if(ExcludeList){delete(ExcludeList);ExcludeList=NULL;}
 if(XmlLocFile){delete(XmlLocFile);LangsList=NULL;}
 return true;
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::GetLangCount(void)
{
 return LangsList->Count;
}
//---------------------------------------------------------------------------
void  DEFCALL CLocalizer::ClearExcludeList(void)
{
 ExcludeList->Clear();
}
//---------------------------------------------------------------------------
void  DEFCALL CLocalizer::RemoveFromExcludeList(LPSTR ComponentName)
{
 int Index = ExcludeList->IndexOf(ComponentName);
 if(Index >= 0)ExcludeList->Delete(Index);
}
//---------------------------------------------------------------------------
void  DEFCALL CLocalizer::AddToExcludeList(LPSTR ComponentName)
{
 int Index = ExcludeList->IndexOf(ComponentName);
 if(Index < 0)ExcludeList->Add(ComponentName);
}
//---------------------------------------------------------------------------
bool  DEFCALL CLocalizer::IsInExcludeList(LPSTR ComponentName)
{
 return (ExcludeList->IndexOf(ComponentName) >= 0);
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::SetLocString(LPSTR StrName, LPSTR StrValue, LPSTR CodePage, LPSTR LangName, LPSTR LangDescr, DWORD LangID)
{
 TiXmlNode *StrRoot;
 TiXmlNode *StringsRoot;

 if((StringsRoot=this->GetLangRootNode(LOCSTRINGNODE, CodePage, LangName, LangDescr, LangID)))
  {
   StrRoot = StringsRoot->FirstChild(StrName);
   if(StrRoot){StrRoot->ToElement()->SetAttribute(LOCOBJVALUE, StrValue);return 1;}
     else
      {
       StrRoot = StringsRoot->InsertEndChild(TiXmlElement(StrName));
       if(StrRoot){StrRoot->ToElement()->SetAttribute(LOCOBJVALUE, StrValue);return 2;}
      }
  }
 return 0;
}
//---------------------------------------------------------------------------
LPSTR DEFCALL CLocalizer::GetLocString(LPSTR LangName, LPSTR StrName)
{
 TiXmlNode *StrRoot;
 TiXmlNode *StringsRoot;

 if((StringsRoot=this->GetLocStringsRoot(LangName)))
  {
   StrRoot = StringsRoot->FirstChild(StrName);
   if(StrRoot)return (LPSTR)StrRoot->ToElement()->Attribute(LOCOBJVALUE);
  }
 return NULL;
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::BuildLanguagesList(void)
{
 LPSTR        LngDescr;
 TiXmlNode    *LangNode;
 TiXmlElement *XmlElem;
 AnsiString   LangString;

 LangsList->Clear();
 if(!(LangNode = XmlLocFile->FirstChild(LOCABASENODE)))return -1;
 LangNode = LangNode->FirstChild();
 while(LangNode)
  {
   XmlElem  = LangNode->ToElement();
   if(XmlElem)LngDescr = (LPSTR)XmlElem->Attribute(LOCLANGDESCR);
   if(LngDescr && XmlElem)
    {
     LangString = LangNode->Value();
     LangsList->AddObject((LangString+"="+LngDescr),(TObject*)LangNode);
    }
   LangNode = LangNode->NextSibling();
  }
 return LangsList->Count;
}
//---------------------------------------------------------------------------
DWORD DEFCALL CLocalizer::GetLangID(int LangIndex)
{
 int        Value = 0;
 TiXmlNode* CurNode;

 CurNode = ((TiXmlNode*)LangsList->Objects[LangIndex]);
 if(CurNode)CurNode->ToElement()->Attribute(LOCLANGID,&Value);
 return Value;
}
//---------------------------------------------------------------------------
LPSTR DEFCALL CLocalizer::GetLangName(int LangIndex)
{
 return ((LangIndex >= 0)&&(LangIndex < LangsList->Count))?((LangsList->Names[LangIndex].Trim()).c_str()):(NULL);    // Trim() makes copy of string - need to keep it
}
//---------------------------------------------------------------------------
LPSTR DEFCALL CLocalizer::GetLangDescription(int LangIndex)
{
 return (LangIndex >= 0)?((LangsList->ValueFromIndex[LangIndex].Trim()).c_str()):(NULL); // Trim() makes copy of string - need to keep it
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::GetLangIndexByID(DWORD LangID)
{
 int        Value  = 0;
 int        Result = -1;
 TiXmlNode* CurNode;

 for(int ctr=0;ctr<LangsList->Count;ctr++)
  {
   CurNode = ((TiXmlNode*)LangsList->Objects[ctr]);
   if(CurNode)CurNode->ToElement()->Attribute(LOCLANGID,&Value);
   if(CurNode && (Value == (int)LangID))return ctr;
  }
 return Result;
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::GetLangIndexByName(LPSTR LangName)
{
 return LangsList->IndexOfName(LangName);
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::LoadLocFile(LPSTR LocFile)
{
 int Result;
 Result = XmlLocFile->LoadFile(LocFile);
 this->BuildLanguagesList();
 return Result;
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::SaveLocFile(LPSTR LocFile)
{
 return XmlLocFile->SaveFile(LocFile);
}
//---------------------------------------------------------------------------
int   DEFCALL CLocalizer::SetLocDocument(LPSTR XmlLocDocument)
{
 if(!XmlLocDocument)return -1;
 this->XmlLocFile->Assign(XmlLocDocument);
 return this->BuildLanguagesList();
}
//---------------------------------------------------------------------------
TiXmlNode* DEFCALL CLocalizer::GetLangFormsRoot(LPSTR LangName)
{
 TiXmlNode  *BaseLRoot;
 TiXmlNode  *LangsRoot;
 TiXmlNode  *FormsRoot;

 if(!(BaseLRoot = this->XmlLocFile->FirstChild(LOCABASENODE)))return NULL;
 if(!(LangsRoot = BaseLRoot->FirstChild(LangName)))return NULL;
 if(!(FormsRoot = LangsRoot->FirstChild(LOCFORMSNODE)))return NULL;
 return FormsRoot;
}
//---------------------------------------------------------------------------
TiXmlNode* DEFCALL CLocalizer::GetLocStringsRoot(LPSTR LangName)
{
 TiXmlNode  *BaseLRoot;
 TiXmlNode  *LangsRoot;
 TiXmlNode  *FormsRoot;

 if(!(BaseLRoot = this->XmlLocFile->FirstChild(LOCABASENODE)))return NULL;
 if(!(LangsRoot = BaseLRoot->FirstChild(LangName)))return NULL;
 if(!(FormsRoot = LangsRoot->FirstChild(LOCSTRINGNODE)))return NULL;
 return FormsRoot;
}
//---------------------------------------------------------------------------
TiXmlNode* DEFCALL CLocalizer::GetLangRootNode(LPSTR NodeName, LPSTR CodePage, LPSTR LangName, LPSTR LangDescr, DWORD LangID)
{
 TiXmlNode  *BaseLRoot;
 TiXmlNode  *LangsRoot;
 TiXmlNode  *DestnRoot;
 AnsiString DocHeader;

 DestnRoot = NULL;
 if(CodePage)DocHeader = DocHeader.sprintf("Windows-%s", CodePage);
   else DocHeader = "Windows-1252";   // Russian
 if(XmlLocFile->NoChildren())
  {
   XmlLocFile->Clear();
   XmlLocFile->InsertEndChild(TiXmlDeclaration("1.0",DocHeader.c_str(),NULL));
   if(!(BaseLRoot = XmlLocFile->InsertEndChild(TiXmlElement(LOCABASENODE))))return NULL;
   if(!(LangsRoot = BaseLRoot->InsertEndChild(TiXmlElement(LangName))))return NULL;
   if(!(DestnRoot = LangsRoot->InsertEndChild(TiXmlElement(NodeName))))return NULL;
   LangsRoot->ToElement()->SetAttribute(LOCLANGDESCR, LangDescr);
   LangsRoot->ToElement()->SetAttribute(LOCLANGID, ("0x"+IntToHex((int)LangID,4)).c_str());
  }
   else
    {
     LangsRoot = XmlLocFile->FirstChild();
     if(CodePage)XmlLocFile->ReplaceChild(LangsRoot,TiXmlDeclaration("1.0",DocHeader.c_str(),NULL));
     BaseLRoot  = XmlLocFile->FirstChild(LOCABASENODE);
     if(BaseLRoot)LangsRoot = BaseLRoot->FirstChild(LangName);
     if(LangsRoot && LangDescr){LangsRoot->ToElement()->SetAttribute(LOCLANGDESCR, LangDescr);LangsRoot->ToElement()->SetAttribute(LOCLANGID, ("0x"+IntToHex((int)LangID,4)).c_str());}
     if(!LangsRoot)
      {
       if(!BaseLRoot){if(!(BaseLRoot = XmlLocFile->InsertEndChild(TiXmlElement(LOCABASENODE))))return NULL;}
       if(!(LangsRoot = BaseLRoot->InsertEndChild(TiXmlElement(LangName))))return NULL;
       if(!(DestnRoot = LangsRoot->InsertEndChild(TiXmlElement(NodeName))))return NULL;
       LangsRoot->ToElement()->SetAttribute(LOCLANGDESCR, LangDescr);
      }
       else
        {
         DestnRoot = LangsRoot->FirstChild(NodeName);
         if(!DestnRoot){if(!(DestnRoot = LangsRoot->InsertEndChild(TiXmlElement(NodeName))))return NULL;}
        }
    }
 this->BuildLanguagesList();
 return DestnRoot;
}
//---------------------------------------------------------------------------
TiXmlNode* DEFCALL CLocalizer::FindFormNode(TiXmlNode* FormsRoot, LPSTR FormName)
{
 TiXmlNode  *FormNode;

 if(!FormsRoot || !FormName)return NULL;
 FormNode = FormsRoot->FirstChild(LOCCLASSCOMP);
 while(FormNode)
  {
   if(lstrcmp(FormName, FormNode->ToElement()->Attribute(LOCOBJNAME)) == 0)return FormNode;
   FormNode = FormNode->NextSibling(LOCCLASSCOMP);
  }
 return NULL;
}
//---------------------------------------------------------------------------
int  DEFCALL CLocalizer::SetAppLanguage(LPSTR LangName)
{
 int        CompCtr;
 TiXmlNode  *FormNode;
 TiXmlNode  *FormsRoot;
 AnsiString CompName;

 CompCtr = 0;
 if((FormsRoot = this->GetLangFormsRoot(LangName)))
  {
   for(int ctr=0;ctr < Application->ComponentCount;ctr++)
    {
     CompName = Application->Components[ctr]->Name.Trim();
     FormNode = FindFormNode(FormsRoot, (LPSTR)CompName.data());
     if(CompName.IsEmpty() || !FormNode)continue;
     CompCtr += this->LoadComponentText(FormNode, Application->Components[ctr]);
    }
  }
 return CompCtr;
}
//---------------------------------------------------------------------------
int  DEFCALL CLocalizer::BuildCurrentLanguage(LPSTR LangName, LPSTR LangDescr, LPSTR CodePage, DWORD LangID)
{
 int        CompCtr;
 TiXmlNode  *FormsRoot;
 AnsiString CompName;

 CompCtr = 0;
 if((FormsRoot = this->GetLangRootNode(LOCFORMSNODE, CodePage, LangName, LangDescr, LangID)))
  {
   // Enumerate application`s forms
   for(int ctr=0;ctr < Application->ComponentCount;ctr++)
    {
     CompName = Application->Components[ctr]->Name.Trim();
     if(CompName.IsEmpty())continue;
     CompCtr += this->SaveComponentText(FormsRoot, Application->Components[ctr], NULL);
    }
  }
 return CompCtr;
}
//---------------------------------------------------------------------------
int DEFCALL CLocalizer::SaveComponentText(TiXmlNode *RootNode, TObject *CurObj, TiXmlNode **ChildNode)
{
 int        CompCtr;
 TObject    *ChildObj;
 TiXmlNode  *PropNode;
 TiXmlNode  *CompNode;
 TMetaClass *SomeClass;
 TComponent *CurComp;
 PTypeInfo  PropTpe;
 PTypeInfo  TypeInf;
 PTypeData  TypeDat;
 PPropInfo  PropInf;
 PPropList  PropLst;

 if(!RootNode || !CurObj)return 0;
 if(CurObj->InheritsFrom(__classid(TComponent)) &&
    ((TComponent*)CurObj)->Owner &&
   // ((TComponent*)CurObj)->Owner->InheritsFrom(__classid(TWinControl)) &&   // Allow exclusion of an entire forms
    this->IsInExcludeList(((TComponent*)CurObj)->Name.c_str()))return 0;  // Check if the component are excluded

 AnsiString ClassName;
 AnsiString PropName;
 AnsiString CompText;
 AnsiString CompData;

 CompCtr   = 1;
 PropLst   = NULL;
 PropNode  = NULL;
 CurComp   = (TComponent*)CurObj;
 ClassName = CurObj->ClassName();
 TypeInf   = (PTypeInfo)CurObj->ClassInfo();
 TypeDat   = GetTypeData(TypeInf);
 CompNode  = RootNode->InsertEndChild(TiXmlElement(LOCCLASSCOMP));
 CompNode->ToElement()->SetAttribute(LOCOBJTYPE, ClassName.c_str());

 if(TypeDat->PropCount)
  {
   PropLst = (PPropList)malloc(sizeof(PPropList)*TypeDat->PropCount);
   GetPropInfos(TypeInf, PropLst);
  }

 for(int ctr=0;ctr < TypeDat->PropCount;ctr++)
  {
   PropInf = (*PropLst)[ctr];
   PropTpe = (*PropInf->PropType);
   switch(PropTpe->Kind)
    {
     case tkClass:
       PropNode = NULL;
       ChildObj = GetObjectProp(CurObj, PropInf);
       if(ChildObj && !ChildObj->InheritsFrom(__classid(TControl)))this->SaveComponentText(CompNode, ChildObj, &PropNode);   // Recursive process classes, skip props like 'ActiveControl'
       if(PropNode)PropNode->ToElement()->SetAttribute(LOCOBJNAME, ExtractPropName(PropInf).c_str());
       break;

     case tkLString:
       CompText = GetPropValue(CurObj, PropInf, true);  // Do not make 'Trim()' for values !
       PropName = ExtractPropName(PropInf).Trim();
	   if(!CompText.IsEmpty())
        {
         if(PropName.AnsiCompare(LOCOBJNAMEF) == 0)CompNode->ToElement()->SetAttribute(LOCOBJNAMEF, CompText.c_str());  // Add object name as node attribute
           else
            {
             PropNode = CompNode->InsertEndChild(TiXmlElement(LOCPROPCOMPN));
             PropNode->ToElement()->SetAttribute(LOCOBJTYPE, ExtractPropTypeName(PropInf).c_str());
             PropNode->ToElement()->SetAttribute(LOCOBJNAME, ExtractPropName(PropInf).c_str());
             PropNode->ToElement()->SetAttribute(LOCOBJVALUE, CompText.c_str());
            }
        }
       break;
    }
  }

 if(CurObj->InheritsFrom(__classid(TCustomRadioGroup)))goto DropChilds;   // Buttons are saved as 'Strings' property
 if(!CompNode->NoChildren())goto Finish;

 // TComboBox, TListBox, TCheckListBox, TValueListEditor, TTabControl, ...
 if(CurObj->InheritsFrom(__classid(TStrings)))
  {
   CompText = ((TStrings*)CurObj)->CommaText;
   PropNode = CompNode->InsertEndChild(TiXmlElement(LOCPROPCOMPN));
   PropNode->ToElement()->SetAttribute(LOCOBJTYPE,  LOCTYPESTR);
   PropNode->ToElement()->SetAttribute(LOCOBJNAME,  OBJTXTCOMMAT);
   PropNode->ToElement()->SetAttribute(LOCOBJVALUE, CompText.c_str());

   CompData = "";
   for(int ctr=0;ctr < ((TStrings*)CurObj)->Count;ctr++)CompData += IntToHex((int)((TStrings*)CurObj)->Objects[ctr],8);
   if(!CompData.IsEmpty())PropNode->ToElement()->SetAttribute(OBJDATA, CompData.c_str());
   goto Finish;
  }

 // TStringGrid - by rows
 if(CurObj->InheritsFrom(__classid(TStringGrid)))
  {
   for(int ctr=0;ctr<((TStringGrid*)CurObj)->RowCount;ctr++)this->SaveComponentText(CompNode, ((TStringGrid*)CurObj)->Rows[ctr], NULL);
   goto Finish;
  }

 // TListView, ...
 if(CurObj->InheritsFrom(__classid(TCollection)))
  {
   for(int ctr=0;ctr<((TCollection*)CurObj)->Count;ctr++)this->SaveComponentText(CompNode, ((TCollection*)CurObj)->Items[ctr], NULL);
   goto Finish;
  }
 // TListView
 if(CurObj->InheritsFrom(__classid(TListItems)))
  {
   for(int ctr=0;ctr<((TListItems*)CurObj)->Count;ctr++)this->SaveComponentText(CompNode, ((TListItems*)CurObj)->Item[ctr], NULL);
   goto Finish;
  }
 // TListView : Do not have RTTI props ?
 if(CurObj->InheritsFrom(__classid(TListItem)))
  {
   CompText = ((TListItem*)CurObj)->Caption;
   PropNode = CompNode->InsertEndChild(TiXmlElement(LOCPROPCOMPN));
   PropNode->ToElement()->SetAttribute(LOCOBJTYPE,  LOCTYPESTR);
   PropNode->ToElement()->SetAttribute(LOCOBJNAME,  OBJTXTCAPTION);
   PropNode->ToElement()->SetAttribute(LOCOBJVALUE, CompText.c_str());
   PropNode->ToElement()->SetAttribute(OBJDATA, IntToHex((int)((TListItem*)CurObj)->Data,8).c_str());
   this->SaveComponentText(CompNode, ((TListItem*)CurObj)->SubItems, NULL);
   goto Finish;
  }

 // TTreeView
 if(CurObj->InheritsFrom(__classid(TTreeNodes)))
  {
   this->SaveComponentText(CompNode, ((TTreeNodes*)CurObj)->GetFirstNode(), NULL);
   goto Finish;
  }
 // TTreeView - Keep tree structure
 if(CurObj->InheritsFrom(__classid(TTreeNode)))
  {
   CompText = ((TTreeNode*)CurObj)->Text;
   PropNode = CompNode->InsertEndChild(TiXmlElement(LOCPROPCOMPN));
   PropNode->ToElement()->SetAttribute(LOCOBJTYPE,  LOCTYPESTR);
   PropNode->ToElement()->SetAttribute(LOCOBJNAME,  OBJTXTTEXT);
   PropNode->ToElement()->SetAttribute(LOCOBJVALUE, CompText.c_str());
   PropNode->ToElement()->SetAttribute(OBJDATA, IntToHex((int)((TTreeNode*)CurObj)->Data,8).c_str());
   this->SaveComponentText(CompNode, ((TTreeNode*)CurObj)->getFirstChild(), NULL);
   this->SaveComponentText(RootNode, ((TTreeNode*)CurObj)->getNextSibling(), NULL);
   goto Finish;
  }

Finish:
 if(CompNode->NoChildren())RootNode->RemoveChild(CompNode);   // Remove an item with no a text fields
   else {if(ChildNode)(*ChildNode) = CompNode;}
 if(CurObj->InheritsFrom(__classid(TComponent))){for(int ctr=0;ctr < CurComp->ComponentCount;ctr++)CompCtr += this->SaveComponentText(CompNode, CurComp->Components[ctr], ChildNode);}
DropChilds:
 if(PropLst)free(PropLst);
 return CompCtr;
}
//---------------------------------------------------------------------------
int DEFCALL CLocalizer::LoadComponentText(TiXmlNode *RootNode, TObject *CurObj)
{
 int        CompCtr;
 TObject    *ChildObj;
 TiXmlNode  *ChildNode;
 TComponent *CurComp;
 TComponent *ChildComp;

 if(!RootNode || !CurObj)return 0;
 if(CurObj->InheritsFrom(__classid(TComponent)) &&
    ((TComponent*)CurObj)->Owner &&
  //  ((TComponent*)CurObj)->Owner->InheritsFrom(__classid(TWinControl)) && // Allow exclude an entire forms
    this->IsInExcludeList(((TComponent*)CurObj)->Name.c_str()))return 0;  // Check if the component are excluded

 AnsiString PropName   = CurObj->ClassName();
 AnsiString PropValue;

 CompCtr     = 0;
 CurComp     = (TComponent*)CurObj;
 ChildNode   = RootNode->FirstChild(LOCPROPCOMPN);

 // TComboBox, TListBox, TCheckListBox, TValueListEditor, TTabControl, ...
 if(CurObj->InheritsFrom(__classid(TStrings)))
  {
   if(ChildNode)
	{
	 AnsiString Data = ChildNode->ToElement()->Attribute(OBJDATA);
	 ((TStrings*)CurObj)->CommaText = ChildNode->ToElement()->Attribute(LOCOBJVALUE);
	 for(int ctr=0;!Data.IsEmpty() && (ctr < ((TStrings*)CurObj)->Count);ctr++)
      {
	   AnsiString Object = Data;
	   if(Object.Length() > 8){Object.SetLength(8);Data = Data.Delete(1,8);}
		 else Data = "";
	   ((TStrings*)CurObj)->Objects[ctr] = (TObject*)StrToInt("0x"+Object);
      }
	}
   return 0;
  }

 // TStringGrid - by rows
 if(CurObj->InheritsFrom(__classid(TStringGrid)))
  {
   ChildNode = RootNode->FirstChild(LOCCLASSCOMP);
   for(int ctr=0;ctr<((TStringGrid*)CurObj)->RowCount && ChildNode;ctr++)
    {
     this->LoadComponentText(ChildNode, ((TStringGrid*)CurObj)->Rows[ctr]);
     ChildNode = ChildNode->NextSibling(LOCCLASSCOMP);
    }
   return 0;
  }

 // TListView, ...
 if(CurObj->InheritsFrom(__classid(TCollection)))
  {
   for(int ctr=0;ctr<((TCollection*)CurObj)->Count;ctr++)this->LoadComponentText(RootNode->FirstChild(LOCCLASSCOMP), ((TCollection*)CurObj)->Items[ctr]);
   return 0;
  }
 // TListView
 if(CurObj->InheritsFrom(__classid(TListItems)))
  {
   int ictr = ((TListItems*)CurObj)->Count;
   ChildNode = RootNode->FirstChild(LOCCLASSCOMP);
   for(int ctr=0;(ctr < ictr) && ChildNode;ctr++)
    {
     this->LoadComponentText(ChildNode, ((TListItems*)CurObj)->Item[ctr]);
     ChildNode = ChildNode->NextSibling(LOCCLASSCOMP);
	}
   return 0;
  }
 // TListView : Do not have RTTI props ?
 if(CurObj->InheritsFrom(__classid(TListItem)))
  {
   ChildNode = RootNode->FirstChild(LOCPROPCOMPN);
   if(ChildNode)
	{
	 AnsiString Data = ChildNode->ToElement()->Attribute(OBJDATA);
	 ((TListItem*)CurObj)->Caption = ChildNode->ToElement()->Attribute(LOCOBJVALUE);
	 if(!Data.IsEmpty())((TListItem*)CurObj)->Data = (PVOID)StrToInt("0x"+Data);
	}
   this->LoadComponentText(RootNode->FirstChild(LOCCLASSCOMP), ((TListItem*)CurObj)->SubItems);
   return 0;
  }

 // TTreeView
 if(CurObj->InheritsFrom(__classid(TTreeNodes)))
  {
   TiXmlNode* XNode = RootNode->FirstChild(LOCCLASSCOMP);
   TTreeNode* TNode = ((TTreeNodes*)CurObj)->GetFirstNode();
   if(XNode  && !TNode)TNode = ((TTreeNodes*)CurObj)->Add(NULL, ""); // Add new node to tree, if there is no matching
   if(!XNode && TNode){TNode->Delete();TNode=NULL;}            // Delete node from tree, if it is missing from XML
   this->LoadComponentText(XNode, TNode);
   return 0;
  }
 // TTreeView
 if(CurObj->InheritsFrom(__classid(TTreeNode)))
  {
   while(CurObj && RootNode)
    {
	 if(ChildNode)
	  {
	   AnsiString Data = ChildNode->ToElement()->Attribute(OBJDATA);
	   ((TTreeNode*)CurObj)->Text = ChildNode->ToElement()->Attribute(LOCOBJVALUE);
	   if(!Data.IsEmpty())((TTreeNode*)CurObj)->Data = (PVOID)StrToInt("0x"+Data.Trim());
	  }
	 TiXmlNode* XNode = RootNode->FirstChild(LOCCLASSCOMP);
	 TTreeNode* TNode = ((TTreeNode*)CurObj)->getFirstChild();
	 if(XNode  && !TNode)TNode = ((TTreeNode*)CurObj)->Owner->AddChild(((TTreeNode*)CurObj), ""); // Add new node to tree, if trehe is no matching
	 if(!XNode && TNode){TNode->Delete();TNode=NULL;}            // Delete node from tree, if it is missing from XML
	 this->LoadComponentText(XNode, TNode);
	 TNode     = ((TTreeNode*)CurObj);
	 CurObj    = ((TTreeNode*)CurObj)->getNextSibling();
	 RootNode  = RootNode->NextSibling(LOCCLASSCOMP);
	 ChildNode = (RootNode)?(RootNode->FirstChild(LOCPROPCOMPN)):(NULL);
	 if(RootNode  && !CurObj)CurObj = TNode->Owner->Add(TNode,"");
	 if(!RootNode && CurObj){((TTreeNode*)CurObj)->Delete();CurObj=NULL;}   // Check if this is correct!
	}
   return 0;
  }

 // Read text props
 while(ChildNode)
  {
   PropName  = ChildNode->ToElement()->Attribute(LOCOBJNAME);
   PropValue = ChildNode->ToElement()->Attribute(LOCOBJVALUE);
   if(!PropName.IsEmpty() && !PropValue.IsEmpty())SetStrProp(CurObj, PropName.c_str(), PropValue.c_str());
   ChildNode = RootNode->NextSibling(LOCPROPCOMPN);
   CompCtr   = 1;   // Here only for components ?
  }
 // Read class props
 ChildNode  = RootNode->FirstChild(LOCCLASSCOMP);
 while(ChildNode)
  {
   PropName = ChildNode->ToElement()->Attribute(LOCOBJNAME);
   if(!PropName.IsEmpty())
    {
     if(CurObj->InheritsFrom(__classid(TComponent))&&(ChildComp = CurComp->FindComponent(PropName)))CompCtr += this->LoadComponentText(ChildNode, ChildComp);  // Search as a child component
      else                                   // Search as a child property
        {
         try {
           ChildObj = GetObjectProp(CurObj, PropName);
           this->LoadComponentText(ChildNode, ChildObj);
             } catch (...) {}  // Catch exception if prop is not found
        }
    }
   ChildNode = ChildNode->NextSibling(LOCCLASSCOMP);
  }
 return CompCtr;
}
//---------------------------------------------------------------------------


