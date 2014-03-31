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
        && "�������� ����������� ��� ������ ����������." );
}




ManagerEvent::~ManagerEvent() {
}




void ManagerEvent::pulse( int currentTime ) {
    if ( mTerritory.expired() ) {
        // ���������� ������ �� ����������
        return;
    }

    // ������������� ������� ����������� �� ���������� ���������
    auto tp = mTerritory.lock();
    auto ie = tp->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( !element || !element->live() ) {
            // �������� ������ � ������ ����������
            continue;
        }

        auto ce = element->currentEvent();
        if ( !ce ) {
            // ���������� ������� �������������� ��� WAIT_EVENT
            element->addEvent<
                WaitEvent< Element >
            >( currentTime );
            ce = element->currentEvent();
        }

        // ���������, ��������� �� ��� ��� �������
        if ( ce->actuality( currentTime ) ) {
            // ������� ��������� - ������������
            ce->make( currentTime );
            // ������� ����� ��������� ���� ������ make(), ������������
            if ( !ce->actuality() ) {
                element->startNextEvent();
            }

        } else {
            // ������� ��������� (����� ������) - ��������� �� ���������
            element->startNextEvent();
        }

    } // for( auto i = ie.first(); ...
}




Element* ManagerEvent::on( const typelib::coord2Int_t&  vc ) const {
    if ( mTerritory.expired() ) {
        // ���������� ������ �� ����������
        return nullptr;
    }

    // ��������� ���������� ���������� ���������� � ����������
    const auto tp = mTerritory.lock();
    const auto lc = tp->allWorld()->logic()->logicByVisualCoord( vc );

    const auto ie = tp->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( !element || !element->live() ) {
            continue;
        }

        // ���������� ���������� ���������� �������� � ���������
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
        // ���������� ������ �� ����������
        return nullptr;
    }

    const auto ie = mTerritory.lock()->incarnateElement();
    for( auto itr = ie->cbegin(); itr != ie->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( element && element->live()
         && (element->uidCurrentEvent() == e)
            // #! ��� ���������� ���������� �������.
         && (element != exclude)
        ) {
            return element;
        }
    }

    return nullptr;
}


} // matchn
