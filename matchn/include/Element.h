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
* Элемент. Физическое тело.
*
* # Элемент всегда проинициализирован каким-то событием.
*   Event::WAIT - по умолчанию.
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
    * Обменивает содержимое текущего элемента с указанным.
    */
    void swap( Element* b );




    /**
    * @virtual CLIncarnate
    */
    virtual typelib::size2Int_t needVisualSize() const;




    // достаточно сравнить UID элементов
    inline bool operator==( const Element& b ) const {
        return (uid() == b.uid());
    }


    inline bool operator!=( const Element& b ) const {
        return (uid() != b.uid());
    }




    /**
    * Визуализирует элемент.
    */
    void draw();




    /**
    * Добавляет элементу новое событие.
    * След. устанавливается бесконечное событие WAIT.
    */
    template< class A >
    inline void addEvent( int currentTime, int duration = -1) {
        assert( ((duration > 0) || (duration == -1))
            && "Просто упустите 'duration' для бесконечного события." );
        addEvent<
            A,
            WaitEvent< Element >
        >( currentTime, duration );
    }




    /**
    * Добавляет элементу новое и следующее события.
    */
    template< class A, class B >
    inline void addEvent( int currentTime, int durationA = -1, int durationB = -1 ) {
        assert( ((durationB > 0) || (durationB == -1))
            && "Просто упустите 'durationB' для бесконечного события." );
        std::unique_ptr< A >  a( new A( this, currentTime, durationA ) );
        currentEvent( std::move( a ) );
        std::unique_ptr< B >  b( new B( this, currentTime, durationB ) );
        nextEvent( std::move( b ) );
    }




    /**
    * Устанавливает визуальные координаты по текущим логическим.
    */
    void updateVisualCoordByLogic();




    /**
    * Устанавливает логические координаты по текущим визуальным.
    */
    void updateLogicCoordByVisual();




private:
    /**
    * Загружает нужные для элемента ресурсы (в частности, графику).
    */
    void load();
};



} // matchn








namespace std {

// #! Передача разыменованной 'e' сразу после std::move( e ) этому методу
//    вызовет сбой.
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
