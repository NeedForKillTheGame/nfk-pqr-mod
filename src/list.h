#ifndef __LIST_H__
#define __LIST_H__

struct ListElement
{
	ListElement* Next;
	ListElement* Prev;
	void* Data;
};

class CList
{
	protected:
		ListElement* CurElement;
		ListElement* TopElement;
		ListElement* BottomElement;
	public:
		CList();
		void* GetCur();
		bool MoveTo(void*);
		void MoveTop();
		void MoveBottom();
		bool MoveNext();
		bool MovePrev();
		void AddAfter(void*);
		void AddBottom(void*);
		void Delete();
		void Remove();
		void DeleteAll();
		void Set(void*);
		bool IsEmpty();
		~CList();
};



#endif //__LIST_H__