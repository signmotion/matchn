#include "../include/stdafx.h"
#include "../include/Element.h"
#include "../include/World.h"
#include "../include/Territory.h"
#include "../include/LogicWorld.h"


namespace matchn {


Element::Element(
    std::shared_ptr< World >     world,
    uidElement_t                 uid,
    const typelib::coord2Int_t&  logicCoord
) :
    GE( world ),
    LIncarnate( world, uid, logicCoord ),
    // логические коорд. переводятся в визуальные
    CLIncarnate(
        world,
        world->logic()->visualByLogicCoord( logicCoord ),
        0
    ),
    StoreAction< Element >( world )
{
    assert( world->logic()->inside( logicCoord )
        && "Попытка создать элемент за пределами выделенной области." );

    load();
}




std::unique_ptr< Element >  Element::valueOf(
    std::shared_ptr< World >     world,
    uidElement_t                 uid,
    const typelib::coord2Int_t&  logicCoord
) {
    return std::unique_ptr< Element >( new Element( world, uid, logicCoord ) );
}




Element::~Element() {
}




typelib::size2Int_t Element::needVisualSize() const {
    const auto le = static_cast< const LIncarnate* >( this );
    return le->world()->visualSpriteSize();
}




void Element::draw() {
    const enum UIDEvent euid = uidCurrentEvent();
    drawSprite( euid );
}




void Element::updateVisualCoordByLogic() {
    const auto& lc = coord();
    const auto& vc = allWorld()->logic()->visualByLogicCoord( lc );
    visualCoord( vc );
}




void Element::updateLogicCoordByVisual() {
    const auto& vc = visualCoord();
    const auto& lc = allWorld()->logic()->logicByVisualCoord( vc );
    coord( lc );
}




void Element::load() {
    const std::string EM =
        PATH_MEDIA + "/element/" + std::string( 1, mUID );
    addSprite( EM, ".png" );
}


} // matchn
