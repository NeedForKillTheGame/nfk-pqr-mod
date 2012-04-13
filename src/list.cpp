#include <stdio.h> //for NULL
#include "list.h"

CList::CList()
{
		CurElement = NULL;
		TopElement = NULL;
		BottomElement = NULL;
}
CList::~CList()
{
	DeleteAll();
}
void* CList::GetCur()
{
	if (CurElement==NULL) return NULL;
	return CurElement->Data;
}
bool CList::MoveTo(void* d)
{
	if (d==NULL) return true;
	if (IsEmpty()) return false;
	ListElement* Cur = CurElement;
	MoveTop();
	do {
		if (GetCur()==d) return true;
	}
	while(MoveNext());
	CurElement = Cur;
	return false;
}
void CList::MoveTop()
{
	CurElement = TopElement;
}
void CList::MoveBottom()
{
	CurElement = BottomElement;
}
bool CList::MoveNext()
{
	if (CurElement==NULL) return false;
	if (CurElement->Next == NULL) return false;
	CurElement = CurElement->Next;
	return true;
}
bool CList::MovePrev()
{
	if (CurElement==NULL) return false;
	if (CurElement->Prev == NULL) return false;
	CurElement = CurElement->Prev;
	return true;
}
void CList::AddAfter(void* data)
{
	if (CurElement == BottomElement) {AddBottom(data);return;}
	ListElement* n = new ListElement;
	n->Data = data;
	n->Next = CurElement->Next;
	n->Prev = CurElement;
	CurElement->Next->Prev = n;
	CurElement->Next = n;
}
void CList::AddBottom(void* data)
{
	ListElement* n = new ListElement;
	if (BottomElement != NULL) BottomElement->Next = n;
	else {
		TopElement=n; //List was empty
		CurElement=n;
	}
	n->Data = data;
	n->Next = NULL;
	n->Prev = BottomElement;
	BottomElement = n;

}
void CList::Delete()
{
	if (CurElement == NULL) return;
	if (CurElement == TopElement) TopElement = CurElement->Next;
	else CurElement->Prev->Next = CurElement->Next;
	if (CurElement == BottomElement) BottomElement = CurElement->Prev;
	else CurElement->Next->Prev = CurElement->Prev;

	ListElement* NewCur;
	NewCur = CurElement->Next;
	if (NewCur==NULL) NewCur = CurElement->Prev;
	delete CurElement;
	CurElement = NewCur;
}
void CList::Remove()
{
	if (CurElement == NULL) return;
	if (CurElement == TopElement) TopElement = CurElement->Next;
	else CurElement->Prev->Next = CurElement->Next;
	if (CurElement == BottomElement) BottomElement = CurElement->Prev;
	else CurElement->Next->Prev = CurElement->Prev;

	ListElement* NewCur;
	NewCur = CurElement->Next;
	if (NewCur==NULL) NewCur = CurElement->Prev;
	CurElement = NewCur;
}
void CList::DeleteAll()
{
	while(!IsEmpty()) Delete();
}
void CList::Set(void* d)
{
	if (CurElement == NULL) return;
	CurElement->Data = d;
}
bool CList::IsEmpty()
{
	if (TopElement==NULL) return true;
	return false;
}
