#pragma once

#include "configure.h"
#include "Event.h"


namespace matchn {


class World;


/**
* Хранилище событий для элемента.
*/
template< class E >
class StoreAction :
    public std::enable_shared_from_this< StoreAction< E > >
{
public:
    typedef Event< E >  event_t;



public:
    explicit StoreAction( std::shared_ptr< World > );


    virtual ~StoreAction();



    std::shared_ptr< World >  world() const;



    std::shared_ptr< World >  world();




    /**
    * Назначает текущее событие.
    *
    * # Элементу может быть назначено только одно событие.
    */
    void currentEvent( std::unique_ptr< event_t > );




    /**
    * @return Текущее событие у элемента.
    */
    event_t* currentEvent() const;




    /**
    * @return UID текущего события у элемента.
    *         Если у элемента нет тек. события, возвращает WAIT_EVENT.
    */
    enum UIDEvent uidCurrentEvent() const;




    /**
    * Назначает следующее событие.
    *
    * # Элементу может быть назначено только одно событие.
    */
    void nextEvent( std::unique_ptr< event_t > );




    /**
    * @return Следующее событие у элемента.
    */
    event_t* nextEvent() const;




    /**
    * @return UID следующего события у элемента.
    *         Если у элемента нет след. события, возвращает WAIT_EVENT.
    */
    enum UIDEvent uidNextEvent() const;




    /**
    * Текущее событие сбрасывается.
    * Следующее событие элемента назначается текущим.
    */
    void startNextEvent();




protected:
    std::weak_ptr< World >  mWorld;


    /**
    * Отрабатываемое сейчас событие.
    *
    * # Элемент всегда проинициализирован каким-то событием.
    *   Event::WAIT - по умолчанию.
    */
    std::unique_ptr< event_t >  mCurrentEvent;


    /**
    * Событие, следующее после отрабатываемого.
    *
    * # Элемент всегда проинициализирован каким-то событием.
    *   Event::WAIT - по умолчанию.
    */
    std::unique_ptr< event_t >  mNextEvent;
};



} // matchn
