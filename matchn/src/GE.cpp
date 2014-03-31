#include "../include/stdafx.h"
#include "../include/GE.h"
#include "../include/World.h"


namespace matchn {


GE::GE(
    std::shared_ptr< World >  world
) :
    mWorld( world ),
    mDeleted( false )
{
    assert( !mWorld.expired()
        && "Получен разрушенный мир." );
}




GE::~GE() {
}




std::shared_ptr< World >  GE::allWorld() const {
    return mWorld.lock();
}




std::shared_ptr< World >  GE::allWorld() {
    return mWorld.lock();
}


} // matchn
