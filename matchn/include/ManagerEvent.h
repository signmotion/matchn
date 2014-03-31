#pragma once

#include "configure.h"


namespace matchn {


class Territory;
class Element;


/**
* ”правл€ет игровыми событи€ми.
*
* @see Event
*/
class ManagerEvent {
public:
    /**
    * ¬оплощЄнные на территории элементы.
    *
    * # ƒолжен совпадать с Territory::incarnateElement_t.
    */
    typedef std::vector< std::unique_ptr< Element > >
        incarnateElement_t;



    ManagerEvent( std::shared_ptr< Territory > );




public:
    virtual ~ManagerEvent();




    /**
    * ќтрабатывает событи€.
    */
    void pulse( int currentTime );




    /**
    * @return Ёлемент на экране, на который показывает заданна€ координата
    *         (курсор, область внимани€).
    *         nullptr, если элемента в этой точке нет.
    */
    Element* on( const typelib::coord2Int_t& ) const;




    /**
    * @return ѕервый найденный элемент, у которого задействовано заданное
    *         событие.
    *         nullptr, если такого элемента нет.
    */
    inline Element* first( enum UIDEvent uid ) const {
        return other( uid, nullptr );
    }




    /**
    * @return Ёлемент, у которого задействовано заданное событие. ”казанный
    *         в параметрах элемент (свер€ютс€ ссылки) не учитываетс€.
    *         nullptr, если такого элемента нет.
    */
    Element* other( enum UIDEvent,  const Element* exclude ) const;




private:
    std::weak_ptr< Territory >  mTerritory;
};


} // matchn
