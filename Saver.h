﻿#ifndef SaverH
#define SaverH

#include <System.Classes.hpp>
#include <System.Generics.Collections.hpp>
#include <functional>
#include <stdlib>
#include <list>
#include <System.Math.hpp>

UnicodeString GetFileName(UnicodeString name, UnicodeString style, UnicodeString id, UnicodeString path, UnicodeString exstamp,
	UnicodeString ext = "csv");

class ProtoSaver {
private:
	bool canAI;

protected:
	UnicodeString fileName;
	TStringList *data;

	ProtoSaver(UnicodeString fName) {
		fileName = fName;
		data = new TStringList();
		canAI = true;
	}

	~ProtoSaver() {
		delete data;
	}

	void FinAI() {
		canAI = false;
	}

	void TestAI() {
		if (!canAI)
			throw new Exception("Нельзя добавить новый столбец после сохранения значений!");
	}
	static UnicodeString DefaultFormater(long double v) {
		return FloatToStr(v);
	}

public:

	void Final() {
		data->SaveToFile(fileName);
	}
};

class Saver : public ProtoSaver {
private:
	std::list < std::function < UnicodeString() >> *fl;

public:
	Saver(UnicodeString fName);
	~Saver();
	void AddItem(UnicodeString name, std::function < long double() > f, std::function < UnicodeString(long double) > formater =
		ProtoSaver::DefaultFormater);
	void SaveValues(long double time);
};

class FinalSaver : public ProtoSaver {
private:
	std::list < std::function < UnicodeString(int) >> *fl;

public:
	FinalSaver(UnicodeString fName);
	~FinalSaver();
	void AddItem(UnicodeString name, std::function < long double(int) > f, std::function < UnicodeString(long double) > formater =
		ProtoSaver::DefaultFormater);
	void SaveValues(int index);
};

#endif
