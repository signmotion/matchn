#include "include/stdafx.h"
#include "include/configure.h"
#include "include/World.h"
#include "include/LogicWorld.h"




void run();




/**
* Первая версия демо-игры MatchN.
*
* -> См. MatchN2.
*/
int main( int argc, char** argv ) {

    using namespace matchn;

            
    setlocale( LC_ALL, "Russian" );
    setlocale( LC_NUMERIC, "C" );


    // создаём мир
    try {
        run();

	} catch ( const CL_Exception& ex ) {
		CL_ConsoleWindow  console( "Console", 100, 200 );
		CL_Console::write_line(
            "(!) Exception caught: " + ex.get_message_and_stack_trace()
        );
		console.display_close_message();
        std::cin.ignore();
		return -1;

    } catch ( const std::exception& ex ) {
        std::cerr << std::endl << ex.what() << std::endl;
        std::cin.ignore();
        return -2;

    } catch ( ... ) {
        std::cerr << std::endl << "(!) Unknown exception." << std::endl;
        std::cin.ignore();
        return -100;
    }


}; // main()








inline void run() {

    using namespace matchn;

    std::shared_ptr< World >  world =
        World::valueOf( 3, 5, 5 );
    for (auto command = World::command_t();
         command.first != World::QUIT;
         command = world->go()
    ) {
        // отрабатываем некоторые команды мира
        const auto lw = world->logic();
        size_t K = lw->K;
        size_t S = lw->N;
        switch ( command.first ) {
            case World::DECREASE_CHAIN :
                K = lw->K - 1;
                K = (K < 2) ? 2 : K;
                break;

            case World::DECREASE_TERRITORY :
                S = lw->N - 2;
                S = (S < 3) ? 3 : S;
                break;

            case World::INCREASE_CHAIN :
                K = lw->K + 1;
                K = (K > 10) ? 10 : K;
                break;

            case World::INCREASE_TERRITORY :
                S = lw->N + 2;
                S = (S > 15) ? 15 : S;
                break;
        }

        world = World::valueOf( K, S, S );

        if (command.first == World::QUIT) {
            // завершаем работу с миром
            break;
        }

    } // for (auto command = ...

}
