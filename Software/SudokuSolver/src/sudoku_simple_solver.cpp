#include "sudoku_simple_solver.h"

#include <cstdlib>
#include <iostream>

using namespace std;


sudoku::simple_solver::simple_solver(std::vector<std::string> grid) : _grid(grid),
        _availables(9, vector< bitset<10> >(9, bitset<10>(0x3fe)))
{
    // Create binary masks of all possible values in each grid location
    // NB Bit 0 ignored for easier indexing of values 1 to 9
    for (int i=0; i<9; i++)
        for (int j=0; j<9; j++)
            if (_grid[i][j] != '0')
                _availables[i][j] = bitset<10>(1 << (_grid[i][j]-'0'));
}

bool sudoku::simple_solver::try_to_finalise(int i, int j)
{
    // Try to find exactly one available value
    bool found = false;
    char v = '0';
    for (int k=1; k<=9; k++)
        if (_availables[i][j][k])
        {
            if (found)
                return false;
            else
            {
                v = '0' + k;
                found = true;
            }
        }
    
    // Update if exactly one available value found
    if (v != '0')
    {
        _grid[i][j] = v;
        return true;
    }
    else
        return false;
}

sudoku::grid::state sudoku::simple_solver::attempt_to_solve()
{   
    // Keep trying until "availables" stops changing
    vector< vector< bitset<10> > > availables_prev;
    int iterations = 0;
    do
    {
        availables_prev = _availables;
        
        for (int i=0; i<9; i++)
            for (int j=0; j<9; j++)
            {
                // Continue if this location already finalised
                if (_grid[i][j] != '0')
                    continue;                
                
                // Search variable q (across row / down column / in block)
                for (int q=0; q<9; q++)
                {
                    // Check across own row
                    if (q != j)
                    {
                        _availables[i][j] &= ~(1 << (_grid[i][q] - '0'));
                    
                        if (try_to_finalise(i, j))
                            break;
                    }
                    
                    // Check down own column
                    if (q != i)
                    {
                        _availables[i][j] &= ~(1 << (*_grid.columns[j][q] - '0'));
                        
                        if (try_to_finalise(i, j))
                            break;
                    }
                    
                    // Check within own block
                    int blk = (j/3) + 3*(i/3);
                    int idx = (j%3) + 3*(i%3);
                    if (q != idx)
                    {
                        _availables[i][j] &= ~(1 << (*_grid.blocks[blk][q] - '0'));
                    
                        if (try_to_finalise(i, j))
                            break;  
                    }
                }
            }
        
        iterations++;
    } while (availables_prev != _availables && iterations<1000);
    
    // Check nothing silly happened
    if (iterations >= 1000)
    {
        cout << __FUNCTION__ << " : Error! Iteration limit reached." << endl;
        exit(EXIT_FAILURE);
    }
    
    // ##### DEBUGGING #####
    if (_grid.calc_grid_state() == sudoku::grid::solved_state)
        _grid.print();
    
    return _grid.calc_grid_state();
}

void sudoku::simple_solver::apply_guess(sudoku::guess_struct guess)
{
    // Record the guess
    _guess = guess;
    
    // Apply guess
    _grid[guess.loc.first][guess.loc.second] = guess.val + '0';
    _availables[guess.loc.first][guess.loc.second] = bitset<10>(1 << guess.val);
}

void sudoku::simple_solver::forbid_guess(sudoku::guess_struct guess)
{
    _availables[guess.loc.first][guess.loc.second] &= ~(1 << guess.val);
    try_to_finalise(guess.loc.first, guess.loc.second);
}
