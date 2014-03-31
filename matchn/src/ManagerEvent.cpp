#include "../include/stdafx.h"
#include "../include/ManagerEvent.h"
#include "../include/Territory.h"
#include "../include/Element.h"
#include "../include/Event.h"
#include "../include/World.h"
#include "../include/LogicWorld.h"


namespace matchn {

    
ManagerEvent::ManagerEvent(
    std::shared_ptr< Territory >  territory
) :
    mTerritory( territory )
{
    assert( !mTerritory.expired()
        && "Получена разрушенная или пустая территория." );
}




ManagerEvent::~ManagerEvent() {
}




void ManagerEvent::pulse( int currentTime ) {
    if ( mTerritory.expired() ) {
        // территории больше не существует
        return;
    }

    // просматриваем события воплощённых на территории элементов
    auto tp = mTerritory.lock();
    auto ie = tp->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( !element || !element->live() ) {
            // работаем только с живыми элементами
            continue;
        }

        auto ce = element->currentEvent();
        if ( !ce ) {
            // отсутствие события воспринимается как WAIT_EVENT
            element->addEvent<
                WaitEvent< Element >
            >( currentTime );
            ce = element->currentEvent();
        }

        // проверяем, актуально ли ещё это событие
        if ( ce->actuality( currentTime ) ) {
            // событие актуально - отрабатываем
            ce->make( currentTime );
            // событие может завершить себя внутри make(), отрабатываем
            if ( !ce->actuality() ) {
                element->startNextEvent();
            }

        } else {
            // событие завершено (время прошло) - переводим на следующее
            element->startNextEvent();
        }

    } // for( auto i = ie.first(); ...
}




Element* ManagerEvent::on( const typelib::coord2Int_t&  vc ) const {
    if ( mTerritory.expired() ) {
        // территории больше не существует
        return nullptr;
    }

    // переводим полученные визуальные координаты в логические
    const auto tp = mTerritory.lock();
    const auto lc = tp->allWorld()->logic()->logicByVisualCoord( vc );

    const auto ie = tp->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( !element || !element->live() ) {
            continue;
        }

        // сравниваем логические координаты элемента и указателя
        const auto coord = element->coord();
        if (lc == coord) {
            return element;
        }

    } // for( auto itr = ie->cbegin(); ...

    return nullptr;
}




Element* ManagerEvent::other(
    enum UIDEvent   e,
    const Element*  exclude
) const {
    if ( mTerritory.expired() ) {
        // территории больше не существует
        return nullptr;
    }

    const auto ie = mTerritory.lock()->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( element && element->live()
         && (element->uidCurrentEvent() == e)
            // #! Нас интересует конкретный элемент.
         && (element != exclude)
        ) {
            return element;
        }
    }

    return nullptr;
}


} // matchn
