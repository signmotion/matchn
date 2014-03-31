#include "../include/stdafx.h"
#include "../include/LIncarnate.h"
#include "../include/World.h"


namespace matchn {


LIncarnate::LIncarnate(
    std::shared_ptr< World >     world,
    uidElement_t                 uid,
    const typelib::coord2Int_t&  coord
) :
    mWorld( world ),
    mUID( uid ),
    mCoord( coord )
{
}




LIncarnate::~LIncarnate() {
}




const LogicWorld* LIncarnate::world() const {
    return mWorld.lock()->logic();
}


LogicWorld* LIncarnate::world() {
    return mWorld.lock()->logic();
}


} // matchn
