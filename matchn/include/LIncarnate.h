#pragma once

#include "struct.h"
#include "configure.h"


namespace matchn {


class World;
class LogicWorld;

    
class LIncarnate {
protected:
    LIncarnate(
        std::shared_ptr< World >,
        uidElement_t,
        const typelib::coord2Int_t&  coord
    );




    virtual ~LIncarnate();




public:
    inline uidElement_t uid() const {
        return mUID;
    }




    inline typelib::coord2Int_t const&  coord() const {
        return mCoord;
    };




    template< typename T1, typename T2 >
    inline void coord( T1 x, T2 y ) {
        coord( typelib::coord2Int_t( x, y ) );
    };




    inline void coord( const typelib::coord2Int_t& c ) {
        mCoord = c;
    };




    const LogicWorld* world() const;




    LogicWorld* world();




    inline bool operator==( const LIncarnate& b ) const {
        return (mUID == b.uid());
    }




protected:
    /**
    * Миры, включая логический.
    */
    std::weak_ptr< World >  mWorld;

    uidElement_t          mUID;
    typelib::coord2Int_t  mCoord;
};


} // matchn
