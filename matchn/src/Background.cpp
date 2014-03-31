#include "../include/stdafx.h"
#include "../include/Background.h"
#include "../include/World.h"
#include "../include/LogicWorld.h"


namespace matchn {


Background::Background(
    std::shared_ptr< World >  world,
    const std::string&        name
) :
    GE( world ),
    // логические коорд. перевод€тс€ в визуальные
    CLIncarnate(
        world,
        world->logic()->visualByLogicCoord( typelib::coord2Int_t::ZERO() ),
        0
    ),
    mName( name )
{
    assert( !mName.empty()
        && "Ќе указано название фона." );

    load();
}




Background::~Background() {
}




std::unique_ptr< Background >  Background::valueOf(
    std::shared_ptr< World >  world,
    const std::string&        name
) {
    return std::unique_ptr< Background >( new Background( world, name ) );
}




void Background::load() {
    // загружаем спрайты фона
    const std::string BM =
        PATH_MEDIA + "/territory/" + mName + "/background";
    addSprite( BM, ".jpg" );
}




void Background::draw() {
    drawImage();
    drawArea();
}




void Background::drawImage() {
    drawSprite( WAIT_EVENT );
}




void Background::drawArea() {
}


} // matchn
