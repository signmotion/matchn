#include "../include/stdafx.h"
#include "../include/Territory.h"
#include "../include/World.h"
#include "../include/Background.h"
#include "../include/Element.h"
#include "../include/LogicWorld.h"


namespace matchn {


Territory::Territory(
    std::shared_ptr< World >  world,
    const std::string&  name
) :
    GE( world ),
    mIncarnateElement( new incarnateElement_t( world->logic()->volume() ) ),
    name( name )
{
    assert( !this->name.empty()
        && "�� ������� �������� ����������." );

    // ������ ������ ���� N x M
    // @todo ...

    load();
}




std::shared_ptr< Territory >  Territory::valueOf(
    std::shared_ptr< World >  world,
    const std::string&  name
) {
    return std::shared_ptr< Territory >( new Territory( world, name ) );
}




Territory::~Territory() {
}




Territory::incarnateElement_t const*
Territory::incarnateElement() const {
    return mIncarnateElement.get();
}




Territory::incarnateElement_t*
Territory::incarnateElement() {
    return mIncarnateElement.get();
}




void Territory::load() {
    loadBackground();
}




void Territory::loadBackground() {
    mBackground = Background::valueOf( allWorld(), name );
    assert( mBackground
        && "��� ��� ���������� �� �������." );
}




void Territory::incarnateElement(
    uidElement_t                 uid,
    const typelib::coord2Int_t&  coord
) {
    auto element = Element::valueOf( allWorld(), uid, coord );
    
#ifdef _DEBUG
    std::cout << "���������� �������� " << *element << "\n";
#endif

    const auto i = allWorld()->logic()->ic( coord );
    mIncarnateElement->at( i ) = std::move( element );

#ifdef _DEBUG
    std::cout << "�� ���������� " << *this << std::endl;
#endif
}




void Territory::draw() {
    drawBackground();
    drawIncarnateElement();
}




void Territory::drawBackground() {
    mBackground->draw();
}




void Territory::drawIncarnateElement() {
    for (auto itr = mIncarnateElement->cbegin(); itr != mIncarnateElement->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        // ������������ ������ ����� ��������
        if ( element && element->live() ) {
            // # ���������� �. � ���������� ���� ����������� ���
            //   ������ incarnateElement()
            element->draw();
        }
    }
}




size_t Territory::countElement() const {
    size_t n = 0;
    for (auto itr = mIncarnateElement->cbegin(); itr != mIncarnateElement->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( element ) {
            n++;
        }
    }

    return n;
}




void Territory::fillAllEmpty() {
    while ( fillEmpty() ) {
        // fillEmpty() ����� ����� ����������
    }
}




bool Territory::fillEmpty() {

    // ����� ������ ������ ������ � �������� � ��������� ���������
    for (auto itr = mIncarnateElement->begin();
         itr != mIncarnateElement->end(); ++itr
    ) {
        Element* const element = ( *itr ).get();
        if ( !element ) {
            const auto uid = nextRandom();
            const auto i = std::distance( mIncarnateElement->begin(), itr );
            const auto cc = allWorld()->logic()->ci( i );
            incarnateElement( uid, cc );
            return true;
        }
    }

    // ��������� ������
    return false;
}




bool Territory::fillTopEmpty() {

    // ����� � ������� ���� ������ ������ ������ � �������� �
    // ��������� ���������
    const auto lw = allWorld()->logic();
    typelib::coord2Int_t  cc( 0, lw->maxCoord().y );
    for (cc.x = lw->minCoord().x; cc.x <= lw->maxCoord().x; ++cc.x) {
        const auto i = lw->ic( cc );
        auto& element = incarnateElement()->at( i );
        if ( !element ) {
            const auto uid = nextRandom();
            incarnateElement( uid, cc );
            return true;
        }
    }

    // ��������� ������
    return false;
}




bool Territory::removeUnlive() {

    // ����� ������ ������ ������� � ������� ������ ���
    for (auto itr = mIncarnateElement->begin();
         itr != mIncarnateElement->end(); ++itr
    ) {
        Element* const element = ( *itr ).get();
        if ( element && !element->live() ) {
#ifdef _DEBUG
            std::cout << "�������� �������� " << *element << std::endl;
#endif
            itr->release();
            return true;
        }
    }

    // ������� ������
    return false;
}




uidElement_t Territory::nextRandom() const {

    static boost::mt19937  gen;
    // # ��������� ������ ��� �������� �������: ��� ���� ���������, �������
    //   ������ ������. �������: -2.
    const size_t ne = allWorld()->logic()->countAvailableElement();
    assert( (ne >= 2)
        && "� ����� 'media/element' ������ ���� ������ ������ ��������." );
    static boost::uniform_int<>   uni( 'a', 'a' + ne - 2 );
    static boost::variate_generator< boost::mt19937&, boost::uniform_int<> >
        random( gen, uni );

    return static_cast< uidElement_t >( random() );
}




} // matchn








namespace std {

inline std::ostream& operator<<(
    std::ostream& out,
    const matchn::Territory& tr
) {
    out << tr.countElement() << " ���������" << std::endl;
    return out;
}

} // std
