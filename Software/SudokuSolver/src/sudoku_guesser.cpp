#include "sudoku_guesser.h"
#include "sudoku_simple_solver.h"

#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;

sudoku::guesser::guesser(std::vector<std::string> grid)
{
    // Shove the initial grid into the grid list
    _stages.push_back(sudoku::simple_solver(sudoku::grid(grid)));
}

void sudoku::guesser::solve()
{    
    int iterations = 0;
    while (true)
    {        
        // Attempt to solve
        sudoku::grid::state state = _stages.back().attempt_to_solve();
        iterations++;
        
        if (state == sudoku::grid::solved_state)
        {
            // SOLVED: Exit.
            break;
        }
        else if (state == sudoku::grid::valid_state)
        {
            // VALID: Make a guess and push new grid onto list
            sudoku::grid grid = _stages.back().get_grid();
            vector< vector< bitset<10> > > availables = _stages.back().get_availables();
            
            // Make a guess
            sudoku::guess_struct guess = make_a_guess(grid, availables);
            
            // Duplicate grid onto end of list and apply guess to it
            _stages.push_back(_stages.back());
            _stages.back().apply_guess(guess);
        }
        else if (state == sudoku::grid::invalid_state)
        {            
            if (_stages.size() <= 1)
            {
                cout << "Error: No valid grid to fall back to" << endl;
                _stages.back().get_grid().print();
                exit(EXIT_FAILURE);
            }
            
            // INVALID: Fall back to previous level and forbid invalid guess
            _stages[_stages.size()-2].forbid_guess(_stages.back().get_guess());
            _stages.pop_back();
        }
    }
}

sudoku::guess_struct sudoku::guesser::make_a_guess(const sudoku::grid& grid, const vector< vector< bitset<10> > >& availables) const
{
    sudoku::guess_struct guess;
    guess.loc = get_sensible_guess_loc(grid, availables);
    guess.val = get_guess_val(availables, guess.loc);
    
    return guess;
}

pair<int,int> sudoku::guesser::get_sensible_guess_loc(const sudoku::grid& grid, const vector< vector< bitset<10> > >& availables) const
{
    int min = 9;
    int imin = -1;
    int jmin = -1;

    // Heuristic: Fewest remaining possible values
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
        {
            if (grid[i][j] != '0')
                continue;
            
            int sum = 0;
            for (int k=1; k<10; k++)
                sum += availables[i][j][k];
            
            if (sum < min)
            {
                imin = i;
                jmin = j;
                min = sum;
            }
        }

    if (imin != -1)
    {
        return pair<int,int>(imin,jmin);
    }
    
    cout << __FUNCTION__ << " : No availables found." << endl;
    exit(EXIT_FAILURE);
}

int sudoku::guesser::get_guess_val(const vector< vector< bitset<10> > >& availables, pair<int,int> loc) const
{
    // Naive approach : return lowest available value
    for (int i=1; i<10; i++)
        if (availables[loc.first][loc.second][i])
            return i;
    
    cout << __FUNCTION__ << " : No availables found." << endl;
    exit(EXIT_FAILURE);
}
