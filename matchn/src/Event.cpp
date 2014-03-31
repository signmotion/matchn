#include "../include/stdafx.h"
#include "../include/Event.h"
#include "../include/Element.h"
#include "../include/ManagerEvent.h"
#include "../include/World.h"
#include "../include/LogicWorld.h"
#include "../include/Territory.h"


namespace matchn {


template< class E >
Event< E >::Event(
    E* e,
    int startTime,
    int duration
) :
    mElement( e ),
    mStartTime( startTime ),
    mDuration( duration ),
    mEndTime( mStartTime + mDuration ),
    mCompleted( false )
{
    assert( e
        && "�� ����� ������� ���� ��� �������." );
    assert( ( (mDuration > 0) || (mDuration == -1) )
        && "# ������������ ������� ������ ���� > 0 ��� == -1." );
}




template< class E >
Event< E >::~Event() {
}







#if 0
// @todo ������� �������� �������� � ����� ����� ��-�� ������������
//       ��������� ������������� ��� ������ � ���������. �����������
//       ���������� �����.

template< class E >
void VerifyHarvestedEvent< E >::make( int currentTime ) {
    std::cout << "VerifyHarvestedEvent " << *mElement << "\n";

    // ���������, ����� �������� ��������� �����
    auto ne = beside();
    std::cout << "�����: " << ne.size() << "\n";
    assert( ne.size()
        && "��������� �������� ���� �� 1 �������." );
    const auto K = mElement->allWorld()->logic()->K;
    (ne.size() < K)
        ? notHarvested()
        : yesHarvested( ne );

    // ������� �����������
    complete();
}


template
void
VerifyHarvestedEvent< Element >::make( int currentTime );




template< class E >
std::set< E* >
VerifyHarvestedEvent< E >::beside() const {

    std::set< E* >  r;

    // �������� ���� �������
    const auto lw = mElement->allWorld()->logic();
    const auto i = lw->ic( mElement->coord() );
    assert( (i < lw->volume())
        && "������� ������ �������� �� ��������� �������� ����." );
    r.insert( mElement );

    // ������� ���� ������� ������, ������� ������� ���� �������
    //signAllNeighbour( &r, mElement->uid(), mElement );

    return r;
}


template
std::set< Element* >
VerifyHarvestedEvent< Element >::beside() const;




template< class E >
void VerifyHarvestedEvent< E >::signAllNeighbour(
    std::set< E* >*  r,
    uidElement_t  uid,
    const E*  e
) {
    assert( r );
    assert( uid != 0 );
    assert( e );

    const auto lw = e->allWorld()->logic();
    const auto ie = e->allWorld()->territory()->incarnateElement();

    // @see typelib::DynamicMapContent2D::isc()
    for (size_t k = 1; k <= 4; ++k) {
        const typelib::coord2Int_t c = e->coord() + lw->isc( k );
        if ( lw->inside( c ) ) {
            const auto i = lw->ic( e->coord() );
            assert( (i < lw->volume())
                && "������� ������ �������� �� ��������� �������� ����." );
            Element* other = ie->at( i ).get();
            if ( other && (other->uid() == uid) ) {
                // ���������� ��������
                const bool inserted = r->insert( other ).second;
                if ( inserted ) {
                    // ��������� ������ ������ �� ��������� ���� ��
                    signAllNeighbour( r, uid, other );
                }
            }
        } // if
    } // for ...
}


template
void
VerifyHarvestedEvent< Element >::signAllNeighbour(
    std::set< Element* >*,
    uidElement_t,
    const Element*
);




template< class E >
void VerifyHarvestedEvent< E >::notHarvested() {
    // ������� ��������� �������� �� ��� �����
    mElement->addEvent<
        ChangeEvent< Element >
    >( mStartTime );
}


template
void
VerifyHarvestedEvent< Element >::notHarvested();




template< class E >
void VerifyHarvestedEvent< E >::yesHarvested( std::set< E* >&  ne ) {
    // �������� ���������� ��������� ������� "�������� ���������"
    // � �������� �� ���� � ����������
    for (auto itr = ne.begin(); itr != ne.end(); ++itr) {
        Element* e = *itr;
        assert( e
            && "������� ������ �������." );
        e->addEvent<
            HideHarvestedEvent< E >,
            LeaveEvent< E >
        >( mStartTime );
    }
}


template
void
VerifyHarvestedEvent< Element >::yesHarvested( std::set< Element* >& );

#endif








template< class E >
WaitEvent< E >::WaitEvent( E* e, int startTime, int duration ) :
    Event< E >( e, startTime, duration )
{}




template
WaitEvent< Element >::WaitEvent( Element* e, int startTime, int duration );




template< class E >
void WaitEvent< E >::make( int currentTime ) {

    // �������...

    // ...� ��� �� ���������, �� ����� �� ��� ���� �������
    const auto lw = mElement->allWorld()->logic();
    if (mElement->coord().y == lw->minCoord().y) {
        // ������ ������� ������ ������
        return;
    }

    const auto shift = lw->isc( 3 );
    const auto coordAbove = mElement->coord() + shift;
    // # ���� ���������, ��� ��������� �� � ����� ����.
    const auto incarnate =
        mElement->allWorld()->territory()->incarnateElement();
    const auto i = lw->ic( coordAbove );
    assert( (i < lw->volume())
        && "������� ������, ����������� ������� ����������." );
    const auto& b = incarnate->at( i );
    if ( b ) {
        // ������� ���� ����������
        // �� ����� ���� !live(), �� ������ �� �������������, ���� ���
        // �� ����� ���������� � ������� ������
        return;
    }

    // ���� ��� ��������, ����� ������
    // �� �����������, ��� ������� ����� ��������� ����� �� �������:
    // �� ����� ������, ��� ������� ����� �������� - WAIT
    mElement->addEvent<
        DropEvent< Element >,
        CompleteDropEvent< Element >
    >( currentTime, 250 );
}


template
void
WaitEvent< Element >::make( int currentTime );




} // matchn
