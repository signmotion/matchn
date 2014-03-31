#pragma once

#include "GE.h"
#include "LIncarnate.h"
#include "CLIncarnate.h"
#include "StoreAction.h"


namespace matchn {


class LIncarnate;
class CLIncarnate;
class World;
class Territory;


/**
* �������. ���������� ����.
*
* # ������� ������ ������������������ �����-�� ��������.
*   Event::WAIT - �� ���������.
*/
class Element :
    public GE,
    public LIncarnate,
    public CLIncarnate,
    public StoreAction< Element >
{
protected:
    Element(
        std::shared_ptr< World >,
        uidElement_t,
        const typelib::coord2Int_t&  logicCoord
    );



public:
    static std::unique_ptr< Element >  valueOf(
        std::shared_ptr< World >,
        uidElement_t,
        const typelib::coord2Int_t&  logicCoord
    );




    virtual ~Element();




    /**
    * ���������� ���������� �������� �������� � ���������.
    */
    void swap( Element* b );




    /**
    * @virtual CLIncarnate
    */
    virtual typelib::size2Int_t needVisualSize() const;




    // ���������� �������� UID ���������
    inline bool operator==( const Element& b ) const {
        return (uid() == b.uid());
    }


    inline bool operator!=( const Element& b ) const {
        return (uid() != b.uid());
    }




    /**
    * ������������� �������.
    */
    void draw();




    /**
    * ��������� �������� ����� �������.
    * ����. ��������������� ����������� ������� WAIT.
    */
    template< class A >
    inline void addEvent( int currentTime, int duration = -1) {
        assert( ((duration > 0) || (duration == -1))
            && "������ �������� 'duration' ��� ������������ �������." );
        addEvent<
            A,
            WaitEvent< Element >
        >( currentTime, duration );
    }




    /**
    * ��������� �������� ����� � ��������� �������.
    */
    template< class A, class B >
    inline void addEvent( int currentTime, int durationA = -1, int durationB = -1 ) {
        assert( ((durationB > 0) || (durationB == -1))
            && "������ �������� 'durationB' ��� ������������ �������." );
        std::unique_ptr< A >  a( new A( this, currentTime, durationA ) );
        currentEvent( std::move( a ) );
        std::unique_ptr< B >  b( new B( this, currentTime, durationB ) );
        nextEvent( std::move( b ) );
    }




    /**
    * ������������� ���������� ���������� �� ������� ����������.
    */
    void updateVisualCoordByLogic();




    /**
    * ������������� ���������� ���������� �� ������� ����������.
    */
    void updateLogicCoordByVisual();




private:
    /**
    * ��������� ������ ��� �������� ������� (� ���������, �������).
    */
    void load();
};



} // matchn








namespace std {

// #! �������� �������������� 'e' ����� ����� std::move( e ) ����� ������
//    ������� ����.
inline std::ostream& operator<<(
    std::ostream& out,
    const matchn::Element& e
) {
    const auto& li = static_cast< const matchn::LIncarnate& >( e );
    out << e.uid() << "  " << li.coord();
    return out;
}

} // std








namespace boost {

inline size_t hash_value( const matchn::Element& e ) {
    return boost::hash_value( e.uid() );
}

} // boost
