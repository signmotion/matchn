#include "../include/stdafx.h"
#include "../include/StoreAction.h"
#include "../include/World.h"


namespace matchn {


template< class E >
StoreAction< E >::StoreAction(
    std::shared_ptr< World >  world
) :
    mWorld( world ),
    mCurrentEvent( nullptr ),
    mNextEvent( nullptr )
{
    assert( !mWorld.expired()
        && "Получен разрушенный мир. Для создания прототипа следует исп. пустой конструктор." );
}


template
StoreAction< Element >::StoreAction(
    std::shared_ptr< World >  world
);




template< class E >
StoreAction< E >::~StoreAction() {
}


template
StoreAction< Element >::~StoreAction();




template< class E >
std::shared_ptr< World >  StoreAction< E >::world() const {
    return mWorld.lock();
}


template
std::shared_ptr< World >  StoreAction< Element >::world() const;




template< class E >
std::shared_ptr< World >  StoreAction< E >::world() {
    return mWorld.lock();
}


template
std::shared_ptr< World >  StoreAction< Element >::world();




template< class E >
void StoreAction< E >::currentEvent(
    std::unique_ptr< typename StoreAction< E >::event_t >  e
) {
    // # Элемент может иметь только одно событие.
    mCurrentEvent.reset();
    mCurrentEvent = std::move( e );
}


template
void StoreAction< Element >::currentEvent(
    std::unique_ptr< typename StoreAction< Element >::event_t >
);




template< class E >
typename StoreAction< E >::event_t*  StoreAction< E >::currentEvent() const {
    return mCurrentEvent ? mCurrentEvent.get() : nullptr;
}


template
StoreAction< Element >::event_t*  StoreAction< Element >::currentEvent() const;




template< class E >
UIDEvent StoreAction< E >::uidCurrentEvent() const {
    return mCurrentEvent ? mCurrentEvent->uid() : WAIT_EVENT;
}


template
UIDEvent StoreAction< Element >::uidCurrentEvent() const;




template< class E >
void StoreAction< E >::nextEvent(
    std::unique_ptr< typename StoreAction< E >::event_t >  e
) {
    assert( e
        && "Попытка передать пустое событие." );
    // # Элемент может иметь только одно событие.
    mNextEvent = std::move( e );
}


template
void StoreAction< Element >::nextEvent(
    std::unique_ptr< typename StoreAction< Element >::event_t >
);




template< class E >
typename StoreAction< E >::event_t*  StoreAction< E >::nextEvent() const {
    return mNextEvent ? mNextEvent.get() : nullptr;
}


template
StoreAction< Element >::event_t*  StoreAction< Element >::nextEvent() const;




template< class E >
UIDEvent StoreAction< E >::uidNextEvent() const {
    return mNextEvent ? mNextEvent->uid() : WAIT_EVENT;
}


template
UIDEvent StoreAction< Element >::uidNextEvent() const;




template< class E >
void StoreAction< E >::startNextEvent() {
    currentEvent( std::move( mNextEvent ) );
    //mNextEvent.release();
}


template
void StoreAction< Element >::startNextEvent();



} // matchn
