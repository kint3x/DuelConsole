#include <app/games/SlideaLama/SlideLamaGrid.hpp>

SlideLamaGrid::SlideLamaGrid(DuelConsole *console) : console(console) {
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

void SlideLamaGrid::fillWithBlocks(std::vector<SlideLamaBlockType> blocks){
    for(int i = 0; i < SLIDELAMA_GRIDSIZE * SLIDELAMA_GRIDSIZE; i++) {
        cells[i] = blocks[i];
    }
}

SlideLamaBlockType SlideLamaGrid::pickRandomBlock()
{
    uint32_t total = 0;
    for (int t = 1; t < static_cast<int>(SlideLamaBlockType::COUNT); ++t) {
        total += blockSpanFreq[t];
    }

    uint32_t r = console->getRand() % total;

    for (int t = 1; t < static_cast<int>(SlideLamaBlockType::COUNT); ++t) {
        if (r < blockSpanFreq[t]) {
            return static_cast<SlideLamaBlockType>(t);
        }
        r -= blockSpanFreq[t];
    }

    return SlideLamaBlockType::EMPTY; // safety
}


void SlideLamaGrid::generateCells()
{
    const int size = SLIDELAMA_GRIDSIZE;

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {

            SlideLamaBlockType chosen = SlideLamaBlockType::EMPTY;

            // Try a few times (will succeed fast)
            for (int attempt = 0; attempt < 10; ++attempt) {
                SlideLamaBlockType type = pickRandomBlock();
                bool valid = true;

                // Horizontal check
                if (x >= 2) {
                    auto a = cells[y * size + (x - 1)];
                    auto b = cells[y * size + (x - 2)];
                    if (a == type && b == type) {
                        valid = false;
                    }
                }

                // Vertical check
                if (y >= 2) {
                    auto a = cells[(y - 1) * size + x];
                    auto b = cells[(y - 2) * size + x];
                    if (a == type && b == type) {
                        valid = false;
                    }
                }

                if (valid) {
                    chosen = type;
                    break;
                }
            }

            // Safety fallback (very unlikely)
            if (chosen == SlideLamaBlockType::EMPTY) {
                chosen = pickRandomBlock();
            }

            cells[y * size + x] = chosen;
        }
    }
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

position_t SlideLamaGrid::gridIdxToFramePos(uint16_t x,uint16_t y){
    return {static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.x + (x)*35), static_cast<uint16_t>(LEFT_TOP_GRID_START_POS.y + y*40)};
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
    
    // Check if the rightmost cell (where new block will go) is occupied
    int rightmostIdx = rowStartIndex + SLIDELAMA_GRIDSIZE - 1;
    
    if(cells[rightmostIdx] != SlideLamaBlockType::EMPTY) {
        // The block that will be shifted is the rightmost one
        temp1 = cells[rightmostIdx];
        
        // Now shift remaining blocks
        for(int x = SLIDELAMA_GRIDSIZE - 2; x >= 0; x--) {
            int idx = rowStartIndex + x;
            
            temp2 = cells[idx];
            
            cells[idx] = temp1;
            temp1 = temp2;
            
            if(temp2 == SlideLamaBlockType::EMPTY)
                break;
        }
    }

    // Insert new block at rightmost position
    cells[rightmostIdx] = block;

}

void SlideLamaGrid::slideFromLeft(SlideLamaBlockType block, int row)
{
    uint16_t rowStartIndex = row * SLIDELAMA_GRIDSIZE;
    SlideLamaBlockType temp1, temp2;
    
    // Check if the leftmost cell (where new block will go) is occupied
    int leftmostIdx = rowStartIndex;
    
    if(cells[leftmostIdx] != SlideLamaBlockType::EMPTY) {
        // The block that will be shifted is the leftmost one
        temp1 = cells[leftmostIdx];
        
        // Now shift remaining blocks to the right
        for(int x = 1; x < SLIDELAMA_GRIDSIZE; x++) {
            int idx = rowStartIndex + x;
            
            temp2 = cells[idx];
            
            cells[idx] = temp1;
            temp1 = temp2;
            
            if(temp2 == SlideLamaBlockType::EMPTY)
                break;
        }
    }

    // Insert new block at leftmost position
    cells[leftmostIdx] = block;

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