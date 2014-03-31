#include "../include/stdafx.h"
#include "../include/World.h"
#include "../include/LogicWorld.h"
#include "../include/Element.h"
#include "../include/Territory.h"
#include "../include/Event.h"
#include "../include/ManagerEvent.h"


namespace matchn {


World::World(
    size_t K, size_t N, size_t M
) :
    mLogicWorld( new LogicWorld( K, N, M ) ),
    mAttentionPoint( typelib::coord2Int_t::ZERO() ),
    mCurrentTime( 0 )
{
    // *���������� ���*
    {
        // ����� ����������� � go()
    }


    // *����������� ���*
    // @source Example "SpritesRTS" from http://clanlib.org
    // @source Example "TileMap" from http://clanlib.org
    {
        // �������������� ClanLib
	    CL_SetupCore setup_core;
	    CL_SetupDisplay setup_display;
	    #ifdef USE_SOFTWARE_RENDERER
		    CL_SetupSWRender setup_swrender;
	    #endif
	    #ifdef USE_OPENGL_1
		    CL_SetupGL1 setup_gl1;
	    #endif
	    #ifdef USE_OPENGL_2
		    CL_SetupGL setup_gl;
	    #endif
	    //CL_SetupSound setup_sound;

        std::stringstream  ss;
        ss << "Match" << K << "    " << N << " x " << M;
        CL_DisplayWindowDescription  desc( ss.str() );
	    desc.set_size( CL_Size( WINDOW_WIDTH, WINDOW_HEIGHT ),  false );
	    desc.set_allow_resize( false );
	    mVisualWorld =
            std::unique_ptr< CL_DisplayWindow >( new CL_DisplayWindow( desc ) );
    }


    // *��� ������*
    // @todo mAudiuWorld = ...


    // ���������� ���������� ������� �� ������
    {
        mSlotMouseDown =
            mVisualWorld->get_ic().get_mouse().sig_key_down()
            .connect( this, &World::onMouseDown );
        mSlotMouseMove =
            mVisualWorld->get_ic().get_mouse().sig_pointer_move()
            .connect( this, &World::onMouseMove );
	    mSlotKeyboardDown =
            mVisualWorld->get_ic().get_keyboard().sig_key_down()
            .connect( this, &World::onKeyboardDown );
        mSlotWindowClose =
            mVisualWorld->sig_window_close()
            .connect( this, &World::onWindowClose );
    }


    //mVisualWorld->hide_cursor();
}




std::shared_ptr< World >  World::valueOf( size_t K, size_t N, size_t M ) {
    return std::shared_ptr< World >( new World( K, N, M ) );
}




World::~World() {
}




World::command_t World::go() {

    auto it = this->shared_from_this();

    // ��������
    // @todo ...


    // ����������
    mTerritory = Territory::valueOf( it, "fable" );
    // @todo �������� ���������� ����������


    // ���������� ���
    // @todo fine? ����������� ������ �� Territory � LogicWorld.
    mLogicWorld->territory( mTerritory );


    // �������� �������
    mManagerEvent =
        std::unique_ptr< ManagerEvent >( new ManagerEvent( mTerritory ) );


    // ������ ��� �������� ���� ���������
    mTerritory->fillAllEmpty();


    // ����������
    auto gc = mVisualWorld->get_gc();
    auto ic = mVisualWorld->get_ic();
    CL_Font  font( gc, "Tahoma", 16 );
    CL_BlendMode  blendTransparent;
    int lastTime = CL_System::get_time();
    for (auto keyboard = ic.get_keyboard();
         !keyboard.get_keycode( CL_KEY_ESCAPE );
    ) {
        // @source http://clanlib.org/wiki/MainDocs:Timing
		mCurrentTime = CL_System::get_time();
		int deltaTime = mCurrentTime - lastTime;
        // ���: ���� ���������� ����� �������������
		if (deltaTime > 1000) { deltaTime = 1000; }
        // 'deltaTimeMs' ����� ������������ ��� ����������� ��������
		//const float deltaTimeMs = static_cast< float >( deltaTime );
        lastTime = mCurrentTime;


#ifdef _DEBUG
        // ��� FPS
        // @source http://clanlib.org/docs/clanlib-0.8.0/Overview/tipsandtricks.html
        const size_t fps = World::calcFPS( deltaTime );
#endif


        gc.clear( CL_Colorf::aquamarine );

        //gc.set_map_mode( cl_map_2d_upper_left );
        //gc.set_blend_mode( blendTransparent );


        mTerritory->draw();
        pulse( deltaTime );

#ifdef _DEBUG
        {
            std::ostringstream ss;
            ss << fps << "\n" <<
                mLogicWorld->logicByVisualCoord( mAttentionPoint ) << " " <<
                mAttentionPoint;
            //const auto s = CL_StringHelp::int_to_text( fps );
            font.draw_text( gc, 10, 20, ss.str(), CL_Colorf::lightcoral );
        }
#endif


        mVisualWorld->flip( 1 );
        CL_KeepAlive::process();

		mCurrentTime = CL_System::get_time();
        // 10 ms (100 hz)
		static const int mainLoopRate = 10;
		const int timeToSleep = mainLoopRate - (mCurrentTime - lastTime);
		if (timeToSleep > 0) {
            CL_System::sleep( timeToSleep );
        }


        // ���������� ����������
        if ( keyboard.get_keycode( CL_KEY_LEFT ) ) {
            // ��������� ����� �������
            return command_t( DECREASE_CHAIN, nullptr );

        } else if ( keyboard.get_keycode( CL_KEY_RIGHT ) ) {
            // ����������� ����� �������
            return command_t( INCREASE_CHAIN, nullptr );

        } if ( keyboard.get_keycode( CL_KEY_DOWN ) ) {
            // ��������� ������ ����������
            return command_t( DECREASE_TERRITORY, nullptr );

        } if ( keyboard.get_keycode( CL_KEY_UP ) ) {
            // ����������� ������ ����������
            return command_t( INCREASE_TERRITORY, nullptr );
        }

    } // for (auto keyboard = ic.get_keyboard(); ...


    return command_t( QUIT, nullptr );
}




void World::pulse( int deltaTime ) {

    // ������� ����� (�����)
    // @todo optimize bad ��������� ������������:
    //       mCurrentTime + deltaTime = ~ mCurrentTime.
    const auto ct = mCurrentTime + deltaTime;

    // ������������ �������
    mManagerEvent->pulse( ct );

    // ������� ������������ ��������� ��������
    mTerritory->removeUnlive();

#if 1
    // ��������� ������ ������ ���������� ����������
    // @todo fine ���������� ����� �������. ManagerEvent. �������������
    //       ������� ��� *����* ������� ���������.
    mTerritory->fillTopEmpty();
#endif
}




void World::onMouseDown( const CL_InputEvent& ie, const CL_InputState& is ) {
    
    if ( locked() ) {
        return;
    }

    // "���� ��������" ������
    mAttentionPoint = typelib::coord2Int_t( ie.mouse_pos.x, ie.mouse_pos.y );
    Element* const onElement = mManagerEvent->on( mAttentionPoint );
    if ( !onElement ) {
        return;
    }

    // �������� �������
#ifdef _DEBUG
    std::cout << *onElement << std::endl;
#endif
    // ��������� �������� ������� "������"
    // ������� ����� ������� 60 �
    // ����� ������, ��������� ������� ������� ������� "��������"
    onElement->addEvent<
        SelectEvent< Element >,
        AttentionEvent< Element >
    >( mCurrentTime, 60000, 2000 );

    // ���������: ��������, ���� ��������� ����� �������
    Element* const otherElement =
        mManagerEvent->other( SELECT_EVENT, onElement );
    if ( !otherElement ) {
        return;
    }

    // ������� ������� ������� ���� �� ���� �������� ����. �����
    if (onElement == otherElement) {
        otherElement->startNextEvent();
    }

    // ���������, ����� �� ��������� ��� ��������
    const int dist =
        (onElement->coord() - otherElement->coord()).estimateDistance();
    if (dist == 1) {
        // ������� ��������� ���������� �������
#ifdef _DEBUG
        std::cout << "������ " << *onElement <<
            " & " << *otherElement <<
            ", ���������� " << dist << std::endl;
#endif
        changeEvent( onElement, otherElement );
    }

    // ���������� ��������� ���������
    const auto l = territory()->incarnateElement();
    for (auto itr = l->cbegin(); itr != l->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( element && element->live()
         && (element->uidCurrentEvent() == SELECT_EVENT)
        ) {
            element->startNextEvent();
        }
    }
}




void World::onMouseMove( const CL_InputEvent& ie, const CL_InputState& is ) {

    if ( locked() ) {
        return;
    }

    // "���� ��������" ������
    mAttentionPoint = typelib::coord2Int_t( ie.mouse_pos.x, ie.mouse_pos.y );
    Element* const onElement = mManagerEvent->on( mAttentionPoint );
    if ( !onElement ) {
        return;
    }

    // �������� ���������� ��������, ����� ���������
    const auto l = territory()->incarnateElement();
    for( auto itr = l->cbegin(); itr != l->cend(); ++itr) {
        Element* const element = ( *itr ).get();
        if ( !element || !element->live() ) {
            continue;
        }

        if (element->uidCurrentEvent() == HIDE_HARVESTED_EVENT) {
            // ���������� ������� ��������� ��� ���������
            continue;
        }

        if (element->uidCurrentEvent() == SELECT_EVENT) {
            // ���������� ������� ������� ��������� ��� ���������
            continue;
        }

        if (*element != *onElement) {
            // ������ ������
            element->addEvent<
                WaitEvent< Element >
            >( mCurrentTime );
            continue;

        } else {
            // ��������� �������� ������� "��������"
            // ������� ����� ������� 2 �
            element->addEvent<
                AttentionEvent< Element >
            >( mCurrentTime, 2000 );
        }

    } // for( auto itr = ie->cbegin(); ...
}




void World::onKeyboardDown( const CL_InputEvent& ie, const CL_InputState& is ) {
}




void World::onWindowClose() {
}




bool World::locked() const {
    // # �� ��������� ����������������� � ����������, ���� � ���� ����
    //   ���� �� ���� �� ������������� �������.
    const bool presentEvent =
        mManagerEvent->first( CHANGE_EVENT )
     || mManagerEvent->first( DROP_EVENT )
     || mManagerEvent->first( HIDE_HARVESTED_EVENT )
     || mManagerEvent->first( LEAVE_EVENT );

    return presentEvent;
}



void World::changeEvent( Element* a,  Element* b ) {

#if 0
// @todo ������� �������� �������� � ����� ����� ��-�� ������������
//       ��������� ������������� ��� ������ � ���������. �����������
//       ���������� �����.
// ��. ����.

    // # ��������, ��� �������� ������������� �����, ��������� ��
    //   ������ ����� ������.
    // ������� "��������� ���������" ��������� �����
    // ���������� ������� "�������� �������"
    a->addEvent<
        ChangeEvent< Element >,
        VerifyHarvestedEvent< Element >
    >( mCurrentTime );

    b->addEvent<
        ChangeEvent< Element >,
        VerifyHarvestedEvent< Element >
    >( mCurrentTime );

    //@todo ...

#endif


    // ������ �������
    swap( a, b );

    //  ���������, ����� �������� ��������� �����
    const auto la = beside( a );
    const auto lb = beside( b );

#ifdef _DEBUG
        std::cout <<
            "���������� " << *a << "\t" << la.size() << std::endl <<
            "���������� " << *b << "\t" << lb.size() << std::endl;
#endif

    const auto K = logic()->K;
    if ( (la.size() < K) && (lb.size() < K) ) {
#ifdef _DEBUG
        std::cout << "��� ������� ����������. ���������� �������." << std::endl;
#endif
        swap( a, b );
        return;
    }

    // ���-�� ������ ���������...
    std::set< Element* >  h;

    if (la.size() >= K) {
        h.insert( la.cbegin(), la.cend() );
    }

    if (lb.size() >= K) {
        h.insert( lb.cbegin(), lb.cend() );
    }

    // ...����� ��������
    harvest( h );
}




void World::swap( Element* a,  Element* b ) {

    // ��������� � ������� ����������
    // @todo fine ����������� �������� �� Territory � LogicWorld?
    // @todo fine ������� ����� ����������, ������ ����� �������
    //       ���������� ��� ���������.
    {
        const auto lw = a->allWorld()->logic();
        const auto ia = lw->ic( a->coord() );
        const auto ib = lw->ic( b->coord() );
        assert( (ia != ib)
            && "������� �������� ���� � ��� �� �������."
               "��������, �������� ����������� ����." );
        auto incarnate = a->allWorld()->territory()->incarnateElement();
        std::swap(
            incarnate->at( ia ),
            incarnate->at( ib )
        );
    }

    // ���������� ����������
    {
        const auto coordA = a->visualCoord();
        a->visualCoord( b->visualCoord() );
        b->visualCoord( coordA );
    }

    // ���������� ����������
    {
        const auto coordA = a->coord();
        a->coord( b->coord() );
        b->coord( coordA );
    }

    // ��������� �������
    auto sa = a->sprite( CHANGE_EVENT );
    sa->update();
    auto sb = b->sprite( CHANGE_EVENT );
    sb->update();
}




std::set< Element* >  World::beside( Element* e ) const {

    std::set< Element* >  r;

    // �������� ���� �������
    const auto i = logic()->ic( e->coord() );
    assert( (i < logic()->volume())
        && "������� ������ �������� �� ��������� �������� ����." );
    r.insert( e );

    // ������� ���� ������� ������, ������� ������� ���� �������
    signAllNeighbour( &r, e->uid(), e );

    return r;
}




void World::signAllNeighbour(
    std::set< Element* >*  r,
    uidElement_t  uid,
    const Element*  e
) {
    assert( r );
    assert( uid != 0 );
    assert( e );

    const auto lw = e->allWorld()->logic();
    const auto ie = e->allWorld()->territory()->incarnateElement();

    // @see typelib::DynamicMapContent2D::isc()
    // # ��������� ��������� (����).
    for (size_t k = 1; k <= 8; ++k) {
        const typelib::coord2Int_t c = e->coord() + lw->isc( k );
        if ( lw->inside( c ) ) {
            const auto i = lw->ic( c );
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




void World::harvest( std::set< Element* >&  h ) {
    // �������� ���������� ��������� ������� "�������� ���������"
    for (auto itr = h.begin(); itr != h.end(); ++itr) {
        Element* e = *itr;
        assert( e
            && "������� ������ �������." );
        e->addEvent<
            HideHarvestedEvent< Element >,
            LeaveEvent< Element >
        >( mCurrentTime, 1000 );
    }
}




size_t World::calcFPS( size_t frameTime ) {
	static size_t fpsResult  = 0;
	static size_t fpsCounter = 0;
	static size_t totalTime  = 0;
	
	totalTime += frameTime;
	if (totalTime >= 1000) {
		fpsResult = fpsCounter + 1;
		fpsCounter = totalTime = 0;
	}
	++fpsCounter;

	return fpsResult;
}


} // matchn
