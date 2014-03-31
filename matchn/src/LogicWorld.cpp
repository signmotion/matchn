#include "../include/stdafx.h"
#include "../include/LogicWorld.h"
#include "../include/Territory.h"
#include "../include/LIncarnate.h"


namespace matchn {


LogicWorld::LogicWorld(
    size_t K, size_t N, size_t M
) :
    K( K ), N( N ), M( M ),
    countAvailableElement_( 0 )
{
    assert( (K >= 2 )
        && "Количество собираемых вместе элементов должно быть 2 или больше." );
    assert( ( (N >= 3) && (M >= 3) )
        && "Размер поля не может быть меньше чем 3 x 3." );
}




LogicWorld::~LogicWorld() {
}




const Territory*  LogicWorld::territory() const {
    return mTerritory.lock().get();
}




Territory*  LogicWorld::territory() {
    return mTerritory.lock().get();
}




void LogicWorld::territory( std::shared_ptr< Territory >  tr ) {
    assert( tr
        && "Получена не созданная или разрушенная территория." );
    mTerritory = tr;
}




size_t LogicWorld::countAvailableElement() const {
    if (countAvailableElement_ == 0) {
        std::set< std::string >  folder;
        typelib::file::listFolder( &folder, PATH_MEDIA + "/element" );
        countAvailableElement_ = folder.size();
        assert( (countAvailableElement_ > 0)
            && "В папке 'media/element' не обнаружено ни одного элемента." );
    }

    return countAvailableElement_;
}




typelib::coord2Int_t
LogicWorld::visualByLogicCoord( const typelib::coord2Int_t& lc ) const {
    const auto vnlc =
        static_cast< typelib::coord2_t >( lc ) *
        static_cast< typelib::coord2_t >( visualSpriteSize() );
    const auto c = typelib::coord2Int_t(
        vnlc.x + CENTER_WINDOW_WIDTH,
       -vnlc.y + CENTER_WINDOW_HEIGHT
    );
    return c;
}




typelib::coord2Int_t
LogicWorld::logicByVisualCoord( const typelib::coord2Int_t& vc ) const {
    const auto lnvc = typelib::coord2_t(
        vc.x - CENTER_WINDOW_WIDTH,
      -(vc.y - CENTER_WINDOW_HEIGHT)
    );
    const auto c =
        lnvc / static_cast< typelib::coord2_t >( visualSpriteSize() );
    return typelib::coord2Int_t(
        boost::math::round( c.x ),
        boost::math::round( c.y )
    );
}




typelib::size2Int_t
LogicWorld::visualSpriteSize() const {
    return
        static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) *
        visualScale();
}




typelib::size2_t
LogicWorld::visualScale() const {
    const auto vs = 
        typelib::size2_t( WINDOW_WIDTH, WINDOW_HEIGHT ) /
        static_cast< typelib::size2_t >( ELEMENT_SPRITE_SIZE ) /
        typelib::size2_t( N, M ) *
        0.9;
    return vs;
}



} // matchn
