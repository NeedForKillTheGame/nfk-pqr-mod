#include "ShevList.h"

ShevItem::~ShevItem() {}

SLSorter::~SLSorter() {}

ShevList::~ShevList()
{
    while ( ( cPtr = fPtr ) != 0 )
    {
        fPtr = fPtr->nextPtr;
        delete cPtr;
    }
}

void ShevList::delAll ()
{
    while ( ( cPtr = fPtr ) != 0 )
    {
        fPtr = fPtr->nextPtr;
        delete cPtr;
    }
    lPtr = 0;
    listSize = 0;
}

void ShevList::addBefFir ( ShevItem * m )
{
    if ( ! m ) return;
    if ( cPtr )
    {
        m->nextPtr = fPtr;
        fPtr = fPtr->prevPtr = m;
    }
    else
    {
        m->nextPtr = 0;
        cPtr = fPtr = lPtr = m;
    }
    m->prevPtr = 0;
    ++listSize;
}

void ShevList::addBefCur ( ShevItem * m )
{
    if ( ! m ) return;
    if ( cPtr )
    {
        m->prevPtr = cPtr->prevPtr;
        m->nextPtr = cPtr;
        cPtr->prevPtr = m;
        if ( m->prevPtr ) m->prevPtr->nextPtr = m;
        else fPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0; fPtr = lPtr = m;
    }
    cPtr = m;
    ++listSize;
}

void ShevList::addAftCur ( ShevItem * m )
{
    if ( ! m ) return;
    if ( cPtr )
    {
        m->prevPtr = cPtr;
        m->nextPtr = cPtr->nextPtr;
        cPtr->nextPtr = m;
        if ( m->nextPtr ) m->nextPtr->prevPtr = m;
        else lPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0; fPtr = lPtr = m;
    }
    cPtr = m;
    ++listSize;
}

void ShevList::addAftLas ( ShevItem * m )
{
    if ( ! m ) return;
    if ( cPtr )
    {
        m->prevPtr = lPtr;
        lPtr = lPtr->nextPtr = m;
    }
    else
    {
        m->prevPtr = 0;
        cPtr = fPtr = lPtr = m;
    }
    m->nextPtr = 0;
    ++listSize;
}

void ShevList::movCurBefFir ( ShevList & list )
{
    if ( !cPtr ) return;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.lPtr )
    {
        m->nextPtr = list.fPtr;
        list.fPtr = list.fPtr->prevPtr = m;
    }
    else
    {
        m->nextPtr = 0;
        list.cPtr = list.fPtr = list.lPtr = m;
    }
    m->prevPtr = 0;
    ++list.listSize;
}

void ShevList::movCurBefCur ( ShevList & list )
{
    if ( !cPtr ) return;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.cPtr )
    {
        m->nextPtr = list.cPtr;
        m->prevPtr = list.cPtr->prevPtr;
        list.cPtr->prevPtr = m;
        if ( m->prevPtr ) m->prevPtr->nextPtr = m;
        else list.fPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0;
        list.fPtr = list.lPtr = m;
    }
    list.cPtr = m;
    ++list.listSize;
}

void ShevList::movCurAftCur ( ShevList & list )
{
    if ( !cPtr ) return;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.cPtr )
    {
        m->prevPtr = list.cPtr;
        m->nextPtr = list.cPtr->nextPtr;
        list.cPtr->nextPtr = m;
        if ( m->nextPtr ) m->nextPtr->prevPtr = m;
        else list.lPtr = m;
    }
    else
    {
        m->prevPtr = m->nextPtr = 0;
        list.fPtr = list.lPtr = m;
    }
    list.cPtr = m;
    ++list.listSize;
}

void ShevList::movCurAftLas ( ShevList & list )
{
    if ( !cPtr ) return;
    ShevItem * m = cPtr;
    if ( m->nextPtr ) ( cPtr = m->nextPtr )->prevPtr = m->prevPtr;
    else lPtr = cPtr = m->prevPtr;
    if ( m->prevPtr ) m->prevPtr->nextPtr = m->nextPtr;
    else fPtr = cPtr;
    --listSize;
    if ( list.lPtr )
    {
        m->prevPtr = list.lPtr;
        list.lPtr = list.lPtr->nextPtr = m;
    }
    else
    {
        m->prevPtr = 0;
        list.cPtr = list.fPtr = list.lPtr = m;
    }
    m->nextPtr = 0;
    ++list.listSize;
}

void ShevList::movAllBefFir ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return;
    if ( list.cPtr )
    {
        lPtr->nextPtr = list.fPtr;
        list.fPtr->prevPtr = lPtr;
        list.fPtr = fPtr;
        list.listSize += listSize;
    }
    else
    {
        list.cPtr = cPtr;
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
}

void ShevList::movAllBefCur ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.cPtr->prevPtr;
        lPtr->nextPtr = list.cPtr;
        list.cPtr->prevPtr = lPtr;
        if ( fPtr->prevPtr )
             fPtr->prevPtr->nextPtr = fPtr;
        else list.fPtr = fPtr;
        list.listSize += listSize;
    }
    else
    {
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    list.cPtr = cPtr;
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
}

void ShevList::movAllAftCur ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.cPtr;
        lPtr->nextPtr = list.cPtr->nextPtr;
        list.cPtr->nextPtr = fPtr;
        if ( lPtr->nextPtr )
             lPtr->nextPtr->prevPtr = lPtr;
        else list.lPtr = lPtr;
        list.listSize += listSize;
    }
    else
    {
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    list.cPtr = cPtr;
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
}

void ShevList::movAllAftLas ( ShevList & list )
{
    if ( ! cPtr || this == &list ) return;
    if ( list.cPtr )
    {
        fPtr->prevPtr = list.lPtr;
        list.lPtr->nextPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize += listSize;
    }
    else
    {
        list.cPtr = cPtr;
        list.fPtr = fPtr;
        list.lPtr = lPtr;
        list.listSize = listSize;
    }
    fPtr = cPtr = lPtr = 0;
    listSize = 0;
}

ShevItem * ShevList::outFir()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = fPtr;
    if ( fPtr == lPtr )
    {
        fPtr = cPtr = lPtr = 0;
        listSize = 0;
    }
    else
    {
        ( fPtr = v -> nextPtr ) -> prevPtr = 0;
        if ( cPtr == v ) cPtr = fPtr;
        -- listSize;
    }
    return v;
}

ShevItem * ShevList::outCur()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = cPtr;
    if ( v -> nextPtr ) ( cPtr = v -> nextPtr ) -> prevPtr = v -> prevPtr;
    else lPtr = cPtr = v -> prevPtr;
    if ( v -> prevPtr ) v -> prevPtr -> nextPtr = v -> nextPtr;
    else fPtr = cPtr;
    -- listSize;
    return v;
}

ShevItem * ShevList::outLas()
{
    if ( ! cPtr ) return cPtr;
    ShevItem * v = lPtr;
    if ( fPtr == lPtr )
    {
        fPtr = cPtr = lPtr = 0;
        listSize = 0;
    }
    else
    {
        ( lPtr = v -> prevPtr ) -> nextPtr = 0;
        if ( cPtr == v ) cPtr = lPtr;
        -- listSize;
    }
    return v;
}

void ShevList::invert ()
{
    ShevItem * temp, * item;

    for ( item = fPtr; item; item = temp )
    {
        temp = item -> nextPtr;
        item -> nextPtr = item -> prevPtr;
        item -> prevPtr = temp;
    }

    temp = lPtr;
    lPtr = fPtr;
    fPtr = temp;
}

void ShevList::sort123 ()
{
    if ( listSize == 2 )
    {
        if ( fPtr->info > lPtr->info )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr -> nextPtr = lPtr;
            lPtr -> prevPtr = fPtr;
            fPtr -> prevPtr = 0;
            lPtr -> nextPtr = 0;
        }
        return;
    }
    if ( listSize < 2 ) return;
    ShevList list;
    movLasAftLas ( listSize / 2, list );
    sort123 ();
    list.sort123 ();
    top ();
    list.top ();
    for (;;)
    {
        if ( cPtr->info <= list.cPtr->info )
        {
            if ( ! next () )
            {
                list.movAllAftLas ( *this ); break;
            }
        }
        else
        {
            list.movCurBefCur ( *this );
            if ( ! list.cPtr ) break;
            cPtr = cPtr->nextPtr;
        }
    }
}

void ShevList::sort321 ()
{
    if ( listSize == 2 )
    {
        if ( fPtr->info < lPtr->info )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr -> nextPtr = lPtr;
            lPtr -> prevPtr = fPtr;
            fPtr -> prevPtr = 0;
            lPtr -> nextPtr = 0;
        }
        return;
    }
    if ( listSize < 2 ) return;
    ShevList list;
    movLasAftLas ( listSize / 2, list );
    sort321 ();
    list.sort321 ();
    top ();
    list.top ();
    for (;;)
    {
        if ( cPtr->info >= list.cPtr->info )
        {
            if ( ! next () )
            {
                list.movAllAftLas ( *this ); break;
            }
        }
        else
        {
            list.movCurBefCur ( *this );
            if ( ! list.cPtr ) break;
            cPtr = cPtr->nextPtr;
        }
    }
}

void ShevList::sort ( bool (*f) ( ShevItem *, ShevItem * ) )
{
    if ( listSize == 2 )
    {
        if ( ! f ( fPtr, lPtr ) )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr -> nextPtr = lPtr;
            lPtr -> prevPtr = fPtr;
            fPtr -> prevPtr = 0;
            lPtr -> nextPtr = 0;
        }
        return;
    }
    if ( listSize < 2 ) return;
    ShevList list;
    movLasAftLas ( listSize / 2, list );
    sort ( f );
    list.sort ( f );
    top ();
    list.top ();
    for (;;)
    {
        if ( f ( cPtr, list.cPtr ) )
        {
            if ( ! next () )
            {
                list.movAllAftLas ( *this ); break;
            }
        }
        else
        {
            list.movCurBefCur ( *this );
            if ( ! list.cPtr ) break;
            cPtr = cPtr->nextPtr;
        }
    }
}

void ShevList::sort ( const SLSorter & s )
{
    if ( listSize == 2 )
    {
        if ( ! s.isOrder ( fPtr, lPtr ) )
        {
            cPtr = fPtr;
            fPtr = lPtr;
            lPtr = cPtr;
            fPtr -> nextPtr = lPtr;
            lPtr -> prevPtr = fPtr;
            fPtr -> prevPtr = 0;
            lPtr -> nextPtr = 0;
        }
        return;
    }
    if ( listSize < 2 ) return;
    ShevList list;
    movLasAftLas ( listSize / 2, list );
    sort ( s );
    list.sort ( s );
    top ();
    list.top ();
    for (;;)
    {
        if ( s.isOrder ( cPtr, list.cPtr ) )
        {
            if ( ! next () )
            {
                list.movAllAftLas ( *this ); break;
            }
        }
        else
        {
            list.movCurBefCur ( *this );
            if ( ! list.cPtr ) break;
            cPtr = cPtr->nextPtr;
        }
    }
}

bool ShevList::has ( const ShevItem * m ) const
{
    const ShevItem * t = fPtr;
    while ( t )
    {
        if ( t == m ) return true;
        t = t->nextPtr;
    }
    return false;
}

void ShevList::setAllInfo ( int i )
{
    ShevItem * t = fPtr;
    while ( t )
    {
        t->info = i;
        t = t->nextPtr;
    }
}

void ShevList::makeCurFir()
{
    if ( fPtr == cPtr ) return;
    fPtr->prevPtr = lPtr;
    lPtr->nextPtr = fPtr;
    lPtr = cPtr->prevPtr;
    fPtr = cPtr;
    fPtr->prevPtr = lPtr->nextPtr = 0;
}

void ShevList::makeCurLas()
{
    if ( lPtr == cPtr ) return;
    fPtr->prevPtr = lPtr;
    lPtr->nextPtr = fPtr;
    fPtr = cPtr->nextPtr;
    lPtr = cPtr;
    fPtr->prevPtr = lPtr->nextPtr = 0;
}

void ShevList::swapCurAndLas()
{
    if ( cPtr == lPtr ) return;
    if ( cPtr == fPtr )
    {
        swapFirAndLas(); return;
    }
    ShevItem * t = cPtr;
    cPtr = lPtr;
    lPtr = t;
    if ( lPtr->nextPtr == cPtr )
    {
        lPtr->prevPtr->nextPtr = cPtr;
        cPtr->prevPtr = lPtr->prevPtr;
        cPtr->nextPtr = lPtr;
        lPtr->prevPtr = cPtr;
        lPtr->nextPtr = 0;
    }
    else
    {
        t = cPtr->prevPtr;
        cPtr->prevPtr = lPtr->prevPtr;
        lPtr->prevPtr = t;
        t = cPtr->nextPtr;
        cPtr->nextPtr = lPtr->nextPtr;
        lPtr->nextPtr = t;
        cPtr->prevPtr->nextPtr = cPtr;
        cPtr->nextPtr->prevPtr = cPtr;
        lPtr->prevPtr->nextPtr = lPtr;
    }
}

void ShevList::swapCurAndFir()
{
    if ( cPtr == fPtr ) return;
    if ( cPtr == lPtr )
    {
        swapFirAndLas(); return;
    }
    ShevItem * t = cPtr;
    cPtr = fPtr;
    fPtr = t;
    if ( fPtr->prevPtr == cPtr )
    {
        fPtr->nextPtr->prevPtr = cPtr;
        cPtr->nextPtr = fPtr->nextPtr;
        cPtr->prevPtr = fPtr;
        fPtr->nextPtr = cPtr;
        fPtr->prevPtr = 0;
    }
    else
    {
        t = cPtr->prevPtr;
        cPtr->prevPtr = fPtr->prevPtr;
        fPtr->prevPtr = t;
        t = cPtr->nextPtr;
        cPtr->nextPtr = fPtr->nextPtr;
        fPtr->nextPtr = t;
        cPtr->nextPtr->prevPtr = cPtr;
        cPtr->prevPtr->nextPtr = cPtr;
        fPtr->nextPtr->prevPtr = fPtr;
    }
}

void ShevList::swapFirAndLas()
{
    if ( lPtr == fPtr ) return;
    ShevItem * t = fPtr;
    fPtr = lPtr;
    lPtr = t;
    if ( listSize == 2 )
    {
        fPtr->nextPtr = lPtr;
        lPtr->prevPtr = fPtr;
        fPtr->prevPtr = lPtr->nextPtr = 0;
    }
    else
    {
        fPtr->nextPtr = lPtr->nextPtr;
        lPtr->prevPtr = fPtr->prevPtr;
        fPtr->prevPtr = lPtr->nextPtr = 0;
        fPtr->nextPtr->prevPtr = fPtr;
        lPtr->prevPtr->nextPtr = lPtr;
    }
    if ( cPtr == fPtr ) cPtr = lPtr; else
    if ( cPtr == lPtr ) cPtr = fPtr;
}

void swap ( ShevList & dl1, ShevList & dl2 )
{
    ShevItem * t = dl1.cPtr;
    dl1.cPtr = dl2.cPtr;
    dl2.cPtr = t;
    t = dl1.fPtr;
    dl1.fPtr = dl2.fPtr;
    dl2.fPtr = t;
    t = dl1.lPtr;
    dl1.lPtr = dl2.lPtr;
    dl2.lPtr = t;
    int k = dl1.listSize;
    dl1.listSize = dl2.listSize;
    dl2.listSize = k;
}

void swapCurItems ( ShevList & dl1, ShevList & dl2 )
{
    if ( &dl1 == &dl2 ) return;
    if ( !dl1.cPtr || !dl2.cPtr ) return;
    if ( dl1.cPtr->prevPtr ) dl1.cPtr->prevPtr->nextPtr = dl2.cPtr;
    if ( dl1.cPtr->nextPtr ) dl1.cPtr->nextPtr->prevPtr = dl2.cPtr;
    if ( dl2.cPtr->prevPtr ) dl2.cPtr->prevPtr->nextPtr = dl1.cPtr;
    if ( dl2.cPtr->nextPtr ) dl2.cPtr->nextPtr->prevPtr = dl1.cPtr;
    ShevItem * t = dl1.cPtr->prevPtr;
    dl1.cPtr->prevPtr = dl2.cPtr->prevPtr;
    dl2.cPtr->prevPtr = t;
    t = dl1.cPtr->nextPtr;
    dl1.cPtr->nextPtr = dl2.cPtr->nextPtr;
    dl2.cPtr->nextPtr = t;
    if ( dl1.fPtr == dl1.cPtr ) dl1.fPtr = dl2.cPtr;
    if ( dl1.lPtr == dl1.cPtr ) dl1.lPtr = dl2.cPtr;
    if ( dl2.fPtr == dl2.cPtr ) dl2.fPtr = dl1.cPtr;
    if ( dl2.lPtr == dl2.cPtr ) dl2.lPtr = dl1.cPtr;
    t = dl1.cPtr;
    dl1.cPtr = dl2.cPtr;
    dl2.cPtr = t;
}

void ShevList::movFirBefFir ( int n, ShevList & list )
{
    if ( n <= 0 || listSize <= 0 || this == &list ) return;
    if ( n >= listSize )
    {
        movAllBefFir ( list );
        return;
    }
    cPtr = fPtr;
    for ( int i = n; --i >= 0; ) cPtr = cPtr->nextPtr;
    if ( list.listSize > 0 )
    {
        cPtr->prevPtr->nextPtr = list.fPtr;
        list.fPtr->prevPtr = cPtr->prevPtr;
        list.fPtr = fPtr;
        list.listSize += n;
    }
    else
    {
        list.lPtr = cPtr->prevPtr;
        list.lPtr->nextPtr = 0;
        list.cPtr = list.fPtr = fPtr;
        list.listSize = n;
    }
    fPtr = cPtr;
    fPtr->prevPtr = 0;
    listSize -= n;
}

void ShevList::movLasAftLas ( int n, ShevList & list )
{
    if ( n <= 0 || listSize <= 0 || this == &list ) return;
    if ( n >= listSize )
    {
        movAllAftLas ( list );
        return;
    }
    cPtr = lPtr;
    for ( int i = n; --i >= 0; ) cPtr = cPtr->prevPtr;
    if ( list.listSize > 0 )
    {
        cPtr->nextPtr->prevPtr = list.lPtr;
        list.lPtr->nextPtr = cPtr->nextPtr;
        list.lPtr = lPtr;
        list.listSize += n;
    }
    else
    {
        list.fPtr = cPtr->nextPtr;
        list.fPtr->prevPtr = 0;
        list.cPtr = list.lPtr = lPtr;
        list.listSize = n;
    }
    lPtr = cPtr;
    lPtr->nextPtr = 0;
    listSize -= n;
}

const char * ShevList::test() const
{
    if ( listSize < 0 ) return "listSize < 0";
    if ( listSize == 0 )
    {
        if ( fPtr ) return "listSize == 0 && fPtr != 0";
        if ( cPtr ) return "listSize == 0 && cPtr != 0";
        if ( lPtr ) return "listSize == 0 && lPtr != 0";
        return 0;
    }
    if ( ! fPtr ) return "listSize > 0 && fPtr == 0";
    if ( ! cPtr ) return "listSize > 0 && cPtr == 0";
    if ( ! lPtr ) return "listSize > 0 && lPtr == 0";
    if ( fPtr->prevPtr ) return "fPtr->prevPtr != 0";
    if ( lPtr->nextPtr ) return "lPtr->nextPtr != 0";
    if ( listSize == 1 )
    {
        if ( fPtr != cPtr ) return "listSize == 1 && fPtr != cPtr";
        if ( cPtr != lPtr ) return "listSize == 1 && cPtr != lPtr";
        if ( lPtr != fPtr ) return "listSize == 1 && lPtr != fPtr";
        return 0;
    }
    if ( ! fPtr->nextPtr ) return "listSize > 1 && fPtr->nextPtr == 0";
    if ( ! lPtr->prevPtr ) return "listSize > 1 && lPtr->prevPtr == 0";
    if ( fPtr == lPtr    ) return "listSize > 1 && fPtr == lPtr";
    if ( listSize == 2 )
    {
        if ( fPtr != cPtr && cPtr != lPtr )
            return "listSize == 2 && fPtr != cPtr && cPtr != lPtr";
        if ( fPtr->nextPtr != lPtr )
            return "listSize == 2 && fPtr->nextPtr != lPtr";
        if ( lPtr->prevPtr != fPtr )
            return "listSize == 2 && lPtr->prevPtr != fPtr";
        return 0;
    }
    ShevItem * p = fPtr;
    for ( int i = 1; i < listSize; ++i )
    {
        if ( p == lPtr ) return "p == lPtr && i < listSize";
        if ( p->nextPtr == 0 ) return "p->nextPtr == 0 && p != lPtr";
        if ( p->nextPtr == fPtr ) return "p->nextPtr == fPtr";
        if ( p->nextPtr->prevPtr != p ) return "p->nextPtr->prevPtr != p";
        p = p->nextPtr;
    }
    if ( p != lPtr ) return "p != lPtr && i == listSize";
    return 0;
}
