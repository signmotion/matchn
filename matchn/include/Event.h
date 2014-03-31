#pragma once

#include "configure.h"
#include "struct.h"


namespace matchn {


class Element;


/**
* ������������ ��������� ������� � ����.
*
* # ������� ����������� �� ��������.
*
* @see ���������� � �������. � ������� Event* ����.
*
* @todo # ������� �������� �������� � ����� ����� ��-�� ������������
*       ��������� ������������� ��� ������ � ���������. �����������
*       ���������� �����.
*/
enum UIDEvent {
    NONE_EVENT = -1,

    /**
    * ����� ������� �������� ����� �������� �������� (���� ������).
    */
    ATTENTION_EVENT,

    /**
    * ������� ������������ �� ������ ����.
    */
    COMPLETE_DROP_EVENT,

    /**
    * �������� �������� �������.
    */
    CHANGE_EVENT,

    /**
    * ������� ����� ������ ����.
    */
    DROP_EVENT,

    /**
    * ������� ������ ��������� (������� ������ ���-��).
    */
    HIDE_HARVESTED_EVENT,

    /**
    * ������� ��������� � ���������� (� �������� ����).
    */
    LEAVE_EVENT,

    /**
    * ����� ������� ���� �������.
    */
    SELECT_EVENT,

    /**
    * ��������� ���������� ���������� �� ������� ����������.
    */
    UPDATE_LOGIC_COORD_EVENT,

    /**
    * �������� "��� �������".
    */
    VERIFY_HARVESTED_EVENT,

    /**
    * ������� "�������� ������ �������".
    */
    WAIT_EVENT
};




/**
* ������������ �������� UID ��������� ��� �������.
*/
typedef boost::bimap< enum UIDEvent, std::string >  accordanceEvent_t;




static const accordanceEvent_t accordanceEvent =
    boost::assign::list_of< accordanceEvent_t::relation >
        ( NONE_EVENT,               "" )
        ( ATTENTION_EVENT,          "attention" )
        ( COMPLETE_DROP_EVENT,      "complete-drop-event" )
        ( DROP_EVENT,               "drop" )
        ( HIDE_HARVESTED_EVENT,     "hide-harvested" )
        ( CHANGE_EVENT,             "change" )
        ( LEAVE_EVENT,              "leave" )
        ( VERIFY_HARVESTED_EVENT,   "verify-harvested" )
        ( SELECT_EVENT,             "select" )
        ( UPDATE_LOGIC_COORD_EVENT, "update-logic-coord" )
        ( WAIT_EVENT,               "wait" )
    ;




/**
* @return UID ������� �� ��������.
*/
static inline enum UIDEvent convertEvent( const std::string& name ) {
    const auto ftr = accordanceEvent.right.find( name );
    assert( (ftr != accordanceEvent.right.end())
        && "������� � ����� ��������� �� ����������." );
    return ftr->second;
}




/**
* @return �������� ������� �� UID.
*/
static inline std::string const&  convertEvent( UIDEvent uid ) {
    const auto ftr = accordanceEvent.left.find( uid );
    assert( (ftr != accordanceEvent.left.end())
        && "������� � ����� UID �� ����������." );
    return ftr->second;
}








/**
* ������� �������.
*
* # ����� ������� ����� ����� ������ (��), ������������ (��) � �������,
*   ��������� �� ����������.
*/
template< class E >
class Event {
public:
    typedef Event< E >  event_t;



protected:
    /*
    * @param startTime  ����� ������ �������.
    * @param duration   ������������ �������. ���� == -1, ������� ����������.
    * @param next       ��������� �������. ���� �� �������, ����������� WaitEvent.
    *
    * # ���������� ����. ������� ��������� ������� � ������� 'WAIT'.
    * # ������������ ������� ������ ���� > 0 ��� == -1.
    */
    Event( E*, int startTime, int duration );




public:
    virtual ~Event();




    // @todo fine ���������� ���� ��������: UID � ��������� ����������
    //       �������. �������� ���������� �� ���������� �������.
    //       ��� �����: �������� ������ � ���������-��������, �� ��������
    //       �� � enum EVENT.
    virtual enum UIDEvent uid() const = 0;




    /**
    * @alias (uid() == enum Event::NONE)
    */
    inline bool undefined() const {
        return (uid() == NONE_EVENT);
    }




    inline int startTime() const {
        return mStartTime;
    }




    inline int duration() const {
        return mDuration;
    }




    /**
    * @return ������� ��������� �� ������ ������ �������.
    *
    * # ��� ������ ��� ���������, ����������� ������ 'mCompleted'.
    */
    inline bool actuality( int currentTime = 0 ) const {
        return !mCompleted && ((currentTime < mEndTime) || infinitely());
    }




    /**
    * @return ������� ����������.
    */
    inline bool infinitely() const {
        return (mStartTime > mEndTime);
    }




    /**
    * �������� ������� �������� ��� �����������.
    *
    * @see actuality()
    */
    inline void complete() {
        mCompleted = true;
    }




    /**
    * @return ������� ������� � �� �������� �� ���������� ����� �������.
    *         ������������� ����� ��� 0 - ������� �����������.
    */
    inline int deltatime( int currentTime ) const {
        return (mEndTime - currentTime);
    }




    /**
    * @return ������� � ���������  [ 0.0 (��������); 1.0 (���������) )
    *         �������� �� ���������� ����� �������.
    */
    inline float nepperDeltatime( int currentTime ) const {
        assert( (mDuration != 0)
            && "�������� ����������: ������������ ������� �� ����� ���� == 0." );
        const float u =
            1.0f -
            static_cast< float >( mEndTime - currentTime ) /
            static_cast< float >( mDuration );
        return (u > 1.0f) ? 1.0f : (u < 0.0f) ? 0.0f : u;
    }




    /**
    * ������������ �������.
    */
    virtual void make( int currentTime ) = 0;




protected:
    E*         mElement;

    /**
    * ������� ����� ����������� �� ��������� �������.
    */
    const int  mStartTime;
    const int  mDuration;


    /**
    * ������� ����� ��������� ���� ����, ����������� make().
    */
    bool mCompleted;


    /**
    * ����� ������� ������ ����������� �� �������.
    * ������� � ������������ ��� ���������.
    */
    const int  mEndTime;
};








/**
* �������� ���� ������� ��������.
*/
template< class E >
class AttentionEvent :
    public Event< E >
{
public:
    inline AttentionEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return ATTENTION_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        //std::cout << "make AttentionEvent " << *mElement << "\n";
#endif

        // # ������� ���������� �� �������.
    }
};








/**
* ������� ������������ �� ������ ����.
*/
template< class E >
class CompleteDropEvent :
    public Event< E >
{
public:
    inline CompleteDropEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return COMPLETE_DROP_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make CompleteDropEvent " << *mElement << "\n";
#endif

        const auto lw = mElement->allWorld()->logic();
        const auto ia = lw->ic( mElement->coord() );

        const auto shift = lw->isc( 3 );
        const auto coordAbove = mElement->coord() + shift;
        assert ( lw->inside( coordAbove )
            && "������� ������� ������� ���� ��������." );
        mElement->coord( coordAbove );
        mElement->updateVisualCoordByLogic();

        // ������ ��������� � ������� ����������
        // @see todo World::swap()
        const auto ib = lw->ic( coordAbove );
        auto incarnate =
            mElement->allWorld()->territory()->incarnateElement();
        std::swap(
            incarnate->at( ia ),
            incarnate->at( ib )
        );


        // ������� �����������
        complete();
    }
};







#if 0
// - ��. ����. � ���� World.cpp.

/**
* �������� ���� �������� ������� � ������ ���������.
*/
template< class E >
class ChangeEvent :
    public Event< E >
{
public:
    inline ChangeEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {
        // # ������ ����� ������� ���������� ����������-�������.
        mCoord = typelib::coord2Int_t::UNDEFINED();
    }




    virtual inline enum UIDEvent uid() const {
        return CHANGE_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make ChangeEvent " << *mElement << "\n";
#endif

        // ������� �����������
        // # �������� �����, �.�. ��� ������ ��������� ������������
        //   ������� �� ����.
        complete();

        // �������� �������� ������� 1 ���
        if ( !mCoord.undefined() ) {
            return;
        }

        auto a = mElement;
        auto b = a->allWorld()->managerEvent()->other( CHANGE_EVENT, a );
        if ( !b ) {
            return;
        }

        auto sa = a->sprite( CHANGE_EVENT );
        auto sb = b->sprite( CHANGE_EVENT );

        // ���������� ����������
        mCoord = a->visualCoord();
        a->visualCoord( b->visualCoord() );
        b->visualCoord( mCoord );
        sa->update();
        sb->update();

        // ���������� ����������
        a->updateLogicCoordByVisual();
        b->updateLogicCoordByVisual();
    }




private:
    /**
    * ������� ������ ����������� ���� ��� ��� ���� ���������.
    */
    static typelib::coord2Int_t  mCoord;
};


template< class E >
typelib::coord2Int_t ChangeEvent< E >::mCoord =
    typelib::coord2Int_t::UNDEFINED();

#endif







/**
* ������� ����� ������ ����.
*/
template< class E >
class DropEvent :
    public Event< E >
{
public:
    inline DropEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return DROP_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make DropEvent " << *mElement << "\n";
#endif

        const auto nepper = nepperDeltatime( currentTime );
        const auto height =
            static_cast< float >( mElement->needVisualSize().y );
        const auto shift = nepper * height;

        const auto lw = mElement->allWorld()->logic();
        const auto lc = mElement->coord();
        const auto vc = lw->visualByLogicCoord( lc );
        mElement->visualCoord( typelib::coord2Int_t(
            vc.x,  vc.y + shift
        ) );

        // # ������� ���������� �� �������.
    }
};








/**
* ������� ������ ��������� (������� ������ ���-��).
*/
template< class E >
class HideHarvestedEvent :
    public Event< E >
{
public:
    inline HideHarvestedEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return HIDE_HARVESTED_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make HideHarvestedEvent " << *mElement << "\n";
#endif

        // ����� ��������
        const auto alpha = 1.0f - nepperDeltatime( currentTime );
        /*
        auto s = mElement->sprite( HIDE_HARVESTED_EVENT );
        s->set_alpha( alpha );
        s->update();
        */
        mElement->visualAlpha( alpha );

        // # ������� ���������� �� �������.
    }
};








/**
* ������� ��������� � ���������� (� �������� ����).
*/
template< class E >
class LeaveEvent :
    public Event< E >
{
public:
    inline LeaveEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return LEAVE_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make LeaveEvent " << *mElement << "\n";
#endif

        // # �������� ������� ��������� � "�������" ���������� �
        //   ������� �����.
        mElement->unlive();

        // ������� �����������
        complete();
    }
};








/**
* ������� ���� �������.
*/
template< class E >
class SelectEvent :
    public Event< E >
{
public:
    inline SelectEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return SELECT_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make SelectEvent " << *mElement << "\n";
#endif

        // # ����� ���� �� �������� �� � �������-������ � ������ �������� ��
        //   ��� ����������� �������. �� ����� �������, ��� ����� ��������
        //   � ���� � ����� �����.
        /*
        auto s = mElement->sprite( SELECT_EVENT );
        s->rotate( CL_Angle( 1.0f, cl_degrees ) );
        s->update();
        */
        const auto angle = mElement->visualRotation();
        mElement->visualRotation( angle + 2 );

        // # ������� ���������� �� �������.
    }
};







/**
* ��������� ���������� ���������� �� ������� ����������.
*/
template< class E >
class UpdateLogicCoordEvent :
    public Event< E >
{
public:
    inline UpdateLogicCoordEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return UPDATE_LOGIC_COORD_EVENT;
    }




    virtual inline void make( int currentTime ) {
#ifdef _DEBUG_EVENT
        std::cout << "make UpdateLogicCoordEvent " << *mElement << "\n";
#endif

        mElement->updateLogicCoordByVisual();

        // ������� �����������
        complete();
    }
};







#if 0
// @todo ������� �������� �������� � ����� ����� ��-�� ������������
//       ��������� ������������� ��� ������ � ���������. �����������
//       ���������� �����.


/**
* �������� "��� �������".
*/
template< class E >
class VerifyHarvestedEvent :
    public Event< E >
{
public:
    inline VerifyHarvestedEvent( E* e, int startTime, int duration ) :
        Event< E >( e, startTime, duration )
    {}




    virtual inline enum UIDEvent uid() const {
        return VERIFY_HARVESTED_EVENT;
    }




    virtual inline void make( int currentTime );




private:
    /**
    * @return �������� �����.
    */
    std::set< E* >  beside() const;


    // �����. ����� ��� beside()
    static void signAllNeighbour(
        std::set< E* >*  result,
        uidElement_t,
        const E*
    );




    /**
    * �������� �� ������� � ������ ���-��.
    */
    void notHarvested();




    /**
    * ������� ��������� ���-�� ���������.
    */
    void yesHarvested( std::set< E* >& );
};

#endif








/**
* ������� ���� ������� �������.
*/
template< class E >
class WaitEvent :
    public Event< E >
{
public:
    inline WaitEvent( E* e, int startTime = 0, int duration = -1 );




    virtual inline enum UIDEvent uid() const {
        return WAIT_EVENT;
    }




    virtual void make( int currentTime );
};



} // matchn








namespace boost {

template< class E >
inline size_t hash_value( const matchn::Event< E >& e ) {
    return static_cast< size_t >( e.uid() );
}

} // boost
