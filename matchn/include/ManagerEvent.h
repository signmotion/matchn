#pragma once

#include "configure.h"


namespace matchn {


class Territory;
class Element;


/**
* ��������� �������� ���������.
*
* @see Event
*/
class ManagerEvent {
public:
    /**
    * ����������� �� ���������� ��������.
    *
    * # ������ ��������� � Territory::incarnateElement_t.
    */
    typedef std::vector< std::unique_ptr< Element > >
        incarnateElement_t;



    ManagerEvent( std::shared_ptr< Territory > );




public:
    virtual ~ManagerEvent();




    /**
    * ������������ �������.
    */
    void pulse( int currentTime );




    /**
    * @return ������� �� ������, �� ������� ���������� �������� ����������
    *         (������, ������� ��������).
    *         nullptr, ���� �������� � ���� ����� ���.
    */
    Element* on( const typelib::coord2Int_t& ) const;




    /**
    * @return ������ ��������� �������, � �������� ������������� ��������
    *         �������.
    *         nullptr, ���� ������ �������� ���.
    */
    inline Element* first( enum UIDEvent uid ) const {
        return other( uid, nullptr );
    }




    /**
    * @return �������, � �������� ������������� �������� �������. ���������
    *         � ���������� ������� (��������� ������) �� �����������.
    *         nullptr, ���� ������ �������� ���.
    */
    Element* other( enum UIDEvent,  const Element* exclude ) const;




private:
    std::weak_ptr< Territory >  mTerritory;
};


} // matchn
