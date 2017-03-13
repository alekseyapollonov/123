﻿// ---------------------------------------------------------------------------

#include <vcl.h>
#include <stdlib>
#include <list>
#include <Vcl.ValEdit.hpp>
#include <System.StrUtils.hpp>
#pragma hdrstop

#include "QM.h"
#include "Main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMQCForm *MQCForm;
// ---------------------------------------------------------------------------

void SetCEV(QC::ComputeElement* v);

QC::QC() {
	elements = new std::list<QC::ComputeElement*>();
	name = "Новая очередь";
	nowRun = -1;
}

QC::~QC() {
	delete elements;
}

QC::ComputeElement* QC::AddItem() {
	QC::ComputeElement* v = new QC::ComputeElement();
	v->name = "Расчёт " + IntToStr((__int64)elements->size());
	v->status = QC::Status::Missing;
	elements->push_back(v);
	return v;
}

QC::ComputeElement* QC::GetElementByIndex(int index) {
	int i = 0;
	for (auto v : *elements) {
		if (index == i++)
			return v;
	}
	return NULL;
}

void QC::DeleteItem(int index) {
	int i = 0;
	for (auto v : *elements) {
		if (index == i++)
			elements->remove(v);
	}
}

UnicodeString QC::GetFormName() {
	return "Менеджер очереди расчёта: " + name;
}

void QC::ResetItem(TListItem* li, ComputeElement* v, int cproc) {
	li->Caption = v->name;
	if (li->Checked && (v->status != QC::Status::Ready))
		li->Checked = false;
	if (!li->Checked && (v->status == QC::Status::Ready))
		li->Checked = true;
	li->SubItems->Text = (QC::StatusToStr(v->status) + ((v->status == QC::Status::Computing && cproc > -1) ?
		(": " + IntToStr(cproc) + "%") : ""));
}

void QC::FillList(TListView* lv) {
	lv->Clear();
	for (auto v : *elements) {
		auto item = lv->Items->Add();
		item->Caption = v->name;
		item->SubItems->Add(StatusToStr(v->status));
		item->Checked = true;
	}
}

UnicodeString QC::StatusToStr(QC::Status s) {
	switch (s) {
	case QC::Status::Ready:
		return "В очереди";
	case QC::Status::Complete:
		return "Завершён";
	case QC::Status::Missing:
		return "Пропущен";
	case QC::Status::Error:
		return "Ошибка";
	case QC::Status::Computing:
		return "Выполнение";
	}
}

void QC::SaveToFile(UnicodeString path) {
	const UnicodeString blank = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <xbcq/>";
	auto xml = MQCForm->XML;
	xml->LoadFromXML(blank);
	xml->Active = true;
	auto tnode = xml->Node->ChildNodes->GetNode((UnicodeString)"xbcq");
	for (auto v : *elements) {
		auto cn = tnode->AddChild((UnicodeString)"ComputeElement");
		cn->Attributes[(UnicodeString)"name"] = v->name;
		cn->Attributes[(UnicodeString)"fixation"] = v->fixation;
		cn->Attributes[(UnicodeString)"fixationT"] = v->fixationT;
		cn->Attributes[(UnicodeString)"timePeriod"] = FloatToStr(v->timePeriod);
		cn->Attributes[(UnicodeString)"timeStepCount"] = v->timeStepCount;
		cn->Attributes[(UnicodeString)"overheadRadius"] = FloatToStr(v->overheadRadius);
		cn->Attributes[(UnicodeString)"baseRadius"] = FloatToStr(v->baseRadius);
		cn->Attributes[(UnicodeString)"rFECount"] = v->rFECount;
		cn->Attributes[(UnicodeString)"zFECount"] = v->zFECount;
		cn->Attributes[(UnicodeString)"overheadHight"] = FloatToStr(v->overheadHight);
		cn->Attributes[(UnicodeString)"baseStrat1Hight"] = FloatToStr(v->baseStrat1Hight);
		cn->Attributes[(UnicodeString)"baseStrat2Hight"] = FloatToStr(v->baseStrat2Hight);
		cn->Attributes[(UnicodeString)"baseStrat3Hight"] = FloatToStr(v->baseStrat3Hight);
		cn->Attributes[(UnicodeString)"recess"] = v->recess;
		cn->Attributes[(UnicodeString)"ohPoints"] = (int)v->ohPoints;
		cn->Attributes[(UnicodeString)"verticalStrats"] = (int)v->verticalStrats;
		cn->Attributes[(UnicodeString)"stratCount"] = v->stratCount;
		cn->Attributes[(UnicodeString)"ohMaterial"] = v->ohMaterial;
		cn->Attributes[(UnicodeString)"baseStrat1Material"] = v->baseStrat1Material;
		cn->Attributes[(UnicodeString)"baseStrat2Material"] = v->baseStrat2Material;
		cn->Attributes[(UnicodeString)"baseStrat3Material"] = v->baseStrat3Material;
		cn->Attributes[(UnicodeString)"sinImpact"] = (int)v->sinImpact;
		cn->Attributes[(UnicodeString)"bottomWave"] = (int)v->bottomWave;
		cn->Attributes[(UnicodeString)"bottomWaveForte"] = (int)v->bottomWaveForte;
		cn->Attributes[(UnicodeString)"bottomContWave1"] = (int)v->bottomContWave1;
		cn->Attributes[(UnicodeString)"bottomContWave2"] = (int)v->bottomContWave2;
		cn->Attributes[(UnicodeString)"bottomContWave3"] = (int)v->bottomContWave3;
		cn->Attributes[(UnicodeString)"lateralSide"] = (int)v->lateralSide;
		cn->Attributes[(UnicodeString)"lateralSideForte"] = (int)v->lateralSideForte;
		cn->Attributes[(UnicodeString)"movingOverhead"] = (int)v->movingOverhead;
		cn->Attributes[(UnicodeString)"internalWave"] = (int)v->internalWave;
		cn->Attributes[(UnicodeString)"indentor"] = (int)v->indentor;
		cn->Attributes[(UnicodeString)"bottomImpact"] = (int)v->bottomImpact;
		cn->Attributes[(UnicodeString)"glassful"] = (int)v->glassful;
		cn->Attributes[(UnicodeString)"overheadSpeed"] = FloatToStr(v->overheadSpeed);
		cn->Attributes[(UnicodeString)"lateralWaveSpeed"] = FloatToStr(v->lateralWaveSpeed);
		cn->Attributes[(UnicodeString)"internalWaveSpeed"] = FloatToStr(v->internalWaveSpeed);
		cn->Attributes[(UnicodeString)"indentorSpeed"] = FloatToStr(v->indentorSpeed);
		cn->Attributes[(UnicodeString)"showHeat"] = (int)v->showHeat;
		cn->Attributes[(UnicodeString)"beautyHeat"] = (int)v->beautyHeat;
		cn->Attributes[(UnicodeString)"cinema"] = (int)v->cinema;
		cn->Attributes[(UnicodeString)"cinemaFrameCount"] = v->cinemaFrameCount;
		cn->Attributes[(UnicodeString)"grayScale"] = (int)v->grayScale;
	}
	xml->SaveToFile(path);
	xml->Active = false;
}

void QC::ReadFromFile(UnicodeString path) {
	auto xml = MQCForm->XML;
	xml->LoadFromFile(path);
	name = ChangeFileExt(ExtractFileName(path), "");
	xml->Active = true;
	elements->clear();
	auto tnode = xml->Node->ChildNodes->GetNode((UnicodeString)"xbcq");
	for (int i = 0; i < tnode->ChildNodes->Count; ++i) {
		auto v = new QC::ComputeElement();
		auto cn = tnode->ChildNodes->Nodes[i];
		v->status = QC::Status::Ready;
		v->name = cn->Attributes[(UnicodeString)"name"];
		v->fixation = cn->Attributes[(UnicodeString)"fixation"];
		v->fixationT = cn->Attributes[(UnicodeString)"fixationT"];
		v->timePeriod = StrToFloat(cn->Attributes[(UnicodeString)"timePeriod"]);
		v->timeStepCount = cn->Attributes[(UnicodeString)"timeStepCount"];
		v->overheadRadius = StrToFloat(cn->Attributes[(UnicodeString)"overheadRadius"]);
		v->baseRadius = StrToFloat(cn->Attributes[(UnicodeString)"baseRadius"]);
		v->rFECount = cn->Attributes[(UnicodeString)"rFECount"];
		v->zFECount = cn->Attributes[(UnicodeString)"zFECount"];
		v->overheadHight = StrToFloat(cn->Attributes[(UnicodeString)"overheadHight"]);
		v->baseStrat1Hight = StrToFloat(cn->Attributes[(UnicodeString)"baseStrat1Hight"]);
		v->baseStrat2Hight = StrToFloat(cn->Attributes[(UnicodeString)"baseStrat2Hight"]);
		v->baseStrat3Hight = StrToFloat(cn->Attributes[(UnicodeString)"baseStrat3Hight"]);
		v->recess = cn->Attributes[(UnicodeString)"recess"];
		v->ohPoints = (int)cn->Attributes[(UnicodeString)"ohPoints"];
		v->verticalStrats = (int)cn->Attributes[(UnicodeString)"verticalStrats"];
		v->stratCount = cn->Attributes[(UnicodeString)"stratCount"];
		v->ohMaterial = cn->Attributes[(UnicodeString)"ohMaterial"];
		v->baseStrat1Material = cn->Attributes[(UnicodeString)"baseStrat1Material"];
		v->baseStrat2Material = cn->Attributes[(UnicodeString)"baseStrat2Material"];
		v->baseStrat3Material = cn->Attributes[(UnicodeString)"baseStrat3Material"];
		v->sinImpact = (int)cn->Attributes[(UnicodeString)"sinImpact"];
		v->bottomWave = (int)cn->Attributes[(UnicodeString)"bottomWave"];
		v->bottomWaveForte = (int)cn->Attributes[(UnicodeString)"bottomWaveForte"];
		v->bottomContWave1 = (int)cn->Attributes[(UnicodeString)"bottomContWave1"];
		v->bottomContWave2 = (int)cn->Attributes[(UnicodeString)"bottomContWave2"];
		v->bottomContWave3 = (int)cn->Attributes[(UnicodeString)"bottomContWave3"];
		v->lateralSide = (int)cn->Attributes[(UnicodeString)"lateralSide"];
		v->lateralSideForte = (int)cn->Attributes[(UnicodeString)"lateralSideForte"];
		v->movingOverhead = (int)cn->Attributes[(UnicodeString)"movingOverhead"];
		v->internalWave = (int)cn->Attributes[(UnicodeString)"internalWave"];
		v->indentor = (int)cn->Attributes[(UnicodeString)"indentor"];
		v->bottomImpact = (int)cn->Attributes[(UnicodeString)"bottomImpact"];
		v->glassful = (int)cn->Attributes[(UnicodeString)"glassful"];
		v->overheadSpeed = StrToFloat(cn->Attributes[(UnicodeString)"overheadSpeed"]);
		v->lateralWaveSpeed = StrToFloat(cn->Attributes[(UnicodeString)"lateralWaveSpeed"]);
		v->internalWaveSpeed = StrToFloat(cn->Attributes[(UnicodeString)"internalWaveSpeed"]);
		v->indentorSpeed = StrToFloat(cn->Attributes[(UnicodeString)"indentorSpeed"]);
		v->showHeat = (int)cn->Attributes[(UnicodeString)"showHeat"];
		v->beautyHeat = (int)cn->Attributes[(UnicodeString)"beautyHeat"];
		v->cinema = (int)cn->Attributes[(UnicodeString)"cinema"];
		v->cinemaFrameCount = cn->Attributes[(UnicodeString)"cinemaFrameCount"];
		v->grayScale = (int)cn->Attributes[(UnicodeString)"grayScale"];
		elements->push_back(v);
	}
}

void QC::Run(TListView* list, bool hideGraph) {
	int i = 0;
	UnicodeString sstamp;
	DateTimeToString(sstamp, "yymmddhhnnss", Now());
	for (auto v : *elements) {
		if (v->status == QC::Status::Ready) {
			auto elem = list->Items->Item[i];
			elem->Checked = false;
			nowRun = i;
			v->status = QC::Status::Computing;
			UnicodeString dtstamp;
			DateTimeToString(dtstamp, "yymmddhhnnss", Now());
			QC::ResetItem(elem, v, 0);
			SetCEV(v);
			Application->ProcessMessages();

			bool res;
			mainForm->ResetUS();
			try {
				res = mainForm->Calculate(dtstamp, hideGraph, "#" + name + "\\series" + sstamp + "\\" +
					ReplaceStr(ReplaceStr(v->name, "/", "_"), "\\", "_") + " ", [v, elem](int i) {QC::ResetItem(elem, v, i);});
			}
			catch (...) {
				res = false;
			}

			v->status = (res) ? QC::Status::Complete : QC::Status::Error;
			QC::ResetItem(elem, v);
		}
		++i;
	}
	nowRun = -1;
}

__fastcall TMQCForm::TMQCForm(TComponent * Owner) : TForm(Owner) {
	computeQueue = new QC();
	Caption = computeQueue->GetFormName();
}

// ---------------------------------------------------------------------------
void __fastcall TMQCForm::SaveBtnClick(TObject *Sender) {
	FileSaveDialog->FileName = computeQueue->name;
	if (FileSaveDialog->Execute()) {
		computeQueue->SaveToFile(FileSaveDialog->FileName);
		computeQueue->name = ChangeFileExt(ExtractFileName(FileSaveDialog->FileName), "");
		Caption = computeQueue->GetFormName();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMQCForm::AddBtnClick(TObject *Sender) {
	QC::ResetItem(EList->Items->Add(), computeQueue->AddItem());
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::EListItemChecked(TObject *Sender, TListItem *Item) {
	auto elem = computeQueue->GetElementByIndex(Item->Index);
	if (Item->Checked) {
		elem->status = QC::Status::Ready;
	}
	else {
		elem->status = QC::Status::Missing;
	}
	QC::ResetItem(Item, elem);
}

// ---------------------------------------------------------------------------
void __fastcall TMQCForm::LoadBtnClick(TObject *Sender) {
	if (FileOpenDialog->Execute()) {
		computeQueue->ReadFromFile(FileOpenDialog->FileName);
		computeQueue->FillList(EList);
		Caption = computeQueue->GetFormName();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::EListSelectItem(TObject *Sender, TListItem *Item, bool Selected) {
	DelBtn->Enabled = Selected;
	DubBtn->Enabled = Selected;
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::DubBtnClick(TObject *Sender) {
	auto vd = computeQueue->AddItem(), vs = computeQueue->GetElementByIndex(EList->ItemIndex);
	memcpy(vd, vs, sizeof(QC::ComputeElement));
	vd->name = vs->name; // дабы не вводить менеджер памяти в заблуждение
	QC::ResetItem(EList->Items->Add(), vd);
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::DelBtnClick(TObject *Sender) {
	computeQueue->DeleteItem(EList->ItemIndex);
	EList->DeleteSelected();
}
// ---------------------------------------------------------------------------

void SetCEV(QC::ComputeElement* v) {
	mainForm->jjjjBox->ItemIndex = v->fixation;
	mainForm->tEdit->Text = IntToStr(v->fixationT);
	mainForm->dtimeprEdit->Text = FloatToStr(v->timePeriod);
	mainForm->ntEdit->Text = IntToStr(v->timeStepCount);
	mainForm->rad0Edit->Text = FloatToStr(v->overheadRadius);
	mainForm->rad1Edit->Text = FloatToStr(v->baseRadius);
	mainForm->n3Edit->Text = IntToStr(v->rFECount);
	mainForm->h0Edit->Text = FloatToStr(v->overheadHight);
	mainForm->n1Edit->Text = IntToStr(v->zFECount);
	mainForm->matstrat0Box->ItemIndex = v->ohMaterial;
	mainForm->nstratBox->ItemIndex = v->stratCount - 1;
	mainForm->h2iEdit1->Text = FloatToStr(v->baseStrat1Hight);
	mainForm->h2iEdit2->Text = FloatToStr(v->baseStrat2Hight);
	mainForm->h2iEdit3->Text = FloatToStr(v->baseStrat3Hight);
	mainForm->matstrat1Box->ItemIndex = v->baseStrat1Material;
	mainForm->matstrat2Box->ItemIndex = v->baseStrat2Material;
	mainForm->matstrat3Box->ItemIndex = v->baseStrat3Material;
	mainForm->CheckBox5->Checked = v->showHeat;
	mainForm->BeautyCBox->Checked = v->beautyHeat;
	mainForm->CheckBox3->Checked = (v->verticalStrats) ? true : false;
	mainForm->CheckBox4->Checked = (v->verticalStrats) ? false : true;
	mainForm->RadioGroup1->ItemIndex = (v->sinImpact) ? 1 : 0;
	mainForm->CheckBoxn->Checked = v->bottomWave;
	mainForm->CheckBoxns->Checked = v->bottomWaveForte;
	mainForm->CheckBoxnp1->Checked = v->bottomContWave1;
	mainForm->CheckBoxnp2->Checked = v->bottomContWave2;
	mainForm->CheckBoxnp3->Checked = v->bottomContWave3;
	mainForm->CheckBoxb->Checked = v->lateralSide;
	mainForm->CheckBoxbs->Checked = v->lateralSideForte;
	mainForm->CheckBoxs->Checked = v->movingOverhead;
	mainForm->CheckBoxinss->Checked = v->internalWave;
	mainForm->CheckBoxbsh->Checked = v->indentor;
	mainForm->BBCBox->Checked = v->bottomImpact;
	mainForm->CheckBoxStakan->Checked = v->glassful;
	mainForm->vindentEdit->Text = FloatToStr(v->overheadSpeed);
	mainForm->Editb->Text = FloatToStr(v->lateralWaveSpeed);
	mainForm->Editinss->Text = FloatToStr(v->internalWaveSpeed);
	mainForm->Editbsh->Text = FloatToStr(v->indentorSpeed);
	mainForm->CinemaCBox->Checked = v->cinema;
	mainForm->CinemaEdit->Value = v->cinemaFrameCount;
	mainForm->BWCBox->Checked = v->grayScale;
	mainForm->InputEdit1->Value = v->recess;
	mainForm->AltInpCBox->Checked = v->ohPoints;
}

void __fastcall TMQCForm::AbNowBtnClick(TObject *Sender) {
	mainForm->SetUS();
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::AbAllBtnClick(TObject *Sender) {
	mainForm->SetUS();
	for (int i = 0; i < EList->Items->Count; ++i)
		if (EList->Items->Item[i]->Checked)
			EList->Items->Item[i]->Checked = false;
}

// ---------------------------------------------------------------------------
void __fastcall TMQCForm::StartBtnClick(TObject *Sender) {
	AddBtn->Enabled = false;
	DelBtn->Enabled = false;
	DubBtn->Enabled = false;
	LoadBtn->Enabled = false;
	SaveBtn->Enabled = false;
	AbNowBtn->Enabled = true;
	AbAllBtn->Enabled = true;
	EList->Enabled = false;
	StartBtn->Enabled = false;
	HideGraphCBox->Enabled = false;
	computeQueue->Run(EList, HideGraphCBox->Checked);
	AddBtn->Enabled = true;
	LoadBtn->Enabled = true;
	SaveBtn->Enabled = true;
	AbNowBtn->Enabled = false;
	AbAllBtn->Enabled = false;
	EList->Enabled = true;
	StartBtn->Enabled = true;
	HideGraphCBox->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMQCForm::FormCloseQuery(TObject *Sender, bool &CanClose) {
	CanClose = computeQueue->GetREI() == -1;
}
// ---------------------------------------------------------------------------
