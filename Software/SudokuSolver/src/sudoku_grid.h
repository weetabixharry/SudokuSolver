#ifndef SUDOKU_GRID_H
#define	SUDOKU_GRID_H

#include <vector>
#include <string>

namespace sudoku
{    
    class grid : public std::vector<std::string>
    {
    public:
        
        grid(std::vector<std::string> grid);
        grid(const grid& grid); 
        
        enum state {solved_state = 0, valid_state = 1, invalid_state = 2};
        
        sudoku::grid::state calc_grid_state() const;
        
        void print() const;
        void print_columns() const { print_ptrs(columns); }
        void print_blocks() const { print_ptrs(blocks); }
        
        std::vector<std::vector<char*> > columns;
        std::vector<std::vector<char*> > blocks;
        
    private:
        
        void init_ptrs();
        std::vector< std::pair<int,int> > get_block_coords(int blk) const;
        void print_ptrs(std::vector< std::vector<char*> > grid) const;
    };    
}

#endif	/* SUDOKU_GRID_H */
