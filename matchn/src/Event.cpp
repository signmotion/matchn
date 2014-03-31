#include "../include/stdafx.h"
#include "../include/Event.h"
#include "../include/Element.h"
#include "../include/ManagerEvent.h"
#include "../include/World.h"
#include "../include/LogicWorld.h"
#include "../include/Territory.h"


namespace matchn {


template< class E >
Event< E >::Event(
    E* e,
    int startTime,
    int duration
) :
    mElement( e ),
    mStartTime( startTime ),
    mDuration( duration ),
    mEndTime( mStartTime + mDuration ),
    mCompleted( false )
{
    assert( e
        && "Не задан элемент игры для события." );
    assert( ( (mDuration > 0) || (mDuration == -1) )
        && "# Длительность события должна быть > 0 или == -1." );
}




template< class E >
Event< E >::~Event() {
}







#if 0
// @todo События проверки вынесены в общий поток из-за небезопасной
//       потоковой синхронизации при работе с веткорами. Проработать
//       безопасную схему.

template< class E >
void VerifyHarvestedEvent< E >::make( int currentTime ) {
    std::cout << "VerifyHarvestedEvent " << *mElement << "\n";

    // проверяем, какие элементы находятся рядом
    auto ne = beside();
    std::cout << "Рядом: " << ne.size() << "\n";
    assert( ne.size()
        && "Ожидалось получить хотя бы 1 элемент." );
    const auto K = mElement->allWorld()->logic()->K;
    (ne.size() < K)
        ? notHarvested()
        : yesHarvested( ne );

    // событие завершается
    complete();
}


template
void
VerifyHarvestedEvent< Element >::make( int currentTime );




template< class E >
std::set< E* >
VerifyHarvestedEvent< E >::beside() const {

    std::set< E* >  r;

    // отмечаем этот элемент
    const auto lw = mElement->allWorld()->logic();
    const auto i = lw->ic( mElement->coord() );
    assert( (i < lw->volume())
        && "Получен индекс элемента за пределами игрового мира." );
    r.insert( mElement );

    // отдадим этот элемент методу, который отметит всех соседей
    //signAllNeighbour( &r, mElement->uid(), mElement );

    return r;
}


template
std::set< Element* >
VerifyHarvestedEvent< Element >::beside() const;




template< class E >
void VerifyHarvestedEvent< E >::signAllNeighbour(
    std::set< E* >*  r,
    uidElement_t  uid,
    const E*  e
) {
    assert( r );
    assert( uid != 0 );
    assert( e );

    const auto lw = e->allWorld()->logic();
    const auto ie = e->allWorld()->territory()->incarnateElement();

    // @see typelib::DynamicMapContent2D::isc()
    for (size_t k = 1; k <= 4; ++k) {
        const typelib::coord2Int_t c = e->coord() + lw->isc( k );
        if ( lw->inside( c ) ) {
            const auto i = lw->ic( e->coord() );
            assert( (i < lw->volume())
                && "Получен индекс элемента за пределами игрового мира." );
            Element* other = ie->at( i ).get();
            if ( other && (other->uid() == uid) ) {
                // однотипные элементы
                const bool inserted = r->insert( other ).second;
                if ( inserted ) {
                    // передадим нового соседа на отработку сюда же
                    signAllNeighbour( r, uid, other );
                }
            }
        } // if
    } // for ...
}


template
void
VerifyHarvestedEvent< Element >::signAllNeighbour(
    std::set< Element* >*,
    uidElement_t,
    const Element*
);




template< class E >
void VerifyHarvestedEvent< E >::notHarvested() {
    // говорим вернуться элементу на своё место
    mElement->addEvent<
        ChangeEvent< Element >
    >( mStartTime );
}


template
void
VerifyHarvestedEvent< Element >::notHarvested();




template< class E >
void VerifyHarvestedEvent< E >::yesHarvested( std::set< E* >&  ne ) {
    // назначим отобранным элементам событие "Спрятать собранное"
    // и попросим их уйти с территории
    for (auto itr = ne.begin(); itr != ne.end(); ++itr) {
        Element* e = *itr;
        assert( e
            && "Получен пустой элемент." );
        e->addEvent<
            HideHarvestedEvent< E >,
            LeaveEvent< E >
        >( mStartTime );
    }
}


template
void
VerifyHarvestedEvent< Element >::yesHarvested( std::set< Element* >& );

#endif








template< class E >
WaitEvent< E >::WaitEvent( E* e, int startTime, int duration ) :
    Event< E >( e, startTime, duration )
{}




template
WaitEvent< Element >::WaitEvent( Element* e, int startTime, int duration );




template< class E >
void WaitEvent< E >::make( int currentTime ) {

    // ожидаем...

    // ...и тут же проверяем, не исчез ли под нами элемент
    const auto lw = mElement->allWorld()->logic();
    if (mElement->coord().y == lw->minCoord().y) {
        // самому нижнему падать некуда
        return;
    }

    const auto shift = lw->isc( 3 );
    const auto coordAbove = mElement->coord() + shift;
    // # Выше проверили, что находимся на в самом низу.
    const auto incarnate =
        mElement->allWorld()->territory()->incarnateElement();
    const auto i = lw->ic( coordAbove );
    assert( (i < lw->volume())
        && "Получен индекс, превышающий размеры территории." );
    const auto& b = incarnate->at( i );
    if ( b ) {
        // элемент ниже существует
        // он может быть !live(), но ничего не предпринимаем, пока его
        // не уберёт чистильщик в главном потоке
        return;
    }

    // ниже нет элемента, будем падать
    // не беспокоимся, что элемент может содержать такое же событие:
    // мы здесь потому, что событие этого элемента - WAIT
    mElement->addEvent<
        DropEvent< Element >,
        CompleteDropEvent< Element >
    >( currentTime, 250 );
}


template
void
WaitEvent< Element >::make( int currentTime );




} // matchn
