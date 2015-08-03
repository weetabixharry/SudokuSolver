#ifndef SUDOKU_GUESSER_H
#define	SUDOKU_GUESSER_H

#include "sudoku_simple_solver.h"

#include <bitset>

namespace sudoku
{
    
    class guesser
    {
    public:

        guesser(std::vector<std::string> grid);
        void solve();
        
    private:
        
        guess_struct make_a_guess(const sudoku::grid& grid,
                const std::vector< std::vector< std::bitset<10> > >& availables) const;
        
        std::pair<int,int> get_sensible_guess_loc(const sudoku::grid& grid,
                const std::vector< std::vector< std::bitset<10> > >& availables) const;
        
        int get_guess_val(const std::vector< std::vector< std::bitset<10> > >& availables,
                std::pair<int,int> loc) const;

        std::vector<sudoku::simple_solver> _stages;
        //std::vector<sudoku::guess_struct> _guesses;
    };

}

#endif	/* SUDOKU_GUESSER_H */
