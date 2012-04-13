/*
	ShevList.h
	Базовый модуль реализующий двунаправленные списки. Удобный интерфейс на основе шаблонных
	классов позволяет применять эти списки везде где требуются двунаправленные списки.
	Практически все модули PQR Mod изпользуют этоти классы.
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
    ShevItem * cPtr; // указатель на текущий элемент списка
    ShevItem * fPtr; // указатель на первый элемент списка
    ShevItem * lPtr; // указатель на последний элемент списка
    int listSize;    // количество элементов в списке
// Запрет конструктора и оператора присваивания:
    ShevList ( ShevList & );
    void operator = ( ShevList & );
public:
// Конструктор и деструктор:
    ShevList () { cPtr = fPtr = lPtr = 0; listSize = 0; }
    ~ShevList ();
// Количество элементов в списке:
    int size () const { return listSize; }
// Текущий элемент первый?
    bool curIsFir () const { return cPtr == fPtr; }
// Текущий элемент последний?
    bool curIsLas () const { return cPtr == lPtr; }
// Имеется ли в списке данный элемент?
    bool has ( const ShevItem * ) const;
// Доступ к отдельному элементу списка:
    const ShevItem * fir () const { return fPtr; }
    const ShevItem * cur () const { return cPtr; }
    const ShevItem * las () const { return lPtr; }
    ShevItem * fir () { return fPtr; }
    ShevItem * cur () { return cPtr; }
    ShevItem * las () { return lPtr; }
// Сделать текущим первый элемент:
    ShevItem * top () { return cPtr = fPtr; }
// Сделать текущим последний элемент:
    ShevItem * end () { return cPtr = lPtr; }
// Сделать текущим предыдущий элемент:
    ShevItem * prev () { return cPtr != fPtr ? cPtr = cPtr->prevPtr : 0; }
// Сделать текущим последующий элемент:
    ShevItem * next () { return cPtr != lPtr ? cPtr = cPtr->nextPtr : 0; }
// Сделать текущим предыдущий элемент в цикле:
    ShevItem * cprev () { return cPtr = (cPtr==fPtr) ? lPtr : cPtr->prevPtr; }
// Сделать текущим последующий элемент в цикле:
    ShevItem * cnext () { return cPtr = (cPtr==lPtr) ? fPtr : cPtr->nextPtr; }
// Сделать текущим данный элемент:
    void jump ( ShevItem * m ) { cPtr = m; }
// Добавить в список один элемент перед первым:
    void addBefFir ( ShevItem * );
// Добавить в список один элемент перед текущим и сделать его текущим:
    void addBefCur ( ShevItem * );
// Добавить в список один элемент после текущего и сделать его текущим:
    void addAftCur ( ShevItem * );
// Добавить в список один элемент после последнего:
    void addAftLas ( ShevItem * );
// Изменение порядка следования элементов в списке:
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
// Перемещение одного элемента из списка в список ( Эти действия двухактовые -
// вначале элемент выносится из первого списка, затем добавляется во второй.
// Это принципиально когда списки совпадают. ):
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
// Перемещение группы элементов из списка в список:
    void movFirBefFir ( int, ShevList & );
    void movLasAftLas ( int, ShevList & );
    void movAllBefFir ( ShevList & );
    void movAllBefCur ( ShevList & );
    void movAllAftCur ( ShevList & );
    void movAllAftLas ( ShevList & );
// Для всех элементов присвоить члену info значение i:
    void setAllInfo ( int i );
protected:
// Вынос элементов из списка:
    ShevItem * outFir ();
    ShevItem * outCur ();
    ShevItem * outLas ();
    void outAll () { cPtr = fPtr = lPtr = 0; listSize = 0; }
public:
// Удаление элементов из списка:
    void delFir () { delete outFir (); }
    void delCur () { delete outCur (); }
    void delLas () { delete outLas (); }
    void delAll ();
// Проверка на наличие ошибок ( возвращает указатель на строку с описанием ошибки
// или 0, если ошибок не обнаружено ):
    const char * test () const;
// Полный обмен элементами:
    friend void swap ( ShevList &, ShevList & );
// Обмен текущими элементами:
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