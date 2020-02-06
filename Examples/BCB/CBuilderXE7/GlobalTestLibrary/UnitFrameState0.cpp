//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFrameState0.h"

#include <memory>
#include <DBXJson.hpp>
#include "UnitLibraryTest.h"
#include "JsonVclUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrameState0 *FrameState0 = NULL;

//---------------------------------------------------------------------------
void IterateTreeView(TJSONObject *AObject, TTreeNode *ANode) {
	while (ANode) {

		TJSONObject *AChildObject = new TJSONObject();

		IterateTreeView(AChildObject, ANode->getFirstChild());

		if (AChildObject->Size()) {
			AObject->AddPair(ANode->Text, AChildObject);
			/* in this case object will be freed automatically */
		}
		else {
			/* we can not add empty object, so delete it and add string*/
			AObject->AddPair(ANode->Text, new TJSONString("(NULL)"));
			delete AChildObject;
		}

		ANode = ANode->getNextSibling();
	}

}

//---------------------------------------------------------------------------
void IterateJsonData(TTreeNodes *AItems, TTreeNode *AParentNode, TJSONObject *AJSONObject) {
	if (AJSONObject) {
		for (int i = 0; i < AJSONObject->Size(); i++) {
			TTreeNode *ANode = AItems->AddChild(AParentNode, AJSONObject->Get(i)->JsonString ? AJSONObject->Get(i)->JsonString->Value()
				: UnicodeString("(NULL)"));

			if (TJSONTrue * AJsonTrue = dynamic_cast<TJSONTrue*>(AJSONObject->Get(i)->JsonValue)) {
				ANode->Text += "=true";
			}
			else if (TJSONFalse * AJsonFalse = dynamic_cast<TJSONFalse*>(AJSONObject->Get(i)->JsonValue)) {
				ANode->Text += "=false";
			}
			else if (TJSONNumber * AJsonNumber = dynamic_cast<TJSONNumber*>(AJSONObject->Get(i)->JsonValue)) {
				ANode->Text += "=" + UnicodeString(AJsonNumber->AsDouble);
			}
			else if (TJSONString * AJsonString = dynamic_cast<TJSONString*>(AJSONObject->Get(i)->JsonValue)) {
				ANode->Text += "=" + AJsonString->Value();
			}
			else if (TJSONObject * AJsonObject = dynamic_cast<TJSONObject*>(AJSONObject->Get(i)->JsonValue)) {
				ANode->Text += "=[Object]";

				IterateJsonData(AItems, ANode, AJsonObject);
			}
			else {
				ANode->Text += "=[" + (AJSONObject->Get(i)->JsonValue ? AJSONObject->Get(i)->JsonValue->ClassName() : //
					UnicodeString("UNKNOWN")) + "]";
			}
		}
	}
}

//---------------------------------------------------------------------------
//------------------------------- TFrameState0 ------------------------------
//---------------------------------------------------------------------------
__fastcall TFrameState0::TFrameState0(TComponent* Owner) : TFrame(Owner) {
	TreeView1->FullExpand();
}

//---------------------------------------------------------------------------
void __fastcall TFrameState0::OnDataChange(TObject *Sender) {
	std::auto_ptr<TJSONObject>AJSONObjectPtr(new TJSONObject());

	/* Testing BOOL, NUMBER, STRING */
	CheckBox1->Checked ? AJSONObjectPtr->AddPair(CheckBox1->Name, new TJSONTrue()) : AJSONObjectPtr->AddPair(CheckBox1->Name,
		new TJSONFalse());

	AJSONObjectPtr->AddPair(TrackBar1->Name, new TJSONNumber(TrackBar1->Position));
	AJSONObjectPtr->AddPair(Edit1->Name, new TJSONString(Edit1->Text));

	/* Testing NESTED DATA */
	TJSONObject *AChildObject = new TJSONObject();
	IterateTreeView(AChildObject, TreeView1->Items->GetFirstNode());

	AJSONObjectPtr->AddPair(TreeView1->Name, AChildObject);

	const AnsiString AJsonString = AJSONObjectPtr->ToString();
	usclib_TriggerJsonEvent(FormGlobalLibraryTest->Interpreter, "Inp.Data", AJsonString.c_str());
}

//---------------------------------------------------------------------------
void __fastcall TFrameState0::TreeView1Edited(TObject *Sender, TTreeNode *Node, UnicodeString &S) {
	/* Force change node text */
	Node->Text = S;

	OnDataChange(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TFrameState0::OnDataRespond(const UnicodeString &sData) {

	TreeViewRespond->Items->BeginUpdate();

	try {

		TreeViewRespond->Items->Clear();

		/* C++ Builder bug with spaces inside JSON */
		const UnicodeString sFixedData = Jsonutils::JsonTrimSpaces(sData);

		std::auto_ptr<TJSONObject>AJSONObjectPtr
		(dynamic_cast<TJSONObject*>(TJSONObject::ParseJSONValue(TEncoding::UTF8->GetBytes(sFixedData), 0)));

		IterateJsonData(TreeViewRespond->Items, NULL, AJSONObjectPtr.get());

		TreeViewRespond->FullExpand();
	}
	__finally {
		TreeViewRespond->Items->EndUpdate();
	}

}
//---------------------------------------------------------------------------
