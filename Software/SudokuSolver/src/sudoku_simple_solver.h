#ifndef SUDOKU_SIMPLE_SOLVER_H
#define	SUDOKU_SIMPLE_SOLVER_H

#include <vector>
#include <string>
#include <bitset>

#include "sudoku_grid.h"

namespace sudoku
{   
    struct guess_struct
    {
        std::pair<int,int> loc;
        int val;
    };
    
    class simple_solver
    {
    public:

        simple_solver(std::vector<std::string> grid);
        
        sudoku::grid::state attempt_to_solve();
        void apply_guess(sudoku::guess_struct guess);
        void forbid_guess(sudoku::guess_struct guess);
        
        sudoku::guess_struct get_guess() const { return _guess; }
        
        // Getters     
        const sudoku::grid& get_grid() const { return _grid; }
        const std::vector< std::vector< std::bitset<10> > >& get_availables() const { return _availables; }
        
    private:

        bool try_to_finalise(int i, int j);

        sudoku::grid _grid;
        std::vector< std::vector< std::bitset<10> > > _availables;
        sudoku::guess_struct _guess;
    };

}

#endif	/* SUDOKU_SIMPLE_SOLVER_H */
