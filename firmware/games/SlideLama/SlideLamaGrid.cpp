#include <games/SlideLama/SlideLamaGrid.hpp>
#include <ICQEngine/include/ICQEngine.hpp>
#include <generated/resources.hpp>

SlideLamaGrid::SlideLamaGrid(){
    // Initialize the grid
    for(int i = 0; i < SLIDELAMA_GRIDSIZE * SLIDELAMA_GRIDSIZE; i++) {
        cells[i] = SlideLamaBlockType::EMPTY;
    }
}

SlideLamaBlockType* SlideLamaGrid::at(position_t pos) {
    if(pos.x >= SLIDELAMA_GRIDSIZE || pos.y >= SLIDELAMA_GRIDSIZE) {
        return nullptr; // Out of bounds
    }
    return &cells[pos.y * SLIDELAMA_GRIDSIZE + pos.x];
}

void SlideLamaGrid::putExact(position_t pos, SlideLamaBlockType type) {
    if(pos.x >= SLIDELAMA_GRIDSIZE || pos.y >= SLIDELAMA_GRIDSIZE) {
        return; // Out of bounds
    }
    cells[pos.y * SLIDELAMA_GRIDSIZE + pos.x] = type;
}

void SlideLamaGrid::resolveGravity()
{
    for (int x = 0; x < SLIDELAMA_GRIDSIZE; ++x)
    {
        int writeY = SLIDELAMA_GRIDSIZE - 1; // bottom

        // move all non-empty blocks down
        for (int y = SLIDELAMA_GRIDSIZE - 1; y >= 0; --y)
        {
            int readIdx = y * SLIDELAMA_GRIDSIZE + x;

            if (cells[readIdx] != SlideLamaBlockType::EMPTY)
            {
                int writeIdx = writeY * SLIDELAMA_GRIDSIZE + x;

                if (writeIdx != readIdx)
                {
                    cells[writeIdx] = cells[readIdx];
                    cells[readIdx] = SlideLamaBlockType::EMPTY;
                }

                writeY--;
            }
        }

        // fill remaining top cells with EMPTY
        for (int y = writeY; y >= 0; --y)
        {
            cells[y * SLIDELAMA_GRIDSIZE + x] = SlideLamaBlockType::EMPTY;
        }
    }
}

void SlideLamaGrid::slideFromTop(SlideLamaBlockType block, int column)
{
    // Implement sliding from top logic here
    uint16_t colStartIndex = column;

    SlideLamaBlockType temp1, temp2;

    // If top is not empty → shift everything down
    if (cells[colStartIndex] != SlideLamaBlockType::EMPTY)
    {
        temp1 = cells[colStartIndex];

        for (int y = 1; y < SLIDELAMA_GRIDSIZE; ++y)
        {
            int idx = y * SLIDELAMA_GRIDSIZE + column;

            temp2 = cells[idx];
            cells[idx] = temp1;

            if (temp2 == SlideLamaBlockType::EMPTY)
                break;

            temp1 = temp2;
        }
    }

    // insert new block at top
    cells[colStartIndex] = block;

}

void SlideLamaGrid::slideFromRight(SlideLamaBlockType block, int row)
{

    uint16_t rowStartIndex = row * SLIDELAMA_GRIDSIZE;
    SlideLamaBlockType temp1, temp2;
    if(cells[rowStartIndex+SLIDELAMA_GRIDSIZE-1] != SlideLamaBlockType::EMPTY) {
        //We shift all blocks to the left
        temp1 = cells[rowStartIndex+SLIDELAMA_GRIDSIZE-1];
        for(int x = SLIDELAMA_GRIDSIZE-2; x >= 0; x--) {
            // Shift blocks to the left
            temp2 = cells[rowStartIndex + x];
            cells[rowStartIndex + x] = temp1;
            if(temp2 == SlideLamaBlockType::EMPTY) {
                break;
            }
            temp1 = temp2;
            
        }
    }

    cells[rowStartIndex+SLIDELAMA_GRIDSIZE-1] = block; // Insert new block at the leftmost position

}

void SlideLamaGrid::slideFromLeft(SlideLamaBlockType block, int row)
{

    // Left is not empty, we need to shift everything to the right
    uint16_t rowStartIndex = row * SLIDELAMA_GRIDSIZE;
    SlideLamaBlockType temp1, temp2;
    if(cells[rowStartIndex] != SlideLamaBlockType::EMPTY) {
        //We shift all blocks to the right
        temp1 = cells[rowStartIndex];
        for(int x = 1; x < SLIDELAMA_GRIDSIZE; x++) {
            // Shift blocks to the right
            temp2 = cells[rowStartIndex + x];
            cells[rowStartIndex + x] = temp1;
            if(temp2 == SlideLamaBlockType::EMPTY) {
                break;
            }
            temp1 = temp2;
            
        }
    }

    cells[rowStartIndex] = block; // Insert new block at the leftmost position

}

/*
* Go through from left top corner [0][0] to right bottom corner [4][4] and find the first set of 3 or more blocks that 
can be resolved. First check horizontall, then vertical. If found, fill the resolvedSet struct with the position of the 
first block in the set, the count of blocks in the set, the type of blocks and the direction (horizontal or vertical). 
If no set is found, return false.
*/

bool SlideLamaGrid::findSetToResolve(resolvedSet &set)
{
    const int SIZE = 5;

    for (int y = 0; y < SIZE; ++y)
    {
        for (int x = 0; x < SIZE; ++x)
        {
            SlideLamaBlockType type = cells[x + y * SLIDELAMA_GRIDSIZE];

            if (type == SlideLamaBlockType::EMPTY)
                continue;

            // --- check horizontal ---
            if (x <= SIZE - 3)
            {
                int count = 1;

                for (int i = x + 1; i < SIZE; ++i)
                {
                    if (cells[i + y * SLIDELAMA_GRIDSIZE] == type)
                        count++;
                    else
                        break;
                }

                if (count >= 3)
                {
                    set.tilePos.x = x;
                    set.tilePos.y = y;
                    set.count = count;
                    set.type = type;
                    set.direction = ResolveDirection::HORIZONTAL;
                    return true;
                }
            }

            // --- check vertical ---
            if (y <= SIZE - 3)
            {
                int count = 1;

                for (int i = y + 1; i < SIZE; ++i)
                {
                    if (cells[x + i * SLIDELAMA_GRIDSIZE] == type)
                        count++;
                    else
                        break;
                }

                if (count >= 3)
                {
                    set.tilePos.x = x;
                    set.tilePos.y = y;
                    set.count = count;
                    set.type = type;
                    set.direction = ResolveDirection::VERTICAL;
                    return true;
                }
            }
        }
    }

    return false;
}

void SlideLamaGrid::resolveSet(const resolvedSet &set)
{
    for (int i = 0; i < set.count; ++i)
    {
        if (set.direction == ResolveDirection::HORIZONTAL)
        {
            cells[(set.tilePos.x + i) + set.tilePos.y * SLIDELAMA_GRIDSIZE] = SlideLamaBlockType::EMPTY;
        }
        else // VERTICAL
        {
            cells[set.tilePos.x + (set.tilePos.y + i) * SLIDELAMA_GRIDSIZE] = SlideLamaBlockType::EMPTY;
        }
    }
}