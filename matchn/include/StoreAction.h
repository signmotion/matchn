#pragma once

#include "configure.h"
#include "Event.h"


namespace matchn {


class World;


/**
* ��������� ������� ��� ��������.
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
    * ��������� ������� �������.
    *
    * # �������� ����� ���� ��������� ������ ���� �������.
    */
    void currentEvent( std::unique_ptr< event_t > );




    /**
    * @return ������� ������� � ��������.
    */
    event_t* currentEvent() const;




    /**
    * @return UID �������� ������� � ��������.
    *         ���� � �������� ��� ���. �������, ���������� WAIT_EVENT.
    */
    enum UIDEvent uidCurrentEvent() const;




    /**
    * ��������� ��������� �������.
    *
    * # �������� ����� ���� ��������� ������ ���� �������.
    */
    void nextEvent( std::unique_ptr< event_t > );




    /**
    * @return ��������� ������� � ��������.
    */
    event_t* nextEvent() const;




    /**
    * @return UID ���������� ������� � ��������.
    *         ���� � �������� ��� ����. �������, ���������� WAIT_EVENT.
    */
    enum UIDEvent uidNextEvent() const;




    /**
    * ������� ������� ������������.
    * ��������� ������� �������� ����������� �������.
    */
    void startNextEvent();




protected:
    std::weak_ptr< World >  mWorld;


    /**
    * �������������� ������ �������.
    *
    * # ������� ������ ������������������ �����-�� ��������.
    *   Event::WAIT - �� ���������.
    */
    std::unique_ptr< event_t >  mCurrentEvent;


    /**
    * �������, ��������� ����� ���������������.
    *
    * # ������� ������ ������������������ �����-�� ��������.
    *   Event::WAIT - �� ���������.
    */
    std::unique_ptr< event_t >  mNextEvent;
};



} // matchn
