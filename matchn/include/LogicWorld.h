#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class Territory;


/**
* Логический мир игры.
* Класс введён, чтобы наглядно показать "абстракцию миров": физического,
* визуального, аудио, ...
*/
class LogicWorld {
public:
    /**
    * Кол-во собираемых вместе элементов и размер мира.
    *
    * # Делаем открытыми константами, чтобы не плодить gett'еры.
    */
    const size_t  K;
    const size_t  N;
    const size_t  M;




public:
    LogicWorld(
        size_t K, size_t N, size_t M
    );




    virtual ~LogicWorld();




    const Territory*  territory() const;


    Territory*  territory();


    void territory( std::shared_ptr< Territory > );




    /**
    * @return Количество элементов, с которыми работает мир.
    *
    * # Определяется по содержимому папки 'media/element'.
    */
    size_t countAvailableElement() const;




    /**
    * @return Визуальные координаты по логическим.
    *         Учитывается разбивка на ячейки.
    */
    typelib::coord2Int_t visualByLogicCoord( const typelib::coord2Int_t& ) const;




    /**
    * @return Логические координаты по визуальным.
    *         Учитывается разбивка на ячейки.
    */
    typelib::coord2Int_t logicByVisualCoord( const typelib::coord2Int_t& ) const;




    /**
    * @return Визуальный размер спрайта с учётом размера окна и его разбивки
    *         на ячейки.
    */
    typelib::size2Int_t visualSpriteSize() const;




    /**
    * @return Масштаб для визуализации элементов с учётом размера окна и его
    *         разбивки на ячейки.
    */
    typelib::size2_t visualScale() const;




    // @todo Вынести в отдельный класс typelib.
    // @see typelib::DynamicMapContent2D
    inline typelib::size2Int_t size() const {
        return typelib::size2Int_t( N, M );
    }




    inline std::size_t volume() const {
        return N * M;
    }




    inline typelib::coord2Int_t minCoord() const {
        return -maxCoord();
    }


    inline typelib::coord2Int_t maxCoord() const {
        return static_cast< typelib::coord2Int_t >( (size() - 1) / 2 );
    }




    inline bool inside( const typelib::coord2Int_t& c ) const {
        const auto mi = minCoord();
        const auto ma = maxCoord();
        return (c.x >= mi.x) && (c.x <= ma.x)
            && (c.y >= mi.y) && (c.y <= ma.y);
    }


    inline bool inside( int x, int y ) const {
        return inside( typelib::coord2Int_t( x, y ) );
    }




    inline size_t ic( const typelib::coord2Int_t& c ) const {
        return ic( c.x, c.y );
    }


    inline size_t ic( int x, int y ) const {
        const auto max = maxCoord();
        const size_t i = (
            static_cast< std::size_t >(x + max.x)
          + static_cast< std::size_t >(y + max.y) * N
        );
        return i;
    }




    inline typelib::coord2Int_t ci( size_t i ) const {
        const size_t y = i / N;
        const size_t x = i - y * N;
        const typelib::coord2Int_t  c(
            static_cast< int >( x ) - maxCoord().x,
            static_cast< int >( y ) - maxCoord().y
        );

        assert( inside( c )
            && "Вычисленная 2D координата лежит за пределами заданной карты." );

        return c;
    }




    static inline typelib::coord2Int_t isc( size_t cell ) {
        using namespace typelib;
        const static coord2Int_t a[ 9 ] = {
            coord2Int_t(  0,   0 ),
            coord2Int_t(  0,  +1 ),
            coord2Int_t( +1,   0 ),
            coord2Int_t(  0,  -1 ),
            coord2Int_t( -1,   0 ),
            coord2Int_t( +1,  +1 ),
            coord2Int_t( +1,  -1 ),
            coord2Int_t( -1,  -1 ),
            coord2Int_t( -1,  +1 )
        };

        return a[ cell ];
    }




private:
    /**
    * Доступ к территории.
    */
    std::weak_ptr< Territory >  mTerritory;


    /**
    * Кеш для одноимённых методов.
    */
    mutable size_t countAvailableElement_;
};


} // matchn
