/*
	ShevList.h
	������� ������ ����������� ��������������� ������. ������� ��������� �� ������ ���������
	������� ��������� ��������� ��� ������ ����� ��� ��������� ��������������� ������.
	����������� ��� ������ PQR Mod ���������� ����� ������.
*/

#ifndef __SHEVLIST_H__
#define __SHEVLIST_H__

class ShevItem
{
    ShevItem * prevPtr;
    ShevItem * nextPtr;
protected:
    virtual ~ShevItem ();
public:
    int info;
friend class ShevList;
friend void swapCurItems ( ShevList &, ShevList & );
};

class SLSorter
{
public:
    virtual bool isOrder ( ShevItem *, ShevItem * ) const = 0;
    virtual ~SLSorter();
};

class ShevList : public ShevItem
{
    ShevItem * cPtr; // ��������� �� ������� ������� ������
    ShevItem * fPtr; // ��������� �� ������ ������� ������
    ShevItem * lPtr; // ��������� �� ��������� ������� ������
    int listSize;    // ���������� ��������� � ������
// ������ ������������ � ��������� ������������:
    ShevList ( ShevList & );
    void operator = ( ShevList & );
public:
// ����������� � ����������:
    ShevList () { cPtr = fPtr = lPtr = 0; listSize = 0; }
    ~ShevList ();
// ���������� ��������� � ������:
    int size () const { return listSize; }
// ������� ������� ������?
    bool curIsFir () const { return cPtr == fPtr; }
// ������� ������� ���������?
    bool curIsLas () const { return cPtr == lPtr; }
// ������� �� � ������ ������ �������?
    bool has ( const ShevItem * ) const;
// ������ � ���������� �������� ������:
    const ShevItem * fir () const { return fPtr; }
    const ShevItem * cur () const { return cPtr; }
    const ShevItem * las () const { return lPtr; }
    ShevItem * fir () { return fPtr; }
    ShevItem * cur () { return cPtr; }
    ShevItem * las () { return lPtr; }
// ������� ������� ������ �������:
    ShevItem * top () { return cPtr = fPtr; }
// ������� ������� ��������� �������:
    ShevItem * end () { return cPtr = lPtr; }
// ������� ������� ���������� �������:
    ShevItem * prev () { return cPtr != fPtr ? cPtr = cPtr->prevPtr : 0; }
// ������� ������� ����������� �������:
    ShevItem * next () { return cPtr != lPtr ? cPtr = cPtr->nextPtr : 0; }
// ������� ������� ���������� ������� � �����:
    ShevItem * cprev () { return cPtr = (cPtr==fPtr) ? lPtr : cPtr->prevPtr; }
// ������� ������� ����������� ������� � �����:
    ShevItem * cnext () { return cPtr = (cPtr==lPtr) ? fPtr : cPtr->nextPtr; }
// ������� ������� ������ �������:
    void jump ( ShevItem * m ) { cPtr = m; }
// �������� � ������ ���� ������� ����� ������:
    void addBefFir ( ShevItem * );
// �������� � ������ ���� ������� ����� ������� � ������� ��� �������:
    void addBefCur ( ShevItem * );
// �������� � ������ ���� ������� ����� �������� � ������� ��� �������:
    void addAftCur ( ShevItem * );
// �������� � ������ ���� ������� ����� ����������:
    void addAftLas ( ShevItem * );
// ��������� ������� ���������� ��������� � ������:
    void invert ();
    void makeCurFir ();
    void makeCurLas ();
    void swapCurAndFir ();
    void swapCurAndLas ();
    void swapFirAndLas ();
    void sort123 ();
    void sort321 ();
    void sort ( const SLSorter & );
    void sort ( bool (*f) ( ShevItem *, ShevItem * ) );
// ����������� ������ �������� �� ������ � ������ ( ��� �������� ����������� -
// ������� ������� ��������� �� ������� ������, ����� ����������� �� ������.
// ��� ������������� ����� ������ ���������. ):
    void movFirBefFir ( ShevList & list ) { list.addBefFir ( outFir() ); }
    void movFirBefCur ( ShevList & list ) { list.addBefCur ( outFir() ); }
    void movFirAftCur ( ShevList & list ) { list.addAftCur ( outFir() ); }
    void movFirAftLas ( ShevList & list ) { list.addAftLas ( outFir() ); }
    void movCurBefFir ( ShevList & list );
    void movCurBefCur ( ShevList & list );
    void movCurAftCur ( ShevList & list );
    void movCurAftLas ( ShevList & list );
    void movLasBefFir ( ShevList & list ) { list.addBefFir ( outLas() ); }
    void movLasBefCur ( ShevList & list ) { list.addBefCur ( outLas() ); }
    void movLasAftCur ( ShevList & list ) { list.addAftCur ( outLas() ); }
    void movLasAftLas ( ShevList & list ) { list.addAftLas ( outLas() ); }
// ����������� ������ ��������� �� ������ � ������:
    void movFirBefFir ( int, ShevList & );
    void movLasAftLas ( int, ShevList & );
    void movAllBefFir ( ShevList & );
    void movAllBefCur ( ShevList & );
    void movAllAftCur ( ShevList & );
    void movAllAftLas ( ShevList & );
// ��� ���� ��������� ��������� ����� info �������� i:
    void setAllInfo ( int i );
protected:
// ����� ��������� �� ������:
    ShevItem * outFir ();
    ShevItem * outCur ();
    ShevItem * outLas ();
    void outAll () { cPtr = fPtr = lPtr = 0; listSize = 0; }
public:
// �������� ��������� �� ������:
    void delFir () { delete outFir (); }
    void delCur () { delete outCur (); }
    void delLas () { delete outLas (); }
    void delAll ();
// �������� �� ������� ������ ( ���������� ��������� �� ������ � ��������� ������
// ��� 0, ���� ������ �� ���������� ):
    const char * test () const;
// ������ ����� ����������:
    friend void swap ( ShevList &, ShevList & );
// ����� �������� ����������:
    friend void swapCurItems ( ShevList &, ShevList & );
};

//***************************************************************************//

namespace Shev
{

template <class T> class Item : public ShevItem
{
public:
    T d;
    Item () {}
    Item ( const T & t ) : d(t) {}
};

template <class T> class List : public ShevList
{
public:
    const T * fir() const { return (const T *) ShevList::fir(); }
    const T * cur() const { return (const T *) ShevList::cur(); }
    const T * las() const { return (const T *) ShevList::las(); }

    T * fir ()  { return (T *) ShevList::fir (); }
    T * cur ()  { return (T *) ShevList::cur (); }
    T * las ()  { return (T *) ShevList::las (); }
    T * top ()  { return (T *) ShevList::top (); }
    T * end ()  { return (T *) ShevList::end (); }
    T * prev()  { return (T *) ShevList::prev(); }
    T * next()  { return (T *) ShevList::next(); }
    T * cprev() { return (T *) ShevList::cprev(); }
    T * cnext() { return (T *) ShevList::cnext(); }

    void jump      ( T * i ) { ShevList::jump     (i); }
    void addBefFir ( T * i ) { ShevList::addBefFir(i); }
    void addBefCur ( T * i ) { ShevList::addBefCur(i); }
    void addAftCur ( T * i ) { ShevList::addAftCur(i); }
    void addAftLas ( T * i ) { ShevList::addAftLas(i); }

    void movFirBefFir ( List<T> & list ) { ShevList::movFirBefFir ( list ); }
    void movFirBefCur ( List<T> & list ) { ShevList::movFirBefCur ( list ); }
    void movFirAftCur ( List<T> & list ) { ShevList::movFirAftCur ( list ); }
    void movFirAftLas ( List<T> & list ) { ShevList::movFirAftLas ( list ); }
    void movCurBefFir ( List<T> & list ) { ShevList::movCurBefFir ( list ); }
    void movCurBefCur ( List<T> & list ) { ShevList::movCurBefCur ( list ); }
    void movCurAftCur ( List<T> & list ) { ShevList::movCurAftCur ( list ); }
    void movCurAftLas ( List<T> & list ) { ShevList::movCurAftLas ( list ); }
    void movLasBefFir ( List<T> & list ) { ShevList::movLasBefFir ( list ); }
    void movLasBefCur ( List<T> & list ) { ShevList::movLasBefCur ( list ); }
    void movLasAftCur ( List<T> & list ) { ShevList::movLasAftCur ( list ); }
    void movLasAftLas ( List<T> & list ) { ShevList::movLasAftLas ( list ); }

    void movFirBefFir ( int n, List<T> & list ) { ShevList::movFirBefFir ( n, list ); }
    void movLasAftLas ( int n, List<T> & list ) { ShevList::movLasAftLas ( n, list ); }

    void movAllBefFir ( List<T> & list ) { ShevList::movAllBefFir ( list ); }
    void movAllBefCur ( List<T> & list ) { ShevList::movAllBefCur ( list ); }
    void movAllAftCur ( List<T> & list ) { ShevList::movAllAftCur ( list ); }
    void movAllAftLas ( List<T> & list ) { ShevList::movAllAftLas ( list ); }
};

} // namespace Shev;

using namespace Shev;

#endif //__SHEVLIST_H__